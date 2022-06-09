/*
 * This file is part of the flashrom project.
 *
 * Copyright (C) 2012 The Chromium OS Authors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of Google or the names of contributors or
 * licensors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty of any kind.
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED.
 * GOOGLE INC AND ITS LICENSORS SHALL NOT BE LIABLE
 * FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.  IN NO EVENT WILL
 * GOOGLE OR ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA,
 * OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR
 * PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF
 * LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE,
 * EVEN IF GOOGLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 */

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "flashchips.h"
#include "flash.h"
#include "fmap.h"
#include "cros_ec.h"
#include "cros_ec_commands.h"
#include "programmer.h"
#include "spi.h"
#include "dep_writeprotect.h"

static int ignore_wp_range_command = 0;

static int cros_ec_list_ranges(const struct flashctx *flash)
{
	struct ec_response_flash_region_info info;
	int rc;

	rc = cros_ec_get_region_info(EC_FLASH_REGION_WP_RO, &info);
	if (rc < 0) {
		msg_perr("Cannot get the WP_RO region info: %d\n", rc);
		return 1;
	}

	msg_pinfo("Supported write protect range:\n");
	msg_pinfo("  disable: start=0x%06x len=0x%06x\n", 0, 0);
	msg_pinfo("  enable:  start=0x%06x len=0x%06x\n", info.offset,
		  info.size);

	return 0;
}


/*
 * Helper function for flash protection.
 *
 *  On EC API v1, the EC write protection has been simplified to one-bit:
 *  EC_FLASH_PROTECT_RO_AT_BOOT, which means the state is either enabled
 *  or disabled. However, this is different from the SPI-style write protect
 *  behavior. Thus, we re-define the flashrom command (SPI-style) so that
 *  either SRP or range is non-zero, the EC_FLASH_PROTECT_RO_AT_BOOT is set.
 *
 *    SRP     Range      | PROTECT_RO_AT_BOOT
 *     0        0        |         0
 *     0     non-zero    |         1
 *     1        0        |         1
 *     1     non-zero    |         1
 *
 *
 *  Besides, to make the protection take effect as soon as possible, we
 *  try to set EC_FLASH_PROTECT_RO_NOW at the same time. However, not
 *  every EC supports RO_NOW, thus we then try to protect the entire chip.
 */
int set_wp(int enable)
{
	struct ec_params_flash_protect p;
	struct ec_response_flash_protect r;
	const int ro_at_boot_flag = EC_FLASH_PROTECT_RO_AT_BOOT;
	const int ro_now_flag = EC_FLASH_PROTECT_RO_NOW;
	int need_an_ec_cold_reset = 0;
	int rc;

	/* Try to set RO_AT_BOOT and RO_NOW first */
	memset(&p, 0, sizeof(p));
	p.mask = (ro_at_boot_flag | ro_now_flag);
	p.flags = enable ? (ro_at_boot_flag | ro_now_flag) : 0;
	rc = cros_ec_priv->ec_command(EC_CMD_FLASH_PROTECT,
			EC_VER_FLASH_PROTECT, &p, sizeof(p), &r, sizeof(r));
	if (rc < 0) {
		msg_perr("FAILED: Cannot set the RO_AT_BOOT and RO_NOW: %d\n",
			 rc);
		return 1;
	}

	/* Read back */
	memset(&p, 0, sizeof(p));
	rc = cros_ec_priv->ec_command(EC_CMD_FLASH_PROTECT,
			EC_VER_FLASH_PROTECT, &p, sizeof(p), &r, sizeof(r));
	if (rc < 0) {
		msg_perr("FAILED: Cannot get RO_AT_BOOT and RO_NOW: %d\n",
			 rc);
		return 1;
	}

	if (!enable) {
		/* The disable case is easier to check. */
		if (r.flags & ro_at_boot_flag) {
			msg_perr("FAILED: RO_AT_BOOT is not clear.\n");
			return 1;
		} else if (r.flags & ro_now_flag) {
			msg_perr("FAILED: RO_NOW is asserted unexpectedly.\n");
			need_an_ec_cold_reset = 1;
			goto exit;
		}

		msg_pdbg("INFO: RO_AT_BOOT is clear.\n");
		return 0;
	}

	/* Check if RO_AT_BOOT is set. If not, fail in anyway. */
	if (r.flags & ro_at_boot_flag) {
		msg_pdbg("INFO: RO_AT_BOOT has been set.\n");
	} else {
		msg_perr("FAILED: RO_AT_BOOT is not set.\n");
		return 1;
	}

	/* Then, we check if the protection has been activated. */
	if (r.flags & ro_now_flag) {
		/* Good, RO_NOW is set. */
		msg_pdbg("INFO: RO_NOW is set. WP is active now.\n");
	} else if (r.writable_flags & EC_FLASH_PROTECT_ALL_NOW) {
		msg_pdbg("WARN: RO_NOW is not set. Trying ALL_NOW.\n");

		memset(&p, 0, sizeof(p));
		p.mask = EC_FLASH_PROTECT_ALL_NOW;
		p.flags = EC_FLASH_PROTECT_ALL_NOW;
		rc = cros_ec_priv->ec_command(EC_CMD_FLASH_PROTECT,
				      EC_VER_FLASH_PROTECT,
				      &p, sizeof(p), &r, sizeof(r));
		if (rc < 0) {
			msg_perr("FAILED: Cannot set ALL_NOW: %d\n", rc);
			return 1;
		}

		/* Read back */
		memset(&p, 0, sizeof(p));
		rc = cros_ec_priv->ec_command(EC_CMD_FLASH_PROTECT,
				      EC_VER_FLASH_PROTECT,
				      &p, sizeof(p), &r, sizeof(r));
		if (rc < 0) {
			msg_perr("FAILED:Cannot get ALL_NOW: %d\n", rc);
			return 1;
		}

		if (!(r.flags & EC_FLASH_PROTECT_ALL_NOW)) {
			msg_perr("FAILED: ALL_NOW is not set.\n");
			need_an_ec_cold_reset = 1;
			goto exit;
		}

		msg_pdbg("INFO: ALL_NOW has been set. WP is active now.\n");

		/*
		 * Our goal is to protect the RO ASAP. The entire protection
		 * is just a workaround for platform not supporting RO_NOW.
		 * It has side-effect that the RW is also protected and leads
		 * the RW update failed. So, we arrange an EC code reset to
		 * unlock RW ASAP.
		 */
		rc = cros_ec_cold_reboot(EC_REBOOT_FLAG_ON_AP_SHUTDOWN);
		if (rc < 0) {
			msg_perr("WARN: Cannot arrange a cold reset at next "
				 "shutdown to unlock entire protect.\n");
			msg_perr("      But you can do it manually.\n");
		} else {
			msg_pdbg("INFO: A cold reset is arranged at next "
				 "shutdown.\n");
		}

	} else {
		msg_perr("FAILED: RO_NOW is not set.\n");
		msg_perr("FAILED: The PROTECT_RO_AT_BOOT is set, but cannot "
			 "make write protection active now.\n");
		need_an_ec_cold_reset = 1;
	}

exit:
	if (need_an_ec_cold_reset) {
		msg_perr("FAILED: You may need a reboot to take effect of "
			 "PROTECT_RO_AT_BOOT.\n");
		return 1;
	}

	return 0;
}

static int cros_ec_set_range(const struct flashctx *flash, unsigned int start, unsigned int len)
{
	struct ec_response_flash_region_info info;
	int rc;

	/* Check if the given range is supported */
	rc = cros_ec_get_region_info(EC_FLASH_REGION_WP_RO, &info);
	if (rc < 0) {
		msg_perr("FAILED: Cannot get the WP_RO region info: %d\n", rc);
		return 1;
	}
	if ((!start && !len) ||  /* list supported ranges */
	    ((start == info.offset) && (len == info.size))) {
		/* pass */
	} else {
		msg_perr("FAILED: Unsupported write protection range "
			 "(0x%06x,0x%06x)\n\n", start, len);
		msg_perr("Currently supported range:\n");
		msg_perr("  disable: (0x%06x,0x%06x)\n", 0, 0);
		msg_perr("  enable:  (0x%06x,0x%06x)\n", info.offset,
			 info.size);
		return 1;
	}

	if (ignore_wp_range_command)
		return 0;
	return set_wp(!!len);
}


static int cros_ec_enable_writeprotect(const struct flashctx *flash, enum wp_mode wp_mode)
{
	int ret;

	switch (wp_mode) {
	case WP_MODE_HARDWARE:
		ret = set_wp(1);
		break;
	default:
		msg_perr("%s():%d Unsupported write-protection mode\n",
				__func__, __LINE__);
		ret = 1;
		break;
	}

	return ret;
}


static int cros_ec_disable_writeprotect(const struct flashctx *flash)
{
	/* --wp-range implicitly enables write protection on CrOS EC, so force
	   it not to if --wp-disable is what the user really wants. */
	ignore_wp_range_command = 1;
	return set_wp(0);
}


static int cros_ec_wp_status(const struct flashctx *flash,
		uint32_t *_start, uint32_t *_len, bool *_wp_en)
{
	struct ec_params_flash_protect p;
	struct ec_response_flash_protect r;
	int start, len;  /* wp range */
	int enabled;
	int rc;

	memset(&p, 0, sizeof(p));
	rc = cros_ec_priv->ec_command(EC_CMD_FLASH_PROTECT,
			EC_VER_FLASH_PROTECT, &p, sizeof(p), &r, sizeof(r));
	if (rc < 0) {
		msg_perr("FAILED: Cannot get the write protection status: %d\n",
			 rc);
		return 1;
	} else if (rc < (int)sizeof(r)) {
		msg_perr("FAILED: Too little data returned (expected:%zd, "
			 "actual:%d)\n", sizeof(r), rc);
		return 1;
	}

	start = len = 0;
	if (r.flags & EC_FLASH_PROTECT_RO_AT_BOOT) {
		struct ec_response_flash_region_info info;

		msg_pdbg("%s(): EC_FLASH_PROTECT_RO_AT_BOOT is set.\n",
			 __func__);
		rc = cros_ec_get_region_info(EC_FLASH_REGION_WP_RO, &info);
		if (rc < 0) {
			msg_perr("FAILED: Cannot get the WP_RO region info: "
				 "%d\n", rc);
			return 1;
		}
		start = info.offset;
		len = info.size;
	} else {
		msg_pdbg("%s(): EC_FLASH_PROTECT_RO_AT_BOOT is clear.\n",
			 __func__);
	}

	/*
	 * If neither RO_NOW or ALL_NOW is set, it means write protect is
	 * NOT active now.
	 */
	if (!(r.flags & (EC_FLASH_PROTECT_RO_NOW | EC_FLASH_PROTECT_ALL_NOW)))
		start = len = 0;

	/* Remove the SPI-style messages. */
	enabled = r.flags & EC_FLASH_PROTECT_RO_AT_BOOT ? 1 : 0;
	msg_pinfo("WP: status: 0x%02x\n", enabled ? 0x80 : 0x00);
	msg_pinfo("WP: status.srp0: %x\n", enabled);
	msg_pinfo("WP: write protect is %s.\n",
			enabled ? "enabled" : "disabled");
	msg_pinfo("WP: write protect range: start=0x%08x, len=0x%08x\n",
	          start, len);

	return 0;
}

struct wp cros_ec_wp = {
	.list_ranges    = cros_ec_list_ranges,
	.set_range      = cros_ec_set_range,
	.enable         = cros_ec_enable_writeprotect,
	.disable        = cros_ec_disable_writeprotect,
	.wp_status      = cros_ec_wp_status,
};

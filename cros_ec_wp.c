/*
 * This file is part of the flashrom project.
 *
 * Copyright (C) 2012 Google Inc.
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

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cros_ec_commands.h"
#include "cros_ec.h"
#include "flash.h"
#include "programmer.h"

enum flashrom_wp_result cros_ec_wp_read_cfg(struct flashrom_wp_cfg *cfg, struct flashctx *flash)
{
	struct ec_params_flash_protect p;
	struct ec_response_flash_protect r;

	memset(&p, 0, sizeof(p));
	int rc = cros_ec_priv->ec_command(EC_CMD_FLASH_PROTECT,
			EC_VER_FLASH_PROTECT, &p, sizeof(p), &r, sizeof(r));

	if (rc < (int)sizeof(r)) {
		msg_perr("FAILED: Too little data returned (expected:%zd, "
			 "actual:%d)\n", sizeof(r), rc);
		return FLASHROM_WP_ERR_READ_FAILED;
	}

	if (r.flags & EC_FLASH_PROTECT_RO_AT_BOOT) {
		struct ec_response_flash_region_info info;

		rc = cros_ec_get_region_info(EC_FLASH_REGION_WP_RO, &info);
		if (rc < 0) {
			msg_perr("FAILED: Cannot get the WP_RO region info: "
				 "%d\n", rc);
			return FLASHROM_WP_ERR_READ_FAILED;
		}

		cfg->range.start = info.offset;
		cfg->range.len = info.size;
		cfg->mode = FLASHROM_WP_MODE_HARDWARE;
	} else {
		cfg->range.start = 0;
		cfg->range.len = 0;
		cfg->mode = FLASHROM_WP_MODE_DISABLED;
	}

	/*
	 * If neither RO_NOW or ALL_NOW is set, it means write protect is
	 * NOT active now.
	 */
	if (!(r.flags & (EC_FLASH_PROTECT_RO_NOW | EC_FLASH_PROTECT_ALL_NOW))) {
		cfg->range.start = 0;
		cfg->range.len = 0;
	}

	return FLASHROM_WP_OK;
}

enum flashrom_wp_result cros_ec_wp_write_cfg(struct flashctx *flash, const struct flashrom_wp_cfg *cfg)
{
	/* Read the size of the EC's protection region */
	struct ec_response_flash_region_info info;
	if (cros_ec_get_region_info(EC_FLASH_REGION_WP_RO, &info) < 0)
		return FLASHROM_WP_ERR_OTHER;

	bool enable = cfg->mode == FLASHROM_WP_MODE_HARDWARE;

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
		return FLASHROM_WP_ERR_WRITE_FAILED;
	}

	/* Read back */
	memset(&p, 0, sizeof(p));
	rc = cros_ec_priv->ec_command(EC_CMD_FLASH_PROTECT,
			EC_VER_FLASH_PROTECT, &p, sizeof(p), &r, sizeof(r));
	if (rc < 0) {
		msg_perr("FAILED: Cannot get RO_AT_BOOT and RO_NOW: %d\n",
			 rc);
		return FLASHROM_WP_ERR_WRITE_FAILED;
	}

	if (!enable) {
		/* The disable case is easier to check. */
		if (r.flags & ro_at_boot_flag) {
			msg_perr("FAILED: RO_AT_BOOT is not clear.\n");
			return FLASHROM_WP_ERR_WRITE_FAILED;
		} else if (r.flags & ro_now_flag) {
			msg_perr("FAILED: RO_NOW is asserted unexpectedly.\n");
			need_an_ec_cold_reset = 1;
			goto exit;
		}

		msg_pdbg("INFO: RO_AT_BOOT is clear.\n");
		return FLASHROM_WP_OK;
	}

	/* Check if RO_AT_BOOT is set. If not, fail in anyway. */
	if (r.flags & ro_at_boot_flag) {
		msg_pdbg("INFO: RO_AT_BOOT has been set.\n");
	} else {
		msg_perr("FAILED: RO_AT_BOOT is not set.\n");
		return FLASHROM_WP_ERR_WRITE_FAILED;
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
			return FLASHROM_WP_ERR_WRITE_FAILED;
		}

		/* Read back */
		memset(&p, 0, sizeof(p));
		rc = cros_ec_priv->ec_command(EC_CMD_FLASH_PROTECT,
				      EC_VER_FLASH_PROTECT,
				      &p, sizeof(p), &r, sizeof(r));
		if (rc < 0) {
			msg_perr("FAILED:Cannot get ALL_NOW: %d\n", rc);
			return FLASHROM_WP_ERR_WRITE_FAILED;
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
		return FLASHROM_WP_ERR_WRITE_FAILED;
	}

	return FLASHROM_WP_OK;
}

enum flashrom_wp_result cros_ec_wp_get_available_ranges(struct flashrom_wp_ranges **list, struct flashctx *flash)
{
	/* Allocate output buffer */
	*list = calloc(1, sizeof(struct flashrom_wp_ranges));
	if (!*list)
		return FLASHROM_WP_ERR_OTHER;

	(*list)->ranges = calloc(2, sizeof(struct wp_range));
	if (!(*list)->ranges) {
		free(*list);
		return FLASHROM_WP_ERR_OTHER;
	}

	/* Read the size of the EC's only protection region */
	struct ec_response_flash_region_info info;
	if (cros_ec_get_region_info(EC_FLASH_REGION_WP_RO, &info) < 0)
		return FLASHROM_WP_ERR_OTHER;

	/* WP disabled */
	(*list)->ranges[0].start = 0;
	(*list)->ranges[0].len = 0;

	/* WP enabled */
	(*list)->ranges[1].start = info.offset;
	(*list)->ranges[1].len = info.size;

	(*list)->count = 2;

	return FLASHROM_WP_OK;
}

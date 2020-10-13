/*
 * This file is part of the flashrom project.
 *
 * Copyright (C) 2014 Boris Baykov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * SPI chip driver functions for 4-bytes addressing
 */

#include <string.h>
#include "flash.h"
#include "chipdrivers.h"
#include "spi.h"
#include "programmer.h"
#include "spi4ba.h"

/* #define MSG_TRACE_4BA_FUNCS 1 */

#ifdef MSG_TRACE_4BA_FUNCS
#define msg_trace(...) print(FLASHROM_MSG_DEBUG, __VA_ARGS__)
#else
#define msg_trace(...)
#endif

/* Enter 4-bytes addressing mode (without sending WREN before) */
int spi_enter_4ba_b7(struct flashctx *flash)
{
	int result;
	const unsigned char cmd[JEDEC_ENTER_4_BYTE_ADDR_MODE_OUTSIZE] = { JEDEC_ENTER_4_BYTE_ADDR_MODE };

	msg_trace("-> %s\n", __func__);

	/* Switch to 4-bytes addressing mode */
	result = spi_send_command(flash, sizeof(cmd), 0, cmd, NULL);
	if (!result)
		flash->in_4ba_mode = true;

	return result;
}

/* Enter 4-bytes addressing mode with sending WREN before */
int spi_enter_4ba_b7_we(struct flashctx *flash)
{
	int result;
	struct spi_command cmds[] = {
	{
		.writecnt	= JEDEC_WREN_OUTSIZE,
		.writearr	= (const unsigned char[]){ JEDEC_WREN },
		.readcnt	= 0,
		.readarr	= NULL,
	}, {
		.writecnt	= JEDEC_ENTER_4_BYTE_ADDR_MODE_OUTSIZE,
		.writearr	= (const unsigned char[]){ JEDEC_ENTER_4_BYTE_ADDR_MODE },
		.readcnt	= 0,
		.readarr	= NULL,
	}, {
		.writecnt	= 0,
		.writearr	= NULL,
		.readcnt	= 0,
		.readarr	= NULL,
	}};

	msg_trace("-> %s\n", __func__);

	/* Switch to 4-bytes addressing mode */
	result = spi_send_multicommand(flash, cmds);
	if (result)
		msg_cerr("%s failed during command execution\n", __func__);
	else
		flash->in_4ba_mode = true;
	return result;
}

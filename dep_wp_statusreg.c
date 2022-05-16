/*
 * This file is part of the flashrom project.
 *
 * Copyright (C) 2010 Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "flash.h"
#include "flashchips.h"
#include "chipdrivers.h"
#include "spi.h"
#include "dep_writeprotect.h"
#include "programmer.h"

/* FIXME: Move to spi25.c if it's a JEDEC standard opcode */
uint8_t w25q_read_status_register_2(const struct flashctx *flash)
{
	static const unsigned char cmd[JEDEC_RDSR_OUTSIZE] = { 0x35 };
	unsigned char readarr[2];
	int ret;

	if ((flash->mst->buses_supported & BUS_PROG) && flash->mst->opaque.read_status) {
		msg_cdbg("RDSR2 failed! cmd=0x35 unimpl for opaque chips\n");
		return 0;
	}

	/* Read Status Register */
	ret = spi_send_command(flash, sizeof(cmd), sizeof(readarr), cmd, readarr);
	if (ret) {
		/*
		 * FIXME: make this a benign failure for now in case we are
		 * unable to execute the opcode
		 */
		msg_cdbg("RDSR2 failed!\n");
		readarr[0] = 0x00;
	}

	return readarr[0];
}

/* FIXME: Move to spi25.c if it's a JEDEC standard opcode */
uint8_t mx25l_read_config_register(const struct flashctx *flash)
{
	static const unsigned char cmd[JEDEC_RDSR_OUTSIZE] = { 0x15 };
	unsigned char readarr[2];
	int ret;

	if ((flash->mst->buses_supported & BUS_PROG) && flash->mst->opaque.read_status) {
		msg_cdbg("RDCR failed! cmd=0x15 unimpl for opaque chips\n");
		return 0;
	}

	ret = spi_send_command(flash, sizeof(cmd), sizeof(readarr), cmd, readarr);
	if (ret) {
		msg_cdbg("RDCR failed!\n");
		readarr[0] = 0x00;
	}

	return readarr[0];
}

/*
 * W25Q adds an optional byte to the standard WRSR opcode. If /CS is
 * de-asserted after the first byte, then it acts like a JEDEC-standard
 * WRSR command. if /CS is asserted, then the next data byte is written
 * into status register 2.
 */
#define W25Q_WRSR_OUTSIZE	0x03
int w25q_write_status_register_WREN(const struct flashctx *flash, uint8_t s1, uint8_t s2)
{
	int result;
	struct spi_command cmds[] = {
	{
	/* FIXME: WRSR requires either EWSR or WREN depending on chip type. */
		.writecnt       = JEDEC_WREN_OUTSIZE,
		.writearr       = (const unsigned char[]){ JEDEC_WREN },
		.readcnt        = 0,
		.readarr        = NULL,
	}, {
		.writecnt       = W25Q_WRSR_OUTSIZE,
		.writearr       = (const unsigned char[]){ JEDEC_WRSR, s1, s2 },
		.readcnt        = 0,
		.readarr        = NULL,
	}, {
		.writecnt       = 0,
		.writearr       = NULL,
		.readcnt        = 0,
		.readarr        = NULL,
	}};

	result = spi_send_multicommand(flash, cmds);
	if (result) {
	        msg_cerr("%s failed during command execution\n",
	                __func__);
	}

	/* WRSR performs a self-timed erase before the changes take effect. */
	programmer_delay(100 * 1000);

	return result;
}

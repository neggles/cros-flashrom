/*
 * This file is part of the flashrom project.
 *
 * Copyright (C) 2000 Silicon Integrated System Corporation
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

#include "flash.h"
#include "chipdrivers.h"
#include <inttypes.h>

/* FIXME: The datasheet is unclear whether we should use toggle_ready_jedec
 * or wait_82802ab.
 * FIXME: This file is unused.
 */

int erase_lhf00l04_block(struct flashctx *flash, unsigned int blockaddr, unsigned int blocklen)
{
	chipaddr bios = flash->virtual_memory + blockaddr;
	chipaddr wrprotect = flash->virtual_registers + blockaddr + 2;
	uint8_t status;

	// clear status register
	chip_writeb(flash, 0x50, bios);
	status = wait_82802ab(flash);
	print_status_82802ab(status);
	// clear write protect
	msg_cspew("write protect is at 0x%" PRIxPTR "\n", (wrprotect));
	msg_cspew("write protect is 0x%x\n", chip_readb(flash, wrprotect));
	chip_writeb(flash, 0, wrprotect);
	msg_cspew("write protect is 0x%x\n", chip_readb(flash, wrprotect));

	// now start it
	chip_writeb(flash, 0x20, bios);
	chip_writeb(flash, 0xd0, bios);
	programmer_delay(10);
	// now let's see what the register is
	status = wait_82802ab(flash);
	print_status_82802ab(status);

	/* FIXME: Check the status register for errors. */
	return 0;
}

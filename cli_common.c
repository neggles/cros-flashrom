/*
 * This file is part of the flashrom project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2009 Carl-Daniel Hailfinger
 * Copyright (C) 2011-2014 Stefan Tauner
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
 */

#include <stdlib.h>
#include <string.h>
#include "flash.h"

void print_chip_support_status(const struct flashctx *flash)
{
	if (flash->chip->feature_bits & FEATURE_OTP) {
		msg_cdbg("This chip may contain one-time programmable memory. "
			 "flashrom cannot read\nand may never be able to write "
			 "it, hence it may not be able to completely\n"
			 "clone the contents of this chip (see man page for "
			 "details).\n");
	}
	if ((flash->chip->tested.probe != OK) ||
	    (flash->chip->tested.read != OK) ||
	    (flash->chip->tested.erase != OK) ||
	    (flash->chip->tested.write != OK) ||
	    (flash->chip->tested.uread != OK)) {
		msg_cdbg("===\n");
		if ((flash->chip->tested.probe == BAD) ||
		    (flash->chip->tested.read == BAD) ||
		    (flash->chip->tested.erase == BAD) ||
		    (flash->chip->tested.write == BAD) ||
		    (flash->chip->tested.uread == BAD)) {
			msg_cdbg("This flash part has status NOT WORKING for operations:");
			if (flash->chip->tested.probe == BAD)
				msg_cdbg(" PROBE");
			if (flash->chip->tested.read == BAD)
				msg_cdbg(" READ");
			if (flash->chip->tested.erase == BAD)
				msg_cdbg(" ERASE");
			if (flash->chip->tested.write == BAD)
				msg_cdbg(" WRITE");
			if (flash->chip->tested.uread == BAD)
				msg_cdbg(" UNBOUNDED READ");
			msg_cdbg("\n");
		}
		if ((flash->chip->tested.probe == NT) ||
		    (flash->chip->tested.read == NT) ||
		    (flash->chip->tested.erase == NT) ||
		    (flash->chip->tested.write == NT) ||
		    (flash->chip->tested.uread == NT)) {
			msg_cdbg("This flash part has status UNTESTED for operations:");
			if (flash->chip->tested.probe == NT)
				msg_cdbg(" PROBE");
			if (flash->chip->tested.read == NT)
				msg_cdbg(" READ");
			if (flash->chip->tested.erase == NT)
				msg_cdbg(" ERASE");
			if (flash->chip->tested.write == NT)
				msg_cdbg(" WRITE");
			if (flash->chip->tested.uread == NT)
				msg_cdbg(" UNBOUNDED READ");
			msg_cdbg("\n");
		}
		/* FIXME: This message is designed towards CLI users. */
		msg_cdbg("The test status of this chip may have been updated "
			    "in the latest development\n"
			  "version of flashrom. If you are running the latest "
			    "development version,\n"
			  "please email a report to flashrom@flashrom.org if "
			    "any of the above operations\n"
			  "work correctly for you with this flash part. Please "
			    "include the flashrom\n"
			  "output with the additional -V option for all "
			    "operations you tested (-V, -Vr,\n"
			  "-VE, -Vw), and mention which mainboard or "
			    "programmer you tested.\n"
			  "Please mention your board in the subject line. "
			    "Thanks for your help!\n");
	}
}

/*
 * Return a string corresponding to the bustype parameter.
 * Memory is obtained with malloc() and must be freed with free() by the caller.
 */
char *flashbuses_to_text(enum chipbustype bustype)
{
	char *ret = calloc(1, 1);
	/*
	 * FIXME: Once all chipsets and flash chips have been updated, NONSPI
	 * will cease to exist and should be eliminated here as well.
	 */
	if (bustype == BUS_NONSPI) {
		ret = strcat_realloc(ret, "Non-SPI, ");
	} else {
		if (bustype & BUS_PARALLEL)
			ret = strcat_realloc(ret, "Parallel, ");
		if (bustype & BUS_LPC)
			ret = strcat_realloc(ret, "LPC, ");
		if (bustype & BUS_FWH)
			ret = strcat_realloc(ret, "FWH, ");
		if (bustype & BUS_SPI)
			ret = strcat_realloc(ret, "SPI, ");
		if (bustype & BUS_PROG)
			ret = strcat_realloc(ret, "Programmer-specific, ");
		if (bustype == BUS_NONE)
			ret = strcat_realloc(ret, "None, ");
	}
	/* Kill last comma. */
	ret[strlen(ret) - 2] = '\0';
	ret = realloc(ret, strlen(ret) + 1);
	return ret;
}

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

void print_chip_support_status(const struct flashchip *chip)
{
	if (chip->feature_bits & FEATURE_OTP) {
		msg_cdbg("This chip may contain one-time programmable memory. "
			 "flashrom cannot read\nand may never be able to write "
			 "it, hence it may not be able to completely\n"
			 "clone the contents of this chip (see man page for "
			 "details).\n");
	}
	if ((chip->tested.probe != OK) ||
	    (chip->tested.read != OK) ||
	    (chip->tested.erase != OK) ||
	    (chip->tested.write != OK)) {
		msg_cdbg("===\n");
		if ((chip->tested.probe == BAD) ||
		    (chip->tested.read == BAD) ||
		    (chip->tested.erase == BAD) ||
		    (chip->tested.write == BAD)) {
			msg_cdbg("This flash part has status NOT WORKING for operations:");
			if (chip->tested.probe == BAD)
				msg_cdbg(" PROBE");
			if (chip->tested.read == BAD)
				msg_cdbg(" READ");
			if (chip->tested.erase == BAD)
				msg_cdbg(" ERASE");
			if (chip->tested.write == BAD)
				msg_cdbg(" WRITE");
			msg_cdbg("\n");
		}
		if ((chip->tested.probe == NT) ||
		    (chip->tested.read == NT) ||
		    (chip->tested.erase == NT) ||
		    (chip->tested.write == NT)) {
			msg_cdbg("This flash part has status UNTESTED for operations:");
			if (chip->tested.probe == NT)
				msg_cdbg(" PROBE");
			if (chip->tested.read == NT)
				msg_cdbg(" READ");
			if (chip->tested.erase == NT)
				msg_cdbg(" ERASE");
			if (chip->tested.write == NT)
				msg_cdbg(" WRITE");
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

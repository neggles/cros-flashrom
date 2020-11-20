/*
 * This file is part of the flashrom project.
 *
 * Copyright (C) 2020 The Chromium OS Authors. All rights reserved.
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

#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "flash.h"
#include "programmer.h"
#include "hwaccess.h"

/* ugly singleton to work around cros layering violations in action_descriptor.c */
static int ec_alias_path = 0;

int programming_ec(void)
{
	return ec_alias_path;
}

int cros_ec_alias_init(void)
{
	/* User called '-p ec' and so toggle ec-alias path detection on. */
	ec_alias_path = 1;

	/* probe for programmers that bridge LPC <--> SPI */
	/* Try to probe via kernel device first */
	if (!cros_ec_probe_dev()) {
		return 0;
	}

#if defined(__i386__) || defined(__x86_64__)
#if CONFIG_MEC1308 == 1
	if (!mec1308_init()) {
		msg_cdbg("legacy x86 EC: mec1308 found!\n");
		return 0;
	}
#endif
#if CONFIG_ENE_LPC == 1
	if (!ene_lpc_init()) {
		msg_cdbg("legacy x86 EC: ene_lpc found!\n");
		return 0;
	}
#endif
	msg_cdbg("legacy x86 EC not found!\n");
	return 1;	/* EC not found */
#else
	return 0;
#endif /* !__i386__ || __x86_64__ */
}

int cros_host_alias_init(void)
{
	msg_pdbg("%s(): Redirecting dispatch -> internal_init().\n", __func__);
	return internal_init();
}

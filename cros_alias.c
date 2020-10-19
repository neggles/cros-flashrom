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
		buses_supported &= ~(BUS_LPC|BUS_SPI);
		return 0;
	}
#if defined(__i386__) || defined(__x86_64__)
	if (wpce775x_probe_spi_flash(NULL)
#if CONFIG_MEC1308 == 1
		&& mec1308_init()
#endif
#if CONFIG_ENE_LPC == 1
		&& ene_lpc_init()
#endif
		)
		return 1;	/* EC not found */
#endif /* __i386__ || __x86_64__ */

	return 0;
}

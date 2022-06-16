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

#include "programmer.h"
#include "hwaccess_physmap.h"

static int cros_host_alias_init(void)
{
	msg_pdbg("%s(): Redirecting dispatch -> internal_init().\n", __func__);
	return internal_init();
}

const struct programmer_entry programmer_google_host_alias = {
	.name			= "host",
	.type			= OTHER,
	.devs.note		= "Google host alias mechanism.\n",
	.init			= cros_host_alias_init,
	.map_flash_region	= physmap,
	.unmap_flash_region	= physunmap,
	.delay			= internal_delay,

	/*
	 * "Internal" implies in-system programming on a live system, so
	 * handle with paranoia to catch errors early. If something goes
	 * wrong then hopefully the system will still be recoverable.
	 */
	.paranoid		= 1,
};

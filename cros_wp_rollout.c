/*
 * This file is part of the flashrom project.
 *
 * Copyright 2022 Google LLC
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

#include <string.h>
#include <stdbool.h>
#include "programmer.h"
#include "cros_wp_rollout.h"

/* Global ICH generation variable used to determine what platform we're on and
 * enable incremental switch over to upstream writeprotect. */
extern enum ich_chipset ich_generation;

static bool use_dep_wp_host()
{
#if (defined (__i386__) || defined (__x86_64__) || defined(__amd64__))
	if (ich_generation == CHIPSET_ICH_UNKNOWN)
		return true; /* AMD - sb600spi */
	else
		return true; /* Intel - ichspi */
#else
	return true; /* ARM - linux_mtd */
#endif
}

/* TODO: Switch over to new wp and delete old. */
bool use_dep_wp(const char *programmer_name)
{
	bool use_old_wp;

	/* TODO(b/236214660): enable new writeprotect for internal/host */
	if (!strcmp(programmer_name, "host") || !strcmp(programmer_name, "internal"))
		use_old_wp = use_dep_wp_host();
	/* TODO(b/236214918): enable new writeprotect for EC */
	else if (!strcmp(programmer_name, "ec"))
		use_old_wp = true;
	else /* not EC || AP. */
		use_old_wp = false;

	return use_old_wp;
}

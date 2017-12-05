/*
 * This file is part of the flashrom project.
 *
 * Copyright (C) 2011 The Chromium OS Authors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * power.c: power management routines
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>

#include "flash.h"	/* for msg_* */
#include "power.h"

/*
 * Path to directory and file containing flashrom's PID.
 * While present, powerd avoids suspending or shutting down the system.
 */
#define POWERD_LOCK_DIR		"/run/lock/power_override"
#define POWERD_LOCK_FILE	POWERD_LOCK_DIR "/flashrom.lock"

int disable_power_management()
{
	FILE *lock_file = NULL;
	int rc = 0;

	/* Don't do anything if powerd isn't expected to run. */
	if (access(POWERD_LOCK_DIR, F_OK) != 0) {
		return 0;
	}

	msg_pdbg("%s: Disabling power management.\n", __func__);

	if (!(lock_file = fopen(POWERD_LOCK_FILE, "w"))) {
		msg_perr("%s: Failed to open %s for writing: %s\n",
			__func__, POWERD_LOCK_FILE, strerror(errno));
		return 1;
	}

	if (fprintf(lock_file, "%ld", (long)getpid()) < 0) {
		msg_perr("%s: Failed to write PID to %s: %s\n",
			__func__, POWERD_LOCK_FILE, strerror(errno));
		rc = 1;
	}

	if (fclose(lock_file) != 0) {
		msg_perr("%s: Failed to close %s: %s\n",
			__func__, POWERD_LOCK_FILE, strerror(errno));
	}
	return rc;

}

int restore_power_management()
{
	int result = 0;

	/* Don't do anything if powerd isn't expected to run. */
	if (access(POWERD_LOCK_DIR, F_OK) != 0) {
		return 0;
	}

	msg_pdbg("%s: Re-enabling power management.\n", __func__);

	result = unlink(POWERD_LOCK_FILE);
	if (result != 0 && errno != ENOENT)  {
		msg_perr("%s: Failed to unlink %s: %s\n",
			__func__, POWERD_LOCK_FILE, strerror(errno));
		return 1;
	}
	return 0;
}

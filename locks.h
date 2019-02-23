/*
 * Copyright (C) 2010 Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * locks.h: locks are used to preserve atomicity of operations.
 */

#ifndef LOCKS_H__
#define LOCKS_H__

#define SYSTEM_LOCKFILE_DIR	"/run/lock"
#define LOCKFILE_NAME		"firmware_utility_lock"
#define CROS_EC_LOCKFILE_NAME	"cros_ec_lock"

#endif /* LOCKS_H__ */

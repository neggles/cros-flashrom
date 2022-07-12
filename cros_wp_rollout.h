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

#ifndef __CROS_WP_ROLLOUT_H__
#define __CROS_WP_ROLLOUT_H__

/* Must be called after programmer init so that g_ich_generation is set. */
bool use_dep_wp(const char *programmer_name);

#endif /* __CROS_WP_ROLLOUT_H__ */

/*
 * This file is part of the flashrom project.
 *
 * Copyright (C) 2010 Google Inc.
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

#ifndef __DEP_WRITEPROTECT_H__
#define __DEP_WRITEPROTECT_H__ 1

#include "flash.h"

enum wp_mode {
	WP_MODE_UNKNOWN = -1,
	WP_MODE_HARDWARE,	/* hardware WP pin determines status */
	WP_MODE_POWER_CYCLE,	/* WP active until power off/on cycle */
	WP_MODE_PERMANENT,	/* status register permanently locked,
				   WP permanently enabled */
};

struct wp {
	int (*list_ranges)(const struct flashctx *flash);
	int (*set_range)(const struct flashctx *flash,
			 unsigned int start, unsigned int len);
	int (*enable)(const struct flashctx *flash, enum wp_mode mode);
	int (*disable)(const struct flashctx *flash);
	int (*wp_status)(const struct flashctx *flash,
			uint32_t *start, uint32_t *len, bool *wp_en);
};

/* winbond w25-series */
extern struct wp wp_w25;	/* older winbond chips (w25p, w25x, etc) */
extern struct wp wp_w25q;
extern struct wp wp_w25q_large; /* large winbond chips (>= 32MB) */

extern struct wp wp_generic;
extern struct wp wp_wpce775x;

struct wp *get_wp_for_flashchip(const struct flashchip *chip);
enum wp_mode get_wp_mode(const char *mode_str);

/*
 * Generic write-protect stuff
 */

struct modifier_bits {
	int sec;	/* if 1, bp bits describe sectors */
	int tb;		/* value of top/bottom select bit */
};

/* wp_statusreg.c */
uint8_t w25q_read_status_register_2(const struct flashctx *flash);
uint8_t mx25l_read_config_register(const struct flashctx *flash);
int w25q_write_status_register_WREN(const struct flashctx *flash, uint8_t s1, uint8_t s2);

#endif /* !__DEP_WRITEPROTECT_H__ */

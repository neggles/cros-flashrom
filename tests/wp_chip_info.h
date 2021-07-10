/*
 * This file is part of the flashrom project.
 *
 * Copyright 2021 Google LLC
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

#ifndef WP_CHIP_INFO_H
#define WP_CHIP_INFO_H

#include <stdint.h>
#include <stdbool.h>
#include "flash.h"

/* Stores all additional chip-specific information required for testing. */
struct wp_chip_info {
	uint8_t mask;
	bool has_sr2;

	// Some W25 chips have CMP bit in SR2
	bool sr2_bit6_is_cmp;

	// Some MX chips have TB bit in separate config register
	bool mxcr_bit3_is_tb;

	// S25F chips have TB bit stored in separate register
	bool is_s25f;

	// Bits in SR1 used to select protection range (BP, SEC, etc).
	// Any bits not in this mask do not affect selected wp range.
	uint8_t protect_mask;

	const char *name;
};

extern const struct wp_chip_info chips_to_test[];
extern const size_t chips_to_test_len;

#endif

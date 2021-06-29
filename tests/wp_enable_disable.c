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

#include <include/test.h>
#include <string.h>

#include "flashchips.h"
#include "writeprotect.h"
#include "wp_tests.h"


struct chip_info {
	uint8_t mask;
	bool has_sr2;
	const char *name;
};

static const struct chip_info chips_to_test[] = {
	{ 0x9C, 0, "A25L040" },
	{ 0x9C, 1, "AT25SF128A" },
	{ 0x9C, 1, "AT25SL128A" },
	{ 0x9C, 0, "EN25F40" },
	{ 0x9C, 0, "EN25Q128" },
	{ 0x9C, 0, "EN25Q32(A/B)" },
	{ 0x9C, 0, "EN25Q40" },
	{ 0x9C, 0, "EN25Q64" },
	{ 0x9C, 0, "EN25Q80(A)" },
	{ 0x9C, 0, "EN25QH128" },
	{ 0x9C, 0, "EN25S64" },
	{ 0xFC, 0, "GD25LQ128C/GD25LQ128D" },
	{ 0x9C, 0, "GD25LQ32" },
	{ 0x9C, 0, "GD25LQ64(B)" },
	{ 0x9C, 0, "GD25Q127C/GD25Q128C" },
	{ 0x9C, 1, "GD25Q256D" },
	{ 0xFC, 0, "GD25Q32(B)" },
	{ 0x9C, 0, "GD25Q64(B)" },
	{ 0x9C, 0, "MX25L1005(C)/MX25L1006E" },
	{ 0x9C, 0, "MX25L1605" },
	{ 0x9C, 0, "MX25L2005(C)/MX25L2006E" },
	{ 0xBC, 0, "MX25L25635F/MX25L25645G" },
	{ 0x9C, 0, "MX25L3205(A)" },
	{ 0x9C, 0, "MX25L4005(A/C)/MX25L4006E" },
	{ 0xBC, 0, "MX25L6405" },
	{ 0xBC, 0, "MX25L6495F" },
	{ 0x9C, 0, "MX25L8005/MX25L8006E/MX25L8008E/MX25V8005" },
	{ 0x9C, 1, "MX25U12835F" },
	{ 0x9C, 0, "MX25U3235E/F" },
	{ 0x9C, 0, "MX25U6435E/F" },
	{ 0x9C, 0, "N25Q064..1E" },
	{ 0x9C, 0, "N25Q064..3E" },
	{ 0x9C, 1, "W25Q128.JW.DTR" },
	{ 0x9C, 1, "W25Q128.V" },
	{ 0x9C, 0, "W25Q128.V..M" },
	{ 0x9C, 1, "W25Q128.W" },
	{ 0x9C, 1, "W25Q16.V" },
	{ 0x9C, 1, "W25Q256JV_M" },
	{ 0x9C, 1, "W25Q256.V" },
	{ 0x9C, 1, "W25Q32JW...M" },
	{ 0x9C, 1, "W25Q32.V" },
	{ 0x9C, 1, "W25Q32.W" },
	{ 0x9C, 1, "W25Q64.V" },
	{ 0x9C, 1, "W25Q64.W" },
	{ 0x9C, 1, "W25Q80.V" },
	{ 0x9C, 0, "W25X10" },
	{ 0x9C, 0, "W25X20" },
	{ 0x9C, 0, "W25X40" },
	{ 0x9C, 0, "W25X80" },
	{ 0x9C, 1, "XM25QH256C" },

	// Not properly supported, see comment in get_wp_for_flashchip()
	// { ????, ?, "W25Q64JW..IM" },
};
static size_t chips_to_test_len = sizeof(chips_to_test) / sizeof(struct chip_info);

static const struct flashchip *get_chip_by_name(const char *name)
{
	for(size_t i = 0; i < flashchips_size; i++) {
		if(!strcmp(flashchips[i].name, name)) {
			return &flashchips[i];
		}
	}
	return NULL;
}

static void test_wp_disable_with_parameters(
		const struct wp *wp,
		const struct flashctx *flash,
		const struct chip_info info,
		uint8_t first_sr1_read_value,
		uint8_t second_sr1_read_value)
{
	// Expect wp_disable to read current SR1 value
	expect_sr1_read(first_sr1_read_value);

	// Expect wp_disable to write back the SR1 value with the SRP0 bit cleared
	uint8_t expected_write = first_sr1_read_value & ~0x80;
	expect_sr1_write(expected_write);

	// Expect wp_disable to read SR1 again for verification
	expect_sr1_read(second_sr1_read_value);

	// Operation should fail if any wp-related bits are different to
	// the value that was previously written
	int expected_result = (second_sr1_read_value & info.mask) !=
		(expected_write & info.mask);

	// Various code paths return inconsistent error codes, accept anything
	// non-zero as an error
	int result = wp->disable(flash) != 0;

	assert_int_equal(result, expected_result);
}

static void test_wp_disable_for_chip(const struct chip_info info)
{
	struct flashctx flash;
	flash.chip = (struct flashchip *)get_chip_by_name(info.name);
	const struct wp* wp = get_wp_for_flashchip(flash.chip);

	// wp_disable() reads the SR1 twice; loop over all values for both
	// reads to ensure all possible cases are tested.
	for (int first_sr1 = 0; first_sr1 < 0x100; first_sr1++) {
		for (int second_sr1 = 0; second_sr1 < 0x100; second_sr1++) {
			test_wp_disable_with_parameters(wp, &flash, info,
					first_sr1, second_sr1);
		}
	}
}

void test_wp_disable(void **state)
{
	(void) state;

	for(size_t i = 0; i < chips_to_test_len; i++) {
		test_wp_disable_for_chip(chips_to_test[i]);
	}
}

static void test_wp_enable_with_parameters(
		const struct wp *wp,
		const struct flashctx *flash,
		const struct chip_info info,
		uint8_t first_sr1_read_value,
		uint8_t second_sr1_read_value,
		uint8_t first_sr2_read_value)
{

	bool expect_early_exit = false;
	int expected_result = 0;

	uint8_t expected_write = first_sr1_read_value | 0x80;

	// Chips with a second status register have some additional checks
	// performed before setting the SRP0 bit
	if(info.has_sr2) {
		// WP code will read SR2 to check if powercycle/permanent wp is enabled
		expect_sr2_read(first_sr2_read_value);

		// If powercycle/permanent wp is enabled, expect exit with error
		if (first_sr2_read_value & 0x1) {
			expect_early_exit = true;
			expected_result = 1;
		}
		else {
			// Writeprotect checks to see if SRP0 bit is already set
			expect_sr1_read(first_sr1_read_value);

			// Expect return success if the bit is already set
			if(expected_write == first_sr1_read_value) {
				expect_early_exit = true;
				expected_result = 0;
			}
		}
	}

	if(!expect_early_exit) {
		// Expect wp_disable to read current SR1 value
		// Note: the SR1 may have already been read, but assume chips
		// won't change the SR1 between successive reads and return the
		// same value here.
		expect_sr1_read(first_sr1_read_value);

		// Expect wp_disable to write back the SR1 value with the SRP0 bit set
		expect_sr1_write(expected_write);

		// Expect wp_disable to read SR1 again for verification
		expect_sr1_read(second_sr1_read_value);

		// Operation should fail if any wp-related bits are different to
		// the value that was previously written
		expected_result = (second_sr1_read_value & info.mask) !=
			(expected_write & info.mask);
	}

	// Various code paths return inconsistent error codes, accept anything
	// non-zero as an error
	int result = wp->enable(flash, WP_MODE_HARDWARE) != 0;

	assert_int_equal(result, expected_result);
}

static void test_wp_enable_for_chip(const struct chip_info info)
{
	struct flashctx flash;
	flash.chip = (struct flashchip *)get_chip_by_name(info.name);
	const struct wp* wp = get_wp_for_flashchip(flash.chip);

	for (int first_sr1 = 0; first_sr1 < 0x100; first_sr1++) {
		for (int second_sr1 = 0; second_sr1 < 0x100; second_sr1++) {
			// Only test with SR2 = 0x00 and SR2 = 0x01, since
			// we know that only the lowest bit is examined by
			// writeprotect code. Not ideal, but much faster than
			// testing everything.
			for (int first_sr2 = 0; first_sr2 < 2; first_sr2++) {
				test_wp_enable_with_parameters(
						wp, &flash, info,
						first_sr1, second_sr1, first_sr2);
			}
		}
	}
}

void test_wp_enable(void **state)
{
	(void) state;

	for(size_t i = 0; i < chips_to_test_len; i++) {
		test_wp_enable_for_chip(chips_to_test[i]);
	}
}

/* Copyright 2018 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "action_descriptor.h"
#include "flash.h"
#include "flashchips.h"


#define SMALLEST_BLOCK_SZ  4096
#define SECOND_SMALLEST_BLOCK_SZ (SMALLEST_BLOCK_SZ * 8)
#define LARGER_BLOCK_SZ  (SECOND_SMALLEST_BLOCK_SZ * 2)

/* Fake erase function pointer to plug into the test chip descriptor. */
static int dummy_erase(struct flashctx *flash,
		       unsigned int addr,
		       unsigned int blocklen)
{
	return 0;
}

/*
 * Let's pretend we are dealing with a slightly modified Windbond W25Q64 chip,
 * copy its description from flashchips.c dropping pointers to functions and
 * adding a second eraseblocks option for the lowest size range.
 */
static struct flashchip test_chip = {
	.vendor		= "Winbond",
	.name		= "W25Q64",
	.bustype	= BUS_SPI,
	.manufacture_id	= WINBOND_NEX_ID,
	.model_id	= WINBOND_NEX_W25Q64_V,
	.total_size	= 8192,
	.page_size	= 256,
	.feature_bits	= FEATURE_WRSR_WREN | FEATURE_UNBOUND_READ | FEATURE_OTP,
	.tested		= TEST_OK_PREWU,
	.block_erasers	=
	{
		{
			.eraseblocks = { {SMALLEST_BLOCK_SZ, 128},
					 {SMALLEST_BLOCK_SZ, 2048} },
			.block_erase = dummy_erase,
		}, {
			.eraseblocks = { {SECOND_SMALLEST_BLOCK_SZ, 256} },
			.block_erase = dummy_erase,
		}, {
			.eraseblocks = { {LARGER_BLOCK_SZ, 128} },
			.block_erase =  dummy_erase,
		}, {
			.eraseblocks = { {8 * 1024 * 1024, 1} },
			.block_erase = dummy_erase,
		}, {
			.eraseblocks = { {8 * 1024 * 1024, 1} },
			.block_erase = dummy_erase,
		},
	}
};

/*
 * Check processing units array contents for various fixed size tests.
 *
 * @ad           pointer to a generated action descriptor structure
 * @expected_pu  pointer to the expected values in processing_units array
 * @num_blocks   number of blocks expected to be added to the array
 *
 * returns zero on success, on error - 1 based block number where the error
 *		was detected.
 */
static int check_pus(struct action_descriptor *ad,
		     const struct processing_unit *expected_pu,
		     size_t num_blocks)
{
	size_t i;
	struct processing_unit pu = *expected_pu;

	for (i = 0; i < num_blocks; i++) {
		if (memcmp(ad->processing_units + i, &pu, sizeof(pu)))
			return i + 1;
		/*
		 * In case more than one processing units are present, they
		 * are expected to be 8 blocks apart.
		 */
		pu.offset = pu.offset + pu.block_size * 8;
	}

	if (ad->processing_units[i].num_blocks)
		return i + 1;

	return 0;
}

/*
 * Test one erase operation.
 *
 * The caller prepares old and new buffers and the created action descriptor
 * is supposed to include a processing unit with a certain number of blocks
 * and of certain contents.
 *
 * @fctx         flash context
 *
 * @oldi         pointer to buffer of the flash chip size bytes containing old
 *		 data
 *
 * @newi	 pointer to buffer of the flash chip size bytes containing new
 *		 data
 * @expected_pu pointer to the expected contents of the processing_units array
 * @num_blocks  number of blocks in the array
 * @test_num    test number (for reporting purposes)
 */
static int test_one_erase(struct flashctx *fctx,
			  uint8_t *oldi,
			  uint8_t *newi,
			  const struct processing_unit *expected_pu,
			  size_t num_blocks,
			  int test_num)
{
	int result = 0;
	struct action_descriptor *ad;

	msg_pinfo("%s test %d...", __func__, test_num);
	ad = prepare_action_descriptor(fctx, oldi, newi);

	if (ad) {
		int rv = check_pus(ad, expected_pu, num_blocks);

		if (rv) {
			result = 1;
			msg_pinfo("failed\n");
			fprintf(stderr, "\%s: test %d failed on processing unit %d!\n",
				__func__, test_num, rv - 1);
		} else {
			msg_pinfo("passed\n");
		}
		free(ad);
	} else {
		msg_pinfo("failed\n");
		fprintf(stderr, "%s:%d action description creation failed!\n",
				__func__, __LINE__);
		result = 1;
	}

	return result;
}

/*
 * Swap two eraser structures in the flash descriptor to verify that the
 * erasers are sorted properly.
 */
static void swap_erasers(struct flashctx *fctx, unsigned a, unsigned b)
{
	struct block_eraser swap_eraser;

	/* Modify descriptor to verify erases sort by size works. */
	swap_eraser = fctx->chip->block_erasers[a];
	fctx->chip->block_erasers[a] = fctx->chip->block_erasers[b];
	fctx->chip->block_erasers[b] = swap_eraser;
}

/* Verify that largest erase size is selected when appropriate. */
static int test_largest_erase(uint8_t *oldi, uint8_t *newi,
			      struct flashctx *fctx, size_t chip_size)
{
	int result = 0;
	unsigned i;
	size_t block_count;

	memset(oldi, ~flash_erase_value(fctx), chip_size);
	memset(newi, flash_erase_value(fctx), chip_size);

	{
		struct processing_unit expected_pu = { chip_size, 0, 1, 4, 0 };

		result += test_one_erase(fctx, oldi, newi, &expected_pu, 1, 1);
	}

	swap_erasers(fctx, 0, 4);
	{
		struct processing_unit expected_pu = { chip_size, 0, 1, 3, 0 };

		result += test_one_erase(fctx, oldi, newi, &expected_pu, 1, 2);
	}

	/* Restore erasers order. */
	swap_erasers(fctx, 0, 4);

	/*
	 * Modify old contents such that enough smaller blocks require
	 * erasing.
	 */
	/* Let's corrupt 6 out every 8 blocks. */
	memset(oldi, flash_erase_value(fctx), chip_size);
	block_count = chip_size/SMALLEST_BLOCK_SZ;
	for (i = 0; i < block_count; i++)
		if ((i % 8) < 6)
			oldi[i * SMALLEST_BLOCK_SZ] =
				~flash_erase_value(fctx);

	{
		struct processing_unit expected_pu = { chip_size, 0, 1, 4, 0 };

		result += test_one_erase(fctx, oldi, newi, &expected_pu, 1, 3);
	}

	/* Now, let's corrupt less than 6 out of every 8 blocks. */
	memset(oldi, flash_erase_value(fctx), chip_size);
	block_count = chip_size/SMALLEST_BLOCK_SZ;
	for (i = 0; i < block_count; i++)
		if ((i % 8) < 5)
			oldi[i * SMALLEST_BLOCK_SZ] =
				~flash_erase_value(fctx);

	{
		struct processing_unit expected_pu = { SMALLEST_BLOCK_SZ,
						       0, 5, 0, 1 };

		result += test_one_erase(fctx, oldi, newi,
					 &expected_pu, 256, 4);
	}

	return result;
}

/* Verify that smallest erase size is selected when appropriate. */
static int test_smallest_erase(uint8_t *oldi, uint8_t *newi,
			       struct flashctx *fctx, size_t chip_size)
{
	int result = 0;

	memset(oldi, flash_erase_value(fctx), chip_size);
	memset(newi, flash_erase_value(fctx), chip_size);

	oldi[0] = ~oldi[0];
	{
		struct processing_unit expected_pu = {
			SMALLEST_BLOCK_SZ, 0, 1, 0, 0
		};

		result += test_one_erase(fctx, oldi, newi, &expected_pu, 1, 5);
	}

	oldi[0] = ~oldi[0];
	oldi[chip_size/2] = ~oldi[chip_size/2];
	{
		struct processing_unit expected_pu = {
			SMALLEST_BLOCK_SZ, chip_size/2, 1, 0, 1
		};

		result += test_one_erase(fctx, oldi, newi, &expected_pu, 1, 6);
	}

	return result;
}

/* Verify a case when variable sizes need to be erased. */
static int test_assorted_erase(uint8_t *oldi, uint8_t *newi,
			       struct flashctx *fctx, size_t chip_size)
{
	int i;
	int j;
	int k;
	struct action_descriptor *ad;
	int rv;
	const struct processing_unit pus[] = {
		{
			.block_size = 0x1000,
			.offset = 0x0,
			.num_blocks = 0x5,
			.block_eraser_index = 0x0,
			.block_region_index = 0x1
		}, {
			.block_size = 0x1000,
			.offset = 0x103000,
			.num_blocks = 0x5,
			.block_eraser_index = 0x0,
			.block_region_index = 0x1
		}, {
			.block_size = 0x1000,
			.offset = 0x10b000,
			.num_blocks = 0x5,
			.block_eraser_index = 0x0,
			.block_region_index = 0x1
		}, {
			.block_size = 0x1000,
			.offset = 0x608000,
			.num_blocks = 0x4,
			.block_eraser_index = 0x0,
			.block_region_index = 0x1
		}, {
			.block_size = 0x1000,
			.offset = 0x618000,
			.num_blocks = 0x4,
			.block_eraser_index = 0x0,
			.block_region_index = 0x1
		}, {
			.block_size = 0x8000,
			.offset = 0x600000,
			.num_blocks = 0x1,
			.block_eraser_index = 0x1,
			.block_region_index = 0x0
		}, {
			.block_size = 0x8000,
			.offset = 0x610000,
			.num_blocks = 0x1,
			.block_eraser_index = 0x1,
			.block_region_index = 0x0
		}, {
			.block_size = 0x10000,
			.offset = 0x210000,
			.num_blocks = 0x1,
			.block_eraser_index = 0x2,
			.block_region_index = 0x0
		}, {
			.block_size = 0x1000,
			.offset = 0x40000,
			.num_blocks = 0x0,
			.block_eraser_index = 0x0,
			.block_region_index = 0x1
		}
	};

	msg_pinfo("Test assorted sizes...");
	memset(oldi, flash_erase_value(fctx), chip_size);
	memset(newi, flash_erase_value(fctx), chip_size);

	/* Five smallest blocks at the bottom. */
	for (i = 0; i < 5; i++)
		oldi[i * SMALLEST_BLOCK_SZ + i] =  ~oldi[1];

	/* Couple of second smallest block sizes at 1MB+ */
	i = 0x100000;
	for (j = 0; j < 2; j++) {
		/*
		 * Setting up less than 6 smallest blocks, should be no
		 * folding into larger block.
		 */
		for (k = 3;
		     k < SECOND_SMALLEST_BLOCK_SZ / SMALLEST_BLOCK_SZ;
		     k++) {
			int index = i +
				j * SECOND_SMALLEST_BLOCK_SZ +
			        k * SMALLEST_BLOCK_SZ + j + k;
			oldi[index] =  ~oldi[1];
		}
	}

	i = 0x200000;
	for (j = 2; j < 4; j++) {
		/*
		 * Setting up 6 smallest blocks, should fold into larger block.
		 */
		for (k = 2;
		     k < SECOND_SMALLEST_BLOCK_SZ / SMALLEST_BLOCK_SZ;
		     k++) {
			int index = i +
				j * SECOND_SMALLEST_BLOCK_SZ +
			        k * SMALLEST_BLOCK_SZ + j + k;

			oldi[index] = ~oldi[1];
		}
	}

	/* Couple of sparsely changed larger block sizes at 6MB */
	i = 0x600000;
	for (j = 0; j < 2; j++) {
		for (k = 2; k < 4; k++) {
			int index = i +
				j * LARGER_BLOCK_SZ +
				k * SMALLEST_BLOCK_SZ + j + k;

			memset(oldi + index, ~oldi[1], SMALLEST_BLOCK_SZ * 8);
		}
	}

	ad = prepare_action_descriptor(fctx, oldi, newi);
	rv = memcmp(ad->processing_units, pus, sizeof(pus));
	free(ad);

	if (rv)
		msg_pinfo("failed\n");
	else
		msg_pinfo("passed\n");

	return rv;
}

static int run_tests(uint8_t *oldi, uint8_t *newi,
		     struct flashctx *fctx, size_t chip_size)
{
	int result = 0;

	result += test_largest_erase(oldi, newi, fctx, chip_size);
	result += test_smallest_erase(oldi, newi, fctx, chip_size);
	result += test_assorted_erase(oldi, newi, fctx, chip_size);

	return result;
}

int test_action_descriptor(void)
{
	struct flashctx fctx;
	size_t chip_size;
	int rv = 1;

	void *oldimage, *newimage;

	memset(&fctx, 0, sizeof(fctx));
	fctx.chip = &test_chip;

	/* Cache it here to avoid extra calculations. */
	chip_size = test_chip.total_size * 1024;

	oldimage = malloc(chip_size);
	newimage = malloc(chip_size);
	if (oldimage && newimage)
		rv = run_tests(oldimage, newimage, &fctx, chip_size);
	else
		fprintf(stderr, "ERROR: malloc failed for %zd bytes\n",
			2 * chip_size);

	free(oldimage);
	free(newimage);

	return rv;
}

/*
 * This file is part of the flashrom project.
 *
 * Copyright (C) 2000 Silicon Integrated System Corporation
 * Copyright (C) 2004 Tyan Corp
 * Copyright (C) 2005-2008 coresystems GmbH <stepan@openbios.org>
 * Copyright (C) 2006-2009 Carl-Daniel Hailfinger
 * Copyright (C) 2009 Sean Nelson <audiohacked@gmail.com>
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

#include <stdlib.h>
#include <string.h>

#include "flash.h"
#include "flashchips.h"
#include "chipdrivers.h"
#include "writeprotect.h"

/**
 * List of supported flash chips.
 *
 * Please keep the list sorted by vendor name and chip name, so that
 * the output of 'flashrom -L' is alphabetically sorted.
 */
const struct flashchip flashchips[] = {

	/*
	 * .vendor		= Vendor name
	 * .name		= Chip name
	 * .bustype		= Supported flash bus types (Parallel, LPC...)
	 * .manufacture_id	= Manufacturer chip ID
	 * .model_id		= Model chip ID
	 * .total_size		= Total size in (binary) kbytes
	 * .page_size		= Page or eraseblock(?) size in bytes
	 * .tested		= Test status
	 * .probe		= Probe function
	 * .probe_timing	= Probe function delay
	 * .block_erasers[]	= Array of erase layouts and erase functions
	 * {
	 *	.eraseblocks[]	= Array of { blocksize, blockcount }
	 *	.block_erase	= Block erase function
	 * }
	 * .printlock		= Chip lock status function
	 * .unlock		= Chip unlock function
	 * .write		= Chip write function
	 * .read		= Chip read function
	 * .voltage		= Voltage range in millivolt
	 */

	{
		.vendor		= "AMD",
		.name		= "Am29F002(N)BB",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29F002BB,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_SHORT_RESET | FEATURE_ADDR_2AA,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 3},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4750, 5250}, /* 4.75-5.25V for type -55, others 4.5-5.5V */
	},

	{
		.vendor		= "AMD",
		.name		= "Am29F002(N)BT",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29F002BT,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_EITHER_RESET | FEATURE_ADDR_2AA,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4750, 5250}, /* 4.75-5.25V for type -55, others 4.5-5.5V */
	},

	{
		.vendor		= "AMD",
		.name		= "Am29F010A/B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29F010,	/* Same as Am29F010A */
		.total_size	= 128,
		.page_size	= 16 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PRE,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {16 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "AMD",
		.name		= "Am29F016D",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29F016D,
		.total_size	= 2 * 1024,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {2048 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "AMD",
		.name		= "Am29F040B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29F040,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "AMD",
		.name		= "Am29F080B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29F080,
		.total_size	= 1024,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "AMD",
		.name		= "Am29LV001BB",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29LV001BB,
		.total_size	= 128,
		.page_size	= 64 * 1024, /* unused */
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{8 * 1024, 1},
					{4 * 1024, 2},
					{16 * 1024, 7},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* 3.0-3.6V for type -45R, others 2.7-3.6V */
	},

	{
		.vendor		= "AMD",
		.name		= "Am29LV001BT",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29LV001BT,
		.total_size	= 128,
		.page_size	= 64 * 1024, /* unused */
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 7},
					{4 * 1024, 2},
					{8 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* 3.0-3.6V for type -45R, others 2.7-3.6V */
	},

	{
		.vendor		= "AMD",
		.name		= "Am29LV002BB",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29LV002BB,
		.total_size	= 256,
		.page_size	= 64 * 1024, /* unused */
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 3},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* 3.0-3.6V for type -55, others 2.7-3.6V */
	},

	{
		.vendor		= "AMD",
		.name		= "Am29LV002BT",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29LV002BT,
		.total_size	= 256,
		.page_size	= 64 * 1024, /* unused */
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* 3.0-3.6V for type -55, others 2.7-3.6V */
	},

	{
		.vendor		= "AMD",
		.name		= "Am29LV004BB",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29LV004BB,
		.total_size	= 512,
		.page_size	= 64 * 1024, /* unused */
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 7},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMD",
		.name		= "Am29LV004BT",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29LV004BT,
		.total_size	= 512,
		.page_size	= 64 * 1024, /* unused */
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 7},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMD",
		.name		= "Am29LV008BB",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29LV008BB,
		.total_size	= 1024,
		.page_size	= 64 * 1024, /* unused */
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 15},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* 3.0-3.6V for type -70R, others 2.7-3.6V */
	},

	{
		.vendor		= "AMD",
		.name		= "Am29LV008BT",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29LV008BT,
		.total_size	= 1024,
		.page_size	= 64 * 1024, /* unused */
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 15},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* 3.0-3.6V for type -70R, others 2.7-3.6V */
	},

	{
		.vendor		= "AMD",
		.name		= "Am29LV040B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29LV040B,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_OK_PRE,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* 3.0-3.6V for type -60R, others 2.7-3.6V*/
	},

	{
		.vendor		= "AMD",
		.name		= "Am29LV081B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMD_ID,
		.model_id	= AMD_AM29LV080B,
		.total_size	= 1024,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET, /* datasheet specifies address as don't care */
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* 3.0-3.6V for type -70R, others 2.7-3.6V */
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L010",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID_NOPREFIX,
		.model_id	= AMIC_A25L010,
		.total_size	= 128,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { { 4 * 1024, 32 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { { 64 * 1024, 2 } },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { { 128 * 1024, 1 } },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp2_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L016",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID_NOPREFIX,
		.model_id	= AMIC_A25L016,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { { 4 * 1024, 512 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { { 64 * 1024, 32 } },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { { 2048 * 1024, 1 } },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp2_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L020",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID_NOPREFIX,
		.model_id	= AMIC_A25L020,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { { 4 * 1024, 64 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { { 64 * 1024, 4 } },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { { 256 * 1024, 1 } },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp2_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L032",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID_NOPREFIX,
		.model_id	= AMIC_A25L032,
		.total_size	= 4096,
		.page_size	= 256,
		/* OTP: 64B total; read 0x4B, 0x48; write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { { 4 * 1024, 1024 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { { 64 * 1024, 64 } },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { { 64 * 1024, 64 } },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { { 4096 * 1024, 1 } },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { { 4096 * 1024, 1 } },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_amic_a25l032, /* bit5: T/B, bit6: prot size */
		.unlock		= spi_disable_blockprotect_bp2_srwd, /* TODO: 2nd status reg (read with 0x35) */
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L040",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID_NOPREFIX,
		.model_id	= AMIC_A25L040,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { { 4 * 1024, 128 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { { 64 * 1024, 8 } },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { { 512 * 1024, 1 } },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp2_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L05PT",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID,
		.model_id	= AMIC_A25L05PT,
		.total_size	= 64,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid4,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{32 * 1024, 1},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp1_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L05PU",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID,
		.model_id	= AMIC_A25L05PU,
		.total_size	= 64,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid4,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp1_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L080",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID_NOPREFIX,
		.model_id	= AMIC_A25L080,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { { 4 * 1024, 256 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { { 64 * 1024, 16 } },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { { 1024 * 1024, 1 } },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp2_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L10PT",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID,
		.model_id	= AMIC_A25L10PT,
		.total_size	= 128,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid4,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 1},
					{32 * 1024, 1},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp1_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L10PU",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID,
		.model_id	= AMIC_A25L10PU,
		.total_size	= 128,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid4,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
					{64 * 1024, 1},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp1_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L16PT",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID,
		.model_id	= AMIC_A25L16PT,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid4,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 31},
					{32 * 1024, 1},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2048 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2048 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp2_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L16PU",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID,
		.model_id	= AMIC_A25L16PU,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PR,
		.probe		= probe_spi_rdid4,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
					{64 * 1024, 31},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2048 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2048 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp2_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L20PT",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID,
		.model_id	= AMIC_A25L20PT,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid4,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp1_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L20PU",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID,
		.model_id	= AMIC_A25L20PU,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid4,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
					{64 * 1024, 3},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp1_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	/* The A25L40P{T,U} chips are distinguished by their
	 * erase block layouts, but without any distinction in RDID.
	 * This inexplicable quirk was verified by Rudolf Marek
	 * and discussed on the flashrom mailing list on 2010-07-12.
	 */
	{
		.vendor		= "AMIC",
		.name		= "A25L40PT",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID,
		.model_id	= AMIC_A25L40PT,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PR,
		.probe		= probe_spi_rdid4,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 7},
					{32 * 1024, 1},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp2_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L40PU",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID,
		.model_id	= AMIC_A25L40PU,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PR,
		.probe		= probe_spi_rdid4,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
					{64 * 1024, 7},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp2_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L512",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID_NOPREFIX,
		.model_id	= AMIC_A25L512,
		.total_size	= 64,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { { 4 * 1024, 16 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { { 64 * 1024, 1 } },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { { 64 * 1024, 1 } },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp2_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25L80P",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID,
		.model_id	= AMIC_A25L80P,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PRE,
		.probe		= probe_spi_rdid4,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
					{64 * 1024, 15},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp2_srwd,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A25LQ032/A25LQ32A",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID_NOPREFIX,
		.model_id	= AMIC_A25LQ032,
		.total_size	= 4096,
		.page_size	= 256,
		/* A25LQ32A supports SFDP */
		/* OTP: 64B total; read 0x4B, 0x48; write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { { 4 * 1024, 1024 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { { 64 * 1024, 64 } },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { { 64 * 1024, 64 } },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { { 4096 * 1024, 1 } },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { { 4096 * 1024, 1 } },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_amic_a25lq032,
		.unlock		= NULL, /* Two status reg bytes (read with 0x35 and 0x05) */
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "AMIC",
		.name		= "A29002B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMIC_ID_NOPREFIX,
		.model_id	= AMIC_A29002B,
		.total_size	= 256,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 3},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "AMIC",
		.name		= "A29002T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMIC_ID_NOPREFIX,
		.model_id	= AMIC_A29002T,
		.total_size	= 256,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "AMIC",
		.name		= "A29040B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= AMIC_ID_NOPREFIX,
		.model_id	= AMIC_A29040B,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "AMIC",
		.name		= "A49LF040A",
		.bustype	= BUS_LPC,
		.manufacture_id	= AMIC_ID_NOPREFIX,
		.model_id	= AMIC_A49LF040A,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* routine is wrapper to probe_jedec (pm49fl00x.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.unlock		= unlock_49fl00x,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25DF021",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25DF021,
		.total_size	= 256,
		.page_size	= 256,
		/* OTP: 128B total, 64B pre-programmed; read 0x77; write 0x9B */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 8} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25df,
		.unlock		= spi_disable_blockprotect_at25df,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600}, /* 2.3-3.6V & 2.7-3.6V models available */
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25DF041A",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25DF041A,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 16} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25df,
		.unlock		= spi_disable_blockprotect_at25df,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600}, /* 2.3-3.6V & 2.7-3.6V models available */
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25DF081",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25DF081,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 32} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25df,
		.unlock		= spi_disable_blockprotect_at25df,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1600, 2000}, /* Datasheet says range is 1.65-1.95 V */
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25DF081A",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25DF081A,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 32} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25df_sec,
		.unlock		= spi_disable_blockprotect_at25df_sec,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25DF161",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25DF161,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 64} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25df_sec,
		.unlock		= spi_disable_blockprotect_at25df_sec,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25DF321",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25DF321,
		.total_size	= 4096,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PRE,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 128} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25df,
		.unlock		= spi_disable_blockprotect_at25df,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25DF321A",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25DF321A,
		.total_size	= 4096,
		.page_size	= 256,
		/* OTP: 128B total, 64B pre-programmed; read 0x77; write 0x9B */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PROBE,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 128} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25df_sec,
		.unlock		= spi_disable_blockprotect_at25df_sec,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25DF641(A)",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25DF641,
		.total_size	= 8192,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 256} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25df_sec,
		.unlock		= spi_disable_blockprotect_at25df_sec,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25DQ161",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25DQ161,
		.total_size	= 2048,
		.page_size	= 256,
		/* OTP: 128B total, 64B pre-programmed; read 0x77; write 0x9B */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 64} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25df_sec,
		.unlock		= spi_disable_blockprotect_at25df_sec,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25F512B",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25F512B,
		.total_size	= 64,
		.page_size	= 256,
		/* OTP: 128B total, 64B pre-programmed; read 0x77; write 0x9B */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 16} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 2} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {32 * 1024, 2} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25f,
		.unlock		= spi_disable_blockprotect_at25f,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25FS010",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25FS010,
		.total_size	= 128,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 32} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {4 * 1024, 32} },
				.block_erase = spi_block_erase_d7,
			}, {
				.eraseblocks = { {32 * 1024, 4} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {32 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25fs010,
		.unlock		= spi_disable_blockprotect_at25fs010,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25FS040",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25FS040,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25fs040,
		.unlock		= spi_disable_blockprotect_at25fs040,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25SF128A",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25SF128A,
		.total_size	= 16384,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PR,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 4096} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 512} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp4_srwd,
		.unlock		= spi_disable_blockprotect_bp4_srwd,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1700, 2000},
		.wp		= &wp_w25q,
	},

	{
		.vendor		= "Atmel",
		.name		= "AT25SL128A",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT25SL128A,
		.total_size	= 16384,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 4096} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 512} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25df,
		.unlock		= spi_disable_blockprotect_at25df,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1700, 2000},
		.wp		= &wp_w25q,
	},

	{
		.vendor		= "Atmel",
		.name		= "AT26DF041",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT26DF041,
		.total_size	= 512,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.write		= NULL, /* Incompatible Page write */
		.read		= spi_chip_read,
		.voltage	= {2700, 3600}, /* 3.0-3.6V for higher speed, 2.7-3.6V normal */
	},

	{
		.vendor		= "Atmel",
		.name		= "AT26DF081A",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT26DF081A,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 32} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at26df081a,
		.unlock		= spi_disable_blockprotect_at25df,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT26DF161",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT26DF161,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 64} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at25df,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT26DF161A",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT26DF161A,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 64} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_at26df081a,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT26F004",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT26F004,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= {.probe = NT, .read = NT, .erase = NT, .write = BAD},
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 16} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.write		= NULL, /* Incompatible Page write */
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT29C010A",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT29C010A,
		.total_size	= 128,
		.page_size	= 128,
		.feature_bits	= FEATURE_LONG_RESET,
		.tested		= TEST_OK_PRE,
		.probe		= probe_jedec,
		.probe_timing	= 10000, /* 10mS, Enter=Exec */
		.block_erasers	=
		{
			{
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec,	/* FIXME */
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT29C020",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT29C020,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_LONG_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10000,			/* 10ms */
		.block_erasers	=
		{
			{
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT29C040A",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT29C040A,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_LONG_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= 10000,			/* 10 ms */
		.block_erasers	=
		{
			{
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT29C512",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT29C512,
		.total_size	= 64,
		.page_size	= 128,
		.feature_bits	= FEATURE_LONG_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10000, /* 10mS, Enter=Exec */
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT45CS1282",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT45CS1282,
		.total_size	= 16896, /* No power of two sizes */
		.page_size	= 1056, /* No power of two sizes */
		.tested		= { .probe = NT, .read = BAD, .erase = NT, .write = NT },
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.printlock	= spi_prettyprint_status_register_plain,
		.write		= NULL, /* Incompatible Page write */
		.read		= NULL, /* Incompatible read */
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT45DB011D",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT45DB011D,
		.total_size	= 128, /* Size can only be determined from status register */
		.page_size	= 256, /* Size can only be determined from status register */
		.tested		= { .probe = NT, .read = BAD, .erase = NT, .write = NT },
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT45DB021D",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT45DB021D,
		.total_size	= 256, /* Size can only be determined from status register */
		.page_size	= 256, /* Size can only be determined from status register */
		.tested		= { .probe = NT, .read = BAD, .erase = NT, .write = NT },
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT45DB041D",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT45DB041D,
		.total_size	= 512, /* Size can only be determined from status register */
		.page_size	= 256, /* Size can only be determined from status register */
		.tested		= { .probe = NT, .read = BAD, .erase = NT, .write = NT },
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
		.voltage	= {2700, 3600}, /* 2.5-3.6V & 2.7-3.6V models available */
	},

	{
		.vendor		= "Atmel",
		.name		= "AT45DB081D",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT45DB081D,
		.total_size	= 1024, /* Size can only be determined from status register */
		.page_size	= 256, /* Size can only be determined from status register */
		.tested		= { .probe = NT, .read = BAD, .erase = NT, .write = NT },
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
		.voltage	= {2700, 3600}, /* 2.5-3.6V & 2.7-3.6V models available */
	},

	{
		.vendor		= "Atmel",
		.name		= "AT45DB161D",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT45DB161D,
		.total_size	= 2048, /* Size can only be determined from status register */
		.page_size	= 512, /* Size can only be determined from status register */
		.tested		= { .probe = NT, .read = BAD, .erase = NT, .write = NT },
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
		.voltage	= {2700, 3600}, /* 2.5-3.6V & 2.7-3.6V models available */
	},

	{
		.vendor		= "Atmel",
		.name		= "AT45DB321C",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT45DB321C,
		.total_size	= 4224, /* No power of two sizes */
		.page_size	= 528, /* No power of two sizes */
		.tested		= { .probe = NT, .read = BAD, .erase = NT, .write = NT },
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL, /* Incompatible read */
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT45DB321D",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT45DB321D,
		.total_size	= 4096, /* Size can only be determined from status register */
		.page_size	= 512, /* Size can only be determined from status register */
		/* OTP: 128B total, 64B pre-programmed; read 0x77; write 0x9B */
		.feature_bits	= FEATURE_OTP,
		.tested		= { .probe = NT, .read = BAD, .erase = NT, .write = NT },
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT45DB642D",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT45DB642D,
		.total_size	= 8192, /* Size can only be determined from status register */
		.page_size	= 1024, /* Size can only be determined from status register */
		.tested		= { .probe = NT, .read = BAD, .erase = NT, .write = NT },
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT49BV512",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT49BV512,
		.total_size	= 64,
		.page_size	= 64,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT49F002(N)",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT49F002N,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{96 * 1024, 1},
					{128 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT49F002(N)T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT49F002NT,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PR,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{128 * 1024, 1},
					{96 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT49F020",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT49F020,
		.total_size	= 256,
		.page_size	= 0, /* unused */
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PRE,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
			/* Chip features an optional permanent write protection
			 * of the first 8 kB. The erase function is the same as
			 * above, but 00000H to 01FFFH will not be erased.
			 * FIXME: add another eraser when partial erasers are
			 * supported.
			 */
		},
		.printlock	= printlock_at49f,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Atmel",
		.name		= "AT49LH002",
		.bustype	= BUS_LPC | BUS_FWH, /* A/A Mux */
		.manufacture_id	= ATMEL_ID,
		.model_id	= ATMEL_AT49LH002,
		.total_size	= 256,
		.page_size	= 0, /* unused */
		.feature_bits	= FEATURE_REGISTERMAP, /* TODO: LPC OK too? */
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab, /* TODO: 0xff cmd not documented? */
		.probe_timing	= TIMING_FIXME,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_block_82802ab,
			}, {
				.eraseblocks = {
					{64 * 1024, 4},
				},
				.block_erase = NULL, /* TODO: Implement. */
			},
		},
		.printlock	= NULL, /* TODO */
		.unlock		= NULL, /* unlock_82802ab() not correct(?) */
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Bright",
		.name		= "BM29F040",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= BRIGHT_ID,
		.model_id	= BRIGHT_BM29F040,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PR,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Catalyst",
		.name		= "CAT28F512",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= CATALYST_ID,
		.model_id	= CATALYST_CAT28F512,
		.total_size	= 64,
		.page_size	= 0, /* unused */
		.feature_bits	= 0,
		.tested		= {.probe = OK, .read = OK, .erase = BAD, .write = BAD},
		.probe		= probe_jedec, /* FIXME! */
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = NULL, /* TODO */
			},
		},
		.write		= NULL, /* TODO */
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "ENE",
		.name		= "KB9012 (EDI)",
		.bustype	= BUS_SPI,
		.total_size	= 128,
		.page_size	= 128,
		.feature_bits	= FEATURE_ERASE_TO_ZERO,
		.tested		= TEST_OK_PREW,
		.spi_cmd_set	= SPI_EDI,
		.probe		= edi_probe_kb9012,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {128, 1024} },
				.block_erase = edi_chip_block_erase,
			},
		},
		.write		= edi_chip_write,
		.read		= edi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "ESMT",
		.name		= "F25L008A",
		.bustype	= BUS_SPI,
		.manufacture_id	= ESMT_ID,
		.model_id	= ESMT_F25L008A,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_EITHER,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_1,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "ESMT",
		.name		= "F49B002UA",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ESMT_ID,
		.model_id	= ESMT_F49B002UA,
		.total_size	= 256,
		.page_size	= 4096,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{128 * 1024, 1},
					{96 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B05",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B05,
		.total_size	= 64,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B05T",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B05,
		.total_size	= 64,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{32 * 1024, 1},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B10",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B10,
		.total_size	= 128,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 3},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B10T",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B10,
		.total_size	= 128,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{32 * 1024, 3},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B16",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B16,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
					{64 * 1024, 31},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B16T",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B16,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 31},
					{32 * 1024, 1},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B20",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B20,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
					{64 * 1024, 3}
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B20T",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B20,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B32",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B32,
		.total_size	= 4096,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
					{64 * 1024, 63},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B32T",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B32,
		.total_size	= 4096,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 63},
					{32 * 1024, 1},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B40",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B40,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
					{64 * 1024, 7}
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B40T",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B40,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 7},
					{32 * 1024, 1},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B64",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B64,
		.total_size	= 8192,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
					{64 * 1024, 127},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B64T",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B64,
		.total_size	= 8192,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 127},
					{32 * 1024, 1},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B80",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B80,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 2},
					{8 * 1024, 1},
					{16 * 1024, 1},
					{32 * 1024, 1},
					{64 * 1024, 15}
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25B80T",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25B80,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 15},
					{32 * 1024, 1},
					{16 * 1024, 1},
					{8 * 1024, 1},
					{4 * 1024, 2},
				},
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		/* Note: EN25Q16 is an evil twin which shares the model ID
		   but has different write protection capabilities */
		.vendor		= "Eon",
		.name		= "EN25D16",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25D16,
		.total_size	= 2048,
		.page_size	= 256,
		/* OTP: D16 512B/Q16 128B total; enter 0x3A */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25F05",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25F05,
		.total_size	= 64,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 16} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 2} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {32 * 1024, 2} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25F10",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25F10,
		.total_size	= 128,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 32} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {32 * 1024, 4} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25F16",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25F16,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25F20",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25F20,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {64 * 1024, 4} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25F32",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25F32,
		.total_size	= 4096,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25F40",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25F40,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Eon",
		.name		= "EN25F80",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25F80,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25Q128",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25Q128,
		.total_size	= 16384,
		.page_size	= 256,
		/* OTP: 512B total; enter 0x3A */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 4096} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Eon",
		.name		= "EN25Q32(A/B)",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25Q32,
		.total_size	= 4096,
		.page_size	= 256,
		/* OTP: 512B total; enter 0x3A */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Eon",
		.name		= "EN25Q40",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25Q40,
		.total_size	= 512,
		.page_size	= 256,
		/* OTP: 256B total; enter 0x3A */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Eon",
		.name		= "EN25Q64",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25Q64,
		.total_size	= 8192,
		.page_size	= 256,
		/* OTP: 512B total; enter 0x3A */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Eon",
		.name		= "EN25Q80(A)",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25Q80,
		.total_size	= 1024,
		.page_size	= 256,
		/* OTP: 256B total; enter 0x3A */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Eon",
		.name		= "EN25QH16",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25QH16,
		.total_size	= 2048,
		.page_size	= 256,
		/* supports SFDP */
		/* OTP: 512B total; enter 0x3A */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 2048, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {1024 * 2048, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Eon",
		.name		= "EN25S64",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= EON_EN25S64,
		.total_size	= 8192,
		.page_size	= 256,
		/* FIXME: add FEATURE_QPI removed */
		/* OTP: 512B total; enter 0x3A */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8192 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8192 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1650, 1950},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Eon",
		.name		= "EN29F002(A)(N)B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= EON_ID,
		.model_id	= EON_EN29F002B,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_ADDR_AAA | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 3},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Eon",
		.name		= "EN29F002(A)(N)T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= EON_ID,
		.model_id	= EON_EN29F002T,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_ADDR_AAA | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Eon",
		.name		= "EN29F010",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= EON_ID,
		.model_id	= EON_EN29F010,
		.total_size	= 128,
		.page_size	= 128,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PRE,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {16 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			},
			{
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Fujitsu",
		.name		= "MBM29F004BC",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= FUJITSU_ID,
		.model_id	= FUJITSU_MBM29F004BC,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 7},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= NULL,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Fujitsu",
		.name		= "MBM29F004TC",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= FUJITSU_ID,
		.model_id	= FUJITSU_MBM29F004TC,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 7},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= NULL,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		/* FIXME: this has WORD/BYTE sequences; 2AA for word, 555 for byte */
		.vendor		= "Fujitsu",
		.name		= "MBM29F400BC",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= FUJITSU_ID,
		.model_id	= FUJITSU_MBM29F400BC,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_SHIFTED | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_m29f400bt,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (m29f400bt.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 7},
				},
				.block_erase = block_erase_m29f400bt,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = block_erase_chip_m29f400bt,
			},
		},
		.write		= write_m29f400bt,
		.read		= read_memmapped,
		.voltage	= {4750, 5250}, /* 4.75-5.25V for type -55, others 4.5-5.5V */
	},

	{
		.vendor		= "Fujitsu",
		.name		= "MBM29F400TC",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= FUJITSU_ID,
		.model_id	= FUJITSU_MBM29F400TC,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_SHIFTED | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_m29f400bt,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (m29f400bt.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 7},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = block_erase_m29f400bt,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = block_erase_chip_m29f400bt,
			},
		},
		.write		= write_m29f400bt,
		.read		= read_memmapped,
		.voltage	= {4750, 5250}, /* 4.75-5.25V for type -55, others 4.5-5.5V */
	},

	{
		.vendor		= "GigaDevice",
		.name		= "GD25LQ128C/GD25LQ128D",
		.bustype	= BUS_SPI,
		.manufacture_id	= GIGADEVICE_ID,
		.model_id	= GIGADEVICE_GD25LQ128CD,
		.total_size	= 16384,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 4096} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 512} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1695, 1950},
		.wp		= &wp_generic,
	},

	{
		.vendor		= "GigaDevice",
		.name		= "GD25LQ32",
		.bustype	= BUS_SPI,
		.manufacture_id	= GIGADEVICE_ID,
		.model_id	= GIGADEVICE_GD25LQ32,
		.total_size	= 4096,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 128} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1695, 1950},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "GigaDevice",
		.name		= "GD25LQ40",
		.bustype	= BUS_SPI,
		.manufacture_id	= GIGADEVICE_ID,
		.model_id	= GIGADEVICE_GD25LQ40,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 16} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1695, 1950},
	},

	{
		.vendor		= "GigaDevice",
		.name		= "GD25LQ64(B)",
		.bustype	= BUS_SPI,
		.manufacture_id	= GIGADEVICE_ID,
		.model_id	= GIGADEVICE_GD25LQ64,
		.total_size	= 8192,
		.page_size	= 256,
		/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42, erase 0x44 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 256} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_bp4_srwd,
		.unlock		= spi_disable_blockprotect_bp4_srwd, /* TODO: 2nd status reg (read with 0x35) */
		.write		= spi_chip_write_256,
		.read		= spi_chip_read, /* Fast read (0x0B) and multi I/O supported */
		.voltage	= {1695, 1950},
		.wp		= &wp_w25,
	},

	{
		/*
		 * TODO: There is a GD25Q128B without QPI support which can
		 * be differentiated using SFDP, but for now we only care
		 * about the GD25Q128C.
		 */
		.vendor		= "GigaDevice",
		.name		= "GD25Q127C/GD25Q128C",
		.bustype	= BUS_SPI,
		.manufacture_id	= GIGADEVICE_ID,
		.model_id	= GIGADEVICE_GD25Q128,
		.total_size	= 16384,
		.page_size	= 256,
		/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 4096} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 512} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "GigaDevice",
		.name		= "GD25Q16(B)",
		.bustype	= BUS_SPI,
		.manufacture_id	= GIGADEVICE_ID,
		.model_id	= GIGADEVICE_GD25Q16,
		.total_size	= 2048,
		.page_size	= 256,
		/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 64} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "GigaDevice",
		.name		= "GD25Q20(B)",
		.bustype	= BUS_SPI,
		.manufacture_id	= GIGADEVICE_ID,
		.model_id	= GIGADEVICE_GD25Q20,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 8} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "GigaDevice",
		.name		= "GD25Q256D",
		.bustype	= BUS_SPI,
		.manufacture_id	= GIGADEVICE_ID,
		.model_id	= GIGADEVICE_GD25Q256D,
		.total_size	= 32768,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP | FEATURE_4BA_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 8192} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 1024} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 512} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {32 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {32 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.set_4ba	= spi_enter_4ba_b7_we,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25q_large,
	},

	{
		.vendor		= "GigaDevice",
		.name		= "GD25Q32(B)",
		.bustype	= BUS_SPI,
		.manufacture_id	= GIGADEVICE_ID,
		.model_id	= GIGADEVICE_GD25Q32,
		.total_size	= 4096,
		.page_size	= 256,
		/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 128} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_generic,
	},

	{
		.vendor		= "GigaDevice",
		.name		= "GD25Q40(B)",
		.bustype	= BUS_SPI,
		.manufacture_id	= GIGADEVICE_ID,
		.model_id	= GIGADEVICE_GD25Q40,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 16} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "GigaDevice",
		.name		= "GD25Q64(B)",
		.bustype	= BUS_SPI,
		.manufacture_id	= GIGADEVICE_ID,
		.model_id	= GIGADEVICE_GD25Q64,
		.total_size	= 8192,
		.page_size	= 256,
		/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 256} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "GigaDevice",
		.name		= "GD25Q80(B)",
		.bustype	= BUS_SPI,
		.manufacture_id	= GIGADEVICE_ID,
		.model_id	= GIGADEVICE_GD25Q80,
		.total_size	= 1024,
		.page_size	= 256,
		/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 32} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Hyundai",
		.name		= "HY29F002B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= HYUNDAI_ID,
		.model_id	= HYUNDAI_HY29F002B,
		.total_size	= 256,
		.page_size	= 256 * 1024,
		.feature_bits	= FEATURE_EITHER_RESET, /* Some revisions may need FEATURE_ADDR_2AA */
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO, /* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 3},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4750, 5250}, /* 4.75-5.25V for type -45, others 4.5-5.5V */
	},

	{
		.vendor		= "Hyundai",
		.name		= "HY29F002T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= HYUNDAI_ID,
		.model_id	= HYUNDAI_HY29F002T,
		.total_size	= 256,
		.page_size	= 256 * 1024,
		.feature_bits	= FEATURE_EITHER_RESET, /* Some revisions may need FEATURE_ADDR_2AA */
		.tested		= TEST_OK_PRE,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO, /* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4750, 5250}, /* 4.75-5.25V for type -45, others 4.5-5.5V */
	},

	{
		.vendor		= "Hyundai",
		.name		= "HY29F040A",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= HYUNDAI_ID,
		.model_id	= HYUNDAI_HY29F040A,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Intel",
		.name		= "28F001BN/BX-B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= INTEL_ID,
		.model_id	= INTEL_28F001B,
		.total_size	= 128,
		.page_size	= 128 * 1024, /* 8k + 2x4k + 112k */
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{8 * 1024, 1},
					{4 * 1024, 2},
					{112 * 1024, 1},
				},
				.block_erase = erase_block_82802ab,
			},
		},
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Intel",
		.name		= "28F001BN/BX-T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= INTEL_ID,
		.model_id	= INTEL_28F001T,
		.total_size	= 128,
		.page_size	= 128 * 1024, /* 112k + 2x4k + 8k */
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{112 * 1024, 1},
					{4 * 1024, 2},
					{8 * 1024, 1},
				},
				.block_erase = erase_block_82802ab,
			},
		},
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Intel",
		.name		= "28F002BC/BL/BV/BX-T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= INTEL_ID,
		.model_id	= INTEL_28F002T,
		.total_size	= 256,
		.page_size	= 256 * 1024,
		.tested		= TEST_OK_PRE,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_ZERO, /* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{128 * 1024, 1},
					{96 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_block_82802ab,
			},
		},
		.write		= write_82802ab,
		.read		= read_memmapped,
	},

	{
		.vendor		= "Intel",
		.name		= "28F004B5/BE/BV/BX-B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= INTEL_ID,
		.model_id	= INTEL_28F004B,
		.total_size	= 512,
		.page_size	= 128 * 1024, /* maximal block size */
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{96 * 1024, 1},
					{128 * 1024, 3},
				},
				.block_erase = erase_block_82802ab,
			},
		},
		.write		= write_82802ab,
		.read		= read_memmapped,
	},

	{
		.vendor		= "Intel",
		.name		= "28F004B5/BE/BV/BX-T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= INTEL_ID,
		.model_id	= INTEL_28F004T,
		.total_size	= 512,
		.page_size	= 128 * 1024, /* maximal block size */
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{128 * 1024, 3},
					{96 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_block_82802ab,
			},
		},
		.write		= write_82802ab,
		.read		= read_memmapped,
	},

	{
		.vendor		= "Intel",
		.name		= "28F008S3/S5/SC",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= INTEL_ID,
		.model_id	= INTEL_28F004S3,
		.total_size	= 512,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_block_82802ab,
			},
		},
		.unlock		= unlock_28f004s5,
		.write		= write_82802ab,
		.read		= read_memmapped,
	},

	{
		.vendor		= "Intel",
		.name		= "28F400BV/BX/CE/CV-B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= INTEL_ID,
		.model_id	= INTEL_28F400B,
		.total_size	= 512,
		.page_size	= 128 * 1024, /* maximal block size */
		.feature_bits	= FEATURE_ADDR_SHIFTED,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{96 * 1024, 1},
					{128 * 1024, 3},
				},
				.block_erase = erase_block_82802ab,
			},
		},
		.write		= write_82802ab,
		.read		= read_memmapped,
	},

	{
		.vendor		= "Intel",
		.name		= "28F400BV/BX/CE/CV-T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= INTEL_ID,
		.model_id	= INTEL_28F400T,
		.total_size	= 512,
		.page_size	= 128 * 1024, /* maximal block size */
		.feature_bits	= FEATURE_ADDR_SHIFTED,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{128 * 1024, 3},
					{96 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_block_82802ab,
			},
		},
		.write		= write_82802ab,
		.read		= read_memmapped,
	},

	{
		.vendor		= "Intel",
		.name		= "82802AB",
		.bustype	= BUS_FWH,
		.manufacture_id	= INTEL_ID,
		.model_id	= INTEL_82802AB,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_IGNORED, /* routine does not use probe_timing (82802ab.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_block_82802ab,
			},
		},
		.unlock		= unlock_82802ab,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Intel",
		.name		= "82802AC",
		.bustype	= BUS_FWH,
		.manufacture_id	= INTEL_ID,
		.model_id	= INTEL_82802AC,
		.total_size	= 1024,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_OK_PR,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_IGNORED, /* routine does not use probe_timing (82802ab.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = erase_block_82802ab,
			},
		},
		.unlock		= unlock_82802ab,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L1005(C)/MX25L1006E",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L1005,
		.total_size	= 128,
		.page_size	= 256,
		/* MX25L1006E supports SFDP */
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 32} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 2} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read, /* Fast read (0x0B) supported, MX25L1006E supports dual I/O */
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L12805D",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L12805,
		.total_size	= 16384,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 4096} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L1605",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L1605,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,	/* This erase function has 64k blocksize for eLiteFlash */
			}, {
				.eraseblocks = { {64 * 1024, 32} },	/* Not supported in MX25L1605 (eLiteFlash) and MX25L1605D */
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L1635D",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L1635D,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L1635E",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L1635E,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L2005(C)/MX25L2006E",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L2005,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 4} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read, /* Fast read (0x0B) supported */
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L25635F/MX25L25645G",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L25635F,
		.total_size	= 32768,
		.page_size	= 256,
		.feature_bits	=
			FEATURE_WRSR_WREN | FEATURE_4BA_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 8192} },
				.block_erase = spi_block_erase_21,
			}, {
				.eraseblocks = { {32 * 1024, 1024} },
				.block_erase = spi_block_erase_5c,
			}, {
				.eraseblocks = { {64 * 1024, 512} },
				.block_erase = spi_block_erase_dc,
			}, {
				.eraseblocks = { {32768 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {32768 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.set_4ba	= spi_enter_4ba_b7,
		.voltage	= {2700, 3600},
		.wp		= &wp_generic,
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L3205(A)",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L3205,
		.total_size	= 4096,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L3235D",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L3235D,
		.total_size	= 4096,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L4005(A/C)/MX25L4006E",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L4005,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read, /* Fast read (0x0B) supported */
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L512(E)/MX25V512(C)",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L512,
		.total_size	= 64,
		.page_size	= 256,
		/* MX25L512E supports SFDP */
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 16} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read, /* Fast read (0x0B) supported, MX25L512E supports dual I/O */
		.voltage	= {2700, 3600}, /* 2.35-3.6V for MX25V512(C) */
	},

	{
		/* FIXME: This is an evil twin of the MX25L6405, with the same
		 * ID bytes but different block erase capabilities */
		.vendor		= "Macronix",
		.name		= "MX25L6406E/MX25L6408E",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L6405,
		.total_size	= 8192,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read, /* Fast read (0x0B) supported */
		.voltage	= {2700, 3600}, /* 2.35-3.6V for MX25V8005 */
		.wp		= &wp_generic,
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L6495F",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L6495F,
		.total_size	= 8192,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {32 * 1024, 256} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read, /* Fast read (0x0B) supported */
		.voltage	= {2700, 3600},
		.wp		= &wp_generic,
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25L8005/MX25L8006E/MX25L8008E/MX25V8005",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25L8005,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25U12835F",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25U12835E,
		.total_size	= 16384,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 4096} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 512} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 2048, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8 * 1024 * 2048, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read, /* Fast read (0x0B) supported */
		.voltage	= {1650, 2000},
		.wp		= &wp_w25q_large,
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25U25635F",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25U25635F,
		/* FIXME(dhendrix): support 32-bit addressing */
		.total_size	= 32768/2,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			/* FIXME(dhendrix): support 32-bit addressing */
			{
				.eraseblocks = { {4 * 1024, 8192/2} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 1024/2} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 512/2} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {32/2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {32/2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read, /* Fast read (0x0B) supported */
		.voltage	= {1650, 2000},
		/* FIXME(dhendrix): write-protect support */
		/* .wp		= &wp_generic, */
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25U3235E/F",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25U3235E,
		.total_size	= 4096,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 128} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read, /* Fast read (0x0B) supported */
		.voltage	= {1650, 2000},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Macronix",
		.name		= "MX25U6435E/F",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX25U6435E,
		.total_size	= 8192,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 256} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read, /* Fast read (0x0B) supported */
		.voltage	= {1650, 2000},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Macronix",
		.name		= "MX29F001B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX29F001B,
		.total_size	= 128,
		.page_size	= 32 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{8 * 1024, 1},
					{4 * 1024, 2},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Macronix",
		.name		= "MX29F001T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX29F001T,
		.total_size	= 128,
		.page_size	= 32 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 1},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{4 * 1024, 2},
					{8 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Macronix",
		.name		= "MX29F002(N)B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX29F002B,
		.total_size	= 256,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 3},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Macronix",
		.name		= "MX29F002(N)T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX29F002T,
		.total_size	= 256,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Macronix",
		.name		= "MX29F040",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX29F040,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Macronix",
		.name		= "MX29LV040",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= MACRONIX_MX29LV040,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_SHORT_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	/* The ST M25P05 is a bit of a problem. It has the same ID as the
	 * ST M25P05-A in RES mode, but supports only 128 byte writes instead
	 * of 256 byte writes. We rely heavily on the fact that probe_spi_res1
	 * only is successful if RDID does not work.
	 */
	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25P05",
		.bustype	= BUS_SPI,
		.manufacture_id	= 0, /* Not used. */
		.model_id	= ST_M25P05_RES,
		.total_size	= 64,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_res1,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {32 * 1024, 2} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_1, /* 128 */
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25P05-A",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25P05A,
		.total_size	= 64,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {32 * 1024, 2} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	/* The ST M25P10 has the same problem as the M25P05. */
	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25P10",
		.bustype	= BUS_SPI,
		.manufacture_id	= 0, /* Not used. */
		.model_id	= ST_M25P10_RES,
		.total_size	= 128,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_res1,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {32 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_1, /* 128 */
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25P10-A",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25P10A,
		.total_size	= 128,
		.page_size	= 256,
		.tested		= TEST_OK_PRE,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {32 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25P128",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25P128,
		.total_size	= 16384,
		.page_size	= 256,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {256 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25P16",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25P16,
		.total_size	= 2048,
		.page_size	= 256,
		.tested		= TEST_OK_PR,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST", /* Numonyx */
		.name		= "M25P20",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25P20,
		.total_size	= 256,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read, /* Fast read (0x0B) supported */
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25P32",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25P32,
		.total_size	= 4096,
		.page_size	= 256,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST", /* Numonyx */
		.name		= "M25P40",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25P40,
		.total_size	= 512,
		.page_size	= 256,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25P40-old",
		.bustype	= BUS_SPI,
		.manufacture_id	= 0, /* Not used. */
		.model_id	= ST_M25P40_RES,
		.total_size	= 512,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_res1,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25P64",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25P64,
		.total_size	= 8192,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25P80",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25P80,
		.total_size	= 1024,
		.page_size	= 256,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25PE10",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25PE10,
		.total_size	= 128,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 32} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 2} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25PE16",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25PE16,
		.total_size	= 2048,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25PE20",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25PE20,
		.total_size	= 256,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25PE40",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25PE40,
		.total_size	= 512,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25PE80",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25PE80,
		.total_size	= 1024,
		.page_size	= 256,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25PX16",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25PX16,
		.total_size	= 2048,
		.page_size	= 256,
		/* OTP: 64B total; read 0x4B; write 0x42 */
		.feature_bits	= FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { { 4 * 1024, 512 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2300, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25PX32",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25PX32,
		.total_size	= 4096,
		.page_size	= 256,
		.tested		= TEST_OK_PRE,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { { 4 * 1024, 1024 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "M25PX64",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M25PX64,
		.total_size	= 8192,
		.page_size	= 256,
		.tested		= TEST_OK_PRE,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { { 4 * 1024, 2048 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "MT25QL256", /* ..3E = 3V, uniform 64KB/4KB blocks/sectors */
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_N25Q256__3E,
		.total_size	= 32768,
		.page_size	= 256,
		/* supports SFDP */
		/* OTP: 64B total; read 0x4B, write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP | FEATURE_4BA_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 8192} },
				.block_erase = spi_block_erase_21,
			}, {
				.eraseblocks = { {64 * 1024, 512} },
				.block_erase = spi_block_erase_dc,
			}, {
				.eraseblocks = { {32768 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect, /* TODO: per 64kB sector lock registers */
		.write		= spi_chip_write_256, /* Multi I/O supported */
		.read		= spi_chip_read, /* Fast read (0x0B) and multi I/O supported */
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		.name		= "MT25QL512", /* ..3E = 3V, uniform 64KB/4KB blocks/sectors */
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_N25Q512__3E,
		.total_size	= 65536,
		.page_size	= 256,
		/* supports SFDP */
		/* OTP: 64B total; read 0x4B, write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP | FEATURE_4BA_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 16384} },
				.block_erase = spi_block_erase_21,
			}, {
				.eraseblocks = { {4 * 1024, 16384} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 1024} },
				.block_erase = spi_block_erase_dc,
			}, {
				.eraseblocks = { {64 * 1024, 1024} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {65536 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_n25q, /* TODO: config, lock, flag regs */
		.unlock		= spi_disable_blockprotect_n25q, /* TODO: per 64kB sector lock registers */
		.write		= spi_chip_write_256, /* Multi I/O supported */
		.read		= spi_chip_read, /* Fast read (0x0B) and multi I/O supported */
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		/* ..1E = 1.8V, uniform 64KB/4KB blocks/sectors */
		.name		= "N25Q064..1E",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_N25Q064__1E,
		.total_size	= 8192,
		.page_size	= 256,
		/* supports SFDP */
		/* OTP: 64B total; read 0x4B, write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1700, 2000},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Micron/Numonyx/ST",
		/* ..3E = 3V, uniform 64KB/4KB blocks/sectors */
		.name		= "N25Q064..3E",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= ST_N25Q064__3E,
		.total_size	= 8192,
		.page_size	= 256,
		/* supports SFDP */
		/* OTP: 64B total; read 0x4B, write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048 } },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "MoselVitelic",
		.name		= "V29C51000B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= MVC_V29C51000B,
		.total_size	= 64,
		.page_size	= 512,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {512, 128} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "MoselVitelic",
		.name		= "V29C51000T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= MVC_V29C51000T,
		.total_size	= 64,
		.page_size	= 512,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {512, 128} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "MoselVitelic",
		.name		= "V29C51400B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= MVC_V29C51400B,
		.total_size	= 512,
		.page_size	= 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {1024, 512} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "MoselVitelic",
		.name		= "V29C51400T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= MVC_V29C51400T,
		.total_size	= 512,
		.page_size	= 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {1024, 512} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "MoselVitelic",
		.name		= "V29LC51000",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= MVC_V29LC51000,
		.total_size	= 64,
		.page_size	= 512,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {512, 128} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "MoselVitelic",
		.name		= "V29LC51001",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= MVC_V29LC51001,
		.total_size	= 128,
		.page_size	= 512,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {512, 256} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "MoselVitelic",
		.name		= "V29LC51002",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= MVC_V29LC51002,
		.total_size	= 256,
		.page_size	= 512,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {512, 512} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm25LV010",
		.bustype	= BUS_SPI,
		.manufacture_id	= PMC_ID,
		.model_id	= PMC_PM25LV010,
		.total_size	= 128,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 32} },
				.block_erase = spi_block_erase_d7,
			}, {
				.eraseblocks = { {32 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm25LV016B",
		.bustype	= BUS_SPI,
		.manufacture_id	= PMC_ID,
		.model_id	= PMC_PM25LV016B,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_d7,
			}, {
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm25LV020",
		.bustype	= BUS_SPI,
		.manufacture_id	= PMC_ID,
		.model_id	= PMC_PM25LV020,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = spi_block_erase_d7,
			}, {
				.eraseblocks = { {64 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm25LV040",
		.bustype	= BUS_SPI,
		.manufacture_id	= PMC_ID,
		.model_id	= PMC_PM25LV040,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_d7,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm25LV080B",
		.bustype	= BUS_SPI,
		.manufacture_id	= PMC_ID,
		.model_id	= PMC_PM25LV080B,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_d7,
			}, {
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm25LV512(A)",
		.bustype	= BUS_SPI,
		.manufacture_id	= PMC_ID,
		.model_id	= PMC_PM25LV512,
		.total_size	= 64,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 16} },
				.block_erase = spi_block_erase_d7,
			}, {
				.eraseblocks = { {32 * 1024, 2} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm29F002B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= PMC_ID_NOPREFIX,
		.model_id	= PMC_PM29F002B,
		.total_size	= 256,
		.page_size	= 8 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_FIXME,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{96 * 1024, 1},
					{128 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm29F002T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= PMC_ID_NOPREFIX,
		.model_id	= PMC_PM29F002T,
		.total_size	= 256,
		.page_size	= 8 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_FIXME,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{128 * 1024, 1},
					{96 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm39LV010",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= PMC_ID_NOPREFIX,
		.model_id	= PMC_PM39F010,	/* Pm39LV010 and Pm39F010 have identical IDs but different voltage */
		.total_size	= 128,
		.page_size	= 4096,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 32} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 2} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm39LV020",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= PMC_ID_NOPREFIX,
		.model_id	= PMC_PM39LV020,
		.total_size	= 256,
		.page_size	= 4096,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 4} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm39LV040",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= PMC_ID_NOPREFIX,
		.model_id	= PMC_PM39LV040,
		.total_size	= 512,
		.page_size	= 4096,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PR,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm49FL002",
		.bustype	= BUS_LPC | BUS_FWH, /* A/A Mux */
		.manufacture_id	= PMC_ID_NOPREFIX,
		.model_id	= PMC_PM49FL002,
		.total_size	= 256,
		.page_size	= 16 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PRE,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* routine is wrapper to probe_jedec (pm49fl00x.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {16 * 1024, 16} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.unlock		= unlock_49fl00x,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "PMC",
		.name		= "Pm49FL004",
		.bustype	= BUS_LPC | BUS_FWH, /* A/A Mux */
		.manufacture_id	= PMC_ID_NOPREFIX,
		.model_id	= PMC_PM49FL004,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PRE,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* routine is wrapper to probe_jedec (pm49fl00x.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.unlock		= unlock_49fl00x,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST25LF040A",
		.bustype	= BUS_SPI,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST25VF040_REMS,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_EWSR,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_res2,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 16} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_1, /* AAI supported, but opcode is 0xAF */
		.read		= spi_chip_read,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST25LF080A",
		.bustype	= BUS_SPI,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST25VF080_REMS,
		.total_size	= 1024,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_res2,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 32} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_1, /* AAI supported, but opcode is 0xAF */
		.read		= spi_chip_read,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST25VF010",
		.bustype	= BUS_SPI,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST25VF010_REMS,
		.total_size	= 128,
		.page_size	= 256,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rems,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 32} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 4} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_aai_write,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST25VF010(A)",
		.bustype	= BUS_SPI,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST25VF010_REMS,
		.total_size	= 128,
		.page_size	= 256,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rems,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 32} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 4} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_1,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST25VF016B",
		.bustype	= BUS_SPI,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST25VF016B,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_EITHER,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 64} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_1, /* AAI supported, but opcode is 0xAF */
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST25VF032B",
		.bustype	= BUS_SPI,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST25VF032B,
		.total_size	= 4096,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_EWSR,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 128} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_aai_write,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST25VF040",
		.bustype	= BUS_SPI,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST25VF040_REMS,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_EWSR,
		.tested		= TEST_OK_PR,
		.probe		= probe_spi_rems,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 16} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_aai_write,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST25VF040B",
		.bustype	= BUS_SPI,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST25VF040B,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_EWSR,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 16} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_aai_write,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST25VF040B.REMS",
		.bustype	= BUS_SPI,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST25VF040B_REMS,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_EWSR,
		.tested		= TEST_OK_PR,
		.probe		= probe_spi_rems,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 16} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_aai_write,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST25VF064C",
		.bustype	= BUS_SPI,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST25VF064C,
		.total_size	= 8192,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_EWSR,
		.tested		= TEST_OK_PRE,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 256} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST25VF080B",
		.bustype	= BUS_SPI,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST25VF080B,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_EWSR,
		.tested		= TEST_OK_PRE,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 32} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_1,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST28SF040A",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST28SF040,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= 0,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (sst28sf040.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {128, 4096} },
				.block_erase = erase_sector_28sf040,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_28sf040,
			}
		},
		.unlock		= unprotect_28sf040,
		.write		= write_28sf040,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "SST",
		.name		= "SST29EE010",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST29EE010,
		.total_size	= 128,
		.page_size	= 128,
		.feature_bits	= FEATURE_LONG_RESET,
		.tested		= TEST_OK_PR,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "SST",
		.name		= "SST29EE020A",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST29EE020A,
		.total_size	= 256,
		.page_size	= 128,
		.feature_bits	= FEATURE_LONG_RESET,
		.tested		= TEST_OK_PRE,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "SST",
		.name		= "SST29LE010",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST29LE010,
		.total_size	= 128,
		.page_size	= 128,
		.feature_bits	= FEATURE_LONG_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST29LE020",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST29LE020,
		.total_size	= 256,
		.page_size	= 128,
		.feature_bits	= FEATURE_LONG_RESET,
		.tested		= TEST_OK_PRE,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST39SF010A",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST39SF010,
		.total_size	= 128,
		.page_size	= 4096,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,			/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 32} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "SST",
		.name		= "SST39SF020A",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST39SF020,
		.total_size	= 256,
		.page_size	= 4096,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,			/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "SST",
		.name		= "SST39SF040",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST39SF040,
		.total_size	= 512,
		.page_size	= 4096,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,			/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "SST",
		.name		= "SST39SF512",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST39SF512,
		.total_size	= 64,
		.page_size	= 4096,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,			/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 16} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "SST",
		.name		= "SST39VF010",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST39VF010,
		.total_size	= 128,
		.page_size	= 4096,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,			/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 32} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST39VF020",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST39VF020,
		.total_size	= 256,
		.page_size	= 4096,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,			/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST39VF040",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST39VF040,
		.total_size	= 512,
		.page_size	= 4096,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,			/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST39VF080",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST39VF080,
		.total_size	= 1024,
		.page_size	= 4096,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= 1,			/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST39VF512",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST39VF512,
		.total_size	= 64,
		.page_size	= 4096,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,			/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 16} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST49LF002A/B",
		.bustype	= BUS_FWH, /* A/A Mux */
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF002A,
		.total_size	= 256,
		.page_size	= 16 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,		/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {16 * 1024, 16} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = NULL, /* AA 55 80 AA 55 10, only in A/A mux mode */
			}
		},
		.printlock	= printlock_sst_fwhub,
		.unlock		= unlock_sst_fwhub,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST49LF003A/B",
		.bustype	= BUS_FWH, /* A/A Mux */
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF003A,
		.total_size	= 384,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,		/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 96} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 6} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {384 * 1024, 1} },
				.block_erase = NULL, /* AA 55 80 AA 55 10, only in A/A mux mode */
			}
		},
		.printlock	= printlock_sst_fwhub,
		.unlock		= unlock_sst_fwhub,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		/* Contrary to the data sheet, TBL# on the SST49LF004B affects the top 128kB (instead of 64kB)
		 * and is only honored for 64k block erase, but not 4k sector erase.
		 */
		.vendor		= "SST",
		.name		= "SST49LF004A/B",
		.bustype	= BUS_FWH, /* A/A Mux */
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF004A,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,		/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = NULL, /* AA 55 80 AA 55 10, only in A/A mux mode */
			},
		},
		.printlock	= printlock_sst_fwhub,
		.unlock		= unlock_sst_fwhub,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST49LF004C",
		.bustype	= BUS_FWH,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF004C,
		.total_size	= 512,
		.page_size	= 4 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (sst49lfxxxc.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = erase_sector_49lfxxxc,
			}, {
				.eraseblocks = {
					{64 * 1024, 7},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_49lfxxxc,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST49LF008A",
		.bustype	= BUS_FWH, /* A/A Mux */
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF008A,
		.total_size	= 1024,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,		/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = NULL, /* AA 55 80 AA 55 10, only in A/A mux mode */
			}
		},
		.printlock	= printlock_sst_fwhub,
		.unlock		= unlock_sst_fwhub,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST49LF008C",
		.bustype	= BUS_FWH,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF008C,
		.total_size	= 1024,
		.page_size	= 4 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (sst49lfxxxc.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = erase_sector_49lfxxxc,
			}, {
				.eraseblocks = {
					{64 * 1024, 15},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_49lfxxxc,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST49LF016C",
		.bustype	= BUS_FWH,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF016C,
		.total_size	= 2048,
		.page_size	= 4 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_OK_PR,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (sst49lfxxxc.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = erase_sector_49lfxxxc,
			}, {
				.eraseblocks = {
					{64 * 1024, 31},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_49lfxxxc,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST49LF020",
		.bustype	= BUS_LPC,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF020,
		.total_size	= 256,
		.page_size	= 16 * 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,			/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {16 * 1024, 16} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = NULL,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST49LF020A",
		.bustype	= BUS_LPC,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF020A,
		.total_size	= 256,
		.page_size	= 4 * 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PRE,
		.probe		= probe_jedec,
		.probe_timing	= 1,			/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {16 * 1024, 16} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = NULL,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST49LF040",
		.bustype	= BUS_LPC,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF040,
		.total_size	= 512,
		.page_size	= 4096,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,			/* 150 ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = NULL,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST49LF040B",
		.bustype	= BUS_LPC, /* A/A Mux */
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF040B,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_EITHER_RESET | FEATURE_REGISTERMAP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 1,		/* 150ns */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = NULL,
			}
		},
		.unlock		= unlock_82802ab,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST49LF080A",
		.bustype	= BUS_LPC, /* A/A Mux */
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF080A,
		.total_size	= 1024,
		.page_size	= 4096,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_FIXME,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = NULL,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SST",
		.name		= "SST49LF160C",
		.bustype	= BUS_LPC,
		.manufacture_id	= SST_ID,
		.model_id	= SST_SST49LF160C,
		.total_size	= 2048,
		.page_size	= 4 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_OK_PRE,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (sst49lfxxxc.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = erase_sector_49lfxxxc,
			}, {
				.eraseblocks = {
					{64 * 1024, 31},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_49lfxxxc,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "ST",
		.name		= "M29F002B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M29F002B,
		.total_size	= 256,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_AAA | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 3},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4750, 5250}, /* 4.75-5.25V for type -X, others 4.5-5.5V */
	},

	{
		.vendor		= "ST",
		.name		= "M29F002T/NT",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M29F002T,
		.total_size	= 256,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_AAA | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4750, 5250}, /* 4.75-5.25V for type -X, others 4.5-5.5V */
	},

	{
		.vendor		= "ST",
		.name		= "M29F040B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M29F040B,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO, /* datasheet specifies no timing */
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		/* FIXME: this has WORD/BYTE sequences; 2AA for word, 555 for byte */
		.vendor		= "ST",
		.name		= "M29F400BB",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M29F400BB,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_SHIFTED | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_m29f400bt,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (m29f400bt.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 7},
				},
				.block_erase = block_erase_m29f400bt,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = block_erase_chip_m29f400bt,
			}
		},
		.write		= write_m29f400bt,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		/* FIXME: this has WORD/BYTE sequences; 2AA for word, 555 for byte */
		.vendor		= "ST",
		.name		= "M29F400BT",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M29F400BT,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_SHIFTED | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_m29f400bt,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (m29f400bt.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 7},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = block_erase_m29f400bt,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = block_erase_chip_m29f400bt,
			}
		},
		.write		= write_m29f400bt,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "ST",
		.name		= "M29W010B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M29W010B,
		.total_size	= 128,
		.page_size	= 16 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {16 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "ST",
		.name		= "M29W040B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M29W040B,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "ST",
		.name		= "M29W512B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= ST_ID,
		.model_id	= ST_M29W512B,
		.total_size	= 64,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "ST",
		.name		= "M50FLW040A",
		.bustype	= BUS_FWH | BUS_LPC, /* A/A Mux */
		.manufacture_id	= ST_ID,
		.model_id	= ST_M50FLW040A,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_FIXME,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 16}, /* sector */
					{64 * 1024, 5}, /* block */
					{4 * 1024, 16}, /* sector */
					{4 * 1024, 16}, /* sector */
				},
				.block_erase = NULL,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_stm50flw0x0x,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program & erase */
	},

	{
		.vendor		= "ST",
		.name		= "M50FLW040B",
		.bustype	= BUS_FWH | BUS_LPC, /* A/A Mux */
		.manufacture_id	= ST_ID,
		.model_id	= ST_M50FLW040B,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_FIXME,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 16}, /* sector */
					{4 * 1024, 16}, /* sector */
					{64 * 1024, 5}, /* block */
					{4 * 1024, 16}, /* sector */
				},
				.block_erase = NULL,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_stm50flw0x0x,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program & erase */
	},

	{
		.vendor		= "ST",
		.name		= "M50FLW080A",
		.bustype	= BUS_FWH | BUS_LPC, /* A/A Mux */
		.manufacture_id	= ST_ID,
		.model_id	= ST_M50FLW080A,
		.total_size	= 1024,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_OK_PRE,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_FIXME,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 16}, /* sector */
					{64 * 1024, 13}, /* block */
					{4 * 1024, 16}, /* sector */
					{4 * 1024, 16}, /* sector */
				},
				.block_erase = NULL,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_stm50flw0x0x,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program & erase */
	},

	{
		.vendor		= "ST",
		.name		= "M50FLW080B",
		.bustype	= BUS_FWH | BUS_LPC, /* A/A Mux */
		.manufacture_id	= ST_ID,
		.model_id	= ST_M50FLW080B,
		.total_size	= 1024,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_FIXME,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 16}, /* sector */
					{4 * 1024, 16}, /* sector */
					{64 * 1024, 13}, /* block */
					{4 * 1024, 16}, /* sector */
				},
				.block_erase = NULL,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_stm50flw0x0x,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program & erase */
	},

	{
		.vendor		= "ST",
		.name		= "M50FW002",
		.bustype	= BUS_FWH, /* A/A Mux */
		.manufacture_id	= ST_ID,
		.model_id	= ST_M50FW002,
		.total_size	= 256,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (sst49lfxxxc.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_stm50flw0x0x,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program & erase */
	},

	{
		.vendor		= "ST",
		.name		= "M50FW016",
		.bustype	= BUS_FWH, /* A/A Mux */
		.manufacture_id	= ST_ID,
		.model_id	= ST_M50FW016,
		.total_size	= 2048,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (82802ab.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_stm50flw0x0x,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program & erase */
	},

	{
		.vendor		= "ST",
		.name		= "M50FW040",
		.bustype	= BUS_FWH, /* A/A Mux */
		.manufacture_id	= ST_ID,
		.model_id	= ST_M50FW040,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_OK_PR,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (82802ab.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_stm50flw0x0x,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program & erase */
	},

	{
		.vendor		= "ST",
		.name		= "M50FW080",
		.bustype	= BUS_FWH, /* A/A Mux */
		.manufacture_id	= ST_ID,
		.model_id	= ST_M50FW080,
		.total_size	= 1024,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (82802ab.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_stm50flw0x0x,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program & erase */
	},

	{
		.vendor		= "ST",
		.name		= "M50LPW116",
		.bustype	= BUS_LPC, /* A/A Mux */
		.manufacture_id	= ST_ID,
		.model_id	= ST_M50LPW116,
		.total_size	= 2048,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{4 * 1024, 16},
					{64 * 1024, 30},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_block_82802ab,
			}
		},
		.unlock		= unlock_stm50flw0x0x,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program & erase */
	},

	{
		.vendor		= "Sanyo",
		.name		= "LF25FW203A",
		.bustype	= BUS_SPI,
		.manufacture_id	= SANYO_ID,
		.model_id	= SANYO_LE25FW203A,
		.total_size	= 2048,
		.page_size	= 256,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			},	{
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
	},

	{
		.vendor		= "Sharp",
		.name		= "LH28F008BJT-BTLZ1",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SHARP_ID,
		.model_id	= SHARP_LH28F008BJ__PB,
		.total_size	= 1024,
		.page_size	= 64 * 1024,
		.tested		= TEST_OK_PREW,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{8 * 1024, 8},
					{64 * 1024, 15}
				 },
				.block_erase = erase_block_82802ab,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = erase_sector_49lfxxxc,
			}
		},
		.unlock		= unlock_lh28f008bjt,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Sharp",
		.name		= "LHF00L04",
		.bustype	= BUS_FWH, /* A/A Mux */
		.manufacture_id	= SHARP_ID,
		.model_id	= SHARP_LHF00L04,
		.total_size	= 1024,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_EITHER_RESET | FEATURE_REGISTERMAP,
		.tested		= TEST_UNTESTED,
		.probe		= probe_82802ab,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 15},
					{8 * 1024, 8}
				 },
				.block_erase = erase_block_82802ab,
			}, {
				.eraseblocks = {
					{1024 * 1024, 1}
				},
				.block_erase = NULL, /* 30 D0, only in A/A mux mode */
			},
		},
		.unlock		= unlock_82802ab,
		.write		= write_82802ab,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Spansion",
		.name		= "S25FL004A",
		.bustype	= BUS_SPI,
		.manufacture_id	= SPANSION_ID,
		.model_id	= SPANSION_S25FL004A,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Spansion",
		.name		= "S25FL008A",
		.bustype	= BUS_SPI,
		.manufacture_id	= SPANSION_ID,
		.model_id	= SPANSION_S25FL008A,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PRE,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Spansion",
		.name		= "S25FL016A",
		.bustype	= BUS_SPI,
		.manufacture_id	= SPANSION_ID,
		.model_id	= SPANSION_S25FL016A,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Spansion",
		.name		= "S25FL032A/P",
		.bustype	= BUS_SPI,
		.manufacture_id	= SPANSION_ID,
		.model_id	= SPANSION_S25FL032A,
		.total_size	= 4096,
		.page_size	= 256,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Spansion",
		.name		= "S25FL064A/P",
		.bustype	= BUS_SPI,
		.manufacture_id	= SPANSION_ID,
		.model_id	= SPANSION_S25FL064A,
		.total_size	= 8192,
		.page_size	= 256,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Spansion",
		.name		= "S25FL116K/S25FL216K", /* FIXME: separate them */
		.bustype	= BUS_SPI,
		.manufacture_id	= SPANSION_ID,
		.model_id	= SPANSION_S25FL116K,
		.total_size	= 2048,
		.page_size	= 256,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		/* TODO: write-protection */
	},

	{
		.vendor		= "Spansion",
		.name		= "S25FL128S_UL Uniform 128 kB Sectors",
		.bustype	= BUS_SPI,
		.manufacture_id	= SPANSION_ID,
		.model_id	= SPANSION_S25FL128S_UL,
		.total_size	= 16384,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_big_spansion,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {128 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1700, 2000},
		.wp		= &wp_generic,
	},

	{
		.vendor		= "Spansion",
		.name		= "S25FL128S_US Uniform 64 kB Sectors",
		.bustype	= BUS_SPI,
		.manufacture_id	= SPANSION_ID,
		.model_id	= SPANSION_S25FL128S_US,
		.total_size	= 16384,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_big_spansion,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1700, 2000},
		.wp		= &wp_generic,
	},

	{
		.vendor		= "Spansion",
		.name		= "S25FL256S Large Sectors",
		.bustype	= BUS_SPI,
		.manufacture_id	= SPANSION_ID,
		.model_id	= SPANSION_S25FL256S_UL,
		.total_size	= 16384,  /* This is just half the size.... */
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_big_spansion,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {256 * 1024, 64} },
				.block_erase = s25fl_block_erase,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1700, 2000},
		.wp		= &wp_generic,
	},

	{
		.vendor		= "Spansion",
		.name		= "S25FL256S Small Sectors",
		.bustype	= BUS_SPI,
		.manufacture_id	= SPANSION_ID,
		.model_id	= SPANSION_S25FL256S_US,
		.total_size	= 16384,   /* This is just half the size.... */
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_big_spansion,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = s25fl_block_erase,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1700, 2000},
		.wp		= &wp_generic,
	},

	{
		.vendor		= "Spansion",
		.name		= "S25FS128S Large Sectors",
		.bustype	= BUS_SPI,
		.manufacture_id	= SPANSION_ID,
		.model_id	= SPANSION_S25FS128S_L,
		.total_size	= 16384,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_UNTESTED,
		.probe		= probe_spi_big_spansion,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = s25fs_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1700, 2000},
		.wp		= &wp_generic,
	},

	{
		.vendor		= "Spansion",
		.name		= "S25FS128S Small Sectors",
		.bustype	= BUS_SPI,
		.manufacture_id	= SPANSION_ID,
		.model_id	= SPANSION_S25FS128S_S,
		.total_size	= 16384,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_big_spansion,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = s25fs_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			},
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1700, 2000},
		.wp		= &wp_generic,
	},

	{
		.vendor		= "SyncMOS/MoselVitelic",
		.name		= "{F,S,V}29C51001B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= SM_MVC_29C51001B,
		.total_size	= 128,
		.page_size	= 512,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {512, 256} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "SyncMOS/MoselVitelic",
		.name		= "{F,S,V}29C51001T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= SM_MVC_29C51001T,
		.total_size	= 128,
		.page_size	= 512,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {512, 256} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "SyncMOS/MoselVitelic",
		.name		= "{F,S,V}29C51002B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= SM_MVC_29C51002B,
		.total_size	= 256,
		.page_size	= 512,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {512, 512} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
	},

	{
		.vendor		= "SyncMOS/MoselVitelic",
		.name		= "{F,S,V}29C51002T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= SM_MVC_29C51002T,
		.total_size	= 256,
		.page_size	= 512,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {512, 512} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
	},

	{
		.vendor		= "SyncMOS/MoselVitelic",
		.name		= "{F,S,V}29C51004B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= SM_MVC_29C51004B,
		.total_size	= 512,
		.page_size	= 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {1024, 512} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "SyncMOS/MoselVitelic",
		.name		= "{F,S,V}29C51004T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= SM_MVC_29C51004T,
		.total_size	= 512,
		.page_size	= 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {1024, 512} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "SyncMOS/MoselVitelic",
		.name		= "{S,V}29C31004B",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= SM_MVC_29C31004B,
		.total_size	= 512,
		.page_size	= 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {1024, 512} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "SyncMOS/MoselVitelic",
		.name		= "{S,V}29C31004T",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= SYNCMOS_MVC_ID,
		.model_id	= SM_MVC_29C31004T,
		.total_size	= 512,
		.page_size	= 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = { {1024, 512} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "TI",
		.name		= "TMS29F002RB",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= TI_OLD_ID,
		.model_id	= TI_TMS29F002RB,
		.total_size	= 256,
		.page_size	= 16384, /* Non-uniform sectors */
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{16 * 1024, 1},
					{8 * 1024, 2},
					{32 * 1024, 1},
					{64 * 1024, 3},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "TI",
		.name		= "TMS29F002RT",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= TI_OLD_ID,
		.model_id	= TI_TMS29F002RT,
		.total_size	= 256,
		.page_size	= 16384, /* Non-uniform sectors */
		.feature_bits	= FEATURE_ADDR_2AA | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= TIMING_ZERO,	/* Datasheet has no timing info specified */
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			},
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q128.V",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q128_V,
		.total_size	= 16384,
		.page_size	= 256,
		/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 4096} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 512} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25q,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q128.V..M",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q128_V_M,
		.total_size	= 16384,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 4096} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 512} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q128.W",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q128_W,
		.total_size	= 16384,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 4096} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 512} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 256} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {16 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		/*
		 * W25Q128FW is a 1.8V chip, however 3.3V variants with the same
		 * model ID exist. We'll err on the side of caution here. A user
		 * with a 3.3V chip sharing the model ID will need to either
		 * specify voltage on the command line or duplicate this struct
		 * with a different name/voltage and specify it with "-c".
		 */
		.voltage	= {1650, 1950},
		.wp		= &wp_w25q,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q16",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q16_V,
		.total_size	= 2048,
		.page_size	= 256,
		/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 64} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25q,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q256.V",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q256_V,
		.total_size	= 32768,
		.page_size	= 256,
		/* supports SFDP */
		/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
		/* FOUR_BYTE_ADDR: supports 4-bytes addressing mode */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP | FEATURE_4BA_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 8192} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 1024} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 512} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {32 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {32 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.set_4ba	= spi_enter_4ba_b7_we,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q256JV_M",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q256JV_M,
		.total_size	= 32768,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP | FEATURE_4BA_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 8192} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 1024} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 512} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {32 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {32 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.set_4ba	= spi_enter_4ba_b7_we,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25q_large,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q32.W",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q32_W,
		.total_size	= 4096,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 128} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1700, 1950},
		.wp		= &wp_w25q,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q32JW",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q32JW,
		.total_size	= 4096,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 128} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1700, 1950},
		.wp		= &wp_w25q,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q32_V",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q32_V,
		.total_size	= 4096,
		.page_size	= 256,
		/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 128} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25q,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q40EW",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q40EW,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 16} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1650, 1950},
		/* FIXME(dhendrix): Add write-protection support */
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q64",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q64_V,
		.total_size	= 8192,
		.page_size	= 256,
		/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 256} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25q,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q64DW",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q64_W,
		.total_size	= 8192,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 256} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {1700, 1950},
		.wp		= &wp_w25q,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25Q80",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25Q80_V,
		.total_size	= 1024,
		.page_size	= 256,
		/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42 */
		.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 32} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25q,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25X10",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25X10,
		.total_size	= 128,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 32} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 2} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25X16",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25X16,
		.total_size	= 2048,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 512} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 64} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 32} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {2 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Winbond",
		.name		= "W25X20",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25X20,
		.total_size	= 256,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 64} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 4} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25X32",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25X32,
		.total_size	= 4096,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 1024} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 128} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 64} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {4 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Winbond",
		.name		= "W25X40",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25X40,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	{
		.vendor		= "Winbond",
		.name		= "W25X64",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25X64,
		.total_size	= 8192,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 2048} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {32 * 1024, 256} },
				.block_erase = spi_block_erase_52,
			}, {
				.eraseblocks = { {64 * 1024, 128} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_60,
			}, {
				.eraseblocks = { {8 * 1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
	},

	{
		.vendor		= "Winbond",
		.name		= "W25X80",
		.bustype	= BUS_SPI,
		.manufacture_id	= WINBOND_NEX_ID,
		.model_id	= WINBOND_NEX_W25X80,
		.total_size	= 1024,
		.page_size	= 256,
		.feature_bits	= FEATURE_WRSR_WREN,
		.tested		= TEST_OK_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 256} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = spi_block_erase_d8,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = spi_block_erase_c7,
			}
		},
		.printlock	= spi_prettyprint_status_register_plain,
		.unlock		= spi_disable_blockprotect,
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
		.voltage	= {2700, 3600},
		.wp		= &wp_w25,
	},

	/* W29EE011, W29EE012, W29C010M, W29C011A do not support probe_jedec according to the datasheet, but it works for newer(?) steppings. */
	{
		.vendor		= "Winbond",
		.name		= "W29C010(M)/W29C011A/W29EE011/W29EE012",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W29C010,
		.total_size	= 128,
		.page_size	= 128,
		.feature_bits	= FEATURE_LONG_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10,		/* used datasheet for the W29C011A */
		.block_erasers	=
		{
			{
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec,
		.read		= read_memmapped,
	},

	{
		.vendor		= "Winbond",
		.name		= "W29C010(M)/W29C011A/W29EE011/W29EE012-old",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W29C010,
		.total_size	= 128,
		.page_size	= 128,
		.feature_bits	= FEATURE_LONG_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_w29ee011,
		.probe_timing	= TIMING_IGNORED, /* routine doesn't use probe_timing (w29ee011.c) */
		.block_erasers	=
		{
			{
				.eraseblocks = { {128 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec,
		.read		= read_memmapped,
	},

	{
		.vendor		= "Winbond",
		.name		= "W29C020(C)/W29C022",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W29C020,
		.total_size	= 256,
		.page_size	= 128,
		.feature_bits	= FEATURE_LONG_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Winbond",
		.name		= "W29C040/P",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W29C040,
		.total_size	= 512,
		.page_size	= 256,
		.feature_bits	= FEATURE_LONG_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Winbond",
		.name		= "W39L040",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W39L040,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PR,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.printlock	= printlock_w39l040,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Winbond",
		.name		= "W39V040A",
		.bustype	= BUS_LPC,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W39V040A,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.printlock	= printlock_w39v040a,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Winbond",
		.name		= "W39V040B",
		.bustype	= BUS_LPC,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W39V040B,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.printlock	= printlock_w39v040b,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Winbond",
		.name		= "W39V040C",
		.bustype	= BUS_LPC,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W39V040C,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.printlock	= printlock_w39v040c,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Winbond",
		.name		= "W39V040FA",
		.bustype	= BUS_FWH,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W39V040FA,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PRE,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 128} },
				.block_erase = erase_block_jedec,
			}, {
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.printlock	= printlock_w39v040fa,
		.unlock		= unlock_sst_fwhub,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Winbond",
		.name		= "W39V040FB",
		.bustype	= BUS_FWH,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W39V040B,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.printlock	= printlock_w39v040fb,
		.unlock		= unlock_w39v040fb,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program */
	},

	{
		.vendor		= "Winbond",
		.name		= "W39V040FC",
		.bustype	= BUS_FWH,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W39V040C,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.printlock	= printlock_w39v040fc,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program */
	},

	{
		.vendor		= "Winbond",
		.name		= "W39V080A",
		.bustype	= BUS_LPC,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W39V080A,
		.total_size	= 1024,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.printlock	= printlock_w39v080a,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Winbond",
		.name		= "W39V080FA",
		.bustype	= BUS_FWH,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W39V080FA,
		.total_size	= 1024,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP | FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 16} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {1024 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.printlock	= printlock_w39v080fa,
		.unlock		= unlock_w39v080fa,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program */
	},

	{
		.vendor		= "Winbond",
		.name		= "W39V080FA (dual mode)",
		.bustype	= BUS_FWH,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W39V080FA_DM,
		.total_size	= 512,
		.page_size	= 64 * 1024,
		.feature_bits	= FEATURE_REGISTERMAP | FEATURE_EITHER_RESET,
		.tested		= TEST_UNTESTED,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {64 * 1024, 8} },
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {512 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.printlock	= printlock_w39v080fa_dual,
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600}, /* Also has 12V fast program */
	},

	{
		.vendor		= "Winbond",
		.name		= "W49F002U/N",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W49F002U,
		.total_size	= 256,
		.page_size	= 128,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{128 * 1024, 1},
					{96 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Winbond",
		.name		= "W49F020",
		.bustype	= BUS_PARALLEL,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W49F020,
		.total_size	= 256,
		.page_size	= 128,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PROBE,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {4500, 5500},
	},

	{
		.vendor		= "Winbond",
		.name		= "W49V002A",
		.bustype	= BUS_LPC,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W49V002A,
		.total_size	= 256,
		.page_size	= 128,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Winbond",
		.name		= "W49V002FA",
		.bustype	= BUS_FWH,
		.manufacture_id	= WINBOND_ID,
		.model_id	= WINBOND_W49V002FA,
		.total_size	= 256,
		.page_size	= 128,
		.feature_bits	= FEATURE_EITHER_RESET,
		.tested		= TEST_OK_PREW,
		.probe		= probe_jedec,
		.probe_timing	= 10,
		.block_erasers	=
		{
			{
				.eraseblocks = {
					{64 * 1024, 3},
					{32 * 1024, 1},
					{8 * 1024, 2},
					{16 * 1024, 1},
				},
				.block_erase = erase_sector_jedec,
			}, {
				.eraseblocks = { {256 * 1024, 1} },
				.block_erase = erase_chip_block_jedec,
			}
		},
		.write		= write_jedec_1,
		.read		= read_memmapped,
		.voltage	= {3000, 3600},
	},

	{
		.vendor		= "Programmer",
		.name		= "Opaque flash chip",
		.bustype	= BUS_PROG,
		.manufacture_id	= PROGMANUF_ID,
		.model_id	= PROGDEV_ID,
		.total_size	= 0,
		.page_size	= 256,
		/* probe is assumed to work, rest will be filled in by probe */
		.tested		= TEST_OK_PROBE,
		.probe		= probe_opaque,
		/* eraseblock sizes will be set by the probing function */
		.block_erasers	=
		{
			{
				.block_erase = erase_opaque,
			}
		},
		.write		= write_opaque,
		.read		= read_opaque,
	},

	{
		.vendor		= "AMIC",
		.name		= "unknown AMIC SPI chip",
		.bustype	= BUS_SPI,
		.manufacture_id	= AMIC_ID,
		.model_id	= GENERIC_DEVICE_ID,
		.total_size	= 0,
		.page_size	= 256,
		.tested		= TEST_BAD_PREW,
		.probe		= probe_spi_rdid4,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
	},

	{
		.vendor		= "Atmel",
		.name		= "unknown Atmel SPI chip",
		.bustype	= BUS_SPI,
		.manufacture_id	= ATMEL_ID,
		.model_id	= GENERIC_DEVICE_ID,
		.total_size	= 0,
		.page_size	= 256,
		.tested		= TEST_BAD_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
	},

	{
		.vendor		= "Eon",
		.name		= "unknown Eon SPI chip",
		.bustype	= BUS_SPI,
		.manufacture_id	= EON_ID_NOPREFIX,
		.model_id	= GENERIC_DEVICE_ID,
		.total_size	= 0,
		.page_size	= 256,
		.tested		= TEST_BAD_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
	},

	{
		.vendor		= "Macronix",
		.name		= "unknown Macronix SPI chip",
		.bustype	= BUS_SPI,
		.manufacture_id	= MACRONIX_ID,
		.model_id	= GENERIC_DEVICE_ID,
		.total_size	= 0,
		.page_size	= 256,
		.tested		= TEST_BAD_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
	},

	{
		.vendor		= "PMC",
		.name		= "unknown PMC SPI chip",
		.bustype	= BUS_SPI,
		.manufacture_id	= PMC_ID,
		.model_id	= GENERIC_DEVICE_ID,
		.total_size	= 0,
		.page_size	= 256,
		.tested		= TEST_BAD_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
	},

	{
		.vendor		= "SST",
		.name		= "unknown SST SPI chip",
		.bustype	= BUS_SPI,
		.manufacture_id	= SST_ID,
		.model_id	= GENERIC_DEVICE_ID,
		.total_size	= 0,
		.page_size	= 256,
		.tested		= TEST_BAD_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
	},

	{
		.vendor		= "ST",
		.name		= "unknown ST SPI chip",
		.bustype	= BUS_SPI,
		.manufacture_id	= ST_ID,
		.model_id	= GENERIC_DEVICE_ID,
		.total_size	= 0,
		.page_size	= 256,
		.tested		= TEST_BAD_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
	},

	{
		.vendor		= "Sanyo",
		.name		= "unknown Sanyo SPI chip",
		.bustype	= BUS_SPI,
		.manufacture_id	= SANYO_ID,
		.model_id	= GENERIC_DEVICE_ID,
		.total_size	= 0,
		.page_size	= 256,
		.tested		= TEST_BAD_PREW,
		.probe		= probe_spi_rdid,
		.probe_timing	= TIMING_ZERO,
		.write		= NULL,
		.read		= NULL,
	},

	{
		.vendor		= "Generic",
		.name		= "Variable Size SPI chip",
		.bustype	= BUS_SPI,
		.manufacture_id	= VARIABLE_SIZE_MANUF_ID,
		.model_id	= VARIABLE_SIZE_DEVICE_ID,
		.total_size	= 64,  /* This size is set temporarily */
		.page_size	= 256,
		.tested		= TEST_OK_PREW,
		.probe		= probe_variable_size,
		.block_erasers	=
		{
			{
				.eraseblocks = { {4 * 1024, 16} },
				.block_erase = spi_block_erase_20,
			}, {
				.eraseblocks = { {64 * 1024, 1} },
				.block_erase = spi_block_erase_d8,
			}
		},
		.write		= spi_chip_write_256,
		.read		= spi_chip_read,
	},

	{
		.vendor		= "Generic",
		.name		= "unknown SPI chip (RDID)",
		.bustype	= BUS_SPI,
		.manufacture_id	= GENERIC_MANUF_ID,
		.model_id	= GENERIC_DEVICE_ID,
		.total_size	= 0,
		.page_size	= 256,
		.tested		= TEST_BAD_PREW,
		.probe		= probe_spi_rdid,
		.write		= NULL,
	},

	{
		.vendor		= "Generic",
		.name		= "unknown SPI chip (REMS)",
		.bustype	= BUS_SPI,
		.manufacture_id	= GENERIC_MANUF_ID,
		.model_id	= GENERIC_DEVICE_ID,
		.total_size	= 0,
		.page_size	= 256,
		.tested		= TEST_BAD_PREW,
		.probe		= probe_spi_rems,
		.write		= NULL,
	},

	{0}
};

const unsigned int flashchips_size = ARRAY_SIZE(flashchips);

/* Generic flashchip struct for platforms that use Intel hardware sequencing. */
const struct flashchip flashchips_hwseq[] = {
	{
		.vendor		= "Generic",
		.name		= "HWSEQ chip",
		.bustype	= BUS_PROG,
		/* probe is assumed to work, rest will be filled in by probe */
		.tested		= TEST_OK_PREW,
		.probe		= probe_opaque,
		/* eraseblock sizes will be set by the probing function */
		.block_erasers	=
		{
			{
				.block_erase = erase_opaque,
			}
		},
		.write		= write_opaque,
		.read		= read_opaque,
		.read_status	= read_status_opaque,
		.write_status	= write_status_opaque,
		.check_access	= check_access_opaque,
		.wp		= &wp_w25,
		.unlock	= &spi_disable_blockprotect,
	},

	{NULL}
};

int flash_erase_value(struct flashctx *flash)
{
	return flash->chip->feature_bits & FEATURE_ERASE_TO_ZERO ? 0 : 0xff;
}

int flash_unerased_value(struct flashctx *flash)
{
	return flash->chip->feature_bits & FEATURE_ERASE_TO_ZERO ? 0xff : 0;
}

const struct flashchip *flash_id_to_entry(uint32_t mfg_id, uint32_t model_id)
{
	const struct flashchip *chip;

	for (chip = &flashchips[0]; chip->vendor; chip++) {
		if ((chip->manufacture_id == mfg_id) &&
			(chip->model_id == model_id))
			return chip;
	}

	return NULL;
}

struct voltage_range voltage_ranges[NUM_VOLTAGE_RANGES];

static int compar(const void *_x, const void *_y)
{
	const struct voltage_range *x = _x;
	const struct voltage_range *y = _y;

	/*
	 * qsort() places entries in ascending order. We will sort by minimum
	 * voltage primarily and max voltage secondarily, and move empty sets
	 * to the end of array.
	 */
	if (x->min == 0)
		return 1;
	if (y->min == 0)
		return -1;

	if (x->min < y->min)
		return -1;
	if (x->min > y->min)
		return 1;

	if (x->min == y->min) {
		if (x->max < y->max)
			return -1;
		if (x->max > y->max)
			return 1;
	}

	return 0;
}

int flash_supported_voltage_ranges(enum chipbustype bus)
{
	int i;
	int unique_ranges = 0;

	/* clear array in case user calls this function multiple times */
	memset(voltage_ranges, 0, sizeof(voltage_ranges));

	for (i = 0; i < ARRAY_SIZE(flashchips); i++) {
		int j;
		int match_found = 0;

		if (unique_ranges >= NUM_VOLTAGE_RANGES) {
			msg_cerr("Increase NUM_VOLTAGE_RANGES.\n");
			return -1;
		}

		if (!(flashchips[i].bustype & bus))
			continue;

		for (j = 0; j < NUM_VOLTAGE_RANGES; j++) {
			if ((flashchips[i].voltage.min == voltage_ranges[j].min) &&
				(flashchips[i].voltage.max == voltage_ranges[j].max))
				match_found |= 1;

			if (!voltage_ranges[j].min && !voltage_ranges[j].max)
				break;
		}

		if (!match_found) {
			voltage_ranges[j] = flashchips[i].voltage;
			unique_ranges++;
		}
	}

	qsort(&voltage_ranges[0], NUM_VOLTAGE_RANGES,
			sizeof(voltage_ranges[0]), compar);

	for (i = 0; i < NUM_VOLTAGE_RANGES; i++) {
		msg_cspew("%s: voltage_range[%d]: { %u, %u }\n",
			__func__, i, voltage_ranges[i].min, voltage_ranges[i].max);
	}

	return unique_ranges;
}

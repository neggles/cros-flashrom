/*
 * This file is part of the flashrom project.
 *
 * Copyright 2020 Google LLC
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
#include <stdint.h>

#include "flash.h"
#include "wp_tests.h"

int __wrap_spi_send_command(const struct flashctx *flash,
		unsigned int writecnt, unsigned int readcnt,
		const unsigned char *writearr, unsigned char *readarr)
{
	function_called();
	check_expected_ptr(flash);
	assert_int_equal(writecnt,    mock_type(int));
	assert_int_equal(writearr[0], mock_type(int));

	int rcnt = mock_type(int);
	assert_int_equal(readcnt, rcnt);
	for (int i = 0; i < rcnt; i++)
		readarr[i] = i;

	return 0;
}

uint8_t __wrap_spi_read_status_register(const struct flashctx *flash)
{
	function_called();
	return mock_type(int);
}


int __wrap_spi_write_status_register(const struct flashctx *flash, int value)
{
	function_called();
	assert_int_equal(value, mock_type(int));
	return 0;
}

uint8_t __wrap_w25q_read_status_register_2(const struct flashctx *flash)
{
	function_called();
	return mock_type(int);
}

int __wrap_w25q_write_status_register_WREN(const struct flashctx *flash, uint8_t s1, uint8_t s2)
{
	function_called();
	assert_int_equal(s1, mock_type(int));
	assert_int_equal(s2, mock_type(int));
	return 0;
}

uint8_t __wrap_mx25l_read_config_register(const struct flashctx *flash)
{
	function_called();
	return mock_type(int);
}

void expect_sr1_read(uint8_t mock_value)
{
	expect_function_call(__wrap_spi_read_status_register);
	will_return(__wrap_spi_read_status_register, mock_value);
}

void expect_sr1_write(uint8_t expected_value)
{
	expect_function_call(__wrap_spi_write_status_register);
	will_return(__wrap_spi_write_status_register, expected_value);
}

void expect_sr2_read(uint8_t mock_value)
{
	expect_function_call(__wrap_w25q_read_status_register_2);
	will_return(__wrap_w25q_read_status_register_2, mock_value);
}

void expect_sr1_sr2_write(uint8_t expected_sr1, uint8_t expected_sr2)
{
	expect_function_call(__wrap_w25q_write_status_register_WREN);
	will_return(__wrap_w25q_write_status_register_WREN, expected_sr1);
	will_return(__wrap_w25q_write_status_register_WREN, expected_sr2);
}

uint8_t expect_cr1_read(uint8_t mock_value)
{
	expect_function_call(__wrap_mx25l_read_config_register);
	will_return(__wrap_mx25l_read_config_register, mock_value);
}

int main(void)
{
	int ret = 0;

	cmocka_set_message_output(CM_OUTPUT_STDOUT);

	const struct CMUnitTest writeprotect_tests[] = {};
	ret |= cmocka_run_group_tests_name("writeprotect.c tests", writeprotect_tests, NULL, NULL);

	return ret;
}

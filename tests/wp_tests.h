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

#ifndef WP_TESTS_H
#define WP_TESTS_H

void expect_sr1_read(uint8_t mock_value);
void expect_sr1_write(uint8_t expected_value);

void expect_sr2_read(uint8_t mock_value);
void expect_sr1_sr2_write(uint8_t expected_sr1, uint8_t expected_sr2);

uint8_t expect_cr1_read(uint8_t mock_value);


#endif

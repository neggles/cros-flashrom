/* Copyright 2018 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* This file is a wrapper to invoke all available flashrom test modules. */

#include <stdio.h>

#include "../action_descriptor.h"
#include "../flash.h"

/*
 * Stubs/replacement functions to avoid linking too much when building the
 * test image.
 */
char *logfile; /* No logging into a file. */
int verbose_logfile; /* Does not matter if there is no logging into a file. */
enum flashrom_log_level verbose_screen = FLASHROM_MSG_ERROR;

void print_version(void)
{
}

int flash_erase_value(struct flashctx *flash)
{
	return flash->chip->feature_bits & FEATURE_ERASED_ZERO ? 0 : 0xff;
}



int main(int argc, char *argv[])
{
	int result = 0;

	result += test_action_descriptor();

	if (!result)
		printf("Success!\n");
	else
		fprintf(stderr, "Error: %d tests FAILED!\n", result);

	return result;
}



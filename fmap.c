/*
 * Copyright 2015, Google Inc.
 * Copyright 2018-present, Facebook Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *    * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *    * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "flash.h"
#include "fmap.h"

static size_t fmap_size(const struct fmap *fmap)
{
	return sizeof(*fmap) + (fmap->nareas * sizeof(struct fmap_area));
}

static int is_valid_fmap(const struct fmap *fmap)
{
	if (memcmp(fmap, FMAP_SIGNATURE, strlen(FMAP_SIGNATURE)) != 0)
		return 0;
	/* strings containing the magic tend to fail here */
	if (fmap->ver_major > FMAP_VER_MAJOR)
		return 0;
	if (fmap->ver_minor > FMAP_VER_MINOR)
		return 0;
	/* a basic consistency check: flash address space size should be larger
	 * than the size of the fmap data structure */
	if (fmap->size < fmap_size(fmap))
		return 0;

	/* fmap-alikes along binary data tend to fail on having a valid,
	 * null-terminated string in the name field.*/
	int i;
	for (i = 0; i < FMAP_STRLEN; i++) {
		if (fmap->name[i] == 0)
			break;
		if (!isgraph(fmap->name[i]))
			return 0;
		if (i == FMAP_STRLEN - 1) {
			/* name is specified to be null terminated single-word string
			 * without spaces. We did not break in the 0 test, we know it
			 * is a printable spaceless string but we're seeing FMAP_STRLEN
			 * symbols, which is one too many.
			 */
			 return 0;
		}
	}
	return 1;

}

/**
 * @brief Do a brute-force linear search for fmap in provided buffer
 *
 * @param[in] buffer The buffer to search
 * @param[in] len Length (in bytes) to search
 *
 * @return offset in buffer where fmap is found if successful
 *         -1 to indicate that fmap was not found
 *         -2 to indicate fmap is truncated or exceeds buffer + len
 */
static off_t fmap_lsearch(const uint8_t *buf, size_t len)
{
	off_t offset;
	bool fmap_found = 0;

	for (offset = 0; offset <= (off_t)(len - sizeof(struct fmap)); offset++) {
		if (is_valid_fmap((struct fmap *)&buf[offset])) {
			fmap_found = 1;
			break;
		}
	}

	if (!fmap_found)
		return -1;

	if (offset + fmap_size((struct fmap *)&buf[offset]) > len) {
		msg_gerr("fmap size exceeds buffer boundary.\n");
		return -2;
	}

	return offset;
}

/**
 * @brief Read fmap from provided buffer and copy it to fmap_out
 *
 * @param[out] fmap_out Double-pointer to location to store fmap contents.
 *                      Caller must free allocated fmap contents.
 * @param[in] buf Buffer to search
 * @param[in] len Length (in bytes) to search
 *
 * @return 0 if successful
 *         1 to indicate error
 *         2 to indicate fmap is not found
 */
int fmap_read_from_buffer(struct fmap **fmap_out, const uint8_t *const buf, size_t len)
{
	off_t offset = fmap_lsearch(buf, len);
	if (offset < 0) {
		msg_gdbg("Unable to find fmap in provided buffer.\n");
		return 2;
	}
	msg_gdbg("Found fmap at offset 0x%06zx\n", (size_t)offset);

	const struct fmap *fmap = (const struct fmap *)(buf + offset);
	*fmap_out = malloc(fmap_size(fmap));
	if (*fmap_out == NULL) {
		msg_gerr("Out of memory.\n");
		return 1;
	}

	memcpy(*fmap_out, fmap, fmap_size(fmap));
	return 0;
}

int fmap_find(void *source_handle,
	      int (*read_chunk)(void *handle,
				void *dest,
				size_t offset,
				size_t size),
	      struct fmap *fmap,
	      loff_t offset,
	      uint8_t **buf)
{
	int buf_size;

	if (!is_valid_fmap(fmap))
		return 0;

	buf_size = fmap_size(fmap);
	*buf = malloc(buf_size);

	if (read_chunk(source_handle, *buf, offset, buf_size)) {
		msg_gdbg("[L%d] failed to read %d bytes at offset 0x%lx\n",
			 __LINE__, buf_size, (unsigned long)offset);
		return -1;
	}

	return 1;
}

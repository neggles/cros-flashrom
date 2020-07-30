/*
 * This file is part of the flashrom project.
 *
 * Copyright (C) 2005-2008 coresystems GmbH
 * (Written by Stefan Reinauer <stepan@coresystems.de> for coresystems GmbH)
 * Copyright (C) 2011-2013 Stefan Tauner
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>

#include "flash.h"
#include "fmap.h"
#include "layout.h"
#include "platform.h"
#include "programmer.h"
#include "search.h"

#define ACPI_FMAP_PATH "/sys/devices/platform/chromeos_acpi/FMAP"
#define FDT_FMAP_PATH "/proc/device-tree/firmware/chromeos/fmap-offset"

static struct romentry entries[MAX_ROMLAYOUT];
static struct flashrom_layout global_layout = { entries, 0 };

/*
 * This variable is set to the lowest erase granularity; it is used when
 * deciding if the layout map needs to be adjusted such that erase boundaries
 * match this granularity.
 */
static unsigned int required_erase_size;

/*
 * include_args lists arguments specified at the command line with -i. They
 * must be processed at some point so that desired regions are marked as
 * "included" in the master layout.
 */
static char *include_args[MAX_ROMLAYOUT];
static int num_include_args = 0;  /* the number of successfully parsed entries. */

struct flashrom_layout *get_global_layout(void)
{
	return &global_layout;
}

#ifndef __LIBPAYLOAD__
int read_romlayout(char *name)
{
	struct flashrom_layout *const layout = get_global_layout();
	FILE *romlayout;
	char tempstr[256];
	int i;

	romlayout = fopen(name, "r");

	if (!romlayout) {
		msg_gerr("ERROR: Could not open ROM layout (%s).\n",
			name);
		return -1;
	}

	while (!feof(romlayout)) {
		char *tstr1, *tstr2;

		if (layout->num_entries >= MAX_ROMLAYOUT) {
			msg_gerr("Maximum number of ROM images (%i) in layout "
				 "file reached before end of layout file.\n",
				 MAX_ROMLAYOUT);
			msg_gerr("Ignoring the rest of the layout file.\n");
			break;
		}
		if (2 != fscanf(romlayout, "%255s %255s\n", tempstr, layout->entries[layout->num_entries].name))
			continue;
#if 0
		// fscanf does not like arbitrary comments like that :( later
		if (tempstr[0] == '#') {
			continue;
		}
#endif
		tstr1 = strtok(tempstr, ":");
		tstr2 = strtok(NULL, ":");
		if (!tstr1 || !tstr2) {
			msg_gerr("Error parsing layout file.\n");
			fclose(romlayout);
			return 1;
		}
		layout->entries[layout->num_entries].start = strtol(tstr1, (char **)NULL, 16);
		layout->entries[layout->num_entries].end = strtol(tstr2, (char **)NULL, 16);
		layout->entries[layout->num_entries].included = 0;
		strcpy(layout->entries[layout->num_entries].file, "");
		layout->num_entries++;
	}

	for (i = 0; i < layout->num_entries; i++) {
		msg_gdbg("romlayout %08x - %08x named %s\n",
			     layout->entries[i].start,
			     layout->entries[i].end, layout->entries[i].name);
	}

	fclose(romlayout);

	return 0;
}
#endif

/* Read value from ACPI in sysfs, if it exists. */
__attribute__((unused)) static int read_fmap_base_acpi(uint32_t *out)
{
	int rv = 0;
	FILE *f;

	if (!(f = fopen(ACPI_FMAP_PATH, "r")))
		return -1;

	/* FMAP base is an ASCII signed integer. */
	if (fscanf(f, "%d", (int *)out) != 1)
		rv = -1;

	fclose(f);

	if (rv)
		msg_gdbg("%s: failed to read fmap_base from ACPI\n", __func__);
	else
		msg_gdbg("%s: read fmap_base from ACPI\n", __func__);

	return rv;
}

/* Read value from FDT, if it exists. */
__attribute__((unused)) static int read_fmap_base_fdt(uint32_t *out)
{
	int rv = 0;
	uint32_t data;
	FILE *f;

	if (!(f = fopen(FDT_FMAP_PATH, "r")))
		return -1;

	/* Value is stored as network-byte order dword. */
	if (fread(&data, sizeof(data), 1, f) != 1)
		rv = -1;
	else
		*out = ntohl(data);

	fclose(f);

	if (rv)
		msg_gdbg("%s: failed to read fmap_base from FDT\n", __func__);
	else
		msg_gdbg("%s: read fmap_base from FDT\n", __func__);

	return rv;
}

/*
 * Find the FMAP base from ACPI or FDT.
 * @search: Search information
 * @offset: Place to put offset
 * @return 0 if offset found, -1 if not
 */
static int get_fmap_base(struct search_info *search, off_t *offset)
{
	uint32_t fmap_base;
	uint32_t from_top;

#if IS_X86
	if (read_fmap_base_acpi(&fmap_base) < 0)
		return -1;
#elif IS_ARM
	if (read_fmap_base_fdt(&fmap_base) < 0)
		return -1;
#else
	return -1;
#endif

	/*
	 * TODO(b/158017386): see if we can remove this hack.  It may
	 * only apply to older platforms which are now AUE.
	 *
	 * There are 2 kinds of fmap_base.
	 *
	 *  1. Shadow ROM/BIOS area (x86), such as 0xFFxxxxxx.
	 *  2. Offset to start of flash, such as 0x00xxxxxx.
	 *
	 * The shadow ROM is a cached copy of the BIOS ROM which resides below
	 * 4GB host/CPU memory address space on x86. The top of BIOS address
	 * aligns to the last byte of address space, 0xFFFFFFFF. So to obtain
	 * the ROM offset when shadow ROM is used, we subtract the fmap_base
	 * from 4G minus 1.
	 *
	 *  CPU address                  flash address
	 *      space                    p     space
	 *  0xFFFFFFFF   +-------+  ---  +-------+  0x400000
	 *               |       |   ^   |       | ^
	 *               |  4MB  |   |   |       | | from_top
	 *               |       |   v   |       | v
	 *  fmap_base--> | -fmap | ------|--fmap-|-- the offset we need.
	 *       ^       |       |       |       |
	 *       |       +-------+-------+-------+  0x000000
	 *       |       |       |
	 *       |       |       |
	 *       |       |       |
	 *       |       |       |
	 *  0x00000000   +-------+
	 *
	 * We'll use bit 31 to determine if the shadow BIOS area is being used.
	 * This is sort of a hack, but allows us to perform sanity checking for
	 * older x86-based Chrome OS platforms.
	 */

	msg_gdbg("%s: fmap_base: %#x, ROM size: 0x%zx\n",
		 __func__, fmap_base, search->total_size);

	if (fmap_base & (1 << 31)) {
		from_top = 0xFFFFFFFF - fmap_base + 1;
		msg_gdbg("%s: fmap is located in shadow ROM, from_top: %#x\n",
			 __func__, from_top);
		if (from_top > search->total_size)
			return -1;
		*offset = search->total_size - from_top;
	} else {
		msg_gdbg("%s: fmap is located in physical ROM\n", __func__);
		if (fmap_base > search->total_size)
			return -1;
		*offset = fmap_base;
	}

	msg_gdbg("%s: ROM offset: %#jx\n", __func__, (intmax_t)*offset);
	return 0;
}

static int add_fmap_entries_from_buf(const uint8_t *buf)
{
	struct fmap *fmap;
	int i;
	struct flashrom_layout *const layout = get_global_layout();

	fmap = (struct fmap *)(buf);

	for (i = 0; i < fmap->nareas; i++) {
		if (layout->num_entries >= MAX_ROMLAYOUT) {
			msg_gerr("ROM image contains too many regions\n");
			return -1;
		}
		layout->entries[layout->num_entries].start = fmap->areas[i].offset;

		/*
		 * Flashrom rom entries use absolute addresses. So for non-zero
		 * length entries, we need to subtract 1 from offset + size to
		 * determine the end address.
		 */
		layout->entries[layout->num_entries].end = fmap->areas[i].offset +
		                             fmap->areas[i].size;
		if (fmap->areas[i].size)
			layout->entries[layout->num_entries].end--;

		memset(layout->entries[layout->num_entries].name, 0,
		       sizeof(layout->entries[layout->num_entries].name));
		memcpy(layout->entries[layout->num_entries].name, fmap->areas[i].name,
		       min(sizeof(layout->entries[layout->num_entries].name),
		           sizeof(fmap->areas[i].name)));

		layout->entries[layout->num_entries].included = 0;
		strcpy(layout->entries[layout->num_entries].file, "");

		msg_gdbg("added fmap region \"%s\" (file=\"%s\") as %sincluded,"
			 " start: 0x%08x, end: 0x%08x\n",
			  layout->entries[layout->num_entries].name,
			  layout->entries[layout->num_entries].file,
			  layout->entries[layout->num_entries].included ? "" : "not ",
			  layout->entries[layout->num_entries].start,
			  layout->entries[layout->num_entries].end);
		layout->num_entries++;
	}

	return layout->num_entries;
}

enum found_t {
	FOUND_NONE,
	FOUND_FMAP,
};

/* returns the number of entries added, or <0 to indicate error */
static int add_fmap_entries(void *source_handle,
			    size_t image_size,
			    int (*read_chunk)(void *handle,
					      void *dest,
					      size_t offset,
					      size_t size))
{
	static enum found_t found = FOUND_NONE;
	int ret = -1;
	struct search_info search;
	struct fmap fmap_header;
	uint8_t *buf = NULL;
	off_t offset;
	struct flashrom_layout *const layout = get_global_layout();

	if (found != FOUND_NONE) {
		msg_gdbg("Already found fmap entries, not searching again.\n");
		return 0;
	}

	search_init(&search, source_handle,
		    image_size, sizeof(fmap_header), read_chunk);
	search.handler = get_fmap_base;
	while (found == FOUND_NONE && !search_find_next(&search, &offset)) {
		if (search.image) {
			memcpy(&fmap_header, search.image + offset,
			       sizeof(fmap_header));
		} else if (read_chunk(source_handle, (uint8_t *)&fmap_header,
				      offset, sizeof(fmap_header))) {
			msg_gdbg("[L%d] failed to read flash at offset %#jx\n",
				 __LINE__, (intmax_t)offset);
			return -1;
		}
		ret = fmap_find(source_handle, read_chunk,
				&fmap_header, offset, &buf);
		if (ret == 1) {
			found = FOUND_FMAP;
		}
		if (ret < 0)
			return ret;
	}

	switch (found) {
	case FOUND_FMAP:
		layout->num_entries = add_fmap_entries_from_buf(buf);
		break;
	default:
		msg_gdbg("%s: no fmap present\n", __func__);
	}
	if (buf)
		free(buf);
	search_free(&search);

	return layout->num_entries;
}

int get_num_include_args(void) {
  return num_include_args;
}

size_t top_section_offset(void)
{
	size_t top = 0;
	int i;
	struct flashrom_layout *const layout = get_global_layout();

	for (i = 0; i < layout->num_entries; i++) {

		if (!layout->entries[i].included)
			continue;

		if (layout->entries[i].end > top)
			top = layout->entries[i].end;
	}

	return top;
}
/* register an include argument (-i) for later processing */
int register_include_arg(char *name)
{
	if (num_include_args >= MAX_ROMLAYOUT) {
		msg_gerr("too many regions included\n");
		return -1;
	}

	include_args[num_include_args] = name;
	num_include_args++;
	return num_include_args;
}

int find_romentry(char *name)
{
	int i;
	char *file = NULL;
	char *has_colon;
	struct flashrom_layout *const layout = get_global_layout();

	if (!layout->num_entries)
		return -1;

	/* -i <image>[:<file>] */
	has_colon = strchr(name, ':');
	if (strtok(name, ":")) {
		file = strtok(NULL, "");
		if (has_colon && file == NULL) {
			msg_gerr("Missing filename parameter in %s\n", name);
			return -1;
		}
	}
	msg_gdbg("Looking for \"%s\" (file=\"%s\")... ",
	         name, file ? file : "<not specified>");

	for (i = 0; i < layout->num_entries; i++) {
		if (!strcmp(layout->entries[i].name, name)) {
			layout->entries[i].included = 1;
			snprintf(layout->entries[i].file,
			         sizeof(layout->entries[i].file),
			         "%s", file ? file : "");
			msg_gdbg("found.\n");
			return i;
		}
	}
	msg_gdbg("not found.\n");	// Not found. Error.

	return -1;
}


int fill_romentry(struct romentry *entry, int n)
{
	if (!entry)
		return 1;
	struct flashrom_layout *const layout = get_global_layout();

	memcpy(entry, &layout->entries[n], sizeof(*entry));
	return 0;
}

/*
 * num_include_files - count filenames used with -i args
 *
 * This function is intended to help command syntax parser determine if
 * operations such as read and write require a file as an argument. This can
 * be used with get_num_include_args() to determine if all -i args have
 * filenames.
 *
 * returns number of filenames supplied with -i args
 */
int num_include_files(void)
{
	int i, count = 0;

	for (i = 0; i < get_num_include_args(); i++) {
		if (strchr(include_args[i], ':'))
			count++;
	}

	return count;
}

/*
 * process_include_args - process -i arguments
 *
 * returns 0 to indicate success, <0 to indicate failure
 */
int process_include_args() {
	int i;
	struct flashrom_layout *const layout = get_global_layout();

	for (i = 0; i < num_include_args; i++) {
		if (include_args[i]) {
			/* User has specified the area name, but no layout file
			 * is loaded, and no fmap is stored in BIOS.
			 * Return error. */
			if (!layout->num_entries) {
				msg_gerr("No layout info is available.\n");
				return -1;
			}

			if (find_romentry(include_args[i]) < 0) {
				msg_gerr("Invalid entry specified: %s\n",
				         include_args[i]);
				return -1;
			}
		} else {
			break;
		}
	}

	return 0;
}

void layout_cleanup(void)
{
	int i;
	for (i = 0; i < num_include_args; i++) {
		free(include_args[i]);
		include_args[i] = NULL;
	}
	num_include_args = 0;

	struct flashrom_layout *const layout = get_global_layout();
	for (i = 0; i < layout->num_entries; i++) {
		layout->entries[i].included = 0;
	}
	layout->num_entries = 0;
}

/* returns boolean 1 if regions overlap, 0 otherwise */
int included_regions_overlap()
{
	int i;
	int overlap_detected = 0;
	struct flashrom_layout *const layout = get_global_layout();

	for (i = 0; i < layout->num_entries; i++) {
		int j;

		if (!layout->entries[i].included)
			continue;

		for (j = 0; j < layout->num_entries; j++) {
			if (!layout->entries[j].included)
				continue;

			if (i == j)
				continue;

			if (layout->entries[i].start > layout->entries[j].end)
				continue;

			if (layout->entries[i].end < layout->entries[j].start)
				continue;

			msg_gdbg("Regions %s [0x%08x-0x%08x] and "
				"%s [0x%08x-0x%08x] overlap\n",
				layout->entries[i].name, layout->entries[i].start,
				layout->entries[i].end, layout->entries[j].name,
				layout->entries[j].start, layout->entries[j].end);
			overlap_detected = 1;
			goto out;
		}

	}
out:
	return overlap_detected;
}

static int read_content_from_file(struct romentry *entry, uint8_t *newcontents) {
	char *file;
	FILE *fp;
	int len;

	/* If file name is specified for this partition, read file
	 * content to overwrite. */
	file = entry->file;
	len = entry->end - entry->start + 1;
	if (file[0]) {
		int numbytes;
		struct stat s;

		if (stat(file, &s) < 0) {
			msg_gerr("Cannot stat file %s: %s.\n",
					file, strerror(errno));
			return -1;
		}

		if (s.st_size > len) {
			msg_gerr("File %s is %d bytes, region %s is %d bytes.\n"
				 , file, (int)s.st_size,
				 entry->name, len);
			return -1;
		}

		if ((fp = fopen(file, "rb")) == NULL) {
			perror(file);
			return -1;
		}
		numbytes = fread(newcontents + entry->start,
		                 1, s.st_size, fp);
		fclose(fp);
		if (numbytes == -1) {
			perror(file);
			return -1;
		}
	}
	return 0;
}

static struct romentry *get_next_included_romentry(unsigned int start)
{
	int i;
	unsigned int best_start = UINT_MAX;
	struct romentry *best_entry = NULL;
	struct romentry *cur;
	struct flashrom_layout *const layout = get_global_layout();

	/* First come, first serve for overlapping regions. */
	for (i = 0; i < layout->num_entries; i++) {
		cur = &layout->entries[i];
		if (!cur->included)
			continue;
		/* Already past the current entry? */
		if (start > cur->end)
			continue;
		/* Inside the current entry? */
		if (start >= cur->start)
			return cur;
		/* Entry begins after start. */
		if (best_start > cur->start) {
			best_start = cur->start;
			best_entry = cur;
		}
	}
	return best_entry;
}

/* Validate and - if needed - normalize layout entries. */
int normalize_romentries(const struct flashctx *flash)
{
	chipsize_t total_size = flash->chip->total_size * 1024;
	int ret = 0;
	struct flashrom_layout *const layout = get_global_layout();

	int i;
	for (i = 0; i < layout->num_entries; i++) {
		struct romentry *entry = &layout->entries[i];
		if (entry->start >= total_size || entry->end >= total_size) {
			msg_gerr("Warning: Address range of region \"%s\" exceeds the current chip's "
				 "address space.\n", entry->name);
			if (!entry->included)
				ret = 1;
		}
		if (entry->start > entry->end) {
			msg_gerr("Layout entry \"%s\" has an invalid range.\n",
						entry->name);
			ret = 1;
		}
	}

	return ret;
}

int build_new_image(const struct flashctx *flash, uint8_t *oldcontents,
		      uint8_t *newcontents, int erase_mode)
{
	unsigned int start = 0;
	struct romentry *entry;
	unsigned int size = flash->chip->total_size * 1024;

	/* If no regions were specified for inclusion, assume
	 * that the user wants to write the complete new image.
	 */
	if (num_include_args == 0)
		return 0;

	/* Non-included romentries are ignored.
	 * The union of all included romentries is used from the new image.
	 */
	while (start < size) {
		entry = get_next_included_romentry(start);
		/* No more romentries for remaining region? */
		if (!entry) {
			memcpy(newcontents + start, oldcontents + start,
			       size - start);
			break;
		}

		/* For non-included region, copy from old content. */
		if (entry->start > start)
			memcpy(newcontents + start, oldcontents + start,
			       entry->start - start);

		if (!erase_mode) {
			/* For included region, copy from file if specified. */
			if (read_content_from_file(entry, newcontents) < 0)
				return -1;
		}

		/* Skip to location after current romentry. */
		start = entry->end + 1;
		/* Catch overflow. */
		if (!start)
			break;
	}
	return 0;
}
static int write_content_to_file(struct romentry *entry, uint8_t *buf) {
	char *file;
	FILE *fp;
	int len = entry->end - entry->start + 1;

	file = entry->file;
	if (file[0]) {  /* save to file if name is specified. */
		int numbytes;
		if ((fp = fopen(file, "wb")) == NULL) {
			perror(file);
			return -1;
		}
		numbytes = fwrite(buf + entry->start, 1, len, fp);
		fclose(fp);
		if (numbytes != len) {
			perror(file);
			return -1;
		}
	}
	return 0;
}

/* sets required_erase_size, returns 0 if successful */
static int set_required_erase_size(struct flashctx *flash)
{
	int i, erase_size_found = 0;

	/*
	 * Find eraseable block size for read alignment.
	 * FIXME: This assumes the smallest block erase size is useable
	 * by erase_and_write_flash().
	 */
	required_erase_size = ~0;
	for (i = 0; i < NUM_ERASEFUNCTIONS; i++) {
		struct block_eraser eraser = flash->chip->block_erasers[i];
		int j;

		for (j = 0; j < NUM_ERASEREGIONS; j++) {
			unsigned int size = eraser.eraseblocks[j].size;

			if (size && (size < required_erase_size)) {
				required_erase_size = size;
				erase_size_found = 1;
			}
		}
	}

	/* likely an error in flashchips[] */
	if (!erase_size_found) {
		msg_cerr("%s: No usable erase size found.\n", __func__);
		return -1;
	}

	return 0;
}

/*  Reads flash content specified with -i argument into *buf. */
int handle_partial_read(
    struct flashctx *flash,
    uint8_t *buf,
    int (*read) (struct flashctx *flash, uint8_t *buf,
                 unsigned int start, unsigned int len),
    int write_to_file) {
	int i, count = 0;

	/* If no regions were specified for inclusion, assume
	 * that the user wants to read the complete image.
	 */
	if (num_include_args == 0)
		return 0;

	if (set_required_erase_size(flash))
		return -1;

	struct flashrom_layout *const layout = get_global_layout();
	for (i = 0; i < layout->num_entries; i++) {
		unsigned int start, len, start_align, len_align;

		if (!layout->entries[i].included)
			continue;

		/* round down to nearest eraseable block boundary */
		start_align = layout->entries[i].start % required_erase_size;
		start = layout->entries[i].start - start_align;

		/* round up to nearest eraseable block boundary */
		len = layout->entries[i].end - start + 1;
		len_align = len % required_erase_size;
		if (len_align)
			len = len + required_erase_size - len_align;

		if (start_align || len_align) {
			msg_gdbg("\n%s: Re-aligned partial read due to "
				"eraseable block size requirement:\n"
				"\tlayout->entries[%d].start: 0x%06x, len: 0x%06x, "
				"aligned start: 0x%06x, len: 0x%06x\n",
				__func__, i, layout->entries[i].start,
				layout->entries[i].end - layout->entries[i].start + 1,
				start, len);
		}

		if (read(flash, buf + start, start, len)) {
			msg_perr("flash partial read failed.");
			return -1;
		}

		/* If file is specified, write this partition to file. */
		if (write_to_file) {
			if (write_content_to_file(&layout->entries[i], buf) < 0)
				return -1;
		}

		count++;
	}
	return count;
}

/* Instead of verifying the whole chip, this functions only verifies those
 * content in specified partitions (-i).
 */
int handle_partial_verify(
    struct flashctx *flash,
    uint8_t *buf,
    int (*verify) (struct flashctx *flash, uint8_t *buf,
                   unsigned int start, unsigned int len, const char *message)) {
	int i;

	/* If no regions were specified for inclusion, assume
	 * that the user wants to read the complete image.
	 */
	if (num_include_args == 0)
		return 0;

	if (set_required_erase_size(flash))
		return -1;

	struct flashrom_layout *const layout = get_global_layout();
	/* Walk through the table and write content to file for those included
	 * partition. */
	for (i = 0; i < layout->num_entries; i++) {
		unsigned int start, len, start_align, len_align;

		if (!layout->entries[i].included)
			continue;

		/* round down to nearest eraseable block boundary */
		start_align = layout->entries[i].start % required_erase_size;
		start = layout->entries[i].start - start_align;

		/* round up to nearest eraseable block boundary */
		len = layout->entries[i].end - start + 1;
		len_align = len % required_erase_size;
		if (len_align)
			len = len + required_erase_size - len_align;

		if (start_align || len_align) {
			msg_gdbg("\n%s: Re-aligned partial verify due to "
				"eraseable block size requirement:\n"
				"\tlayout->entries[%d].start: 0x%06x, len: 0x%06x, "
				"aligned start: 0x%06x, len: 0x%06x\n",
				__func__, i, layout->entries[i].start,
				layout->entries[i].end -
				layout->entries[i].start + 1,
				start, len);
		}

		/* read content from flash. */
		if (verify(flash, buf + start, start, len, NULL)) {
			msg_perr("flash partial verify failed.");
			return -1;
		}
	}

	return 0;
}

int extract_regions(struct flashctx *flash)
{
	unsigned long size = flash->chip->total_size * 1024;
	unsigned char *buf = calloc(size, sizeof(char));
	int i, ret = 0;

	if (!buf) {
		msg_gerr("Memory allocation failed!\n");
		msg_cinfo("FAILED.\n");
		return 1;
	}

	msg_cinfo("Reading flash... ");
	if (read_flash(flash, buf, 0, size)) {
		msg_cerr("Read operation failed!\n");
		ret = 1;
		goto out_free;
	}

	struct flashrom_layout *const layout = get_global_layout();
	msg_gdbg("Extracting %zd images\n", layout->num_entries);
	for (i = 0; !ret && i < layout->num_entries; i++) {
		struct romentry *region = &layout->entries[i];
		char fname[256];
		char *from, *to;
		unsigned long region_size;

		for (to = fname, from = region->name; *from; from++, to++) {
			if (*from == ' ')
				*to = '_';
			else
				*to = *from;
		}
		*to = '\0';

		msg_gdbg("dumping region %s to %s\n", region->name,
			fname);
		region_size = region->end - region->start + 1;
		ret = write_buf_to_file(buf + region->start, region_size,
					fname);
	}

out_free:
	free(buf);
	if (ret)
		msg_cerr("FAILED.");
	else
		msg_cdbg("done.");
	return ret;
}

/*
 * read_chunk() callback used when reading contents from a file.
 */
static int read_from_file(void *fhandle,
			  void *dest,
			  size_t offset,
			  size_t size)
{
	FILE *handle = fhandle;

	if (fseek(handle, offset, SEEK_SET)) {
		msg_cerr("%s failed to seek to position %zd\n",
			 __func__, offset);
		return 1;
	}

	if (fread(dest, 1, size, handle) != size) {
		msg_cerr("%s failed to read %zd bytes\n",
			 __func__, offset);
		return 1;
	}

	return 0;
}

/*
 * read_chunk() callback used when reading contents from the flash device.
 */
static int read_from_flash(void *handle,
			   void *dest,
			   size_t offset,
			   size_t size)
{
	struct flashctx *flash = handle;

	return read_flash(flash, dest, offset, size);
}

int get_fmap_entries(const char *filename, struct flashctx *flash)
{
	int rv;
	size_t image_size = flash->chip->total_size * 1024;

	/* Let's try retrieving entries from file. */
	if (filename) {
		FILE *handle;

		handle = fopen(filename, "r");
		if (handle) {
			rv = add_fmap_entries(handle,
					      image_size, read_from_file);
			fclose(handle);
			if (rv > 0)
				return rv;
			msg_cerr("No fmap entries found in %s\n", filename);
		}
	}

	return add_fmap_entries(flash, image_size, read_from_flash);
}

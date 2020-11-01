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

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>

#include "flash.h"
#include "layout.h"
#include "platform.h"
#include "programmer.h"

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
int read_romlayout(const char *name)
{
	struct flashrom_layout *const layout = get_global_layout();
	FILE *romlayout;
	char tempstr[256], tempname[256];
	unsigned int i;
	int ret = 1;

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
				 "file reached.\n", MAX_ROMLAYOUT);
			goto _close_ret;
		}
		if (2 != fscanf(romlayout, "%255s %255s\n", tempstr, tempname))
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
			msg_gerr("Error parsing layout file. Offending string: \"%s\"\n", tempstr);
			goto _close_ret;
		}
		layout->entries[layout->num_entries].start = strtol(tstr1, (char **)NULL, 16);
		layout->entries[layout->num_entries].end = strtol(tstr2, (char **)NULL, 16);
		layout->entries[layout->num_entries].included = 0;
		layout->entries[layout->num_entries].name = strdup(tempname);
		if (!layout->entries[layout->num_entries].name) {
			msg_gerr("Error adding layout entry: %s\n", strerror(errno));
			goto _close_ret;
		}
		layout->num_entries++;
	}

	for (i = 0; i < layout->num_entries; i++) {
		msg_gdbg("romlayout %08x - %08x named %s\n",
			     layout->entries[i].start,
			     layout->entries[i].end, layout->entries[i].name);
	}

	ret = 0;

_close_ret:
	(void)fclose(romlayout);
	return ret;
}
#endif

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

int flashrom_layout_include_region(struct flashrom_layout *const layout, const char *name, char *file);

/* returns -1 if an entry is not found, i if found. */
int find_romentry(struct flashrom_layout *const l, char *name)
{
	int ret;
	char *file = NULL;
	char *has_colon;

	if (l->num_entries == 0)
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

	msg_gspew("Looking for region \"%s\"... ", name);
	ret = flashrom_layout_include_region(l, name, file);
	if (ret < 0) {
		msg_gspew("not found.\n");
		return -1;
	}
	msg_gspew("found.\n");
	return ret;
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

			if (find_romentry(layout, include_args[i]) < 0) {
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
		free(layout->entries[i].name);
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
	struct flashrom_layout *const layout = get_global_layout();
	chipsize_t total_size = flash->chip->total_size * 1024;
	int ret = 0;

	unsigned int i;
	for (i = 0; i < layout->num_entries; i++) {
		if (layout->entries[i].start >= total_size || layout->entries[i].end >= total_size) {
			msg_gwarn("Warning: Address range of region \"%s\" exceeds the current chip's "
				  "address space.\n", layout->entries[i].name);
			if (layout->entries[i].included)
				ret = 1;
		}
		if (layout->entries[i].start > layout->entries[i].end) {
			msg_gerr("Error: Size of the address range of region \"%s\" is not positive.\n",
				  layout->entries[i].name);
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

/**
 * @addtogroup flashrom-layout
 * @{
 */

/**
 * @brief Mark given region as included.
 *
 * @param layout The layout to alter.
 * @param name   The name of the region to include.
 *
 * @return i on success,
 *         -1 if the given name can't be found.
 */
int flashrom_layout_include_region(struct flashrom_layout *const layout, const char *name, char *file)
{
	size_t i;
	for (i = 0; i < layout->num_entries; ++i) {
		if (!strcmp(layout->entries[i].name, name)) {
			layout->entries[i].included = true;
			snprintf(layout->entries[i].file,
				 sizeof(layout->entries[i].file),
				 "%s", file ? file : "");
			return i;
		}
	}
	return -1;
}

/** @} */ /* end flashrom-layout */

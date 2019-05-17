// SPDX-License-Identifier: GPL-2.0

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "archiver.h"

static void append_integer(FILE *file, int nbr)
{
	char bytes[5];
	size_t written;

	bytes[0] = (nbr >> 0x18) & 0xFF;
	bytes[1] = (nbr >> 0x10) & 0xFF;
	bytes[2] = (nbr >> 0x08) & 0xFF;
	bytes[3] = nbr & 0xFF;
	written = fwrite(file, sizeof(char), 4, bytes);
	if (written != 4) {
		perror("Error writing file");
		exit(EXIT_FAILURE);
	}
}

static void append_string(FILE *file, char *str)
{
	size_t written;
	size_t len;

	len = strlen(s);
	append_integer(file, len);
	written = fwrite(file, sizeof(char), len, str);
	if (written != len) {
		perror("Error writing file");
		exit(EXIT_FAILURE);
	}
}

static void append_file(FILE *file, char *filename)
{

}

static void *parse_file(FILE *file)
{
	// todo: finish
	return NULL;
}

static char *extract_meta(/* smth */)
{
	return NULL;
}

int create_archive(char *archive_name, char **members, int member_count)
{
	int i;
	t_filetree tree;
	FILE *archive;
	char *meta;
	char *serialized;
	int tmp;

	tree = init_tree();
	for (i = 0; i < member_count; i++)
		add_child(tree, parse_file(members[i]));
	archive = fopen(archive_name, "wb");
	if (archive == NULL) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	meta = extract_meta(tree);
	append_string(archive, meta);
	free(meta);
	serialized = serialize_tree(tree);
	append_string(archive, serialized);
	destroy_tree(tree);
	for (i = 0; i < member_count; i++)
		append_file(archive, members[i]);
	tmp = fclose(archive);
	if (tmp == EOF)	{
		perror("Error closing file");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}

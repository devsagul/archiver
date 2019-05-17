// SPDX-License-Identifier: GPL-2.0

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "archiver.h"

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
	append_integer(archive, strlen(meta));
	append_string(archive, meta);
	serialized = serialize_tree(tree);
	append_ineger(archive, strlen(serialized));
	append_string(archive, serialized);
	destroy_tree(tree);
	for (i = 0; i < member_count; i++) {
		tmp = append_file(archive, members[i]);
		if (tmp == EOF) {
			perror("Error adding file to the tree");
			exit(EXIT_FAILURE);
		}
	}
	tmp = fclose(archive);
	if (tmp == EOF)	{
		perror("Error closing file");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}

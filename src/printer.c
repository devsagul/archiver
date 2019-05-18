// SPDX-License-Identifier: GPL-2.0

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "smartstr.h"
#include "tree.h"
#include "archiver.h"

static void			print_info(t_fileinfo *meta, t_tree *tree)
{
	unsigned long	id;
	size_t		i;

	id = tree->value;
	if (id != 0) {
		char	type;

		type = S_ISDIR(meta[id - 1].mode) ? 'd' : 'f';
		printf("%lu;%s;%lu;%c;%jd\n", id, meta[id - 1].name,
		       tree->parent->value, type, meta[id - 1].size);
	}
	for (i = 0; i < tree->children_count; i++)
		print_info(meta, tree->children[i]);
}

int					print_table(char *archive_name)
{
	FILE			*archive;
	unsigned long	count;
	t_fileinfo		*meta;
	size_t			tree_size;
	t_smartstr		*sstr;
	char			*str;
	t_tree			*tree;
	size_t			i;

	archive = fopen(archive_name, "rb");
	if (!archive) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	fread(&count, sizeof(unsigned long), 1, archive);
	printf("Number of files in the archive: %lu\n", count);
	meta = malloc(sizeof(t_fileinfo) * count);
	for (i = 0; i < count; i++) {
		fread(&meta[i].owner, sizeof(uid_t), 1, archive);
		fread(&meta[i].group, sizeof(gid_t), 1, archive);
		fread(&meta[i].size, sizeof(off_t), 1, archive);
		fread(&meta[i].mode, sizeof(mode_t), 1, archive);
		fread(meta[i].name, sizeof(char), 255, archive);
	}
	fread(&tree_size, sizeof(size_t), 1, archive);
	str = malloc(sizeof(char) * tree_size);
	fread(str, sizeof(char), tree_size, archive);
	fclose(archive);
	sstr = init_smartstr();
	append_bytes(sstr, str, tree_size);
	free(str);
	tree = deserialize_tree(sstr);
	delete_smartstr(sstr);
	printf("id;name;parent;type;size\n");
	print_info(meta, tree);
	return 0;
}

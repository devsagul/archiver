// SPDX-License-Identifier: GPL-2.0

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "smartstr.h"
#include "tree.h"
#include "archiver.h"

static void	extract_tree(t_tree *tree, t_fileinfo *meta, FILE *src)
{
	unsigned long	id;
	FILE		*f;
	char		*filename;
	off_t		size;
	size_t		buff_size;
	char		buffer[BUFFSIZE];
	size_t		i;

	id = tree->value;
	if (S_ISDIR(meta[id - 1].mode)) {
		filename = meta[id - 1].name;
		chown(filename, meta[id - 1].owner, meta[id-1].group);
		mkdir(filename, meta[id - 1].mode);
		chdir(filename);
		for (i = 0; i < tree->children_count; i++) {
			extract_tree(tree->children[i], meta, src);
		}
		chdir("..");
	} else {
		// todo error handling
		filename = meta[id - 1].name;
		f = fopen(filename, "wb");
		chown(filename, meta[id - 1].owner, meta[id-1].group);
		chmod(filename, meta[id - 1].mode);
		size = meta[id - 1].size;
		while((buff_size = size > BUFFSIZE ? BUFFSIZE : size) != 0) {
			size -= buff_size;
			fread(buffer, sizeof(char), buff_size, src);
			fwrite(buffer, sizeof(char), buff_size, f);
		}
		fclose(f);
	}
}

int	extract_archive(char *archive_name)
{
	FILE *archive;
	unsigned long	count;
	t_fileinfo	*meta;
	size_t		tree_size;
	t_smartstr	*sstr;
	char		*str;
	t_tree		*tree;
	size_t		i;

	archive = fopen(archive_name, "rb");
	fread(&count, sizeof(unsigned long), 1, archive);
	meta = malloc(sizeof(t_fileinfo) * count);
	for (i = 0; i < count; i++) {
		fread(&(meta[i].owner), sizeof(uid_t), 1, archive);
		fread(&(meta[i].group), sizeof(gid_t), 1, archive);
		fread(&(meta[i].size), sizeof(off_t), 1, archive);
		fread(&(meta[i].mode), sizeof(mode_t), 1, archive);
		fread(meta[i].name, sizeof(char), 255, archive);
	}
	fread(&tree_size, sizeof(size_t), 1, archive);
	str = malloc(sizeof(char) * tree_size);
	fread(str, sizeof(char), tree_size, archive);
	sstr = init_smartstr();
	append_bytes(sstr, str, tree_size);
	free(str);
	tree = deserialize_tree(sstr);
	delete_smartstr(sstr);
	for (i = 0; i < tree->children_count; i++) {
		extract_tree(tree->children[i], meta, archive);
	}
	return EXIT_SUCCESS;
}

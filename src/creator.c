// SPDX-License-Identifier: GPL-2.0

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include "tree.h"
#include "archiver.h"

static void		add_arc_mem(char *filename,
				    t_tree * tree,
				    t_fileinfo * *meta,
				    unsigned long *id);

static void		add_arc_dir(char *filename, t_tree *tree,
				    t_fileinfo **meta,
				    unsigned long *id)
{
	t_tree		*child;
	int		tmp;
	DIR		*dir;
	struct dirent	*entry;

	child = insert_child(tree, *id);
	if (!child || !meta) {
		perror("Error allocating memory");
		exit(EXIT_FAILURE);
	}
	dir = opendir(filename);
	tmp = chdir(filename);
	while ((entry = readdir(dir))) {
		if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
			add_arc_mem(entry->d_name, child, meta, id);
	}
	closedir(dir);
	tmp = chdir("..");
	if (tmp == -1) {
		perror("Error accessing file");
		exit(EXIT_FAILURE);
	}
}

static void		add_arc_file(t_tree *tree,
				     t_fileinfo **meta,
				     unsigned long *id)
{
	t_tree		*child;

	child = insert_child(tree, *id);
	if (!child || !meta) {
		perror("Error allocating memory");
		exit(EXIT_FAILURE);
	}
}

static void		add_arc_mem(char *filename,
				    t_tree *tree,
				    t_fileinfo **meta,
				    unsigned long *id)
{
	struct stat	filestat;
	// todo: add error checking
	(*id)++;
	*meta = realloc(*meta, sizeof(t_fileinfo) * (*id));
	stat(filename, &filestat);
	(*meta)[*id - 1].owner = filestat.st_uid;
	(*meta)[*id - 1].group = filestat.st_gid;
	(*meta)[*id - 1].size = filestat.st_size;
	(*meta)[*id - 1].mode = filestat.st_mode;
	strcpy((*meta)[*id - 1].name, basename(filename));
	if (S_ISDIR(filestat.st_mode))
		add_arc_dir(filename, tree, meta, id);
	else
		add_arc_file(tree, meta, id);
}

void				check_writing_error(size_t res)
{
	if (res == (size_t)EOF) {
		perror("Error writing file");
		exit(EXIT_FAILURE);
	}
}

size_t			push_meta(FILE *archive,
				  t_fileinfo *meta,
				  unsigned int count)
{
	unsigned int	i;
	size_t		written;

	// todo error handling
	for (i = 0; i < count; i++) {
		written = fwrite(&meta[i].owner, sizeof(uid_t), 1, archive);
		written = fwrite(&meta[i].group, sizeof(gid_t), 1, archive);
		written = fwrite(&meta[i].size, sizeof(off_t), 1, archive);
		written = fwrite(&meta[i].mode, sizeof(mode_t), 1, archive);
		written = fwrite(meta[i].name, sizeof(char), 255, archive);
	}
	return written;
}

size_t			push_tree(FILE *archive, t_tree *tree)
{
	t_smartstr	*sstr;
	size_t		tmp;

	sstr = serialize_tree(tree);
	tmp = fwrite(&sstr->actual_size, sizeof(size_t), 1, archive);
	if (tmp == (size_t)EOF) {
		perror("Error writing file");
		exit(EXIT_FAILURE);
	}
	tmp = fwrite(sstr->str, sizeof(char), sstr->actual_size, archive);
	if (tmp == (size_t)EOF) {
		perror("Error writing file");
		exit(EXIT_FAILURE);
	}
	return 0;
}

size_t			push_files(FILE *archive,
				   t_tree *tree,
				   t_fileinfo *meta)
{
	char		buffer[BUFFSIZE];
	size_t		i;
	size_t		size_read;
	FILE		*f;

	if (tree->value == 0) {
		for (i = 0; i < tree->children_count; i++)
			push_files(archive, tree->children[i], meta);
	} else if (tree->children_count == 0) {
		f = fopen(meta[tree->value - 1].name, "rb");
		while ((size_read = fread(buffer,
					  sizeof(char),
					  BUFFSIZE, f)) != 0) {
			fwrite(buffer, sizeof(char), size_read, archive);
		}
		fclose(f);
	} else {
		// todo error handling
		chdir(meta[tree->value - 1].name);
		for (i = 0; i < tree->children_count; i++)
			push_files(archive, tree->children[i], meta);
		chdir("..");
	}
	return 0;
}

int			create_archive(char *archive_name,
				       char **members, int member_count)
{
	FILE		*archive;
	t_fileinfo	*meta;
	unsigned long	id;
	t_tree		*tree;
	int		i;
	size_t		tmp;

	archive = fopen(archive_name, "wb");
	if (!archive) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	meta = NULL;
	id = 0;
	tree = init_tree(id);
	for (i = 0; i < member_count; i++)
		add_arc_mem(members[i], tree, &meta, &id);
	tmp = fwrite(&id, sizeof(unsigned long), 1, archive);
	if (tmp != 1) {
		perror("Error writing file");
		exit(EXIT_FAILURE);
	}
	// todo: error checking
	push_meta(archive, meta, id);
	push_tree(archive, tree);
	push_files(archive, tree, meta);
	return EXIT_SUCCESS;
}

/* SPDX-License-Identifier: GPL-2.0 */

#ifndef ARCHIVER_H
# define ARCHIVER_H
# include <sys/types.h>

# define NAMELEN 256
# define BUFFSIZE 4096

typedef struct	s_fileinfo {
	uid_t		owner;
	gid_t		group;
	off_t		size;
	mode_t		mode;
	char		name[NAMELEN];
}		t_fileinfo;

int	print_table(char *archive_name);
int	extract_archive(char *archive_name);
int	create_archive(char *archive_name, char **members, int member_count);

# endif

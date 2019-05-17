/* SPDX-License-Identifier: GPL-2.0 */

#ifndef ARCHIVER_H
# define ARCHIVER_H

int print_table(char *archive_name);
int extract_archive(char *archive_name);
int create_archive(char *archive_name, char **members, int member_count);

# endif

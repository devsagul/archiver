// SPDX-License-Identifier: GPL-2.0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "archiver.h"

int			print_usage(char *name)
{
	printf("Usage: %s [operation] archive [member...]", name);
	printf("mods:\nt -- prints contents table of the given arhive.\n");
	printf("c -- creates archive from given members.\n");
	printf("x -- extracts contents of the archive ");
	printf("into current directory.\n");
	return EXIT_SUCCESS;
}

void		exit_alt(char *error1, char *error2, int condition)
{
	fprintf(stderr, "%s\n", condition ? error1 : error2);
	exit(EXIT_FAILURE);
}

int			main(int argc, char **argv)
{
	char	*mod;

	if (argc == 1)
		return print_usage(argv[0]);
	mod = argv[1];
	if (!strcmp(mod, "t") || !strcmp(mod, "x")) {
		if (argc != 3)
			exit_alt("No archive provided",
				 "Too many arguments",
				 argc == 2);
		return !strcmp(mod, "t") ?
			print_table(argv[2]) :
			extract_archive(argv[2]);
	}
	if (!strcmp(mod, "c")) {
		if (argc < 4)
			exit_alt("No archive name provided",
				 "No archive member(s) provided)",
				 argc == 2);
		return create_archive(argv[2], argv + 3, argc - 3);
	}
	fprintf(stderr, "Unknown operation\n");
	return EXIT_FAILURE;
}

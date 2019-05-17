#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void print_usage(char *name)
{
	printf("Usage: %s [operation] archive [member...]", name);
	printf("mods:\nt -- prints contents table of the given arhive.\n");
	printf("c -- creates archive from given members.\n");
	printf("x -- extracts contents of the archive into current directory.\n");
}

int main (int argc, char **argv)
{
	if (argc == 1)
	{
		print_usage(argv[0]);
		return EXIT_SUCCESS;
	}

	option = argv[1]
	if (!strcmp(option, "t"))
	{
		if (argc != 3)
		{
			error_msg = argc == 2 ?
				"No archive provided\n" :
				"To many arguments";
			fprintf(2, "%s\n", error_msg);
			return EXIT_FAILURE;
		}
		print_table(argv[2]);
		return EXIT_SUCCESS;
	}
	if (!strcmp(option. "c"))
	{
		if (argc < 4)
		{
			error_msg = argc == 2 ?
				"No archive name provided" :
				"No arhive member(s) provided)";
			fprintf(2, "%s\n", error_msg);
			return EXIT_FAILURE;
		}
		create_archive(argv[2], argv + 3);
		return EXIT_SUCCESS;
	}
	if (!strcmp(option, "x"))
	{
		if (argc != 3)
		{
			error_msg = argc == 2 ?
				"No archive provided\n" :
				"To many arguments";
			fprintf(2, "%s\n", error_msg);
			return EXIT_FAILURE;
		}
		extract_archive(argv[2]);
		return EXIT_SUCCESS;
	}
	fprintf(2, "Unknown operation\n");
	return EXIT_FAILURE;
}

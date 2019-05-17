// SPDX-License-Identifier: GPL-2.0

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smartstr.h"
#include "tree.h"

int	main(void)
{
	t_tree	*tree;
	t_tree	*tree_2;
	t_tree	*tmp;
	t_smartstr *sstr;

	tree = init_tree(0);
	insert_child(tree, 1);
	insert_child(tree, 2);
	tmp = tree->children[0];
	insert_child(tmp, 3);
	sstr = serialize_tree(tree);
	tree_2 = deserialize_tree(sstr);
	if (tree_2->value != 0 || tree_2->children_count != 2 || 
	    tree_2->children[0]->value != 1 || tree_2->children[0]->children_count != 1 ||
	    tree_2->children[1]->value != 2 || tree_2->children[1]->children_count != 0 ||
	    tree_2->children[0]->children[0]->value !=3 ||
	    tree_2->children[0]->children[0]->children_count != 0) {
		printf("%s\n", "Test failed");
		printf("Tree root value; %lu\n", tree_2->value);
		printf("Children count root value; %lu\n", tree_2->children_count);
		printf("Tree children 0 value; %lu\n", tree_2->children[0]->value);
		printf("Children count children 0 value; %lu\n",
				tree_2->children[0]->children_count);
		printf("Tree children 1 value; %lu\n", tree_2->children[1]->value);
		printf("Children count children 1 value; %lu\n",
				tree_2->children[1]->children_count);
		printf("Tree children 0 children 0 value; %lu\n",
				tree_2->children[0]->children[0]->value);
		printf("Children count children 0 children 0 value; %lu\n",
				tree_2->children[0]->children[0]->children_count);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

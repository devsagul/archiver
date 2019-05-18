// SPDX-License-Identifier: GPL-2.0

#include <stdlib.h>
#include "smartstr.h"
#include "tree.h"

t_tree				*init_tree(unsigned long value)
{
	t_tree			*res;

	res = (t_tree *)malloc(sizeof(t_tree));
	if (!res)
		return NULL;
	res->value = value;
	res->parent = NULL;
	res->children_count = 0;
	res->children = NULL;
	return res;
}

static t_tree			*insert_child_t(t_tree *tree, t_tree *child)
{
	t_tree			**tmp;

	tmp = realloc(tree->children, sizeof(t_tree *) *
				      (tree->children_count + 1));
	if (!tmp)
		return NULL;
	tree->children = tmp;
	tree->children[tree->children_count] = child;
	tree->children_count++;
	child->parent = tree;
	return child;
}

t_tree				*insert_child(t_tree *tree, unsigned long value)
{
	t_tree			*tmp;

	tmp = init_tree(value);
	return insert_child_t(tree, tmp);
}

void				destroy_tree(t_tree *tree)
{
	size_t			i;

	for (i = 0; i < tree->children_count; i++)
		destroy_tree(tree->children[i]);
	if (tree->children)
		free(tree->children);
	free(tree);
}

t_smartstr			*serialize_tree(t_tree *tree)
{
	t_smartstr		*res;
	size_t			i;

	res = init_smartstr();
	append_ul(res, tree->value);
	for (i = 0; i < tree->children_count; i++) {
		t_smartstr	*tmp;

		tmp = serialize_tree(tree->children[i]);
		join_smartstrs(res, tmp);
		delete_smartstr(tmp);
	}
	append_str(res, "^");
	return res;
}

t_tree				*deserialize_tree(t_smartstr *serialized)
{
	unsigned long		value;
	t_tree			*res;

	value = get_ul(serialized);
	res = init_tree(value);
	if (!res)
		return NULL;
	while (get_current(serialized) != '^') {
		t_tree		*tmp;

		tmp = insert_child_t(res, deserialize_tree(serialized));
		if (!tmp) {
			destroy_tree(res);
			return NULL;
		}
	}
	smartstr_move(serialized, 1);
	return res;
}

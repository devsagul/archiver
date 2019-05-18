/* SPDX-License-Identifier: GPL-2.0 */

#include <stdlib.h>
#include "smartstr.h"

#ifndef TREE_H
# define TREE_H

typedef struct		s_tree {
	unsigned long	value;
	struct s_tree	*parent;
	size_t		children_count;
	struct s_tree	**children;
}			t_tree;

t_tree			*init_tree(unsigned long value);
t_tree			*insert_child(t_tree *tree, unsigned long value);
void			destroy_tree(t_tree *tree);
t_smartstr		*serialize_tree(t_tree *tree);
t_tree			*deserialize_tree(t_smartstr *serialized);

# endif

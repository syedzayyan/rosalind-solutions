/*
    This is set with Red-Black tree, because I can and want to.
    Code has been "inspired" by the postgres repo and perhaps the Wiki article.
*/

#include "set.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize a new Red-Black Tree
RedBlackTree* rbt_create() {
    RedBlackTree *tree = (RedBlackTree *)malloc(sizeof(RedBlackTree));
    tree->root = NULL;
    tree->node_size = sizeof(RedBlackTreeNode);
    return tree;
}


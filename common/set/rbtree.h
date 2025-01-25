#include <stddef.h> // for size_t

// Binary Tree Node Definition
typedef struct RBTreeNode;

// Function Declarations
RBTreeNode* createRBTreeNode(void* value, size_t data_size, int color);
// BTNode* contains(BTNode* root, void* target, int (*compare)(void*, void*));
RBTreeNode* rbtree_add(RBTreeNode* node, void* value, size_t data_size, int (*compare)(void*, void*));
// RBTreeNode* rbt_rotate_left(RBTreeNode *rbt, RBTreeNode *x);
// RBTreeNode* rbt_rotate_rght(RBTreeNode *rbt, RBTreeNode *x);
// RBTreeNode* rbt_insert_fixup(RBTreeNode *root, RBTreeNode *z);


RBTreeNode* rbt_find(RBTreeNode *root, void *data, int (*compare)(void*, void*));
RBTreeNode* delete_rbt_node(RBTreeNode *root, RBTreeNode *z);



typedef struct RBTreeIterator RBTreeIterator;
void rbt_begin_iterate(RBTreeNode *root, int ctrl, RBTreeIterator *iter);
// void inOrderTraversalPrintString(BTNode* root);
// int compareInt(void* a, void* b);
// int compareString(void* a, void* b);
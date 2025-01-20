typedef struct RedBlackTreeNode{
    int color;              // RBTRED (1) or RBTBLACK (0)
    void *data;
    struct RedBlackTreeNode *left;
    struct RedBlackTreeNode *rght;
    struct RedBlackTreeNode *parn;
} RedBlackTreeNode;

typedef struct RedBlackTree{
    RedBlackTreeNode *root;
    size_t node_size;
} RedBlackTree;

// 1. Initialize and Cleanup
RedBlackTree* rbt_create();
void rbt_destroy(RedBlackTree *tree);
void rbt_destroy_node(RedBlackTreeNode *node);

// 2. Insertion
void rbt_insert(RedBlackTree *tree, int value);
void rbt_insert_fixup(RedBlackTree *tree, RedBlackTreeNode *node);

// 3. Searching
RedBlackTreeNode* rbt_search(RedBlackTree *tree, int value);

// 4. Deletion
void rbt_delete(RedBlackTree *tree, int value);
void rbt_delete_fixup(RedBlackTree *tree, RedBlackTreeNode *node);
RedBlackTreeNode* rbt_transplant(RedBlackTree *tree, RedBlackTreeNode *u, RedBlackTreeNode *v);
RedBlackTreeNode* rbt_minimum(RedBlackTreeNode *node);

// 5. Rotation Functions
void rbt_left_rotate(RedBlackTree *tree, RedBlackTreeNode *x);
void rbt_right_rotate(RedBlackTree *tree, RedBlackTreeNode *x);

// 6. Utility Functions
void rbt_inorder_traversal(RedBlackTreeNode *node);
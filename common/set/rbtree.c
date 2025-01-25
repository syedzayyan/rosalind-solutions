#include "rbtree.h"

typedef struct RBTreeNode {
    void* data;
    int color; // 0 for black and 1 for red
    struct RBTreeNode *left, *right, *prn;
} RBTreeNode;

RBTreeNode* createRBTreeNode(void* value, size_t data_size, int color){
    RBTreeNode* temp = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    if (temp->data == NULL) {
        free(temp); // Free the node if data allocation fails
        return NULL;
    }
    temp->data = malloc(data_size);
    memcpy(temp->data, value, data_size);
    temp->left = temp->right = NULL;
    if (color == 1 || color == 0){
        temp->color = color;
    }
    return temp;
}

/*
    Before        After
      x             y
     / \           / \
    a   y         x   c
       / \       / \
      b   c     a   b

*/

RBTreeNode* rbt_rotate_left(RBTreeNode *root, RBTreeNode *x){
    RBTreeNode *y = x->right;
    x->right = y->left;
    if (y->left != NULL) y->left->prn = x;

    if ( y != NULL ) y->prn = x->prn;

    if (x->prn){
        if (x == x->prn->left) x->prn->left = y;
        else x->prn->right = y;
    } else root = y;

    y->left = x;
    if (x != NULL) x->prn = y;

    return root;   // Return the new root of the tree
}
/*
        Before             After
          x                 y
         / \               / \
        y   c    -->      a   x
       / \                   / \
      a   b                 b   c

*/ 
RBTreeNode* rbt_rotate_rght(RBTreeNode *root, RBTreeNode *x){
    RBTreeNode *y = x->left;
    x->left = y->right;
    if (y->right != NULL) y->right->prn = x;

    if (y != NULL) y->prn = x->prn;

    if (x->prn){
        if (x == x->prn->right) x->prn->right = y;
        else x->prn->left = y;
    } else root = y;

    y->right = x;
    if (x != NULL) x->prn = y;
}
/*
Before:
        G (Black)
       / \
   P (Red)  U (Black)
      \
       z (Red)

Step 1 (Left Rotate P):
        G (Black)
       / \
   z (Red)  U (Black)
     /
  P (Red)

Step 2 (Right Rotate G):
        z (Black)
       / \
    P (Red) G (Red)
               \
                U (Black)
*/
RBTreeNode* rbt_insert_fixup(RBTreeNode *root, RBTreeNode *z) {
    /*
     * z is always a red node. Initially, it is the newly inserted node. Each
     * iteration of this loop moves it higher up in the tree.
     */
    while (z != root && z->prn->color == 1) { // Continue while parent is red
        if (z->prn == z->prn->prn->left) { // Parent is the left child
            RBTreeNode *y = z->prn->prn->right; // Uncle is capitalist
            if (y && y->color == 1) { // Case 1: Uncle exists and is red, ik weird
                z->prn->color = 0;          // Recolor parent to black
                y->color = 0;              // Recolor uncle to black
                z->prn->prn->color = 1;    // Recolor grandparent to red
                z = z->prn->prn;           // Move z up to grandparent
            } else { // Case 2 and 3: Uncle is black
                if (z == z->prn->right) { // Case 2: z is the right child
                    z = z->prn;
                    rbt_rotate_left(root, z); // Left rotation around parent
                }
                // Case 3: z is the left child
                z->prn->color = 0;          // Recolor parent to black
                z->prn->prn->color = 1;    // Recolor grandparent to red
                rbt_rotate_rght(root, z->prn->prn); // Right rotation around grandparent
            }
        } else { // Parent is the right child (mirror case)
            RBTreeNode *y = z->prn->prn->left; // Uncle is communist
            if (y && y->color == 1) { // Case 1: Uncle is red
                z->prn->color = 0;          // Recolor parent to black
                y->color = 0;              // Recolor uncle to black
                z->prn->prn->color = 1;    // Recolor grandparent to red
                z = z->prn->prn;           // Move z up to grandparent
            } else { // Case 2 and 3: Uncle is black
                if (z == z->prn->left) { // Case 2: z is the left child
                    z = z->prn;
                    rbt_rotate_rght(root, z); // Right rotation around parent
                }
                // Case 3: z is the right child
                z->prn->color = 0;          // Recolor parent to black
                z->prn->prn->color = 1;    // Recolor grandparent to red
                rbt_rotate_left(root, z->prn->prn); // Left rotation around grandparent
            }
        }
    }
    root->color = 0; // Ensure root is always black
    return root;
}


RBTreeNode* rbtree_add(RBTreeNode* root, void* value, size_t data_size, int (*compare)(void*, void*)) {
    // Create a new node
    RBTreeNode *z = createRBTreeNode(value, data_size, 1); // New nodes are always red
    if (z == NULL) {
        printf("Error: Memory allocation failed for new node.\n");
        return root;
    }

    // If the tree is empty, return the new node as the root
    if (root == NULL) {
        z->color = 0; // Root is always black
        return z;
    }

    // Find the correct location for the new node
    RBTreeNode *x = root;
    RBTreeNode *y = NULL;

    while (x != NULL) {
        y = x; // Keep track of the parent
        if (compare(value, x->data) < 0) { // Go to the left subtree
            x = x->left;
        } else { // Go to the right subtree
            x = x->right;
        }
    }

    // Attach the new node to the parent
    z->prn = y;
    if (compare(value, y->data) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }

    // Fix red-black tree violations
    root = rbt_insert_fixup(root, z);

    return root; // Return the root of the tree
}
/*
Case 1:
Before:           P (Black)                After:        w (Black)
                 /       \                              /       \
          x (Black)   w (Red)                  P (Red)    wR (Black)
                            /   \               /   \
                  wL (Black) wR (Black)  x (Black) wL (Black)

Case 2:
Before:           P (Black)                After:        P (Red)
                 /       \                              /       \
          x (Black)   w (Black)          x (Black)    w (Red)
                            /   \                          /      \
                  wL (Black) wR (Black)  wL (Black)  wR (Black)

Case 3:
Before:           P (Black)                After:        P (Black)
                 /       \                              /       \
          x (Black)   w (Black)          x (Black)  wL (Black)
                            /   \                                 \
                  wL (Red)   wR (Black)                   w (Red)
                                                                         \
                                                                         wR (Black)
*/


RBTreeNode* rbt_dlt_fixup(RBTreeNode *root, RBTreeNode *x){
    while (x != root && x->color == 0){
    /*
		 * Left and right cases are symmetric.  Any nodes that are children of
		 * x have a black-height one less than the remainder of the nodes in
		 * the tree.  We rotate and recolor nodes to move the problem up the
		 * tree: at some stage we'll either fix the problem, or reach the root
		 * (where the black-height is allowed to decrease).
	*/
        // left case
        if (x == x->prn->left){
            RBTreeNode* w = x->prn->right;
            if (w->color == 1){
                w->color = 0;
                x->prn->color = 1;
                rbt_rotate_left(root, x->prn);
                w = x->prn->right;
            }
            if (w->left->color == 1 && w->right->color == 0){
                w->color = 1;
                x = x->prn;
            }else{
                if (w->right->color == 0){
                    w->left->color = 0;
                    w->color = 1;
                    rbt_rotate_rght(root, w);
                    w = x->prn->right;
                }
                w->color = x->prn->color;
                x->prn->color = 0;
                w->right->color = 0;
                rbt_rotate_left(root, x->prn);
                x = root;
            }
        } else {
            RBTreeNode* w = x->prn->left;
            if (w->color == 1){
                w->color = 0;
                x->prn->color = 1;
                rbt_rotate_rght(root, x->prn);
                w = x->prn->left;
            }

            if (w->right->color == 0 && w->left->color == 0){
                w->color = 1;
                x = x->prn;
            }else{
                if (w->left->color == 0){
                    w->right->color = 0;
                    w->color = 1;
                    rbt_rotate_left(root, w);
                    w = x->prn->left;
                }
                w->color = x->prn->color;
                x->prn->color = 0;
                w->left->color = 0;
                rbt_rotate_rght(root, x->prn);
                x = root;
            }
        }
    }
    if (x) x->color = 0;
    return root;
}

RBTreeNode* delete_rbt_node(RBTreeNode *root, RBTreeNode *z) {
    if (z == NULL) { // Paranoia check
        printf("Node doesn't exist, nothing deleted\n");
        return root;
    }

    RBTreeNode *y, *x;

    // Determine the node to splice out
    if (z->left == NULL || z->right == NULL) {
        // z has at least one NULL child
        y = z;
    } else {
        // Find the in-order successor of z
        y = z->right;
        while (y->left != NULL) {
            y = y->left;
        }
    }

    // x is the only child of y (or NULL if y has no children)
    x = (y->left != NULL) ? y->left : y->right;

    // Remove y from the tree
    if (x != NULL) {
        x->prn = y->prn; // Link x to y's parent
    }
    if (y->prn == NULL) {
        // If y was the root, update the root pointer
        root = x;
    } else if (y == y->prn->left) {
        y->prn->left = x;
    } else {
        y->prn->right = x;
    }

    // If y != z, move y's data to z
    if (y != z) {
        memcpy(z->data, y->data, sizeof(*(z->data))); // Copy the data
    }

    // If y was black, fix the red-black tree
    if (y->color == 0 /* Black */) {
        root = rbt_dlt_fixup(root, x);
    }

    // Free the removed node
    free(y->data);
    free(y);

    return root;
}

RBTreeNode* rbt_find(RBTreeNode *root, void *data, int (*compare)(void*, void*)){
    if (root == NULL || compare == NULL)
        return NULL;
    RBTreeNode *current = root;
    while (current != NULL)
    {
        int cmp = compare(data, current->data);
        if (cmp == 0)
            return current;          // Node found
        else if (cmp < 0)
            current = current->left; // Traverse left
        else
            current = current->right; // Traverse right
    }
    return NULL; // Node not found
}

struct RBTreeIterator
{
    RBTreeNode* root;       // Root of the tree
    RBTreeNode* last_visited; // Last node visited during traversal
    RBTreeNode* (*iterate)(struct RBTreeIterator*); // Function pointer for traversal
    int is_over;          // Flag to indicate traversal completion
};

/**********************************************************************
 *						  Traverse									  *
 **********************************************************************/

static RBTreeNode * rbt_left_right_iterator(RBTreeIterator *iter){
    // First call - start from leftmost node
	if (iter->last_visited == NULL)
	{
		iter->last_visited = iter->root;
		while (iter->last_visited->left != NULL)
			iter->last_visited = iter->root->left;

		return iter->last_visited;
	}
    // If right child exists, move to rightmost node of right subtree
	if (iter->last_visited->right != NULL)
	{
		iter->last_visited = iter->last_visited->right;
		while (iter->last_visited->left != NULL)
			iter->last_visited = iter->last_visited->left;

		return iter->last_visited;
	}

	for (;;)
	{
		RBTreeNode  *came_from = iter->last_visited;
		iter->last_visited = iter->last_visited->prn;
		if (iter->last_visited == NULL)
		{
			iter->is_over = 1;
			break;
		}

		if (iter->last_visited->left == came_from)
			break;				/* came from left sub-tree, return current
								 * node */

		/* else - came from right sub-tree, continue to move up */
	}

	return iter->last_visited;
}

static RBTreeNode *rbt_right_left_iterator(RBTreeIterator *iter)
{
	if (iter->last_visited == NULL)
	{
		iter->last_visited = iter->root;
		while (iter->last_visited->right != NULL)
			iter->last_visited = iter->last_visited->right;

		return iter->last_visited;
	}

	if (iter->last_visited->left != NULL)
	{
		iter->last_visited = iter->last_visited->left;
		while (iter->last_visited->right != NULL)
			iter->last_visited = iter->last_visited->right;

		return iter->last_visited;
	}

	for (;;)
	{
		RBTreeNode *came_from = iter->last_visited;

		iter->last_visited = iter->last_visited->prn;
		if (iter->last_visited == NULL)
		{
			iter->is_over = 1;
			break;
		}

		if (iter->last_visited->right == came_from)
			break;				/* came from right sub-tree, return current
								 * node */

		/* else - came from left sub-tree, continue to move up */
	}

	return iter->last_visited;
}

/*
 * rbt_begin_iterate: prepare to traverse the tree in any of several orders
 *
 * After calling rbt_begin_iterate, call rbt_iterate repeatedly until it
 * returns NULL or the traversal stops being of interest.
 *
 * If the tree is changed during traversal, results of further calls to
 * rbt_iterate are unspecified.  Multiple concurrent iterators on the same
 * tree are allowed.
 *
 * The iterator state is stored in the 'iter' struct.  The caller should
 * treat it as an opaque struct.
 */
void
rbt_begin_iterate(RBTreeNode *root, int ctrl, RBTreeIterator *iter)
{
	/* Common initialization for all traversal orders */
	iter->root = root;
	iter->last_visited = NULL;
	iter->is_over = (root == NULL);

	switch (ctrl)
	{
		case -1:		/* visit left, then self, then right */
			iter->iterate = rbt_left_right_iterator;
			break;
		case 1:		/* visit right, then self, then left */
			iter->iterate = rbt_right_left_iterator;
			break;
		default:
			printf("unrecognized rbtree iteration order: %d", ctrl);
	}
}

/*
 * rbt_iterate: return the next node in traversal order, or NULL if no more
 */
RBTreeNode* rbt_iterate(RBTreeIterator *iter)
{
	if (iter->is_over)
		return NULL;

	return iter->iterate(iter);
}
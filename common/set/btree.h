#include <stddef.h> // for size_t

// Binary Tree Node Definition
typedef struct BTNode {
    void* data;
    struct BTNode *left, *right;
} BTNode;

// Function Declarations
BTNode* createNode(void* value, size_t data_size);
BTNode* contains(BTNode* root, void* target, int (*compare)(void*, void*));
BTNode* add(BTNode* node, void* value, size_t data_size, int (*compare)(void*, void*));
BTNode* findMinimum(BTNode* root);
BTNode* deleteNode(BTNode* root, void* val, int (*compare)(void*, void*), size_t data_size);
void inOrderTraversalPrintString(BTNode* root);
int compareInt(void* a, void* b);
int compareString(void* a, void* b);
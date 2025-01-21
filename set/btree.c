// Binary Tree Implementation for Sets

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"

BTNode* createNode(void* value, size_t data_size){
    struct BTNode* temp = (BTNode*)malloc(sizeof(BTNode));
    temp->data = malloc(data_size);
    memcpy(temp->data, value, data_size);
    temp->left = temp->right = NULL;
    return temp;
}

BTNode* contains(BTNode* root, void* target, int (*compare)(void*, void*)){
    if (root == NULL){
        return NULL;
    } 
    int cmp = compare(target, root->data);
    if (cmp == 0) {
        return root;
    } else if (cmp < 0) {
        return contains(root->left, target, compare);
    }
    return contains(root->right, target, compare);
}
BTNode* add(BTNode* node, void* value, size_t data_size, int(*compare)(void*, void*)){
    if (node == NULL){
        return createNode(value, data_size);
    }
    // Digressing from B-Tree to set as values need to be compared before being put in
    int cmp = compare(value, node->data);
    if (cmp < 0) {
        node->left = add(node->left, value, data_size, compare);
    }
    else if (cmp > 0){
        node->right = add(node->right, value, data_size, compare);
    }
    return node;
}


BTNode* findMinimum(BTNode* root){
    if (root == NULL) {
        return NULL;
    } else if (root->left != NULL) {
        return findMinimum(root->left);
    }
    return root;
}

BTNode* deleteNode(BTNode* root, void* val, int (*compare)(void*, void*), size_t data_size) {
    if (root == NULL) return NULL;
    int cmp = compare(val, root->data);

    if (cmp > 0) {
        root->right = deleteNode(root->right, val, compare, data_size);
    } else if (cmp < 0){
        root->left = deleteNode(root->left, val, compare, data_size);
    } else {
        if (root->left == NULL && root->right == NULL){
            free(root->data);
            free(root);
            return NULL;
        }else if(root->left == NULL || root->right == NULL){
            BTNode* temp;
            if (root->left == NULL) {
                temp = root->right;
            }else{
                temp = root->left;
            }
            free(root->data);
            free(root);
            return temp;
        }else{
            BTNode* temp = findMinimum(root->right);
            memcpy(root->data, temp->data, data_size);
            root->right = deleteNode(root->right, temp->data, compare, data_size);
        }
    }
    return root;
}

void inOrderTraversalPrintString(BTNode* root) {
    if (root == NULL) {
        return;
    }
    inOrderTraversalPrintString(root->left);
    printf("%s\n", (char*)root->data);
    inOrderTraversalPrintString(root->right);
}

int compareInt(void* a, void* b) {
    int intA = *(int*)a;
    int intB = *(int*)b;
    if (intA < intB) return -1;
    if (intA > intB) return 1;
    return 0;
}

int compareString(void* a, void* b) {
    return strcmp((char*)a, (char*)b);
}

// int main(){
//     BTNode* root = NULL;

//     int values[] = {50, 30, 20, 40, 70, 60, 80};
//     size_t size = sizeof(int);

//     for (int i = 0; i < 7; i++) {
//         root = add(root, &values[i], size, compareInt);
//     }

//     int searchVal = 60;
//     if (contains(root, &searchVal, compareInt) != NULL) {
//         printf("60 is in the set\n");
//     } else {
//         printf("60 is not in the set\n");
//     }

//     // Test with strings
//     BTNode* rootStr = NULL;
//     char* strings[] = {"apple", "banana", "cherry", "date", "fig", "grape"};
//     size_t sizeStr = sizeof(char*);

//     for (int i = 0; i < 6; i++) {
//         rootStr = add(rootStr, strings[i], strlen(strings[i]) + 1, compareString);
//     }

//     char* searchStr = "cherry";
//     if (contains(rootStr, searchStr, compareString) != NULL) {
//         printf("\"cherry\" is in the set\n");
//     } else {
//         printf("\"cherry\" is not in the set\n");
//     }

//     // Test deleting strings
//     rootStr = delete(rootStr, "banana", compareString, sizeStr);
//     searchStr = "banana";
//     if (contains(rootStr, searchStr, compareString) != NULL) {
//         printf("\"banana\" is in the set\n");
//     } else {
//         printf("\"banana\" is not in the set\n");
//     }

//     return 0;
// }
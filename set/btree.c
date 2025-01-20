// Binary Tree Implementation for Sets

#include <stdio.h>
#include <stdlib.h>

typedef struct BTNode {
    int key;
    struct BTNode *left, *right;
} BTNode;

BTNode* createNode(int value){
    struct BTNode* temp = (BTNode*)malloc(sizeof(BTNode));
    temp->key = value;
    temp->left = temp->right = NULL;
    return temp;
}

BTNode* searchNode(BTNode* root, int target){
    if (root == NULL || root->key == target){
        return root;
    } 
    if (root->key < target){
        return searchNode(root->right, target);
    }
    return searchNode(root->left, target);
}
BTNode* insertNode(BTNode* node, int value){
    if (node == NULL){
        return createNode(value);
    }
    if (value < node->key) {
        node->left = insertNode(node->left, value);
    }
    else if (value > node->key){
        node->right = insertNode(node->right, value);
    }
    return node;
}

void postOrderTraversal(BTNode* root){
    if (root != NULL){
        postOrderTraversal(root->left);
        postOrderTraversal(root->right);
        printf("%d\n", root->key);
    }
}

void inOrder(BTNode* root){
    if (root != NULL){
        inOrder(root->left);
        printf("%d\n", root->key);
        inOrder(root->right);
    }
}

void preOrder(BTNode* root){
    if (root != NULL){
        printf("%d", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}

BTNode* findMinimum(BTNode* root){
    if (root == NULL) {
        return NULL;
    } else if (root->left != NULL) {
        return findMinimum(root->left);
    }
    return root;
}

BTNode* delete(BTNode* root, int val) {
    if (root == NULL) return NULL;

    if (val > root->key) {
        root->right = delete(root->right, val);
    } else if (val < root->key){
        root->left = delete(root->left, val);
    } else {
        if (root->left == NULL && root->right == NULL){
            free(root);
            return NULL;
        }else if(root->left == NULL || root->right == NULL){
            BTNode* temp;
            if (root->left == NULL) {
                temp = root->right;
            }else{
                temp = root->left;
            }
            free(root);
            return temp;
        }else{
            BTNode* temp = findMinimum(root->right);
            root->key = temp->key;
            root->right = delete(root->right, temp->key);
        }
    }
    return root;
}

int main(){
    BTNode* root = NULL;
    root = insertNode(root, 50);
    insertNode(root, 30);
    insertNode(root, 20);
    insertNode(root, 40);
    insertNode(root, 70);
    insertNode(root, 60);
    insertNode(root, 80);

    // Search for a node with key 60
    if (searchNode(root, 60) != NULL) {
        printf("60 found");
    }
    else {
        printf("60 not found");
    }

    printf("\n");

    // Perform post-order traversal
    postOrderTraversal(root);
    printf("\n");

    // Perform pre-order traversal
    preOrder(root);
    printf("\n");

    // Perform in-order traversal
    inOrder(root);
    printf("\n");

    // Perform delete the node (70)
    BTNode* temp = delete (root, 70);
    printf("After Delete: \n");
    inOrder(root);
}
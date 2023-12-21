﻿#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


struct _node;
typedef struct _node* Position;
typedef struct _node {
    int value;
    Position left;
    Position right;
} Node;

Position createNode(int value);
Position insert(Position root, int value);
int inorder(Position root);
int preorder(Position root);
int postorder(Position root);
Position search(Position root, int value);
int replace(Position root);
int random();

int main() {
    Position root = NULL;


    root = insert(root, 50);
    insert(root, 30);
    insert(root, 20);
    insert(root, 40);
    insert(root, 70);
    insert(root, 60);
    insert(root, 80);


    printf("Inorder: ");
    inorder(root);
    printf("\n");

    printf("Preorder: ");
    preorder(root);
    printf("\n");

    printf("Postorder: ");
    postorder(root);
    printf("\n");


    int keyToFind = 40;
    Position foundNode = search(root, keyToFind);
    if (foundNode != NULL)
        printf("Element %d pronađen.\n", keyToFind);
    else
        printf("Element %d nije pronađen.\n", keyToFind);


    replace(root);



    return 0;
}

Position createNode(int value) {
    Position newNode = (Position)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Unable to allocate memory for a new node");
        exit(EXIT_FAILURE);
    }

    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Position insert(Position root, int value) {
    if (root == NULL)
        return createNode(value);

    if (value < root->value)
        root->left = insert(root->left, value);
    else if (value > root->value)
        root->right = insert(root->right, value);

    return root;
}

int inorder(Position root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->value);
        inorder(root->right);
    }
}

int preorder(Position root) {
    if (root != NULL) {
        printf("%d ", root->value);
        preorder(root->left);
        preorder(root->right);
    }
}

int postorder(Position root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->value);
    }
}

Position search(Position root, int value) {
    if (root == NULL || root->value == value)
        return root;

    if (value < root->value)
        return search(root->left, value);
    else
        return search(root->right, value);
}

int replace(Position root) {
    if (root != NULL) {
        root->value = random();
        replace(root->left);
        replace(root->right);
    }
}

int random() {
    srand((unsigned)time(NULL));
    return rand();
}
/*
 * C implementation of red balck color tree in C.
 */

/* 
 * File:   main.c
 * Author: Connor McCrum
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
struct Node {
    int data;
    struct Node *lChild;
    struct Node *rChild;
    struct Node *parent;
    unsigned int color : 1; // 0 for black 1 for red
};

int main(int argc, char** argv) {
    struct Node root = newNode(srand() % 10000);
    root.color = 0;
    //fill tree with 50 random values
    for (int i = 0; i < 50; i++) {
        addNode(root, newNode(srand() % 10000));
        
    }
    //we'll use some recursion to print out the tree 
    return 0;
}
//return pointers to memory locations and forcible allocate memory as otherwise it would fall out of scope and be lost

struct Node * newNode(int data) {
    struct Node node = (struct Node) malloc(sizeof (struct Node)); //allocate memory and cast to the node struct type
    node.data = data;
    node.lChild = NULL;
    node.rChild = NULL;
    node.parent = NULL;
    node.color = 1;
    return node;
}

/*
 * root can be considered the parent it is of newNode
 */
void addNode(struct Node root, struct Node newNode1) {
    if (newNode1.data < root.data) {
        if (root.lChild == NULL) {
            root.lChild = newNode;
            newNode1.parent = root;
            fixAfterInsert(root);
        } else {
            addNode(root.lChild, newNode1);
            return;
        }
    } else {
        if (root.rChild == NULL) {
            root.rChild = newNode;
            newNode1.parent = root;
            fixAfterInsert(root);

        } else {
            addNode(root.rChild, newNode1);
            return;
        }

    }

}

void fixAfterInsert(struct Node root) {
    if(root.parent==NULL){//if this is true we reached the root and the tree has been fixed after insertions
        root.color = 0;
        return;
    }
    if (root.color == 1) {//we have a double red and must rebalance the tree to correct this


        if (&root == &root.parent->lChild) {
            if (root.parent->rChild == NULL || root.parent->rChild->color == 0) {
                struct Node tempNode = root.lChild;
                root.lChild = root.parent;
                root.parent->lChild = tempNode;
                if (root.parent->parent != NULL) {
                    fixAfterInsert(root.parent); //we move up the tree recursively checking for double reds
                    //once we cannot find any more we check to make sure the tree has a valid black depth
                } else {
                    //if the above is false it means that root must now be the root node of the entire tree
                    root.parent = NULL;
                    root.color = 0;
                }
            } else {
                root.parent->color = 1;
                root.color = 0;
                root.parent->rChild->color=0;
                fixAfterInsert(root.parent);
            }
        } else {
            if (root.parent->lChild == NULL || root.parent->lChild->color == 0) {
                //inverse of above right children are changed instead of left
                struct Node tempNode = root.rChild;
                root.rChild = root.parent;
                root.parent->rChild = tempNode;
                if (root.parent->parent != NULL) {
                    fixAfterInsert(root.parent); //we move up the tree recursively checking for double reds
                    //once we cannot find any more we check to make sure the tree has a valid black depth
                } else {
                    //if the above is false it means that root must now be the root node of the entire tree
                    root.parent = NULL;
                    root.color = 0;
                }


            } else {
                root.parent->color = 1;
                root.color = 0;
                root.parent->lChild->color=0;
                fixAfterInsert(root.parent);
            }
        }
    }


}


void swap(void *a, void *b) {
    void *temp = *a;
    *a = *b;
    *b = *temp;
}

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
    struct Node node; //allocate memory and cast to the node struct type
    node.data = data;
    node.lChild = NULL;
    node.rChild = NULL;
    node.parent = NULL;
    node.color = 1;
    return node;
}


/*
 * Data indicates the value inside the node in which you wish to delete, found same with binary search tree
 */
void deleteNode(Node root, int data) {
    struct Node *nodeToDelete = findNode(data);
    if (nodeToDelete == NULL) {
        return -1; //error not found
    }
    struct Node *tempNode = root.rChild;
    while (1 == 1) {
        if (tempNode->lChild != NULL) {
            tempNode = tempNode->lChild;
        } else {
            break;
        }
    }
    nodeToDelete->data = tempNode->data; //just copy tempNodes data don't worry
    //about what we overright as that is the data we are deleting anyway
    if (tempNode->rChild != NULL) {

        int colorOfChild = tempNode->rChild->color;
        tempNode->rChild->parent = NULL;
        tempNode->data = tempNode->rChild->data;
        swapPointers(tempNode->rChild, tempNode->rChild->rChild);
        swapPointers(tempNode->lChild, tempNode->rChild->lChild);
        //Node is now deleted tree must be checked for balance issues that may have been created.
        if (tempNode->color == 1) {
            tempNode->color = colorOfChild;
        } else {//double black case
            if (colorOfChild == 0) {//if it's red we don't have to do anything as it's supposed to be recolored black
                //but as we never swaped the color values it is already black
                fixDoubleBlack(tempNode);
            }
        }

    } else {
        //just delete the node no other steps needed
        tempNode->parent->lChild = NULL;
        tempNode->parent = NULL;
    }
}

/*
 * HTe node passed in is considered to be colored double black
 */
void Node fixDoubleBlack(struct Node *doubleBlack) {
    if (doubleBlack->parent == NULL) {
        return; //we've reached the root node so the double black can safely be turned into a single black without impacting the balance.
    }
    struct Node *x, *y, *e, *z, *c, *d;
    x = doubleBlack->parent;
    y = doubleBlack->parent->rChild;
    e = doubleBlack->parent->rChild->rChild;
    z = doubleBlack->parent->rChild->lChild;
    c = doubleBlack->parent->rChild->lChild->lChild;
    d = doubleBlack->parent->rChild->lChild->rChild;
    if (y->color == 1) {
        swap(&x->data, &y->data); //data only is swaped as why is the root node of this subtree and we do not want to
        //destroy the parent child relationship between it and it's potentail parent
        y->lChild = doubleBlack;
        doubleBlack->parent = y;
        x->rChild = e;
        y->rChild = z;
        y->color = 1;
        x->lChild = y;
        y->parent = x;

        fixDoubleBlack(doubleBlack);
    } else {//y color = 0

        if (z != NULL && z->color == 1) {
            swap(&z->data, &x->data);
            x->lChild = z;
            z->rChild = c;
            c->parent = z;
            z->parent = x;
            y->lChild = d;
            d->parent = y;
            z->lChild = doubleBlack;
            doubleBlack->parent=z;
            z->color=0;
            

        } else if (e != NULL && e->color == 1) {
            swap(&x->data, &y->data);
            x->lChild = y;
            y->parent = x;
            y->rChild = y->lChild;
            y->lChild=doubleBlack;
            doubleBlack->parent=y;
            x->rChild = e;
            e->parent=x;
            e->color=0;
        } else {
            x->color = 0;
            y->color = 1;
            fixDoubleBlack(x);
        }
    }

}

struct Node findNode(Node root, int data) {
    struct Node *tempNode = &root;
    while (tempNode != NULL && *tempNode->data != data) {
        if (tempNode->data > data) {
            tempNode = tempNode->lChild;
        } else {
            tempNode = tempNode->rChild;
        }
    }
    return tempNode;
}

/*
 * root can be considered the parent it is of newNode
 */
void addNode(struct Node root, struct Node newNode1) {
    if (newNode1.data < root.data) {
        if (root.lChild == NULL) {
            root.lChild = newNode;
            newNode1.parent = root;
            fixAfterInsert(&root);
        } else {
            addNode(root.lChild, newNode1);
            return;
        }
    } else {
        if (root.rChild == NULL) {
            root.rChild = newNode;
            newNode1.parent = root;
            fixAfterInsert(&root);

        } else {
            addNode(root.rChild, newNode1);
            return;
        }

    }

}
void fixAfterInsert(struct Node *root) {
    if (root->parent == NULL) {//if this is true we reached the root and the tree has been fixed after insertions
        root->color = 0;
        return;
    }
    if (root->color == 1) {//we have a double red and must rebalance the tree to correct this


        if (root == root->parent->lChild) {
            if (root->parent->rChild == NULL || root->parent->rChild->color == 0) {
                struct Node tempNode = root->lChild;
                root->lChild = root->parent;
                root->parent->lChild = tempNode;
                if (root->parent->parent != NULL) {
                    fixAfterInsert(root->parent); //we move up the tree recursively checking for double reds
                    //once we cannot find any more we check to make sure the tree has a valid black depth
                } else {
                    //if the above is false it means that root must now be the root node of the entire tree
                    root->parent = NULL;
                    root->color = 0;
                }
            } else {
                root->parent->color = 1;
                root->color = 0;
                root->parent->rChild->color = 0;
                fixAfterInsert(root->parent);
            }
        } else {
            if (root->parent->lChild == NULL || root->parent->lChild->color == 0) {
                //inverse of above right children are changed instead of left
                struct Node *tempNode = root->rChild;
                root->rChild = root->parent;
                root->parent->rChild = tempNode;
                if (root->parent->parent != NULL) {
                    fixAfterInsert(root->parent); //we move up the tree recursively checking for double reds
                    //once we cannot find any more we check to make sure the tree has a valid black depth
                } else {
                    //if the above is false it means that root must now be the root node of the entire tree
                    root->parent = NULL;
                    root->color = 0;
                }


            } else {
                root->parent->color = 1;
                root->color = 0;
                root->parent->lChild->color = 0;
                fixAfterInsert(root->parent);
            }
        }
    }


}

/*Swaps the location where to pointers point to
 */
void swapPointers(void *a, void *b) {
    void *temp = a;
    a = b;
    b = temp;
}

/*Swaps the values stored at each pointers/memory location
 */
void swap(void *a, void *b) {
    void *temp = *a;
    *a = *b;
    *b = *temp;
}

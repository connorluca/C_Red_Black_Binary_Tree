/*
 * C implementation of red balck color tree in C.
 */

/* 
 * File:   main.c
 * Author: Connor McCrum
 *https://www.usna.edu/Users/cs/crabbe/SI321/current/red-black/red-black.html - may be helpful for understanding the insertion and deletion algorithms
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
            fixAfterInsert(&newNode1);
        } else {
            addNode(root.lChild, newNode1);
            return;
        }
    } else {
        if (root.rChild == NULL) {
            root.rChild = newNode;
            newNode1.parent = root;
            fixAfterInsert(&newNode1);

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
    if (root->parent->color == 1) {//we have a double red and must rebalance the tree to correct this
        if(root->parent->parent->rChild == root->parent){
            if(root->parent->parent->lChild->color==1){//colorflip the nodes.
                root->parent->color=0;
                root->parent->parent->color=1;
                root->parent->parent->lChild->color=0;
                fixAfterInsert(root->parent->parent);
            }
            else{//rotate
                struct Node *v, *x, *u, *z;
                v= root->parent;
                u= root->parent->parent;
                swap(&u->data, &v->data);
                if(root==root->parent->lChild){
                    u->lChild=v;
                    v->rChild=root;
                    root->parent=v;
                    fixAfterInsert(v);
                    
                }
                else{//root=parent-rchild
                    x=root->parent->lChild;
                    z=root->parent->parent->lChild;
                    
                    u->lChild=v;
                    v->lChild=z;
                    z->parent=v;
                    u->color=0;
                    v->color=1;
                }
                
            }
        }
        else{
            if(root->parent->parent->rChild->color==1){
                root->parent->color=0;
                root->parent->parent->color=1;
                root->parent->parent->rChild->color=0;
                fixAfterInsert(root->parent->parent);
            }
            else{
                struct Node *v, *x, *u, *z;
                v= root->parent;
                u=root->parent->parent;
                
                if(root==root->parent->lChild){
                    swap(&u->data, &v->data);
                    x=root->parent->rChild;
                    u->lChild=root;
                    root->parent=u;
                    v->lChild=x;
                    x->parent=v;
                    v->rChild=z;
                }
                else{                 
                    swap(&root->data, &u->data);
                    x=root->rChild;
                    z=root->lChild;
                    root->rChild=u->rChild;
                    v->rChild=z;//used to be root but now thats the root of this subtree
                    u->rChild=root;
                    root->parent=u;
                    root->lChild=x;
                    
                }
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

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
struct Node{
    int data;
    struct Node *lChild;
    struct Node *rChild;
    unsigned int color: 1;// 0 for black 1 for red
};
int main(int argc, char** argv) {
    struct Node *root = newNode(srand()%1000);
    //fill tree with 50 random values
    for(int i = 0; i < 50; i++){
        
    }
    //we'll use some recursion to print out the tree 
    return 0;
}
//return pointers to memory locations and forcible allocate memory as otherwise it would fall out of scope and be lost
struct Node * newNode(int data){
    struct Node *node = (struct node*)malloc(sizeof(struct node));//allocate memory and cast to the node struct type
    node->data = data;
    node->lChild = NULL;
    node->rChild = NULL;
    node->color = 0;
    return node;
}
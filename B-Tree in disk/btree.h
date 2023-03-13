//
// Created by Alvaro on 10/03/2023.
//

#ifndef TESTEGITHUB_BTREE_H
#define TESTEGITHUB_BTREE_H


#include <stdio.h>
#include <malloc.h>
#include "queue.h"



typedef struct element{
    int codigo;
    float salario;
}element;

typedef struct btNode{
    int numKeys;
    int isLeaf;
    int pos_in_disk;
    element *keys;
    int *kids;
}btNode;

typedef struct bTree {
    int order;
    btNode root;
    int node_count;
} bTree;

typedef struct queue queue;


int calculate_offset(int disk, int order);


btNode btSearchNode(btNode node, int order, element key, FILE *fp);


void btPrintElement(bTree tree, element key, FILE *fp);


bTree *btCreate(int order);


int btSearch(btNode node, int order, element key, FILE *fp);


void btInsert(bTree *tree, element key, FILE *fp);


int btDelete(bTree *tree, element key, FILE *fp);


void btPrintTree(bTree *tree, queue *q,FILE *fp);


void btDestroy(bTree *tree, FILE *fp);


element btfindMax(btNode node, int order, FILE *fp);


element btfindMin(btNode node, int order, FILE *fp);


btNode disk_read(int disk, int order, FILE *fp);

#endif //TESTEGITHUB_BTREE_H

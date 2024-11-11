#ifndef TREE_H
#define TREE_H

#include <stdio.h>

#include <error.h>

typedef char* NodeVal;

struct Node
{
    NodeVal val;
    Node *parent, *lft, *rgt;
};

struct Tree
{
    Node *root;
    int n_nodes;

    FILE* fdump;
};

ErrEnum treeCtor(Tree* tree);
void treeDtor(Tree* tree);

ErrEnum nodeCtor(Node** node, const NodeVal val, Node* parent, Node* lft, Node* rgt);
ErrEnum nodeChangeVal(Node* node, NodeVal val);
void nodeDtor(Node* node);

ErrEnum insertNode(Tree* tree, NodeVal val);
void addSubtrees(Node* node, int* n_nodes, Tree* lft, Tree* rgt);

void printNodeDot(FILE* fout, Node* node);
ErrEnum treeMakeGraph(Tree* tree);
ErrEnum treeDump(Tree* tree);

void nodeWrite(FILE* fout, Node* node);
void treeWrite(FILE* fout, Tree* tree);
ErrEnum nodeRead(char* buf, int* buf_pos, Node* node, int* n_nodes);
ErrEnum treeRead(FILE* fin, Tree* tree);

#endif // TREE_H
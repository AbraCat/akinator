#ifndef TREE_H
#define TREE_H

#include <stdio.h>

#include <error.h>

typedef short NodeVal;

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

ErrEnum nodeCtor(Node** node, NodeVal val, Node* parent, Node* lft, Node* rgt);
void nodeDtor(Node* node);

ErrEnum insertNode(Tree* tree, NodeVal val);
void addSubtrees(Node* node, Node* lft, Node* rgt);

ErrEnum treeMakeGraph(Tree* tree);
ErrEnum treeDump(Tree* tree);

ErrEnum treeWrite(FILE* fout, Tree* tree);
ErrEnum treeRead(FILE* fin, Tree* tree);

#endif // TREE_H
#ifndef AKINATOR_H
#define AKINATOR_H

#include <akinator.h>
#include <tree.h>

ErrEnum mainAkinCycle();

ErrEnum akinInit(Tree* tree);
ErrEnum addEntry(Tree* tree, Node* node, const char* name, const char* diff);
ErrEnum akinPlay(Tree* tree, FILE* fin, FILE* fout);
ErrEnum printDescr(FILE* fout, Tree* tree, Node* node);
ErrEnum printCmp(FILE* fout, Tree* tree, Node* node1, Node* node2);

ErrEnum scanYesNo(FILE* fin, FILE* fout, int* ans);
void akinHelpMsg(FILE* fout);

#endif // AKINATOR_H
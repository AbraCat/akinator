#ifndef AKINATOR_H
#define AKINATOR_H

#include <akinator.h>
#include <tree.h>

ErrEnum initAkinator(Tree* tree);
ErrEnum addEntry(Tree* tree, Node* node, const char* name, const char* diff);
ErrEnum play(Tree* tree, FILE* fin, FILE* fout);
ErrEnum printDescr(FILE* fout, Tree* tree, Node* node);
ErrEnum printCmp(FILE* fout, Tree* tree, Node* node1, Node* node2);

ErrEnum scanYesNo(FILE* fin, FILE* fout, int* ans);

#endif // AKINATOR_H
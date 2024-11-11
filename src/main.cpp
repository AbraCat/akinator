#include <stdio.h>

#include <akinator.h>
#include <error.h>

// returnErr(nodeCtor(&tree.root, "aaa", NULL, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->lft, "bbb", tree.root, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->rgt, "ccc", tree.root, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->rgt->lft, "ddd", tree.root->rgt, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->rgt->lft->lft, "eee", tree.root->rgt->lft, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->rgt->lft->rgt, "fff", tree.root->rgt->lft, NULL, NULL));

int main(int argc, const char* argv[])
{

    Tree tree = {};
    handleErr(treeCtor(&tree));

    // FILE *fout = fopen("txt/database.txt", "w");
    // if (fout == NULL) handleErr(ERR_OPEN_FILE);
    // treeWrite(fout, &tree);
    // fclose(fout);

    FILE *fin = fopen("txt/database.txt", "r");
    if (fin == NULL) handleErr(ERR_OPEN_FILE);
    returnErr(treeRead(fin, &tree));
    fclose(fin);
    returnErr(play(&tree, stdin, stdout));

    treeDump(&tree);
    treeDtor(&tree);
    return 0;
}
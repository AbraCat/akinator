#include <stdio.h>
#include <string.h>

#include <akinator.h>
#include <error.h>
#include <utils.h>

int main(int argc, const char* argv[])
{
    // Tree tree = {};
    // handleErr(treeCtor(&tree));

    // returnErr(nodeCtor(&tree.root, "aaa", NULL, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->lft, "bbb", tree.root, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->rgt, "ccc", tree.root, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->rgt->lft, "ddd", tree.root->rgt, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->rgt->lft->lft, "eee", tree.root->rgt->lft, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->rgt->lft->rgt, "fff", tree.root->rgt->lft, NULL, NULL));
    // returnErr(treeDump(&tree));

    // FILE *fout = fopen("txt/database.txt", "w");
    // if (fout == NULL) handleErr(ERR_OPEN_FILE);
    // treeWrite(fout, &tree);
    // fclose(fout);

    // treeDtor(&tree);
    // return 0;

    return mainAkinCycle();
}
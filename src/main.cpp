#include <stdio.h>

#include <tree.h>
#include <error.h>

#include <stdlib.h>

// char* val
// addSubtrees
// treeRead/Write

int main(int argc, const char* argv[])
{
    // char buf[] = "aabab", buf2[20] = "";
    // printf("%d\n", sscanf(buf, "%[^b]", buf2));
    // printf("%s\n", buf2);
    // return 0;

    Tree tree = {};
    handleErr(treeCtor(&tree));

    // returnErr(nodeCtor(&tree.root, "aaa", NULL, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->lft, "bbb", tree.root, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->rgt, "ccc", tree.root, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->rgt->lft, "ddd", tree.root->rgt, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->rgt->lft->lft, "eee", tree.root->rgt->lft, NULL, NULL));
    // returnErr(nodeCtor(&tree.root->rgt->lft->rgt, "fff", tree.root->rgt->lft, NULL, NULL));

    // FILE *fout = fopen("txt/tree.txt", "w");
    // if (fout == NULL) handleErr(ERR_OPEN_FILE);
    // treeWrite(fout, &tree);
    // fclose(fout);

    FILE *fin = fopen("txt/database.txt", "r");
    if (fin == NULL) handleErr(ERR_OPEN_FILE);
    returnErr(treeRead(fin, &tree));
    fclose(fin);
    printf("n_nodes: %d\n", tree.n_nodes);

    treeDump(&tree);
    treeDtor(&tree);
    return 0;
}
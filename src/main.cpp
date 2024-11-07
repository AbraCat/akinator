#include <stdio.h>

#include <tree.h>
#include <error.h>

#include <stdlib.h>

int main(int argc, const char* argv[])
{
    Tree tree = {};
    srand(1);
    handleErr(treeCtor(&tree));

    for (int i = 0; i < 50; ++i)
    {
        handleErr(insertNode(&tree, rand()));
    }

    handleErr(treeDump(&tree));

    treeDtor(&tree);
    return 0;
}
#include <stdio.h>

#include <tree.h>
#include <error.h>

int main(int argc, const char* argv[])
{
    Tree tree = {};
    handleErr(treeCtor(&tree));

    handleErr(treeDump(&tree));
    handleErr(treeDump(&tree));

    treeDtor(&tree);
    return 0;
}
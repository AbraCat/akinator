#include <stdlib.h>

#include <tree.h>

const int elem_ptr = 0;
const char elem_fmt[] = "%d";

ErrEnum treeCtor(Tree* tree)
{
    tree->root = NULL;
    tree->n_nodes = 0;
    tree->fdump = NULL;
    return ERR_OK;
}
void treeDtor(Tree* tree)
{
    nodeDtor(tree->root);
    if (tree->fdump != NULL) free(tree->fdump);
}

ErrEnum nodeCtor(Node** node, NodeVal val, Node* parent, Node* lft, Node* rgt)
{
    *node = (Node*)calloc(1, sizeof(Node));
    if (*node == NULL) return ERR_MEM;
    (*node)->val = val;
    (*node)->parent = parent;
    (*node)->lft = lft;
    (*node)->rgt = rgt;
    return ERR_OK;
}

void nodeDtor(Node* node)
{
    if (node == NULL) return;
    nodeDtor(node->lft);
    nodeDtor(node->rgt);
    free(node);
}

ErrEnum insertNode(Tree* tree, NodeVal val)
{
    if (tree->root == NULL)
    {
        returnErr(nodeCtor(&tree->root, val, NULL, NULL, NULL));
        ++(tree->n_nodes);
        return ERR_OK;
    }

    Node* node = tree->root;
    while (node->lft != NULL)
    {
        if (val <= node->val) node = node->lft;
        else node = node->rgt;
    }
    if (val <= node->val) returnErr(nodeCtor(&node->lft, val, node, NULL, NULL))
    else returnErr(nodeCtor(&node->rgt, val, node, NULL, NULL))
    ++(tree->n_nodes);
    return ERR_OK;
}

void addSubtrees(Node* node, Node* lft, Node* rgt)
{
    node->lft = lft;
    node->rgt = rgt;
}

const char log_path[] = "./log";
const int buffer_size = 500;
int dump_cnt = 0;

ErrEnum treeWrite(FILE* fout, Tree* tree)
{
    //

    return ERR_OK;
}

ErrEnum treeRead(FILE* fin, Tree* tree)
{
    //
    
    return ERR_OK;
}

ErrEnum treeMakeGraph(Tree* tree)
{
    myAssert(tree != NULL);

    static int first_call = 1;
    char buf[buffer_size] = "";
    if (first_call)
    {
        first_call = 0;
        sprintf(buf, "type NUL > %s/dot-src/dot-src.txt", log_path);
        system(buf);
    }

    sprintf(buf, "%s/dot-src/dot-src.txt", log_path);
    FILE *fout = fopen(buf, "w");
    if (fout == NULL) return ERR_OPEN_FILE;

    fputs("digraph Tree\n{aaa->bbb\n}\n", fout);

    fclose(fout);


    sprintf(buf, "dot %s/dot-src/dot-src.txt -Tpng -o%s/dot-img/dot-img-%d.png", 
    log_path, log_path, dump_cnt);
    system(buf);

    return ERR_OK;
}

ErrEnum treeDump(Tree* tree)
{
    myAssert(tree != NULL);

    static int first_call = 1;
    char buf[buffer_size] = "";
    if (first_call)
    {
        first_call = 0;
        sprintf(buf, "type NUL > %s/dump/dump.html", log_path);
        system(buf);
        sprintf(buf, "%s/dump/dump.html", log_path);
        tree->fdump = fopen(buf, "w");
        if (tree->fdump == NULL) return ERR_OPEN_FILE;
    }

    FILE *fout = tree->fdump;

    // dump text

    sprintf(buf, "../dot-img/dot-img-%d.png", dump_cnt);
    fprintf(fout, "\n\n<img src=\"%s\" alt=\"graph image\"/>\n\n", buf);


    treeMakeGraph(tree);
    ++dump_cnt;
    return ERR_OK;
}
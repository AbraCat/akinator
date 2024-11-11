#include <stdlib.h>
#include <string.h>

#include <tree.h>

#define ELEM_VAL(elem) elem
#define ELEM_ADR(elem) elem
#define ELEM_FMT "%s"

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
    if (tree->fdump != NULL) fclose(tree->fdump);
}

ErrEnum nodeCtor(Node** node, ConstNodeVal val, Node* parent, Node* lft, Node* rgt)
{
    *node = (Node*)calloc(1, sizeof(Node));
    if (*node == NULL) return ERR_MEM;
    
    returnErr(nodeChangeVal(*node, val));

    (*node)->parent = parent;
    (*node)->lft = lft;
    (*node)->rgt = rgt;
    return ERR_OK;
}

ErrEnum nodeChangeVal(Node* node, ConstNodeVal val)
{
    if (val == NULL) return ERR_OK;

    if (node->val == NULL)
    {
        node->val = (NodeVal)calloc(small_buf_size, sizeof(char));
        if (node->val == NULL) return ERR_MEM;
        strncpy(node->val, val, small_buf_size);
    }

    return ERR_OK;
}

void nodeDtor(Node* node)
{
    if (node == NULL) return;
    nodeDtor(node->lft);
    nodeDtor(node->rgt);
    free(node->val);
    free(node);
}

ErrEnum insertNode(Tree* tree, NodeVal val)
{
    // works for integer NodeVal
    
    if (tree->root == NULL)
    {
        returnErr(nodeCtor(&tree->root, val, NULL, NULL, NULL));
        ++(tree->n_nodes);
        return ERR_OK;
    }

    Node* node = tree->root;
    int insert_lft = 0;
    while (1)
    {
        if (val <= node->val) // if (nodeCmp(val, node->val) <= 0)
        {
            if (node->lft == NULL)
            {
                insert_lft = 1;
                break;
            }
            node = node->lft;
        }
        else
        {
            if (node->rgt == NULL)
            {
                insert_lft = 0;
                break;
            }
            node = node->rgt;
        }
    }
    if (insert_lft) returnErr(nodeCtor(&node->lft, val, node, NULL, NULL))
    else returnErr(nodeCtor(&node->rgt, val, node, NULL, NULL))
    ++(tree->n_nodes);
    return ERR_OK;
}

void addSubtrees(Node* node, int* n_nodes, Tree* lft, Tree* rgt)
{
    myAssert(node->lft == NULL && node->rgt == NULL);

    node->lft = lft->root;
    node->rgt = rgt->root;
    n_nodes += lft->n_nodes + rgt->n_nodes;
}

int nodeCmp(NodeVal lft, NodeVal rgt)
{
    return strncmp(lft, rgt, small_buf_size);
}

void nodeFind(Node* node, const NodeVal val, Node** ans)
{
    myAssert(ans != NULL);

    if (node == NULL)
    {
        *ans = NULL;
        return;
    }

    if (nodeCmp(val, node->val) == 0)
    {
        *ans = node;
        return;
    }
    nodeFind(node->lft, val, ans);
    if (*ans != NULL) return;
    nodeFind(node->rgt, val, ans);
}

const char log_path[] = "./log";
const int buffer_size = 500;
int dump_cnt = 0;

void treeWrite(FILE* fout, Tree* tree)
{
    nodeWrite(fout, tree->root);
}

void nodeWrite(FILE* fout, Node* node)
{
    if (node == NULL) return;
    fprintf(fout, "{\"" ELEM_FMT "\"", ELEM_VAL(node->val));
    nodeWrite(fout, node->lft);
    nodeWrite(fout, node->rgt);
    fputs("}", fout);
}

ErrEnum nodeRead(char* buf, int* buf_pos, Node* node, int* n_nodes)
{
    #define cur_buf (buf + *buf_pos)
    #define scanfBuf(fmt, ...)             \
    (                                      \
        scanf_res = sscanf(cur_buf,        \
        fmt "%n", __VA_ARGS__, &pos_incr), \
        *buf_pos += pos_incr,              \
        scanf_res                          \
    )

    // change error code
    #define scanfBufExp(exp_res, fmt, ...)         \
        if (scanfBuf(fmt, __VA_ARGS__) != exp_res) \
            return ERR_FILE;

    ++(*n_nodes);
    int scanf_res = 0, pos_incr = 0;
    char val_buf[small_buf_size] = "";

    if (cur_buf[0] != '{') {return ERR_FILE;} // change err code
    ++(*buf_pos);
    if (cur_buf[0] != '"') return ERR_FILE; // change err code
    ++(*buf_pos);

    // returnErr(nodeCtor(node, cur_buf, ))
    scanfBufExp(1, "%[^\"]", val_buf);
    returnErr(nodeChangeVal(node, val_buf));
    if (*cur_buf != '"') return ERR_FILE; //
    ++(*buf_pos);

    if (*cur_buf == '}')
    {
        ++(*buf_pos);
        node->lft = node->rgt = NULL;
        return ERR_OK;
    }
    returnErr(nodeCtor(&node->lft, NULL, node, NULL, NULL));
    returnErr(nodeRead(buf, buf_pos, node->lft, n_nodes));

    if (*cur_buf == '}')
    {
        ++(*buf_pos);
        node->rgt = NULL;
        return ERR_OK;
    }
    returnErr(nodeCtor(&node->rgt, NULL, node, NULL, NULL));
    returnErr(nodeRead(buf, buf_pos, node->rgt, n_nodes));

    if (*cur_buf != '}') return ERR_FILE;
    ++(*buf_pos);

    return ERR_OK;
}

ErrEnum treeRead(FILE* fin, Tree* tree)
{
    // doesn't work for empty file
    
    if (tree->root != NULL) return ERR_FILE; // change err code

    tree->n_nodes = 0;
    int buf_pos = 0;
    char buf[buffer_size] = "";
    fread(buf, sizeof(char), buffer_size, fin);
    
    returnErr(nodeCtor(&tree->root, NULL, NULL, NULL, NULL));
    returnErr(nodeRead(buf, &buf_pos, tree->root, &tree->n_nodes));

    return ERR_OK;
}

ErrEnum treeVerify(Tree* tree)
{
    //

    return ERR_OK;
}

void printNodeDot(FILE* fout, Node* node)
{
    fprintf(fout, "node%p [shape=Mrecord,label=\"" ELEM_FMT "\"]\n", node, ELEM_VAL(node->val));
    if (node->lft != NULL)
    {
        printNodeDot(fout, node->lft);
        fprintf(fout, "node%p->node%p[color=blue]\n", node, node->lft);
    }
    if (node->rgt != NULL)
    {
        printNodeDot(fout, node->rgt);
        fprintf(fout, "node%p->node%p[color=red]\n", node, node->rgt);
    }
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

    fputs("digraph Tree\n{\nrankdir=TB\n", fout);
    if (tree->root == NULL) fputs("NULL [shape=Mrecord]\n", fout);
    else printNodeDot(fout, tree->root);
    fputs("}\n", fout);

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
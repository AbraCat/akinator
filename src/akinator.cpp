#include <string.h>

#include <akinator.h>

const int ans_buf_size = 20;
const char yes_str[] = "да", no_str[] = "нет";

ErrEnum initAkinator(Tree* tree)
{
    myAssert(tree != NULL);
    if (tree->root == NULL) return ERR_NOT_NULL_TREE;

    returnErr(nodeCtor(&tree->root, "Unknown thing", NULL, NULL, NULL));

    return ERR_OK;
}

ErrEnum addEntry(Tree* tree, Node* node, const char* name, const char* diff)
{
    myAssert(tree != NULL && node != NULL && name != NULL && diff != NULL);

    returnErr(nodeCtor(&node->lft, node->val, node, NULL, NULL));
    returnErr(nodeCtor(&node->rgt, name, node, NULL, NULL));
    strncpy(node->val, diff, node_buf_size);
    ++(tree->n_nodes);

    return ERR_OK;
}

ErrEnum scanYesNo(FILE* fin, FILE* fout, int* ans)
{
    char ans_buf[ans_buf_size] = "";
    int ans_atts_cnt = 3, ans_success = 0;
    while (!ans_success)
    {
        fscanf(fin, "%s", ans_buf);
        if (strncmp(ans_buf, no_str, ans_buf_size) == 0)
        {
            *ans = 0;
            ans_success = 1;
            break;
        }
        if (strncmp(ans_buf, yes_str, ans_buf_size) == 0) 
        {
            *ans = 1;
            ans_success = 1;
            break;
        }
        if (--ans_atts_cnt == 0)
        {
            fprintf(fout, "Input attempts exceeded. Goodbye.\n");
            return ERR_INVAL_YES_NO_ANS;
        }
        else fprintf(fout, "Invalid answer. Try again (you have %d more attempts): ", ans_atts_cnt);
    }

    return ERR_OK;
}

ErrEnum play(Tree* tree, FILE* fin, FILE* fout)
{
    myAssert(tree != NULL && fin != NULL && fout != NULL);

    fprintf(fout, "Загадайте объект и отвечайте на вопросы\n\n");

    char ans_buf[ans_buf_size] = "";
    int ans = 0;
    Node* node = tree->root;
    while (node->lft != NULL)
    {
        fprintf(fout, "Ваш объект %s? [%s/%s]", node->val, yes_str, no_str);
        returnErr(scanYesNo(fin, fout, &ans));
        if (ans) node = node->rgt;
        else node = node->lft;
    }

    fprintf(fout, "Ваш объект - %s? [%s/%s]\n", node->val, yes_str, no_str);
    returnErr(scanYesNo(fin, fout, &ans));

    if (ans)
    {
        fprintf(fout, "Я угадал. Ура.\n");
        return ERR_OK;
    }

    fprintf(fout, "Введите название загаданного объекта: ");
    fscanf(fin, "%s", ans_buf);
    char diff_buf[ans_buf_size] = "";
    fprintf(fout, "Чем ваш объект отличается от %s?\n", node->val);
    fscanf(fin, "%s", diff_buf);
    returnErr(addEntry(tree, node, ans_buf, diff_buf));

    fprintf(fout, "Спасибо за помощь в пополнении базы данных\n");
    return ERR_OK;
}

ErrEnum printDescr(FILE* fout, Tree* tree, Node* node)
{
    Node* init_node = node;
    //

    return ERR_OK;
}

ErrEnum printCmp(FILE* fout, Tree* tree, Node* node1, Node* node2)
{
    //
    
    return ERR_OK;
}
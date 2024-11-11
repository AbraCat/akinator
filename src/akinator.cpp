#include <string.h>

#include <akinator.h>
#include <stack.h>
#include <utils.h>

const char yes_str[] = "yes", no_str[] = "no";

ErrEnum akinInit(Tree* tree)
{
    myAssert(tree != NULL);
    
    treeDtor(tree);
    returnErr(treeCtor(tree));
    returnErr(nodeCtor(&tree->root, "Unknown thing", NULL, NULL, NULL));

    return ERR_OK;
}

ErrEnum addEntry(Tree* tree, Node* node, const char* name, const char* diff)
{
    myAssert(tree != NULL && node != NULL && name != NULL && diff != NULL);

    returnErr(nodeCtor(&node->lft, node->val, node, NULL, NULL));
    returnErr(nodeCtor(&node->rgt, name, node, NULL, NULL));
    strncpy(node->val, diff, small_buf_size);
    ++(tree->n_nodes);

    return ERR_OK;
}

ErrEnum scanYesNo(FILE* fin, FILE* fout, int* ans)
{
    char ans_buf[small_buf_size] = "";
    int ans_atts_cnt = 3, ans_success = 0;
    while (!ans_success)
    {
        fscanf(fin, "%s", ans_buf);
        if (strncmp(ans_buf, no_str, small_buf_size) == 0)
        {
            *ans = 0;
            ans_success = 1;
            break;
        }
        if (strncmp(ans_buf, yes_str, small_buf_size) == 0) 
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
        else fputs("Invalid answer. Try again:\n", fout);
    }

    return ERR_OK;
}

ErrEnum akinPlay(Tree* tree, FILE* fin, FILE* fout)
{
    myAssert(tree != NULL && fin != NULL && fout != NULL);

    fprintf(fout, "Загадайте объект и отвечайте на вопросы\n\n");

    char ans_buf[small_buf_size] = "";
    int ans = 0;
    Node* node = tree->root;
    while (node->lft != NULL)
    {
        fprintf(fout, "Ваш объект %s? [%s/%s]\n", node->val, yes_str, no_str);
        returnErr(scanYesNo(fin, fout, &ans));
        if (ans) node = node->rgt;
        else node = node->lft;
    }

    fprintf(fout, "Ваш объект - %s? [%s/%s]\n", node->val, yes_str, no_str);
    returnErr(scanYesNo(fin, fout, &ans));

    if (ans)
    {
        fprintf(fout, "Я угадал. Ура\n");
        return ERR_OK;
    }

    fprintf(fout, "Введите название загаданного объекта: ");
    fscanf(fin, " %[^\n]", ans_buf);
    char diff_buf[small_buf_size] = "";
    fprintf(fout, "Чем ваш объект отличается от %s?\n", node->val);
    fscanf(fin, " %[^\n]", diff_buf);
    returnErr(addEntry(tree, node, ans_buf, diff_buf));

    fprintf(fout, "Спасибо за помощь в пополнении базы данных\n");
    return ERR_OK;
}

ErrEnum printDescr(FILE* fout, Tree* tree, Node* node)
{
    myAssert(fout != NULL && tree != NULL && node != NULL);

    // if (node->lft != NULL) return ERR_NOT_OBJECT;

    Node* init_node = node;
    Stack st = {};
    returnErr(stCtor(&st, 0));

    node = node->parent;
    while (node != NULL)
    {
        returnErr(stPush(&st, node));
        node = node->parent;
    }

    Node* pop_node = NULL;
    fprintf(fout, "Определение %s: ", init_node->val);
    while (st.size > 0)
    {
        returnErr(stPop(&st, &pop_node));
        fprintf(fout, "%s ", pop_node->val);
    }
    putc('\n', fout);

    return ERR_OK;
}

ErrEnum printCmp(FILE* fout, Tree* tree, Node* node1, Node* node2)
{
    myAssert(fout != NULL && tree != NULL && node1 != NULL && node2 != NULL);

    Node* init_node1 = node1, *init_node2 = node2;
    Stack st1 = {}, st2 = {};
    returnErr(stCtor(&st1, 0));
    returnErr(stCtor(&st2, 0));

    node1 = node1->parent;
    while (node1 != NULL)
    {
        returnErr(stPush(&st1, node1));
        node1 = node1->parent;
    }
    node2 = node2->parent;
    while (node2 != NULL)
    {
        returnErr(stPush(&st2, node2));
        node2 = node2->parent;
    }

    Node *pop_node1 = NULL, *pop_node2 = NULL;
    fprintf(fout, "%s похож на %s тем что они оба ", init_node1->val, init_node2->val);
    while (st1.size > 0 && st2.size > 0)
    {
        returnErr(stPop(&st1, &pop_node1));
        returnErr(stPop(&st2, &pop_node2));
        if (pop_node1 != pop_node2) break;
        fprintf(fout, "%s ", pop_node1->val);
    }
    fprintf(fout, ", но они отличаются тем, что %s ", init_node1->val);
    while (st1.size > 0)
    {
        returnErr(stPop(&st1, &pop_node1));
        fprintf(fout, "%s ", pop_node1->val);
    }
    fprintf(fout, ", а %s ", init_node2->val);
    while (st2.size > 0)
    {
        returnErr(stPop(&st2, &pop_node2));
        fprintf(fout, "%s ", pop_node2->val);
    }
    putc('\n', fout);

    return ERR_OK;
}

// const char help_cmd[] = "помощь", quit_cmd[] = "выйти", play_cmd[] = "играть", descr_cmd[] = "описание", 
// cmp_cmd[] = "сравнить", dump_cmd[] = "думп", save_cmd[] = "сохранить", clean_cmd[] = "очистить";

const char database_path[] = "./txt/database.txt";

#define CMD_CASE(cmd) const char cmd ## _cmd[] = #cmd;
CMD_CASE(help)
CMD_CASE(quit)
CMD_CASE(play)
CMD_CASE(descr)
CMD_CASE(cmp)
CMD_CASE(dump)
CMD_CASE(save)
CMD_CASE(clean)
#undef CMD_CASE

void akinHelpMsg(FILE* fout)
{
    myAssert(fout != NULL);

    fprintf(fout, "%s - список команд\n%s - завершить программу\n%s - играть\n%s - составить описание объекта\n%s - сравнить 2 объекта\n"
    "%s - записать состояние дерева в dump-файл\n%s - сохранить базу данных на диск\n%s - очистить базу данных (без сохранения на диск)\n",
    help_cmd, quit_cmd, play_cmd, descr_cmd, cmp_cmd, dump_cmd, save_cmd, clean_cmd);
}

ErrEnum mainAkinCycle()
{
    Tree tree = {};
    handleErr(treeCtor(&tree));

    OPEN_FILE(fdb_in, database_path, "r");
    treeRead(fdb_in, &tree);
    fclose(fdb_in);

    char cmd_buf[small_buf_size] = "", arg_buf1[small_buf_size] = "", arg_buf2[small_buf_size] = "";
    int inval_cmd_cnt = 0;
    while (1)
    {
        printf("Введите команду (\"%s\" для списка команд):\n", help_cmd);
        scanf("%s", cmd_buf);
        if (strcmp(cmd_buf, help_cmd) == 0)
        {
            akinHelpMsg(stdout);
            continue;
        }
        if (strcmp(cmd_buf, quit_cmd) == 0) break;
        if (strcmp(cmd_buf, play_cmd) == 0)
        {
            returnErr(akinPlay(&tree, stdin, stdout));
            continue;
        }
        if (strcmp(cmd_buf, descr_cmd) == 0)
        {
            Node* node = NULL;
            puts("Введите название объекта: ");
            scanf(" %[^\n]", arg_buf1);
            nodeFind(tree.root, arg_buf1, &node);
            if (node == NULL)
            {
                printf("Такого объекта нет (%s)\n", arg_buf1);
                continue;
            }
            returnErr(printDescr(stdout, &tree, node));
            continue;
        }
        if (strcmp(cmd_buf, cmp_cmd) == 0)
        {
            Node *node1 = NULL, *node2 = NULL;

            puts("Введите название первого объекта: ");
            scanf(" %[^\n]", arg_buf1);
            nodeFind(tree.root, arg_buf1, &node1);
            if (node1 == NULL)
            {
                printf("Такого объекта нет (%s)\n", arg_buf1);
                continue;
            }

            puts("Введите название второго объекта: ");
            scanf(" %[^\n]", arg_buf2);
            nodeFind(tree.root, arg_buf2, &node2);
            if (node2 == NULL)
            {
                printf("Такого объекта нет (%s)\n", arg_buf2);
                continue;
            }

            returnErr(printCmp(stdout, &tree, node1, node2));
            continue;
        }
        if (strcmp(cmd_buf, dump_cmd) == 0)
        {
            returnErr(treeDump(&tree));
            continue;
        }
        if (strcmp(cmd_buf, save_cmd) == 0)
        {
            OPEN_FILE(fdb_out, database_path, "w");
            treeWrite(fdb_out, &tree);
            fclose(fdb_out);
            continue;
        }
        if (strcmp(cmd_buf, clean_cmd) == 0)
        {
            returnErr(akinInit(&tree));
            continue;
        }

        ++inval_cmd_cnt;
        if (inval_cmd_cnt == 3)
        {
            puts("Number of input attempts exceeded");
            break;
        }
        printf("Invalid command. Try again (type \"%s\" for help):\n", help_cmd);
    }
    
    treeDtor(&tree);
    return ERR_OK;
}
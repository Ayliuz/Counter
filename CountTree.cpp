//// TREE.CPP

#include "Tree.h"


void tree_Ctor(Tree* tree)
{
    assert(tree);

    tree->tree_guard_begin = GUARD;
    tree->tree_guard_end = GUARD;

    ROOT(tree) = NULL;

    tree->ver_num = 0;

    tree_make_hash(tree);
}

void tree_Dtor(Tree* tree)
{
    assert(tree);

    tree_clear(tree);

    tree->TreeHash_struct = HASHPOIS;

    tree->tree_guard_begin = GUARDPOIS;
    tree->tree_guard_end = GUARDPOIS;
}

void node_Ctor(Node* node)
{
    assert(node);

    node->node_guard_begin = GUARD;
    node->node_guard_end = GUARD;

    PARENT(node) = NULL;
    LEFT(node) = NULL;
    RIGHT(node)= NULL;

    node->info = NAN;
    node->type = NO_TYPE;
}

void node_Dtor(Node* node)
{
    assert(node);

    node->node_guard_begin = GUARDPOIS;
    node->node_guard_end = GUARDPOIS;

    PARENT(node) = NULL;
    LEFT(node) = NULL;
    RIGHT(node)= NULL;

    node->info = NAN;
    node->type = NO_TYPE;
}

void tree_make_hash(Tree* tree)
{
    assert(tree);

    tree->TreeHash_struct = HASHDEFAULT;
    tree->TreeHash_struct = hash (tree, sizeof (*tree));
}

Node* node_create(int type, tree_type val)
{
    Node* new_node = (Node*) calloc(1, sizeof(*new_node));
    node_Ctor(new_node);
    new_node->info = val;
    new_node->type = type;

    return new_node;
}

Node* tree_set_root(Tree* tree, Node* root)
{
    if (!root)
    {
        return ROOT(tree);
    }

    if (ROOT(tree))
    {
        LEFT(root) = LEFT(ROOT(tree));
        RIGHT(root) = RIGHT(ROOT(tree));
        PARENT(root) = PARENT(ROOT(tree));
        node_Dtor(ROOT(tree));
        free(ROOT(tree));
        ROOT(tree) = root;
    }
    else
    {
        ROOT(tree) = root;
        if (!tree->ver_num)
            tree->ver_num++;
    }

    tree_make_hash(tree);

    return ROOT(tree);
}
#define INSERT(pos) \
                if (parent_node->pos)\
                {\
                    parent_node->pos->info = new_val;\
                    parent_node->pos->type = type;\
                    return parent_node->pos;\
                }\
                Node* new_node = node_create(type, new_val);\
                PARENT(new_node) = parent_node;\
                parent_node->pos = new_node;\
                tree->ver_num++;\
                tree_make_hash(tree);

Node* tree_insert_left(Tree* tree, Node* parent_node, int type, tree_type new_val)
{
    assert(tree && parent_node);

    INSERT(left);

    return new_node;
}

Node* tree_insert_right(Tree* tree, Node* parent_node, tree_type new_val)
{
    assert(tree && parent_node);

    INSERT(right);

    return new_node;
}

int tree_erase(Tree* tree, Node* node)
{
    assert(tree && node);

    tree_cut(tree, node);
    tree_make_hash(tree);

    return 0;
}

Tree* tree_clear(Tree* tree)
{
    assert(tree);

    tree_cut(tree, ROOT(tree));

    ROOT(tree) = NULL;

    tree->ver_num = 0;

    tree_make_hash(tree);

    return tree;
}

int tree_cut(Tree* tree, Node* node)
{
    if (!node)
    {
        return 0;
    }

    tree_cut(tree, LEFT(node));
    tree_cut(tree, RIGHT(node));

    switch (node_define(node))
    {
        case L:
            LEFT(PARENT(node)) = NULL;
            break;

        case R:
            RIGHT(PARENT(node)) = NULL;
            break;

        default:;
    }

    node_Dtor(node);
    free(node);

    (tree->ver_num)--;
    return 0;
}

int node_define(Node* node)                       // get side of the node related to the parent
{
    assert(node);

    if (PARENT(node))
    {
        if (LEFT(PARENT(node)) == node)
            return L;
        if (RIGHT(PARENT(node)) == node)
            return R;
    }
    return NOPARENT;
}

Node* node_find(Node* start_node, tree_type val, int (*info_cmp)(tree_type, tree_type))
{
    assert(info_cmp);

    if (!start_node)
    {
        return NULL;
    }

    if (!info_cmp(val, start_node->info))
    {
        return start_node;
    }
    else
    {
        Node* left = node_find(LEFT(start_node), val, info_cmp);
        if (left)
            return left;

        Node* right = node_find(RIGHT(start_node), val, info_cmp);
        if (right)
            return right;

        return NULL;
    }
}

Node* tree_find_common(Node* node1, Node* node2, int* out_node1_position)              // out_node1_position - side of node1 related to common
{
    assert(node1 && node2);

    size_t height1 = node_get_height(node1);
    size_t height2 = node_get_height(node2);

    while (height1 != height2)
    {
        if (height1 > height2)
        {
            node1 = PARENT(node1);
            height1--;
        }
        else
        {
            node2 = PARENT(node2);
            height2--;
        }
    }

    while (node1 != node2)
    {
        *out_node1_position = node_define(node1);
        node1 = PARENT(node1);
        node2 = PARENT(node2);
    }

    return node1;
}

size_t node_get_height(Node* node)
{
    assert(node);

    size_t height = 0;

    while (PARENT(node))
    {
        node = PARENT(node);
        height++;
    }

    return height;
}

void tree_print(Tree* tree, const char WRITENAME[])
{
    FILE* print_file = fopen(WRITENAME, "w");
    assert(tree && print_file);

    node_print(ROOT(tree), print_file);

    fclose(print_file);
}

void node_print(Node* node, FILE* print_file)
{
    assert(print_file);

    if (!node)
    {
        fprintf(print_file, "{nil}");
        return;
    }
    fprintf(print_file, "{%s\n", node->info);

    node_print(LEFT(node), print_file);
    node_print(RIGHT(node), print_file);

    fprintf(print_file, "}\n");
}

Tree* tree_read(const char READNAME[])
{
    Tree* tree = (Tree*) calloc(1, sizeof(*tree));
    tree_Ctor(tree);

    FILE* readtxt = fopen(READNAME, "r");
    assert(readtxt);

    int error = 0;

    tree_set_root(tree, node_read(readtxt, tree, &error));

    if (error)
    {
        fseek(readtxt, error, SEEK_SET);
        printf("~In reading from file \"%s\": char %i(\'%c\') \n\n", READNAME, error, fgetc(readtxt));
    }

    fclose(readtxt);
    return tree;
}

#define SKIPSPACES while(isspace(c = fgetc(read_file)));\
                    ungetc(c, read_file);

#define SYNERROR *error = ftell(read_file);\
                 return NULL;

#define C_READ (c = fgetc(read_file))
#define IFERROR if (*error) return NULL;

Node* node_read(FILE* read_file, Tree* tree, int* error)
{
    assert(read_file && tree && error);

    int c = 0;

    SKIPSPACES;
    if (C_READ == '{')
    {
        Node* node = get_node(read_file, error);
        if (!node)
        {
            return NULL;
        }

        LEFT(node)= node_read(read_file, tree, error);

        if (LEFT(node))
            PARENT(LEFT(node)) = node;

        IFERROR;

        RIGHT(node) = node_read(read_file, tree, error);

        if (RIGHT(node))
            PARENT(RIGHT(node)) = node;

        IFERROR;

        ++(tree->ver_num);

        SKIPSPACES;
        if (C_READ != '}')
        {
            SYNERROR;
        }

        SKIPSPACES;

        return node;
    }
    else
    {
        ungetc(c, read_file);
        SYNERROR;
    }
}

Node* get_node(FILE* read_file, int* error)
{
    assert(read_file && error);
    Node* node = NULL;

    int c = 0;
    SKIPSPACES;
    if (C_READ == 'n')
    {
        int count = 0;
        while(C_READ != '}' && c!= EOF)
        {
            SKIPSPACES;
            if (count >= 3)
            {
                SYNERROR;
            }
            count ++;
        }
        return node;
    }
    ungetc(c, read_file);

    tree_type info = get_str(read_file, error);
    node = node_create(info);

    return node;
}

char* get_str(FILE* read_file, int* error)
{
    assert(read_file);

    char* buff = (char*) calloc(STRLEN, sizeof(char));

    if (!(fgets(buff, STRLEN, read_file)))
    {
        SYNERROR;
    }

    buff = remove_shift(buff, STRLEN);

    return buff;
}

char* remove_shift(char* str, int length)
{
    assert(str);

    for(int i = 0; i < length && str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
        }
    }
    return str;
}
#define DO_REAL_HASH  \
                    int StructHash_buf = tree->TreeHash_struct;\
                    tree->TreeHash_struct = HASHDEFAULT;\
                    int RealHash_buf = hash (tree, sizeof (*tree));

#define RE_STORE tree->TreeHash_struct = StructHash_buf;\

int tree_is_OK(Tree* tree)
{
    assert(tree);
    int error = TRERROK;

    unsigned int counter = 0;

    node_is_OK(ROOT(tree), &counter);

    if (tree->ver_num > counter)
    {
        return TRERLEN;
    }

    DO_REAL_HASH;
    if ((tree->tree_guard_begin != GUARD) || (tree->tree_guard_end != GUARD) ||  RealHash_buf != StructHash_buf)
    {
        return TRERSTRUCT;
    }
    RE_STORE;

    return error;
}


#define IF_RETURN(cond) if (cond) return (cond);

int node_is_OK(Node* node, unsigned int* counter)
{
    if (!node)
    {
        return TRERROK;
    }

    int left_error = node_is_OK(LEFT(node), counter);
    int right_error = node_is_OK(RIGHT(node), counter);

    IF_RETURN(left_error);
    IF_RETURN(right_error);

    if ((node->node_guard_begin != GUARD) || (node->node_guard_end != GUARD))
    {
        return TRERDATA;
    }

    (*counter)++;
    return TRERROK;
}

#define CASE_TREE_OK(ERROR,MESSAGE)\
                                    case ERROR:\
                                    printf("   " MESSAGE "\n");\
                                    break;

int tree_dump(const char dot[], const char DUMPNAME[], Tree* tree)
{
    assert(tree);

    printf("\n~In File: %s\n~In Line: %d\n", __FILE__, __LINE__);
    printf("~Tree [0x%X]\n~{\n   Vertex number = %u\n   Root = [0x%X]\n", (out_ptr) tree, tree->ver_num, (out_ptr) ROOT(tree));
    printf("   Struct_guard_begin  = %s\n", ((tree->tree_guard_begin) == GUARD) ? "GUARD": "ERROR");
    printf("   Struct_guard_end    = %s\n",   ((tree->tree_guard_end) == GUARD) ? "GUARD": "ERROR");

    DO_REAL_HASH;
    printf("   In memory Struct_Hash = %i\n", StructHash_buf);
    printf("        Real Struct_Hash = %i\n", RealHash_buf);
    RE_STORE;
    printf("   INFO: \n");\
    switch(tree_is_OK(tree))
    {
        CASE_TREE_OK(TRERROK,    "Tree is OK");
        CASE_TREE_OK(TRERDATA,   "Data memory in the tree was damaged");
        CASE_TREE_OK(TRERLEN,    "Tree length is not suitable");
        CASE_TREE_OK(TRERPOS,    "Links between tree elements are damaged");
        CASE_TREE_OK(TRERSTRUCT, "Structure of the tree is damaged");
    default:
        printf("   Tree verification failed\n");
        return 1;
    }

    printf("~}\n\n");

    if (tree_draw(dot, DUMPNAME, tree))
        return 1;

    return 0;
}

#define NONE_ND(pos) strcat(dump_str, #pos "%i[shape = none, label = \"\"];\n %i:<" #pos "> -> " #pos "%i[style = \"invis\"];\n")

#define GET_ND(pos, label) strcat(dump_str, "%i:<" #pos "> -> %i [label = \"" #label "\"|;\n")

#define WRITE_ARG   dump_file, dump_str, node, PARENT(node), node->info, node, LEFT(node), RIGHT(node)

#define CASE_EDGE(val, body) case (val):\
                                    body;\
                                    break

#define CHILD_SUM_IF(cond, increm) if (cond)\
                                {\
                                    child += (increm);\
                                }


int node_dump(FILE* dump_file, Node* node)
{
    if (!node)
    {
        return 0;
    }

    char dump_str[150] = "%i [shape = record, label = \" {%p | \\\"%s?\\\" | %p | {<l> %p | <r> %p }} \"];\n";

    int child = 0;
    CHILD_SUM_IF (LEFT(node), 1);
    CHILD_SUM_IF (RIGHT(node),2);

    switch(child)
    {
        CASE_EDGE (1, NONE_ND(r); GET_ND(l, "Да"); fprintf(WRITE_ARG, node, node, node, node, LEFT(node)));
        CASE_EDGE (2, NONE_ND(l); GET_ND(r, "Нет"); fprintf(WRITE_ARG, node, node, node, node, RIGHT(node)));
        CASE_EDGE (3, strcat(dump_str, "%i:<l> -> %i [label = \"Да\"];\n %i:<r> -> %i [label = \"Нет\"];\n");
                    fprintf(WRITE_ARG, node, LEFT(node), node, RIGHT(node)));
    default:
        fprintf(WRITE_ARG);
    }

    node_dump(dump_file, LEFT(node));
    node_dump(dump_file, RIGHT(node));

    return 0;
}

#define TO_GRAPHVIZ(gv_name, DUMPNAME) strdup(DUMPNAME); strcat(gv_name, ".gv")

#define TO_PNG(png_name, DUMPNAME) strdup(DUMPNAME); strcat(png_name, ".png")

#define CONCAT_DOT(arg)    strcat(dotty, arg)


int tree_draw(const char dot[], const char DUMPNAME[], Tree* tree)
{
    char* gv_name = TO_GRAPHVIZ(gv_name, DUMPNAME);  // make grathviz file name
    char* png_name = TO_PNG(png_name, DUMPNAME);     // make bmp file name


    FILE* dump_file = fopen(gv_name, "w");
    if (!dump_file) return 1;

    fprintf(dump_file, "digraph ge\n{\n splines = \"polyline\";\n");

    node_dump(dump_file, ROOT(tree));

    fprintf(dump_file, "}");
    fclose(dump_file);


    char* dotty = (char*) calloc(200, sizeof(*dotty));
    dotty = strcpy(dotty, dot);                         // make
    CONCAT_DOT(gv_name);                                // dot
    //CONCAT_DOT(" -o ");                                 // compile
    //CONCAT_DOT(png_name);                               // command

    system(dotty);                                  //compile graphviz
    //system(png_name);                               //open png

    free(gv_name);
    free(png_name);
    free(dotty);

    return 0;
}

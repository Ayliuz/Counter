// TREE.H

#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <direct.h>
#include "stack.h"

#define NO_TYPE 0
#define CONST 1
#define VAR 2
#define OP 3            // operator

#define L 1
#define R 0
#define NOPARENT -1
#define GUARD 7933
#define GUARDPOIS -1
#define HASHPOIS -1
#define HASHDEFAULT 0
#define TRERROK 0
#define TRERREMOVE 1
#define TRERDATA 2
#define TRERPOS 4
#define TRERSTRUCT 5
#define TRERLEN 6

#ifndef $
#define $(x) std::cout<<"~"<<#x " = "<<x<<"\n";
#endif

#define PARENT(node) (node)->parent
#define LEFT(node) (node)->left
#define RIGHT(node) (node)->right
#define ROOT(tree)  (tree)->root

typedef double tree_type;
typedef unsigned int out_ptr;

const size_t STRLEN = 100;

struct Node
{
    int node_guard_begin = GUARDPOIS;

    Node* parent = NULL;

    tree_type info = NAN;
    int type = NO_TYPE;

    Node* left = NULL;
    Node* right = NULL;

    int node_guard_end = GUARDPOIS;
};

struct Tree
{
    int tree_guard_begin = GUARDPOIS;

    unsigned int ver_num = 0;

    Node* root = NULL;

    int TreeHash_struct = HASHDEFAULT;

    int tree_guard_end = GUARDPOIS;
};

void tree_test(const char dot[]);

void tree_Ctor(Tree* tree);
void tree_Dtor(Tree* tree);

void node_Ctor(Node* node);
void node_Dtor(Node* node);

void tree_make_hash(Tree* tree);

Node* tree_set_root(Tree* tree, Node* root);
Node* node_create(int type, tree_type val);

Node* tree_insert_left(Tree* tree, Node* parent_node, int type, tree_type new_val);
Node* tree_insert_right(Tree* tree, Node* parent_node, int type, tree_type new_val);

Node* node_find(Node* start_node, tree_type val, int (*compare)(tree_type value1, tree_type value2));
int node_define(Node* node);                                            // get side of the node related to the parent

Node* tree_find_common(Node* node1, Node* node2, int* out_node1_position);
size_t node_get_height(Node* node);

Tree* tree_clear(Tree* tree);
int tree_cut(Tree* tree, Node* node);
int tree_erase(Tree* tree, Node* node);

int tree_is_OK(Tree* tree);
int node_is_OK(Node* node, unsigned int* counter);

int tree_dump(const char dot[], const char DUMPNAME[], Tree* tree);
int node_dump(FILE* dump_file, Node* node);
int tree_draw(const char dot[], const char DUMPNAME[], Tree* tree);

void tree_print(Tree* tree, const char PRINTNAME[]);
void node_print(Node* node, FILE* print_file);

Tree* tree_read(const char READNAME[]);
Node* node_read(FILE* read_file, Tree* tree, int* error);
Node* get_node(FILE* read_file, int* error);
char* get_str(FILE* read_file, int* error);
char* remove_shift(char* str, int length);

#endif // TREE_H_INCLUDED

// COUNTER.H

#ifndef COUNTER_H_INCLUDED
#define COUNTER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "CountTree.h"

#ifndef $
#define $(x) std::cout<<"~"<<#x " = "<<x<<"\n";
#endif

#define SynAssert(fu ,expr) if(!(expr))\
                        {\
                            printf("Syntax error: \"%c\", char %i\n", *S, S - BEGIN+1);\
                            CountError = 1;\
                            return NAN;\
                        }\

static const char* S = NULL;
static const char* BEGIN = NULL;
static int CountError = 0;

Node* get_G(Tree* tree, const char* str);
Node* get_E(Tree* tree);
Node* get_T(Tree* tree);
Node* get_P(Tree* tree);
Node* get_N(Tree* tree);

#endif // COUNTER_H_INCLUDED

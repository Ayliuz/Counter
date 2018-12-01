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

int get_G(const char* str);
int get_E();
int get_T();
int get_P();
int get_N();

#endif // COUNTER_H_INCLUDED

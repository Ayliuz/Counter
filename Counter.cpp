// COUNTER.CPP

#include "Counter.h"

Node* get_G(Tree* tr, const char* str)
{
    S = str;
    BEGIN = str;
    CountError = 0;

    Node* result = get_E();
    if(CountError) return NULL;

    SynAssert(G, *S == '\0');
    SynAssert(G, S != str);

    tree_set_root(tr, result);

    return result;
}

Node* get_E(Tree* tr)
{
    const char* old_s = S;

    int val = get_T();
    if(CountError) return NULL;

    while(*S == '+' || *S == '-')
    {
        int op = *S;
        S++;
        int summand = get_T();
        if(op == '+')
        {
            val += summand;
        }
        else
        {
            val -= summand;
        }
    }
    SynAssert(E, S != old_s);

    return val;
}

Node* get_T(Tree* tr)
{
    const char* old_s = S;

    Node* result = get_P();
    if(CountError) return NULL;

    while(*S == '*' || *S == '/')
    {
        int op = *S;
        S++;
        Node* mul = get_P();
        if(op == '*')
        {
            result->info *= mul->info;
        }
        else
        {
            val /= mul;
        }
    }
    SynAssert(T, S != old_s);

    return result;
}

Node* get_P(Tree* tr)
{
    if(CountError) return NULL;

    if(*S == '(')
    {
        S++;
        Node* result = get_E();
        SynAssert(P, *S == ')');
        S++;
        return result;
    }
    else
    {
        return get_N();
    }
}

Node* get_N(Tree* tr)
{
    const char* old_s = S;

    if(CountError) return NULL;

    int val = 0;
    while(isdigit(*S))
    {
        val = val * 10 + (*S - '0');
        S++;
    }
    SynAssert(N, S != old_s);

    return node_create(CONST, val);
}

Node* tree_operate_create(Tree* tr, tree_type oper, tree_type value1, tree_type value2)
{
    assert(tr);

    Node* new_branch = node_create(OP, oper);

    tree_insert_left(tr, new_branch, CONST, val1);
    tree_insert_right(tr, new_branch, CONST, val2);

    return new_branch;
}

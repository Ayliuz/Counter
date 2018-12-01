// COUNTER.CPP

#include "Counter.h"

int get_G(const char* str)
{
    S = str;
    BEGIN = str;
    CountError = 0;

    int val = get_E();
    if(CountError) return NAN;

    SynAssert(G, *S == '\0');
    SynAssert(G, S != str);
    return val;
}

int get_E()
{
    const char* old_s = S;

    int val = get_T();
    if(CountError) return NAN;

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

int get_T()
{
    const char* old_s = S;

    int val = get_P();
    if(CountError) return NAN;

    while(*S == '*' || *S == '/')
    {
        int op = *S;
        S++;
        int mul = get_P();
        if(op == '*')
        {
            val *= mul;
        }
        else
        {
            val /= mul;
        }
    }
    SynAssert(T, S != old_s);
    return val;
}

int get_P()
{
    if(CountError) return NAN;

    if(*S == '(')
    {
        S++;
        int val = get_E();
        SynAssert(P, *S == ')');
        S++;
        return val;
    }
    else
    {
        return get_N();
    }
}

int get_N()
{
    const char* old_s = S;

    if(CountError) return NAN;

    int val = 0;
    while(isdigit(*S))
    {
        val = val * 10 + (*S - '0');
        S++;
    }
    SynAssert(N, S != old_s);
    return val;
}

#include "D:\TX\TXlib.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Counter.h"

const char dot[300] = "\"C:\\Program Files (x86)\\Graphviz2.38\\bin\\gvedit.exe\" -Tbmp ";

const char* DUMPNAME = "Counter";

int main()
{
    txSetConsoleAttr (0xf0);


    Tree Counter;
    tree_Ctor(&Counter);

    char expression[1000] = "";
    scanf("%s", expression);
    printf("%i", get_G(&Counter, expression));

    tree_draw(dot, DUMPNAME, &Counter);
    tree_Dtor(&Counter);
    return 0;
}

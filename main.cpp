#include "D:\TX\TXlib.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Counter.h"


int main()
{
    txSetConsoleAttr (0xf0);

    char expression[1000] = "";
    scanf("%s", expression);
    printf("%i", get_G(expression));
    return 0;
}

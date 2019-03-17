#include "D:\TX\TXlib.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Counter.h"

const char dot[100] = "\"C:\\Program Files (x86)\\Graphviz2.38\\bin\\gvedit.exe\" -Tbmp ";

const char* DUMPNAME  = "Counter";
const char* SAVENAME  = "printCounter.txt";
const char* PRINTNAME = "output";

FILE* out_report = NULL;
Tree* Common_AST = NULL;

int main()
{

    char* tex_name = strdup(PRINTNAME); strcat(tex_name, ".tex");      // make tex file name
    char* pdf_name = strdup(PRINTNAME); strcat(pdf_name, ".pdf");       // make pdf file name


    out_report = fopen(tex_name, "w");
    tex_begin(out_report);

    txSetConsoleAttr (0xf0);

    char expression[1000] = "";//"(3 - (cos(ln(x)))^3)^(1/2)";
    scanf("%s", expression);

    Tree* Counter = get_G(expression);

    Common_AST = Counter;
    if (Counter)
    {
        fprintf(out_report, "$$Initial:$$ \\left(");
        leaf_print(out_report, Common_AST, ROOT(Common_AST));
        fprintf(out_report, "\\right)'");

        AST_simplify(Counter, ROOT(Counter));

        AST_diff("x", Counter, ROOT(Counter));

        AST_simplify(Counter, ROOT(Counter));

        tree_Dtor(Counter);
    }


    tex_end(out_report);
    fclose(out_report);

    tex_launch(tex_name, pdf_name);

    free(Counter);
    free(tex_name);
    free(pdf_name);
    return 0;
}

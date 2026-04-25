#include <stdio.h>

typedef double (*math_func)(double, double);

struct math_op
{
    char name[24];
    math_func func;
};

double add(double x, double y)
{
    return (x + y);
}

double sub(double x, double y)
{
    return (x - y);
}

double mul(double x, double y)
{
    return (x * y);
}

double div(double x, double y)
{
    return (x / y);
}

enum operations
{
    ADD = 0,
    SUB,
    MUL,
    DIV
};

struct math_op ops[] =
{
    { "ADD", add },
    { "SUB", sub },
    { "MUL", mul },
    { "MUL", div },
};

double do_math_operation(enum operations op, double x, double y)
{
    return (ops[op].func(x, y));
}

int main(void)
{
    double x = 10.0;
    double y = 5.0;
    printf("%.2f + %.2f = %.2f\n", x, y, do_math_operation(ADD, x, y));
    printf("%.2f - %.2f = %.2f\n", x, y, do_math_operation(SUB, x, y));
    printf("%.2f * %.2f = %.2f\n", x, y, do_math_operation(MUL, x, y));
    printf("%.2f / %.2f = %.2f\n", x, y, do_math_operation(DIV, x, y));
    return 0;
}

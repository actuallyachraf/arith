#include <stdio.h>

#include "include/fmath.h"

int main()
{

    /* check single init */
    f_int a;
    f_init(&a);
    f_clear(&a);

    /* check multi init */
    f_int b, c, d, e;

    f_init(&b);
    f_init(&c);
    f_init(&d);
    f_init(&e);

    f_clear(&b);
    f_clear(&c);
    f_clear(&d);
    f_clear(&e);

    /* check grow */
    f_int f;
    f_init(&f);
    f_grow(&f, F_PREC * 8);
    f_clear(&f);

    f_digit x = 3431123;
    printf("Hello World !\n");
    printf("%ld \n", x);
}
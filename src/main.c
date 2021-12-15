#include <stdio.h>
#include <assert.h>

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
    /* assert iszero */
    assert(f_iszero(&b));
    f_clear(&b);
    f_clear(&c);

    f_init_set(&d,100000);
    f_init_set(&e,10000);

    assert(f_cmp(&d, &e) == F_GT);

    f_clear(&d);
    f_clear(&e);

    /* check grow */
    f_int f;
    f_init(&f);
    f_grow(&f, F_PREC * 8);
    f_clear(&f);

    /* check copy */
    f_int y;
    f_int z;
    f_init_set(&y,2345678);
    f_init_set(&z,2345678);
    assert(f_cmp(&y, &z) == F_EQ);
    f_clear(&y);
    f_clear(&z);

    f_digit x = 3431123;
    f_int big_x;
    f_init_set(&big_x, x);
    f_clear(&big_x);
    printf("Hello World !\n");
    printf("%ld \n", x);
    printf("Mask (2**k - 1) = %ld \n", F_MASK);
}
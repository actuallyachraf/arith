#include <stdio.h>
#include <assert.h>

#include "include/fmath.h"
#include "include/debug.h"
#include "include/test.h"

int main()
{
    DEBUG("TESTING FMATH\n");
    /* check single init */
    f_int a;
    f_init(&a);
    f_clear(&a);

    /* check multi init */
    f_int b, c, d, e;

    f_init(&b);
    f_init(&c);

    /* assert iszero */
    ASSERT_TRUE(f_iszero(&b));
    ASSERT_TRUE(f_iszero(&c));
    /* cleanup */
    f_clear(&b);
    f_clear(&c);

    /* assert compare */
    f_init_set(&d,100000);
    f_init_set(&e,10000);

    ASSERT_EQ(f_cmp_mag(&d, &e),F_GT);

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
    // TODO: check for z == NULL in  neg/abs init if null
    f_init(&z);
    f_neg(&y, &z);
    ASSERT_TRUE(z.sign == F_NEG);
    ASSERT_EQ(f_cmp(&y, &z), F_GT);
    /* clean up */
    f_clear(&y);
    f_clear(&z);

    f_digit x = 3431123;
    f_int big_x;
    f_init_set(&big_x, x);
    f_clear(&big_x);
}
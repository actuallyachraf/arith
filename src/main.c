#include <stdio.h>
#include <assert.h>

#include "include/arith.h"
#include "include/debug.h"
#include "include/test.h"

void test_init()
{
    /* check single init */
    mp_int a;
    mp_init(&a);
    mp_clear(&a);
}

void test_multi_init()
{
    /* check multi init */
    mp_int b, c;

    mp_init(&b);
    mp_init(&c);
    /* assert iszero */
    ASSERT_TRUE(mp_iszero(&b));
    ASSERT_TRUE(mp_iszero(&c));
    /* cleanup */
    mp_clear(&b);
    mp_clear(&c);
}

void test_cmp()
{
    mp_int d, e;
    /* assert compare */
    mp_init_set(&d, 100000);
    mp_init_set(&e, 10000);

    ASSERT_EQ(mp_cmp_mag(&d, &e), MP_GT);

    mp_clear(&d);
    mp_clear(&e);
}

void test_neg_cmp()
{
    /* check copy */
    mp_int y;
    mp_int z;
    mp_init_set(&y, 2345678);
    // TODO: check for z == NULL in  neg/abs init if null
    mp_init(&z);
    mp_neg(&y, &z);
    ASSERT_TRUE(z.sign == MP_NEG);
    ASSERT_EQ(mp_cmp(&y, &z), MP_GT);
    /* clean up */
    mp_clear(&y);
    mp_clear(&z);
}

void test_grow()
{
    /* check grow */
    mp_int f;
    mp_init(&f);
    mp_grow(&f, MP_PREC * 8);
    mp_clear(&f);
}

void test_s_add() {
    mp_int a, b, c;
    mp_int expected;

    mp_init(&c);
    mp_init_set(&expected, 12345 + 12345);
    mp_init_set(&a, 12345);
    mp_init_set(&b, 12345);

    s_mp_add(&a, &b, &c);

    ASSERT_EQ(mp_cmp(&c, &expected), MP_EQ);

    mp_clear(&a);
    mp_clear(&b);
    mp_clear(&c);
    mp_clear(&expected);
}

void test_s_add_64() {
    mp_int a, b, c;
    mp_int expected;

    mp_init(&c);
    mp_init_set(&expected, 123457891011121314 + 123457891011121314);
    mp_init_set(&a, 123457891011121314);
    mp_init_set(&b, 123457891011121314);

    s_mp_add(&a, &b, &c);

    ASSERT_EQ(mp_cmp(&c, &expected), MP_EQ);

    mp_clear(&a);
    mp_clear(&b);
    mp_clear(&c);
    mp_clear(&expected);
}

void test_s_sub() {
    mp_int a, b, c;
    mp_int expected;

    mp_init(&c);
    mp_init_set(&expected, 12345 - 1234);
    mp_init_set(&a, 12345);
    mp_init_set(&b, 1234);

    s_mp_sub(&a, &b, &c);

    ASSERT_EQ(mp_cmp(&c, &expected), MP_EQ);

    mp_clear(&a);
    mp_clear(&b);
    mp_clear(&c);
    mp_clear(&expected);
}

void test_s_sub_64() {
    mp_int a, b, c;
    mp_int expected;

    mp_init(&c);
    mp_init_set(&expected, 123457891011121314 - 123457891011);
    mp_init_set(&a, 123457891011121314);
    mp_init_set(&b, 123457891011);

    s_mp_sub(&a, &b, &c);

    ASSERT_EQ(mp_cmp(&c, &expected), MP_EQ);

    mp_clear(&a);
    mp_clear(&b);
    mp_clear(&c);
    mp_clear(&expected);
}


int main()
{
    DEBUG("TESTING ARITH\n");
    test_init();
    test_multi_init();
    test_cmp();
    test_neg_cmp();
    test_grow();
    test_s_add();
    test_s_sub();
    test_s_add_64();
    test_s_sub_64();
}
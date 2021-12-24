/* module mp_math_2k implements fast arithmetic on powers of two */
#include "include/arith.h"

// multiply by a power of two this is equivalent
// to a left bitshift.
int mp_mul_2k(mp_int *a, int b, mp_int *c)
{
    mp_digit d;
    int res;

    if (a != c)
    {
        if ((res = mp_copy(a, c)) != MP_OKAY)
        {
            return res;
        }
    }

    if (c->alloc < (int)(c->used + b / MP_DIGIT_BIT + 1))
    {
        if ((res = mp_grow(c, c->used + b / MP_DIGIT_BIT + 1)) != MP_OKAY)
        {
            return res;
        }
    }
    /* shift by as many digits in the bit count */
    if (b >= (int)MP_DIGIT_BIT)
    {
        if ((res = mp_lshd(c, b / MP_DIGIT_BIT)) != MP_OKAY)
        {
            return res;
        }
    }
    /* shift any bit count < DIGIT_BIT */
    d = (mp_digit)(b % MP_DIGIT_BIT);
    if (d != 0)
    {
        register mp_digit *tmpc, shift, mask, r, rr;
        register int x;
        /* bitmask for carries */
        mask = (((mp_digit)1) << d) - 1;

        /* shift for msbs */
        shift = MP_DIGIT_BIT - d;
        /* alias */
        tmpc = c->dp;
        /* carry */
        r = 0;
        for (x = 0; x < c->used; x++)
        {
            /* get the higher bits of the current word */
            rr = (*tmpc >> shift) & mask;
            /* shift the current word and OR in the carry */
            *tmpc = ((*tmpc << d) | r) & MP_MASK;
            ++tmpc;
            /* set the carry to the carry bits of the current word */
            r = rr;
        }
        /* set final carry */
        if (r != 0)
        {
            c->dp[(c->used)++] = r;
        }
    }
    mp_clamp(c);
    return MP_OKAY;
}
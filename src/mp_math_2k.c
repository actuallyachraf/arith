/* module mp_math_2k implements fast arithmetic on powers of two */
#include "include/arith.h"

// multiply by a power of two this is equivalent
// to a left bitshift by b.
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

/* calc a value mod 2**b */
int mp_mod_2k(mp_int *a, int b, mp_int *c)
{
    int x,res;
    /* if b is <= 0 then zero the int */
    if (b <= 0)
    {
        mp_zero(c);
        return MP_OKAY;
    }
    /* if the modulus is larger than the value than return */
    if (b >= (int)(a->used * MP_DIGIT_BIT))
    {
        res = mp_copy(a, c);
        return res;
    }
    /* copy */
    if ((res = mp_copy(a, c)) != MP_OKAY)
    {
        return res;
    }
    /* zero digits above the last digit of the modulus */
    for (x = (b / MP_DIGIT_BIT) + ((b % MP_DIGIT_BIT) == 0 ? 0 : 1);
         x < c->used; x++)
    {
        c->dp[x] = 0;
    }
    /* clear the digit that is not completely outside/inside the modulus */
    c->dp[b / MP_DIGIT_BIT] &=
        (mp_digit)((((mp_digit)1) << (((mp_digit)b) % MP_DIGIT_BIT)) -
                   ((mp_digit)1));
    mp_clamp(c);
    return MP_OKAY;
}

// divide by a power of two this is equivalent
// to a right bitshift by b.
// division integer based with the quotient in c
// and any remainder in d.
int mp_div_2k(mp_int *a, int b, mp_int *c, mp_int *d)
{
    mp_digit D, r, rr;
    int x, res;
    mp_int t;

    // if the shift count is less than zero
    // return early.
    if (b <= 0)
    {
        res = mp_copy(a, c);
        if (d != NULL)
        {
            mp_zero(d);
        }
        return res;
    }
    if ((res = mp_init(&t)) != MP_OKAY)
    {
        return res;
    }
    // get the remainder first
    if (d != NULL)
    {
        if ((res = mp_mod_2k(a, b, &t)) != MP_OKAY)
        {
            mp_clear(&t);
            return res;
        }
    }
    // copy a to c.
    if ((res = mp_copy(a, c)) != MP_OKAY)
    {
        mp_clear(&t);
        return res;
    }
    // shift by equivalent digit to bitcount
    if (b >= MP_DIGIT_BIT)
    {
        mp_rshd(c, b / MP_DIGIT_BIT);
    }
    // else shift by equivalents bits.
    /* shift by as many digits in the bit count */
    if (b >= (int)MP_DIGIT_BIT)
    {
        mp_rshd(c, b / MP_DIGIT_BIT);
    }
    /* shift any bit count < MP_DIGIT_BIT */
    D = (mp_digit)(b % MP_DIGIT_BIT);
    if (D != 0)
    {
        register mp_digit *tmpc, mask, shift;
        /* mask */
        mask = (((mp_digit)1) << D) - 1;
        /* shift for lsb */
        shift = MP_DIGIT_BIT - D;
        /* alias */
        tmpc = c->dp + (c->used - 1);
        /* carry */
        r = 0;
        for (x = c->used - 1; x >= 0; x--)
        {
            /* get the lower bits of this word in a temp */
            rr = *tmpc & mask;
            /* shift current word and mix carry bits from the previous word */
            *tmpc = (*tmpc >> D) | (r << shift);
            --tmpc;
            /* set the carry to the bits of the current word found above */
            r = rr;
        }
    }
    mp_clamp(c);
    if (d != NULL)
    {
        mp_swap(&t, d);
    }
    mp_clear(&t);
    return MP_OKAY;
}

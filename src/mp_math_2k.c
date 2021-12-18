/* module mp_math_2k.c implements math operations on 2^k */
#include "include/arith.h"

/* mp _mul_2 is a fast implementation of mul by 2 */
int mp_mul_2(mp_int *a, mp_int *b)
{
    /**
     * @brief
     * multiply a by 2 and store the result in b.
     */
    int x, res, oldused;

    /* grow b as necessary */
    if (b->alloc < a->used + 1)
    {
        if ((res = mp_grow(b, a->used + 1)) != MP_OKAY)
        {
            return res;
        }
    }
    oldused = b->used;
    b->used = a->used;

    {
        register mp_digit r, rr, *tmpa, *tmpb;

        /* set pointer aliases */
        tmpa = a->dp;
        tmpb = b->dp;

        /* carry */
        r = 0;
        for (x = 0; x < a->used; x++)
        {
            /**
             * fetch the next carry from the MSB of the current digit
             */
            rr = *tmpa >> ((mp_digit)(MP_DIGIT_BIT - 1));
            /* shift the current digit and add in the carry */
            *tmpb++ = ((*tmpa++ << ((mp_digit)1)) | r) & MP_MASK;
            /**
             * propagate the carry to the next iteration
             */
            r = rr;
        }
        /**
             * if there's a new leading digit add 1.
             */
        if (r != 0)
        {
            *tmpb = 1;
            ++(b->used);
        }
        /**
             * zero any leading digits
             */
        tmpb = b->dp + b->used;
        for (x = b->used; x < oldused; x++)
        {
            *tmpb++ = 0;
        }
    }
    b->sign = a->sign;
    return MP_OKAY;
}

/* mp_div_2 implements division by two */
int mp_div_2(mp_int *a, mp_int *b)
{
    int
        x,
        res, oldused;
    /* copy */
    if (b->alloc < a->used)
    {
        if ((res = mp_grow(b, a->used)) != MP_OKAY)
        {
            return res;
        }
    }
    oldused = b->used;
    b->used = a->used;
    {
        register mp_digit r, rr, *tmpa, *tmpb;
        /* source alias */
        tmpa = a->dp + b->used - 1;
        /* dest alias */
        tmpb = b->dp + b->used - 1;
        /* carry */
        r = 0;
        for (x = b->used - 1; x >= 0; x--)
        {
            /* get the carry for the next iteration */
            rr = *tmpa & 1;
            /* shift the current digit, add in carry and store */
            *tmpb-- = (*tmpa-- >> 1) | (r << (MP_DIGIT_BIT - 1));
            /* forward carry to next iteration */
            r = rr;
        }
        /* zero excess digits */
        tmpb = b->dp + b->used;
        for (x = b->used; x < oldused; x++)
        {
            *tmpb++ = 0;
        }
    }
    b->sign = a->sign;
    mp_clamp(b);
    return MP_OKAY;
}
#include "include/arith.h"

/**
 * @brief file s_mp_mul_comba implements comba multiplier
 * method based on HAC 14.12.
 */
int s_mp_mul_comba(mp_int *a, mp_int *b, mp_int *c, int digs)
{
    int oldused, pa, ix;
    mp_err err;
    mp_digit W[MP_WARRAY];
    mp_word _W;

    /* grow the destination as required */
    if ((err = mp_grow(c, digs)) != MP_OKAY)
    {
        return err;
    }

    /* number of output digits to produce */
    pa = MP_MIN(digs, a->used + b->used);

    /* clear the carry */
    _W = 0;
    for (ix = 0; ix < pa; ix++)
    {
        int tx, ty, iy, iz;

        /* get offsets into the two bignums */
        ty = MP_MIN(b->used - 1, ix);
        tx = ix - ty;

        /* this is the number of times the loop will iterrate, essentially
         while (tx++ < a->used && ty-- >= 0) { ... }
       */
        iy = MP_MIN(a->used - tx, ty + 1);

        /* execute loop */
        for (iz = 0; iz < iy; ++iz)
        {
            _W += (mp_word)a->dp[tx + iz] * (mp_word)b->dp[ty - iz];
        }

        /* store term */
        W[ix] = (mp_digit)_W & MP_MASK;

        /* make next carry */
        _W = _W >> (mp_word)MP_DIGIT_BIT;
    }

    /* setup dest */
    oldused = c->used;
    c->used = pa;

    for (ix = 0; ix < pa; ix++)
    {
        /* now extract the previous digit [below the carry] */
        c->dp[ix] = W[ix];
    }

    /* clear unused digits [that existed in the old copy of c] */
    mp_zero_digs(c->dp + c->used, oldused - c->used);

    mp_clamp(c);
    return MP_OKAY;
}
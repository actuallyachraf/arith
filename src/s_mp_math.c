/*  s_math modules defines low level operations not part
    of the public API.
    Operations in s_math are considered low_level.
*/
#include "include/arith.h"

int s_mp_add(mp_int* a, mp_int* b, mp_int* c){
    mp_int *x;
    int olduse, res, min, max;

    if (a->used > b->used) {
        min = b->used;
        max = a->used;
        x = a;
    } else {
        min = a->used;
        max = b->used;
        x = b;
    }
    if (c->alloc < max + 1) {
        if((res = mp_grow(c,max+1)) != MP_OKAY) {
            return res;
        }
    }
    olduse = c->used;
    c->used = max + 1;
    {
        register mp_digit u, *tmpa, *tmpb, *tmpc;
        register int i;

        /* alias points to digit arrays */
        tmpa = a->dp;
        tmpb = b->dp;
        tmpc = c->dp;

        /* zero the carry */
        u = 0;
        for (i = 0; i < min;i++) {
            *tmpc = *tmpa++ + *tmpb++ + u;
            /* set the carry */
            u = *tmpc >> ((mp_digit)MP_DIGIT_BIT);
            /* take away carry bit from c[i] */
            *tmpc++ &= MP_MASK;
        }
        /* once lo-words are added and carried we treat hi-words */
        if (min != max) {
            for (; i < max;i++){
                *tmpc = x->dp[i] + u;
                /* set the carry */
                u = *tmpc >> ((mp_digit)MP_DIGIT_BIT);
                /* take away carry from c[i] */
                *tmpc++ &= MP_MASK;
            }
        }
        /* set the last carry */
        *tmpc++ = u;

        /* clear unused digits */
        /* TODO: this could be abstracted away */
        for (i = c->used; i < olduse;i++){
            *tmpc++ = 0;
        }
    }
    /* clamp any tailing zeroes */
    mp_clamp(c);
    return MP_OKAY;
}
/*  s_math modules defines low level operations not part
    of the public API.
    Operations in s_math are considered low_level.
*/
#include "include/arith.h"

/* low level addition */
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
            /*
             this branch might seem confused but the trick is
             in understanding what right-shift does.
             a right-shift operator of the form a >> k
             is equivalent of dividing a by 2^k.
             c[i] = a[i] + b[i] + carry.
             divding c[i] by 2^60 allows us to truncate c[i]
             in the radix allowed range.
            */
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

/* low level substraction assumes |a| > |b| */
int s_mp_sub(mp_int* a, mp_int *b, mp_int *c) {
    int olduse, res, min, max;

    /* based on the assumption that |a| > |b| */
    min = b->used;
    max = a->used;

    /* init c allocs */
    if (c->alloc < max) {
        if ((res = mp_grow(c, max)) != MP_OKAY) {
            return res;
        }
    }
    olduse = c->used;
    c->used = max;

    {
        register mp_digit u, *tmpa, *tmpb, *tmpc;
        register int i;

        /* setup pointer aliases */
        /**
         * @note
         * this should "in theory" save us a few cycles spent on
         * pointer chasing
        */
        tmpa = a->dp;
        tmpb = b->dp;
        tmpc = c->dp;

        /* set the carry to zero */
        u = 0;
        /* start the sub loop */
        for (i = 0; i < min;i++) {
            // c[i] = a[i] - b[i] -u
            *tmpc = *tmpa++ - *tmpb++ - u;
            /**
             * @note
             * assume we have a carry u > 0
             * then the carry u will propage to the msb
             * on successive substractions.
             * the reason this is even possible is
             * that unsigned types (used to represent digits)
             * overflow on sucessive operations UINT64_MAX + 1 = 1
             * and 0U - 1 = UINT64_MAX.
             * ref : arith.h we have mp_digit = uint64_t
             * ref2 : https://stackoverflow.com/a/12335930/16185897
             */
            u = *tmpc >> ((mp_digit)(CHAR_BIT * sizeof(mp_digit) - 1));

            /**
             * @note
             * once the operation above is done we remove the carry from c[i]
             * reminder : carry operations on unsigned types overlap
             * so we need to cache the carry by essentially substracting it
             * from c[i] to get the correct c[i] value.
             */
            *tmpc++ &= MP_MASK;
        }
        /**
         * @note similar to s_mp_add after adding the lower bits
         * we (the bound on the loop is min) we need to hi bits of a[i]
         */
        for (; i < max;i++){
            /* c[i] = a[i] -u */
            *tmpc = *tmpa++ - u;
            u = *tmpc >> ((mp_digit)(CHAR_BIT * sizeof(mp_digit) - 1));
            *tmpc++ &= MP_MASK;
        }
        /* clear c[i] higher bits */
        for (i = c->used; i < olduse;i++){
            *tmpc++ = 0;
        }
    }
    mp_clamp(c);
    return MP_OKAY;
}
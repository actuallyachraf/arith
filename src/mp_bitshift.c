/* module mp_bitshift implements bitshift operations on mp_int */
#include "include/arith.h"

/**
 * @brief mp_lshd left shift a by b-digits.
 * the operation acts on offset digits of a->dp.
 * meaning the shift occurs by displacing hi-lo bits
 * here named top-bottom by an offset of length b.
 */
int mp_lshd(mp_int* a, int b){
    int x, res;

    /* if b < 0 do nothing */
    if (b <= 0) {
        return MP_OKAY;
    }
    if (a->alloc < a->used + b) {
        if ((res = mp_grow(a,a->used+b)) != MP_OKAY){
            return res;
        }
    }
    {
        register mp_digit *top, *bottom;
        a->used += b;

        /* top */
        top = a->dp + a->used - 1;
        /* bottom */
        bottom = a->dp + a->used - 1 - b;
        /**
         * @note similar to mp_rshd this part works
         * in a sliding window manner, copying from
         * bottom to top.
         * the goal here is to essentially "shift"
         * the digits from top to the bottom.
         */
        for (x = a->used - 1; x >= b;x--) {
            *top-- = *bottom--;
        }
        top = a->dp;
        for (x = 0; x < b;x++){
            *top++ = 0;
        }
     }
     return MP_OKAY;
}

/**
 * @brief mp_rshd right shifts a by b-digits.
 */
int mp_rshd(mp_int* a, int b) {
    int x, res;
    if (b <= 0) {
        return MP_OKAY;
    }
    /**
     * @note if a has less digits than b
     * then a right shift by b will zero-out
     * all a digits.
     */
    if (a->used <= b) {
        mp_zero(a);
        return MP_OKAY;
    }
    {
        register mp_digit *top, *bottom;

        /* top */
        top = a->dp + b;
        /* bottom */
        bottom = a->dp;

        /**
         * @note similarly to lshd above the operation
         * acts using a sliding window of size b.
         * in the case of left shifts the window
         * copies lo-bits into hi-bits.
         * in the case of right shifts we do the opposite
         * hi-bits are copied into lo-bits.
         * e.g |b0|b1|bk|bk+1|bn
         * hi bits are the rightmost ones.
         */
        for (x = 0; x < (a->used - b);x++) {
            *bottom++ = *top++;
        }
        /* zero the lo digits */
        for (; x < a->used;x++) {
            *bottom++ = 0;
        }
    }
    /* clamp a by removing excess digits */
    a->used -= b;
    return MP_OKAY;
}
#include "include/arith.h"

/* trim unused leading zeroes */
void mp_clamp(mp_int* a) {
    /* while the most significant digit is zero decrement used */
    while (a->used >0 && a->dp[a->used-1] == 0){
        --(a->used);
    }
    /* if used = 0 reset the sign */
    if (a->used == 0){
        a->sign = MP_ZPOS;
    }
}
#include "include/fmath.h"

/* trim unused leading zeroes */
void f_clamp(f_int* a) {
    /* while the most significant digit is zero decrement used */
    while (a->used >0 && a->dp[a->used-1] == 0){
        --(a->used);
    }
    /* if used = 0 reset the sign */
    if (a->used == 0){
        a->sign = F_ZPOS;
    }
}
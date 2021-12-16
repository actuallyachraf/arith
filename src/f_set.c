#include "include/arith.h"


/* set a to digit */
void mp_set(mp_int*a, mp_digit b) {
    mp_zero(a);
    a->dp[0] = b & MP_MASK;
    a->sign = MP_ZPOS;
    a->used = (a->dp[0] != 0) ? 1 : 0;
}
#include "include/fmath.h"


/* set a to digit */
void f_set(f_int*a, f_digit b) {
    f_zero(a);
    a->dp[0] = b & F_MASK;
    a->sign = F_ZPOS;
    a->used = (a->dp[0] != 0) ? 1 : 0;
}
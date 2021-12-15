#include "../include/fmath.h"


/* init a new f_int */
int f_init(f_int* a) {
    int i = 0;

    /* allocate necessary memory and zero it */
    a->dp = (f_digit*) F_ALLOC(sizeof(f_digit) * F_PREC);
    if (a->dp == NULL) {
        return F_MEM;
    }
    for (i = 0; i < F_PREC;i++){
        a->dp[i] = 0;
    }
    a->used = 0;
    a->alloc = F_PREC;
    a->sign = F_ZPOS;

    return F_OKAY;
}

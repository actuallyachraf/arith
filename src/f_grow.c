#include "include/fmath.h"

/* grow allocated array by requested */
int f_grow(f_int* a,int size) {
    int i = 0;
    f_digit *tmp;
    /* if the allocated size is smaller allocate more*/
    if (a->alloc < size){
        /* ensure at least MP_PREC */
        size += (F_PREC * 2) - (size % F_PREC);
        tmp = (f_digit *)F_REALLOC(a->dp, sizeof(f_digit) * size);
        if (tmp == NULL) {
            return F_MEM;
        }
        a->dp = tmp;
        /* zero excess digits */
        i = a->alloc;
        a->alloc = size;
        for (; i < a->alloc;i++){
            a->dp[i] = 0;
        }
    }
    return F_OKAY;
}
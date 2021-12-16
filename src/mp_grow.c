#include "include/arith.h"

/* grow allocated array by requested */
int mp_grow(mp_int* a,int size) {
    int i = 0;
    mp_digit *tmp;
    /* if the allocated size is smaller allocate more*/
    if (a->alloc < size){
        /* ensure at least MP_PREC */
        size += (MP_PREC * 2) - (size % MP_PREC);
        tmp = (mp_digit *)MP_REALLOC(a->dp, sizeof(mp_digit) * size);
        if (tmp == NULL) {
            return MP_MEM;
        }
        a->dp = tmp;
        /* zero excess digits */
        i = a->alloc;
        a->alloc = size;
        for (; i < a->alloc;i++){
            a->dp[i] = 0;
        }
    }
    return MP_OKAY;
}
#include "include/fmath.h"

/* basic mathematical operations */

/* compute the absolute value b = |a| */
int f_abs(f_int* a,f_int* b){
    int res;

    if (a != b){
        if((res=f_copy(a,b)) != F_OKAY){
            return res;
        }
    }
    b->sign = F_ZPOS;
    return F_OKAY;
}

/* compute the negation b = -a */
int f_neg(f_int* a,f_int* b) {
    int res;
     if (a != b){
        if((res=f_copy(a,b)) != F_OKAY){
            return res;
        }
    }
    if (f_iszero(b) != true) {
        b->sign = (a->sign == F_ZPOS) ? F_NEG : F_ZPOS;
    } else {
        b->sign = F_ZPOS;
    }
    return F_OKAY;
}
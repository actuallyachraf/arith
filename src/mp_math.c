#include "include/arith.h"

/* basic mathematical operations */

/* compute the absolute value b = |a| */
int mp_abs(mp_int* a,mp_int* b){
    int res;

    if (a != b){
        if((res=mp_copy(a,b)) != MP_OKAY){
            return res;
        }
    }
    b->sign = MP_ZPOS;
    return MP_OKAY;
}

/* compute the negation b = -a */
int mp_neg(mp_int* a,mp_int* b) {
    int res;
     if (a != b){
        if((res=mp_copy(a,b)) != MP_OKAY){
            return res;
        }
    }
    if (mp_iszero(b) != true) {
        b->sign = (a->sign == MP_ZPOS) ? MP_NEG : MP_ZPOS;
    } else {
        b->sign = MP_ZPOS;
    }
    return MP_OKAY;
}
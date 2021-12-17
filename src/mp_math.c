#include "include/arith.h"

/* basic mathematical operations */

/* compute addition */
int mp_add(mp_int* a, mp_int *b, mp_int *c) {

    int sa, sb, res;
    sa = a->sign;
    sb = b->sign;
    if (sa == sb)
    {
        c->sign = sa;
        res = s_mp_add(a, b, c);
    } else {
        if(mp_cmp_mag(a,b) == MP_LT) {
            c->sign = sb;
            res = s_mp_sub(b, a, c);
        } else {
            c->sign = sa;
            res = s_mp_sub(a, b, c);
        }
    }
    return res;
}

/* compute substraction */
int mp_sub(mp_int* a, mp_int* b, mp_int* c) {
    int sa, sb, res;
    sa = a->sign;
    sb = b->sign;

    if (sa != sb) {
        c->sign = sa;
        res = s_mp_add(a, b, c);
    } else {
        if(mp_cmp_mag(a,b) != MP_LT) {
            c->sign = sa;
            res = s_mp_sub(a, b, c);
        } else {
            c->sign = (sa == MP_ZPOS) ? MP_NEG : MP_ZPOS;
            res = s_mp_sub(b, a, c);
        }
    }
    return res;
}
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
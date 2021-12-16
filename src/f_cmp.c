#include "include/arith.h"

/* mp_cmp_mag compares unsigned mp_int based on their digit values */
int mp_cmp_mag(mp_int*a , mp_int*b){
    int n;
    mp_digit *tmpa, *tmpb;

    if (a->used > b->used) {
        return MP_GT;
    }
    if (a->used < b->used) {
        return MP_LT;
    }

    tmpa = a->dp + (a->used - 1);
    tmpb = b->dp + (a->used - 1);

    for (n = 0; n < a->used;++n,--tmpa,--tmpb) {
        if(*tmpa > *tmpb) {
            return MP_GT;
        }
        if (*tmpa < *tmpb) {
            return MP_LT;
        }
    }
    return MP_EQ;
}

/* mp_cmp compares signed/unsigned mp_int based on their sign and digit values */
int mp_cmp(mp_int *a, mp_int* b){
    /* compare based on sign */
    if (a->sign != b->sign) {
        if(a->sign == MP_NEG){
            return MP_LT;
        } else {
            return MP_GT;
        }
    }
    if (a->sign == MP_NEG) {
        return mp_cmp_mag(b, a);
    } else {
        return mp_cmp_mag(a, b);
    }
}
#include "include/fmath.h"

/* f_cmp_mag compares unsigned f_int based on their digit values */
int f_cmp_mag(f_int*a , f_int*b){
    int n;
    f_digit *tmpa, *tmpb;

    if (a->used > b->used) {
        return F_GT;
    }
    if (a->used < b->used) {
        return F_LT;
    }

    tmpa = a->dp + (a->used - 1);
    tmpb = b->dp + (a->used - 1);

    for (n = 0; n < a->used;++n,--tmpa,--tmpb) {
        if(*tmpa > *tmpb) {
            return F_GT;
        }
        if (*tmpa < *tmpb) {
            return F_LT;
        }
    }
    return F_EQ;
}

/* f_cmp compares signed/unsigned f_int based on their sign and digit values */
int f_cmp(f_int *a, f_int* b){
    /* compare based on sign */
    if (a->sign != b->sign) {
        if(a->sign == F_NEG){
            return F_LT;
        } else {
            return F_GT;
        }
    }
    if (a->sign == F_NEG) {
        return f_cmp_mag(b, a);
    } else {
        return f_cmp_mag(a, b);
    }
}
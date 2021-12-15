#include "include/fmath.h"

int f_cmp(f_int*a , f_int*b){
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
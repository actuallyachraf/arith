#include "include/fmath.h"


int f_copy(f_int* a, f_int* b) {

    int res, n;
    if (a == b) {
        return F_OKAY;
    }
    if(b->alloc < a->alloc) {
        if((res = f_grow(b,a->used)) != F_OKAY) {
            return res;
        }
    }
    register f_digit *tmp_a, *tmp_b;
    tmp_a = a->dp;
    tmp_b = b->dp;

    for (n = 0; n < a->used;n++) {
        *tmp_b++ = *tmp_a++;
    }
    for (; n < b->used;n++){
        *tmp_b++ = 0;
    }
    b->used = a->used;
    b->sign = a->sign;
    return F_OKAY;
}
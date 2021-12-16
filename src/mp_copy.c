#include "include/arith.h"


int mp_copy(mp_int* a, mp_int* b) {

    int res, n;
    if (a == b) {
        return MP_OKAY;
    }
    if(b->alloc < a->alloc) {
        if((res = mp_grow(b,a->used)) != MP_OKAY) {
            return res;
        }
    }
    register mp_digit *tmp_a, *tmp_b;
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
    return MP_OKAY;
}
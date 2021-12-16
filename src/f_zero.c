#include "include/arith.h"


/* zero an mp_int */
void mp_zero(mp_int* a) {
    int n = 0;
    mp_digit *tmp;

    a->sign = MP_ZPOS;
    a->used = 0;

    tmp = a->dp;
    for (n = 0; n < a->alloc;n++){
        *tmp++ = 0;
    }
}
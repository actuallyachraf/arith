#include "include/fmath.h"


/* zero an f_int */
void f_zero(f_int* a) {
    int n = 0;
    f_digit *tmp;

    a->sign = F_ZPOS;
    a->used = 0;

    tmp = a->dp;
    for (n = 0; n < a->alloc;n++){
        *tmp++ = 0;
    }
}
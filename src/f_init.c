#include "include/fmath.h"


/* init a new f_int */
int f_init(f_int* a) {
    int i = 0;

    /* allocate necessary memory and zero it */
    a->dp = (f_digit*) F_ALLOC(sizeof(f_digit) * F_PREC);
    if (a->dp == NULL) {
        return F_MEM;
    }
    for (i = 0; i < F_PREC;i++){
        a->dp[i] = 0;
    }
    a->used = 0;
    a->alloc = F_PREC;
    a->sign = F_ZPOS;

    return F_OKAY;
}

/* init a new f_int with predefined precision */
int f_init_size(f_int* a, int size) {
    int i;
    /* pad size to avoid trivial allocations */
    size += (F_PREC * 2) - (size % F_PREC);

    /* allocate the memory */
    a->dp = (f_digit *)F_ALLOC(sizeof(f_digit) * size);
    if (a->dp == NULL) {
        return F_MEM;
    }
    /* set the members */
    a->used = 0;
    a->alloc = size;
    a->sign = F_ZPOS;
    for (i = 0; i < size;i++){
        a->dp[i] = 0;
    }
    return F_OKAY;
}

/* init multiple f_int using variadic arguments */
// TODO: fix leak in f_init_multi
#include "../include/debug.h"
#include <stdarg.h>
int f_init_multi(f_int* a,...){
    f_err res = F_OKAY;
    int n = 0;
    f_int *curr_arg = a;
    va_list args;

    va_start(args, a);
    while (curr_arg != NULL) {
        if(f_init(curr_arg) != F_OKAY) {
            DEBUG("ALLOCATION FAILED");
            /* if one init fails we backtrack and return */
            va_list clean_args;
            /* start cleaning up */
            curr_arg = a;
            va_start(clean_args, a);
            while(n-- != 0) {
                f_clear(curr_arg);
                curr_arg = va_arg(clean_args, f_int*);
            }
            va_end(clean_args);
            res = F_MEM;
            break;
        }
        DEBUG("ONE ALLOCATION SUCCEEDED");
        n++;
        curr_arg = va_arg(args, f_int *);
    }
    DEBUG("ALL ALLOCATION SUCCEEDED");
    va_end(args);
    return res;
}
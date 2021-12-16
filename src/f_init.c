#include "include/arith.h"


/* init a new mp_int */
int mp_init(mp_int* a) {
    int i = 0;

    /* allocate necessary memory and zero it */
    a->dp = (mp_digit*) MP_ALLOC(sizeof(mp_digit) * MP_PREC);
    if (a->dp == NULL) {
        return MP_MEM;
    }
    for (i = 0; i < MP_PREC;i++){
        a->dp[i] = 0;
    }
    a->used = 0;
    a->alloc = MP_PREC;
    a->sign = MP_ZPOS;

    return MP_OKAY;
}

/* init a new mp_int with predefined precision */
int mp_init_size(mp_int* a, int size) {
    int i;
    /* pad size to avoid trivial allocations */
    size += (MP_PREC * 2) - (size % MP_PREC);

    /* allocate the memory */
    a->dp = (mp_digit *)MP_ALLOC(sizeof(mp_digit) * size);
    if (a->dp == NULL) {
        return MP_MEM;
    }
    /* set the members */
    a->used = 0;
    a->alloc = size;
    a->sign = MP_ZPOS;
    for (i = 0; i < size;i++){
        a->dp[i] = 0;
    }
    return MP_OKAY;
}


/* init with a digit value */

/* initialize and set a digit */
int mp_init_set(mp_int *a, mp_digit b)
{
   int err;
   if ((err = mp_init(a)) != MP_OKAY) {
      return err;
   }
   mp_set(a, b);
   return err;
}

/* init a with a copy of b */
// TODO: use dst,src naming for copy, move operations.
int mp_init_copy(mp_int* a,mp_int *b) {
    int res;
    if((res= mp_init(a)) != MP_OKAY) {
        return res;
    }
    return mp_copy(b,a);
}

/* init multiple mp_int using variadic arguments */
// TODO: fix leak in mp_init_multi
#include "../include/debug.h"
#include <stdarg.h>
int mp_init_multi(mp_int* a,...){
    mp_err res = MP_OKAY;
    int n = 0;
    mp_int *curr_arg = a;
    va_list args;

    va_start(args, a);
    while (curr_arg != NULL) {
        if(mp_init(curr_arg) != MP_OKAY) {
            DEBUG("ALLOCATION FAILED");
            /* if one init fails we backtrack and return */
            va_list clean_args;
            /* start cleaning up */
            curr_arg = a;
            va_start(clean_args, a);
            while(n-- != 0) {
                mp_clear(curr_arg);
                curr_arg = va_arg(clean_args, mp_int*);
            }
            va_end(clean_args);
            res = MP_MEM;
            break;
        }
        DEBUG("ONE ALLOCATION SUCCEEDED");
        n++;
        curr_arg = va_arg(args, mp_int *);
    }
    DEBUG("ALL ALLOCATION SUCCEEDED");
    va_end(args);
    return res;
}
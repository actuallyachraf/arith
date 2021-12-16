#include "include/arith.h"
#include "../include/debug.h"

/* free an mp_int */
int mp_clear(mp_int* a){
    int i = 0;

    /* avoid double frees */
    if (a->dp != NULL) {
        for (i = 0; i < a->used;i++){
            a->dp[i] = 0;
        }
        /* free heap allocated array */
        MP_CLEAR(a->dp);
        /* reset mp_int members */
        a->dp = NULL;
        a->alloc = 0;
        a->used = 0;
        a->sign = MP_ZPOS;
    }
    return MP_OKAY;
}

/* free multiples */
#include <stdarg.h>
void mp_clear_multi(mp_int *a, ...)
{
   va_list args;
   va_start(args, a);
   while (a != NULL) {
       mp_clear(a);
       a = va_arg(args, mp_int *);
   }
   va_end(args);
}
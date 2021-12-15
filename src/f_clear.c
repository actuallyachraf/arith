#include "include/fmath.h"
#include "../include/debug.h"

/* free an f_int */
int f_clear(f_int* a){
    int i = 0;

    /* avoid double frees */
    if (a->dp != NULL) {
        for (i = 0; i < a->used;i++){
            a->dp[i] = 0;
        }
        /* free heap allocated array */
        F_CLEAR(a->dp);
        /* reset f_int members */
        a->dp = NULL;
        a->alloc = 0;
        a->used = 0;
        a->sign = F_ZPOS;
    }
    return F_OKAY;
}

/* free multiples */
#include <stdarg.h>
void f_clear_multi(f_int *a, ...)
{
   va_list args;
   va_start(args, a);
   while (a != NULL) {
       f_clear(a);
       a = va_arg(args, f_int *);
   }
   va_end(args);
}
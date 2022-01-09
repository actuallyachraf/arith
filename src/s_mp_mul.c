#include "include/arith.h"

/**
 * @brief mp_mul.c provides low and high level
 * routines for multiplication.
 */

/**
 * @brief s_mp_mul multiplies |a| * |b| and only computes up to digs
 * precision of the result.
 */
int s_mp_mul( mp_int *a,  mp_int *b, mp_int *c, int digs)
{
   mp_int  t;
   mp_err  err;
   int     pa, ix;

   /* can we use the fast multiplier? */
   if ((digs < MP_WARRAY) &&
       (MP_MIN(a->used, b->used) < MP_MAX_COMBA)) {
      return s_mp_mul_comba(a, b, c, digs);
   }

   if ((err = mp_init_size(&t, digs)) != MP_OKAY) {
      return err;
   }
   t.used = digs;

   /* compute the digits of the product directly */
   pa = a->used;
   for (ix = 0; ix < pa; ix++) {
      int iy, pb;
      mp_digit u = 0;

      /* limit ourselves to making digs digits of output */
      pb = MP_MIN(b->used, digs - ix);

      /* compute the columns of the output and propagate the carry */
      for (iy = 0; iy < pb; iy++) {
         /* compute the column as a mp_word */
         mp_word r = (mp_word)t.dp[ix + iy] +
                     ((mp_word)a->dp[ix] * (mp_word)b->dp[iy]) +
                     (mp_word)u;

         /* the new column is the lower part of the result */
         t.dp[ix + iy] = (mp_digit)(r & (mp_word)MP_MASK);

         /* get the carry word from the result */
         u       = (mp_digit)(r >> (mp_word)MP_DIGIT_BIT);
      }
      /* set carry if it is placed below digs */
      if ((ix + iy) < digs) {
         t.dp[ix + pb] = u;
      }
   }

   mp_clamp(&t);
   mp_swap(&t, c);

   mp_clear(&t);
   return MP_OKAY;
}
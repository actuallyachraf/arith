#include "include/arith.h"

/**
 * @brief mp_digit.c implements various digit based operations.
 * such as adding a digit or multiplying by a digit.
 */

/* substract a digit from an mp_int */
int mp_sub_d(mp_int *a, mp_digit b, mp_int *c)
{
   int res;

   /* fast path for a == c */
   if (a == c) {
      if ((c->sign == MP_NEG) &&
          ((c->dp[0] + b) < MP_DIGIT_MAX)) {
         c->dp[0] += b;
         return MP_OKAY;
      }
      if ((c->sign == MP_ZPOS) &&
          (c->dp[0] > b)) {
         c->dp[0] -= b;
         return MP_OKAY;
      }
   }

   /* grow c as required */
   if ((res = mp_grow(c, a->used + 1)) != MP_OKAY) {
      return res;
   }

   /* if a is negative just do an unsigned
    * addition [with fudged signs]
    */
   if (a->sign == MP_NEG) {
      mp_int a_ = *a;
      a_.sign = MP_ZPOS;
      res     = mp_add_d(&a_, b, c);
      c->sign = MP_NEG;

      /* clamp */
      mp_clamp(c);

      return res;
   }

   /* if a <= b simply fix the single digit */
   if (((a->used == 1) && (a->dp[0] <= b)) || mp_iszero(a)) {
      c->dp[0] = (a->used == 1) ? b - a->dp[0] : b;

      /* negative/1digit */
      c->sign = MP_NEG;
      c->used = 1;
   } else {
      int i;
      mp_digit mu = b;

      /* positive/size */
      c->sign = MP_ZPOS;
      c->used = a->used;

      /* subtract digits, mu is carry */
      for (i = 0; i < a->used; i++) {
         c->dp[i] = a->dp[i] - mu;
         mu = c->dp[i] >> (MP_SIZE_OF_BITS(mp_digit) - 1u);
         c->dp[i] &= MP_MASK;
      }
   }
   mp_clamp(c);
   return MP_OKAY;
}
/* add a digit to an mp_int */
int mp_add_d(mp_int *a, mp_digit b, mp_int *c)
{
    int res, ix, oldused;
    mp_digit *tmpa, *tmpc, mu;

    if (c->alloc < a->used + 1)
    {
        if ((res = mp_grow(c, a->used + 1)) != MP_OKAY)
        {
            return res;
        }
    }
    /**
     * @note if a is negative and |a|>= b set c = |a| - b
     */
    if (a->sign == MP_NEG && (a->used > 1 || a->dp[0] >= b))
    {
        /* fix a's sign to positive */
        a->sign = MP_ZPOS;
        res = mp_sub_d(a, b, c);
        a->sign = MP_NEG;
        c->sign = MP_NEG;
        mp_clamp(c);
        return res;
    }
    oldused = c->used;
    c->sign = MP_ZPOS;

    tmpa = a->dp;
    tmpc = c->dp;

    if (a->sign == MP_ZPOS)
    {
        /**
         * @brief add digit then propage the carry
         *
         */
        *tmpc = *tmpa++ + b;
        mu = *tmpc >> MP_DIGIT_BIT;
        *tmpc++ &= MP_MASK;

        for (ix = 1; ix < a->used; ix++)
        {
            *tmpc = *tmpa++ + mu;
            mu = *tmpc >> MP_DIGIT_BIT;
            *tmpc++ &= MP_MASK;
        }
        ix++;
        *tmpc++ = mu;
        c->used = a->used + 1;
    }
    else
    {
        /* a was negative and |a| < b */
        c->used = 1;
        /* the result is a single digit */
        if (a->used == 1)
        {
            *tmpc++ = b - a->dp[0];
        }
        else
        {
            *tmpc++ = b;
        }
        /* setup count so the clearing of oldused
        * can fall through correctly
        */
        ix = 1;
    }
    /* now zero to oldused */
    while (ix++ < oldused)
    {
        *tmpc++ = 0;
    }
    mp_clamp(c);
    return MP_OKAY;
}
/* multiply an mp_int by a digit */
int mp_mul_d(mp_int* a, mp_digit b,mp_int* c) {
    mp_digit u, *tmpa, *tmpc;
    mp_qword r;
    int ix, res, olduse;

    if(c->alloc < a->used + 1) {
        if((res=mp_grow(c,a->used+1)) != MP_OKAY) {
            return res;
        }
    }
    olduse = c->used;

    /* set the sign */
    c->sign = a->sign;

    /* set pointer aliases */
    tmpa = a->dp;
    tmpc = c->dp;

    /* zero the carry */
    u = 0;

    /* compute columnar multiplication */
    for (ix = 0; ix < a->used;ix++) {
        r = ((mp_qword)u) + ((mp_qword)*tmpa++) * ((mp_qword)b);
        /* mask off higher bits */
        *tmpc++ = (mp_digit)(r & ((mp_qword)MP_MASK));

        /* propagte carry outwards */
        u = (mp_digit)(r >> ((mp_qword)MP_DIGIT_BIT));
    }
    /* store final carry */
    *tmpc++ = u;
    ++ix;

    /* zero leading digits */
    while(ix++ < olduse) {
        *tmpc++ = 0;
    }
    /* set used count */
    c->used = a->used + 1;
    mp_clamp(c);

    return MP_OKAY;
}
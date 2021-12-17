/* module mp_radix.c implements radix transformations */
#include <ctype.h>

#include "include/arith.h"
#include "include/debug.h"

/* read an ASCII string as a number in a radix beta */
int mp_read_radix(mp_int *a, const char *str, int radix)
{
    int y, res, neg;
    char ch;

    mp_zero(a);

    /* assert radix in 2-64 */
    if (radix < 2 || radix > 64)
    {
        return MP_VAL;
    }
    /* set the sign if the first char is '-' */
    if (*str == '-')
    {
        ++str;
        neg = MP_NEG;
    }
    else
    {
        neg = MP_ZPOS;
    }
    /* process each digit of the string */
    while (*str)
    {
        /**
         * @note when the radix is < 32 then the representation
         * will almost always be case insensitive this lets us
         * assume 1AB = 1ab for hex values.
         */
        ch = (char)((radix < 36) ? toupper(*str) : *str);
        for (y = 0; y < 64; y++)
        {
            if (ch == mp_radix(y))
            {
                break;
            }
        }
        /* if the digit was found in the map and is less than radix
            add it to the number using digit arithmetic.
        */
        if (y < radix)
        {
            if ((res = mp_mul_d(a, (mp_digit)radix, a)) != MP_OKAY)
            {
                return res;
            }
            if ((res = mp_add_d(a, (mp_digit)y, a)) != MP_OKAY)
            {
                return res;
            }
        }
        else
        {
            break;
        }
        str++;
    }
    /* set the sign correctly */
    if (mp_iszero(a) != 1)
    {
        a->sign = neg;
    }
    return MP_OKAY;
}
static void s_mp_reverse(unsigned char *s, size_t len)
{
   size_t   ix, iy;
   unsigned char t;

   ix = 0u;
   iy = len - 1u;
   while (ix < iy) {
      t     = s[ix];
      s[ix] = s[iy];
      s[iy] = t;
      ++ix;
      --iy;
   }
}


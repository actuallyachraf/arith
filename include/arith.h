#ifndef ARITH_H
#define ARITH_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MP_64BIT

typedef uint64_t mp_digit;

#define MP_DIGIT_BIT 60

#define MP_MASK ((((mp_digit)1) << ((mp_digit)MP_DIGIT_BIT)) - ((mp_digit)1))
#define MP_DIGIT_MAX MP_MASK
#define MP_PREC 32

typedef enum
{
    MP_ZPOS = 0, /* positive */
    MP_NEG = 1   /* negative */
} mp_sign;

typedef enum
{
    MP_LT = -1,
    MP_EQ = 0,
    MP_GT = 1,
} mp_ord;

typedef enum
{
    MP_OKAY = 0,  /* no error */
    MP_ERR = -1,  /* unknown error */
    MP_MEM = -2,  /* out of mem */
    MP_VAL = -3,  /* invalid input */
    MP_ITER = -4, /* maximum iterations reached */
    MP_BUF = -5,  /* buffer overflow, supplied buffer too small */
    MP_OVF = -6   /* mp_int overflow, too many digits */
} mp_err;

typedef enum
{
    MP_LSB_FIRST = -1,
    MP_MSB_FIRST = 1
} mp_order;

typedef enum
{
    MP_LITTLE_ENDIAN = -1,
    MP_NATIVE_ENDIAN = 0,
    MP_BIG_ENDIAN = 1
} mp_endian;

/* the infamous mp_int structure */
typedef struct
{
    int used, alloc;
    mp_sign sign;
    mp_digit *dp;
} mp_int;

/* Define basic checks */
#define mp_iszero(a) ((a)->used == 0)
#define mp_isneg(a)  ((a)->sign == MP_NEG)
#define mp_iseven(a) (((a)->used == 0) || (((a)->dp[0] & 1u) == 0u))
#define mp_isodd(a)  (!mp_iseven(a))

/*
  MP_MALLOC defines macros for heap memory allocations.
*/
#ifndef MP_MALLOC
#include <stdlib.h>
#define MP_ALLOC(size) malloc(size)
#define MP_CLEAR(mem) free(mem)
#define MP_REALLOC(mem,newsize) realloc((mem),(newsize))
#define MP_CALLOC(nmemb,size) calloc((nmemb),(size))
#endif


/*
    Define external API
*/

int mp_init(mp_int *a);
int mp_init_size(mp_int *a, int size);
int mp_init_set(mp_int *a, mp_digit b);

int mp_abs(mp_int *a, mp_int *b);
int mp_neg(mp_int *a, mp_int *b);
int mp_cmp(mp_int *a, mp_int *b);
int mp_cmp_mag(mp_int *a, mp_int *b);

int mp_clear(mp_int *a);
int mp_copy(mp_int *a, mp_int *b);
int mp_grow(mp_int *a, int size);

void mp_clamp(mp_int *a);
void mp_set(mp_int *a, mp_digit b);
void mp_zero(mp_int *a);



/*
  Low level arithmetic API.
*/
int s_mp_add(mp_int *a, mp_int *b, mp_int *c);
#endif
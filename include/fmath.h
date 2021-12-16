#ifndef FMATH_H
#define FMATH_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define F_64BIT

typedef uint64_t f_digit;

#define F_DIGIT_BIT 60

#define F_MASK ((((f_digit)1) << ((f_digit)F_DIGIT_BIT)) - ((f_digit)1))
#define F_DIGIT_MAX F_MASK
#define F_PREC 32

typedef enum
{
    F_ZPOS = 0, /* positive */
    F_NEG = 1   /* negative */
} f_sign;

typedef enum
{
    F_LT = -1,
    F_EQ = 0,
    F_GT = 1,
} f_ord;

typedef enum
{
    F_OKAY = 0,  /* no error */
    F_ERR = -1,  /* unknown error */
    F_MEM = -2,  /* out of mem */
    F_VAL = -3,  /* invalid input */
    F_ITER = -4, /* maximum iterations reached */
    F_BUF = -5,  /* buffer overflow, supplied buffer too small */
    F_OVF = -6   /* f_int overflow, too many digits */
} f_err;

typedef enum
{
    F_LSB_FIRST = -1,
    F_MSB_FIRST = 1
} f_order;

typedef enum
{
    F_LITTLE_ENDIAN = -1,
    F_NATIVE_ENDIAN = 0,
    F_BIG_ENDIAN = 1
} f_endian;

/* the infamous f_int structure */
typedef struct
{
    int used, alloc;
    f_sign sign;
    f_digit *dp;
} f_int;

/* Define basic checks */
#define f_iszero(a) ((a)->used == 0)
#define f_isneg(a)  ((a)->sign == MP_NEG)
#define f_iseven(a) (((a)->used == 0) || (((a)->dp[0] & 1u) == 0u))
#define f_isodd(a)  (!mp_iseven(a))

/*
  F_MALLOC defines macros for heap memory allocations.
*/
#ifndef F_MALLOC
#include <stdlib.h>
#define F_ALLOC(size) malloc(size)
#define F_CLEAR(mem) free(mem)
#define F_REALLOC(mem,newsize) realloc((mem),(newsize))
#define F_CALLOC(nmemb,size) calloc((nmemb),(size))
#endif


/*
    Define external API
*/

int f_init(f_int *a);
int f_init_size(f_int *a, int size);

int f_init_set(f_int *a, f_digit b);

int f_abs(f_int *a, f_int *b);
int f_neg(f_int *a, f_int *b);

int f_clear(f_int *a);
int f_cmp_mag(f_int *a, f_int *b);
int f_cmp(f_int *a, f_int *b);
int f_copy(f_int *a, f_int *b);
int f_grow(f_int *a, int size);

void f_clamp(f_int *a);
void f_set(f_int *a, f_digit b);
void f_zero(f_int *a);



#endif
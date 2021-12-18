# arith

`arith` is a C implementation of multiprecision arithmetic written for readability.

The objective of building `arith` was to learn the internals of multiprecision arithmetic and understand
how different algorithms allow faster operations.

## Setup

```sh

make
make memcheck

```

## API

`arith` exposes a C-style API following similar multi-precision arithmetic libraries.

```c

mp_int a,b,c;

mp_init_set(&a, 12345);
mp_init_set(&b, 1234);
// users need to separately call init/clear to manage memory
mp_init(&c);

// result of a + b is stored in c
mp_add(&a,&b,&c);

```

Currently the API exposes high-level arithmetic operations.

```c

int mp_add(mp_int *a, mp_int *b, mp_int *c);
int mp_sub(mp_int *a, mp_int *b, mp_int *c);
int mp_add_d(mp_int *a, mp_digit b, mp_int *c);
int mp_sub_d(mp_int *a, mp_digit b, mp_int *c);
int mp_mul_d(mp_int *a, mp_digit b, mp_int *c);
int mp_mul_2(mp_int *a, mp_int *b);

```

Memory management is exposed through a set of `init` and `clear` functions.

```c

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

```

# License

This project is released under MIT License see [LICENSE] for more information.

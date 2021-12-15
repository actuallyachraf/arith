#include <stdio.h>

#include "../include/fmath.h"

int main() {

    f_int a;
    f_init(&a);
    f_clear(&a);

    f_digit x = 3431123;
    printf("Hello World !\n");
    printf("%ld \n", x);
}
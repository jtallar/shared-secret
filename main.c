#include <stdio.h>
#include <stdint.h>
#include "include/galois.h"

int main() {
    galois_init();
    printf("%d = %d\n", galois_pow(13,3), galois_mul(galois_mul(13,13), 13));
    printf("Add = %d, Sub = %d, Mul = %d, Div = %d", galois_add(84,13), galois_sub(84,13), galois_mul(84,13), galois_div(84,13));
    return 0;
}
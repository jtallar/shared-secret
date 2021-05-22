#include <stdio.h>
#include <stdint.h>
#include "include/galois.h"

int main() {
    galois_print(84);
    galois_print(13);
    printf("%d\n", galois_pow(84,2));
    printf("Add = %d, Sub = %d, Mul = %d, Div = %d", galois_add(84,13), galois_sub(84,13), galois_mul(84,13), galois_div(84,13));
    return 0;
}
/*
 * The following is an implementation of the finite field GF(2^8) as bit vectors of length 8, where the nth bit represents the
 * coefficient of the nth power of the generator in each element, and the generator satisfies the minimal polynomial
 * x^8 + x^4 + x ^3 + x^2 + 1 in the prime field Z_2, in which addition is equivalent to XOR and multiplication to AND.
 * The elements of GF(2^8) thus represent polynomials of degree < 8 in the generator x. Addition in this field is simply
 * bitwise XOR, but multiplication requires the elimination of powers of x <= 8.
 */
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "../include/galois_lib.h"

uint8_t galois_add(uint8_t v1, uint8_t v2){
    return v1 ^ v2;
}

uint8_t galois_sub(uint8_t v1, uint8_t v2){
    return v1 ^ v2;
}

uint8_t galois_mul(uint8_t v1, uint8_t v2){
    return galois_single_multiply(v1,v2,8);
}

uint8_t galois_div(uint8_t v1, uint8_t v2){
    return galois_single_divide(v1,v2,8);
}

uint8_t galois_pow(uint8_t v, uint8_t e){
    return (uint8_t) pow(v, e);
}

void galois_print(uint8_t v){
    int i = 8;
    while (i--)
        putchar((v >> i & 1) + '0');
    putchar('\n');
}



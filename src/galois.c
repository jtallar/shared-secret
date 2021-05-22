/*
 * The following is an implementation of the finite field GF(2^8) as bit vectors of length 8, where the nth bit represents the
 * coefficient of the nth power of the generator in each element, and the generator satisfies the minimal polynomial
 * x^8 + x^4 + x ^3 + x^2 + 1 in the prime field Z_2, in which addition is equivalent to XOR and multiplication to AND.
 * The elements of GF(2^8) thus represent polynomials of degree < 8 in the generator x. Addition in this field is simply
 * bitwise XOR, but multiplication requires the elimination of powers of x <= 8.
 */

#include <stdio.h>
#include <stdint.h>
//#include "../include/galois.h"
#include "../include/galois_lib.h"

uint8_t gal_add(uint8_t a, uint8_t b){
    return a ^ b;
}

uint8_t gal_sub(uint8_t a, uint8_t b){
    return a ^ b;
}

uint8_t gal_mul(uint8_t a, uint8_t b){
    return galois_single_multiply(a,b,8);
}

uint8_t gal_div(uint8_t a, uint8_t b){
    return galois_single_divide(a,b,8);
}

void gal_print(uint8_t a){
    int i = 8;
    while (i--)
        putchar((a >> i & 1) + '0');
    putchar('\n');
}



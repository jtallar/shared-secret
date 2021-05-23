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

uint8_t galois_init(){
    return galois_create_mult_tables(8);   /* Returns 0 on success, -1 on failure */
}

uint8_t galois_add(uint8_t v1, uint8_t v2){
    return v1 ^ v2;
}

uint8_t galois_sub(uint8_t v1, uint8_t v2){
    return v1 ^ v2;
}

uint8_t galois_mul(uint8_t v1, uint8_t v2){
    return galois_multtable_multiply(v1,v2,8);
}

uint8_t galois_div(uint8_t v1, uint8_t v2){
    return galois_multtable_divide(v1,v2,8);
}

uint8_t galois_pow(uint8_t v, uint8_t e){
    if(e==0) return 1;
    uint8_t result = v;
    for(int i = 1; i < e; i++)
        result = galois_mul(result,v);
    return result;
}

void galois_print(uint8_t v){
    int i = 8;
    while (i--)
        putchar((v >> i & 1) + '0');
    putchar('\n');
}
//
// Created by juli on 22/5/21.
//

#ifndef STENOGRAPHY_SHARED_SECRET_GALOIS_H
#define STENOGRAPHY_SHARED_SECRET_GALOIS_H

uint8_t galois_add(uint8_t v1, uint8_t v2);       /* Substraction between two elements of GF(2^8) */
uint8_t galois_sub(uint8_t v1, uint8_t v2);       /* Add two elements of GF(2^8) */
uint8_t galois_mul(uint8_t v1, uint8_t v2);       /* Multiply two elements of GF(2^8) */
uint8_t galois_div(uint8_t v1, uint8_t v2);
uint8_t galois_pow(uint8_t v, uint8_t e);
void galois_print(uint8_t v);                   /* Print an element of GF(2^8) in binary form */

#endif //STENOGRAPHY_SHARED_SECRET_GALOIS_H

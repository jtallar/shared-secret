//
// Created by juli on 22/5/21.
//

#ifndef STENOGRAPHY_SHARED_SECRET_GALOIS_H
#define STENOGRAPHY_SHARED_SECRET_GALOIS_H

uint8_t gal_add(uint8_t a, uint8_t b);       /* Substraction between two elements of GF(2^8) */
uint8_t gal_sub(uint8_t a, uint8_t b);       /* Add two elements of GF(2^8) */
uint8_t gal_mul(uint8_t a, uint8_t b);       /* Multiply two elements of GF(2^8) */
uint8_t gal_div(uint8_t a, uint8_t b);
void gal_print(uint8_t a);                   /* Print an element of GF(2^8) in binary form */

#endif //STENOGRAPHY_SHARED_SECRET_GALOIS_H

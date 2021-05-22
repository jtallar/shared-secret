#include <math.h>

#include "fake_galois.h"

uint16_t MOD_VAL = 256;

uint8_t galois_sum(uint8_t v1, uint8_t v2) {
    return ((uint16_t) v1 + v2) % MOD_VAL;
}

uint8_t galois_sub(uint8_t v1, uint8_t v2) {
    return ((uint16_t) v1 - v2) % MOD_VAL;
}

uint8_t galois_mult(uint8_t v1, uint8_t v2) {
    return ((uint16_t) v1 * v2) % MOD_VAL;
}

uint8_t galois_div(uint8_t v1, uint8_t v2) {
    return ((uint16_t) v1 / v2) % MOD_VAL;
}

uint8_t galois_pow(uint8_t v, uint8_t exp) {
    return ((uint8_t) pow(v, exp)) % MOD_VAL;
}

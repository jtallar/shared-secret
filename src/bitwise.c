#include "bitwise.h"

void replace_bit(uint8_t * el, uint8_t n, uint8_t val) {
    *el = (*el & ~(1 << n)) | (val << n);
}

uint8_t read_bit(uint8_t el, uint8_t n) {
    return ((1 << n) & el) >> n;
}
#ifndef BITWISE_H_1f5a2017dd9ebf2910d60e1460805ecad38338e2
#define BITWISE_H_1f5a2017dd9ebf2910d60e1460805ecad38338e2

#include <stdint.h>

// el = el7 el6 el5 ... el0 ; n e [0, 7] ; val e {0, 1}
// After exec, eg n=6: el = el7 val el5 ... el0
void replace_bit(uint8_t * el, uint8_t n, uint8_t val);

uint8_t read_bit(uint8_t el, uint8_t n);

#endif
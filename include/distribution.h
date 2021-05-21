#ifndef DISTRIBUTION_H_VelRDAxzvnuFmwEaR0ftrkIinkT
#define DISTRIBUTION_H_VelRDAxzvnuFmwEaR0ftrkIinkT

#include <stdint.h>

struct block {
    uint8_t * elements;
    uint8_t size;
};

struct image {
    struct block * blocks;
    uint8_t size;
};

void distribute(struct image secret, struct image * shadows, uint8_t n_sh);

#endif

#ifndef DISTRIBUTION_H_VelRDAxzvnuFmwEaR0ftrkIinkT
#define DISTRIBUTION_H_VelRDAxzvnuFmwEaR0ftrkIinkT

#include <stdint.h>
#include "images.h"

void distribute(struct image secret, struct image * shadows, uint8_t n_sh);

struct image * recover(uint8_t n_sec_blk, struct image * shadows, uint8_t n_sh);

#endif

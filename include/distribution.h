#ifndef DISTRIBUTION_H_VelRDAxzvnuFmwEaR0ftrkIinkT
#define DISTRIBUTION_H_VelRDAxzvnuFmwEaR0ftrkIinkT

#include <stdint.h>
#include "images.h"

/* 
    secret:     image secret to be hidden inside shadows
    shadows:    image vector with shadow blocks, will be modified
    n_sh:       number of shadows
*/
void distribute(struct image secret, struct image * shadows, uint8_t n_sh);

/* 
    shadows:    image vector with shadow blocks, will be modified
    n_sh:       number of shadows == number of elements per secret block == k
    n_sec_blk:  number of secret blocks

    returns     image pointer with uncovered secret (should be freed later) or NULL if error
*/
struct image * recover(struct image * shadows, uint8_t n_sh, uint8_t n_sec_blk);

#endif

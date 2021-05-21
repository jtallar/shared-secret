#include <stdbool.h>

#include "distribution.h"
#include "fake_galois.h"
#include "bitwise.h"

// TODO: Apply galois to each operation here
// F(x) = blk[0] + blk[1] * x + blk[2] * x^2 + ... + blk[size-1] * x^(size-1)
static uint8_t eval(struct block blk, uint8_t x) {
    uint8_t val = 0;
    for (uint8_t i = 0; i < blk.size; i++) {
        val = galois_sum(val, galois_mult(blk.elements[i], galois_pow(x, i)));
    }
    // galois operations already apply mod g(x)
    return val;
}

// shadows WILL BE modified in here
void distribute(struct image secret, struct image * shadows, uint8_t n_sh) {
    // Iterate over all secret blocks
    for (uint8_t j = 0; j < secret.size; j++) {
        struct block sec_blk = secret.blocks[j];

        bool values[UINT8_MAX] = {false};
        // Iterate over all shadows
        for (uint8_t i = 0; i < n_sh; i++) {
            // sh_blk.size should be 4, sh_blk.elements [X, W, V, U]
            struct block sh_blk = shadows[i].blocks[j];
            // TODO: Check if we should modify x inside the shadow, or just save the updated value elsewhere
            // Add decimal 1 until no repeted values
            while (values[sh_blk.elements[0]]) {
                sh_blk.elements[0] = (sh_blk.elements[0] + 1) % UINT8_MAX;
            }
            uint8_t f_x = eval(sec_blk, sh_blk.elements[0]);
            uint8_t parity = 0;
            // Update W, V, U
            const uint8_t el_dic[] = {3, 3, 2, 2, 2, 1, 1, 1};
            const uint8_t n_dic[] = {0, 1, 0, 1, 2, 0, 1, 2};
            for (uint8_t mask = 1, ctr = 0; ctr < 8; mask <<= 1, ctr++) {
                uint8_t bit = (mask & f_x);
                parity ^= bit;
                replace_bit(&sh_blk.elements[el_dic[ctr]], n_dic[ctr], bit);
            }
            // Update parity at U2
            replace_bit(&sh_blk.elements[3], 2, parity);
        }
    }
}
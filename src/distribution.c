#include <stdbool.h> // bool
#include <stdlib.h> // NULL

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
void distribute(struct image secret, struct image ** shadows, uint8_t n_sh) {
    // Iterate over all secret blocks
    for (uint8_t j = 0; j < secret.size; j++) {
        struct block sec_blk = secret.blocks[j];

        bool seen_x_map[UINT8_MAX] = {false};
        // Iterate over all shadows
        for (uint8_t i = 0; i < n_sh; i++) {
            // sh_blk.size should be 4, sh_blk.elements [X, W, V, U]
            struct block sh_blk = shadows[i]->blocks[j];
            // TODO: Check if we should modify x inside the shadow, or just save the updated value elsewhere
            // Add decimal 1 until no repeated x in seen_x_map
            while (seen_x_map[sh_blk.elements[0]]) {
                sh_blk.elements[0] = (sh_blk.elements[0] + 1) % UINT8_MAX;
            }
            uint8_t f_x = eval(sec_blk, sh_blk.elements[0]);
            uint8_t parity = 0;
            // Update W, V, U
            const uint8_t el_dic[] = {3, 3, 2, 2, 2, 1, 1, 1};
            const uint8_t n_dic[] = {0, 1, 0, 1, 2, 0, 1, 2};
            for (uint8_t ctr = 0; ctr < 8; ctr++) {
                uint8_t bit = read_bit(f_x, ctr);
                parity ^= bit;
                replace_bit(&sh_blk.elements[el_dic[ctr]], n_dic[ctr], bit);
            }
            // Update parity at U2
            replace_bit(&sh_blk.elements[3], 2, parity);
        }
    }
}

static uint8_t lagrange_term(uint8_t * x_values, uint8_t * y_values, uint8_t k, uint8_t r) {
    uint8_t sum = 0;
    // Sum loop
    for (uint8_t i = 0; i < k - r + 1; i++) {
        uint8_t prod = 1;
        // Prod loop
        for (uint8_t q = 0; q < k - r + 1; q++) {
            // If i==q, skip iteration
            if (q == i) continue;
            uint8_t factor = galois_div(x_values[q], galois_sub(x_values[i], x_values[q]));
            prod = galois_mult(prod, factor);
        }
        sum = galois_sum(sum, galois_mult(y_values[i], prod));
    }
    return galois_mult(galois_pow(-1, k - r), sum);
}

static void interpolate_block(struct block * dest, uint8_t * x_values, uint8_t * y_values, uint8_t k) {
    // Calculate S1
    dest->elements[0] = lagrange_term(x_values, y_values, k, 1);
    // Update y_values
    uint8_t y_prime_values[k];
    for (uint8_t j = 0; j < k; j++) {
        y_prime_values[j] = galois_div(galois_sub(y_values[j], dest->elements[0]), x_values[j]);
    }
    // Calculate S2,...,Sk
    for (uint8_t j = 1; j < k; j++) {
        dest->elements[j] = lagrange_term(x_values, y_prime_values, k, j + 1);
    }
}

struct image * recover(struct image ** shadows, uint8_t n_sh, uint8_t n_sec_blk) {
    struct image * secret = new_empty_image(n_sec_blk, n_sh);
    // Iterate over all secret blocks
    for (uint8_t j = 0; j < n_sec_blk; j++) {
        bool seen_x_map[UINT8_MAX] = {false};
        uint8_t x_values[n_sh];
        uint8_t y_values[n_sh];
        // Iterate over all shadows
        for (uint8_t i = 0; i < n_sh; i++) {
            // sh_blk.size should be 4, sh_blk.elements [X, W, V, U]
            struct block sh_blk = shadows[i]->blocks[j];
            // TODO: Change according to distribute TODO, if modified condition should always be false
            // Add decimal 1 until no repeated x in seen_x_map
            while (seen_x_map[sh_blk.elements[0]]) {
                sh_blk.elements[0] = (sh_blk.elements[0] + 1) % UINT8_MAX;
            }
            uint8_t f_x = 0;
            uint8_t real_parity = 0, expected_parity = read_bit(sh_blk.elements[3], 2);
            // Read W, V, U
            const uint8_t el_dic[] = {3, 3, 2, 2, 2, 1, 1, 1};
            const uint8_t n_dic[] = {0, 1, 0, 1, 2, 0, 1, 2};
            for (uint8_t ctr = 0; ctr < 8; ctr++) {
                uint8_t bit = read_bit(sh_blk.elements[el_dic[ctr]], n_dic[ctr]);
                real_parity ^= bit;
                replace_bit(&f_x, ctr, bit);
            }
            // TODO: What do we do if wrong parity?
            // Check parity
            if (real_parity != expected_parity) {
                image_destroy(secret);
                return NULL;
            }
            // Save X and f_x values
            x_values[i] = sh_blk.elements[0];
            y_values[i] = f_x;
        }
        interpolate_block(&secret->blocks[j], x_values, y_values, n_sh);
    }
    return secret;
}
#include <stdio.h> // fprintf
#include <stdbool.h> // bool
#include <stdlib.h> // NULL
#include <math.h> // pow

#include "../include/distribution.h"
#include "../include/galois.h"
#include "../include/bitwise.h"

#define MODULUS     256

// TODO: Apply galois to each operation here
// F(x) = blk[0] + blk[1] * x + blk[2] * x^2 + ... + blk[size-1] * x^(size-1)
static uint8_t eval(uint8_t * blk, uint8_t blk_size, uint8_t x) {
    uint8_t val = 0;
    for (uint8_t i = 0; i < blk_size; i++) {
        val = galois_add(val, galois_mul(blk[i], galois_pow(x, i)));
    }
    // galois operations already apply mod g(x)
    return val;
}

// shadows WILL BE modified in here
void distribute(struct image secret, struct image ** shadows, uint8_t n_sh) {
    // Iterate over all secret blocks
    for (uint32_t j = 0; j < secret.total_size; j++) {
        uint8_t * sec_blk = secret.elements[j];

        bool seen_x_map[MODULUS] = {false};
        // Iterate over all shadows
        for (uint8_t i = 0; i < n_sh; i++) {
            // sh_blk.size should be 4, sh_blk.elements [X, W, V, U]
            uint8_t * sh_blk = shadows[i]->elements[j];
            // Add decimal 1 until no repeated x in seen_x_map
            while (seen_x_map[sh_blk[0]]) {
                sh_blk[0] = (sh_blk[0] + 1) % MODULUS;
            }
            seen_x_map[sh_blk[0]] = true;
            uint8_t f_x = eval(sec_blk, secret.block_size, sh_blk[0]);
            uint8_t parity = 0;
            // Update W, V, U
            const uint8_t el_dic[] = {3, 3, 2, 2, 2, 1, 1, 1};
            const uint8_t n_dic[] = {0, 1, 0, 1, 2, 0, 1, 2};
            for (uint8_t ctr = 0; ctr < 8; ctr++) {
                uint8_t bit = read_bit(f_x, ctr);
                parity ^= bit;
                replace_bit(&sh_blk[el_dic[ctr]], n_dic[ctr], bit);
            }
            // Update parity at U2
            replace_bit(&sh_blk[3], 2, parity);
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
            prod = galois_mul(prod, factor);
        }
        sum = galois_add(sum, galois_mul(y_values[i], prod));
    }
    // TODO: Check si el (-1)^(k-r) va o no
    return sum;
    // return galois_mul(pow(-1, k - r), sum);
    // return galois_mul(galois_pow(galois_sub(0, 1), k - r), sum);
}

// TODO: Fix this, only S1 is OK.
static void interpolate_block(uint8_t ** dest, uint8_t * x_values, uint8_t * y_values, uint8_t k) {
    // Calculate S1
    (*dest)[0] = lagrange_term(x_values, y_values, k, 1);
    // Calculate S2,...,Sk
    for (uint8_t j = 1; j < k; j++) {
        // Update Y values
        for (uint8_t w = 0; w < k; w++) {
            // TODO: Ver que pasa si x = 0, me da mal
            y_values[w] = galois_div(galois_sub(y_values[w], (*dest)[j - 1]), x_values[w]);
        }
        (*dest)[j] = lagrange_term(x_values, y_values, k, j + 1);
    }
}

struct image * recover(struct image ** shadows, uint8_t n_sh, uint32_t n_sec_blk, const char * filepath) {
    struct image * secret = new_empty_image(n_sec_blk, n_sh, filepath);
    // Iterate over all secret blocks
    for (uint32_t j = 0; j < n_sec_blk; j++) {
        bool seen_x_map[MODULUS] = {false};
        uint8_t x_values[n_sh];
        uint8_t y_values[n_sh];
        // Iterate over all shadows
        for (uint8_t i = 0; i < n_sh; i++) {
            // sh_blk.size should be 4, sh_blk.elements [X, W, V, U]
            uint8_t * sh_blk = shadows[i]->elements[j];
            // Add decimal 1 until no repeated x in seen_x_map
            if (seen_x_map[sh_blk[0]]) {
                fprintf(stderr, "Error in recover: repeated X element.\n");
                exit(1);
            }
            seen_x_map[sh_blk[0]] = true;
            uint8_t f_x = 0;
            uint8_t real_parity = 0, expected_parity = read_bit(sh_blk[3], 2);
            // Read W, V, U
            const uint8_t el_dic[] = {3, 3, 2, 2, 2, 1, 1, 1};
            const uint8_t n_dic[] = {0, 1, 0, 1, 2, 0, 1, 2};
            for (uint8_t ctr = 0; ctr < 8; ctr++) {
                uint8_t bit = read_bit(sh_blk[el_dic[ctr]], n_dic[ctr]);
                real_parity ^= bit;
                replace_bit(&f_x, ctr, bit);
            }
            // TODO: What do we do if wrong parity?
            // Check parity
            if (real_parity != expected_parity) {
                image_destroy(secret);
                fprintf(stderr, "Error in recover: wrong parity.\n");
                exit(1);
            }
            // Save X and f_x values
            x_values[i] = sh_blk[0];
            y_values[i] = f_x;
        }
        interpolate_block(&secret->elements[j], x_values, y_values, n_sh);
    }
    return secret;
}

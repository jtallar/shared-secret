#ifndef IMAGES_H_da39a3ee5e6b4b0d3255bfef95601890afd80709
#define IMAGES_H_da39a3ee5e6b4b0d3255bfef95601890afd80709

#include <stdint.h>

struct block {
    uint8_t * elements;
    uint8_t size;
};

struct image {
    struct block * blocks;
    uint8_t size;
    char * filepath;
};

struct image * read_image_from_file(const char * path, uint8_t k);

struct image ** read_shadow_images_from_file(char ** paths, uint8_t k, uint8_t shadow_images_count);

void write_images(struct image ** shadow_images, uint8_t shadow_images_count, uint8_t overwrite);

// TODO remove this
struct image * new_empty_image(uint8_t n_blk, uint8_t n_blk_el);

void image_destroy(struct image * img);

void image_print(struct image img);

#endif

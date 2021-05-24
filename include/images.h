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

struct image_extras {
    uint32_t size;
    uint32_t offset;
    uint32_t width;
    uint32_t height;
    uint16_t bits_per_pixel;

    char * image_template;
};

struct image_extras * read_image_extras(const char * path, uint8_t k);

struct image * read_image_from_file(const char * path, uint8_t k);

struct image ** read_shadow_images_from_file(char ** paths, uint8_t k, uint8_t shadow_images_count);

void write_images(struct image ** images, uint8_t count, uint8_t overwrite, struct image_extras * temp);

// TODO remove this
struct image * new_empty_image(uint8_t n_blk, uint8_t n_blk_el);

void image_destroy(struct image * img);

void image_print(struct image img);

#endif

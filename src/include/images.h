#ifndef IMAGES_H_da39a3ee5e6b4b0d3255bfef95601890afd80709
#define IMAGES_H_da39a3ee5e6b4b0d3255bfef95601890afd80709

#include <stdint.h>

struct image {
    uint8_t ** elements;
    uint32_t total_size;
    uint8_t block_size;
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

struct image_extras * read_image_extras(const char * path, uint8_t k, uint16_t bits_per_pixel);

struct image * read_image_from_file(const char * path, uint8_t k, uint8_t secret, struct image_extras * temp);

struct image ** read_images_from_file(char ** paths, uint8_t count, uint8_t k, uint8_t secret, struct image_extras * temp);

int write_image(struct image * image, uint8_t secret, struct image_extras * temp);

int write_images(struct image ** images, uint8_t count, uint8_t secret, struct image_extras * temp);

void image_extras_destroy(struct image_extras * extras);

void images_destroy(struct image ** images, uint8_t count);

void image_destroy(struct image * img);

struct image * new_empty_image(uint32_t total_block_count, uint8_t block_size, const char * filepath);

#endif

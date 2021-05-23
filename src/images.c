#include <stdio.h> // printf
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "images.h"

// TODO implement this 3
struct image * read_image_from_file(const char * path, uint8_t k) {
    return NULL;
}

struct image ** read_shadow_images_from_file(char ** paths, uint8_t k, uint8_t shadow_images_count) {
    return NULL;
}

void write_images(struct image ** shadow_images, uint8_t shadow_images_count, uint8_t overwrite) {
    return;
}


struct image * new_empty_image(uint8_t n_blk, uint8_t n_blk_el) {
    size_t size = sizeof(struct image);
    struct image * ret = malloc(size);
    if (ret != NULL) {
        memset(ret, 0x00, size);
        ret->size = n_blk;
        size_t size_blocks = sizeof(struct block) * n_blk;
        ret->blocks = malloc(size_blocks);
        if (ret->blocks != NULL) {
            memset(ret->blocks, 0x00, size_blocks);
            for (uint8_t i = 0; i < n_blk; i++) {
                ret->blocks[i].size = n_blk_el;
                size_t size_elts = sizeof(uint8_t) * n_blk_el;
                ret->blocks[i].elements = malloc(size_elts);
                if (ret->blocks[i].elements == NULL) break;
                memset(ret->blocks[i].elements, 0x00, size_elts);
            }
        }
    }
    return ret;
}

void image_destroy(struct image * img) {
    if (img != NULL) {
        if (img->blocks != NULL) {
            for (uint8_t i = 0; i < img->size; i++) {
                free(img->blocks[i].elements);
                img->blocks[i].elements = NULL;
                img->blocks[i].size = 0;
            }
            free(img->blocks);
            img->blocks = NULL;
            img->size = 0;
        }
        free(img);
    }
}

void image_print(struct image img) {
    for (uint8_t i = 0; i < img.size; i++) {
        printf("\nBlock %d: \n\t", i);
        for (uint8_t j = 0; j < img.blocks[i].size; j++) {
            printf("%d ", img.blocks[i].elements[j]);
        }
    }
}

#include <stdio.h> // printf
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "images.h"

#define FILE_SIZE_OFFSET        0x0002
#define DATA_OFFSET_OFFSET      0x000A
#define WIDTH_OFFSET            0x0012
#define HEIGHT_OFFSET           0x0016
#define BITS_PER_PIXEL_OFFSET   0x001C

#define SHADOW_BLOCK_SIZE       4

static void stderr_and_exit(char * message) {
    fprintf(stderr, "%s", message);
    exit(1);
}

struct image_extras * read_image_extras(const char * path, uint8_t k) {
    FILE * full_bmp = fopen(path, "rb");
    if (full_bmp == NULL) stderr_and_exit("Image file does not exist.\n");

    struct image_extras * extra_data = malloc(sizeof(struct image_extras));

    // save full file size
    fseek(full_bmp, FILE_SIZE_OFFSET, SEEK_SET);
    fread(&extra_data->size, sizeof(uint32_t), 1, full_bmp);

    // save pixel data offset
    fseek(full_bmp, DATA_OFFSET_OFFSET, SEEK_SET);
    fread(&extra_data->offset, sizeof(uint32_t), 1, full_bmp);

    // save image width in pixels
    fseek(full_bmp, WIDTH_OFFSET, SEEK_SET);
    fread(&extra_data->width, sizeof(uint32_t), 1, full_bmp);

    // save image height in pixels
    fseek(full_bmp, HEIGHT_OFFSET, SEEK_SET);
    fread(&extra_data->height, sizeof(uint32_t), 1, full_bmp);

    // save image bits per pixels
    fseek(full_bmp, BITS_PER_PIXEL_OFFSET, SEEK_SET);
    fread(&extra_data->bits_per_pixel, sizeof(uint32_t), 1, full_bmp);

    // save the entire image as a template for later
    extra_data->image_template = malloc(extra_data->size);
    if (extra_data->image_template == NULL) stderr_and_exit("Not enough heap memory to save an image.\n");
    fseek(full_bmp, 0, SEEK_SET);
    fread(&extra_data->image_template, sizeof(uint8_t), extra_data->size, full_bmp);
    fclose(full_bmp);

    // check if the amount of pixels is divisible by k
    if ((extra_data->height * extra_data->width) % k != 0) stderr_and_exit("The total amount of pixels is not divisible by k.\n");

    return extra_data;
}

struct image * read_image_from_file(const char * path, uint8_t k, uint8_t secret, struct image_extras * temp) {
    FILE * full_bmp = fopen(path, "rb");

    // allocate memory for the image struct
    struct image *pixel_map = malloc(sizeof(struct image));
    if (pixel_map == NULL) stderr_and_exit("Not enough heap memory to create an image struct.\n");

    // initialize sizes and paths
    pixel_map->filepath = path;
    pixel_map->block_size = (secret) ? k : SHADOW_BLOCK_SIZE;
    pixel_map->total_size = (temp->height * temp->width) / pixel_map->block_size;

    // allocate memory for the matrix of elements
    pixel_map->elements = malloc(sizeof(uint8_t * ) * pixel_map->total_size);
    if (pixel_map->elements == NULL) stderr_and_exit("Not enough heap memory to create an elements matrix.\n");

    // allocate memory for all the array of elements
    for (int i = 0; i < pixel_map->total_size; ++i) {
        pixel_map->elements[i] = malloc(sizeof(uint8_t *) * pixel_map->block_size);
        if (pixel_map->elements[i] == NULL) stderr_and_exit("Not enough heap memory to create another single elements array.\n");
    }

    // move to the start of the pixel map
    fseek(full_bmp, temp->offset, SEEK_SET);

    // read and copy every element for secret and fot shadow
    if (secret)
        for (int i = 0; i < pixel_map->total_size; ++i)
            for (int j = 0; j < pixel_map->block_size; ++j)
                fread(&(pixel_map->elements[i][j]), sizeof(uint8_t), 1, full_bmp);
    else
        for (int h = 0; h < temp->height; ++h)
            for (int w = 0; w < temp->width/2; ++w)
                fread(&(pixel_map->elements[w + (temp->width/2) * (h/2)][(h % 2) * 2]), sizeof(uint8_t), 2, full_bmp);

    return pixel_map;
}

struct image ** read_images_from_file(char ** paths, uint8_t count, uint8_t k, uint8_t secret, struct image_extras * temp) {
    struct image ** images = malloc(sizeof(struct image *) * count);
    if (images == NULL) stderr_and_exit("Not enough heap memory to create an array of images struct.\n");

    for (int i = 0; i < count; ++i) images[i] = read_image_from_file(paths[i], k, secret, temp);

    return images;
}

// TODO implement this one
void write_images(struct image ** images, uint8_t count, uint8_t overwrite, struct image_extras * temp) {
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

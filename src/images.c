#include <stdio.h> // printf
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "./include/images.h"

#define FILE_SIZE_OFFSET        0x0002
#define DATA_OFFSET_OFFSET      0x000A
#define WIDTH_OFFSET            0x0012
#define HEIGHT_OFFSET           0x0016
#define BITS_PER_PIXEL_OFFSET   0x001C

#define SHADOW_BLOCK_SIZE       4

static void print_stderr(const char * message) {
    fprintf(stderr, "%s", message);
}

struct image_extras * read_image_extras(const char * path, uint8_t k, uint16_t bits_per_pixel) {
    FILE * full_bmp = fopen(path, "rb");
    if (full_bmp == NULL) {
        print_stderr("Image file does not exist.\n");
        return NULL;
    }

    struct image_extras * extra_data = malloc(sizeof(struct image_extras));
    if (extra_data == NULL) {
        print_stderr("Not enough heap memory to save extra data.\n");
        fclose(full_bmp);
        return NULL;
    }

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

    if (extra_data->bits_per_pixel != bits_per_pixel) {
        print_stderr("Invalid bmp bits per pixel in "); print_stderr(path); print_stderr(".\n");
        fclose(full_bmp);
        image_extras_destroy(extra_data);
        return NULL;
    }

    // save the entire image as a template for later
    extra_data->image_template = malloc(sizeof(uint8_t) * extra_data->size);
    if (extra_data->image_template == NULL) {
        print_stderr("Not enough heap memory to save an image.\n");
        fclose(full_bmp);
        image_extras_destroy(extra_data);
        return NULL;
    }
    fseek(full_bmp, 0, SEEK_SET);
    fread(extra_data->image_template, sizeof(uint8_t), extra_data->size, full_bmp);

    // check if the amount of pixels is divisible by k
    if ((extra_data->height * extra_data->width) % k != 0) {
        print_stderr("The total amount of pixels is not divisible by k in "); print_stderr(path); print_stderr(".\n");
        fclose(full_bmp);
        image_extras_destroy(extra_data);
        return NULL;
    }

    fclose(full_bmp);
    return extra_data;
}

struct image * read_image_from_file(const char * path, uint8_t k, uint8_t secret, struct image_extras * temp) {
    FILE * full_bmp = fopen(path, "rb");
    if (full_bmp == NULL) {
        print_stderr("Error while trying to parse bmp file. bmp file does not exist.\n");
        return NULL;
    }

    // allocate memory for the image struct
    struct image * pixel_map = calloc(1, sizeof(struct image));
    if (pixel_map == NULL) {
        print_stderr("Not enough heap memory to create an image struct.\n");
        fclose(full_bmp);
        return NULL;
    }

    // initialize paths
    pixel_map->filepath = malloc(sizeof(char) * strlen(path) + 1);
    if (pixel_map->filepath == NULL) {
        print_stderr("Not enough heap memory to save image path.\n");
        fclose(full_bmp);
        image_destroy(pixel_map);
        return NULL;
    }
    strcpy(pixel_map->filepath, path);

    // initialize sizes
    pixel_map->block_size = (secret) ? k : SHADOW_BLOCK_SIZE;
    pixel_map->total_size = (temp->height * temp->width) / pixel_map->block_size;

    // allocate memory for the matrix of elements
    pixel_map->elements = calloc(pixel_map->total_size, sizeof(uint8_t * ));
    if (pixel_map->elements == NULL) {
        print_stderr("Not enough heap memory to create an elements matrix.\n");
        fclose(full_bmp);
        image_destroy(pixel_map);
        return NULL;
    }

    // allocate memory for all the array of elements
    for (uint32_t i = 0; i < pixel_map->total_size; ++i) {
        pixel_map->elements[i] = malloc(sizeof(uint8_t) * pixel_map->block_size);
        if (pixel_map->elements[i] == NULL) {
            print_stderr("Not enough heap memory to create another single elements array.\n");
            fclose(full_bmp);
            image_destroy(pixel_map);
            return NULL;
        }
    }

    // move to the start of the pixel map
    fseek(full_bmp, temp->offset, SEEK_SET);

    // read and copy every element for secret and for shadow
    if (secret)
        for (uint32_t i = 0; i < pixel_map->total_size; ++i)
            fread(pixel_map->elements[i], sizeof(uint8_t), pixel_map->block_size, full_bmp);
    else
        for (int32_t h = temp->height-1; h >= 0; --h)
            for (uint16_t w = 0; w < temp->width/2; ++w)
                fread(&(pixel_map->elements[w + (temp->width/2) * (h/2)][(h % 2) * 2]), sizeof(uint8_t), 2, full_bmp);

    fclose(full_bmp);
    return pixel_map;
}

struct image ** read_images_from_file(char ** paths, uint8_t count, uint8_t k, uint8_t secret, struct image_extras * temp) {
    struct image ** images = malloc(sizeof(struct image *) * count);
    if (images == NULL) {
        print_stderr("Not enough heap memory to create an array of images struct.\n");
        return NULL;
    }

    for (uint8_t i = 0; i < count; ++i) {
        images[i] = read_image_from_file(paths[i], k, secret, temp);
        if (images[i] == NULL) {
            images_destroy(images, i + 1);
            return NULL;
        }
    }

    return images;
}

int write_image(struct image * image, uint8_t secret, struct image_extras * temp) {
    FILE * new_bmp = fopen(image->filepath, "w+");
    if (new_bmp == NULL) {
        print_stderr("Error creating file image.\n");
        return -1;
    }

    // copy the first part of the image
    fwrite(temp->image_template, sizeof(uint8_t), temp->offset, new_bmp);

    // write and copy every element for secret and for shadow
    if (secret)
        for (uint32_t i = 0; i < image->total_size; ++i)
            fwrite(image->elements[i], sizeof(uint8_t), image->block_size, new_bmp);
    else
        for (int32_t h = temp->height-1; h >= 0; --h)
            for (uint16_t w = 0; w < temp->width/2; ++w)
                fwrite(&(image->elements[w + (temp->width/2) * (h/2)][(h % 2) * 2]), sizeof(uint8_t), 2, new_bmp);

    // copy the last part of the image
    uint32_t new_offset = temp->offset + temp->width * temp->height;
    if (new_offset < temp->size)
        fwrite(&(temp->image_template[new_offset]), sizeof(uint8_t), temp->size - new_offset, new_bmp);

    fclose(new_bmp);
    return 0;
}

int write_images(struct image ** images, uint8_t count, uint8_t secret, struct image_extras * temp) {
    for (int i = 0; i < count; ++i) {
        if (write_image(images[i], secret, temp) != 0) return -1;
    }
    return 0;
}

void images_destroy(struct image ** images, uint8_t count) {
    if (images == NULL) return;

    for (uint8_t i = 0; i < count; ++i) image_destroy(images[i]);
    if (count > 0) free(images);
}

void image_destroy(struct image * img) {
    if (img == NULL) return;

    if (img->elements != NULL) {
        for (uint32_t i = 0; i < img->total_size; ++i) {
            if (img->elements[i] != NULL) free(img->elements[i]);
            else break;

            img->elements[i] = NULL;
        }
        free(img->elements);
        img->elements = NULL;
    }

    if (img->filepath != NULL) {
        free(img->filepath);
        img->filepath = NULL;
    }
    img->total_size = 0;
    img->block_size = 0;
    free(img);
}

void image_extras_destroy(struct image_extras * extras) {
    if (extras == NULL) return;
    extras->size = 0;
    extras->offset = 0;
    extras->width = 0;
    extras->height = 0;
    extras->bits_per_pixel = 0;
    if (extras->image_template != NULL) {
        free(extras->image_template);
        extras->image_template = NULL;
    };
    free(extras);
}


struct image * new_empty_image(uint32_t total_block_count, uint8_t block_size, const char * filepath) {
    struct image * ret = calloc(1, sizeof(struct image));
    if (ret == NULL) {
        print_stderr("Not enough heap memory to save image.\n");
        return NULL;
    }

    memset(ret, 0x00, sizeof(struct image));

    ret->total_size = total_block_count;
    ret->block_size = block_size;
    ret->elements = calloc(ret->total_size, sizeof(uint8_t *));
    if (ret->elements == NULL) {
        print_stderr("Not enough heap memory to save image elements.\n");
        image_destroy(ret);
        return NULL;
    }

    // initialize filepath
    ret->filepath = malloc(sizeof(char) * strlen(filepath) + 1);
    if (ret->filepath == NULL) {
        print_stderr("Not enough heap memory to save image path.\n");
        image_destroy(ret);
        return NULL;
    }
    strcpy(ret->filepath, filepath);

    memset(ret->elements, 0x00, sizeof(uint8_t *) * ret->total_size);

    for (uint32_t i = 0; i < ret->total_size; i++) {
        ret->elements[i] = malloc(sizeof(uint8_t) * ret->block_size);
        if (ret->elements[i] == NULL) {
            print_stderr("Not enough heap memory to save image element.\n");
            image_destroy(ret);
            return NULL;
        }

        memset(ret->elements[i], 0x00, sizeof(uint8_t) * ret->block_size);
    }

    return ret;
}

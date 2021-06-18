#include <stdio.h>
#include <stdlib.h>

#include "./include/galois.h"
#include "./include/params.h"
#include "./include/images.h"
#include "./include/distribution.h"

#define TRUE 1
#define FALSE 0

static void free_resources(struct stenography * params, struct image_extras * extra_data, struct image ** shadow_images, struct image * secret_image) {
    if (params != NULL) {
        image_destroy(secret_image);
        images_destroy(shadow_images, params->shadow_images_count);
        image_extras_destroy(extra_data);
    }
    destroy_params(params);
    galois_destroy();
}

int main(int argc, char *argv[]) {
    galois_init();
    struct stenography * params = parse_params(argc, argv);
    if (params == NULL) {
        free_resources(params, NULL, NULL, NULL);
        exit(EXIT_FAILURE);
    }

    struct image_extras * extra_data = read_image_extras(params->shadow_images_paths[0], params->k_number);
    if (extra_data == NULL) {
        free_resources(params, extra_data, NULL, NULL);
        exit(EXIT_FAILURE);
    }

    struct image ** shadow_images = read_images_from_file(params->shadow_images_paths, params->shadow_images_count, params->k_number, FALSE, extra_data);
    if (shadow_images == NULL) {
        free_resources(params, extra_data, shadow_images, NULL);
        exit(EXIT_FAILURE);
    }

    struct image * secret_image;
    int ret_write;

    switch (params->action) {
        case DECODE :
            secret_image = read_image_from_file(params->secret_image_path, params->k_number, TRUE, extra_data);
            if (secret_image == NULL) {
                free_resources(params, extra_data, shadow_images, secret_image);
                exit(EXIT_FAILURE);
            }
            distribute(*secret_image, shadow_images, params->shadow_images_count);
            ret_write = write_images(shadow_images, params->shadow_images_count, FALSE, extra_data);
            break;
        case RETRIEVE:
            secret_image = recover(shadow_images, params->k_number, (extra_data->height * extra_data->width) / params->k_number, params->secret_image_path);
            if (secret_image == NULL) {
                free_resources(params, extra_data, shadow_images, secret_image);
                exit(EXIT_FAILURE);
            }
            ret_write = write_image(secret_image, TRUE, extra_data);
            break;
        default:
            fprintf(stderr, "How did we get here?\n");
            exit(EXIT_FAILURE);
    }

    free_resources(params, extra_data, shadow_images, secret_image);
    return (ret_write == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}





#include <stdio.h>
#include <stdlib.h>

#include "../include/galois.h"
#include "../include/params.h"
#include "../include/images.h"
#include "../include/distribution.h"

#define TRUE 1
#define FALSE 0


int main(int argc, char *argv[]) {
    galois_init();
    struct stenography * params = parse_params(argc, argv);
    struct image_extras * extra_data = read_image_extras(params->shadow_images_paths[0], params->k_number);
    struct image ** shadow_images = read_images_from_file(params->shadow_images_paths, params->shadow_images_count, params->k_number, FALSE, extra_data);
    struct image * secret_image;

    switch (params->action) {
        case DECODE :
            secret_image = read_image_from_file(params->secret_image_path, params->k_number, TRUE, extra_data);
            distribute(*secret_image, shadow_images, params->shadow_images_count);
            write_images(shadow_images, params->shadow_images_count, FALSE, extra_data);
            break;
        case RETRIEVE:
            secret_image = recover(shadow_images, params->k_number, (extra_data->height * extra_data->width) / params->k_number, params->secret_image_path);
            write_image(secret_image, TRUE, extra_data);
            break;
        default:
            fprintf(stderr, "How the hell did we got here?\n");
            exit(1);
    }

    image_destroy(secret_image);
    images_destroy(shadow_images, params->shadow_images_count);
    image_extras_destroy(extra_data);
    destroy_params(params);
    return 0;
}





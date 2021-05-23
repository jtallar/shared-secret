#include <stdio.h>
#include <stdlib.h>

#include "../include/params.h"
#include "../include/images.h"
#include "../include/distribution.h"

#define TRUE 1
#define FALSE 0


int main(int argc, char *argv[]) {

    struct stenography params = parse_params(argc, argv);

    struct image ** shadow_images = read_shadow_images_from_file(params.shadow_images_paths, params.k_number, params.shadow_images_count);
    struct image * secret_image;

    switch (params.action) {
        case DECODE :
            secret_image = read_image_from_file(params.secret_image_path, params.k_number);
            distribute(*secret_image, shadow_images, params.shadow_images_count);
            write_images(shadow_images, params.shadow_images_count, FALSE);
            break;
        case RETRIEVE:
            secret_image = recover(shadow_images, params.shadow_images_count, 1); // TODO L / K
            write_images(&secret_image, 1, TRUE);
            break;
        default:
            fprintf(stderr, "How the hell did we got here?\n");
            exit(1);
    }
    return 0;
}





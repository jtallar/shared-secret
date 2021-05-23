#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h> // reading directory
#include <stdlib.h> // malloc
#include <string.h> // string
#include "../include/params.h"


int main(int argc, char *argv[]) {

    struct stenography params = parse_params(argc, argv);

    for (int i = 0; i < params.shadow_images; ++i) {
        printf("%s\n", params.shadow_images_names[i]);
    }
    switch (params.action) {
        case DECODE:

            // TODO get shadow images (create the array of structs)

            // TODO secret image should exist
            // TODO get header and body of one shadow image (create the struct)
            // TODO distribute
            // TODO write the shadows
            break;
        case RETRIEVE:
            // TODO get shadow images (create the array of structs)
            // TODO get header of one shadow image (create the struct)

            // TODO retrieve
            // TODO create the secrete image



            break;
        default:
            return -1;
    }

    printf("Action: %d, Image: %s, K: %d\n", params.action, params.image_name, params.k_number);
    return 0;
}





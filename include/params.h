//
// Created by Matias Brula on 23/05/2021.
//

#ifndef STENOGRAPHY_SHARED_SECRET_PARAMS_H
#define STENOGRAPHY_SHARED_SECRET_PARAMS_H

enum actions {DECODE = 0, RETRIEVE};

struct stenography {
    enum actions action;
    char * image_name;
    uint8_t k_number;
    char ** shadow_images_names;
    uint8_t shadow_images;
};

struct stenography parse_params(int argc, char *argv[]);

#endif //STENOGRAPHY_SHARED_SECRET_PARAMS_H

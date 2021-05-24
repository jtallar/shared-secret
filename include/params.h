//
// Created by Matias Brula on 23/05/2021.
//

#ifndef STENOGRAPHY_SHARED_SECRET_PARAMS_H
#define STENOGRAPHY_SHARED_SECRET_PARAMS_H

enum actions {DECODE = 0, RETRIEVE};

struct stenography {
    enum actions action;
    char * secret_image_path;
    uint8_t k_number;
    char ** shadow_images_paths;
    uint8_t shadow_images_count;
};

struct stenography * parse_params(int argc, char *argv[]);

void destroy_params(struct stenography * params);

#endif //STENOGRAPHY_SHARED_SECRET_PARAMS_H

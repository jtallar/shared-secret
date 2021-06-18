//
// Created by Matias Brula on 23/05/2021.
//
#include <stdio.h>
#include <stdint.h> // uintX_t
#include <stdlib.h> // strtol
#include <string.h> // string
#include <dirent.h> // reading directory

#include "./include/params.h"

#define ACTION          1
#define IMAGE_NAME      2
#define K_NUMBER        3
#define DIRECTORY       4

static void print_stderr(char * message) {
    fprintf(stderr, "%s", message);
}

static uint8_t is_bmp(const char * filename, uint16_t length) {
    return filename[length - 4] == '.' &&
           filename[length - 3] == 'b' &&
           filename[length - 2] == 'm' &&
           filename[length - 1] == 'p';
}

static char ** get_shadow_images(const char * path, uint8_t k, uint8_t * shadow_images) {
    DIR * dir;
    struct dirent * entry;
    uint8_t count = 0;

    // count the amount of .bmp files
    dir = opendir(path);
    if (dir == NULL) {
        print_stderr("Invalid directory for shadow images.\n");
        return NULL;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && is_bmp(entry->d_name, strlen(entry->d_name)))
            count++;
    }
    closedir(dir);

    // check a valid amount of shadow files
    if (k > count) {
        print_stderr("Number k must be smaller or equal to the amount of shadow images.\n");
        return NULL;
    }
    *shadow_images = count;

    // init the array of strings to return
    char ** shadow_images_names = malloc(sizeof(char *) * count);
    if (shadow_images_names == NULL) {
        print_stderr("Not enough heap memory for shadow images names.\n");
        return NULL;
    }

    // iterate over the .bmp files and compose their filepath
    count = 0;
    dir = opendir(path);
    uint16_t path_len = strlen(path);
    if (dir == NULL) {
        print_stderr("Invalid directory for shadow images.\n");
        free(shadow_images_names);
        return NULL;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && is_bmp(entry->d_name, strlen(entry->d_name))) {
            shadow_images_names[count] = malloc(path_len + strlen(entry->d_name) + 1);
            if (shadow_images_names[count] == NULL) {
                print_stderr("Not enough heap memory.\n");
                for (int i = 0; i < count; i++) free(shadow_images_names[i]);
                free(shadow_images_names);
                closedir(dir);
                return NULL;
            }

            // compose the shadow image filepath
            strcpy(shadow_images_names[count], path);
            strcpy(shadow_images_names[count] + path_len, entry->d_name);
            count++;
        }

    }
    closedir(dir);

    return shadow_images_names;
}

struct stenography * parse_params(int argc, char *argv[]) {
    if (argc != 5) {
        print_stderr("Wrong amount of params.\nShould be exactly 4: ./ss (d|r) file.bmp k directory/\n");
        return NULL;
    }

    struct stenography * params = calloc(1, sizeof(struct stenography));
    if (params == NULL) {
        print_stderr("Not enough heap memory to save program params.\n");
        return NULL;
    }

    // get the action to do
    if (strcmp(argv[ACTION], "d") == 0) params->action = DECODE;
    else if (strcmp(argv[ACTION], "r") == 0) params->action = RETRIEVE;
    else {
        print_stderr("Wrong action param option.\nShould be d for decode or, r for retrieve.\n");
        destroy_params(params);
        return NULL;
    }

    if (!is_bmp(argv[IMAGE_NAME], strlen(argv[IMAGE_NAME]))) {
        print_stderr("Secret is not bmp file.\n");
        destroy_params(params);
        return NULL;
    }

    // retrieve the image name
    params->secret_image_path = malloc(sizeof(char) * strlen(argv[IMAGE_NAME]) + 1);
    if (params->secret_image_path == NULL) {
        print_stderr("Not enough heap memory to save secret image path.\n");
        destroy_params(params);
        return NULL;
    }
    strcpy(params->secret_image_path, argv[IMAGE_NAME]);

    // get the number k
    params->k_number = (int) strtol(argv[K_NUMBER], (char **) NULL, 10);
    if (params->k_number < 4) {
        print_stderr("Wrong format number.\nThird param should be a number greater or equal than 4.\n");
        destroy_params(params);
        return NULL;
    }

    // retrieve the directory path
    params->shadow_images_paths = get_shadow_images(argv[DIRECTORY], params->k_number, &params->shadow_images_count);
    if (params->shadow_images_paths == NULL) {
        destroy_params(params);
        return NULL;
    }

    return params;
}

void destroy_params(struct stenography * params) {
    if (params == NULL) return;
    if (params->secret_image_path != NULL) {
        free(params->secret_image_path);
        params->secret_image_path = NULL;
    }
    if (params->shadow_images_paths != NULL) {
        for (int i = 0; i < params->shadow_images_count; ++i) {
            free(params->shadow_images_paths[i]);
            params->shadow_images_paths[i] = NULL;
        }
        free(params->shadow_images_paths);
        params->shadow_images_paths = NULL;
    }
    params->shadow_images_count = 0;
    params->k_number = 0;
    free(params);
}

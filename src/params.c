//
// Created by Matias Brula on 23/05/2021.
//
#include <stdio.h>
#include <stdlib.h> // strtol
#include <string.h> // string
#include <dirent.h> // reading directory

#include "params.h"
#define ACTION          1
#define IMAGE_NAME      2
#define K_NUMBER        3
#define DIRECTORY       4

static void stderr_and_exit(char * message) {
    fprintf(stderr, "%s", message);
    exit(1);
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
    if (dir == NULL) stderr_and_exit("Invalid directory for shadow images.\n");
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && is_bmp(entry->d_name, entry->d_namlen))
            count++;
    }
    closedir(dir);

    // check a valid amount of shadow files
    if (k > count) stderr_and_exit("Number k must be smaller or equal to the mount of shadow images.\n");
    *shadow_images = count;

    // init the array of strings to return
    char ** shadow_images_names = malloc(sizeof(char *) * count);
    if (shadow_images_names == NULL) stderr_and_exit("Not enough heap memory.\n");

    // iterate over the .bmp files and compose their filepath
    count = 0;
    dir = opendir(path);
    uint16_t path_len = strlen(path);
    if (dir == NULL) stderr_and_exit("Invalid directory for shadow images.\n");
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && is_bmp(entry->d_name, entry->d_namlen)) {
            shadow_images_names[count] = malloc(path_len + entry->d_namlen + 1);
            if (shadow_images_names[count] == NULL) stderr_and_exit("Not enough heap memory.\n");

            // compose the shadow image filepath
            strcpy(shadow_images_names[count], path);
            strcpy(shadow_images_names[count] + path_len, entry->d_name);
            count++;
        }

    }
    return shadow_images_names;
}

struct stenography parse_params(int argc, char *argv[]) {
    if (argc != 5) stderr_and_exit("Wrong amount of params.\nShould be exactly 4: ./ss [d|r] file.bmp [k] directory/\n");

    struct stenography params;

    // get the action to do
    if (strcmp(argv[ACTION], "d") == 0) params.action = DECODE;
    else if (strcmp(argv[ACTION], "r") == 0) params.action = RETRIEVE;
    else stderr_and_exit("Wrong action param option.\nShould be d for decode or, r for retrieve.\n");

    // retrieve the image name
    params.image_name = argv[IMAGE_NAME];

    // get the number k
    params.k_number = (int) strtol(argv[K_NUMBER], (char **) NULL, 10);
    if (params.k_number == 0) stderr_and_exit("Wrong format number.\nThird param should be a number.\n");

    // retrieve the directory path
    params.shadow_images_names = get_shadow_images(argv[DIRECTORY], params.k_number, &params.shadow_images);

    return params;
}

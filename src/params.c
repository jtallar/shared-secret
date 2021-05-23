//
// Created by Matias Brula on 23/05/2021.
//
#include <stdio.h>
#include <stdlib.h> // strtol
#include <string.h> // string

#include "params.h"
#define ACTION          1
#define IMAGE_NAME      2
#define K_NUMBER        3
#define DIRECTORY       4

void stderr_and_exit(char * message) {
    fprintf(stderr, "%s", message);
    exit(1);
}

struct stenography parse_params(int argc, char *argv[]) {
    if (argc != 5) stderr_and_exit("Wrong amount of params.\n Should be exactly 4: ./ss [d|r] file.bmp [k] directory/\n");

    struct stenography params;

    // get the action to do
    if (strcmp(argv[ACTION], "d") == 0) params.action = DECODE;
    else if (strcmp(argv[ACTION], "r") == 0) params.action = RETRIEVE;
    else stderr_and_exit("Wrong action param option.\n Should be d for decode or, r for retrieve.\n");

    // retrieve the image name
    params.image_name = argv[IMAGE_NAME];

    // get the number k
    params.k_number = (int) strtol(argv[K_NUMBER], (char **) NULL, 10);
    if (params.k_number == 0) stderr_and_exit("Wrong format number.\n Third param should be a number.\n");

    // retrieve the directory path
    params.directory = argv[DIRECTORY];

    return params;
}


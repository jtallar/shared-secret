#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "../include/params.h"

int main(int argc, char *argv[]) {

    struct stenography params = parse_params(argc, argv);

    printf("Action: %d, Image: %s, K: %d, Directory: %s\n", params.action, params.image_name, params.k_number, params.directory);
    return 0;
}

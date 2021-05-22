#include <stdio.h>
#include <stdint.h>
#include "distribution.h"
#include "images.h"

#define SHADOW_COUNT    3

int main() {

    uint8_t secret[2][3] = {{18, 19, 12}, {20, 21, 14}};
    uint8_t shadows[3][2][4] = {
        {{42, 43, 36, 37}, {44, 45, 38, 39}},
        {{46, 47, 40, 41}, {30, 31, 24, 25}},
        {{32, 33, 26, 27}, {34, 35, 28, 29}}
    };

    struct image * secret_img = new_empty_image(2, 3);
    for (uint8_t i = 0; i < 2; i++) {
        for (uint8_t j = 0; i < 3; i++) {
            secret_img->blocks[i].elements[j] = secret[i][j];
        }
    }

    struct image * shadows_img[SHADOW_COUNT];
    for (uint8_t k = 0; k < SHADOW_COUNT; k++) {
        shadows_img[k] = new_empty_image(2, 4);
        for (uint8_t i = 0; i < 2; i++) {
            for (uint8_t j = 0; i < 3; i++) {
                shadows_img[k]->blocks[i].elements[j] = shadows[k][i][j];
            }
        }
    }

    printf("Original Secret image\n");
    image_print(*secret_img);
    printf("Original Shadows\n");
    for (uint8_t k = 0; k < SHADOW_COUNT; k++) {
        image_print(*shadows_img[k]);
    }

    distribute(*secret_img, shadows_img, SHADOW_COUNT);

    printf("Distributed Shadows\n");
    for (uint8_t k = 0; k < SHADOW_COUNT; k++) {
        image_print(*shadows_img[k]);
    }

    struct image * recovery = recover(shadows_img, 3, 2);

    printf("Recovered Secret image\n");
    image_print(*recovery);

    image_destroy(secret_img);
    for (uint8_t k = 0; k < SHADOW_COUNT; k++) {
        image_destroy(shadows_img[k]);
    }

    return 0;
}

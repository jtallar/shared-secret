#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include "include/galois.h"
//#include "include/galois.h"

int main() {
    gal_print(84);
    gal_print(13);
    printf("Add = %d, Sub = %d, Mul = %d, Div = %d", gal_add(84,13), gal_sub(84,13), gal_mul(84,13), gal_div(84,13));
    return 0;
}
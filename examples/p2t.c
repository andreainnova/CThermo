#include "../include/thermo.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    print_refrigerant();
    if (argc < 2) {
        printf("Usage: %s <pressure>\n", argv[0]);
        return 1;
    }
    printf("Temperature: %d\n", pressure_to_temperature((uint16_t)atoi(argv[1])));
    return 0;
}
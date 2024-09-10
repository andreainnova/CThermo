#include "../include/thermo.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <temperature>\n", argv[0]);
        return 1;
    }
    printf("Pressure: %d\n", temperature_to_pressure((int16_t)atoi(argv[1])));
    return 0;
}
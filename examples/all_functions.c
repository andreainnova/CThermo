#include "../include/thermo.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <function> <values>\n", argv[0]);
        return 1;
    }
    char *function = argv[1];

    if (strcmp(function, "pressure_to_temperature") == 0) {
        printf("Temperature: %d\n", pressure_to_temperature((uint16_t)atoi(argv[2])));

    } else if (strcmp(function, "temperature_to_pressure") == 0) {
        printf("Pressure: %d\n", temperature_to_pressure((int16_t)atoi(argv[2])));

    } else if (strcmp(function, "gas_density") == 0) {
        printf("Density: %d\n", gas_density((int16_t)atoi(argv[2])));

    } else if (strcmp(function, "evaporation_dH") == 0) {
        if (argc < 4) {
            printf("Usage: %s evaporation_dH <evap_temperature> <cond_temperature>\n", argv[0]);
            return 1;
        }
        printf("Enthalpy difference: %d\n", evaporation_dH((int16_t)atoi(argv[2]), (int16_t)atoi(argv[3])));

    } else if (strcmp(function, "condensation_dH") == 0) {
        if (argc < 4) {
            printf("Usage: %s condensation_dH <evap_temperature> <cond_temperature>\n", argv[0]);
            return 1;
        }
        printf("Enthalpy difference: %d\n", condensation_dH((int16_t)atoi(argv[2]), (int16_t)atoi(argv[3])));

    } else if (strcmp(function, "compressor_dH") == 0) {
        if (argc < 4) {
            printf("Usage: %s compressor_dH <evap_temperature> <cond_temperature>\n", argv[0]);
            return 1;
        }
        printf("Enthalpy difference: %d\n", compressor_dH((int16_t)atoi(argv[2]), (int16_t)atoi(argv[3])));
        
    } else if (strcmp(function, "calculate_mass_rate") == 0) {
        if (argc < 5) {
            printf("Usage: %s calculate_mass_rate <compressor_volume> <compressor_speed> <refrigerant_temperature>\n", argv[0]);
            return 1;
        }
        printf("Mass flow rate: %d\n", calculate_mass_rate((uint16_t)atoi(argv[2]), (uint16_t)atoi(argv[3]), (int16_t)atoi(argv[4])));

    } else if (strcmp(function, "calculate_evaporation_power") == 0) {
        if (argc < 5) {
            printf("Usage: %s calculate_evaporation_power <compressor_volume> <compressor_speed> <evap_temperature> <cond_temperature>\n", argv[0]);
            return 1;
        }
        printf("Power: %d\n", calculate_evaporation_power((uint16_t)atoi(argv[2]), (uint16_t)atoi(argv[3]), (int16_t)atoi(argv[4]), (int16_t)atoi(argv[5])));

    } else if (strcmp(function, "calculate_condensation_power") == 0) {
        if (argc < 5) {
            printf("Usage: %s calculate_condensation_power <compressor_volume> <compressor_speed> <evap_temperature> <cond_temperature>\n", argv[0]);
            return 1;
        }
        printf("Power: %d\n", calculate_condensation_power((uint16_t)atoi(argv[2]), (uint16_t)atoi(argv[3]), (int16_t)atoi(argv[4]), (int16_t)atoi(argv[5])));

    } else if (strcmp(function, "calculate_compressor_power") == 0) {
        if (argc < 5) {
            printf("Usage: %s calculate_compressor_power <compressor_volume> <compressor_speed> <evap_temperature> <cond_temperature>\n", argv[0]);
            return 1;
        }
        printf("Power: %d\n", calculate_compressor_power((uint16_t)atoi(argv[2]), (uint16_t)atoi(argv[3]), (int16_t)atoi(argv[4]), (int16_t)atoi(argv[5])));
        
    } else if (strcmp(function, "calculate_discharge_target") == 0) {
        if (argc < 4) {
            printf("Usage: %s calculate_discharge_target <evap_temperature> <cond_temperature>\n", argv[0]);
            return 1;
        }
        printf("Discharge target: %d\n", calculate_discharge_target((int16_t)atoi(argv[2]), (int16_t)atoi(argv[3])));

    } else if (strcmp(function, "calculate_UA") == 0) {
        if (argc < 5) {
            printf("Usage: %s calculate_UA <power> <refrigerant_temperature> <medium_temperature>\n", argv[0]);
            return 1;
        }
        printf("UA: %d\n", calculate_UA((uint16_t)atoi(argv[2]), (int16_t)atoi(argv[3]), (int16_t)atoi(argv[4])));

    } else if (strcmp(function, "calculate_glycol_mixture_CP") == 0) {
        if (argc < 3) {
            printf("Usage: %s calculate_CP <glicole_percentage>\n", argv[0]);
            return 1;
        }
        printf("CP: %d\n", calculate_glycol_mixture_CP((uint16_t)atoi(argv[2])));

    } else {
        printf("Function not found\n");
        return 1;
    }

    return 0;
}
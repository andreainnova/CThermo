#include "thermo.h"
#include <stdio.h>
#include <stdint.h>

#if REFR_ == REFR_R290
    static const char refr_name[] = "R290";
    static const uint16_t refr_pressures[10] = {1111, 2049, 3497, 5606, 8536, 12462, 17569, 24072, 32236, 42512}; // mBar
    static const int16_t refr_temperatures[10] = {-400, -248, -96, 56, 208, 360, 512, 664, 815, 967}; // 0.1°C
    static const uint16_t refr_densities[10] = {26, 47, 77, 122, 185, 273, 398, 581, 885, 2205}; // 0.1kg/m^3

    // Constants for the enthalpy of evaporation and condensation, dH = A + B*evap_temperature/10000 + C*cond_temperature/10000
    static const int16_t evap_dH_A = 3808; // J/0.1g
    static const int16_t evap_dH_B = 975; // J/kg*C
    static const int16_t evap_dH_C = -2945; // J/kg*C
    static const int16_t cond_dH_A = 3874; // J/0.1g
    static const int16_t cond_dH_B = -752; // J/kg*C
    static const int16_t cond_dH_C = -1482; // J/kg*C

    // Constants for the discharge target, discharge temperature = A + B*evap_temperature/10 + C*cond_temperature/10
    static const int16_t disch_A = 64; // 0.1°C
    static const int16_t disch_B = -38; // 10
    static const int16_t disch_C = 119; // 10

#elif REFR_ == REFR_R32
    static const char refr_name[] = "R32";
    static const uint16_t refr_pressures[10] = {1774, 3105, 5104, 7968, 11912, 17176, 24029, 32782, 43826, 57826}; // mBar
    static const int16_t refr_temperatures[10] = {-400, -269, -138, -6, 125, 256, 387, 519, 650, 781}; // 0.1°C
    static const uint16_t refr_densities[10] = {51, 86, 139, 216, 326, 482, 706, 1043, 1610, 4240}; // 0.1kg/m^3

    // Constants for the enthalpy of evaporation and condensation
    static const int16_t evap_dH_A = 3160; // J/0.1g
    static const int16_t evap_dH_B = 48; // J/kg*C
    static const int16_t evap_dH_C = -1993; // J/kg*C
    static const int16_t cond_dH_A = 3170; // J/0.1g
    static const int16_t cond_dH_B = -1698; // J/kg*C
    static const int16_t cond_dH_C = -468; // J/kg*C

    // Constants for the discharge target, discharge temperature = A + B*evap_temperature/10 + C*cond_temperature/10
    static const int16_t disch_A = 75; // 0.1°C
    static const int16_t disch_B = -139; // 10
    static const int16_t disch_C = 196; // 10

#elif REFR_ == REFR_R410A
    static const char refr_name[] = "R410A";
    static const uint16_t refr_pressures[10] = {1748, 2967, 4756, 7272, 10684, 15182, 20974, 28307, 37490, 49012}; // mBar
    static const int16_t refr_temperatures[10] = {-400, -276, -153, -29, 95, 219, 342, 466, 590, 713}; // 0.1°C
    static const uint16_t refr_densities[10] = {70, 116, 183, 278, 413, 600, 868, 1266, 1931, 4605}; // 0.1kg/m^3

    // Constants for the enthalpy of evaporation and condensation
    static const int16_t evap_dH_A = 2220; // J/0.1g
    static const int16_t evap_dH_B = 222; // J/kg*C
    static const int16_t evap_dH_C = -1705; // J/kg*C
    static const int16_t cond_dH_A = 2230; // J/0.1g
    static const int16_t cond_dH_B = -941; // J/kg*C
    static const int16_t cond_dH_C = -687; // J/kg*C

    // Constants for the discharge target, discharge temperature = A + B*evap_temperature/10 + C*cond_temperature/10
    static const int16_t disch_A = 608; // 0.1°C
    static const int16_t disch_B = -77; // 10
    static const int16_t disch_C = 154; // 10

#else
#error "Unsupported refrigerant type"
#endif

// Ensure the arrays have the correct size
_Static_assert(sizeof(refr_pressures) / sizeof(refr_pressures[0]) == 10, "refr_pressures must have 10 elements");
_Static_assert(sizeof(refr_temperatures) / sizeof(refr_temperatures[0]) == 10, "refr_temperatures must have 10 elements");

void print_refrigerant(void) {
    printf("%s\n", refr_name);
}

int16_t pressure_to_temperature(const uint16_t pressure) {
    if (pressure <= refr_pressures[0]) {
        return refr_temperatures[0];
    }
    if (pressure >= refr_pressures[9]) {
        return refr_temperatures[9];
    }
    for (uint8_t i = 1; i < 10; i++) {
        if (pressure < refr_pressures[i]) {
            uint16_t p1 = refr_pressures[i - 1];
            uint16_t p2 = refr_pressures[i];
            int16_t t1 = refr_temperatures[i - 1];
            int16_t t2 = refr_temperatures[i];
            int16_t partial = ((int32_t)(pressure - p1) * (t2 - t1))/(p2 - p1);
            return t1 + partial;
        }
    }
    // Should never reach here if the pressures array is well-defined
    return refr_temperatures[9];
}

uint16_t temperature_to_pressure(const int16_t temperature) {
    if (temperature <= refr_temperatures[0]) {
        return refr_pressures[0];
    }
    if (temperature >= refr_temperatures[9]) {
        return refr_pressures[9];
    }
    for (uint8_t i = 1; i < 10; i++) {
        if (temperature < refr_temperatures[i]) {
            uint16_t p1 = refr_pressures[i - 1];
            uint16_t p2 = refr_pressures[i];
            int16_t t1 = refr_temperatures[i - 1];
            int16_t t2 = refr_temperatures[i];
            int16_t partial = ((int32_t)(temperature - t1) * (p2 - p1))/(t2 - t1);
            return p1 + partial;
        }
    }
    // Should never reach here if the temperatures array is well-defined
    return refr_pressures[9];
}

uint16_t gas_density(const int16_t evaporation_temperature) {
    if (evaporation_temperature <= refr_temperatures[0]) {
        return refr_densities[0];
    }
    if (evaporation_temperature >= refr_temperatures[9]) {
        return refr_densities[9];
    }
    for (uint8_t i = 1; i < 10; i++) {
        if (evaporation_temperature < refr_temperatures[i]) {
            uint16_t d1 = refr_densities[i - 1];
            uint16_t d2 = refr_densities[i];
            int16_t t1 = refr_temperatures[i - 1];
            int16_t t2 = refr_temperatures[i];
            int16_t partial = ((int32_t)(evaporation_temperature - t1) * (d2 - d1))/(t2 - t1);
            return d1 + partial;
        }
    }
    // Should never reach here if the temperatures array is well-defined
    return refr_densities[9];
}

uint16_t evaporation_dH(const int16_t evap_temperature, const int16_t cond_temperature) {
    const int32_t evap_corr = ((int32_t)evap_dH_B * evap_temperature) / 1000;
    const int32_t cond_corr = ((int32_t)evap_dH_C * cond_temperature) / 1000;
    const int32_t dH = (int32_t)evap_dH_A + evap_corr + cond_corr;
    if (dH < 0) return 0;
    if (dH > 65535) return 65535;
    return dH;
}

uint16_t condensation_dH(const int16_t evap_temperature, const int16_t cond_temperature) {
    const int32_t evap_corr = ((int32_t)cond_dH_B * evap_temperature) / 1000;
    const int32_t cond_corr = ((int32_t)cond_dH_C * cond_temperature) / 1000;
    const int32_t dH = (int32_t)cond_dH_A + evap_corr + cond_corr;
    if (dH < 0) return 0;
    if (dH > 65535) return 65535;
    return dH;
}

uint16_t calculate_mass_rate(const uint16_t compressor_volume, const uint16_t compressor_speed, const int16_t refrigerant_temperature) {
    int32_t volume_rate = (int32_t)compressor_volume * compressor_speed / 100;
    if (volume_rate > 65535) volume_rate = 65535;

    int32_t mass_rate = (int32_t)volume_rate * gas_density(refrigerant_temperature) / 1000;
    if (mass_rate > 65535) return 65535;
    return mass_rate;
}

uint16_t calculate_evaporation_power(const uint16_t compressor_volume, const uint16_t compressor_speed, const int16_t evap_temperature, const int16_t cond_temperature) {
    const uint16_t mass_rate = calculate_mass_rate(compressor_volume, compressor_speed, evap_temperature);
    const uint16_t dH = evaporation_dH(evap_temperature, cond_temperature);
    const uint32_t power = (uint32_t)mass_rate * dH / 100;
    if (power > 65535) return 65535;
    return power;
}

uint16_t calculate_condensation_power(const uint16_t compressor_volume, const uint16_t compressor_speed, const int16_t evap_temperature, const int16_t cond_temperature) {
    const uint16_t mass_rate = calculate_mass_rate(compressor_volume, compressor_speed, evap_temperature);
    const uint16_t dH = condensation_dH(evap_temperature, cond_temperature);
    const uint32_t power = (uint32_t)mass_rate * dH / 100;
    if (power > 65535) return 65535;
    return power;
}

int16_t calculate_discharge_target(const int16_t evap_temperature, const int16_t cond_temperature) {
    const int32_t evap_corr = ((int32_t)disch_B * evap_temperature)/10;
    const int32_t cond_corr = ((int32_t)disch_C * cond_temperature)/10;
    const int32_t disch = (int32_t)disch_A + evap_corr + cond_corr;
    if (disch > 65535) return 65535;
    return disch;
}

uint16_t calculate_UA(const uint16_t power, const int16_t refrigerant_temperature, const int16_t medium_temperature) {
    int32_t deltaT = refrigerant_temperature - medium_temperature;
    deltaT = deltaT < 0 ? -deltaT : deltaT;
    deltaT = deltaT < 5 ? 5 : deltaT; // Minimum deltaT is 0.5°C for better stability
    const int32_t UA = (int32_t)power / deltaT;
    if (UA > 65535) return 65535;
    return UA;
}
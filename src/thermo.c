#include <stdint.h>
#include "thermo.h"

#define max_(a, b)           ((a) > (b) ? (a) : (b))
#define min_(a, b)           ((a) < (b) ? (a) : (b))
#define abs_(val)            ((val) > 0 ? (val) : -(val))

#if REFR_ == REFR_R290
    static const uint16_t refr_pressures[15] = {1111, 1663, 2404, 3374, 4611, 6159, 8061, 10363, 13114, 16365, 20171, 24596, 29714, 35625, 42512}; // mBar
    static const int16_t refr_temperatures[15] = {-400, -302, -205, -107, -9, 88, 186, 284, 381, 479, 577, 674, 772, 870, 967}; // 0.1°C
    static const uint16_t refr_densities[15] = {26, 38, 54, 75, 101, 133, 174, 225, 288, 367, 467, 598, 778, 1058, 2205}; // 0.1kg/m^3

    static const int16_t evap_dH_A = 3808; // J/0.1g
    static const int16_t evap_dH_B = 975; // J/kg*C
    static const int16_t evap_dH_C = -2945; // J/kg*C
    static const int16_t cond_dH_A = 3874; // J/0.1g
    static const int16_t cond_dH_B = -752; // J/kg*C
    static const int16_t cond_dH_C = -1482; // J/kg*C
    static const int16_t comp_dH_B = -1766; // J/kg*C
    static const int16_t comp_dH_C = 1588; // J/kg*C

    static const int16_t disch_A = 64; // 0.1°C
    static const int16_t disch_B = -38; // 10
    static const int16_t disch_C = 119; // 10

#elif REFR_ == REFR_R32
    static const uint16_t refr_pressures[15] = {1774, 2562, 3600, 4935, 6621, 8714, 11273, 14363, 18051, 22412, 27527, 33489, 40408, 48428, 57826}; // mBar
    static const int16_t refr_temperatures[15] = {-400, -316, -231, -147, -63, 22, 106, 191, 275, 359, 444, 528, 612, 697, 781}; // 0.1°C
    static const uint16_t refr_densities[15] = {51, 72, 99, 135, 180, 237, 308, 397, 509, 651, 833, 1074, 1410, 1939, 4240}; // 0.1kg/m^3

    static const int16_t evap_dH_A = 3160; // J/0.1g
    static const int16_t evap_dH_B = 48; // J/kg*C
    static const int16_t evap_dH_C = -1993; // J/kg*C
    static const int16_t cond_dH_A = 3170; // J/0.1g
    static const int16_t cond_dH_B = -1698; // J/kg*C
    static const int16_t cond_dH_C = -468; // J/kg*C
    static const int16_t comp_dH_B = -1755; // J/kg*C
    static const int16_t comp_dH_C = 1548; // J/kg*C

    static const int16_t disch_A = 75; // 0.1°C
    static const int16_t disch_B = -139; // 10
    static const int16_t disch_C = 196; // 10

#elif REFR_ == REFR_R410A
    static const uint16_t refr_pressures[15] = {1748, 2474, 3413, 4606, 6094, 7921, 10135, 12785, 15925, 19612, 23912, 28897, 34655, 41298, 49012}; // mBar
    static const int16_t refr_temperatures[15] = {-400, -320, -241, -161, -82, -2, 77, 157, 236, 316, 395, 475, 554, 634, 713}; // 0.1°C
    static const uint16_t refr_densities[15] = {70, 97, 132, 177, 233, 303, 390, 498, 633, 802, 1018, 1302, 1696, 2317, 4605}; // 0.1kg/m^3

    static const int16_t evap_dH_A = 2220; // J/0.1g
    static const int16_t evap_dH_B = 222; // J/kg*C
    static const int16_t evap_dH_C = -1705; // J/kg*C
    static const int16_t cond_dH_A = 2230; // J/0.1g
    static const int16_t cond_dH_B = -941; // J/kg*C
    static const int16_t cond_dH_C = -687; // J/kg*C
    static const int16_t comp_dH_B = -1175; // J/kg*C
    static const int16_t comp_dH_C = 1047; // J/kg*C

    static const int16_t disch_A = 608; // 0.1°C
    static const int16_t disch_B = -77; // 10
    static const int16_t disch_C = 154; // 10

#else
#error "Unsupported refrigerant type"
#endif

int16_t pressure_to_temperature(const uint16_t pressure) {
    uint8_t i;

    if (pressure <= refr_pressures[0]) {
        return refr_temperatures[0];
    }
    if (pressure >= refr_pressures[14]) {
        return refr_temperatures[14];
    }
    for (i = 1; i < 15; i++) {
        if (pressure < refr_pressures[i]) {
            uint16_t p1 = refr_pressures[i - 1];
            uint16_t p2 = refr_pressures[i];
            int16_t t1 = refr_temperatures[i - 1];
            int16_t t2 = refr_temperatures[i];
            int16_t partial = ((int32_t)(pressure - p1) * (t2 - t1))/(p2 - p1);
            return t1 + partial;
        }
    }
    return refr_temperatures[14];
}

uint16_t temperature_to_pressure(const int16_t temperature) {
    uint8_t i;
    if (temperature <= refr_temperatures[0]) {
        return refr_pressures[0];
    }
    if (temperature >= refr_temperatures[14]) {
        return refr_pressures[14];
    }
    for (i = 1; i < 15; i++) {
        if (temperature < refr_temperatures[i]) {
            uint16_t p1 = refr_pressures[i - 1];
            uint16_t p2 = refr_pressures[i];
            int16_t t1 = refr_temperatures[i - 1];
            int16_t t2 = refr_temperatures[i];
            int16_t partial = ((int32_t)(temperature - t1) * (p2 - p1))/(t2 - t1);
            return p1 + partial;
        }
    }
    return refr_pressures[14];
}

uint16_t gas_density(const int16_t evap_temperature) {
    uint8_t i;
    if (evap_temperature <= refr_temperatures[0]) {
        return refr_densities[0];
    }
    if (evap_temperature >= refr_temperatures[14]) {
        return refr_densities[14];
    }
    for (i = 1; i < 15; i++) {
        if (evap_temperature < refr_temperatures[i]) {
            uint16_t d1 = refr_densities[i - 1];
            uint16_t d2 = refr_densities[i];
            int16_t t1 = refr_temperatures[i - 1];
            int16_t t2 = refr_temperatures[i];
            int16_t partial = ((int32_t)(evap_temperature - t1) * (d2 - d1))/(t2 - t1);
            return d1 + partial;
        }
    }
    return refr_densities[14];
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

uint16_t compressor_dH(const int16_t evap_temperature, const int16_t cond_temperature) {
    const int32_t evap_corr = ((int32_t)comp_dH_B * evap_temperature) / 1000;
    const int32_t cond_corr = ((int32_t)comp_dH_C * cond_temperature) / 1000;
    const int32_t dH = evap_corr + cond_corr;
    if (dH < 0) return 0;
    if (dH > 65535) return 65535;
    return dH;
}

uint16_t calculate_mass_rate(const uint16_t compressor_volume, const uint16_t compressor_speed, const int16_t evap_temperature) {
    int32_t volume_rate = min_((int32_t)compressor_volume * compressor_speed / 100, 65535);

    int32_t mass_rate = (int32_t)volume_rate * gas_density(evap_temperature) / 1000;
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

uint16_t calculate_compressor_power(const uint16_t compressor_volume, const uint16_t compressor_speed, const int16_t evap_temperature, const int16_t cond_temperature) {
    const uint16_t mass_rate = calculate_mass_rate(compressor_volume, compressor_speed, evap_temperature);
    const uint16_t dH = compressor_dH(evap_temperature, cond_temperature);
    const uint32_t power = (uint32_t)mass_rate * dH / 100;
    if (power > 65535) return 65535;
    return power;
}

int16_t calculate_discharge_target(const int16_t evap_temperature, const int16_t cond_temperature) {
    const int32_t evap_corr = ((int32_t)disch_B * evap_temperature)/100;
    const int32_t cond_corr = ((int32_t)disch_C * cond_temperature)/100;
    const int32_t disch = (int32_t)disch_A + evap_corr + cond_corr;
    if (disch > 32767) return 32767;
    return disch;
}

#define MIN_UA_DT (5)
uint16_t calculate_UA(const uint16_t power, const int16_t refrigerant_temperature, const int16_t medium_temperature) {
    int32_t deltaT = max_(MIN_UA_DT, abs_(refrigerant_temperature - medium_temperature));
    const int32_t UA = (int32_t)power * 10 / deltaT;
    if (UA > 65535) return 65535;
    return UA;
}

uint16_t calculate_glycol_mixture_CP(const uint16_t glycol_percentage) {
    if (glycol_percentage > 100) return 2450;
    return 4182 - ((173 * glycol_percentage)/10);
}

uint16_t calculate_atmospheric_pressure(const uint16_t altitude) {
    if (altitude > 5000) return 500;
    return 1004 - (altitude/10);
}

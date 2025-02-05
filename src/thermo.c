#include "thermo.h"
#include "stdtypes.h"

#define max_(a, b)           ((a) > (b) ? (a) : (b))
#define min_(a, b)           ((a) < (b) ? (a) : (b))
#define abs_(val)            ((val) > 0 ? (val) : -(val))

#if REFR_ == REFR_R290
    // static const char refr_name[] = "R290";
    static const u16 refr_pressures[10] = {1111, 2049, 3497, 5606, 8536, 12462, 17569, 24072, 32236, 42512}; // mBar
    static const s16 refr_temperatures[10] = {-400, -248, -96, 56, 208, 360, 512, 664, 815, 967}; // 0.1°C
    static const u16 refr_densities[10] = {26, 47, 77, 122, 185, 273, 398, 581, 885, 2205}; // 0.1kg/m^3

    // Constants for the enthalpy of evaporation and condensation, dH = A + B*evap_temperature/10000 + C*cond_temperature/10000
    static const s16 evap_dH_A = 3808; // J/0.1g
    static const s16 evap_dH_B = 975; // J/kg*C
    static const s16 evap_dH_C = -2945; // J/kg*C
    static const s16 cond_dH_A = 3874; // J/0.1g
    static const s16 cond_dH_B = -752; // J/kg*C
    static const s16 cond_dH_C = -1482; // J/kg*C
    static const s16 comp_dH_B = -1766; // J/kg*C
    static const s16 comp_dH_C = 1588; // J/kg*C

    // Constants for the discharge target, discharge temperature = A + B*evap_temperature/10 + C*cond_temperature/10
    static const s16 disch_A = 64; // 0.1°C
    static const s16 disch_B = -38; // 10
    static const s16 disch_C = 119; // 10

#elif REFR_ == REFR_R32
    // static const char refr_name[] = "R32";
    static const u16 refr_pressures[10] = {1774, 3105, 5104, 7968, 11912, 17176, 24029, 32782, 43826, 57826}; // mBar
    static const s16 refr_temperatures[10] = {-400, -269, -138, -6, 125, 256, 387, 519, 650, 781}; // 0.1°C
    static const u16 refr_densities[10] = {51, 86, 139, 216, 326, 482, 706, 1043, 1610, 4240}; // 0.1kg/m^3

    // Constants for the enthalpy of evaporation and condensation
    static const s16 evap_dH_A = 3160; // J/0.1g
    static const s16 evap_dH_B = 48; // J/kg*C
    static const s16 evap_dH_C = -1993; // J/kg*C
    static const s16 cond_dH_A = 3170; // J/0.1g
    static const s16 cond_dH_B = -1698; // J/kg*C
    static const s16 cond_dH_C = -468; // J/kg*C
    static const s16 comp_dH_B = -1755; // J/kg*C
    static const s16 comp_dH_C = 1548; // J/kg*C

    // Constants for the discharge target, discharge temperature = A + B*evap_temperature/10 + C*cond_temperature/10
    static const s16 disch_A = 75; // 0.1°C
    static const s16 disch_B = -139; // 10
    static const s16 disch_C = 196; // 10

#elif REFR_ == REFR_R410A
    // static const char refr_name[] = "R410A";
    static const u16 refr_pressures[10] = {1748, 2967, 4756, 7272, 10684, 15182, 20974, 28307, 37490, 49012}; // mBar
    static const s16 refr_temperatures[10] = {-400, -276, -153, -29, 95, 219, 342, 466, 590, 713}; // 0.1°C
    static const u16 refr_densities[10] = {70, 116, 183, 278, 413, 600, 868, 1266, 1931, 4605}; // 0.1kg/m^3

    // Constants for the enthalpy of evaporation and condensation
    static const s16 evap_dH_A = 2220; // J/0.1g
    static const s16 evap_dH_B = 222; // J/kg*C
    static const s16 evap_dH_C = -1705; // J/kg*C
    static const s16 cond_dH_A = 2230; // J/0.1g
    static const s16 cond_dH_B = -941; // J/kg*C
    static const s16 cond_dH_C = -687; // J/kg*C
    static const s16 comp_dH_B = -1175; // J/kg*C
    static const s16 comp_dH_C = 1047; // J/kg*C

    // Constants for the discharge target, discharge temperature = A + B*evap_temperature/10 + C*cond_temperature/10
    static const s16 disch_A = 608; // 0.1°C
    static const s16 disch_B = -77; // 10
    static const s16 disch_C = 154; // 10

#else
#error "Unsupported refrigerant type"
#endif

s16 pressure_to_temperature(const u16 pressure) {
    u8 i;

    if (pressure <= refr_pressures[0]) {
        return refr_temperatures[0];
    }
    if (pressure >= refr_pressures[9]) {
        return refr_temperatures[9];
    }
    for (i = 1; i < 10; i++) {
        if (pressure < refr_pressures[i]) {
            u16 p1 = refr_pressures[i - 1];
            u16 p2 = refr_pressures[i];
            s16 t1 = refr_temperatures[i - 1];
            s16 t2 = refr_temperatures[i];
            s16 partial = ((s32)(pressure - p1) * (t2 - t1))/(p2 - p1);
            return t1 + partial;
        }
    }
    // Should never reach here if the pressures array is well-defined
    return refr_temperatures[9];
}

u16 temperature_to_pressure(const s16 temperature) {
    u8 i;
    if (temperature <= refr_temperatures[0]) {
        return refr_pressures[0];
    }
    if (temperature >= refr_temperatures[9]) {
        return refr_pressures[9];
    }
    for (i = 1; i < 10; i++) {
        if (temperature < refr_temperatures[i]) {
            u16 p1 = refr_pressures[i - 1];
            u16 p2 = refr_pressures[i];
            s16 t1 = refr_temperatures[i - 1];
            s16 t2 = refr_temperatures[i];
            s16 partial = ((s32)(temperature - t1) * (p2 - p1))/(t2 - t1);
            return p1 + partial;
        }
    }
    // Should never reach here if the temperatures array is well-defined
    return refr_pressures[9];
}

u16 gas_density(const s16 evap_temperature) {
    u8 i;
    if (evap_temperature <= refr_temperatures[0]) {
        return refr_densities[0];
    }
    if (evap_temperature >= refr_temperatures[9]) {
        return refr_densities[9];
    }
    for (i = 1; i < 10; i++) {
        if (evap_temperature < refr_temperatures[i]) {
            u16 d1 = refr_densities[i - 1];
            u16 d2 = refr_densities[i];
            s16 t1 = refr_temperatures[i - 1];
            s16 t2 = refr_temperatures[i];
            s16 partial = ((s32)(evap_temperature - t1) * (d2 - d1))/(t2 - t1);
            return d1 + partial;
        }
    }
    // Should never reach here if the temperatures array is well-defined
    return refr_densities[9];
}

u16 evaporation_dH(const s16 evap_temperature, const s16 cond_temperature) {
    const s32 evap_corr = ((s32)evap_dH_B * evap_temperature) / 1000;
    const s32 cond_corr = ((s32)evap_dH_C * cond_temperature) / 1000;
    const s32 dH = (s32)evap_dH_A + evap_corr + cond_corr;
    if (dH < 0) return 0;
    if (dH > 65535) return 65535;
    return dH;
}

u16 condensation_dH(const s16 evap_temperature, const s16 cond_temperature) {
    const s32 evap_corr = ((s32)cond_dH_B * evap_temperature) / 1000;
    const s32 cond_corr = ((s32)cond_dH_C * cond_temperature) / 1000;
    const s32 dH = (s32)cond_dH_A + evap_corr + cond_corr;
    if (dH < 0) return 0;
    if (dH > 65535) return 65535;
    return dH;
}

u16 compressor_dH(const s16 evap_temperature, const s16 cond_temperature) {
    const s32 evap_corr = ((s32)comp_dH_B * evap_temperature) / 1000;
    const s32 cond_corr = ((s32)comp_dH_C * cond_temperature) / 1000;
    const s32 dH = evap_corr + cond_corr;
    if (dH < 0) return 0;
    if (dH > 65535) return 65535;
    return dH;
}

u16 calculate_mass_rate(const u16 compressor_volume, const u16 compressor_speed, const s16 evap_temperature) {
    s32 volume_rate = min_((s32)compressor_volume * compressor_speed / 100, 65535);

    s32 mass_rate = (s32)volume_rate * gas_density(evap_temperature) / 1000;
    if (mass_rate > 65535) return 65535;
    return mass_rate;
}

u16 calculate_evaporation_power(const u16 compressor_volume, const u16 compressor_speed, const s16 evap_temperature, const s16 cond_temperature) {
    const u16 mass_rate = calculate_mass_rate(compressor_volume, compressor_speed, evap_temperature);
    const u16 dH = evaporation_dH(evap_temperature, cond_temperature);
    const u32 power = (u32)mass_rate * dH / 100;
    if (power > 65535) return 65535;
    return power;
}

u16 calculate_condensation_power(const u16 compressor_volume, const u16 compressor_speed, const s16 evap_temperature, const s16 cond_temperature) {
    const u16 mass_rate = calculate_mass_rate(compressor_volume, compressor_speed, evap_temperature);
    const u16 dH = condensation_dH(evap_temperature, cond_temperature);
    const u32 power = (u32)mass_rate * dH / 100;
    if (power > 65535) return 65535;
    return power;
}

u16 calculate_compressor_power(const u16 compressor_volume, const u16 compressor_speed, const s16 evap_temperature, const s16 cond_temperature) {
    const u16 mass_rate = calculate_mass_rate(compressor_volume, compressor_speed, evap_temperature);
    const u16 dH = compressor_dH(evap_temperature, cond_temperature);
    const u32 power = (u32)mass_rate * dH / 100;
    if (power > 65535) return 65535;
    return power;
}

s16 calculate_discharge_target(const s16 evap_temperature, const s16 cond_temperature) {
    const s32 evap_corr = ((s32)disch_B * evap_temperature)/100;
    const s32 cond_corr = ((s32)disch_C * cond_temperature)/100;
    const s32 disch = (s32)disch_A + evap_corr + cond_corr;
    if (disch > 32767) return 32767;
    return disch;
}

#define MIN_UA_DT (5)
u16 calculate_UA(const u16 power, const s16 refrigerant_temperature, const s16 medium_temperature) {
    s32 deltaT = max_(MIN_UA_DT, abs_(refrigerant_temperature - medium_temperature));
    const s32 UA = (s32)power * 10 / deltaT;
    if (UA > 65535) return 65535;
    return UA;
}

u16 calculate_glycol_mixture_CP(const u16 glycol_percentage) {
    if (glycol_percentage > 100) return 2450;
    return 4182 - ((173 * glycol_percentage)/10);
}

u16 calculate_atmospheric_pressure(const u16 altitude) {
    if (altitude > 5000) return 500;
    return 1004 - (altitude/10);
}
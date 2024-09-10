#ifndef THERMO_H
#define THERMO_H

#include "stdtypes.h"


/**
 * @brief Convert saturation pressure to temperature.
 * 
 * @param pressure Saturation pressure [mbar].
 * @return Saturation temperature [0.1°C].
 */
s16 pressure_to_temperature(const u16 pressure);

/**
 * @brief Convert saturation temperature to pressure.
 * 
 * @param temperature Saturation temperature [0.1°C].
 * @return Saturation pressure [mbar].
 */
u16 temperature_to_pressure(const s16 temperature);

/**
 * @brief Calculates the gas density, asuming 0°C superheating.
 * 
 * @param evap_temperature Temperature [0.1°C] of the evaporating refrigerant.
 * @return Density of the refrigerant [0.1mg/cm3] at the given temperature.
 */
u16 gas_density(const s16 evap_temperature);

/**
 * @brief Calculates the enthalpy difference in the evaporator. 
 * 
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @param cond_temperature The condensation saturation temperature [0.1°C].
 * @return The enthalpy abs difference [100J/0.1g].
 */
u16 evaporation_dH(const s16 evap_temperature, const s16 cond_temperature);

/**
 * @brief Calculates the enthalpy difference in the condenser.
 * 
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @param cond_temperature The condensation saturation temperature [0.1°C].
 * @return The enthalpy abs difference [100J/0.1g].
 */
u16 condensation_dH(const s16 evap_temperature, const s16 cond_temperature);

/**
 * @brief Calculates the mass flow rate of the refrigerant.
 * 
 * @param compressor_volume The compressor volume [0.1cm3].
 * @param compressor_speed The compressor speed [0.1Hz].
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @return The mass flow rate [0.1g/s].
 */
u16 calculate_mass_rate(const u16 compressor_volume, const u16 compressor_speed, const s16 evap_temperature);

/**
 * @brief Calculates the evaporation power.
 * 
 * @param compressor_volume The compressor volume [0.1cm3].
 * @param compressor_speed The compressor speed [0.1Hz].
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @param cond_temperature The condensation saturation temperature [0.1°C].
 * @return The evaporation power [W].
 */
u16 calculate_evaporation_power(const u16 compressor_volume, const u16 compressor_speed, const s16 evap_temperature, const s16 cond_temperature);

/**
 * @brief Calculates the condensation power.
 * 
 * @param compressor_volume The compressor volume [0.1cm3].
 * @param compressor_speed The compressor speed [0.1Hz].
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @param cond_temperature The condensation saturation temperature [0.1°C].
 * @return The condensation power [W].
 */
u16 calculate_condensation_power(const u16 compressor_volume, const u16 compressor_speed, const s16 evap_temperature, const s16 cond_temperature);

/**
 * @brief Calculates the discharge target temperature.
 * 
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @param cond_temperature The condensation saturation temperature [0.1°C].
 * @return The discharge target temperature [0.1°C].
 */
s16 calculate_discharge_target(const s16 evap_temperature, const s16 cond_temperature);

/**
 * @brief Calculates the overall heat transfer coefficient (UA).
 * 
 * @param power The power of the system [W].
 * @param refrigerant_temperature The refrigerant temperature [0.1°C].
 * @param medium_temperature The medium temperature [0.1°C].
 * @return The overall heat transfer coefficient (UA) [W/°C].
 */
u16 calculate_UA(const u16 power, const s16 refrigerant_temperature, const s16 medium_temperature);

/**
 * @brief Calculates the CP value of the glycol (both ethylene and propylene) and water mixture.
 * 
 * @param glycol_percentage The percentage of glycol in the mixture [1%].
 * @return The CP value of the mixture [J/kgK].
 */
u16 calculate_glycol_mixture_CP(const u16 glycol_percentage);
#endif
#ifndef THERMO_H
#define THERMO_H

#include <stdint.h>

/**
 * @brief Convert saturation pressure to temperature.
 * 
 * @param pressure Saturation pressure [mbar].
 * @return Saturation temperature [0.1°C].
 */
int16_t pressure_to_temperature(const uint16_t pressure);

/**
 * @brief Convert saturation temperature to pressure.
 * 
 * @param temperature Saturation temperature [0.1°C].
 * @return Saturation pressure [mbar].
 */
uint16_t temperature_to_pressure(const int16_t temperature);

/**
 * @brief Calculates the gas density, asuming 0°C superheating.
 * 
 * @param evap_temperature Temperature [0.1°C] of the evaporating refrigerant.
 * @return Density of the refrigerant [0.1mg/cm3] at the given temperature.
 */
uint16_t gas_density(const int16_t evap_temperature);

/**
 * @brief Calculates the enthalpy difference in the evaporator. 
 * 
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @param cond_temperature The condensation saturation temperature [0.1°C].
 * @return The enthalpy abs difference [100J/0.1g].
 */
uint16_t evaporation_dH(const int16_t evap_temperature, const int16_t cond_temperature);

/**
 * @brief Calculates the enthalpy difference in the condenser.
 * 
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @param cond_temperature The condensation saturation temperature [0.1°C].
 * @return The enthalpy abs difference [100J/0.1g].
 */
uint16_t condensation_dH(const int16_t evap_temperature, const int16_t cond_temperature);

/**
 * @brief Calculates the enthalpy difference in the compressor.
 * 
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @param cond_temperature The condensation saturation temperature [0.1°C].
 * @return The enthalpy abs difference [100J/0.1g].
 */
uint16_t compressor_dH(const int16_t evap_temperature, const int16_t cond_temperature);

/**
 * @brief Calculates the mass flow rate of the refrigerant.
 * 
 * @param compressor_volume The compressor volume [0.1cm3].
 * @param compressor_speed The compressor speed [0.1Hz].
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @return The mass flow rate [0.1g/s].
 */
uint16_t calculate_mass_rate(const uint16_t compressor_volume, const uint16_t compressor_speed, const int16_t evap_temperature);

/**
 * @brief Calculates the evaporation power.
 * 
 * @param compressor_volume The compressor volume [0.1cm3].
 * @param compressor_speed The compressor speed [0.1Hz].
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @param cond_temperature The condensation saturation temperature [0.1°C].
 * @return The evaporation power [W].
 */
uint16_t calculate_evaporation_power(const uint16_t compressor_volume, const uint16_t compressor_speed, const int16_t evap_temperature, const int16_t cond_temperature);

/**
 * @brief Calculates the condensation power.
 * 
 * @param compressor_volume The compressor volume [0.1cm3].
 * @param compressor_speed The compressor speed [0.1Hz].
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @param cond_temperature The condensation saturation temperature [0.1°C].
 * @return The condensation power [W].
 */
uint16_t calculate_condensation_power(const uint16_t compressor_volume, const uint16_t compressor_speed, const int16_t evap_temperature, const int16_t cond_temperature);

/**
 * @brief Calculates the compressor power.
 * 
 * @param compressor_volume The compressor volume [0.1cm3].
 * @param compressor_speed The compressor speed [0.1Hz].
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @param cond_temperature The condensation saturation temperature [0.1°C].
 * @return The compressor power [W].
 */
uint16_t calculate_compressor_power(const uint16_t compressor_volume, const uint16_t compressor_speed, const int16_t evap_temperature, const int16_t cond_temperature);

/**
 * @brief Calculates the discharge target temperature.
 * 
 * @param evap_temperature The evaporation saturation temperature [0.1°C].
 * @param cond_temperature The condensation saturation temperature [0.1°C].
 * @return The discharge target temperature [0.1°C].
 */
int16_t calculate_discharge_target(const int16_t evap_temperature, const int16_t cond_temperature);

/**
 * @brief Calculates the overall heat transfer coefficient (UA).
 * 
 * @param power The power of the system [W].
 * @param refrigerant_temperature The refrigerant temperature [0.1°C].
 * @param medium_temperature The medium temperature [0.1°C].
 * @return The overall heat transfer coefficient (UA) [W/°C].
 */
uint16_t calculate_UA(const uint16_t power, const int16_t refrigerant_temperature, const int16_t medium_temperature);

/**
 * @brief Calculates the CP value of the glycol (both ethylene and propylene) and water mixture.
 * 
 * @param glycol_percentage The percentage of glycol in the mixture [1%].
 * @return The CP value of the mixture [J/kgK].
 */
uint16_t calculate_glycol_mixture_CP(const uint16_t glycol_percentage);

/**
 * @brief Calculates the atmospheric pressure at a given altitude.
 * 
 * @param altitude The altitude between [m].
 * @return The atmospheric pressure [mbar].
 */
uint16_t calculate_atmospheric_pressure(const uint16_t altitude);

#endif
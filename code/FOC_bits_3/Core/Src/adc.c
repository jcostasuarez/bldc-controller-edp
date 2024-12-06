#include "adc.h"
#include "stm32f4xx_hal.h"
#include <math.h> // Biblioteca matemática

uint32_t adc_values[9];

/** Comienza la conversión del ADC
 */
void start_adc_conversion(void) {
    HAL_ADC_Start(&hadc1); // Inicia la conversión
}

/** Comienza la conversión del ADC en modo DMA
 */
void start_adc_dma(void) {
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_values, 9);
}

/** Lee los valores de los canales del ADC
 *  Requiere la correcta configuración de los canales en CubeMX
 *  y la llamada de start_adc_conversion() previamente.
 */
void read_adc_all_channels(void) {
    for (int i = 0; i < 9; i++) {
        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
        adc_values[i] = HAL_ADC_GetValue(&hadc1);
    }

}

/** Convertir un valor del ADC a voltaje
 *  @param adc_value Valor del ADC
 *  @return Voltaje correspondiente al valor del ADC
 */
float calculate_voltage(uint32_t adc_value) {
    // Convertir el valor ADC a voltaje medido
    float v_adc = (adc_value / ADC_RESOLUTION) * ADC_VREF;

    // Calcular el voltaje real en el divisor resistivo
    float v_real = v_adc * (R1 + R2) / R2;

    return v_real; // Devuelve la tensión real en voltios
}

float get_voltage_1(void) {
    return calculate_voltage(adc_values[0]); // ADC1_IN0 (PA0)
}

float get_voltage_2(void) {
    return calculate_voltage(adc_values[1]); // ADC1_IN1 (PA1)
}

float get_voltage_3(void) {
    return calculate_voltage(adc_values[2]); // ADC1_IN2 (PA2)
}


/** Convertir un valor del ADC a corriente
 *  @param adc_value Valor del ADC
 *  @return Corriente correspondiente al valor del ADC
 */
float calculate_phase_current(uint32_t adc_value) {
    // Convertir ADC_value a voltaje
    float v_adc = (adc_value / ADC_RESOLUTION) * ADC_VREF;

    // Calcular la corriente de fase
    float current = v_adc / (AMPLIFIER_GAIN * SHUNT_RESISTANCE);

    return current; // Devuelve la corriente en amperios
}

/** Obtener la corriente de la fase 1
 *  @return Corriente de la fase 1 en amperios
 */
float get_current_phase_1(void) {
    return calculate_phase_current(adc_values[4]); // ADC1_IN10 (PC0)
}

/** Obtener la corriente de la fase 2
 *  @return Corriente de la fase 2 en amperios
 */
float get_current_phase_2(void) {
    return calculate_phase_current(adc_values[5]); // ADC1_IN11 (PC1)
}

/** Obtener la corriente de la fase 3
 *  @return Corriente de la fase 3 en amperios
 */
float get_current_phase_3(void) {
    return calculate_phase_current(adc_values[6]); // ADC1_IN12 (PC2)
}


/** Convertir un valor del ADC a resistencia
 *  @param adc_value Valor del ADC
 *  @return Resistencia correspondiente al valor del ADC
 */
float adc_to_resistance(uint32_t adc_value) {

    float voltage = (adc_value / ADC_RESOLUTION) * ADC_VREF;
    return (R_SERIE_TEMP * voltage) / (ADC_VREF - voltage);  // Fórmula del divisor de voltaje
}

float calculate_temperature_ntc(float resistance){
    return BETA/ (log(resistance / R0) + (BETA/ TEMP_ROOM_KELVIN)) - 273.15;
}

/** Obtener la temperatura del motor
 *  @return Temperatura del motor en grados Celsius
 */
float get_temp_motor(void) {
    float resistance = adc_to_resistance(adc_values[8]); // Canal ADC1_IN14 (PC4)
    return resistance_to_temperature(resistance);
}

/** Obtener la temperatura del sensor de temperatura
 *  @return Temperatura del sensor de temperatura en grados Celsius
 */
float get_temp_adc(void) {
    float resistance = adc_to_resistance(adc_values[7]); // ADC1_IN13 (PC3)
    return calculate_temperature_ntc(resistance);
}

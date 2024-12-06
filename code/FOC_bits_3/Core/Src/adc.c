#include "adc.h"
#include "stm32f4xx_hal.h"

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
float adc_to_voltage(uint32_t adc_value) {
    const float VREF = 3.3;   // Referencia de voltaje en voltios
    const uint32_t ADC_RES = 4095; // Resolución de 12 bits
    return (adc_value / (float)ADC_RES) * VREF;
}

/** Obtener el voltaje del canal 1 del ADC
 *  @return Voltaje del canal 1 en voltios
 */
float get_voltage_1(void) {
    return adc_to_voltage(adc_values[0]); // Canal ADC1_IN0 (PA0)
}

/** Obtener el voltaje del canal 2 del ADC
 *  @return Voltaje del canal 2 en voltios
 */
float get_voltage_2(void) {
    return adc_to_voltage(adc_values[1]); // Canal ADC1_IN1 (PA1)
}

/** Obtener el voltaje del canal 3 del ADC
 *  @return Voltaje del canal 3 en voltios
 */
float get_voltage_3(void) {
    return adc_to_voltage(adc_values[2]); // Canal ADC1_IN2 (PA2)
}

/** Convertir un valor del ADC a corriente
 *  @param adc_value Valor del ADC
 *  @param sensitivity Sensibilidad del sensor de corriente en V/A
 *  @return Corriente correspondiente al valor del ADC
 */
float adc_to_current(uint32_t adc_value, float sensitivity) {
    const float VREF = 3.3;   // Referencia de voltaje en voltios
    const uint32_t ADC_RES = 4095; // Resolución de 12 bits
    float voltage = (adc_value / (float)ADC_RES) * VREF;
    return voltage / sensitivity;  // Convertir a corriente
}

/** Obtener la corriente del canal 1 del ADC
 *  @return Corriente del canal 1 en amperios
 */
float get_current_1(void) {
    return adc_to_current(adc_values[4], 0.185); // Canal ADC1_IN10 (PC0)
}

/** Obtener la corriente del canal 2 del ADC
 *  @return Corriente del canal 2 en amperios
 */
float get_current_2(void) {
    return adc_to_current(adc_values[5], 0.185); // Canal ADC1_IN11 (PC1)
}

/** Obtener la corriente del canal 3 del ADC
 *  @return Corriente del canal 3 en amperios
 */
float get_current_3(void) {
    return adc_to_current(adc_values[6], 0.185); // Canal ADC1_IN12 (PC2)
}

/** Convertir un valor del ADC a resistencia
 *  @param adc_value Valor del ADC
 *  @return Resistencia correspondiente al valor del ADC
 */
float adc_to_resistance(uint32_t adc_value) {
    const float VREF = 3.3;   // Referencia de voltaje en voltios
    const uint32_t ADC_RES = 4095; // Resolución de 12 bits
    const float R_SERIE = 10000.0; // Resistencia en serie (10k ohm)

    float voltage = (adc_value / (float)ADC_RES) * VREF;
    return (R_SERIE * voltage) / (VREF - voltage);  // Fórmula del divisor de voltaje
}

/** Convertir una resistencia a temperatura
 *  @param resistance Resistencia en ohmios
 *  @return Temperatura en grados Celsius
 */
float resistance_to_temperature(float resistance) {
    const float A = 0.001129148;
    const float B = 0.000234125;
    const float C = 0.0000000876741;

    float ln_r = log(resistance);
    float temp_kelvin = 1.0 / (A + B * ln_r + C * ln_r * ln_r * ln_r);
    return temp_kelvin - 273.15; // Convertir a Celsius
}

/** Obtener la temperatura del motor
 *  @return Temperatura del motor en grados Celsius
 */
float get_temp_motor(void) {
    float resistance = adc_to_resistance(adc_values[8]); // Canal ADC1_IN14 (PC4)
    return resistance_to_temperature(resistance);
}

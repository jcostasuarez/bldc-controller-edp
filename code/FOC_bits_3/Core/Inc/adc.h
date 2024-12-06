#ifndef ADC_H
#define ADC_H

#include <stdint.h>

// Comienza la conversión del ADC
void start_adc_conversion(void);
void start_adc_dma(void);

// Funciones para obtener valores de voltaje
float get_voltage_1(void);
float get_voltage_2(void);
float get_voltage_3(void);

// Funciones para obtener valores de corriente
float get_current_1(void);
float get_current_2(void);
float get_current_3(void);

// Funciones para obtener temperaturas
float get_temp_motor(void);
float get_adc_temp(void);

#endif // ADC_H

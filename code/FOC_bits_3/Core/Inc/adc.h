#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#define ADC_RESOLUTION 4095.0       // Resolución del ADC (12 bits)
#define ADC_VREF 3.3                // Voltaje de referencia del ADC en voltios
#define SHUNT_RESISTANCE 0.0005     // Resistencia del shunt en ohmios
#define AMPLIFIER_GAIN 20.0         // Ganancia del amplificador diferencial
#define R1 39000.0                  // Resistencia en serie (39k ohm)
#define R2 2200.0                   // Resistencia en paralelo (2.2k ohm)
#define R_SERIE_TEMP 10000.0        // Resistencia en serie para el sensor de temperatura (10k ohm)
#define BETA 4100.0                 // Coeficiente beta del sensor de temperatura
#define TEMP_ROOM_KELVIN 298.15     // Temperatura ambiente en Kelvin
#define R0 10000.0                  // Resistencia a 25 grados Celsius

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

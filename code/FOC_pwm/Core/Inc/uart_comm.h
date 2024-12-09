#ifndef INC_UART_COMM_H_
#define INC_UART_COMM_H_

#include "main.h"
#include "math.h"

#define CMD_ID 					'0'
#define CMD_SET_SPEED 			'1'
#define CMD_MOTOR_ON_OFF 		'2'
#define CMD_SET_SETUP_TIME 		'3'
#define CMD_GET_SPEED 			'4'
#define CMD_GET_C1 				'5'
#define CMD_GET_C2 				'6'
#define CMD_GET_C3 				'7'
#define CMD_GET_V1 				'8'
#define CMD_GET_V2 				'9'
#define CMD_GET_V3 				'a'
#define CMD_GET_MOTOR_TEMP 		'b'
#define CMD_GET_DRIVER_TEMP 	'c'
#define CMD_GET_BAT_VOLTAGE 	'd'
#define CMD_GET_HALL 			'e'
#define CMD_SET_MODE 			'f'

#define ST_WAIT_FOR_CMD 0
#define ST_WAIT_FOR_SPEED 1
#define ST_WAIT_FOR_ON_OFF 2


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

void comm(void);
uint8_t comm_parse_command(uint8_t command);

int32_t calculate_voltage(uint32_t adc_value);
int32_t get_voltage_1(void);
int32_t get_voltage_2(void);
int32_t get_voltage_3(void);
int32_t get_voltage_supply(void);
int32_t calculate_phase_current(uint32_t adc_value);
int32_t get_current_phase_1(void);
int32_t get_current_phase_2(void);
int32_t get_current_phase_3(void);
float adc_to_resistance(uint32_t adc_value);
int32_t calculate_temperature_ntc(float resistance);
int32_t get_temp_motor(void);
int32_t get_temp_adc(void);

#endif /* INC_UART_COMM_H_ */

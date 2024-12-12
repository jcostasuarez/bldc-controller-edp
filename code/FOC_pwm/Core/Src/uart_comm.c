#include "uart_comm.h"

/**
 * FSM that handles the UART communication with the computer
 */
void comm(void) {
	static uint8_t state = ST_WAIT_FOR_CMD;
	uint8_t value = 0;

	if(uart_is_empty()) {
		return;
	}

	value = uart_read();

	if (value == 255) {
		return;
	}

	switch(state) {
		case ST_WAIT_FOR_CMD: {
			state = comm_parse_command(value);
			break;
		}
		case ST_WAIT_FOR_ON_OFF: {
			motor_on_off(value);
			state = ST_WAIT_FOR_CMD;
			uart_write(value, 1);
			break;
		}
		case ST_WAIT_FOR_SPEED: {
			//motor_set_speed(value);
			motor_set_duty(value*8);
			state = ST_WAIT_FOR_CMD;
			uart_write(value, 1);
			break;
		}
	}
}

uint8_t comm_parse_command(uint8_t command) {
	uint8_t state = ST_WAIT_FOR_CMD;
	switch(command) {
		case CMD_ID: {
			uart_write(DEVICE_ID, 4);
			break;
		}

		case CMD_SET_SPEED: {
			state = ST_WAIT_FOR_SPEED;
			break;
		}

		case CMD_MOTOR_ON_OFF: {
			state = ST_WAIT_FOR_ON_OFF;
			break;
		}

		case CMD_SET_SETUP_TIME: {
			// Ignore
			break;
		}

		case CMD_GET_SPEED: {
			uart_write(motor_get_speed_rpm(), 1);
			break;
		}

		case CMD_GET_C1: {
			uart_write(get_current_phase_1(), 4);
			break;
		}

		case CMD_GET_C2: {
			//uart_write(get_current_phase_2(), 4);
			uart_write(motor_get_duty(), 4);
			break;
		}

		case CMD_GET_C3: {
			uart_write(get_current_phase_3(), 4);
			break;
		}

		case CMD_GET_V1: {
			uart_write(get_voltage_1(), 4);
			break;
		}

		case CMD_GET_V2: {
			uart_write(get_voltage_2(), 4);
			break;
		}

		case CMD_GET_V3: {
			uart_write(get_voltage_3(), 4);
			break;
		}

		case CMD_GET_MOTOR_TEMP: {
			uart_write(get_temp_motor(), 4);
			break;
		}

		case CMD_GET_DRIVER_TEMP: {
			uart_write(get_temp_adc(), 4);
			break;
		}

		case CMD_GET_BAT_VOLTAGE: {
			uart_write(get_voltage_supply(), 4);
			break;
		}

		case CMD_GET_HALL: {
			uart_write((int32_t) motor_get_hall(), 1);
			break;
		}

		case CMD_SET_MODE: {
			// Ignore
			break;
		}

		default: {
			break;
		}
	}
	return state;
}

/** Convertir un valor del ADC a voltaje
 *  @param adc_value Valor del ADC
 *  @return Voltaje correspondiente al valor del ADC
 */
int32_t calculate_voltage(uint32_t adc_value) {
    // Convertir el valor ADC a voltaje medido
    float v_adc = (adc_value / ADC_RESOLUTION) * ADC_VREF;

    // Calcular el voltaje real en el divisor resistivo
    float v_real = v_adc * (R1 + R2) / R2;

    return (int32_t) v_real*1000; // Devuelve la tensión real en voltios
}

int32_t get_voltage_1(void) {
    return calculate_voltage(adc_buffer[ADC_V1]); // ADC1_IN0 (PA0)
}

int32_t get_voltage_2(void) {
    return calculate_voltage(adc_buffer[ADC_V2]); // ADC1_IN1 (PA1)
}

int32_t get_voltage_3(void) {
    return calculate_voltage(adc_buffer[ADC_V3]); // ADC1_IN2 (PA2)
}

int32_t get_voltage_supply(void) {
    return calculate_voltage(adc_buffer[ADC_V_SUPPLY]); // ADC1_IN2 (PA2)
}

/** Convertir un valor del ADC a corriente
 *  @param adc_value Valor del ADC
 *  @return Corriente correspondiente al valor del ADC
 */
int32_t calculate_phase_current(uint32_t adc_value) {
    // Convertir ADC_value a voltaje
    float v_adc = (adc_value / ADC_RESOLUTION) * ADC_VREF;

    // Calcular la corriente de fase
    float current = v_adc / (AMPLIFIER_GAIN * SHUNT_RESISTANCE);

    return (int32_t) current*1000; // Devuelve la corriente en amperios
}

/** Obtener la corriente de la fase 1
 *  @return Corriente de la fase 1 en amperios
 */
int32_t get_current_phase_1(void) {
    return calculate_phase_current(adc_buffer[ADC_C1]); // ADC1_IN10 (PC0)
}

/** Obtener la corriente de la fase 2
 *  @return Corriente de la fase 2 en amperios
 */
int32_t get_current_phase_2(void) {
    return calculate_phase_current(adc_buffer[ADC_C2]); // ADC1_IN11 (PC1)
}

/** Obtener la corriente de la fase 3
 *  @return Corriente de la fase 3 en amperios
 */
int32_t get_current_phase_3(void) {
    return calculate_phase_current(adc_buffer[ADC_C3]); // ADC1_IN12 (PC2)
}


/** Convertir un valor del ADC a resistencia
 *  @param adc_value Valor del ADC
 *  @return Resistencia correspondiente al valor del ADC
 */
float adc_to_resistance(uint32_t adc_value) {

    float voltage = (adc_value / ADC_RESOLUTION) * ADC_VREF;
    return (R_SERIE_TEMP * voltage) / (ADC_VREF - voltage);  // Fórmula del divisor de voltaje
}

int32_t calculate_temperature_ntc(float resistance){
    return (int32_t) 1000 * (BETA/ (log(resistance / R0) + (BETA/ TEMP_ROOM_KELVIN)) - 273.15);
}

/** Obtener la temperatura del motor
 *  @return Temperatura del motor en grados Celsius
 */
int32_t get_temp_motor(void) {
    float resistance = adc_to_resistance(adc_buffer[ADC_TEMP_MOTOR]); // Canal ADC1_IN14 (PC4)
    return (int32_t) 1000*(1.0 / ((log(resistance / 10000.0) / 3380.0) + (1.0 / 298.15)) - 273.15);
}

/** Obtener la temperatura del sensor de temperatura
 *  @return Temperatura del sensor de temperatura en grados Celsius
 */
int32_t get_temp_adc(void) {
    float resistance = adc_to_resistance(adc_buffer[ADC_TEMP]); // ADC1_IN13 (PC3)
    return calculate_temperature_ntc(resistance);
}

/*
 * motor_bldc.c
 *
 *  Created on: Nov 22, 2024
 *      Author: bruno
 */
#include "main.h"
#include "motor_bldc.h"

static uint8_t g_hall = 0;
static uint8_t g_on_off = 0;
static float g_spins = 0;

// Duty = 800 -> 100%
static int32_t g_duty = 8;
static uint32_t g_speed_filter_rpm[10];
static uint8_t g_speed_filter_pos = 0;
static uint32_t g_speed_rpm = 0;
static uint8_t g_desired_speed_rpm = 0;

// Delta duty is the amount that the duty will be increased or decreased to reach
// the desired speed. This value is divided by two each time, until the speed is
// estabilized
static int32_t g_delta_duty = 0;


/**
 * @brief: Read Hall sensors
 * @arg: orientation. The order of the HALL sensors.
 * @return: Hall sensors values as an 8 bit word with the following structure:
 * 	"0 0 0 0 0 Hx Hx Hx"
 * 	The order of the Hall sensors depends on the "orientation" arguments.
 */
uint8_t motor_read_hall(uint8_t orientation) {
    uint8_t hallState = 0;

    switch(orientation) {
    	case 0:
			hallState |= (HAL_GPIO_ReadPin(HALL1_GPIO_Port, HALL1_Pin) << 2);
			hallState |= (HAL_GPIO_ReadPin(HALL2_GPIO_Port, HALL2_Pin) << 1);
			hallState |= (HAL_GPIO_ReadPin(HALL3_GPIO_Port, HALL3_Pin) << 0);
        break;

    	case 1:
			hallState |= (HAL_GPIO_ReadPin(HALL1_GPIO_Port, HALL1_Pin) << 2);
			hallState |= (HAL_GPIO_ReadPin(HALL2_GPIO_Port, HALL2_Pin) << 0);
			hallState |= (HAL_GPIO_ReadPin(HALL3_GPIO_Port, HALL3_Pin) << 1);
		break;

    	case 2:
			hallState |= (HAL_GPIO_ReadPin(HALL1_GPIO_Port, HALL1_Pin) << 1);
			hallState |= (HAL_GPIO_ReadPin(HALL2_GPIO_Port, HALL2_Pin) << 2);
			hallState |= (HAL_GPIO_ReadPin(HALL3_GPIO_Port, HALL3_Pin) << 0);
		break;

    	case 3:
			hallState |= (HAL_GPIO_ReadPin(HALL1_GPIO_Port, HALL1_Pin) << 1);
			hallState |= (HAL_GPIO_ReadPin(HALL2_GPIO_Port, HALL2_Pin) << 0);
			hallState |= (HAL_GPIO_ReadPin(HALL3_GPIO_Port, HALL3_Pin) << 2);
		break;

    	case 4:
			hallState |= (HAL_GPIO_ReadPin(HALL1_GPIO_Port, HALL1_Pin) << 0);
			hallState |= (HAL_GPIO_ReadPin(HALL2_GPIO_Port, HALL2_Pin) << 1);
			hallState |= (HAL_GPIO_ReadPin(HALL3_GPIO_Port, HALL3_Pin) << 2);
		break;

    	case 5:
			hallState |= (HAL_GPIO_ReadPin(HALL1_GPIO_Port, HALL1_Pin) << 0);
			hallState |= (HAL_GPIO_ReadPin(HALL2_GPIO_Port, HALL2_Pin) << 2);
			hallState |= (HAL_GPIO_ReadPin(HALL3_GPIO_Port, HALL3_Pin) << 1);
		break;
    }

    g_hall = hallState;
    return hallState;
}

int32_t motor_get_duty(void) {
	return g_duty;
}

/**
 * @brief: Return state of hall sensors
 */
uint8_t motor_get_hall(void) {
	return g_hall;
}

/**
 * @brief: Turn motor on or off
 */
void motor_on_off(uint8_t on) {
	if (on == 1) {
		g_on_off = 1;
		g_duty = 8; // Default value, to make sure a smooth start_up is made
		TIM1->CCR1 = g_duty;
		TIM1->CCR2 = g_duty;
		TIM1->CCR3 = g_duty;
		drv8301_init();
	} else {
		g_on_off = 0;
	}
}

/**
 * @brief: Turn off all transistors
 */
void motor_reset(void) {
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
}

/**
 * @brief: Tries to rotate the motor with different Hall Sensor orientation,
 * 	until it finds the one which makes it spin.
 * @return: Hall sensor correct orientation.
 */
uint8_t motor_detect_hall_orientation(void) {
	uint8_t i = 0;
	uint32_t spins = 0;
	uint32_t max_spins = 0;
	uint8_t orientation = 0;

	for (i=0; i <= 5; i++) {
		//flag_timer_10seg = 0;
		HAL_TIM_Base_Start_IT(&htim3);
		//spins = motor_rotate(i, &flag_timer_10seg, 25);

		if (spins > max_spins) {
			orientation = i;
			max_spins = spins;
		}
	}
	//flag_timer_10seg = 0;
	return orientation;
}

/**
 * @brief: Set motor speed
 */
void motor_set_speed(uint8_t desired_speed_rpm) {
	g_desired_speed_rpm = desired_speed_rpm;
	if (desired_speed_rpm > g_speed_rpm) {
		g_delta_duty = 32;
	} else {
		g_delta_duty = -32;
	}
}

void motor_set_duty(uint32_t duty) {
//	if ((g_speed_rpm > g_desired_speed_rpm && g_delta_duty > 0) ||
//		(g_speed_rpm < g_desired_speed_rpm && g_delta_duty < 0)) {
//		g_delta_duty = -g_delta_duty/2;
//	}
//
//	g_duty += g_delta_duty;
//	if (g_duty < 8) {
//		g_duty = 8;
//	}
//
//	if (g_duty > 600) {
//		g_duty = 600;
//	}
	if (g_on_off) {
		g_duty = duty;
		TIM1->CCR1 = g_duty;
		TIM1->CCR2 = g_duty;
		TIM1->CCR3 = g_duty;
	}
}

void motor_init(void) {
	motor_set_duty(g_duty);

	// Start 5 seconds timer to calculate speed
	HAL_TIM_Base_Start_IT(&htim3);
}

void motor_calculate_speed(void) {
	// Each spin is equal to 60° / Poles
	float spins_per_minute = g_spins * 60;	// This function is called each 1 seconds
	float erpm = spins_per_minute / 6;	// Each spin is 60°, 6 spins is one ERPM
	g_speed_filter_rpm[g_speed_filter_pos] = (int32_t) (100*erpm / MOTOR_POLES); // Multiply by 100 instead of 1000. This value is divided by 10 because of the filter.

//	g_speed_rpm += g_speed_filter_rpm[g_speed_filter_pos];
//
//	g_speed_filter_pos++;
//	if (g_speed_filter_pos == 10) {
//		g_speed_filter_pos = 0;
//	}
//
//	g_speed_rpm -= g_speed_filter_rpm[g_speed_filter_pos];


	// Apply control
	if (g_on_off) {
		motor_set_duty(g_duty);
	}

	g_spins = 0;
}

uint8_t motor_get_speed_rpm(void) {
	return g_speed_rpm;
}



/**
 * @brief: Rotates the motor. The rotation speed is given by the voltage applied to the motor,
 * 	which is given by the supply voltage, provided that the transistors are held ON during the
 * 	whole phase (hall state).
 * @arg: orientation. Hall sensors' order.
 * @arg: exit_flag. Pointer to a value, that should be "1" to exit this function.
 * @return number of "spins", given by the amount of hall sensors' shifts.
 */
void motor_rotate(uint8_t orientation) {
	uint8_t hall_state = motor_read_hall(orientation);
	static uint8_t previous_hall_state = 0;

	// Motor is turned off
	if (!g_on_off) {
		motor_reset();
		return;
	}

	// Read hall and, in case of a transition, turn off all transistors to avoid
	// short-circuiting two transistors.
	if (previous_hall_state != hall_state && hall_state != 0 && hall_state != 7) {
		previous_hall_state = hall_state;
		g_spins++;
		motor_reset();
	}

	// Turn on the transistors according to the HALL Sensors
	switch(hall_state) {
		case HALL_STATE_A:
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
			//HAL_GPIO_WritePin(H2_GPIO_Port, H2_Pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(L3_GPIO_Port, L3_Pin, GPIO_PIN_SET);
		break;

		case HALL_STATE_B:
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
			//HAL_GPIO_WritePin(H2_GPIO_Port, H2_Pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(L1_GPIO_Port, L1_Pin, GPIO_PIN_SET);
		break;

		case HALL_STATE_C:
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
			//HAL_GPIO_WritePin(H3_GPIO_Port, H3_Pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(L1_GPIO_Port, L1_Pin, GPIO_PIN_SET);
		break;

		case HALL_STATE_D:
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
			//HAL_GPIO_WritePin(H3_GPIO_Port, H3_Pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(L2_GPIO_Port, L2_Pin, GPIO_PIN_SET);
		break;

		case HALL_STATE_E:
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
			//HAL_GPIO_WritePin(H1_GPIO_Port, H1_Pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(L2_GPIO_Port, L2_Pin, GPIO_PIN_SET);
		break;

		case HALL_STATE_F:
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
			//HAL_GPIO_WritePin(H1_GPIO_Port, H1_Pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(L3_GPIO_Port, L3_Pin, GPIO_PIN_SET);
		break;

		default:
			//motor_reset();
		break;
	}
}


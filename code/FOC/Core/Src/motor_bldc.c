/*
 * motor_bldc.c
 *
 *  Created on: Nov 22, 2024
 *      Author: bruno
 */
#include "main.h"
#include "motor_bldc.h"

extern TIM_HandleTypeDef htim1;
uint8_t last_hallState;

void UpdatePhaseFromHall(void) {
    last_hallState = GetHallState();

    switch (last_hallState) {
        case 0b101: // HALL_A=1, HALL_B=0, HALL_C=1
            UpdatePhase(1); // Paso 1
            break;
        case 0b001: // HALL_A=0, HALL_B=0, HALL_C=1
            UpdatePhase(2); // Paso 2
            break;
        case 0b011: // HALL_A=0, HALL_B=1, HALL_C=1
            UpdatePhase(3); // Paso 3
            break;
        case 0b010: // HALL_A=0, HALL_B=1, HALL_C=0
            UpdatePhase(4); // Paso 4
            break;
        case 0b110: // HALL_A=1, HALL_B=1, HALL_C=0
            UpdatePhase(5); // Paso 5
            break;
        case 0b100: // HALL_A=1, HALL_B=0, HALL_C=0
            UpdatePhase(6); // Paso 6
            break;
        default:
            // Error: Estado inválido
            break;
    }
}

uint8_t GetHallState(void) {
    uint8_t hallState = 0;

    // Leer los estados de los pines Hall
    hallState |= (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) << 2); // Bit 2
    hallState |= (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) << 1); // Bit 1
    hallState |= HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8);      // Bit 0

    return hallState; // Retorna un valor entre 0 y 7
}


void UpdatePhase(uint8_t step) {
    switch (step) {
        case 1: // A+ B- C OFF
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//A_H
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);//B_L
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);//C_OFF
            HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);//C_OFF
            break;
        case 2: // A OFF B- C+
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//A_OFF
            HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);//A_OFF
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);//B_L
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);//C_H
            break;
        case 3: // A- B OFF C+
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);//A_L
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);//B_OFF
			HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);//B_OFF
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);//C_H
			break;
        case 4: // A- B+ C OFF
        	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);//A_L
        	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);//B_H
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);//C_OFF
            HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);//C_OFF
			break;
        case 5: // A OFF B+ C-
        	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);//A_OFF
        	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);//B_H
        	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);//C_L
			break;
        case 6: // A+ B OFF C-
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//A_H
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);//B_OFF
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);//C_L
            break;
    }
}


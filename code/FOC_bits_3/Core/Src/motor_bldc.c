/*
 * motor_bldc.c
 *
 *  Created on: Nov 22, 2024
 *      Author: bruno
 */
#include "main.h"
#include "motor_bldc.h"

#define A_H (GPIOA, GPIO_PIN_8)
#define B_H (GPIOA, GPIO_PIN_9)
#define C_H (GPIOA, GPIO_PIN_10)
#define A_L (GPIOB, GPIO_PIN_15)
#define B_L (GPIOB, GPIO_PIN_14)
#define C_L (GPIOB, GPIO_PIN_13)



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

    return hallState; // Retorna un valor entre 1 y 6
}


void UpdatePhase(uint8_t hallState) {
    // Apaga todas las fases antes de configurar el nuevo estado
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); // A+
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); // A-
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  GPIO_PIN_RESET); // B+
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // B-
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  GPIO_PIN_RESET); // C+
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET); // C-

    // Configura las fases activas según el estado de los sensores Hall
    switch (hallState) {
        case 0b101: // Paso 1: A+ y B-
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); // A+
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // B-
            break;

        case 0b100: // Paso 2: A+ y C-
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); // A+
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); // C-
            break;

        case 0b110: // Paso 3: B+ y C-
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // B+
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); // C-
            break;

        case 0b010: // Paso 4: B+ y A-
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // B+
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); // A-
            break;

        case 0b011: // Paso 5: C+ y A-
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // C+
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); // A-
            break;

        case 0b001: // Paso 6: C+ y B-
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // C+
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // B-
            break;

        default: // Estado no válido
            // Todas las fases apagadas
            break;
    }
}


void Rotate(uint8_t t) {//time in miliseconds
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // A+
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); //B-
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

	HAL_Delay(t);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // A+
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); // C-

	HAL_Delay(t);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  GPIO_PIN_SET); // B+
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); // C-

	HAL_Delay(t);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); // A-
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  GPIO_PIN_SET); // B+
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

	HAL_Delay(t);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); // A-
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,  GPIO_PIN_SET); // C+
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

	HAL_Delay(t);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // B-
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,  GPIO_PIN_SET); // C+
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

	HAL_Delay(t);
}


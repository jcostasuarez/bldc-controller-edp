#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "stm32f4xx_hal.h"

/** 
 * @brief Inicialización del motor
 * @param None
 * @retval None
 */
void Motor_Init(void);

/** 
 * @brief Control de velocidad del motor
 * @param speed_setpoint Velocidad deseada en RPM
 * @retval None
 */
void Motor_ControlSpeed(uint32_t speed_setpoint);

/** 
 * @brief Función para obtener la velocidad del motor a partir de sensores Hall
 * @param None
 * @retval Velocidad en RPM
 */
uint32_t GetSpeedFromHall(void);

/** 
 * @brief Función para leer la corriente del motor
 * @param None
 * @retval Corriente en amperios
 */
uint32_t ReadMotorCurrent(void);

#endif
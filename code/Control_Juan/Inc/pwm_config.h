#ifndef PWM_CONFIG_H
#define PWM_CONFIG_H

#include "stm32f4xx_hal.h"

/** 
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
void MX_TIM1_Init(void);

/** 
 * @brief Configura el duty cycle de un canal de PWM
 * @param channel Canal de PWM (TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3)
 * @param duty_cycle Valor del duty cycle (0 a 4199)
 * @retval None
 */

void PWM_SetDutyCycle(uint32_t channel, uint16_t duty_cycle);

#endif

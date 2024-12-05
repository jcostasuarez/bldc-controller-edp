#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include "stm32f4xx_hal.h"

/** 
 * @brief System Clock Configuration
 * @param None
 * @retval None
 */
void SystemClock_Config(void);

/** 
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void MX_GPIO_Init(void);

#endif

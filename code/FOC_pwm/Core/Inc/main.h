/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/**
 *	8 = TEMP_MOTOR
 *	7 = V_SUPPLY
 *	6 = CURRENT_3
 *	5 = CURRENT_2
 *	4 = CURRENT_1
 *	3 = ADC_TEMP
 *	2 = VOLTAGE_3
 *	1 = VOLTAGE_2
 *	0 = VOLTAGE_1
 */
#define ADC_CHANNELS 9

#define ADC_V1 0
#define ADC_V2 1
#define ADC_V3 2
#define ADC_TEMP 3
#define ADC_C1 4
#define ADC_C2 5
#define ADC_C3 6
#define ADC_V_SUPPLY 7
#define ADC_TEMP_MOTOR 8

extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim1;
extern uint32_t adc_buffer[ADC_CHANNELS];
#include "drv8301.h"
#include "motor_bldc.h"
#include "uart_bitbang.h"
#include "uart_comm.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CURRENT_1_Pin GPIO_PIN_0
#define CURRENT_1_GPIO_Port GPIOC
#define CURRENT_2_Pin GPIO_PIN_1
#define CURRENT_2_GPIO_Port GPIOC
#define CURRENT_3_Pin GPIO_PIN_2
#define CURRENT_3_GPIO_Port GPIOC
#define VOLTAGE_SUPPLY_Pin GPIO_PIN_3
#define VOLTAGE_SUPPLY_GPIO_Port GPIOC
#define VOLTAGE_1_Pin GPIO_PIN_0
#define VOLTAGE_1_GPIO_Port GPIOA
#define VOLTAGE_2_Pin GPIO_PIN_1
#define VOLTAGE_2_GPIO_Port GPIOA
#define VOLTAGE_3_Pin GPIO_PIN_2
#define VOLTAGE_3_GPIO_Port GPIOA
#define ADC_TEMP_Pin GPIO_PIN_3
#define ADC_TEMP_GPIO_Port GPIOA
#define TEMP_MOTOR_Pin GPIO_PIN_4
#define TEMP_MOTOR_GPIO_Port GPIOC
#define LED_G_Pin GPIO_PIN_0
#define LED_G_GPIO_Port GPIOB
#define LED_R_Pin GPIO_PIN_1
#define LED_R_GPIO_Port GPIOB
#define HALL1_Pin GPIO_PIN_6
#define HALL1_GPIO_Port GPIOC
#define HALL2_Pin GPIO_PIN_7
#define HALL2_GPIO_Port GPIOC
#define HALL3_Pin GPIO_PIN_8
#define HALL3_GPIO_Port GPIOC
#define SPI3_CS_Pin GPIO_PIN_9
#define SPI3_CS_GPIO_Port GPIOC
#define SPI3_SCK_Pin GPIO_PIN_10
#define SPI3_SCK_GPIO_Port GPIOC
#define SPI3_MISO_Pin GPIO_PIN_11
#define SPI3_MISO_GPIO_Port GPIOC
#define SPI3_MOSI_Pin GPIO_PIN_12
#define SPI3_MOSI_GPIO_Port GPIOC
#define UART_RX_Pin GPIO_PIN_2
#define UART_RX_GPIO_Port GPIOD
#define UART_RX_EXTI_IRQn EXTI2_IRQn
#define UART_TX_Pin GPIO_PIN_3
#define UART_TX_GPIO_Port GPIOB
#define EN_GATE_Pin GPIO_PIN_5
#define EN_GATE_GPIO_Port GPIOB
#define nFAULT_Pin GPIO_PIN_7
#define nFAULT_GPIO_Port GPIOB
#define nFAULT_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

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
extern SPI_HandleTypeDef hspi3;
extern TIM_HandleTypeDef htim3;
extern uint8_t flag_timer_10seg;
#include "drv8301.h"
#include "motor_bldc.h"
#include "debug.h"
#include "led.h"


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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_G_Pin GPIO_PIN_0
#define LED_G_GPIO_Port GPIOB
#define LED_R_Pin GPIO_PIN_1
#define LED_R_GPIO_Port GPIOB
#define L1_Pin GPIO_PIN_13
#define L1_GPIO_Port GPIOB
#define L2_Pin GPIO_PIN_14
#define L2_GPIO_Port GPIOB
#define L3_Pin GPIO_PIN_15
#define L3_GPIO_Port GPIOB
#define HALL1_Pin GPIO_PIN_6
#define HALL1_GPIO_Port GPIOC
#define HALL2_Pin GPIO_PIN_7
#define HALL2_GPIO_Port GPIOC
#define HALL3_Pin GPIO_PIN_8
#define HALL3_GPIO_Port GPIOC
#define SPI3_CS_Pin GPIO_PIN_9
#define SPI3_CS_GPIO_Port GPIOC
#define H1_Pin GPIO_PIN_8
#define H1_GPIO_Port GPIOA
#define H2_Pin GPIO_PIN_9
#define H2_GPIO_Port GPIOA
#define H3_Pin GPIO_PIN_10
#define H3_GPIO_Port GPIOA
#define EN_GATE_Pin GPIO_PIN_5
#define EN_GATE_GPIO_Port GPIOB
#define nFAULT_Pin GPIO_PIN_7
#define nFAULT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

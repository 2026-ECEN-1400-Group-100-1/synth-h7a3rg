/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac2;
extern TIM_HandleTypeDef htim1;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY12_IN_Pin GPIO_PIN_3
#define KEY12_IN_GPIO_Port GPIOA
#define KEY2_IN_Pin GPIO_PIN_4
#define KEY2_IN_GPIO_Port GPIOA
#define KEY4_IN_Pin GPIO_PIN_5
#define KEY4_IN_GPIO_Port GPIOA
#define KEY6_IN_Pin GPIO_PIN_2
#define KEY6_IN_GPIO_Port GPIOB
#define KEY8_IN_Pin GPIO_PIN_10
#define KEY8_IN_GPIO_Port GPIOB
#define KEY10_IN_Pin GPIO_PIN_12
#define KEY10_IN_GPIO_Port GPIOB
#define KEY11_IN_Pin GPIO_PIN_14
#define KEY11_IN_GPIO_Port GPIOB
#define KEY9_IN_Pin GPIO_PIN_15
#define KEY9_IN_GPIO_Port GPIOB
#define KEY7_IN_Pin GPIO_PIN_6
#define KEY7_IN_GPIO_Port GPIOC
#define KEY5_IN_Pin GPIO_PIN_7
#define KEY5_IN_GPIO_Port GPIOC
#define KEY3_IN_Pin GPIO_PIN_9
#define KEY3_IN_GPIO_Port GPIOC
#define KEY1_IN_Pin GPIO_PIN_8
#define KEY1_IN_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

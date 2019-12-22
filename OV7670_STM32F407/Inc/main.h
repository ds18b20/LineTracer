/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#define PC14_OSC32_IN_Pin GPIO_PIN_14
#define PC14_OSC32_IN_GPIO_Port GPIOC
#define PC15_OSC32_OUT_Pin GPIO_PIN_15
#define PC15_OSC32_OUT_GPIO_Port GPIOC
#define PH0_OSC_IN_Pin GPIO_PIN_0
#define PH0_OSC_IN_GPIO_Port GPIOH
#define PH1_OSC_OUT_Pin GPIO_PIN_1
#define PH1_OSC_OUT_GPIO_Port GPIOH
#define FIFO_WEN_Pin GPIO_PIN_2
#define FIFO_WEN_GPIO_Port GPIOC
#define FIFO_RRST_Pin GPIO_PIN_3
#define FIFO_RRST_GPIO_Port GPIOC
#define B1_Pin GPIO_PIN_0
#define B1_GPIO_Port GPIOA
#define B1_EXTI_IRQn EXTI0_IRQn
#define FIFO_OE_Pin GPIO_PIN_4
#define FIFO_OE_GPIO_Port GPIOC
#define FIFO_RCK_Pin GPIO_PIN_5
#define FIFO_RCK_GPIO_Port GPIOC
#define BIT2_Pin GPIO_PIN_10
#define BIT2_GPIO_Port GPIOB
#define BIT3_Pin GPIO_PIN_11
#define BIT3_GPIO_Port GPIOB
#define BIT4_Pin GPIO_PIN_12
#define BIT4_GPIO_Port GPIOB
#define BIT5_Pin GPIO_PIN_13
#define BIT5_GPIO_Port GPIOB
#define BIT6_Pin GPIO_PIN_14
#define BIT6_GPIO_Port GPIOB
#define BIT7_Pin GPIO_PIN_15
#define BIT7_GPIO_Port GPIOB
#define LD4_Pin GPIO_PIN_12
#define LD4_GPIO_Port GPIOD
#define LD3_Pin GPIO_PIN_13
#define LD3_GPIO_Port GPIOD
#define LD5_Pin GPIO_PIN_14
#define LD5_GPIO_Port GPIOD
#define LD6_Pin GPIO_PIN_15
#define LD6_GPIO_Port GPIOD
#define OLED_CS1_Pin GPIO_PIN_8
#define OLED_CS1_GPIO_Port GPIOC
#define OLED_CS0_Pin GPIO_PIN_9
#define OLED_CS0_GPIO_Port GPIOC
#define OLED_SDO_Pin GPIO_PIN_10
#define OLED_SDO_GPIO_Port GPIOC
#define OLED_SDI_Pin GPIO_PIN_11
#define OLED_SDI_GPIO_Port GPIOC
#define OLED_CLK_Pin GPIO_PIN_12
#define OLED_CLK_GPIO_Port GPIOC
#define SCCB_SCL_Pin GPIO_PIN_5
#define SCCB_SCL_GPIO_Port GPIOB
#define SCCB_SDA_Pin GPIO_PIN_6
#define SCCB_SDA_GPIO_Port GPIOB
#define VSYNC_EXTI_Pin GPIO_PIN_7
#define VSYNC_EXTI_GPIO_Port GPIOB
#define VSYNC_EXTI_EXTI_IRQn EXTI9_5_IRQn
#define BIT0_Pin GPIO_PIN_8
#define BIT0_GPIO_Port GPIOB
#define BIT1_Pin GPIO_PIN_9
#define BIT1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

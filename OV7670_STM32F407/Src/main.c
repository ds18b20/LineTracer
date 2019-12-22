/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sensor.h"
#include "delay.h"
#include "print.h"
#include "oled.h"
#include "ips.h"
#include "pid.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#ifdef QVGA
#define H  240
#define W  320
#elif defined QQVGA
#define H  120
#define W  160
#elif defined CIF
#define H  288
#define W  352
#elif defined QCIF
#define H  144
#define W  176
#endif

//#define SERIAL_PRINT
#define OLED_PRINT

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned char Vsync;
unsigned char send_flag = 0;

//extern
unsigned char image_ori[IMAGE_SIZE][IMAGE_SIZE] = {0};
unsigned char image_bin[IMAGE_SIZE][IMAGE_SIZE] = {0};
unsigned char line_flag_list[IMAGE_SIZE];
unsigned char line_pos_list[IMAGE_SIZE];
unsigned char graph_value[8] = {0};

char str[] = "0x0000";
unsigned char hex_display[] = "00, ";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void OV7670_SendFrameFullSize(void)
{
  unsigned int a, b, c_data;

  for (a = 0; a < H; a++)
  { // rows
    for (b = 0; b < W; b++)
    { // columns
      FIFO_RCK_L();
      asm("NOP");
      asm("NOP");
      c_data = GPIOB->IDR & 0xff00;  // read high-8 bits
      FIFO_RCK_H();
      asm("NOP");
      asm("NOP");
      FIFO_RCK_L();
      asm("NOP");
      asm("NOP");
      c_data |= (GPIOB->IDR >> 8) & 0x00ff;  // read low-8 bits
      FIFO_RCK_H();
      // LCD_WriteRAM(c_data);  // write RGB565 data to TFT GRAM
      sprintf(str, "%x,", c_data);
      HAL_UART_Transmit(&huart2, (uint8_t *) str, strlen(str), 10);
    }
    HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
  }
  print_str("\n", 0);
}

void OV7670_SendFrameSliced_QQVGA_28_28()
{
  unsigned int a, b, c_data;

  for (a = 0; a < H; a++)
  {  // rows
    for (b = 0; b < W; b++)
    {  // columns
      FIFO_RCK_L();
      asm("NOP");
      asm("NOP");
      c_data = GPIOB->IDR & 0xff00;        // read high-8 bits
      FIFO_RCK_H();
      asm("NOP");
      asm("NOP");
      FIFO_RCK_L();
      asm("NOP");
      asm("NOP");
      c_data |= (GPIOB->IDR >> 8) & 0x00ff;  // read low-8 bits
      FIFO_RCK_H();
//      LCD_WriteRAM(c_data);         // write RGB565 data to TFT GRAM
//      if((a<output_h)&&(b<output_w))
      if ((a >= 4) && (a <= 112) && ((a - 4) % 4 == 0) && (b >= 34)
          && (b <= 142) && ((b - 34) % 4 == 0))
      {  // select 28*28 pixels
        sprintf(str, "%x,", c_data);
        HAL_UART_Transmit(&huart2, (uint8_t *) str, strlen(str), 10);
      }
    }
    HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
  }
  print_str("\n", 0);
}

void OV7670_StoreFrameSliced_QQVGA_28_28(uint8_t binarize)
{
  unsigned int a, b, c_data;
  unsigned int i = 0, j = 0;

  for (a = 0; a < H; a++)
  {  // rows
    for (b = 0; b < W; b++)
    {  // columns
      FIFO_RCK_L();
      asm("NOP");
      asm("NOP");
      c_data = GPIOB->IDR & 0xff00;        // read high-8 bits
      FIFO_RCK_H();
      asm("NOP");
      asm("NOP");
      FIFO_RCK_L();
      asm("NOP");
      asm("NOP");
      FIFO_RCK_H();
      if ((a >= 4) && (a <= 112) && ((a - 4) % 4 == 0) && (b >= 34)
          && (b <= 142) && ((b - 34) % 4 == 0))
      {  // select 28*28 pixels
        c_data = (c_data >> 8) & 0x00ff;

        if (binarize)
        {
          if (c_data > THRESHOLD)
          {
            image_bin[i][j] = 1;
          }
          else
          {
            image_bin[i][j] = 0;
          }
          image_ori[i][j] = 0;  // Not used
        }
        image_ori[i][j] = c_data;
        j++;
        if (j == IMAGE_SIZE)
        {
          j = 0;
          i++;
        }
      }
    }
    HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
  }
}

// calc_offset[-1, 1]
float calc_offset(unsigned char *line_flag_list, unsigned char *line_pos_list)
{
  unsigned int i;
  int width = IMAGE_SIZE / 2;
  float tmp;
  for (i = 0; i < IMAGE_SIZE; i++)
  {  // row
    if (line_flag_list[i])
    {
      width = IMAGE_SIZE / 2 - line_pos_list[i];
      break;
    }
  }
  tmp = width / (IMAGE_SIZE / 2.0);

  return tmp;
}

// calc_direction[-1, 1]
float calc_direction(unsigned char *line_flag_list,
    unsigned char *line_pos_list)
{
  unsigned int i;
  int height = IMAGE_SIZE / 2, width = IMAGE_SIZE / 2;
  float tmp, angle;
  for (i = 0; i < IMAGE_SIZE; i++)
  {  // row
    if (line_flag_list[i])
    {
      height = IMAGE_SIZE - i;
      width = IMAGE_SIZE / 2 - line_pos_list[i];
      break;
    }
  }
  tmp = width / (height * 1.0);
  /*
   * arctan x = x - x^3/3 + x^5/5 -鈥︹��(x鈮�1)
   * */
  angle = tmp - tmp * tmp * tmp / 3 + tmp * tmp * tmp * tmp * tmp / 5; // Tylor of arctan()

  return 2.0 * angle / 3.141592653;
}

void control_motor(int base_pwm, int diff_pwm)
{
  int left  = 0;
  int right = 0;

  left  = base_pwm - diff_pwm;
#ifdef SERIAL_PRINT
  print_str("left:", 1);
  print_int(left, 1);
#endif
#ifdef OLED_PRINT
  OLED_DispStr(0, 2, (uint8_t*) "left pwm:", FONT_8X6);
  sprintf(str, "%d", left);
  OLED_DispStr(0, 3, (uint8_t*) str, FONT_8X6);
#endif
  if(left >= 0)
  {
    if(left > 512)
    {left = 512;}
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, left);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
  }
  else
  {
    if(left < -512)
    {left = -512;}
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, -left);
  }

  right = base_pwm + diff_pwm;
#ifdef SERIAL_PRINT
  print_str("right:", 1);
  print_int(right, 1);
#endif
#ifdef OLED_PRINT
  OLED_DispStr(0, 4, (uint8_t*) "right pwm:", FONT_8X6);
  sprintf(str, "%d", right);
  OLED_DispStr(0, 5, (uint8_t*) str, FONT_8X6);
#endif

  if(right >= 0)
  {
    if(right > 512)
    {right = 512;}
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, right);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
  }
  else
  {
    if(right < -512)
    {right = -512;}
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, -right);
  }
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//  unsigned int time_out = 0;
#ifdef SERIAL_PRINT
  unsigned char bmp[IMAGE_SIZE] = {0x01};
#endif
  float offset=0, direction=0.0;
  float control_value = 0.0;
  int base_pwm = 0;
  int diff_pwm = 0;

//  unsigned int pwm_value=0;
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  OLED_Init();

  print_str("ov7670 init...", 1);

  FIFO_OE_L();
  FIFO_WEN_H();

  send_flag = 0;

  while (1 != Sensor_init())
  {
  }  // Init CMOS Sensor
  print_str("ov7670 init OK!", 1);

  Vsync = 0;
  ////////////////////////////////////////
  FIFO_RRST_L();
  FIFO_RCK_L();
  FIFO_RCK_H();
  FIFO_RCK_L();
  FIFO_RRST_H();
  FIFO_RCK_H();
  Delay(50);
  ////////////////////////////////////////
  PID_init();
  print_str("init PID", 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    OLED_Clear();

    if (send_flag == 1)
    {
#ifdef DEBUG
      OV7670_SendOneFrame_ColorBar();
#else
//      Vsync = 0;
//      while (2 != Vsync)
//      {
//        HAL_Delay(100);
//        time_out++;
//        if (time_out > 10)
//        {
//          print_str("Vsync time out", 1);
//          return 0;
//        }
//      }
//      time_out = 0;
      if (Vsync == 2)
      {
        FIFO_RRST_L();
        FIFO_RCK_L();
        FIFO_RCK_H();
        FIFO_RCK_L();
        FIFO_RRST_H();
        FIFO_RCK_H();

//        OV7670_SendFrameFullSize();
//        OV7670_SendFrameSliced_QQVGA_28_28();
        OV7670_StoreFrameSliced_QQVGA_28_28(1);
        Vsync = 0;
        /**/
//        binary_simple(image_ori, image_bin);
        show_image_bin(image_bin);
        find_center_line(image_bin, line_flag_list, line_pos_list);

        print_str("print line flag:", 1);
        print_list(line_flag_list);

        print_str("print line pos:", 1);
        print_list(line_pos_list);

        offset = calc_offset(line_flag_list, line_pos_list);
        direction = calc_direction(line_flag_list, line_pos_list);

#ifdef SERIAL_PRINT
        // show map
        generate_dis_data(line_pos_list, bmp);
        OLED_DrawBMP(0, 1, 28, 2, bmp);
        // print row offset@1st & direction
        print_str("row offset: ", 0);
        print_float(offset, 1);
        print_str("direction: ", 0);
        print_float(direction, 1);
#endif
      /**/
      }
#endif
//      send_flag = 0;
    }
    if(direction > 3)
    {direction = 3;}
    if(direction < -3)
    {direction = -3;}

#ifdef OLED_PRINT
    OLED_DispStr(0, 0, (uint8_t*) "direction:", FONT_8X6);
    sprintf(str, "%f", direction);
    OLED_DispStr(0, 1, (uint8_t*) str, FONT_8X6);
#endif


//    pid.CurrentValue = direction;
////    control_value = PID_realize(0.0);
//    control_value = P_realize(0.0);
//    print_str("control_value:", 1);
//    print_float(control_value, 1);

    base_pwm = 256;
//    diff_pwm = (int)(control_value * 512);
    diff_pwm = (int)(300 * direction);

//    print_str("base_pwm:", 1);
//    print_int(base_pwm, 1);
//    print_str("diff_pwm:", 1);
//    print_int(diff_pwm, 1);
//
    control_motor(base_pwm, diff_pwm);

//    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
//    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);

    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    HAL_Delay(10);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
  /*VSYNC EXTI line: 7*/
  case VSYNC_EXTI_Pin:
    if (Vsync == 0)
    {
      Vsync = 1;
      FIFO_WEN_H();
    }
    else if (Vsync == 1)
    {
      FIFO_WEN_L();
      Vsync = 2;
      HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    }
    break;

    /*KEY1 EXTI line: 0*/
  case B1_Pin:
//		HAL_Delay(10);  // Error
    HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0)
    {  // if KEY1 is still PRESSED DOWN
      send_flag = 1;
    }
    break;

  default:
    break;
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

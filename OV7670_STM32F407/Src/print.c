/*
 * print.c
 *
 *  Created on: 2019/12/16
 *      Author: 0000400921
 */
#include "print.h"
#include <stdio.h>
#include <string.h>

void print_str(const char* str2print, unsigned char new_line)
{
  HAL_UART_Transmit(&huart2, (uint8_t *) str2print, strlen(str2print), 10);
  if (new_line)
  {
    HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", strlen("\r\n"), 10);
  }
}

void print_int(int int2print, unsigned char new_line)
{
  unsigned char display[] = "00000000";
  sprintf((char*) display, "%d", int2print);
  HAL_UART_Transmit(&huart2, (uint8_t *) display, strlen((const char*) display),
                    10);
  if (new_line)
  {
    HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", strlen("\r\n"), 10);
  }
}

void print_int_hex(int int2print, unsigned char new_line)
{
  unsigned char display[] = "00000000";
  sprintf((char*) display, "0x%2x", int2print);
  HAL_UART_Transmit(&huart2, (uint8_t *) display, strlen((const char*) display),
                    10);
  if (new_line)
  {
    HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", strlen("\r\n"), 10);
  }
}

void print_float(float float2print, unsigned char new_line)
{
  unsigned char display[] = "00000000";
  sprintf((char*) display, "%f", float2print);
  HAL_UART_Transmit(&huart2, (uint8_t *) display, strlen((const char*) display),
                    10);
  if (new_line)
  {
    HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", strlen("\r\n"), 10);
  }
}

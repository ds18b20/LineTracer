/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "print.h"
#include "color_bar_test.h"
#include <stdio.h>
#include <string.h>

/* https://afterthoughtsoftware.com/posts/convert-rgb888-to-rgb565 */
unsigned int RGB888toRGB565(unsigned char red, unsigned char green,
                            unsigned char blue)
{
  uint16_t b = (blue >> 3) & 0x1f;
  uint16_t g = ((green >> 2) & 0x3f) << 5;
  uint16_t r = ((red >> 3) & 0x1f) << 11;

  return (uint16_t) (r | g | b);
}

/* color bar test*/
void OV7670_SendOneFrame_ColorBar(void)
{
  unsigned int i, j;
  unsigned int color = 0;
  char str[] = "0x0000";

  for (i = 0; i < 240; i++)  // row
  {
    for (j = 0; j < 320; j++)  // column
    {
      if (i < 30)
      {
        color = RGB888toRGB565(192, 192, 0);
      }
      else if (i < 60)
      {
        color = RGB888toRGB565(0, 192, 192);
      }
      else if (i < 90)
      {
        color = RGB888toRGB565(0, 192, 0);
      }
      else if (i < 120)
      {
        color = RGB888toRGB565(192, 0, 192);
      }
      else if (i < 150)
      {
        color = RGB888toRGB565(192, 0, 0);
      }
      else if (i < 180)
      {
        color = RGB888toRGB565(0, 0, 192);
      }
      else if (i < 210)
      {
        color = RGB888toRGB565(192, 192, 192);
      }
      else
      {
        color = RGB888toRGB565(0, 0, 0);
      }
      sprintf(str, "0x%x,", color);
      HAL_UART_Transmit(&huart2, (uint8_t *) str, strlen(str), 10);
    }
    HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
  }
  print_str("\n", 0);
}

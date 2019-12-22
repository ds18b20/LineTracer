#ifndef __OLED_H
#define __OLED_H		

#include "main.h"
#include "delay.h"

//------OLED PORTs------
#define FONT_8X6 1
#define FONT_16X8 2

//OLED_board  Dev_board
//VCC         3.3V
//GND         GND
//CLK         PD11  15
//SDI         PD10
//SDO         PD9
//CS0         PD8
//CS1         PD7   11

// write
/* CS1 */
#define GB_CS_Clr()     HAL_GPIO_WritePin(OLED_CS1_GPIO_Port, OLED_CS1_Pin, GPIO_PIN_RESET)  // CS1-Chinese character chip select
#define GB_CS_Set()     HAL_GPIO_WritePin(OLED_CS1_GPIO_Port, OLED_CS1_Pin, GPIO_PIN_SET)    // CS1
/* CS0 */
#define OLED_CS_Clr()   HAL_GPIO_WritePin(OLED_CS0_GPIO_Port, OLED_CS0_Pin, GPIO_PIN_RESET)  // CS0
#define OLED_CS_Set()   HAL_GPIO_WritePin(OLED_CS0_GPIO_Port, OLED_CS0_Pin, GPIO_PIN_SET)    // CS0
/* SDO */
#define OLED_DC_Clr()   HAL_GPIO_WritePin(OLED_SDO_GPIO_Port, OLED_SDO_Pin, GPIO_PIN_RESET)  // SDO
#define OLED_DC_Set()   HAL_GPIO_WritePin(OLED_SDO_GPIO_Port, OLED_SDO_Pin, GPIO_PIN_SET)    // SDO
/* SDI */
#define OLED_SDIN_Clr() HAL_GPIO_WritePin(OLED_SDI_GPIO_Port, OLED_SDI_Pin, GPIO_PIN_RESET)  // SDI
#define OLED_SDIN_Set() HAL_GPIO_WritePin(OLED_SDI_GPIO_Port, OLED_SDI_Pin, GPIO_PIN_SET)    // SDI
/* CLK */
#define OLED_SCLK_Clr() HAL_GPIO_WritePin(OLED_CLK_GPIO_Port, OLED_CLK_Pin, GPIO_PIN_RESET)  // CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(OLED_CLK_GPIO_Port, OLED_CLK_Pin, GPIO_PIN_SET)    // CLK

// read
/* SDO */
#define GB_SDO HAL_GPIO_ReadPin(OLED_SDO_GPIO_Port, OLED_SDO_Pin)  // SDO-Chinese character chip DATA output

#define OLED_CMD  0  // write command
#define OLED_DATA 1  // write data

void
OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void
OLED_Display_On(void);
void
OLED_Display_Off(void);

void
OLED_Init(void);
void
OLED_Clear(void);

void
OLED_DispStr(uint8_t x, uint8_t y, uint8_t ch[], uint8_t TextSize);
void
OLED_DispChinese(uint8_t x, uint8_t y, const uint8_t code[]);
void
DispStr_GB2312(uint8_t x, uint8_t y, uint8_t *text);
void
OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
             unsigned char y1, unsigned char BMP[]);

#endif  

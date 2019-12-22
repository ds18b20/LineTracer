/*
 * Initialize sensor by SCCB interface
 * reference: guanfu_wang
 * */
#include "usart.h"
#include "delay.h"
#include "sensor.h"
#include "print.h"
#include <stdio.h>
#include <string.h>

extern const char OV7670_reg[OV7670_REG_NUM][2];
/*
 *Function: supply CLOCK for sensor if no crystal oscillator installed
 *
 * */
/*
 void XCLK_init_ON(void)
 {
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 RCC_MCOConfig(RCC_MCO_HSE  );  // HSI
 }
 */

/*Function: write OV7670 register through SCCB interface
 * if success: return 1
 * if error: return 0
 * */
unsigned char Ov7670_WriteReg(unsigned char regID, unsigned char regDat)
{
  startSCCB();  // send SCCB start sequence, ready to write command
  if (0 == SCCBwriteByte(0x42)) // write ov7670 device ID(last bit controls W=0/R=1)
  {
    stopSCCB();  // send SCCB stop sequence
    return (0);  // if error: return 0
  }
  Delay(20);
  if (0 == SCCBwriteByte(regID))  // write register ID
  {
    stopSCCB();  // send SCCB stop sequence
    return (0);  // if error: return 0
  }
  Delay(20);
  if (0 == SCCBwriteByte(regDat))  // write data into register
  {
    stopSCCB();  // send SCCB stop sequence
    return (0);  // if error: return 0
  }
  stopSCCB();  // send SCCB stop sequence

  return (1);  // if success: return 1
}

/*Function: read OV7670 register through SCCB interface
 * if success: return 1
 * if error: return 0
 * */
unsigned char Ov7670_ReadReg(unsigned char regID, unsigned char *regDat)
{
  /* send register ID */
  startSCCB();  // send SCCB start sequence, ready to write command
  if (0 == SCCBwriteByte(0x42)) // write ov7670 device ID(last bit controls W=0/R=1)
  {
    stopSCCB();  // send SCCB stop sequence
    HAL_UART_Transmit(&huart2, (uint8_t *) "yyy", strlen("yyy"), 10);
    return (0);  // if error: return 0
  }
  Delay(20);
  if (0 == SCCBwriteByte(regID))  // write ov7670 register ID
  {
    stopSCCB();  // send SCCB stop sequence
    HAL_UART_Transmit(&huart2, (uint8_t *) "zzz", strlen("zzz"), 10);
    return (0);  // if error: return 0
  }
  /* a stop is needed(differ from IIC) */
  stopSCCB();  // send SCCB stop sequence

  Delay(20);

  /* read value from register */
  startSCCB();
  if (0 == SCCBwriteByte(0x43)) // write ov7670 device ID(last bit controls W=0/R=1)
  {
    stopSCCB();  // send SCCB stop sequence
    HAL_UART_Transmit(&huart2, (uint8_t *) "qqq", strlen("qqq"), 10);
    return (0);  // if error: return 0
  }
  Delay(20);
  *regDat = SCCBreadByte();  // return read value
  noAck();  // send SCCB NACK sequence
  stopSCCB();  // send SCCB stop sequence
  return (1);  // if success: return 1
}

// set registers
void Reset(void)
{
  Ov7670_WriteReg(REG_COM7, COM7_RESET); // RESET CAMERA
  HAL_Delay(200);
}

void InitForFIFOWriteReset(void)
{
  Ov7670_WriteReg(REG_COM10, COM10_VS_NEG);
}

void InitSetColorbar(void)
{
  unsigned char reg_com7 = 0;
  Ov7670_ReadReg(REG_COM7, &reg_com7);
  // color bar
  Ov7670_WriteReg(REG_COM17, reg_com7 | COM17_CBAR);
}

void InitDefaultReg(void)
{
  // Gamma curve values
  Ov7670_WriteReg(0x7a, 0x20);
  Ov7670_WriteReg(0x7b, 0x10);
  Ov7670_WriteReg(0x7c, 0x1e);
  Ov7670_WriteReg(0x7d, 0x35);
  Ov7670_WriteReg(0x7e, 0x5a);
  Ov7670_WriteReg(0x7f, 0x69);
  Ov7670_WriteReg(0x80, 0x76);
  Ov7670_WriteReg(0x81, 0x80);
  Ov7670_WriteReg(0x82, 0x88);
  Ov7670_WriteReg(0x83, 0x8f);
  Ov7670_WriteReg(0x84, 0x96);
  Ov7670_WriteReg(0x85, 0xa3);
  Ov7670_WriteReg(0x86, 0xaf);
  Ov7670_WriteReg(0x87, 0xc4);
  Ov7670_WriteReg(0x88, 0xd7);
  Ov7670_WriteReg(0x89, 0xe8);

  // AGC and AEC parameters.  Note we start by disabling those features,
  //then turn them only after tweaking the values.
  Ov7670_WriteReg(REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_BFILT);
  Ov7670_WriteReg(REG_GAIN, 0);
  Ov7670_WriteReg(REG_AECH, 0);
  Ov7670_WriteReg(REG_COM4, 0x40);
  // magic reserved bit
  Ov7670_WriteReg(REG_COM9, 0x18);
  // 4x gain + magic rsvd bit
  Ov7670_WriteReg(REG_BD50MAX, 0x05);
  Ov7670_WriteReg(REG_BD60MAX, 0x07);
  Ov7670_WriteReg(REG_AEW, 0x95);
  Ov7670_WriteReg(REG_AEB, 0x33);
  Ov7670_WriteReg(REG_VPT, 0xe3);
  Ov7670_WriteReg(REG_HAECC1, 0x78);
  Ov7670_WriteReg(REG_HAECC2, 0x68);
  Ov7670_WriteReg(0xa1, 0x03);
  // magic
  Ov7670_WriteReg(REG_HAECC3, 0xd8);
  Ov7670_WriteReg(REG_HAECC4, 0xd8);
  Ov7670_WriteReg(REG_HAECC5, 0xf0);
  Ov7670_WriteReg(REG_HAECC6, 0x90);
  Ov7670_WriteReg(REG_HAECC7, 0x94);
  Ov7670_WriteReg(REG_COM8,
  COM8_FASTAEC | COM8_AECSTEP | COM8_BFILT | COM8_AGC | COM8_AEC);

  // Almost all of these are magic "reserved" values.
  Ov7670_WriteReg(REG_COM5, 0x61);
  Ov7670_WriteReg(REG_COM6, 0x4b);
  Ov7670_WriteReg(0x16, 0x02);
  Ov7670_WriteReg(REG_MVFP, 0x07);
  Ov7670_WriteReg(0x21, 0x02);
  Ov7670_WriteReg(0x22, 0x91);
  Ov7670_WriteReg(0x29, 0x07);
  Ov7670_WriteReg(0x33, 0x0b);
  Ov7670_WriteReg(0x35, 0x0b);
  Ov7670_WriteReg(0x37, 0x1d);
  Ov7670_WriteReg(0x38, 0x71);
  Ov7670_WriteReg(0x39, 0x2a);
  Ov7670_WriteReg(REG_COM12, 0x78);
  Ov7670_WriteReg(0x4d, 0x40);
  Ov7670_WriteReg(0x4e, 0x20);
  Ov7670_WriteReg(REG_GFIX, 0);
  Ov7670_WriteReg(0x6b, 0x0a);
  Ov7670_WriteReg(0x74, 0x10);
  Ov7670_WriteReg(0x8d, 0x4f);
  Ov7670_WriteReg(0x8e, 0);
  Ov7670_WriteReg(0x8f, 0);
  Ov7670_WriteReg(0x90, 0);
  Ov7670_WriteReg(0x91, 0);
  Ov7670_WriteReg(0x96, 0);
  Ov7670_WriteReg(0x9a, 0);
  Ov7670_WriteReg(0xb0, 0x84);
  Ov7670_WriteReg(0xb1, 0x0c);
  Ov7670_WriteReg(0xb2, 0x0e);
  Ov7670_WriteReg(0xb3, 0x82);
  Ov7670_WriteReg(0xb8, 0x0a);

  // More reserved magic, some of which tweaks white balance
  Ov7670_WriteReg(0x43, 0x0a);
  Ov7670_WriteReg(0x44, 0xf0);
  Ov7670_WriteReg(0x45, 0x34);
  Ov7670_WriteReg(0x46, 0x58);
  Ov7670_WriteReg(0x47, 0x28);
  Ov7670_WriteReg(0x48, 0x3a);
  Ov7670_WriteReg(0x59, 0x88);
  Ov7670_WriteReg(0x5a, 0x88);
  Ov7670_WriteReg(0x5b, 0x44);
  Ov7670_WriteReg(0x5c, 0x67);
  Ov7670_WriteReg(0x5d, 0x49);
  Ov7670_WriteReg(0x5e, 0x0e);
  Ov7670_WriteReg(0x6c, 0x0a);
  Ov7670_WriteReg(0x6d, 0x55);
  Ov7670_WriteReg(0x6e, 0x11);
  Ov7670_WriteReg(0x6f, 0x9f);
  // "9e for advance AWB"
  Ov7670_WriteReg(0x6a, 0x40);
  Ov7670_WriteReg(REG_BLUE, 0x40);
  Ov7670_WriteReg(REG_RED, 0x60);
  Ov7670_WriteReg(REG_COM8,
  COM8_FASTAEC | COM8_AECSTEP | COM8_BFILT | COM8_AGC | COM8_AEC | COM8_AWB);

  // Matrix coefficients
  Ov7670_WriteReg(0x4f, 0x80);
  Ov7670_WriteReg(0x50, 0x80);
  Ov7670_WriteReg(0x51, 0);
  Ov7670_WriteReg(0x52, 0x22);
  Ov7670_WriteReg(0x53, 0x5e);
  Ov7670_WriteReg(0x54, 0x80);
  Ov7670_WriteReg(0x58, 0x9e);

  Ov7670_WriteReg(REG_COM16, COM16_AWBGAIN);
  Ov7670_WriteReg(REG_EDGE, 0);
  Ov7670_WriteReg(0x75, 0x05);
  Ov7670_WriteReg(0x76, 0xe1);
  Ov7670_WriteReg(0x4c, 0);
  Ov7670_WriteReg(0x77, 0x01);
  Ov7670_WriteReg(0x4b, 0x09);
  Ov7670_WriteReg(0xc9, 0x60);
  Ov7670_WriteReg(REG_COM16, 0x38);
  Ov7670_WriteReg(0x56, 0x40);

  Ov7670_WriteReg(0x34, 0x11);
  Ov7670_WriteReg(REG_COM11, COM11_EXP | COM11_HZAUTO_ON);
  Ov7670_WriteReg(0xa4, 0x88);
  Ov7670_WriteReg(0x96, 0);
  Ov7670_WriteReg(0x97, 0x30);
  Ov7670_WriteReg(0x98, 0x20);
  Ov7670_WriteReg(0x99, 0x30);
  Ov7670_WriteReg(0x9a, 0x84);
  Ov7670_WriteReg(0x9b, 0x29);
  Ov7670_WriteReg(0x9c, 0x03);
  Ov7670_WriteReg(0x9d, 0x4c);
  Ov7670_WriteReg(0x9e, 0x3f);
  Ov7670_WriteReg(0x78, 0x04);

  // Extra-weird stuff.  Some sort of multiplexor register
  Ov7670_WriteReg(0x79, 0x01);
  Ov7670_WriteReg(0xc8, 0xf0);
  Ov7670_WriteReg(0x79, 0x0f);
  Ov7670_WriteReg(0xc8, 0x00);
  Ov7670_WriteReg(0x79, 0x10);
  Ov7670_WriteReg(0xc8, 0x7e);
  Ov7670_WriteReg(0x79, 0x0a);
  Ov7670_WriteReg(0xc8, 0x80);
  Ov7670_WriteReg(0x79, 0x0b);
  Ov7670_WriteReg(0xc8, 0x01);
  Ov7670_WriteReg(0x79, 0x0c);
  Ov7670_WriteReg(0xc8, 0x0f);
  Ov7670_WriteReg(0x79, 0x0d);
  Ov7670_WriteReg(0xc8, 0x20);
  Ov7670_WriteReg(0x79, 0x09);
  Ov7670_WriteReg(0xc8, 0x80);
  Ov7670_WriteReg(0x79, 0x02);
  Ov7670_WriteReg(0xc8, 0xc0);
  Ov7670_WriteReg(0x79, 0x03);
  Ov7670_WriteReg(0xc8, 0x40);
  Ov7670_WriteReg(0x79, 0x05);
  Ov7670_WriteReg(0xc8, 0x30);
  Ov7670_WriteReg(0x79, 0x26);
}

void InitRGB444(void)
{
  unsigned char reg_com7 = 0;
  Ov7670_ReadReg(REG_COM7, &reg_com7);

  Ov7670_WriteReg(REG_COM7, reg_com7 | COM7_RGB);
  Ov7670_WriteReg(REG_RGB444, RGB444_ENABLE | RGB444_XBGR);
  Ov7670_WriteReg(REG_COM15, COM15_R01FE | COM15_RGB444);

  Ov7670_WriteReg(REG_COM1, 0x40);                        // Magic reserved bit
  Ov7670_WriteReg(REG_COM9, 0x38);     // 16x gain ceiling; 0x8 is reserved bit
  Ov7670_WriteReg(0x4f, 0xb3);                        // "matrix coefficient 1"
  Ov7670_WriteReg(0x50, 0xb3);                        // "matrix coefficient 2"
  Ov7670_WriteReg(0x51, 0x00);                              // vb
  Ov7670_WriteReg(0x52, 0x3d);                        // "matrix coefficient 4"
  Ov7670_WriteReg(0x53, 0xa7);                        // "matrix coefficient 5"
  Ov7670_WriteReg(0x54, 0xe4);                        // "matrix coefficient 6"
  Ov7670_WriteReg(REG_COM13, COM13_GAMMA | COM13_UVSAT | 0x2); // Magic rsvd bit

  Ov7670_WriteReg(REG_TSLB, 0x04);
}

void InitRGB555(void)
{
  unsigned char reg_com7 = 0;
  Ov7670_ReadReg(REG_COM7, &reg_com7);

  Ov7670_WriteReg(REG_COM7, reg_com7 | COM7_RGB);
  Ov7670_WriteReg(REG_RGB444, RGB444_DISABLE);
  Ov7670_WriteReg(REG_COM15, COM15_RGB555 | COM15_R00FF);

  Ov7670_WriteReg(REG_TSLB, 0x04);

  Ov7670_WriteReg(REG_COM1, 0x00);
  Ov7670_WriteReg(REG_COM9, 0x38);     // 16x gain ceiling; 0x8 is reserved bit
  Ov7670_WriteReg(0x4f, 0xb3);          // "matrix coefficient 1"
  Ov7670_WriteReg(0x50, 0xb3);          // "matrix coefficient 2"
  Ov7670_WriteReg(0x51, 0x00);          // vb
  Ov7670_WriteReg(0x52, 0x3d);          // "matrix coefficient 4"
  Ov7670_WriteReg(0x53, 0xa7);          // "matrix coefficient 5"
  Ov7670_WriteReg(0x54, 0xe4);          // "matrix coefficient 6"
  Ov7670_WriteReg(REG_COM13, COM13_GAMMA | COM13_UVSAT);
}

void InitRGB565(void)
{
  unsigned char reg_com7 = 0;
  Ov7670_ReadReg(REG_COM7, &reg_com7);

  Ov7670_WriteReg(REG_COM7, reg_com7 | COM7_RGB);
  Ov7670_WriteReg(REG_RGB444, RGB444_DISABLE);
  Ov7670_WriteReg(REG_COM15, COM15_R00FF | COM15_RGB565);

  Ov7670_WriteReg(REG_TSLB, 0x04);

  Ov7670_WriteReg(REG_COM1, 0x00);
  Ov7670_WriteReg(REG_COM9, 0x38);     // 16x gain ceiling; 0x8 is reserved bit
  Ov7670_WriteReg(0x4f, 0xb3);          // "matrix coefficient 1"
  Ov7670_WriteReg(0x50, 0xb3);          // "matrix coefficient 2"
  Ov7670_WriteReg(0x51, 0x00);          // vb
  Ov7670_WriteReg(0x52, 0x3d);          // "matrix coefficient 4"
  Ov7670_WriteReg(0x53, 0xa7);          // "matrix coefficient 5"
  Ov7670_WriteReg(0x54, 0xe4);          // "matrix coefficient 6"
  Ov7670_WriteReg(REG_COM13, COM13_GAMMA | COM13_UVSAT);
}

void InitYUV(void)
{
  unsigned char reg_com7 = 0;
  Ov7670_ReadReg(REG_COM7, &reg_com7);

  Ov7670_WriteReg(REG_COM7, reg_com7 | COM7_YUV);
  Ov7670_WriteReg(REG_RGB444, RGB444_DISABLE);
  Ov7670_WriteReg(REG_COM15, COM15_R00FF);

  Ov7670_WriteReg(REG_TSLB, 0x04);
//       Ov7670_WriteReg(REG_TSLB, 0x14);
//       Ov7670_WriteReg(REG_MANU, 0x00);
//       Ov7670_WriteReg(REG_MANV, 0x00);

  Ov7670_WriteReg(REG_COM1, 0x00);
  Ov7670_WriteReg(REG_COM9, 0x18);     // 4x gain ceiling; 0x8 is reserved bit
  Ov7670_WriteReg(0x4f, 0x80);         // "matrix coefficient 1"
  Ov7670_WriteReg(0x50, 0x80);         // "matrix coefficient 2"
  Ov7670_WriteReg(0x51, 0x00);         // vb
  Ov7670_WriteReg(0x52, 0x22);         // "matrix coefficient 4"
  Ov7670_WriteReg(0x53, 0x5e);         // "matrix coefficient 5"
  Ov7670_WriteReg(0x54, 0x80);         // "matrix coefficient 6"
  Ov7670_WriteReg(REG_COM13, COM13_GAMMA | COM13_UVSAT | COM13_UVSWAP);
}

void InitBayerRGB(void)
{
  unsigned char reg_com7 = 0;
  Ov7670_ReadReg(REG_COM7, &reg_com7);

  // odd line BGBG... even line GRGR...
  Ov7670_WriteReg(REG_COM7, reg_com7 | COM7_BAYER);
  // odd line GBGB... even line RGRG...
  //Ov7670_WriteReg(REG_COM7, reg_com7|COM7_PBAYER);

  Ov7670_WriteReg(REG_RGB444, RGB444_DISABLE);
  Ov7670_WriteReg(REG_COM15, COM15_R00FF);

  Ov7670_WriteReg(REG_COM13, 0x08); /* No gamma, magic rsvd bit */
  Ov7670_WriteReg(REG_COM16, 0x3d); /* Edge enhancement, denoise */
  Ov7670_WriteReg(REG_REG76, 0xe1); /* Pix correction, magic rsvd */

  Ov7670_WriteReg(REG_TSLB, 0x04);
}

void InitVGA(void)
{
  // VGA
  unsigned char reg_com7 = 0;
  Ov7670_ReadReg(REG_COM7, &reg_com7);

  Ov7670_WriteReg(REG_COM7, reg_com7 | COM7_VGA);

  Ov7670_WriteReg(REG_HSTART, HSTART_VGA);
  Ov7670_WriteReg(REG_HSTOP, HSTOP_VGA);
  Ov7670_WriteReg(REG_HREF, HREF_VGA);
  Ov7670_WriteReg(REG_VSTART, VSTART_VGA);
  Ov7670_WriteReg(REG_VSTOP, VSTOP_VGA);
  Ov7670_WriteReg(REG_VREF, VREF_VGA);
  Ov7670_WriteReg(REG_COM3, COM3_VGA);
  Ov7670_WriteReg(REG_COM14, COM14_VGA);
  Ov7670_WriteReg(REG_SCALING_XSC, SCALING_XSC_VGA);
  Ov7670_WriteReg(REG_SCALING_YSC, SCALING_YSC_VGA);
  Ov7670_WriteReg(REG_SCALING_DCWCTR, SCALING_DCWCTR_VGA);
  Ov7670_WriteReg(REG_SCALING_PCLK_DIV, SCALING_PCLK_DIV_VGA);
  Ov7670_WriteReg(REG_SCALING_PCLK_DELAY, SCALING_PCLK_DELAY_VGA);
}

void InitFIFO_2bytes_color_nealy_limit_size(void)
{
  // nealy FIFO limit 544x360
  unsigned char reg_com7 = 0;
  Ov7670_ReadReg(REG_COM7, &reg_com7);

  Ov7670_WriteReg(REG_COM7, reg_com7 | COM7_VGA);

  Ov7670_WriteReg(REG_HSTART, HSTART_VGA);
  Ov7670_WriteReg(REG_HSTOP, HSTOP_VGA);
  Ov7670_WriteReg(REG_HREF, HREF_VGA);
  Ov7670_WriteReg(REG_VSTART, VSTART_VGA);
  Ov7670_WriteReg(REG_VSTOP, VSTOP_VGA);
  Ov7670_WriteReg(REG_VREF, VREF_VGA);
  Ov7670_WriteReg(REG_COM3, COM3_VGA);
  Ov7670_WriteReg(REG_COM14, COM14_VGA);
  Ov7670_WriteReg(REG_SCALING_XSC, SCALING_XSC_VGA);
  Ov7670_WriteReg(REG_SCALING_YSC, SCALING_YSC_VGA);
  Ov7670_WriteReg(REG_SCALING_DCWCTR, SCALING_DCWCTR_VGA);
  Ov7670_WriteReg(REG_SCALING_PCLK_DIV, SCALING_PCLK_DIV_VGA);
  Ov7670_WriteReg(REG_SCALING_PCLK_DELAY, SCALING_PCLK_DELAY_VGA);

  Ov7670_WriteReg(REG_HSTART, 0x17);
  Ov7670_WriteReg(REG_HSTOP, 0x5b);
  Ov7670_WriteReg(REG_VSTART, 0x12);
  Ov7670_WriteReg(REG_VSTOP, 0x6c);
}

void InitVGA_3_4(void)
{
  // VGA 3/4 -> 480x360
  unsigned char reg_com7 = 0;
  Ov7670_ReadReg(REG_COM7, &reg_com7);

  Ov7670_WriteReg(REG_COM7, reg_com7 | COM7_VGA);

  Ov7670_WriteReg(REG_HSTART, HSTART_VGA);
  Ov7670_WriteReg(REG_HSTOP, HSTOP_VGA);
  Ov7670_WriteReg(REG_HREF, HREF_VGA);
  Ov7670_WriteReg(REG_VSTART, VSTART_VGA);
  Ov7670_WriteReg(REG_VSTOP, VSTOP_VGA);
  Ov7670_WriteReg(REG_VREF, VREF_VGA);
  Ov7670_WriteReg(REG_COM3, COM3_VGA);
  Ov7670_WriteReg(REG_COM14, COM14_VGA);
  Ov7670_WriteReg(REG_SCALING_XSC, SCALING_XSC_VGA);
  Ov7670_WriteReg(REG_SCALING_YSC, SCALING_YSC_VGA);
  Ov7670_WriteReg(REG_SCALING_DCWCTR, SCALING_DCWCTR_VGA);
  Ov7670_WriteReg(REG_SCALING_PCLK_DIV, SCALING_PCLK_DIV_VGA);
  Ov7670_WriteReg(REG_SCALING_PCLK_DELAY, SCALING_PCLK_DELAY_VGA);

  Ov7670_WriteReg(REG_HSTART, 0x1b);
  Ov7670_WriteReg(REG_HSTOP, 0x57);
  Ov7670_WriteReg(REG_VSTART, 0x12);
  Ov7670_WriteReg(REG_VSTOP, 0x6c);
}

void InitQVGA(void)
{
  // QQVGA
  unsigned char reg_com7 = 0;
  Ov7670_ReadReg(REG_COM7, &reg_com7);

  Ov7670_WriteReg(REG_COM7, reg_com7 | COM7_QVGA);

  Ov7670_WriteReg(REG_HSTART, HSTART_QVGA);
  Ov7670_WriteReg(REG_HSTOP, HSTOP_QVGA);
  Ov7670_WriteReg(REG_HREF, HREF_QVGA);
  Ov7670_WriteReg(REG_VSTART, VSTART_QVGA);
  Ov7670_WriteReg(REG_VSTOP, VSTOP_QVGA);
  Ov7670_WriteReg(REG_VREF, VREF_QVGA);
  Ov7670_WriteReg(REG_COM3, COM3_QVGA);
  Ov7670_WriteReg(REG_COM14, COM14_QVGA);
  Ov7670_WriteReg(REG_SCALING_XSC, SCALING_XSC_QVGA);
  Ov7670_WriteReg(REG_SCALING_YSC, SCALING_YSC_QVGA);
  Ov7670_WriteReg(REG_SCALING_DCWCTR, SCALING_DCWCTR_QVGA);
  Ov7670_WriteReg(REG_SCALING_PCLK_DIV, SCALING_PCLK_DIV_QVGA);
  Ov7670_WriteReg(REG_SCALING_PCLK_DELAY, SCALING_PCLK_DELAY_QVGA);
}

void InitQQVGA(void)
{
  // QQVGA
  unsigned char reg_com7 = 0;
  Ov7670_ReadReg(REG_COM7, &reg_com7);

  Ov7670_WriteReg(REG_COM7, reg_com7 | COM7_QQVGA);

  Ov7670_WriteReg(REG_HSTART, HSTART_QQVGA);
  Ov7670_WriteReg(REG_HSTOP, HSTOP_QQVGA);
  Ov7670_WriteReg(REG_HREF, HREF_QQVGA);
  Ov7670_WriteReg(REG_VSTART, VSTART_QQVGA);
  Ov7670_WriteReg(REG_VSTOP, VSTOP_QQVGA);
  Ov7670_WriteReg(REG_VREF, VREF_QQVGA);
  Ov7670_WriteReg(REG_COM3, COM3_QQVGA);
  Ov7670_WriteReg(REG_COM14, COM14_QQVGA);
  Ov7670_WriteReg(REG_SCALING_XSC, SCALING_XSC_QQVGA);
  Ov7670_WriteReg(REG_SCALING_YSC, SCALING_YSC_QQVGA);
  Ov7670_WriteReg(REG_SCALING_DCWCTR, SCALING_DCWCTR_QQVGA);
  Ov7670_WriteReg(REG_SCALING_PCLK_DIV, SCALING_PCLK_DIV_QQVGA);
  Ov7670_WriteReg(REG_SCALING_PCLK_DELAY, SCALING_PCLK_DELAY_QQVGA);
}

/* Function: Sensor_init
 * if success: return 1
 * if error: return 0
 * */
unsigned char Sensor_init(void)
{
  unsigned char temp;
//  unsigned char hex_display[] = "0x00\t";
//	unsigned int i = 0;

//  XCLK_init_ON();  // switch on MCO to supply clock for CMOS sensor if there is no external crystal oscillator
//  uchar ovidmsb=0,
//  ovidlsb=0;
//  Sensor_GPIO_Init();  // configured in file: gpio.c
//  SCCB_GPIO_Config();  // configured in file: gpio.c

  temp = 0x80;
  if (0 == Ov7670_WriteReg(0x12, temp))
  {	//Reset SCCB
    print_str("Reset Error", 1);
    return 0;  // return 0 if error
  }
  print_str("Reset OK", 1);
  Delay(1);
  if (0 == Ov7670_ReadReg(0x0b, &temp))
  {  // read ID
    print_str("ID Error", 1);
    return 0;  // return 0 if error
  }
  print_str("ID OK", 1);

  /*integer or char to hex_string */
  print_str("id=", 0);
  print_int_hex(temp, 1);

  if (temp == 0x73)
  {  //OV7670
    Reset();

#ifdef YUV
    InitYUV();
#elif defined RGB444
    InitRGB444();
#elif defined RGB555
    InitRGB555();
#elif defined RGB565
    InitRGB565();
#endif

#ifdef QVGA
    InitQVGA();
#elif defined QQVGA
    InitQQVGA();
#endif

    InitForFIFOWriteReset();
    InitDefaultReg();
//    for(i=0; i<OV7670_REG_NUM; i++)
//    {
//      if(0==Ov7670_WriteReg(OV7670_reg[i][0], OV7670_reg[i][1]))
//      {
//    	  return 0;  // return 0 if error
//      }
//    }

//  Sensor_EXTI_Config();  // configured in file: gpio.c
//  Sensor_Interrupts_Config();  // configured in file: gpio.c
  }
  return 1;  // return 1 = OK
}

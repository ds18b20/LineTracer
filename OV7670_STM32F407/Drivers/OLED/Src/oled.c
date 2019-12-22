#include "oled.h"
#include "oledfont.h"  	 

void OLED_DispChinese(uint8_t x, uint8_t y, const uint8_t code[])
{
  uint8_t n;
  OLED_WR_Byte(0xb0 + y, OLED_CMD);	//����ҳ��ַ��0~7��

  OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);   //������ʾλ�á��е͵�ַ
  OLED_WR_Byte(((x & 0x0f) | 0x01), OLED_CMD);   //������ʾλ�á��иߵ�ַ

  for (n = 0; n < 16; n++)
    OLED_WR_Byte(code[n], OLED_DATA);

  y++;
  OLED_WR_Byte(0xb0 + y, OLED_CMD);    //����ҳ��ַ��0~7��
  OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);   //������ʾλ�á��е͵�ַ
  OLED_WR_Byte(((x & 0x0f) | 0x01), OLED_CMD);   //������ʾλ�á��иߵ�ַ
  for (n = 0; n < 16; n++)
    OLED_WR_Byte(code[n + 16], OLED_DATA);
}

void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
  uint8_t i;
  if (cmd)
    OLED_DC_Set();
  else
    OLED_DC_Clr();

  OLED_CS_Clr();

  for (i = 0; i < 8; i++)
  {
    OLED_SCLK_Clr();
    if (dat & 0x80)
      OLED_SDIN_Set();
    else
      OLED_SDIN_Clr();

    OLED_SCLK_Set();
    dat <<= 1;
  }
  OLED_CS_Set();
  OLED_DC_Set();
}

//����OLED��ʾ    
void OLED_Display_On(void)
{
  OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC����
  OLED_WR_Byte(0X14, OLED_CMD);  //DCDC ON
  OLED_WR_Byte(0XAF, OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
  OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC����
  OLED_WR_Byte(0X10, OLED_CMD);  //DCDC OFF
  OLED_WR_Byte(0XAE, OLED_CMD);  //DISPLAY OFF
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)
{
  uint8_t i, n;
  for (i = 0; i < 8; i++)
  {
    OLED_WR_Byte(0xb0 + i, OLED_CMD);    //����ҳ��ַ��0~7��
    OLED_WR_Byte(0x00, OLED_CMD);      //������ʾλ�á��е͵�ַ
    OLED_WR_Byte(0x10, OLED_CMD);      //������ʾλ�á��иߵ�ַ
    for (n = 0; n < 132; n++)
      OLED_WR_Byte(0, OLED_DATA);
  }
}

void OLED_SetPos(uint8_t x, uint8_t y) //������ʼ������
{
  OLED_WR_Byte(0xb0 + y, OLED_CMD);
  OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
  OLED_WR_Byte((x & 0x0f) | 0x01, OLED_CMD);
}

//��ʾ�ַ���
//x,y:	�������
//*s:	�ַ�����ʼ��ַ
//TextSize:����
void OLED_DispStr(uint8_t x, uint8_t y, uint8_t s[], uint8_t TextSize)
{
  uint8_t c = 0, i = 0, j = 0;
  switch (TextSize)
  {
    case FONT_8X6:
    {
      while (s[j] != '\0')
      {
        c = s[j] - 32;
        if (x > 126)
        {
          x = 0;
          y++;
        }
        OLED_SetPos(x, y);
        for (i = 0; i < 6; i++)
          OLED_WR_Byte(F6x8[c][i], OLED_DATA);
        x += 6;
        j++;
      }
    }
      break;

    case FONT_16X8:
    {
      while (s[j] != '\0')
      {
        c = s[j] - 32;
        if (x > 120)
        {
          x = 0;
          y++;
        }
        OLED_SetPos(x, y);
        for (i = 0; i < 8; i++)
          OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
        OLED_SetPos(x, y + 1);
        for (i = 0; i < 8; i++)
          OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
        x += 8;
        j++;
      }
    }
      break;
  }
}

//��ʼ��SSD1306					    
void OLED_Init(void)
{
  HAL_Delay(100);  // delay > 50 ms

  OLED_WR_Byte(0xAE, OLED_CMD); //�ر���ʾ
  OLED_WR_Byte(0xD5, OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
  OLED_WR_Byte(0x50, OLED_CMD); //[3:0],��Ƶ����;[7:4],��Ƶ��
  OLED_WR_Byte(0xA8, OLED_CMD); //��������·��
  OLED_WR_Byte(0X3F, OLED_CMD); //Ĭ��0X3F(1/64)
  OLED_WR_Byte(0xD3, OLED_CMD); //������ʾƫ��
  OLED_WR_Byte(0X00, OLED_CMD); //Ĭ��Ϊ0

  OLED_WR_Byte(0x40, OLED_CMD); //������ʾ��ʼ�� [5:0],����.

  OLED_WR_Byte(0x8D, OLED_CMD); //��ɱ�����
  OLED_WR_Byte(0x14, OLED_CMD); //bit2������/�ر�
  OLED_WR_Byte(0x20, OLED_CMD); //�����ڴ��ַģʽ
  OLED_WR_Byte(0x02, OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
  OLED_WR_Byte(0xA1, OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;

  OLED_WR_Byte(0xC8, OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
//	OLED_WR_Byte(0xC0, OLED_CMD); //��ֱ����

  OLED_WR_Byte(0xDA, OLED_CMD); //����COMӲ����������
  OLED_WR_Byte(0x12, OLED_CMD); //[5:4]����

  OLED_WR_Byte(0x81, OLED_CMD); //�Աȶ�����
  OLED_WR_Byte(0xEF, OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
  OLED_WR_Byte(0xD9, OLED_CMD); //����Ԥ�������
  OLED_WR_Byte(0xf1, OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
  OLED_WR_Byte(0xDB, OLED_CMD); //����VCOMH ��ѹ����
  OLED_WR_Byte(0x30, OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

  OLED_WR_Byte(0xA4, OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
  OLED_WR_Byte(0xA6, OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
  OLED_WR_Byte(0xAF, OLED_CMD); //������ʾ

  OLED_Clear();
}

/****��ָ��ֿ�IC***/
void GB_WriteCmd(uint8_t cmd)
{
  uint8_t i;
  for (i = 0; i < 8; i++)
  {
    if (cmd & 0x80)
    {
      OLED_SDIN_Set();
      asm("NOP");
    }
    else
    {
      OLED_SDIN_Clr();
      asm("NOP");
    }
    cmd = cmd << 1;
    OLED_SCLK_Clr();
    asm("NOP");
    OLED_SCLK_Set();
    asm("NOP");
  }
}

/****���ֿ�IC��ȡ���ֻ��ַ����ݣ�1���ֽڣ�***/
static uint8_t GB_GetData(void)
{
  uint8_t i;
  uint8_t ret_data = 0;
  OLED_SCLK_Set();
  asm("NOP");

  for (i = 0; i < 8; i++)
  {
    OLED_SCLK_Clr();
    asm("NOP");
    ret_data = ret_data << 1;

    if ( GB_SDO)
    {
      ret_data = ret_data + 1;
      asm("NOP");
    }

    OLED_SCLK_Set();
    asm("NOP");
  }
  return (ret_data);
}

/*����ص�ַ��addrHigh����ַ���ֽ�,addrMid����ַ���ֽ�,addrLow����ַ���ֽڣ�����������DataLen���ֽڵ����ݵ� pBuff�ĵ�ַ*/
/*������ȡ*/
void GB_GetBytes(uint8_t addrHigh, uint8_t addrMid, uint8_t addrLow,
                 uint8_t *pBuff, uint8_t DataLen)
{
  uint8_t i;
  GB_CS_Clr();
  OLED_SCLK_Clr();
  GB_WriteCmd(0x03);
  GB_WriteCmd(addrHigh);
  GB_WriteCmd(addrMid);
  GB_WriteCmd(addrLow);
  for (i = 0; i < DataLen; i++)
    *(pBuff + i) = GB_GetData();
  GB_CS_Set();
}

uint32_t fontaddr = 0;
void DispStr_GB2312(uint8_t x, uint8_t y, uint8_t *text)
{
  uint8_t i = 0;
  uint8_t addrHigh, addrMid, addrLow;
  uint8_t fontbuf[32];

  while ((text[i] > 0x00))
  {
    if (((text[i] >= 0xb0) && (text[i] <= 0xf7)) && (text[i + 1] >= 0xa1))
    {
      /*������壨GB2312���������ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
      /*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
      /*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
      fontaddr = (text[i] - 0xb0) * 94;
      fontaddr += (text[i + 1] - 0xa1) + 846;
      fontaddr = (uint32_t) (fontaddr * 32);

      addrHigh = (fontaddr & 0xff0000) >> 16; /*��ַ�ĸ�8λ,��24λ*/
      addrMid = (fontaddr & 0xff00) >> 8; /*��ַ����8λ,��24λ*/
      addrLow = fontaddr & 0xff; /*��ַ�ĵ�8λ,��24λ*/

      GB_GetBytes(addrHigh, addrMid, addrLow, fontbuf, 32);/*ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"*/
      //display_graphic_16x16(y,x,fontbuf);/*��ʾ���ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����*/
      OLED_DispChinese(x, y, fontbuf);
      i += 2;
      x += 16;
    }
    else if (((text[i] >= 0xa1) && (text[i] <= 0xa3)) && (text[i + 1] >= 0xa1))
    {
      /*������壨GB2312��15x16����ַ����ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
      /*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
      /*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
      fontaddr = (text[i] - 0xa1) * 94;
      fontaddr += (text[i + 1] - 0xa1);
      fontaddr = (uint32_t) (fontaddr * 32);

      addrHigh = (fontaddr & 0xff0000) >> 16; /*��ַ�ĸ�8λ,��24λ*/
      addrMid = (fontaddr & 0xff00) >> 8; /*��ַ����8λ,��24λ*/
      addrLow = fontaddr & 0xff; /*��ַ�ĵ�8λ,��24λ*/
      GB_GetBytes(addrHigh, addrMid, addrLow, fontbuf, 32);/*ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"*/
      OLED_DispChinese(x, y, fontbuf);

      i += 2;
      x += 16;
    }
    else if ((text[i] >= 0x20) && (text[i] <= 0x7e))
    {
      unsigned char fontbuf[16];
      fontaddr = (text[i] - 0x20);
      fontaddr = (unsigned long) (fontaddr * 16);
      fontaddr = (unsigned long) (fontaddr + 0x3cf80);
      addrHigh = (fontaddr & 0xff0000) >> 16;
      addrMid = (fontaddr & 0xff00) >> 8;
      addrLow = fontaddr & 0xff;

      GB_GetBytes(addrHigh, addrMid, addrLow, fontbuf, 16);/*ȡ16���ֽڵ����ݣ��浽"fontbuf[32]"*/
      OLED_DispStr(x, y, fontbuf, FONT_16X8);

      i += 1;
      x += 8;
    }
    else
      i++;
  }
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
                  unsigned char y1, unsigned char BMP[])
{
  unsigned int j = 0;
  unsigned char x, y;

  if (y1 % 8 == 0)
    y = y1 / 8;
  else
    y = y1 / 8 + 1;
  for (y = y0; y < y1; y++)
  {
    OLED_SetPos(x0, y);
    for (x = x0; x < x1; x++)
    {
      OLED_WR_Byte(BMP[j++], OLED_DATA);
    }
  }
}

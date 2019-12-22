/*
 */
#include "sccb.h"
#include "main.h"
#include "delay.h"
/*
 #include "stm32f10x_lib.h"
 */

/*
 * Initialize SCCB GPIO
 */
void SCCB_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct =
  { 0 };

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE()
  ;

  /*Configure GPIO pin Output Level: GPIOB_Pin[5] = SCCB_SCL_Pin : High
   *                                 GPIOB_Pin[6] = SCCB_SDA_Pin : High
   * */
  HAL_GPIO_WritePin(GPIOB, SCCB_SCL_Pin | SCCB_SDA_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins: GPIOB_Pin[5] = SCCB_SCL_Pin
   *                     GPIOB_Pin[6] = SCCB_SDA_Pin
   * */
  GPIO_InitStruct.Pin = SCCB_SCL_Pin | SCCB_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/** Configure SCCB_SDA_Pin to IMPUT mode
 */
void SCCB_SID_GPIO_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct =
  { 0 };

  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pins: GPIOB_Pin[6] = SCCB_SDA_Pin
   * */
  GPIO_InitStruct.Pin = SCCB_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;  // ���� �����ⲿ�������� �����ڲ�������
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/** Configure SCCB_SDA_Pin to OUTPUT mode
 */
void SCCB_SID_GPIO_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct =
  { 0 };

  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pins: GPIOB_Pin[6] = SCCB_SDA_Pin
   * */
  GPIO_InitStruct.Pin = SCCB_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/*
 -----------------------------------------------
 ����: start����,SCCB����ʼ�ź�
 ����: ��
 ����ֵ: ��
 -----------------------------------------------
 */
void startSCCB(void)
{
  SCCB_SID_H();     //�����߸ߵ�ƽ
  Delay(50);

  SCCB_SIC_H();	   //��ʱ���߸ߵ�ʱ���������ɸ�����
  Delay(50);

  SCCB_SID_L();
  Delay(50);

  SCCB_SIC_L();	 //�����߻ָ��͵�ƽ��������������Ҫ
  Delay(50);
}
/*
 -----------------------------------------------
 ����: stop����,SCCB��ֹͣ�ź�
 ����: ��
 ����ֵ: ��
 -----------------------------------------------
 */
void stopSCCB(void)
{
  SCCB_SID_L();
  Delay(50);

  SCCB_SIC_H();
  Delay(50);

  SCCB_SID_H();
  Delay(50);
}

/*
 -----------------------------------------------
 ����: noAck,����������ȡ�е����һ����������
 ����: ��
 ����ֵ: ��
 -----------------------------------------------
 */
void noAck(void)
{
  SCCB_SID_H();
  Delay(50);

  SCCB_SIC_H();
  Delay(50);

  SCCB_SIC_L();
  Delay(50);

  SCCB_SID_L();
  Delay(50);
}

/*
 -----------------------------------------------
 ����: д��һ���ֽڵ����ݵ�SCCB
 ����: д������
 ����ֵ: ���ͳɹ�����1������ʧ�ܷ���0
 -----------------------------------------------
 */
unsigned char SCCBwriteByte(unsigned char m_data)
{
  unsigned char j, tem;

  for (j = 0; j < 8; j++) //ѭ��8�η�������
  {
    if ((m_data << j) & 0x80)
    {
      SCCB_SID_H();
    }
    else
    {
      SCCB_SID_L();
    }
    Delay(50);
    SCCB_SIC_H();
    Delay(50);
    SCCB_SIC_L();
    Delay(2);
  }
  //Delay(50);
  SCCB_SID_IN
  ;/*����SDAΪ����*/
  Delay(10);
  SCCB_SIC_H();
  Delay(80);
  if (SCCB_SID_STATE)
  {
    tem = 0;
  }   //SDA=1����ʧ�ܣ�����0}
  else
  {
    tem = 1;
  }   //SDA=0���ͳɹ�������1
  SCCB_SIC_L();
  Delay(50);
  SCCB_SID_OUT
  ;/*����SDAΪ���*/

  return (tem);
}

/*
 -----------------------------------------------
 ����: һ���ֽ����ݶ�ȡ���ҷ���
 ����: ��
 ����ֵ: ��ȡ��������
 -----------------------------------------------
 */
unsigned char SCCBreadByte(void)
{
  unsigned char read, j;
  read = 0x00;

  SCCB_SID_IN
  ;/*����SDAΪ����*/
  Delay(50);
  for (j = 8; j > 0; j--) //ѭ��8�ν�������
  {
    //Delay(100);
    SCCB_SIC_H();
    Delay(50);
    read = read << 1;
    if (SCCB_SID_STATE)
    {
      read = read + 1;
    }
    SCCB_SIC_L();
    Delay(50);
  }
  SCCB_SID_OUT
  ;/*����SDAΪ���*/
  return (read);
}

#ifndef _sccb_H
#define _sccb_H

#define SCCB_SIC_H()     HAL_GPIO_WritePin(GPIOB, SCCB_SCL_Pin, GPIO_PIN_SET)
#define SCCB_SIC_L()     HAL_GPIO_WritePin(GPIOB, SCCB_SCL_Pin, GPIO_PIN_RESET)

#define SCCB_SID_H()     HAL_GPIO_WritePin(GPIOB, SCCB_SDA_Pin, GPIO_PIN_SET)
#define SCCB_SID_L()     HAL_GPIO_WritePin(GPIOB, SCCB_SDA_Pin, GPIO_PIN_RESET)
/**/
#define SCCB_SID_IN      SCCB_SID_GPIO_INPUT();
#define SCCB_SID_OUT     SCCB_SID_GPIO_OUTPUT();

#define SCCB_SID_STATE	 HAL_GPIO_ReadPin(GPIOB, SCCB_SDA_Pin)

///////////////////////////////////////////
void
SCCB_GPIO_Config(void);
void
SCCB_SID_GPIO_OUTPUT(void);
void
SCCB_SID_GPIO_INPUT(void);
void
startSCCB(void);
void
stopSCCB(void);
void
noAck(void);
unsigned char
SCCBwriteByte(unsigned char m_data);
unsigned char
SCCBreadByte(void);

#endif

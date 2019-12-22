/*
 * pid.h
 *
 *  Created on: 2019��12��18��
 *      Author: ds18b20
 */

#ifndef PID_H_
#define PID_H_

typedef struct _pid
{
  float SetValue;     //�����趨ֵ
  float CurrentValue; //����ʵ��ֵ
  float ControlValue; //������
  float err;          //����ƫ��ֵ
  float err_next;     //������һ��ƫ��ֵ
  float err_last;     //��������ǰ��ƫ��ֵ
  float Kp, Ki, Kd;   //������������֡�΢��ϵ��
} PID_HandleTypeDef;
extern PID_HandleTypeDef pid;

void PID_init(void);
float PID_realize(float speed);
float P_realize(float value);

#endif /* PID_H_ */

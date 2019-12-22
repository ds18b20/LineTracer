/*
 * pid.h
 *
 *  Created on: 2019年12月18日
 *      Author: ds18b20
 */

#ifndef PID_H_
#define PID_H_

typedef struct _pid
{
  float SetValue;     //定义设定值
  float CurrentValue; //定义实际值
  float ControlValue; //控制量
  float err;          //定义偏差值
  float err_next;     //定义上一个偏差值
  float err_last;     //定义最上前的偏差值
  float Kp, Ki, Kd;   //定义比例、积分、微分系数
} PID_HandleTypeDef;
extern PID_HandleTypeDef pid;

void PID_init(void);
float PID_realize(float speed);
float P_realize(float value);

#endif /* PID_H_ */

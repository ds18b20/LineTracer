/*
 * pid.c
 *
 *  Created on: 2019Äê12ÔÂ18ÈÕ
 *      Author: ds18b20
 */
#include "pid.h"
float diff = 0;
PID_HandleTypeDef pid;

void PID_init(void)
{
  pid.SetValue = 0.0;
  pid.CurrentValue = 0.0;
  pid.ControlValue = 0.0;
  pid.err = 0.0;
  pid.err_last = 0.0;
  pid.err_next = 0.0;
  pid.Kp = 0.2;
  pid.Ki = 0.015;
  pid.Kd = 0.2;
}

float PID_realize(float value)
{
  pid.SetValue = value;
  pid.err = pid.SetValue - pid.CurrentValue;
  diff = pid.Kp * (pid.err - pid.err_next) + pid.Ki * pid.err
      + pid.Kd * (pid.err - 2 * pid.err_next + pid.err_last);
  pid.ControlValue += diff;
  pid.err_last = pid.err_next;
  pid.err_next = pid.err;
  return pid.ControlValue;
}

float P_realize(float value)
{
  pid.SetValue = value;
  pid.err = pid.SetValue - pid.CurrentValue;
  diff = 1 * pid.err;

  return diff;
}
//int main()
//{
//  PID_init();
//  int count = 0;
//  while (count < 1000)
//  {
//    float speed = PID_realize(200.0);
//    printf("%f\n", speed);
//    count++;
//  }
//  return 0;
//}

#include "main_process.h"

void Initialize()
{
  //init sbus
  //init mti sensor
  Init_pwm();
  Init_gain_value();
  
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
}

void Tim6_loop_1ms()
{
//sensor read
//sbus read
  Calc_pid_control();
  Mixer();
}

void Tim7_loop_1sec()
{
  
}
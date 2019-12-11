#include "headers.h"

static uint16_t tim6_loop_counter = 0;
uint16_t sensor_counter = 0;

void Initialize()
{
  Sbus_init();  
  Sensor_start();
  
  Init_pwm();
  Init_gain_value();
  
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
}

void Tim6Loop()
{ 
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);
  Sensor_read();
  Sbus_read();
  Calc_pid_control();
  Mixer();
  Send_xbee();
  tim6_loop_counter++;
}

void Tim7Loop()
{
  tim6_loop_counter = 0;
  sensor_counter = 0;
}
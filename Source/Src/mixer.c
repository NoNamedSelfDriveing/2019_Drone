#include "mixer.h"

void Init_pwm()
{
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4); 
  
  htim1.Instance->CCR1 = 500;
  htim1.Instance->CCR2 = 500;
  htim1.Instance->CCR3 = 500;
  htim1.Instance->CCR4 = 500;
}

void Mixer()
{
    float motor_pwm[4];
    int i;
    for (i = 0;i < 4; i++)
    {
      if(motor_pwm[i] < 0)
        motor_pwm[i] = 0;
      else if(motor_pwm[i] > 2000)
        motor_pwm[i] = 2000;
    }

    if(SBUS.throttle == 0.0)
    {
      motor_pwm[0] = 0.0;
      motor_pwm[1] = 0.0;
      motor_pwm[2] = 0.0;
      motor_pwm[3] = 0.0;
    } 
    else
    {
      motor_pwm[0] = - RPY.control_cmd[ROLL] - RPY.control_cmd[PITCH] - RPY.control_cmd[YAW];
      motor_pwm[1] = RPY.control_cmd[ROLL] - RPY.control_cmd[PITCH] + RPY.control_cmd[YAW];
      motor_pwm[2] = RPY.control_cmd[ROLL] + RPY.control_cmd[PITCH] - RPY.control_cmd[YAW];
      motor_pwm[3] = -RPY.control_cmd[ROLL] + RPY.control_cmd[PITCH] + RPY.control_cmd[YAW];
    }
    
    TIM1->CCR1 = (uint32_t)motor_pwm[0]; //Upper right motor
    TIM1->CCR2 = (uint32_t)motor_pwm[1]; //Upper left motor
    TIM1->CCR3 = (uint32_t)motor_pwm[2]; //Left below motor
    TIM1->CCR4 = (uint32_t)motor_pwm[3]; //Right below motor
} 



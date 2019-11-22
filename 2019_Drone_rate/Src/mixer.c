#include "mixer.h"

void Init_pwm()
{
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4); 
  
  htim1.Instance->CCR1 = 1000;
  htim1.Instance->CCR2 = 1000;
  htim1.Instance->CCR3 = 1000;
  htim1.Instance->CCR4 = 1000;
  
  HAL_Delay(500);
}

void Mixer()
{
    float motor_pwm[4];
    int i;
    int throttle_mapping;

    if(SBUS.set_point[THROTTLE] < -98.0f)
    {
      motor_pwm[0] = 1000;
      motor_pwm[1] = 1000;
      motor_pwm[2] = 1000;
      motor_pwm[3] = 1000;
    } 
    else if(SBUS.frame_lost == 1 && SBUS.fail_safe ==1)
    {
      motor_pwm[0] = 1000;
      motor_pwm[1] = 1000;
      motor_pwm[2] = 1000;
      motor_pwm[3] = 1000;      
    }
    else
    {
      throttle_mapping = (int)SBUS.set_point[THROTTLE] * 5 + 1500;
      
      motor_pwm[0] = - RPY.control_cmd[Roll] - RPY.control_cmd[Pitch] - RPY.control_cmd[Yaw] + throttle_mapping;
      motor_pwm[1] = RPY.control_cmd[Roll] - RPY.control_cmd[Pitch] + RPY.control_cmd[Yaw] + throttle_mapping;
      motor_pwm[2] = RPY.control_cmd[Roll] + RPY.control_cmd[Pitch] - RPY.control_cmd[Yaw] + throttle_mapping;
      motor_pwm[3] = -RPY.control_cmd[Roll] + RPY.control_cmd[Pitch] + RPY.control_cmd[Yaw] + throttle_mapping;
    }

    for (i = 0;i < 4; i++)
    {
      if(motor_pwm[i] < 1000)
        motor_pwm[i] = 1000;
      else if(motor_pwm[i] > 2000)
        motor_pwm[i] = 2000;
    }

    TIM1->CCR1 = (uint32_t)motor_pwm[0]; //Upper right motor
    TIM1->CCR2 = (uint32_t)motor_pwm[1]; //Upper left motor
    TIM1->CCR3 = (uint32_t)motor_pwm[2]; //Left below motor
    TIM1->CCR4 = (uint32_t)motor_pwm[3]; //Right below motor
    
    //printf("%d %d %d %d\n\r", (uint32_t)motor_pwm[0], (uint32_t)motor_pwm[1], (uint32_t)motor_pwm[2], (uint32_t)motor_pwm[3]);

} 
#include "headers.h"

static uint16_t tim6LoopCounter = 0;
uint16_t sensorCounter = 0;

void Initialize()
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);  
  SensorStart();
  
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
}
  
void Tim6Loop()
{
  SensorRead();
  tim6LoopCounter++;
}

void Tim7Loop()
{
//  printf("%d %d\r\n",HAL_GetTick(),tim6LoopCounter);
  tim6LoopCounter = 0;
  sensorCounter = 0;
}
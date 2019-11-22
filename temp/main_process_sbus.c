#include "headers.h"

static uint16_t tim6LoopCounter = 0;
uint16_t sensorCounter = 0;

void Initialize()
{
  SensorStart();
  Sbus_init();
  Initcontroller();
  InitPWM();
  
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
}
  
void Tim6Loop()
{
  SensorRead();
  Sbus_read();
  
  if(sensorData.updateFlag == 1)
  {
    sensorData.updateFlag = 0;
    Controller();
    Mixer();
  }
  
  tim6LoopCounter++;
}

void Tim7Loop()
{
//  printf("%d %d\r\n",HAL_GetTick(),tim6LoopCounter);
  tim6LoopCounter = 0;
  sensorCounter = 0;
}
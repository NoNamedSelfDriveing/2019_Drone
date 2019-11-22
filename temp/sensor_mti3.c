 #include "headers.h"

uint8_t CalculateChecksum();
uint16_t CheckDataSize();

extern uint16_t sensorCounter;

static uint8_t sensorRcvData[SENSOR_DMA_SIZE];
static uint16_t sensorDataTail;
static uint16_t sensorDataHead = 0;

union mtiData
{
  float floatData[DATASIZE/4];
  uint8_t uint8Data[DATASIZE];
};

struct mtiValue
{
  float euler[3];
};

union mtiData _mtiData;
struct mtiValue _mti;

extern DMA_HandleTypeDef hdma_usart3_rx;

void SensorStart()
{
  HAL_UART_Receive_DMA(&huart3,sensorRcvData,SENSOR_DMA_SIZE);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);
}

void SensorRead()
{
  //uint8_t testIdx;
    uint8_t i;

  sensorDataTail = SENSOR_DMA_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);

//  for(i=0;i<12;i++)
//    printf("%x ",sensorRcvData[i]);
//  printf("\r\n");

  while(CheckDataSize() >= SENSOR_PACKET_SIZE)
  {
    if((sensorRcvData[sensorDataHead] == 0xfa)&&(sensorRcvData[(sensorDataHead+1)%SENSOR_DMA_SIZE] == 0xff))
    {
      HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);
      if(CalculateChecksum() == 0)
      {
//        for(testIdx = 0; testIdx<12; testIdx++)
//        {
//          _mtiData.uint8Data[11-testIdx] = sensorRcvData[(sensorDataHead+testIdx+7)%SENSOR_DMA_SIZE];
//        }
        DecodingData();
        
        printf("%.3d roll = %.4f pitch = %.4f yaw = %.4f\r\n",++sensorCounter,_mti.euler[0],-_mti.euler[1],-_mti.euler[2]);
        
        sensorDataHead = (sensorDataHead + SENSOR_PACKET_SIZE)%SENSOR_DMA_SIZE;
      }
      else
      {
        sensorDataHead = (sensorDataHead + 1)%SENSOR_DMA_SIZE;
//        printf("checksum is wrong\r\n");
      }
    }
    else
    {
      sensorDataHead = (sensorDataHead + 1)%SENSOR_DMA_SIZE;
//      pritnf("startbit is wrong\r\n");
    }
  }     
  
}

uint16_t CheckDataSize()
{ 
  if(sensorDataTail >= sensorDataHead)
  {
    return sensorDataTail-sensorDataHead;
  }
  else
  {
    return sensorDataTail+SENSOR_DMA_SIZE-sensorDataHead;
  }  
}


uint8_t CalculateChecksum()
{
  uint16_t idx;
  uint16_t result = 0;
  for(idx = 1; idx<SENSOR_PACKET_SIZE ; idx++)
  {
    result = (result + sensorRcvData[(sensorDataHead+idx)%SENSOR_DMA_SIZE])&0xff;
  }
  return (uint8_t)result;
}

void DecodingData()
{
  int i;
  
  /* euler angle */
  for(i=0;i<DATASIZE;i++)
  {
    _mtiData.uint8Data[11-i] = sensorRcvData[(sensorDataHead+i+7)%SENSOR_DMA_SIZE];
  }
  
  _mti.euler[0] = _mtiData.floatData[2];       //roll
  _mti.euler[1] = _mtiData.floatData[1];       //pitch
  _mti.euler[2] = _mtiData.floatData[0];       //yaw    
}
  
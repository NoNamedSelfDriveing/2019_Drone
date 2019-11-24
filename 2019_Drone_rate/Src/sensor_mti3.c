#include "headers.h"
 
uint8_t Calculate_checksum();
uint8_t Check_data_size();
uint16_t Calculate_data_size();

extern uint16_t sensor_counter;
extern _SENSOR SENSOR;

static uint8_t sensor_rcv_data[SENSOR_DMA_SIZE];
static uint16_t sensor_data_tail;
static uint16_t sensor_data_head = 0;

union _MTIDATA
{
  float float_data[DATASIZE/4];
  uint8_t uint8_data[DATASIZE];
};


union _MTIDATA MTIDATA;

extern DMA_HandleTypeDef hdma_usart3_rx;

uint8_t status;

void Sensor_start()
{
  HAL_UART_Receive_DMA(&huart3,sensor_rcv_data,SENSOR_DMA_SIZE);
  for(int i=0;i<DATASIZE;i++)
  {
    MTIDATA.float_data[i] = 0;
    MTIDATA.uint8_data[i] = 0;
  }
}

void Dma_error_check()
{
  if (((uint8_t)huart3.Instance->ISR & USART_ISR_ORE_Msk) == USART_ISR_ORE_Msk)
  {
    SET_BIT(huart3.Instance->ICR, USART_ICR_ORECF);
    HAL_UART_Receive_DMA(&huart3, (unsigned char *)sensor_rcv_data, SENSOR_DMA_SIZE);
  }
}

void Sensor_read()
{
  Dma_error_check();
  sensor_data_tail = SENSOR_DMA_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);

//  for(uint8_t i=0;i<GPS_PACKET_SIZE;i++)
//    printf("%x ",sensor_rcv_data[i]);
//  printf("\r\n");
  
//  printf("%d\r\n",Calculate_data_size());
//  printf("%d\r\n",sensor_data_tail);
//  printf("%d\r\n",__HAL_DMA_GET_COUNTER(&hdma_usart3_rx));
  
  while(Check_data_size())
  {
    status=Check_data_size();
    if((sensor_rcv_data[sensor_data_head] == 0xfa)&&(sensor_rcv_data[(sensor_data_head+1)%SENSOR_DMA_SIZE] == 0xff))
    {
      if(Calculate_checksum() == 0)
      {
        Decoding_data();
       
//        printf("%d %.3d ",status,++sensor_counter);
//        printf("roll = %.4f pitch = %.4f yaw = %.4f ", SENSOR.angle[0], SENSOR.angle[1], SENSOR.angle[2]);
//        printf("gyrX = %.4f gyrY = %.4f gyrZ = %.4f ", SENSOR.rate[0], SENSOR.rate[1], SENSOR.rate[2]);
//        printf("accX = %.4f accY = %.4f accZ = %.4f ", SENSOR.acc[0], SENSOR.acc[1], SENSOR.acc[2]);
//        printf("lat = %.4f lon = %.4f ", SENSOR.gps[0], SENSOR.gps[1]);
//        printf("alt = %.4f\r\n",SENSOR.alt[0]);
        
        if(status == 1)
          sensor_data_head = (sensor_data_head + SENSOR_PACKET_SIZE)%SENSOR_DMA_SIZE;
        else if(status == 2)
          sensor_data_head = (sensor_data_head + GPS_PACKET_SIZE)%SENSOR_DMA_SIZE;
      }
      else
      {
        sensor_data_head = (sensor_data_head + 1)%SENSOR_DMA_SIZE;
        //printf("checksum is wrong\r\n");
      }
    }
    else
    {
      sensor_data_head = (sensor_data_head + 1)%SENSOR_DMA_SIZE;
      //printf("startbit is wrong\r\n");
    }
  }     
  
}

uint16_t Calculate_data_size()
{ 
  if(sensor_data_tail >= sensor_data_head)
  {
    return sensor_data_tail-sensor_data_head;
  }
  else
  {
    return sensor_data_tail+SENSOR_DMA_SIZE-sensor_data_head;
  }  
}

uint8_t Check_data_size()
{
  if(Calculate_data_size() >= GPS_PACKET_SIZE)
  {
    status = 2;
  }
  
  else if(Calculate_data_size() >= SENSOR_PACKET_SIZE)
  {
    status = 1;
  }
  
  else
  {
    status = 0;
  }
  return status;
}


uint8_t Calculate_checksum()
{
  uint16_t idx;
  uint16_t result = 0;
  for(idx = 1; idx<SENSOR_PACKET_SIZE ; idx++)
  {
    result = (result + sensor_rcv_data[(sensor_data_head+idx)%SENSOR_DMA_SIZE])&0xff;
  }
  return (uint8_t)result;
}

void Decoding_data()
{
  int i;
  
  if(status == 1)
  {
    for(i=0;i<12;i++)
      MTIDATA.uint8_data[11-i] = sensor_rcv_data[(sensor_data_head+7+i)%SENSOR_DMA_SIZE];
    for(i=0;i<12;i++)
      MTIDATA.uint8_data[23-i] = sensor_rcv_data[(sensor_data_head+22+i)%SENSOR_DMA_SIZE];
    for(i=0;i<12;i++)
      MTIDATA.uint8_data[35-i] = sensor_rcv_data[(sensor_data_head+37+i)%SENSOR_DMA_SIZE];
    
    SENSOR.angle[0] = MTIDATA.float_data[2];       //roll
    SENSOR.angle[1] = MTIDATA.float_data[1];       //pitch
    SENSOR.angle[2] = MTIDATA.float_data[0];       //yaw
  
    SENSOR.rate[0] = MTIDATA.float_data[8];       //gyrX
    SENSOR.rate[1] = MTIDATA.float_data[7];       //gyrY
    SENSOR.rate[2] = MTIDATA.float_data[6];       //gryZ
  
    SENSOR.acc[0] = MTIDATA.float_data[5];        //accX
    SENSOR.acc[1] = MTIDATA.float_data[4];        //accY
    SENSOR.acc[2] = MTIDATA.float_data[3];        //accZ
  }
  else if(status == 2)
  {
    for(i=0;i<12;i++)
      MTIDATA.uint8_data[11-i] = sensor_rcv_data[(sensor_data_head+7+i)%SENSOR_DMA_SIZE];       //18
    for(i=0;i<12;i++)
      MTIDATA.uint8_data[23-i] = sensor_rcv_data[(sensor_data_head+22+i)%SENSOR_DMA_SIZE];      //33
    for(i=0;i<12;i++)
      MTIDATA.uint8_data[35-i] = sensor_rcv_data[(sensor_data_head+37+i)%SENSOR_DMA_SIZE];      //48
    for(i=0;i<8;i++)
      MTIDATA.uint8_data[47-i] = sensor_rcv_data[(sensor_data_head+59+i)%SENSOR_DMA_SIZE];      //66
    for(i=0;i<4;i++)
      MTIDATA.uint8_data[51-i] = sensor_rcv_data[(sensor_data_head+70+i)%SENSOR_DMA_SIZE];      //74
    
    SENSOR.angle[0] = MTIDATA.float_data[2];       //roll
    SENSOR.angle[1] = MTIDATA.float_data[1];       //pitch
    SENSOR.angle[2] = MTIDATA.float_data[0];       //yaw
  
    SENSOR.rate[0] = MTIDATA.float_data[8];       //gyrX
    SENSOR.rate[1] = MTIDATA.float_data[7];       //gyrY
    SENSOR.rate[2] = MTIDATA.float_data[6];       //gryZ
  
    SENSOR.acc[0] = MTIDATA.float_data[5];        //accX
    SENSOR.acc[1] = MTIDATA.float_data[4];        //accY
    SENSOR.acc[2] = MTIDATA.float_data[3];        //accZ
  
    SENSOR.gps[0] = MTIDATA.float_data[11];       //lat
    SENSOR.gps[1] = MTIDATA.float_data[10];        //lon
  
    SENSOR.alt[0] = MTIDATA.float_data[12];       //alt
   }
   /*
  for(i=0;i<12;i++)
  {
    MTIDATA.uint8_data[23-i] = sensor_rcv_data[(sensor_data_head+i+7)%SENSOR_DMA_SIZE];
  }
  for(i=12;i<24;i++)
  {
    MTIDATA.uint8_data[23-i] = sensor_rcv_data[(sensor_data_head+i+7+3)%SENSOR_DMA_SIZE];
  }*/
}
#include "stdio.h"
#include "dma.h"
#include "sbus.h"
#include "main.h"

uint16_t Check_data_size_sbus();

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

static uint8_t sbus_rcv_data[SBUS_DMA_SIZE];
static uint16_t sbus_data_tail = 0;
static uint16_t sbus_data_head = 0;

_SBUS SBUS;

void Sbus_init() {
  SBUS.decode_finish_flag = 0;
  SBUS.sbus_read_flag = 0;
}

void Sbus_start()
{
  if (SBUS.sbus_read_flag == 0) {
    HAL_UART_Receive_DMA(&huart2, sbus_rcv_data, SBUS_DMA_SIZE);
    SBUS.sbus_read_flag = 1;
  }
}

void Sbus_read() {
  SBUS.decode_finish_flag = 0;
  Sbus_start();
  
  if (SBUS.sbus_read_flag == 1 && SBUS.decode_finish_flag == 0) {
    Sbus_decode();
    
  }
}

void Sbus_decode()
{ 
  sbus_data_tail = SBUS_DMA_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
  while(Check_data_size_sbus() >= SBUS_PACKET_SIZE)
  {
    if((sbus_rcv_data[sbus_data_head] == 0x0f)&&((sbus_rcv_data[(sbus_data_head+24)%SBUS_DMA_SIZE] == 0x04)||(sbus_rcv_data[(sbus_data_head+24)%SBUS_DMA_SIZE] == 0x14)||(sbus_rcv_data[(sbus_data_head+24)%SBUS_DMA_SIZE] == 0x24)||(sbus_rcv_data[(sbus_data_head+24)%SBUS_DMA_SIZE] == 0x34)))
    {

      SBUS.sbus_data[CH1] = ((sbus_rcv_data[(sbus_data_head+1)%SBUS_DMA_SIZE] + (sbus_rcv_data[(sbus_data_head+2)%SBUS_DMA_SIZE] << 8)) & 0x07ff);  //*0.625+880;
      SBUS.sbus_data[CH2] = (((sbus_rcv_data[(sbus_data_head+2)%SBUS_DMA_SIZE] >> 3) + (sbus_rcv_data[(sbus_data_head+3)%SBUS_DMA_SIZE] << 5)) & 0x07ff);
      SBUS.sbus_data[CH3] =
          (((sbus_rcv_data[(sbus_data_head+3)%SBUS_DMA_SIZE] >> 6) + (sbus_rcv_data[(sbus_data_head+4)%SBUS_DMA_SIZE] << 2) + (sbus_rcv_data[(sbus_data_head+5)%SBUS_DMA_SIZE] << 10)) & 0x07ff);
      SBUS.sbus_data[CH4] = (((sbus_rcv_data[(sbus_data_head+5)%SBUS_DMA_SIZE] >> 1) + (sbus_rcv_data[(sbus_data_head+6)%SBUS_DMA_SIZE] << 7)) & 0x07ff);
      SBUS.sbus_data[CH5] = (((sbus_rcv_data[(sbus_data_head+6)%SBUS_DMA_SIZE] >> 4) + (sbus_rcv_data[(sbus_data_head+7)%SBUS_DMA_SIZE] << 4)) & 0x07ff);
      SBUS.sbus_data[CH6] =
          (((sbus_rcv_data[(sbus_data_head+7)%SBUS_DMA_SIZE] >> 7) + (sbus_rcv_data[(sbus_data_head+8)%SBUS_DMA_SIZE] << 1) + (sbus_rcv_data[(sbus_data_head+9)%SBUS_DMA_SIZE] << 9)) & 0x07ff);
      SBUS.sbus_data[CH7] = (((sbus_rcv_data[(sbus_data_head+9)%SBUS_DMA_SIZE] >> 2) + (sbus_rcv_data[(sbus_data_head+10)%SBUS_DMA_SIZE] << 6)) & 0x07ff);
      SBUS.sbus_data[CH8] = (((sbus_rcv_data[(sbus_data_head+10)%SBUS_DMA_SIZE] >> 5) + (sbus_rcv_data[(sbus_data_head+11)%SBUS_DMA_SIZE] << 3)) & 0x07ff);
      SBUS.sbus_data[CH9] = (((sbus_rcv_data[(sbus_data_head+12)%SBUS_DMA_SIZE]) + (sbus_rcv_data[(sbus_data_head+13)%SBUS_DMA_SIZE] << 8)) & 0x07ff);
      SBUS.sbus_data[CH10] = (((sbus_rcv_data[(sbus_data_head+13)%SBUS_DMA_SIZE] >> 3) + (sbus_rcv_data[(sbus_data_head+14)%SBUS_DMA_SIZE] << 5)) & 0x07ff);
      SBUS.sbus_data[CH11] =
          (((sbus_rcv_data[(sbus_data_head+14)%SBUS_DMA_SIZE] >> 6) + (sbus_rcv_data[(sbus_data_head+15)%SBUS_DMA_SIZE] << 2) + (sbus_rcv_data[(sbus_data_head+16)%SBUS_DMA_SIZE] << 10)) &
           0x07ff);
      SBUS.sbus_data[CH12] = (((sbus_rcv_data[(sbus_data_head+16)%SBUS_DMA_SIZE] >> 1) + (sbus_rcv_data[(sbus_data_head+17)%SBUS_DMA_SIZE] << 7)) & 0x07ff);
      SBUS.sbus_data[CH13] = (((sbus_rcv_data[(sbus_data_head+17)%SBUS_DMA_SIZE] >> 4) + (sbus_rcv_data[(sbus_data_head+18)%SBUS_DMA_SIZE] << 4)) & 0x07ff);
      SBUS.sbus_data[CH14] =
          (((sbus_rcv_data[(sbus_data_head+18)%SBUS_DMA_SIZE] >> 7) + (sbus_rcv_data[(sbus_data_head+19)%SBUS_DMA_SIZE] << 1) + (sbus_rcv_data[(sbus_data_head+20)%SBUS_DMA_SIZE] << 9)) & 0x07ff);
      SBUS.sbus_data[CH15] = (((sbus_rcv_data[(sbus_data_head+20)%SBUS_DMA_SIZE] >> 2) + (sbus_rcv_data[(sbus_data_head+21)%SBUS_DMA_SIZE] << 6)) & 0x07ff);
      SBUS.sbus_data[CH16] = (((sbus_rcv_data[(sbus_data_head+21)%SBUS_DMA_SIZE] >> 5) + (sbus_rcv_data[(sbus_data_head+22)%SBUS_DMA_SIZE] << 3)) & 0x07ff);
      SBUS.sbus_data[CH17] = (sbus_rcv_data[(sbus_data_head+23)%SBUS_DMA_SIZE] & 0x01) * (unsigned short)672 * 2 + (unsigned short)352;
      SBUS.sbus_data[CH18] =
          ((sbus_rcv_data[(sbus_data_head+23)%SBUS_DMA_SIZE] >> 1) & 0x01) * (unsigned short)672 * 2 + (unsigned short)352;
      
      
      SBUS.sbus_float[CH1] = ((SBUS.sbus_data[CH1] - 352.0f)/672.0f*100.0f - 100.0f) + 0.5952;
      SBUS.sbus_float[CH2] = (SBUS.sbus_data[CH2] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH3] = -((SBUS.sbus_data[CH3] - 352.0f)/672.0f*100.0f - 100.0f);
      SBUS.sbus_float[CH4] = (SBUS.sbus_data[CH4] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH5] = (SBUS.sbus_data[CH5] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH6] = (SBUS.sbus_data[CH6] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH7] = (SBUS.sbus_data[CH7] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH8] = (SBUS.sbus_data[CH8] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH9] = (SBUS.sbus_data[CH9] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH10] = (SBUS.sbus_data[CH10] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH11] = (SBUS.sbus_data[CH11] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH12] = (SBUS.sbus_data[CH12] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH13] = (SBUS.sbus_data[CH13] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH14] = (SBUS.sbus_data[CH14] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH15] = (SBUS.sbus_data[CH15] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH16] = (SBUS.sbus_data[CH16] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH17] = (SBUS.sbus_data[CH17] - 352.0f)/672.0f*100.0f - 100.0f;
      SBUS.sbus_float[CH18] = (SBUS.sbus_data[CH18] - 352.0f)/672.0f*100.0f - 100.0f;

      SBUS.frame_lost = (sbus_rcv_data[(sbus_data_head+23)%SBUS_DMA_SIZE] >> 2) & 0x01;
      SBUS.fail_safe = (sbus_rcv_data[(sbus_data_head+23)%SBUS_DMA_SIZE] >> 3) & 0x01;
               
      sbus_data_head = (sbus_data_head + 24)%SBUS_DMA_SIZE;
    } else {
      sbus_data_head = (sbus_data_head + 1)%SBUS_DMA_SIZE;
    }
    printf("%.4f %.4f %.4f %.4f\r\n", SBUS.sbus_float[CH1], SBUS.sbus_float[CH2], SBUS.sbus_float[CH3], SBUS.sbus_float[CH4]);
  }  
  SBUS.decode_finish_flag = 1;
  SBUS.sbus_read_flag = 0;
}

uint16_t Check_data_size_sbus()
{ 
  if(sbus_data_tail >= sbus_data_head) {
    return sbus_data_tail-sbus_data_head;
  } else {
    return sbus_data_tail+SBUS_DMA_SIZE-sbus_data_head;
  }  
}

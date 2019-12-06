#include "main.h"
#include "stdio.h"
#include "stm32f7xx_it.h"
#include "usart.h"

#define STX   0x02
#define ETX     0x03
#define START    0x53
#define END    0x54

#define XBEE_SNED_PAKET_SIZE 19
#define XBEE_RCV_PAKET_SIZE 6
#define ERROR_TYPE      0x59

uint8_t xbee_rcv_data[100] = { NULL, };
uint8_t xbee_rcv_check_sum = 1;
uint8_t error_type = 0x59;
uint8_t xbee_decode_flag = 0;
uint8_t xbee_rqeuset_flag = 0;
uint8_t xbee_rcv_idx = 0;

uint8_t xbee_gain_send[9];

uint8_t xbee_type[2] = { 0x4a, 0x4b };
uint8_t xbee_check_sum;
uint8_t xbee_paket[16];
uint8_t xbee_idx;


void Send_xbee() {
  xbee_idx = 3;
  
  xbee_paket[0] = SENSOR.alt[0] / 1;
  xbee_paket[1] = ((SENSOR.alt[0] - (SENSOR.alt[0] / 1)) * 10) / 1;
  xbee_paket[2] = SENSOR.rate[0] / 1;
  xbee_paket[3] = ((SENSOR.rate[0] - (SENSOR.rate[0] / 1)) * 10) / 1;
  xbee_paket[4] = SENSOR.rate[1] / 1;
  xbee_paket[5] = ((SENSOR.rate[1] - (SENSOR.rate[1] / 1)) * 10) / 1;
  xbee_paket[6] = SENSOR.gps[0] / 1;
  xbee_paket[7] = ((SENSOR.gps[0] - (SENSOR.gps[0] / 1)) * 100) / 1;
  xbee_paket[8] = (((SENSOR.gps[0] - (SENSOR.gps[0] / 1)) * 10000) - (xbee_paket[7] * 100)) / 1;
  xbee_paket[9] = (((SENSOR.gps[0] - (SENSOR.gps[0] / 1)) * 1000000) - ((xbee_paket[7] * 10000) + xbee_paket[8] * 100)) / 1;
  xbee_paket[10] = SENSOR.gps[1] / 1;
  xbee_paket[11] = ((SENSOR.gps[1] - (SENSOR.gps[1] / 1)) * 100) / 1;
  xbee_paket[12] = (((SENSOR.gps[1] - (SENSOR.gps[1] / 1)) * 10000) - (xbee_paket[11] * 100)) / 1;
  xbee_paket[13] = (((SENSOR.gps[1] - (SENSOR.gps[1] / 1)) * 1000000) - ((xbee_paket[11] * 10000) + xbee_paket[12] * 100)) / 1;
  

  xbee_send_data[0] = STX;
  
  xbee_send_data[1] = HEAD;
  
  xbee_send_data[2] = xbee_type[0];
  
  for (int i=0; i<14; i++) {
    xbee_send_data[xbee_idx] = xbee_paket[i];
    xbee_idx++;
  }
  
  xbee_send_data[XBEE_SEND_PAKET_SIZE-1] = END;
  
  xbee_send_data[XBEE_SEND_PAKET_SIZE] = ETX;
  
  HAL_UART_Transmit(&huart4, xbee_send_data, XBEE_SEND_PAKET_SIZE, 10);
}


void Send_gain_xbee() {
  xbee_send_data[0] = STX;
  xbee_send_data[1] = START;
  
  xbee_send_data[2] = xbee_type[1];
  
  xbee_send_data[3] = GAIN.roll[0] / 1;
  xbee_send_data[4] = ((GAIN.roll[0] - xbee_send_data[3]) * 100) / 1;
  xbee_send_data[5] = GAIN.roll[1] / 1;
  xbee_send_data[6] = ((GAIN.roll[1] - xbee_send_data[5]) * 100) / 1;
  xbee_send_data[7] = GAIN.roll[2] / 1;
  xbee_send_data[8] = ((GAIN.roll[2] - xbee_send_data[7]) * 100) / 1;
  xbee_send_data[9] = GAIN.pitch[0] / 1;
  xbee_send_data[10] = ((GAIN.pitch[0] - xbee_send_data[9]) * 100) / 1;
  xbee_send_data[11] = GAIN.pitch[1] / 1;
  xbee_send_data[12] = ((GAIN.pitch[1] - xbee_send_data[11]) * 100) / 1;
  xbee_send_data[13] = GAIN.pitch[2] / 1;
  xbee_send_data[14] = ((GAIN.pitch[2] - xbee_send_data[13]) * 100) / 1;
  xbee_send_data[15] = GAIN.yaw[0] / 1;
  xbee_send_data[16] = ((GAIN.yaw[0] - xbee_send_data[15]) * 100) / 1;
  xbee_send_data[17] = GAIN.yaw[1] / 1;
  xbee_send_data[18] = ((GAIN.yaw[1] - xbee_send_data[17]) * 100) / 1;
  xbee_send_data[19] = GAIN.yaw[2] / 1;
  xbee_send_data[20] = ((GAIN.yaw[2] - xbee_send_data[19]) * 100) / 1;
  
  xbee_send_data[21] = END;
  xbee_send_data[22] = ETX;
  
  HAL_UART_Transmit(&huart4, xbee_send_data, 22, 10);
}


void Rcv_xbee() {
  HAL_UART_Receive_IT(&huart4, xbee_rcv_data, 62);
}


void Decoding_xbee() {
  for (int i=1; xbee_rcv_data[i] != NULL; i+=6) {
    for (int j=0; j<XBEE_RCV_PAKET_SIZE; j++) {
      if (xbee_rcv_data[i+1] == 0x52) {
        Send_gain_xbee();
        return;
      }

      xbee_rcv_check_sum += xbee_rcv_data[i+j];
    }
    
    xbee_rcv_check_sum &= 0xff;
    
    if (xbee_rcv_check_sum != 0) {
      xbee_send_data[0] = STX; 
      xbee_send_data[1] = START;
      xbee_send_data[2] = ERROR_TYPE;
      xbee_send_data[3] = 0x00;
      xbee_send_data[4] = 0x00;
      xbee_send_data[5] = END;
      xbee_send_data[6] = ETX;
      
      HAL_UART_Transmit(&huart4, xbee_send_data, 7, 10);
      
      xbee_decode_flag = 1;
      break;
    } else {
      xbee_decode_flag = 0;
    }
  }
  
  if (xbee_decode_flag == 0) {
    GAIN.roll[KP] = xbee_rcv_data[3] + (xbee_rcv_data[4] * 1/100);
    GAIN.roll[KI] = xbee_rcv_data[9] + (xbee_rcv_data[10] * 1/100);
    GAIN.roll[KD] = xbee_rcv_data[15] + (xbee_rcv_data[16] * 1/100);
    GAIN.pitch[KP] = xbee_rcv_data[21] + (xbee_rcv_data[22] * 1/100);
    GAIN.pitch[KI] = xbee_rcv_data[27] + (xbee_rcv_data[28] * 1/100);
    GAIN.pitch[KD] = xbee_rcv_data[33] + (xbee_rcv_data[34] * 1/100);
    GAIN.yaw[KP] = xbee_rcv_data[39] + (xbee_rcv_data[40] * 1/100);
    GAIN.yaw[KI] = xbee_rcv_data[45] + (xbee_rcv_data[46] * 1/100);
    GAIN.yaw[KD] = xbee_rcv_data[51] + (xbee_rcv_data[52] * 1/100);
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART4)
  { 
      Decoding_xbee();  
      Rcv_xbee();
  }
}
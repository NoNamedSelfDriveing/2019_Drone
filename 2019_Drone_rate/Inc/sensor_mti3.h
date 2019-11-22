#ifndef _SENSOR_MTI3_H_
#define _SENSOR_MTI3_H_

#include "headers.h"

#define SENSOR_DMA_SIZE 256
#define SENSOR_PACKET_SIZE 57 
#define GPS_PACKET_SIZE 75
#define DATASIZE 52

void Sensor_start();
void Sensor_read();
void Decoding_data();
void Dma_error_check();


#endif
#ifndef SENSOR_MTI3_H
#define SENSOR_MTI3_H

#include "headers.h"

#define SENSOR_DMA_SIZE 256
#define SENSOR_PACKET_SIZE 35
#define DATASIZE 12

void SensorStart();
void SensorRead();
void DecodingData();

#endif
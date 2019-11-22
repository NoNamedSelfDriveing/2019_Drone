#ifndef SBUS_H
#define SBUS_H

#include "headers.h"

#define SBUS_DMA_SIZE 256
#define SBUS_PACKET_SIZE 25
#define START_BYTE      0x0f

#define CH1     0
#define CH2     1
#define CH3     2
#define CH4     3
#define CH5     4
#define CH6     5
#define CH7     6
#define CH8     7
#define CH9     8
#define CH10     9
#define CH11     10
#define CH12     11
#define CH13     12
#define CH14     13
#define CH15     14
#define CH16     15
#define CH17     16
#define CH18     17

void Sbus_init();
void Sbus_start();
void Sbus_decode();
void Sbus_read();

#endif
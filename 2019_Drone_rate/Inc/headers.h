#ifndef _HEADERS_H_
#define _HEADERS_H_

#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "stdio.h"
#include "string.h"

#include "stm32f7xx_it.h"

#include "define.h"
#include "main_process.h"
#include "sbus.h"
#include "sensor_mti3.h"
#include "pid_control.h"
#include "mixer.h"
#include "xbee.h"

extern _SBUS SBUS;
extern _SENSOR SENSOR;
extern _RPY RPY;
extern _GAIN GAIN;

#endif
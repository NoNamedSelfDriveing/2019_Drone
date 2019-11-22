#ifndef _DEFINE_H_
#define _DEFINE_H_

typedef enum {
  ROLL = 0,
  PITCH,
  YAW
} rpy;

typedef enum {
  P = 0,
  I,
  D
} pid;

typedef enum {
  KP = 0,
  KI,
  KD
} gain;


#endif
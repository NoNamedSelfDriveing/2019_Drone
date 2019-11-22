#ifndef _STRUCT_H_
#define _STRUCT_H_

typedef struct {
  float rate[3]; //current drone`s rpy rate
  float angle[3]; //current drone`s rpy angle
} _SENSOR;

typedef struct {
  float angle[3]; //target drone`s angle
  float throttle; //target motor speed
} _SBUS;

typedef struct {
  float control_cmd[3]; //Rpy pid control value
  float control[3]; //p, i, d control value
  float gain[3]; //kp, ki, kd
} _RPY;

#endif
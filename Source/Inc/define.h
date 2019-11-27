#ifndef _DEFINE_H_
#define _DEFINE_H_


typedef enum {
  YAW= 0,
  PITCH,
  THROTTLE,
  ROLL
} e_sbus_rpy;

typedef enum {
  Roll = 0,
  Pitch,
  Yaw
} e_rpy;

typedef enum {
  P = 0,
  I,
  D
} e_pid;

typedef enum {
  KP = 0,
  KI,
  KD
} e_gain;

typedef struct {
  float rate[3]; //current drone`s rpy angle, gyrX[0], gyrY[1], gyrZ[2]
  float angle[3]; //current drone`s rpy rate, roll[0], pitch[1], yaw[2]
  float acc[3]; //current drone`s acceleration, accX[0], accY[1], accZ[2]
  float gps[2]; //current drone's gps, lat[0], lon[1]
  float alt[1]; //current drone's altitude, alt[0]
} _SENSOR;

typedef struct {
  float control_cmd[3]; //Rpy pid control value
  float control[3]; //p, i, d control value
  //float gain[3]; //kp, ki, kd
} _RPY;

typedef struct {
  float roll[3];
  float pitch[3];
  float yaw[3];
} _GAIN;

typedef struct
{
  uint8_t  decode_finish_flag;
  uint8_t  sbus_read_flag;
  uint16_t sbus_data[18];
  float set_point[18];//0~3 controller value 0: Yaw, 1: pitch, 2: throttle, 3: roll    (-100 ~ +100)
  
  uint8_t frame_lost;
  uint8_t fail_safe;
} _SBUS;


#endif
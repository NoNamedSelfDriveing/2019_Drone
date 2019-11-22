#include "pid_control.h"
#define DT 0.001

void Init_gain_value(){
   GAIN.roll[KP] = 1.0f;
   GAIN.roll[KI] = 0.0f;
   GAIN.roll[KD] = 10.0f;
   
   GAIN.pitch[KP] = 1.0f;
   GAIN.pitch[KI] = 0.0f;
   GAIN.pitch[KD] = 10.0f;
     
   GAIN.yaw[KP] = 1.0f;    
}

void Calc_pid_control(){
  float error[3];
  static float integral[3] = {0,};
  
  //Calculate error
  error[Roll] = SBUS.set_point[ROLL] - SENSOR.angle[Roll];
  error[Pitch] = SBUS.set_point[PITCH] - SENSOR.angle[Pitch];
  error[Yaw] = SBUS.set_point[YAW] - SENSOR.rate[Yaw];
  
  //PID control - Roll
  RPY.control[P] = GAIN.roll[KP] * error[Roll];
  
  integral[Roll] = integral[Roll] + error[Roll] * DT;
  RPY.control[I] = GAIN.roll[KI] * integral[Roll];
  
  RPY.control[D] = GAIN.roll[KD] * (- SENSOR.rate[Roll]); 
  
  RPY.control_cmd[Roll] = RPY.control[P] + RPY.control[I] + RPY.control[D];
 
  //PID control - Pitch
  RPY.control[P] = GAIN.pitch[KP] * error[Pitch];
  
  integral[Pitch] = integral[Pitch] + error[Pitch] * DT;
  RPY.control[I] = GAIN.pitch[KI] * integral[Pitch]; 
  
  RPY.control[D] = GAIN.pitch[KD] * (-SENSOR.rate[Pitch]);
  
  RPY.control_cmd[Pitch] = RPY.control[P] + RPY.control[I] + RPY.control[D];
  
  //P control - Yaw
  RPY.control[P] = GAIN.yaw[KP] * error[Yaw]; 
  RPY.control_cmd[Yaw] = RPY.control[P];  

  //printf("%.4f\r\n", RPY.control_cmd[Yaw]);
  //if drone is on ground, integral init
  if(SBUS.set_point[THROTTLE] < -95.0f)
  {
    integral[Roll] = 0.0f;
    integral[Pitch] = 0.0f;
  }
}
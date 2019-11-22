#include "pid_control.h"

void Init_gain_value(){
   RPY.gain[KP] = 1.0;
   RPY.gain[KI] = 0.0;
   RPY.gain[KD] = 0.0;    
}

void Calc_pid_control(){

  RPY.control[P] = RPY.gain[KP] * (SBUS.angle[ROLL] - SENSOR.angle[ROLL]);
  RPY.control[I] = RPY.gain[KI] * (- SENSOR.rate[ROLL]);
  RPY.control[D] = 0.0 * (SBUS.angle[ROLL] - SENSOR.angle[ROLL]);

  RPY.control_cmd[ROLL] = RPY.control[P] + RPY.control[I] + RPY.control[D];
  
  
  
  RPY.control[P] = RPY.gain[KP] * (SBUS.angle[PITCH] - SENSOR.angle[PITCH]);
  RPY.control[I] = RPY.gain[KI] * (- SENSOR.rate[PITCH]);
  RPY.control[D] = RPY.gain[KD] * (SBUS.angle[PITCH] - SENSOR.angle[PITCH]);

  RPY.control_cmd[PITCH] = RPY.control[P] + RPY.control[I] + RPY.control[D];
  
  

  RPY.control[P] = RPY.gain[KP] * (SBUS.angle[YAW] - SENSOR.angle[YAW]);
  RPY.control[I] = RPY.gain[KI] * (- SENSOR.rate[YAW]);
  RPY.control[D] = RPY.gain[KD] * (SBUS.angle[YAW] - SENSOR.angle[YAW]);
  
  RPY.control_cmd[YAW] = RPY.control[P] + RPY.control[I] + RPY.control[D];
  
}
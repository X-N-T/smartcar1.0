#ifndef _INDUCTANCE_H
#define _INDUCTANCE_H


#include "headfile.h"


#define Servo_pwm_mid   740      //67 //////////////////////////////////修改736
#define Servo_pwm_max   812      //75
#define Servo_pwm_min   668 

extern uint8   Indu_0           ; //离开赛道
extern uint16  Servo_PWM_a      ;

//int ad_ave(ADCn_Ch adcn_ch, ADC_nbit bit,int N);
void _adc(void);
extern void _adc_direction(void);

#endif
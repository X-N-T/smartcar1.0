#ifndef _lcd_h
#define _lcd_h
extern  int16    SpeedSet               ;
extern  int16    LSpeedSet              ;
extern  int16    RSpeedSet              ;
extern  int16    GetRightMotorPules, GetLeftMotorPules   ; 
extern  int16    MotorPwmRight          ;
extern  int16    MotorPwmLeft           ;
extern  uint8    screen_flag            ;
extern  uint8    road_type              ;
extern  uint8    Track_Line_Num         ;
extern  uint8    s                      ;
extern  uint8    hw_flag                ;
extern  float    Differential_P         ;
extern  float    Error                  ;
extern  float    Gyro_Accumulate        ;
extern  int32    SUM_PulseFTM           ;
extern  int16    Fictitious_Beeline_Num ;
extern  uint8   i_R                     ;
extern  uint8   j_L                     ;


void compressed_image_display();
void midline_display();
void midline_display1();
void midline_display2();
void midline_display3();
extern uint8 image_display[120][188];
void straightline_counting_point();
void lcd_show_PID();
void show_motor();
void show_flags();
void show_ramp();
void data_choose_float(int x,int y,double data_serial,int num,int pointnum,float data);
void data_choose_uint16(int x,int y,int data_serial,float data);
#endif
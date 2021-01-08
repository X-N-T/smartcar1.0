#include "headfile.h"

uint8 image_display[120][188];



/***************************
º¯Êý£ºÏßÏÔÊ¾
IN:
OUT£º
****************************/
void midline_display()
{
  int i=0;
  for( i=1;i<60;i=i+2)
  {
    ips200_drawpoint (Mid_Line[i]+10,i+10,RED);
    ips200_drawpoint (Left_Add_Line[i]+10,i+10,BLUE);
    ips200_drawpoint (Right_Add_Line[i]+10,i+10,BLACK);
  }

} 
void midline_display1()
{ 
for(int i=1;i<60;i++)
  {
    ips200_drawpoint (Mid_line[i]+10,i+10,RED);
    ips200_drawpoint (L_verge[i]+10,i+10,BLUE);
    ips200_drawpoint (R_verge[i]+10,i+10,YELLOW);
  }
}
void midline_display2()
{
  int i=0;
  for( i=1;i<60;i=i+2)
  {
    ips200_drawpoint (Mid_Line[i]+10,i+10,RED);
    ips200_drawpoint (Left_Line[i]+10,i+10,BLUE);
     ips200_drawpoint (Right_Line[i]+10,i+10,GREEN);
  }

} 

//void midline_display3()
//{
//  int i=0;
//  for(i=59;i>40;i-=2)
//  {
//    ips200_drawpoint (Mid_ramp_line[i]+10,i+10,RED);
//    ips200_drawpoint (Left_ramp_Line[i]+10,i+10,BLUE);
//    ips200_drawpoint (Right_ramp_Line[i]+10,i+10,GREEN);
//  }
//
//} 

/*****************************************

*******************************************/


void lcd_show_PID()
{
  ips200_displayimage032_zoom1(img[0], 94, 60, 10, 10, 94, 60);
  
  ips200_showstr(0,6,"PID",BLACK);
  ips200_showstr(80,7,"P",BLACK);
  ips200_showstr(130,7,"I",BLACK);
  ips200_showstr(180,7,"D",BLACK);
  ips200_showstr(0,8,"Speed",BLACK);
  ips200_showstr(0,9,"C_S",BLACK);
  ips200_showstr(0,10,  "speed_low",BLACK);
  ips200_showstr(0,11,  "speed_high",BLACK);
  ips200_showstr(0,12,  "Speed_choose",BLACK);
  ips200_showstr(0,14,  "Ending_speed",BLACK);
  ips200_showstr(0,15,  "straight_value1",BLACK);
  ips200_showstr(0,16,  "straight_value2",BLACK);
  
  data_choose_float(70,8,0,1,2,key_show_PID[0]*0.01f);
  data_choose_float(120,8,1,1,2,key_show_PID[1]*0.01f);
  data_choose_float(170,8,2,1,2,key_show_PID[2]*0.01f);
  data_choose_float(80,9,3,1,2,C_S);
  data_choose_float(170,9,4,1,2,key_show_PID[4]*0.01f);
  data_choose_uint16(180,10,5,speed_low);
  data_choose_uint16(180,11,6,speed_high);
  data_choose_uint16(180,12,7,Speed_choose);
  data_choose_uint16(20,13,8,round_left_value);
  data_choose_uint16(180,13,9,round_right_value);
  data_choose_uint16(180,14,10,Ending_speed);
  data_choose_uint16(180,15,11,straight_value1);
  data_choose_uint16(180,16,12,straight_value2);
//
//    ips200_showint16(180,1,garage_enter ,RED);
//            float g_fSlope0 = 0;
//            float g_fSlope1 = 0;
//            float g_fSlope2 = 0;
//            float g_fSlope3 = 0;
//            float g_fSlope4 = 0;
//            g_fSlope0 = 1.0*(Right_Line[49] - Right_Line[45])/4;
//            g_fSlope1 = 1.0*(Right_Line[47] - Right_Line[43])/4;
//            g_fSlope2 = 1.0*(Right_Line[45] - Right_Line[41])/4;
//            g_fSlope3 = 1.0*(Right_Line[43] - Right_Line[39])/4;
//            g_fSlope4 = 1.0*(Right_Line[41] - Right_Line[37])/4;
//            
//               ips200_showfloat(120,1,g_fSlope0,2,3,RED);
//               ips200_showfloat(120,2,g_fSlope1,2,3,RED);
//               ips200_showfloat(120,3,g_fSlope2,2,3,RED);
//               ips200_showfloat(120,4,g_fSlope3,2,3,RED);
//               ips200_showfloat(120,5,g_fSlope4,2,3,RED);
        if(screen_flag==1)
   {        
               ips200_showstr(150,0,"BlackThres",BLACK);
               ips200_showint16(160,1,BlackThres ,RED);
               
              screen_flag=0; 
   }      
}

void show_motor()
{
  ips200_displayimage032_zoom1(image_myself[0], 94, 60, 10, 10, 94, 60);
   if(screen_flag==1)
   {
     
    ips200_showint16(180,5, SpeedSet,RED);
    ips200_showint16(180,6, LSpeedSet,RED);
    ips200_showint16(180,7, RSpeedSet,RED);
    ips200_showint16(180,8, GetLeftMotorPules,RED);
    ips200_showint16(180,9, GetRightMotorPules,RED);
    ips200_showint16(180,10, MotorPwmRight,RED);
    ips200_showint16(180,11, MotorPwmLeft,RED);
    ips200_showint8(180,12, Mode,RED);
    ips200_showint16(180,13, s,RED);//straight
    ips200_showint16(180,14, straight,RED);
    ips200_showint16(180,15, SUM_PulseFTM,RED);
    ips200_showint16(180,16, Line_Count,RED);
    
    ips200_showstr(0,5,  "SpeedSet",BLACK);
    ips200_showstr(0,6,  "LSpeedSet",BLACK);
    ips200_showstr(0,7,  "RSpeedSet",BLACK);
    ips200_showstr(0,8, "Left encoder",BLACK);
    ips200_showstr(0,9, "Right encoder",BLACK);
    ips200_showstr(0,10, "MotorPwmRight",BLACK);
    ips200_showstr(0,11, "MotorPwmLeft",BLACK);
    ips200_showstr(0,12, "Mode",BLACK);
    ips200_showstr(0,13, "s",BLACK);
    ips200_showstr(0,14, "straight",BLACK);//SUM_PulseFTM
    ips200_showstr(0,15, "SUM_PulseFTM",BLACK);
    ips200_showstr(0,16, "Line_Count",BLACK);
    
    screen_flag=0;
   }
}

void show_flags()
{
  ips200_displayimage032_zoom1(image_myself[0], 94, 60, 10, 10, 94, 60);
   if(screen_flag==1)
   {
                    
                  ips200_showstr(160,16,"flaaaaag",BLACK);
                  ips200_showint8(160,17,flaaaaag,RED);
                    
                  ips200_showstr(160,6,"L_straight",BLACK);
                  ips200_showint8(160,7,L_straight,BLUE);
                  
                  ips200_showstr(160,8,"R_straight",BLACK);
                  ips200_showint8(160,9,R_straight,BLUE);              
                  
                  ips200_showstr(160,10,"LastLine",BLACK);
                  ips200_showint8(160,11,LastLine,BLUE); 
                  
                  ips200_showstr(160,12,"_x",BLACK);
                  ips200_showint8(160,13,find_left_point_x,BLUE); 
                  ips200_showstr(160,14,"_y",BLACK);
                  ips200_showint8(160,15,find_left_point_y,BLUE); 
                  
                  
                  ips200_showstr(0,6,"out_ring_num",BLACK);
                  ips200_showint8(100,6,out_ring_num,BLUE);
                  
                  ips200_showstr(0,5,"island_flag",BLACK);
                  ips200_showint8(100,5,island_flag,BLUE);
                  
                  ips200_showstr(0,8,"is_diu_flag",BLACK);
                  ips200_showint8(100,8,is_diu_flag,BLUE);
                  
                  ips200_showstr(0,9,"out_ring_flag",BLACK);
                  ips200_showint8(100,9,out_ring_flag,BLUE);
                  
                  
                  ips200_showstr(0,7,"Ramp_Flag",BLACK);
                  ips200_showint8(100,7,Ramp_Flag,RED); 
                  
                  ips200_showstr(0,10,"BlackEndM",BLACK);
                  ips200_showint8(100,10,BlackEndM,RED);
                  
                  ips200_showstr(0,11,"Error",BLACK);
                  ips200_showfloat(100,11,Error,2,3,RED);
                  
                  ips200_showstr(0,12,"Left_Error_Sum",BLACK);
                  ips200_showint16(100,12,Left_Error_Sum,RED);
                  
                  ips200_showstr(0,13,"Right_Error_Sum",BLACK);
                  ips200_showint16(100,13,Right_Error_Sum,RED);
                  
                  ips200_showstr(0,14,"Foresight",BLACK);
                  ips200_showint8(100,14,Foresight,RED);
                  
                  ips200_showstr(0,16,"tuqi_point_hang",BLACK);
                  ips200_showint8(100,16,tuqi_point_hang,BLUE);
                  
                  ips200_showstr(0,17,"road_type",BLACK);
                  ips200_showint8(100,17,road_type,BLUE);
                  
                  ips200_showstr(0,18,"Camera_error",BLACK);
                  ips200_showint8(100,18,Camera_error,BLUE);
                  
                  ips200_showstr(0,19,"Track_Line_Num",BLACK);
                  ips200_showint8(100,19,Track_Line_Num,BLUE);

                
   screen_flag=0;
   }
}

void show_ramp()
{
   ips200_displayimage032_zoom1(image_garage[0], 94, 60, 10, 10, 94, 60);
  if(screen_flag==1)
   {
     for(int i=0;i<=10;i++)
     ips200_showint8(110,i,Ramp_Error[i],RED);
     
     ips200_showint16(110,11,Ramp_Error_Sum,BLUE);
     ips200_showint16(110,13,hw_flag,GREEN);
     
     ips200_showuint8(100,12,sharp_font,GREEN);//Gyro_Accumulate
     
     ips200_showuint8(100,15,j_L,GREEN);
     ips200_showuint8(100,16,i_R,GREEN);
     
   screen_flag=0;
   }
}

void data_choose_float(int x,int y,double data_serial,int num,int pointnum,float data)
{
              if(change_flag == data_serial)
              {
                 ips200_showfloat(x,y,data,2,2,BLUE);
              }
           else
              {
                 ips200_showfloat(x,y,data,2,2,RED); 
              }        
}

void data_choose_uint16(int x,int y,int data_serial,float data)
{
 
           if(change_flag == data_serial)
              {
                 ips200_showuint16(x,y,data,BLUE);
              }
           else
              {
                 ips200_showuint16(x,y,data,RED); 
              }        
    
}

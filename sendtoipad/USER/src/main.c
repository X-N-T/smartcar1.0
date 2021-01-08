
#include "headfile.h"
#include "math.h"

#define Servo_pwm_mid   740
#define Servo_pwm_max   812
#define Servo_pwm_min   668
//#define LENGTH  200
//#define WIDTH   154
#define RATIO   1.62f
//#define mpu_gyro_x0             11


//uint8   Threshold = 155  ;       /**********************************155********************************/



float   S_Kp = 0.0;
float   S_Kd = 0.0;
float   C_S  = 0.0;         
float   S_d  = 0.0;

float   Error           = 0.0   ;
float   LastError       = 0.0   ;
float   Delta_Error     = 0.0   ;
int     SpeedDropRow    = 0     ;
int     LastSpeedDropRow        ;

uint16  Servo_PWM_i     = 0     ;
//uint16  Servo_PWM_a   = 0     ;
uint16  Servo_PWM       = 0     ;
float   tjsl            =0.0    ;
uint8   hw_flag         = 0     ;
uint8   Dynamic_Point_Num = 0   ;
//int16   valu_in[4] = {0}        ;	//获取的电感值
////int16   sharp_font, sharp_left  ;//sharp
//float   inductance[2]           ;


uint8   outbound_logo           = 0 ; //出界

uint8   straight                = 0 ; //长直道

uint8   endianci                = 0 ; //1---切换电磁    0----摄像头
int     straight_count          = 0 ; //长直道计数


int16   NumPulseFTM, NumPulselptmr              ;//脉冲计数寄存器的值

int16   GetRightMotorPules, GetLeftMotorPules   ; //获得的一次脉冲总数（用于PID电机的数据处理）
int16   Ave_PulseFTM                           ;
int32   SUM_PulseFTM                           ;
uint8   a_ = 1                                  ;

int16   LSpeedSet       = 0     ;
int16   RSpeedSet       = 0     ;
int16   SpeedSet        = 0     ;
int16   dut             = 0     ;
int     men             = 0     ;
float   Differential_P  = 0     ;
float   tan_reg         = 0     ;

int16   SpeedErrorL     = 0     ;
int16   SpeedErrorR     = 0     ;
int16   SpeedLastErrorL = 0     ;
int16   SpeedLastErrorR = 0     ;
int16   SpeedPerErrorL  = 0     ;
int16   SpeedPerErrorR  = 0     ;

int16   MotorPwmRight   = 0     ;
int16   MotorPwmLeft    = 0     ;

int16  Fictitious_Beeline_Num=0 ;
uint8  Track_Line_Num=0,Track_Line_Time=0;
float  Last_Error[5]            ;

float   SpeedP             = 0.0 ;
float   SpeedI             = 0.0 ; 
float   SpeedD             = 0.0 ; 

uint8   breaking_s      = 0     ;
uint8   ending_s        = 0     ;

//uint8   CrossFlag               = 0     ;//十字标志

uint8   start_fl = 0 ;////1 左出      2 右出    0 车库内



uint8   right_flag              = 0     ;
uint8   left_flag               = 0     ;
uint8   out_right_flag          = 0     ;
uint8   out_left_flag           = 0     ;


int     c_              = 0             ;
uint8   screen_flag=0;
uint8   road_type =0 ;
//uint8   decelerate_flag=0;
uint8 after_ramp_flag=0;

uint8 straight_value1=30;
uint8 straight_value2=15;

//unsigned int math_sqrt(unsigned int x)
//{
//
//    unsigned char ans=0,p=0x80;
//    while(p!=0)
//    {
//        ans+=p;
//        if(ans*ans>x)
//            ans-=p;
//        p=(unsigned char)(p/2);
//    }
//    return ans;
//}

/***********************************************************************/
///获得斜率   y1 为上端行数
//            y2 为下端行数
//            x2 为列数的数组 我这里用的是left_line[]和right_line[];
/**********************************************************************/
//
//double get_K(int y1, int y2, uint8_t* x2)//y1 < y2
//{
//    double sumk = 0;
//    uint8_t tx2 = *x2;
//    x2--;
//    for(int i = y2 - 1;i >= y1;i--)
//    {
//        if((y2 - i) != 0)
//        sumk += (double)(*x2 - tx2)/(y2 - i);
//        x2--;
//    }
//    if(y2 - y1 != 0)return sumk/(y2 - y1);
//    else return 0;
//}

void SpeedGet(void)
{
  NumPulselptmr = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0);
  qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0);   

  NumPulseFTM = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER2_C2);
  qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER2_C2);   
  
  GetRightMotorPules = NumPulseFTM ;
  GetLeftMotorPules = -NumPulselptmr ;
  
  /****************************      速度过滤      ****************************/  
  GetLeftMotorPules = (GetLeftMotorPules>400?400:GetLeftMotorPules); 
  GetRightMotorPules = (GetRightMotorPules>400?400:GetRightMotorPules);
  
  Ave_PulseFTM=(GetRightMotorPules + GetLeftMotorPules)/2;
}

void CalculateMotorSpeedError(int16 LeftMotorTarget,int16 RightMotorTarget)
{
    SpeedPerErrorL=SpeedLastErrorL;
    SpeedLastErrorL=SpeedErrorL;
    SpeedErrorL=LeftMotorTarget-GetLeftMotorPules;
    SpeedPerErrorR=SpeedLastErrorR;
    SpeedLastErrorR=SpeedErrorR ;
    SpeedErrorR=RightMotorTarget-GetRightMotorPules;
}

void Track_Line(void)
{
     Track_Line_Time++;
     if(Track_Line_Time==5)
     {
        Track_Line_Time=0;
        Last_Error[0] = Last_Error[1];
        Last_Error[1] = Last_Error[2];
        Last_Error[2] = Last_Error[3];
        Last_Error[3] = Last_Error[4];
        Last_Error[4] = fabs(Error);
     }
     
     if(Last_Error[0]>5 && Last_Error[1]>5 && Last_Error[2]>5 && Line_Count<25) //弯道入直道
     {
            Track_Line_Num = 1;
     }
     else if(Last_Error[0]<5 && Last_Error[1]<5 && Last_Error[2]<5 && BlackEndM==57) //直道
     {
            Track_Line_Num = 2;
     }
     else if(Last_Error[0]<5 && Last_Error[1]<5 && Last_Error[2]<5 && BlackEndM<=55) //直道入弯道
     {
            Track_Line_Num = 3;
     }
     else if(Delta_Error<1.5&&Last_Error[0]>8 && Last_Error[1]>8 && Last_Error[2]>8 && Last_Error[3]>8 && Last_Error[4]>8 && BlackEndM<=55) //弯道中
     {
            Track_Line_Num = 4;
     }
     else
            Track_Line_Num = 0;
}

/*
 *  @brief      // 0-1000反转
 */
//void Motor_fanzhuan(int fanzhuan)
//{
//    pwm_duty(PWM1_MODULE1_CHA_D14,0);
//    pwm_duty(PWM1_MODULE0_CHA_D12,10*fanzhuan);
//    pwm_duty(PWM1_MODULE0_CHB_D13,0);
//    pwm_duty(PWM1_MODULE1_CHB_D15,10*fanzhuan);
//}

//uint8 zhidao_ruwan_flag=0;

void helinsb()
{
//  float S_Error=0.0;
//  S_Error = fabs(Error);
//  if(S_Error>=20)
//    S_Error=20;
    

//  if(s>2)
//  {
//   SpeedI = 1.2;//1.14
//  }
     
  if(Out_Side==1&&sharp_font>=80)//(dark_flag==1||Indu_0==1)////////////////////////////////
    outbound_logo=1;
  if((outbound_logo==1)||(garage_enter==1))//||s>6)////(ending_flag==2)
  {
//    SpeedP = 6;//    SpeedI=0.6;//终点线，起跑//    SpeedD=2;
      SpeedSet = 0;
      if(ending_s>30)
      {
       /* pit_close(pit1);pit_close(pit2);*/
      }
  }
  else if(!start_fl)
    SpeedSet = 165 ;
  else if(ending_flag==2)
    SpeedSet = Ending_speed;//80 ;//
  else if(hw_flag==1||hw_flag==2)
  {
    SpeedSet = 150 ;
    //Ramp_Continue_Flag = 0;
  }
//---------------------入环减速，环中加速，出环减速----------------------------//
  else if(island_flag)
  {
    if(is_diu_flag == 1||is_diu_flag == 2)//环岛
    {
      SpeedSet = 180 ;//speed_low - 0;//-5
    }
    else if(is_diu_flag == 3||is_diu_flag == 4)
    {
      SpeedSet = 180 ;//speed_low - 0;
    }
    else if(out_ring_flag)
      SpeedSet = 180 ;//speed_low;
  }
  else if(straight)
  {
    if(straight==1)
      
      SpeedSet = speed_high + straight_value1 ;//250//25///
    else if(straight==2)
      SpeedSet = speed_high + straight_value2 ;//15  ;//230
    else if(straight==3)
      SpeedSet = speed_high + 5  ;//150
    
    if(straight_count>150)
    {
      if(straight_count > 300)
        straight_count = 300;
      //SpeedSet = SpeedSet+(straight_count-150)/2;
    }
  }
  else if(road_type)
    SpeedSet = speed_low-5; 
  else
  {
    if (Foresight >= 21)
      SpeedSet = speed_low; 
    else if (Foresight >= 18)
      SpeedSet = speed_low+2; 
    else if (Foresight >= 15)
      SpeedSet = speed_low+5; 
    else if (Foresight >= 13)
      SpeedSet = speed_low+7; 
    else if (Foresight >= 11)
      SpeedSet = speed_low+10; 
    else if (Foresight >= 9)
    {
//      SpeedSet -=0.7;
//      if(SpeedSet>((speed_high+speed_low)/2))
      SpeedSet = speed_low + 15;
    }
    else if (Foresight >= 6)
      SpeedSet = speed_low + 20;
    else
      SpeedSet = speed_high - 5;//+=0.7;

//     SpeedSet=(int16)((speed_high-speed_low)/400.0f*(20-S_Error)*(20-S_Error)+speed_low);//2;//Delta_Error
  }
//        if (Ramp_Time>Ramp_Delay)
//            {
//              Ramp_Flag=0;
//            }
//            if (Ramp_Time>250) //3S 后 开启坡道检测
//            {
//              Ramp_Time=0;
//              Ramp_Continue_Flag=1; //开启坡道检测
//            }
}

void SpeedSet_Control(void)
{
  helinsb();
//SpeedSet=100;/////////////
//  //********************设置反转力度**************//
//  if((outbound_logo==1)||ending_flag==2)
//  {
//    dut = (int)(Ave_PulseFTM*1.2);
//    men = -200;
//  }
////  else if(zhidao_ruwan_flag)
////  {
////    dut = 300;//最大3000
////    men = -40;
////  }
//  else if(Ramp_Flag)//||road_type==1)island_flag||
//  {
//    dut=250;
//    men=-50;
//  }
//  else if(road_type)
//  {
//    dut=300;
//    men=-40;
//  }
//  else if(Foresight>7)
//  {
//    dut=150;
//    men=-50;
//  }
//  else
//  {
//    dut=0;
//    men=-200;
//  }

       

//SpeedSet=(int)(SpeedSet-6*(57-BlackEndM)-10*(SpeedDropRow-LastSpeedDropRow)-(4*fabs(Error)+25*(Error-LastError))-0.3*magari);//2
//SpeedSet=(int16)(14*SpeedDropRow+40*(SpeedDropRow-LastSpeedDropRow)-(4*fabs(Error)+10*(Error-LastError)));

     LastSpeedDropRow=SpeedDropRow;
     SpeedDropRow=(int)(BlackEndM);
     if(SpeedDropRow>=55)
       SpeedDropRow=55;
     else if(SpeedDropRow<35)
       SpeedDropRow=35;//急弯减速很好
     


// SpeedSet=(int32)(14*SpeedDropRow+40*(SpeedDropRow-LastSpeedDropRow)-(4*fabs(Error)+10*(Error-LastError)));


//SpeedSet=(uint16)((speed_high-speed_low)/400.0*(20-S_Error)*(20-S_Error)+speed_low);//2
//SpeedSet=SpeedSet-(uint16)(20*(fabs(Error)-fabs(LastError)));
//SpeedSet=(int32)(6*(39-Foresight)+250);//(1.0*math_sqrt((39-Foresight)*(39-Foresight)*(39-Foresight))+250);//-4.5*(55-SpeedDropRow)+40*(SpeedDropRow-LastSpeedDropRow)-(4*fabs(Error)+25*(Error-LastError)));//2
    if((fabs(Error)>=4.5&&SpeedSet)||!start_fl)//9//&&SpeedDropRow<54
    {
       tan_reg = (Servo_PWM_i-Servo_pwm_mid)/128.57f;
       if(tan_reg<0)
       {
         LSpeedSet = SpeedSet;
         RSpeedSet = (int16)(SpeedSet*(1+C_S*tan_reg*0.385f)) ;
       }
       else
       {
         RSpeedSet = SpeedSet;//(1+RATIO*WIDTH*tan_reg*0.5f/LENGTH//tan_reg->Servo_PWM_i-Servo_pwm_mid)
         LSpeedSet = (int16)(SpeedSet*(1-C_S*tan_reg*0.385f));//0.385!!!
       }
       if(RSpeedSet<=50)    RSpeedSet=50;
       if(RSpeedSet>=400)    RSpeedSet=400;
       if(LSpeedSet<=50)    LSpeedSet=50;
       if(LSpeedSet>=400)    LSpeedSet=400;
    }
    else
    {
        LSpeedSet=SpeedSet ;
        RSpeedSet=SpeedSet ;
    }
}

void Motor_output_i(void)
{
  CalculateMotorSpeedError(LSpeedSet,RSpeedSet);//设定目标速度计算偏差
//  if(SpeedSet-Ave_PulseFTM<men)
//     Motor_fanzhuan(dut);
//  else
//  {
  
  
//      //不对称积分，输出量大于某值时，只积累负增加量
//    //有效防止出弯过度超调
//    if( motor_info->error[0] < 0 || NumPulseFTM > 990 )
//    {
//        if( motor_info->error[0] < 0 )
//        {
//            delta_uk += 1.05f * motor_info->ki * motor_info->error[0];
//        }
//    }
//    else
//    {
//        delta_uk += motor_info->ki * motor_info->error[0];
//    }
//    /*------------------------遇限削弱抗积分饱和等处理-----------------------*/
//    if( motor_info->last_uk > MOTOR_DUTY_MAX )
//        if( delta_uk > 0 )
//        {
//            delta_uk = 0;    //当前一时刻控制量(占空比)已经达到最大时，若现在增量为正则不累加
//        }
//        else if( motor_info->last_uk < MOTOR_DUTY_MIN )
//            if( delta_uk < 0 )
//            {
//                delta_uk = 0;    //当前一时刻控制量(占空比)已经达到最大时，若现在增量为负则不累加
//            }
//    /*------------------------遇限削弱抗积分饱和等处理结束-------------------*/
//    out_duty = motor_info->last_uk + delta_uk;//当前需要输出的实际占空比
  

    MotorPwmRight+=(int16)((SpeedP+SpeedI+SpeedD)*SpeedErrorR-(SpeedP+2*SpeedD)*SpeedLastErrorR+SpeedD*SpeedPerErrorR);

    MotorPwmLeft+=(int16)((SpeedP+SpeedI+SpeedD)*SpeedErrorL-(SpeedP+2*SpeedD)*SpeedLastErrorL+SpeedD*SpeedPerErrorL);

  if(MotorPwmLeft>=990)
    MotorPwmLeft=990;
  if(MotorPwmLeft<-990)
    MotorPwmLeft=-990;
  if(MotorPwmRight>=990)
    MotorPwmRight=990;
  if(MotorPwmRight<=-990)
    MotorPwmRight=-990;
  
   if(MotorPwmRight>0)
  {
    pwm_duty(PWM1_MODULE0_CHA_D12,0);
    pwm_duty(PWM1_MODULE1_CHA_D14,MotorPwmRight*10); 
  }
  else
  {
    pwm_duty(PWM1_MODULE1_CHA_D14,0);
    pwm_duty(PWM1_MODULE0_CHA_D12,-MotorPwmRight*10);
  }
   if(MotorPwmLeft>0)
  {
    pwm_duty(PWM1_MODULE1_CHB_D15,0);
    pwm_duty(PWM1_MODULE0_CHB_D13,MotorPwmLeft*10);
  }
  else
  {
    pwm_duty(PWM1_MODULE0_CHB_D13,0);
    pwm_duty(PWM1_MODULE1_CHB_D15,-MotorPwmLeft*10);
  }
//  }
}

void Dynamic_PID(void)
{
   if(Error>4.5&&Delta_Error>1.4&&BlackEndM<=55)////left 入左弯
     road_type=1;
//   else if(Error>0&&Delta_Error<-2)
//     road_type=0;
//   else if(Error<0&&Delta_Error>2)
//     road_type=0;
   else if(Error<-4.5&&Delta_Error<-1.4&&BlackEndM<=55)////right 入右弯&&(L_straight-R_straight)>6
     road_type=1;
   else
     road_type=0;
   
   
  if(!island_flag)//判断直道
  {
    if(BlackEndM>=53&&fabs(Error)<2&&L_straight>=52&&R_straight>=52)
      straight = 1;
    else if(BlackEndM>=52&&L_straight>=50&&R_straight>=50)///
      straight = 2;
    else if(BlackEndM>=55)
      straight = 3;
    else
      straight = 0;
  }
  else
    straight = 0;
  
  if(straight)
    straight_count++;
//  else
//    straight_count = 0;
  
}

float garage_point_()
{
    uint8 i=0;
    float SteerSum=0.0f;
    float helinsl=0.0f;
    for(i=50;i>40;i--)
    SteerSum+=Mid_line[i];
    helinsl=SteerSum/10.0f;
    return helinsl;
}

void _image_direction(void)/////////////////////////////////////////////////////////
{
   tjsl = Dynamic_Point_Weight(Dynamic_Point_Num);//偶数——
//   if(ending_flag==2)
//     tjsl = garage_point_();
   Error = (float)(47-tjsl);//44.5-
   //flaaaaag= (int16)(Error*10);
   Delta_Error = Error - LastError;
   //flaaaaag1=(int16)(Delta_Error*10);
   
   S_Kp = Fuzzy(Error, Delta_Error);//0.36//0.1


   if(!island_flag)
   {
     S_Kd = Fuzzy_D(Error, Delta_Error);//0.28
   }
    Servo_PWM_i=(int)((1.1f+0.13f*S_Kp)*Error+(1.1f+0.1f*S_Kd)*Delta_Error+Servo_pwm_mid);//2.4 0.26 1.1 0.12///0.9,0.13
   
   LastError = Error;
   
    if(Servo_PWM_i>Servo_pwm_max)
      Servo_PWM_i=Servo_pwm_max;
    if(Servo_PWM_i<Servo_pwm_min)
      Servo_PWM_i=Servo_pwm_min;


}

uint8 i_R = 0 ,j_L = 0 ;
void On(void)
{
  i_R = 0 ;
  j_L = 0 ;
  for (uint8 i = 59; i > 21 ; i-=2 )
  {
    if( Right_Line[i] > Right_Line[i - 4]&&Right_Line[i] - Right_Line[i - 4] < 4)
      i_R++;
    if( Left_Line[i] < Left_Line[i - 4]&&Left_Line[i - 4] - Left_Line[i] < 4)
      j_L++;
  }

  if(gpio_get(BM3)==1)
  {
    if((i_R+j_L)>8)//((i_R>12&&j_L>5)||(i_R>5&&j_L>12))
      start_fl=1;
  }
  else
  {
    if((i_R+j_L)>8)//((i_R>12&&j_L>5)||(i_R>5&&j_L>12))//弯道8直道
      start_fl=2;
  }
}       
      
void Normal(void)/*******/
{
/********纯摄像头***********/
  _image_direction();

  if(start_fl==0)///0
  {
    if(gpio_get(BM3)==1)
      Servo_PWM_i = Servo_pwm_mid + 55;//50//747
    else
      Servo_PWM_i = Servo_pwm_mid - 55;//647
  }
  if(garage_enter==1)
    ending_flag = 0;
  if(ending_flag==2&&start_fl==1)
    Servo_PWM_i = Servo_pwm_max;
  else if(ending_flag==2&&start_fl==2)
    Servo_PWM_i = Servo_pwm_min;
  
//  if(Ramp_Flag)
//   {
//     Servo_PWM_i=Servo_pwm_mid;
//   }
  
  Servo_PWM=Servo_PWM_i;
  pwm_duty(PWM4_MODULE2_CHA_C30, Servo_PWM);
}

void D_I(void)
{
/********正常赛道***********/
  _image_direction();
  Servo_PWM=Servo_PWM_i;
  
/********断路判断及处理***********/
  //Breaking();
  if(endianci==1)
  {
    _adc_direction();
    Servo_PWM=Servo_PWM_a;
  }
  pwm_duty(PWM4_MODULE2_CHA_C30, Servo_PWM);
}

void ramp_speed_deal(uint8 data1,uint8 data2,uint8 data3,uint8 data4,uint8 data5)//////////////////////////////
{
  uint8 hw_start_flag=0;
      for(int j=41;j>=21;)
    {
      j -= 2;
      if(Left_Line[j]>5&&Left_Line[j]<47&&Right_Line[j]<88&&Right_Line[j]>47)///////////////////////////////////////////////////////////
        hw_start_flag++;
    }
	if((data1 >= data2&&hw_flag == 0)&&(hw_start_flag>8))//////////////
	{
     
                 hw_flag  = 1;                //检测到坡道
	}
    
	if((data1 <= data3&&hw_flag == 1)) 
	{
		hw_flag = 2;                   //在坡道上
	}

	if((data1 >= data4&&hw_flag == 2))  
	{
		hw_flag = 3;                  //下坡
	}                    

	if((data1 <= data5&&hw_flag == 3))  
	{
		hw_flag = 0;                   //正常
	}

}

void Servo_control(uint8 n)
{
  switch(n)
  {
  case 0:       Normal();      break;
  case 1:       D_I();         break;
  default:                     break;
  }
}

void PIT_IRQHandler(void)
{
    if(PIT_FLAG_GET(PIT_CH0))
    {
      static uint8 cnt=0;
      static uint16 island_time=0;
      
      sharp_font = adc_mean_filter(ADC_1,ADC1_CH6_B17,4);
      if(gpio_get(BM2)==1)
        ramp_speed_deal(sharp_font ,85 ,15 ,30 ,20 );/////////////////////////////45
      
      SpeedGet();
      
      if(ending_flag==2)
        SUM_PulseFTM += Ave_PulseFTM;
      Motor_output_i();
      
      cnt++;
      if(cnt%50==0)
        screen_flag=1;
      if(cnt>=100)
      {
        cnt=0;
        s++;
      }
      
      if(island_flag!=0)
      {
        island_time++;
        if(island_time>=350)///350
        {
          island_flag=0;
          is_diu_flag=0;
          island_time=0;
          island_right_mid_add_flag = 0;
          island_left_mid_add_flag = 0;

          ring_big_less_choose = 0;
          out_ring_flag = 0;
          find_left_point_x = 20;//20;
          find_left_point_y = 40;//40;
          find_right_point_x = 29;
          find_right_point_y = 54;
          
        }
      }
      
      if(fand_ring_time != 0)//出环岛后沿一侧跑一段时间
      {
        fand_ring_time -= 1;
        if(fand_ring_time == 0)
        {
          island_right_mid_add_flag = 0;
          island_left_mid_add_flag = 0;
        }
      }
        PIT_FLAG_CLEAR(PIT_CH0);
    }
    
    if(PIT_FLAG_GET(PIT_CH1))
    {
     static uint16 timer1 = 0;
    // static uint16 timer2 = 0;
//     uint8 ramp_count=0;
     evaluate2();
     
     
     if(hw_flag)
     {
       timer1++;
       if(timer1>140)
       {
         hw_flag=0;
         timer1=0;
       }
     }
//        if(Ramp_Time_Count!=0)
//        {
//          Ramp_Time_Count -=1;
//          if(Ramp_Time_Count==0)
//          {
//            Ramp_Flag=0;
//            
//          }
//        }
//        if(Ramp_Flag==1)
//        {
//          timer1++;
//         if(timer1>=10)
//         {
//            Ramp_Flag=0;
//            timer1=0;
//            after_ramp_flag=1;
//         }
//        }
//           if(after_ramp_flag==1)
//            {
//              timer2++;
//              if(timer2>=100)
//              {
//                after_ramp_flag=0;
//                timer2=0;
//              }
//            }
     
//     ////坡道
//     
//            if (Ramp_Flag || Ramp_Time)
//            {
//                Ramp_Time++;
//            }
//            if(Ramp_Time>Ramp_Delay)
//              Ramp_Flag=0;
//            if(Ramp_Time>200)
//            {
//             Ramp_Time=0;
//             Ramp_Continue_Flag=1;
//            }
//            
            
//   get_gyro();
//    Gyro_Accumulate+=((mpu_gyro_y+2.5)/16.384/100);
//    Deg_=(int16)(Gyro_Accumulate);//3.1416*180);/////   0---0   5---45   11---90     44---360
//  if(Deg_>=5)
//    turn_right=1; 
           
        
        
        PIT_FLAG_CLEAR(PIT_CH1);
     }
    if(PIT_FLAG_GET(PIT_CH2))
    {
        PIT_FLAG_CLEAR(PIT_CH2);
    }
    if(PIT_FLAG_GET(PIT_CH3))
    {
        PIT_FLAG_CLEAR(PIT_CH3);
    }
    __DSB();
}

void Control_p()////////////////
{
  if((s<=6&&start_fl==0)||(ending_flag==2))
    On();///

  Servo_control(0);//比赛3
  Dynamic_PID();
  Track_Line();
  SpeedSet_Control();
}

int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
    
    all_init();
    EnableGlobalIRQ(0);
 
    //int j = Servo_pwm_max;
    
    for(;;)
	{
           if(mt9v03x_csi_finish_flag)
          {
             mt9v03x_csi_finish_flag=0;

             Image_Get();
             Image_Processing();///
             image_processing(image_myself);
            // Ramp_Check();

//             if(hw_flag==1||hw_flag==2)
//               Image_Handle(image_garage);    ///////////不误判就关掉  ///

             if(gpio_get(BM4)==1)
             {
               LCD_choose();
               key_deal();
               Camera_Check();//
             }

             Control_p();///////

             if(hw_flag||island_flag)//(hw_flag==1||hw_flag==2)//road_type==1//garage_enter//(island_flag !=0)road_type  //||flaaaaag
               gpio_set(B14,1);
             else
               gpio_set(B14,0);
          }
        }
}

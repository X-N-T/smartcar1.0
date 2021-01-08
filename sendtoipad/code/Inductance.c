#include "Inductance.h"
int16   valu_in[4] = {0}        ;	//获取的电感值
//int16   sharp_font, sharp_left  ;//sharp
float   inductance[2]           ;

float A_Kp = -15        ;
float A_Kd = -12.5      ;
float Lastinductance = 0.0 ;


uint16  Servo_PWM_a = 0 ;
uint8   Indu_0                  = 0 ; //离开赛道

/*                             //1064已经有这个函数了，所以可以去掉了
//ADC硬件平均
int ad_ave(ADCn_Ch adcn_ch, ADC_nbit bit,int N)
{
  uint32_t tmp=0;
  uint8_t i;
  for(i=0;i<N;i++)
    tmp+=adc_once(adcn_ch,bit);
    tmp=tmp/N;
    return (int)tmp;
}
*/             


void _adc(void)
{
  int16 i,j,k,temp;
  int16 ad_valu[4][5],ad_valu1[4],ad_sum[4];
  for(i=0;i<5;i++)
  {
    //ad_valu[0][i]=adc_mean_filter(ADC1_SE9 , ADC_10BIT, 5);
    //ad_valu[1][i]=adc_mean_filter(ADC1_SE10, ADC_10BIT, 5);     //详情请见zf_adc.h改变引脚
    //ad_valu[2][i]=adc_mean_filter(ADC1_SE11, ADC_10BIT, 5);
    //ad_valu[3][i]=adc_mean_filter(ADC1_SE13, ADC_10BIT, 5);
  }
//冒泡排序升序,舍弃最大值和最小值
  for(i=0;i<4;i++)
  {
    for(j=0;j<4;j++)
    {
      for(k=0;k<4-j;k++)
      {
        if(ad_valu[i][k]>ad_valu[i][k+1])  //前面的比后面的大,则进行交换
        {
          temp=ad_valu[i][k+1];
          ad_valu[i][k+1]=ad_valu[i][k];
          ad_valu[i][k]=temp;
        }
      }
    }
  }
//中值滤波
  for(i=0;i<4;i++)
  {
    ad_sum[i]=ad_valu[i][1]+ad_valu[i][2]+ad_valu[i][3];//求中间三项的和
    ad_valu1[i]=ad_sum[i]/3;
    valu_in[i]=(int16)(ad_valu1[i]/10*10);//将数值中个位数除掉
  }
}

void _adc_direction(void)
{
    _adc();//获取电感值
    if(valu_in[0]<200&&valu_in[1]<200)
      Indu_0=1;//冲出赛道停车保护
    valu_in[0]=(valu_in[0]<10?10:valu_in[0]);//四个电感值限幅
    valu_in[1]=(valu_in[1]<10?10:valu_in[1]);
    valu_in[2]=(valu_in[2]<10?10:valu_in[2]);
    valu_in[3]=(valu_in[3]<10?10:valu_in[3]);
    //左右水平电感循迹
    inductance[0]=(float)(valu_in[0]-valu_in[1])/(valu_in[0]+valu_in[1]);//水平电感的差比和作为偏差
    inductance[0]=(inductance[0]>= 1? 1:inductance[0]);	//偏差限幅
    inductance[0]=(inductance[0]<=-1?-1:inductance[0]);
    //左右竖直电感循迹
    inductance[1]=(float)(valu_in[2]-valu_in[3])/(valu_in[2]+valu_in[3]);//垂直电感的差比和作为偏差
    inductance[1]=(inductance[1]>= 1? 1:inductance[1]);	//偏差限幅
    inductance[1]=(inductance[1]<=-1?-1:inductance[1]);

    Servo_PWM_a=(int)(A_Kp*inductance[0]+A_Kd*(inductance[0]-Lastinductance)+Servo_pwm_mid);
    Lastinductance=inductance[0];

    if(Servo_PWM_a>Servo_pwm_max)
      Servo_PWM_a=Servo_pwm_max;
    if(Servo_PWM_a<Servo_pwm_min)
      Servo_PWM_a=Servo_pwm_min;
    if((inductance[0]<0.02)&&(inductance[0]>-0.02))
      Servo_PWM_a=Servo_pwm_mid;
}

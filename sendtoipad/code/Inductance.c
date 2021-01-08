#include "Inductance.h"
int16   valu_in[4] = {0}        ;	//��ȡ�ĵ��ֵ
//int16   sharp_font, sharp_left  ;//sharp
float   inductance[2]           ;

float A_Kp = -15        ;
float A_Kd = -12.5      ;
float Lastinductance = 0.0 ;


uint16  Servo_PWM_a = 0 ;
uint8   Indu_0                  = 0 ; //�뿪����

/*                             //1064�Ѿ�����������ˣ����Կ���ȥ����
//ADCӲ��ƽ��
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
    //ad_valu[1][i]=adc_mean_filter(ADC1_SE10, ADC_10BIT, 5);     //�������zf_adc.h�ı�����
    //ad_valu[2][i]=adc_mean_filter(ADC1_SE11, ADC_10BIT, 5);
    //ad_valu[3][i]=adc_mean_filter(ADC1_SE13, ADC_10BIT, 5);
  }
//ð����������,�������ֵ����Сֵ
  for(i=0;i<4;i++)
  {
    for(j=0;j<4;j++)
    {
      for(k=0;k<4-j;k++)
      {
        if(ad_valu[i][k]>ad_valu[i][k+1])  //ǰ��ıȺ���Ĵ�,����н���
        {
          temp=ad_valu[i][k+1];
          ad_valu[i][k+1]=ad_valu[i][k];
          ad_valu[i][k]=temp;
        }
      }
    }
  }
//��ֵ�˲�
  for(i=0;i<4;i++)
  {
    ad_sum[i]=ad_valu[i][1]+ad_valu[i][2]+ad_valu[i][3];//���м�����ĺ�
    ad_valu1[i]=ad_sum[i]/3;
    valu_in[i]=(int16)(ad_valu1[i]/10*10);//����ֵ�и�λ������
  }
}

void _adc_direction(void)
{
    _adc();//��ȡ���ֵ
    if(valu_in[0]<200&&valu_in[1]<200)
      Indu_0=1;//�������ͣ������
    valu_in[0]=(valu_in[0]<10?10:valu_in[0]);//�ĸ����ֵ�޷�
    valu_in[1]=(valu_in[1]<10?10:valu_in[1]);
    valu_in[2]=(valu_in[2]<10?10:valu_in[2]);
    valu_in[3]=(valu_in[3]<10?10:valu_in[3]);
    //����ˮƽ���ѭ��
    inductance[0]=(float)(valu_in[0]-valu_in[1])/(valu_in[0]+valu_in[1]);//ˮƽ��еĲ�Ⱥ���Ϊƫ��
    inductance[0]=(inductance[0]>= 1? 1:inductance[0]);	//ƫ���޷�
    inductance[0]=(inductance[0]<=-1?-1:inductance[0]);
    //������ֱ���ѭ��
    inductance[1]=(float)(valu_in[2]-valu_in[3])/(valu_in[2]+valu_in[3]);//��ֱ��еĲ�Ⱥ���Ϊƫ��
    inductance[1]=(inductance[1]>= 1? 1:inductance[1]);	//ƫ���޷�
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

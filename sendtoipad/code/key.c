#include "headfile.h"

int key1 = 1;  
int key2 = 1; 
int key3 = 1; 
int key4 = 1; 
int key5 = 1;
int key6 = 1;
//PID����                   
int32 key_show_PID[15]   = {788,296,363, 174,13, 59,67,1,23,35,14,30,15,14,15};                       
uint8  change_flag  =  0;
uint8  change_flag1 =  1;
uint8  image_change_flag  = 1;
uint8 page = 0 ;
uint8 Speed_choose = 0;
uint8 round_left_value = 0;
uint8 round_right_value = 0;
uint8 Ending_speed = 0;
void  LCD_choose()
{
  
  switch(page)
  {
           
    case 0: 
      {
           lcd_show_PID();    
           midline_display();
           break;
      }
    case 1:
      {
           show_motor();
           midline_display1();
           break;
      }
    case 2:
      {
           show_flags();
           midline_display();
           break;
      }
    case 3:
      {
           show_ramp();
           midline_display2();
           break;
      }
  }
}

void key_deal()
{  
  
    if(gpio_get (K1) == 0)//�ð������ڴ�����������ļ�
    {
     
    
      systick_delay_ms(20);
      if((gpio_get (K1) == 0)&&(key1 == 0))
        {  
          lcd_data_add();
           key1 = 1;
        } 
           
    }  
  else
   {
     if((gpio_get (K1) == 1)&&(key1 == 1))
     {
          key1 = 0;
     }
    }  
   
/**����2��ʼ**/
  if(gpio_get (K2) == 0)//�ð������ڴ�����������ļ�
  {
    systick_delay_ms(20);
    if((gpio_get (K2) == 0)&&(key2 == 0))
      {
         lcd_data_minus();
         key2 = 1;
       }
  }
  else
  {
    if((gpio_get (K2) == 1)&&(key2 == 1))
    {
       key2 = 0;
    }
  }  
  /**����2����**/
  
  /**����3��ʼ**/
  if(gpio_get (K3) == 0)//�ð�������ѡ������ļ�
  {
     systick_delay_ms(20);
     if((gpio_get (K3) == 0)&&(key3 == 0))
      {
           
          lcd_flag_add();
          key3 = 1;
       }
  }
  else
  {
    if((gpio_get (K3) == 1)&&(key3 == 1))
    {
       key3 = 0;
    }
  }  
  /**����3����**/
  
  /**����4��ʼ**/
  if(gpio_get (K4) == 0)//�ð�������ѡ������ļ�
  {
    systick_delay_ms(20);
    if((gpio_get (K4) == 0)&&(key4 == 0))
      {
            /*������flash���*/
          if(flash_check(EXAMPLE_FLASH_SECTOR,EXAMPLE_FLASH_SECTOR_PAGE))//У�鵱ǰ ��������ҳ�Ƿ������ݣ�����������������������
           {
             flash_symbol = flash_erase_sector(EXAMPLE_FLASH_SECTOR);//������������������Ѿ�������������������֮������ٴ�д���µ�����
              if(flash_symbol)  while(1);//����ʧ��
           }
        leave_with_flash();
        //���ݸ�ֵ��write_buf
            flash_symbol = flash_page_program(EXAMPLE_FLASH_SECTOR, EXAMPLE_FLASH_SECTOR_PAGE, write_buf, FLASH_SAVE_NUM);
           if(flash_symbol)  while(1);//д��ʧ��
         key4 = 1;
       }
  }
  else
  {
    if((gpio_get (K4) == 1)&&(key4 == 1))
    {
       key4 = 0;
    }
  }  
  /**����4����**/  
  
 if(gpio_get (K5) == 0)//�ð�������ѡ������ļ�
  {
     systick_delay_ms(20);
     if((gpio_get (K5) == 0)&&(key5 == 0))
      {
           page++;
           if(page>=4)
             page = 0;
           ips200_clear(WHITE);
         
          key5 = 1;
       }
  }
  else
  {
    if((gpio_get (K5) == 1)&&(key5 == 1))
    {
       key5 = 0;
    }
  }
 
   if(gpio_get (K6) == 0)//�ð�������ѡ������ļ�
  {
     systick_delay_ms(20);
     if((gpio_get (K6) == 0)&&(key6 == 0))
      {
           
          lcd_flag_minus();
          key6 = 1;
       }
  }
  else
  {
    if((gpio_get (K6) == 1)&&(key6 == 1))
    {
       key6 = 0;
    }
  }  
}


//�ú�������ѡ��Ҫ�޸���һ��ҳ�����һ������
//��ŵı���Ϊ��ʾ�����ݵ���������



void lcd_data_add()
{
 
           key_show_PID[change_flag]++; 
   
}



//�ú������ڴ�������ļ�
void lcd_data_minus()
{
           key_show_PID[change_flag]--;  

}

void lcd_flag_add()
{ 
     
     if(change_flag<12)
      change_flag+=1;
     else
       change_flag=12;
}

void lcd_flag_minus()
{       
  if(change_flag!=0)
      change_flag-=1;
    else change_flag=0;
    
}


void evaluate2()
{
  SpeedP = key_show_PID[0]*0.01f;
  SpeedI = key_show_PID[1]*0.01f;
  SpeedD = key_show_PID[2]*0.01f;
  
  speed_low = key_show_PID[5]*3;
  speed_high = key_show_PID[6]*3;
  
  C_S = key_show_PID[3]*0.01f;
  S_d = key_show_PID[4]*0.01f;
  
  if(key_show_PID[7]>=5)
    key_show_PID[7]=5;
  if(key_show_PID[7]<=1)
    key_show_PID[7]=1;
    
  Speed_choose = key_show_PID[7];
  
  round_left_value=key_show_PID[8];
  round_right_value=key_show_PID[9];
  
  Ending_speed = key_show_PID[10]*5;
  straight_value1 = key_show_PID[11];
  straight_value2 = key_show_PID[12];
  
  if(Speed_choose==1)           //����//ɱ���ؼ���
  {
    speed_low           = 150   ;
    speed_high          = 175   ;
    Dynamic_Point_Num   = 0     ;
    C_S                 = 1.78f ;
  }
  else if(Speed_choose==2)      //����//���غ���
  {
    speed_low           = 186   ;
    speed_high          = 216   ;
    Dynamic_Point_Num   = 0     ;
    C_S                 = 1.74f ;
  }
  else if(Speed_choose==3)      //����//������
  {
    speed_low           = 190   ;
    speed_high          = 225   ;
    Dynamic_Point_Num   = 2     ;
    C_S                 = 1.72f ;
  }
  else if(Speed_choose==4)      //��///�ɷɷ�//
  {
    speed_low           = 180   ;
    speed_high          = 240   ;
    Dynamic_Point_Num   = 4     ;
    C_S                 = 1.72f ;
  }
  
}


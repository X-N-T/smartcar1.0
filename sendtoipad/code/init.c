#include "headfile.h"
//flash
uint32  write_buf[FLASH_SAVE_NUM];  //向flash内写入信息的数组
uint32  read_buf[FLASH_SAVE_NUM];   //向flash内读出信息的数组
uint8   flash_symbol;               //检测flash是否卡死的标志位
uint8   Mode            = 0     ;
uint16  speed_high      = 0     ;
uint16  speed_low       = 0     ;

void all_init()
{  
  //摄像头初始化 
   mt9v03x_csi_init();

   //蜂鸣器初始化
  gpio_init(B14, GPO, 1, GPIO_PIN_CONFIG);
  
  //按键初始化
  gpio_init(K1 ,GPI,0,GPIO_PIN_CONFIG);
  gpio_init(K2 ,GPI,0,GPIO_PIN_CONFIG);
  gpio_init(K3 ,GPI,0,GPIO_PIN_CONFIG);
  gpio_init(K4 ,GPI,0,GPIO_PIN_CONFIG);
  gpio_init(K5 ,GPI,0,GPIO_PIN_CONFIG);
  gpio_init(K6 ,GPI,0,GPIO_PIN_CONFIG);
    
  //拨码初始化
  gpio_init(BM1 ,GPI,1,GPIO_PIN_CONFIG);
  gpio_init(BM2 ,GPI,1,GPIO_PIN_CONFIG);
  gpio_init(BM3 ,GPI,1,GPIO_PIN_CONFIG);
  gpio_init(BM4 ,GPI,1,GPIO_PIN_CONFIG);

  
  if(gpio_get(BM1)==1&&gpio_get(BM2)==1)
    Mode=3;
  else if(gpio_get(BM1)==1&&gpio_get(BM2)==0)
    Mode=2;
  else if(gpio_get(BM1)==0&&gpio_get(BM2)==1)
    Mode=1;
  else if(gpio_get(BM1)==0&&gpio_get(BM2)==0)
    Mode=0;
  
  //ADC初始化
  adc_init(ADC_1,ADC_IO1,ADC_8BIT);

    //屏幕初始化
  if(gpio_get(BM4)==1)
  {
   ips200_init();
   ips200_clear(WHITE);
  }
  
  //正交解码初始化
  qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
  qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);
  
  //flash初始化
  myself_flash_deal();
  
  //PIT初始化
  pit_init();                     //初始化pit外设
  pit_interrupt_ms(PIT_CH0,10);  //初始化pit通道0 周期
  pit_interrupt_ms(PIT_CH1,15);
  NVIC_SetPriority(PIT_IRQn,2);  //设置中断优先级 范围0-15 越小优先级越高 四路PIT共用一个PIT中断函数
  
     //舵机初始化
  pwm_init(PWM4_MODULE2_CHA_C30, 50,Servo_pwm_mid); // 3800  3490  3180    70 
  
//  simiic_init();
//  mpu6050_init();
  
  gpio_set(B14,1);
  systick_delay_ms(200);
  gpio_set(B14,0);
  systick_delay_ms(100);
  gpio_set(B14,1);
  systick_delay_ms(200);
  gpio_set(B14,0);
  
//  gpio_set(C5,1);
//  systick_delay_ms(400);
//  gpio_set(C5,0);
//  systick_delay_ms(150);
//  gpio_set(C5,1);
//  systick_delay_ms(250);
//  gpio_set(C5,0);
  
  //电机初始化

    pwm_init(PWM1_MODULE1_CHA_D14, 13000, 3800);//13k运放影响小哦
    pwm_init(PWM1_MODULE0_CHA_D12, 13000, 0);
    
    pwm_init(PWM1_MODULE0_CHB_D13, 13000, 3800);
    pwm_init(PWM1_MODULE1_CHB_D15, 13000, 0);

    if(gpio_get(BM1)==1)
    {
      uart_init(USART_1,115200,UART1_TX_B12,UART1_RX_B13);
    }
} 


void myself_flash_deal()
{
  flash_init();   //初始化flash
  //数据从flash中读取
  flash_read_page(EXAMPLE_FLASH_SECTOR, EXAMPLE_FLASH_SECTOR_PAGE, read_buf, FLASH_SAVE_NUM);
  //将数据从read_buf中读取出来
  get_with_flash();
  //数据从flash中读取结束
}

//数据赋值给write_buf
void leave_with_flash()
{
   uint8 i = 0;
  uint8 j = 0;
  
  for(i=0;i<15;i++)
  {
    write_buf[j] = key_show_PID[i];
    j+=1;
  }
  i = 0;
  
}

//将数据从read_buf中读取出来
void get_with_flash()
{
  uint8 i = 0;
  uint8 j = 0;

  
  for(i=0;i<15;i++)
  {
    key_show_PID[j] = read_buf[i];
    j+=1;
  }
  j = 0;
  

  
}
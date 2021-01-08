#ifndef _KEY_h
#define _KEY_h


extern float key_show_image[14];
extern uint8   change_flag;
extern uint8   change_flag1;
extern int32 key_show_PID[15];
extern uint8 page  ;
extern uint8 Speed_choose  ;
extern uint8 round_left_value ;
extern uint8 round_right_value;
extern uint8 Ending_speed ;
extern float   SpeedP          ;
extern float   SpeedI          ;
extern float   SpeedD          ;
extern float   C_S           ;
extern float   S_d            ;
extern uint8   Dynamic_Point_Num ;
extern uint8 straight_value1;
extern uint8 straight_value2;

void key_deal();                                          //按键处理
void lcd_flag_add();                                      //LCD显示标志加
void lcd_data_add();                                      //数据加
void lcd_data_minus();
void lcd_flag_minus();
void evaluate2();
void LCD_choose();

#endif



//#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
//#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
//#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
//#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
//uint8 testdatatosend[50];	//发送数据缓存
//int16 data1,data2,data3;

//            if(gpio_get(BM1)==1)
//            {
//            data1 = (int16)((LSpeedSet+RSpeedSet)/2);//SpeedSet;
//            data2 = (int16)(SpeedSet);//Ave_PulseFTM;//Ave_PulseFTM;
//            data3 = (int16)(Ave_PulseFTM);
//            testdatatosend[0]=0x88;
//            testdatatosend[1]=0xAA;
//            testdatatosend[2]=0;//本字节表示数据长度，这里先=0，函数最后再赋值，这样就不用人工计算长度了
////            testdatatosend[3]=BYTE3(data1);
////            testdatatosend[4]=BYTE2(data1);
//            testdatatosend[3]=BYTE1(data1);//将要发送的数据放至发送缓冲区
//            testdatatosend[4]=BYTE0(data1);
////            testdatatosend[7]=BYTE3(data2);
////            testdatatosend[8]=BYTE2(data2);
//            testdatatosend[5]=BYTE1(data2);//将要发送的数据放至发送缓冲区
//            testdatatosend[6]=BYTE0(data2);
////            testdatatosend[11]=BYTE3(data3);
////            testdatatosend[12]=BYTE2(data3);
//            testdatatosend[7]=BYTE1(data3);//将要发送的数据放至发送缓冲区
//            testdatatosend[8]=BYTE0(data3);
//            
//            testdatatosend[2] = 6;//4
//            uint8 sum = 0;	//以下为计算sum校验字节，从0xAA也就是首字节，一直到sum字节前一字节
//            for(uint8 i=0;i<9;i++)
//            sum += testdatatosend[i];
//            testdatatosend[9] = sum;	//将sum校验数据放置最后一字节
//            uart_putbuff(USART_1,&testdatatosend[0],9);
//            }



//             for( j = Servo_pwm_max ; j >= Servo_pwm_min; j--)
//             {
//               pwm_duty(PWM4_MODULE2_CHA_C30, j);
//               systick_delay_ms(1);
//             }
//             systick_delay_ms(1);
//             for( j = Servo_pwm_min ; j <= Servo_pwm_max; j++)
//             {
//               pwm_duty(PWM4_MODULE2_CHA_C30, j);
//               systick_delay_ms(1);
//             }
//             systick_delay_ms(1);

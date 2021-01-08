#ifndef _INIT_h
#define _INIT_h


//按键预定义
#define K1 C26
#define K2 C25
#define K3 B18
#define K4 C21
#define K5 C22
#define K6 C23

//拨码预定义
#define BM1 B19
#define BM2 C31
#define BM3 B9
#define BM4 C27

//ADC端口预定义 
#define ADC_IO1 ADC1_CH6_B17
#define ADC_IO2 ADC1_CH10_B21
#define ADC_IO3 ADC1_CH8_B19
#define ADC_IO4 ADC1_CH12_B23
#define ADC_HD1 ADC1_CH3_B14
#define ADC_HD2 ADC1_CH4_B15
#define ADC_HW1 ADC1_CH5_B16
#define ADC_HW2 ADC1_CH7_B18

//flash相关定义
//定义flash最后一个扇区，避免与程序冲突
#define EXAMPLE_FLASH_SECTOR        (FLASH_SECTOR_NUM-1)//存储参数最好从最后一个扇区开始使用
#define EXAMPLE_FLASH_SECTOR_PAGE   (0)//定义所在扇区的页编号
#define FLASH_SAVE_NUM  52//宏定义需要存储参数的个数    一个页最多为256字节，一个参数需要占4个字节，因此最多这里为256/4=64
extern uint32 write_buf[FLASH_SAVE_NUM];  //向flash内写入信息的数组
extern uint32 read_buf[FLASH_SAVE_NUM];   //向flash内读出信息的数组
extern uint8  flash_symbol;
extern uint8   Mode ;
extern uint16   speed_high;
extern uint16   speed_low;

//函数声明
void all_init();
void myself_flash_deal();
void leave_with_flash();
void get_with_flash();

#endif
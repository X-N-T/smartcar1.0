#include "headfile.h"

#define SSSSSSS_OFF_ON 1    //小S检测开关，1为开 0为关

int16  EdgeThres = 18;  //跳变沿阈值  18

uint8  BlackThres = 150;   //黑白阈值  58

uint8 image_myself[ROW_H][COL_W];
uint8 image_garage[60][COL_W];
//uint8 aaa = 0;

int16 Is_Right_Line[62] = {0},Is_Left_Line[62] = {0};
int16 Outring_Right_Line[62] = {0},Outring_Left_Line[62] = {0};
int16 Left_Line[62] = {0}, Right_Line[62] = {0}, Mid_Line[62] = {0};	// 原始左右边界数据
int16 Left_Add_Line[62], Right_Add_Line[62];		// 左右边界补线数据
int16 Left_Add_Flag[62], Right_Add_Flag[62];		// 左右边界补线标志位

//int16 Left_ramp_Line[62] = {0}, Right_ramp_Line[62] = {0}, Mid_ramp_line[62] = {0};	
//int16 Left_ramp_Add_Line[62], Right_ramp_Add_Line[62];	

int16 Width_Real[62];	// 实际赛道宽度
int16 Width_Add[62];	// 补线赛道宽度
int16 Width_Min;		// 最小赛道宽度



int16 S_temp;
int16 Foresight,Last_Foresight,Ramp_Flag_Last=0;	// 前瞻，用于速度控制

//uint8 Weight[60] = {
//  4,  6,  6,  6,  6,  6,  6,  6,  6,  6,     //50比赛
//  6,  6,  9, 10, 11, 12, 13, 15, 12, 13,     //40
// 10, 11,  5,  4,  4,  3,  3,  3,  3,  2,      //30
//  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, //20
//  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//加权平均参数
//uint8 Weight[60] = {
//  4,  4,  4,  6,  6,  6,  6,  6,  6,  6,  6,  6,     //50比赛
//  6,  6,  9, 10, 11, 12, 13, 15, 12, 13,     //40
// 10, 11,  5,  4,  4,  3,  3,  3,  3,  2,      //30
//  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, //20
//  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//  1,  1,  1,  1,  1,  1,  1,  1};	//加权平均参数
uint8 Weight[60] = {
    4,  4,  4,  4,  4,  4,  5,  5,  5,  5,
    5,  6,  6,  6,  6,  6,  6,  7,  7,  7,
    8,  9,  10, 11, 12, 13, 14, 15, 14, 13,
    12, 11, 10, 9,  8,  7,  6,  5,  4,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//加权平均参数

uint8 Ring_Weight[60] = {
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//加权平均参数

uint8 Island_Width_Add[60] = {
                           0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
                           0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
                           31,32,33,34,35,36,37,38,39,40,
                           41,42,43,44,45,46,47,48,49,50,
                           51,52,53,54,55,56,57,58,59,60,
                           61,62,63,64,65,66,67,68,69,70
};


uint8 Width_Max[61]={
//                          16,
//                          17,17,18,19,20,21,22,23,24,25,
//                          26,27,28,29,30,31,31,32,32,33,
//                          34,35,36,37,38,39,40,41,42,43,
//                          44,45,46,47,48,48,49,50,51,51,
//                          52,53,53,54,55,56,57,57,58,59,
//                          60,60,61,61,62,63,64,64,64,64
                           12,
                           12,13,13,14,15,16,17,18,19,20,
                           21,22,23,24,25,26,27,28,29,30,
                           31,32,33,34,35,36,37,38,39,40,
                           41,42,43,44,45,46,47,48,49,50,
                           51,52,53,54,55,56,57,58,59,60,
                           61,62,63,64,65,66,67,68,69,70
                     };
/****************************坡道检测*************************************/
int8 Ramp_Error[11];    //坡道宽度误差
uint8 Ramp_Flag = 0;
uint16 Ramp_Time_Count = 0;
uint16 Ramp_Error_Sum=0;
uint16 Ramp_Time=0;
uint16 Ramp_Delay=40;       //坡道延时时间 1.1S
uint8 Ramp_Continue_Flag=0; ///////////////////////////默认1
//定义文件变量
uint8 Left_Max, Right_Min;
int16 Starting_Line_Flag = 0;	// 起跑线标志位
int16 Starting_Line_Times = 0;	// 起跑线标志位
int16 Starting_Line_Time = 0;	// 起跑线标志位
int16 Starting_Line_num=0; //起跑线检测行
float Last_Point = MIDVALUE;  //??????

int16 Left_Hazard_Flag, Right_Hazard_Flag, Hazard_Flag;	// 左右障碍物标志

int16 Left_Add_Start, Right_Add_Start;	// 左右补线起始行坐标
int16 Left_Add_Stop, Right_Add_Stop;	// 左右补线结束行坐标
float Left_Ka = 0, Right_Ka = 0,ring_out_left_ka = 0,ring_out_left_kb = 0,ring_out_Right_kb = 0,ring_out_Right_ka = 0;
float Last_Right_Ka = 1,Last_Right_Kb = 59;//70
float Last_Left_Ka = 1,Last_Left_Kb = 35;
float Left_Kb = 1, Right_Kb = COL_W-1;	// 最小二乘法参数

uint8 Out_Side = 0;	// 丢线控制
int16 Line_Count;	// 记录成功识别到的赛道行数

/****************************    起跑识别变量    ****************************/
uint8   Game_End = 1;
uint8   Stop_Flag = 0;
uint8   Go_Flag = 0;
uint8   zhidao_Flag=0;
uint8   Annulus_Check=0;
//uint8   Obstacle_Check=0;
/****************************   环岛变量    ****************************/
int island_flag = 0;  //标志为0不是环岛   标志为1是左环岛   标志为2是右环岛
int island_left_mid_add_flag = 0,island_right_mid_add_flag = 0;//检测环岛后保持中线
int fand_ring_time = 0;  //出圆环之后，1s后在检测圆环

int diu_hang_num;
int tuqi_point_hang=0;
int out_ring_point_hang = 0;
int out_ring_num = 0;
extern int go_ring_flag = 0,out_ring_flag = 0;
int is_diu_flag = 0;
uint8 find_point = 0;
int zhaoguidian_diu_flag_num = 0;
extern int ring_big_less_choose = 0;
//uint8 image_tiaobian_flag = 0;
extern uint16 flaaaaag=0; 
extern uint16 flaaaaag1=0; 
uint8 find_right_point_x = 29,find_right_point_y = 54;//55
uint8 find_left_point_x = 20,find_left_point_y = 40;
//****************************    变量定义结束     ***********************************************//
void Image_Get(void)
{
  uint8 i,j;
     for(i=30;i<150;i++)
    {
      for(j=0;j<188;j++)
      {
       image_display[i-30][j]=mt9v03x_csi_image[i][j];
      }
    }
    for(i = 10; i < 70; i++)//30 90 ||
    {
        for( j = 0; j < 94; j++)
        {
            image_myself[i-10][j] = image_display[i][j*2];       //修改完毕
        }
    }
    for(i= 60;i<120;i++)
    {
      for(j=0;j<94;j++)
      {
        image_garage[i-60][j] = image_display[i][j*2];
      }
    }
}

char Error_Transform(uint8 Data, uint8 Set_num)   //求绝对值
{
	char Error;

	Error = Set_num - Data;
	if (Error < 0)
	{
		Error = -Error;
	}

	return Error;
}
/******** 限幅保护 *********/
int32 range_protect(int32 duty, int32 min, int32 max)//限幅保护
{
	if (duty >= max)
	{
		return max;
	}
	if (duty <= min)
	{
		return min;
	}
	else
	{
		return duty;
	}
}
float range_protect_point(float duty, float min, float max)//限幅保护
{
	if (duty >= max)
	{
		return max;
	}
	if (duty <= min)
	{
		return min;
	}
	else
	{
		return duty;
	}
}
/*
*	计算补线坐标
*
*	说明：使用两点法计算拟合出的补线坐标
*/
int16 Calculate_Add(uint8 i, float Ka, float Kb)	// 计算补线坐标
{
	float res;
	int16 Result;

	res = i * Ka + Kb;
	Result = range_protect((int32)res, 1, COL_W-1);

	return Result;
}
/*
*	两点法求直线
*
*	说明：拟合直线 y = Ka * x + Kb   Mode == 1代表左边界，Mode == 2代表右边界
*/
void Curve_Fitting(float *Ka, float *Kb, int16 *Start, int16 *Line, int16 *Add_Flag, int16 Mode)
{
	*Start += 2;
	if (Add_Flag[*Start] == 1)
	{
		if (*Start <= 51)
		{
			*Start += 2;
		}
		if (Mode == 2)
		{
            *Ka = 1.0*(Line[*Start+2] - Line[*Start]) / 2;
			if (*Ka < 0)
			{
				*Ka = 0;
			}
		}
		if (Mode == 1)
		{
            *Ka = 1.0*(Line[*Start+2] - Line[*Start]) / 2;
			if (*Ka > 0)
			{
				*Ka = 0;
			}
		}
	}
	else
	{
        if(Mode == 2)
            *Ka = 1.0*(Line[*Start+2] - Line[*Start]) / 2;
        if(Mode == 1)
            *Ka = 1.0*(Line[*Start+2] - Line[*Start]) / 2;
	}
	*Kb = 1.0*Line[*Start] - (*Ka * (*Start));
}
/*      1
*	两点法求直线
*
*	说明：拟合直线 y = Ka * x + Kb   Mode == 1代表左边界，Mode == 2代表右边界
*/
void Curve2_Fitting(float *Ka, float *Kb, uint8 Start,uint8 End, int16 *Line, int16 Mode,int16 num)
{
    if (Mode==1)
    {
        *Ka = 1.0*((Line[Start]+num) - Line[End]) / (Start-End);
        *Kb = 1.0*Line[End] - (*Ka * End);
    }
    else
    {
        *Ka = 1.0*((Line[Start]-num) - Line[End]) / (Start-End);
        *Kb = 1.0*Line[End] - (*Ka * End);
    }

}
/*
*	两点法求直线
*
*	说明：拟合直线 y = Ka * x + Kb   Mode == 1代表左边界，Mode == 2代表右边界
*/
void Curve3_Fitting(float *Ka, float *Kb, uint8 Start,uint8 End, int16 *Line, int16 Mode)//环岛检测专用，最正规的求线
{
    if (Mode==1)
    {
        *Ka = 1.0*((Line[Start]) - Line[End]) / (Start-End);
        *Kb = 1.0*Line[End] - (*Ka * End);
    }
    else
    {
        *Ka = 1.0*((Line[Start]) - Line[End]) / (Start-End);
        *Kb = 1.0*Line[End] - (*Ka * End);
    }

}
void Curve2_Fitting_Island(float *Ka, float *Kb, uint8 Start,uint8 End, int16 *Left,int16 *Right, int16 Mode,uint8 power)//环岛补线使用
{
        if (Mode==1)
        {
             *Ka = 1.0*((Left[Start]) - Right[End] +power) / (Start-End);
             *Kb = 1.0*Right[End] - (*Ka * End);
        }
        else
        {
              *Ka = 1.0*((Right[Start]) - Left[End] -power)/ (Start-End);
              *Kb = 1.0*Left[End] - (*Ka * End);
        }
}
//curve函数未使用
float Curve(int16 x1,int16 y1,int16 x2,int16 y2)
{
    float Cur;
    float s1,s2;
    s1 = x1 - x2;
    s2 = y1 - y2;
    Cur = s2 / s1;
    return Cur;
}

float edgechazhi[92] = {0};

void GetVal(void)
{
//  signed int i, j;
//  signed int gx;
//  signed int gx_temp;
//  signed int EdgeSum = 0;
//  signed int EdgeScale[256] = {0}; //边缘强度分布数组
//  for(i=19; i<ROW_H; i++)
//  {
//    for(j=1; j<COL_W - 2; j++)
//    {
//      gx = image[i][j+1] - image[i][j-1];  //差的绝对值范围0-255
//      gx_temp = abs(gx);
//      ++EdgeScale[gx_temp];                //边缘强度
//    }
//  }
//
//  for(i=255; i>=0; i--)
//  {
//    EdgeSum = EdgeSum+EdgeScale[i];
//    if (EdgeSum > 320)
//    {
//      EdgeThres = i-1;
//      break;
//    }
//  }
//  if(EdgeThres <= 17) EdgeThres = 17;
//  if(EdgeThres >= 23) EdgeThres = 23;
//  for(i=0; i<256; i++)
//    EdgeScale[i] = 0;    //清零
}
/****************** 新算法 ******************/

//2019年10月23日 11:32:18   修改到这里 
/*
*	图像算法参数初始化
*
*	说明：仅影响第一行特殊处理     94列
*/
void Image_Para_Init(void)
{
	Mid_Line[61] = MIDVALUE;
	Left_Line[61] = 1;
	Right_Line[61] = 93;
	Left_Add_Line[61] = 1;
	Right_Add_Line[61] = 93;
	Width_Real[61] = 92;
	Width_Add[61] = 92;
}
/*
*	从中间向两边搜索边界
*
*	说明：本函数使用后将保存边界数据    Mid上一行中线的值   Left_Min左边最小值  Right_Max右边最大值
Left_Line实际左边界     Right_Line实际右边界    Left_Add_Line补线左边界      Right_Add_Line补线右边界
*/
void Traversal_Mid_Line(int16 i,uint8 (*data)[COL_W], int16 Mid, int16 Left_Min, int16 Right_Max, int16 *Left_Line, int16 *Right_Line, int16 *Left_Add_Line, int16 *Right_Add_Line)
{
    int16 j;
    Left_Add_Flag[i] = 1;	// 初始化补线标志位
	Right_Add_Flag[i] = 1;

    Right_Line[i] = Right_Max;
    Left_Line[i] = Left_Min;	// 给定边界初始值

    for (j = Mid; j >= Left_Min+1; j--)	// 以前一行中点为起点向左查找边界
    {
        if(((data[i][j+1] - data[i][j-1]) > EdgeThres) && ((data[i][j] - data[i][j-2]) > EdgeThres) && (data[i][j-1] < BlackThres+20))
            //if(data[i][j] < BlackThres  && data[i][j-1] < BlackThres)
        {
            Left_Add_Flag[i] = 0;	//左边界不需要补线，清除标志位
	    Left_Line[i] = j;	        //记录当前j值为本行实际左边界
	    Left_Add_Line[i] = j;	//记录实际左边界为补线左边界
	    break;
        }
    }
    for (j = Mid; j <= Right_Max-1; j++)	// 以前一行中点为起点向右查找右边界
    {
        if(((data[i][j-1] - data[i][j+1]) > EdgeThres) && ((data[i][j] - data[i][j+2]) > EdgeThres) && (data[i][j+1] < BlackThres+20))	//检测到黑点
            //if(data[i][j] < BlackThres  && data[i][j+1] < BlackThres)
        {
	    Right_Add_Flag[i] = 0;		//右边界不需要补线，清除标志位
	    Right_Line[i] = j;	                //记录当前j值为本行右边界
	    Right_Add_Line[i] = j;	        // 记录实际右边界为补线左边界
            break;
	}
    }
    if(Left_Add_Flag[i])	// 左边界需要补线
	{
		if (i >= 55)	// 前6行
		{
			Left_Add_Line[i] = Left_Line[59];	// 使用底行数据   ????底行特别容易丢线
		}
		else                     ///
		{
			Left_Add_Line[i] = Left_Add_Line[i+2];	// 使用前2行左边界作为本行左边界
		}
	}
	if(Right_Add_Flag[i])	// 右边界需要补线
	{
		if (i >= 55)	// 前6行
		{
			Right_Add_Line[i] = Right_Line[59];	// 使用底行数据   ????底行特别容易丢线
		}
		else         //////
		{
			Right_Add_Line[i] = Right_Add_Line[i+2];	// 使用前2行右边界作为本行右边界
		}
	}
	Width_Real[i] = Right_Line[i]     - Left_Line[i];		// 计算实际赛道宽度
	Width_Add[i]  = Right_Add_Line[i] - Left_Add_Line[i];	        // 计算补线赛道宽度
}
/*
*	从左侧开始搜索边界，返回赛道宽度
*
*	说明：本函数仅仅作为探测赛道使用，仅返回赛道宽度，不保存边界数据
*/
int16 Traversal_Left(int16 i,uint8 (*data)[COL_W], int16 *Mid, int16 Left_Min, int16 Right_Max)
{
	int16 j, White_Flag = 0;
	int16 Left_Line = Left_Min, Right_Line = Right_Max;

	for (j = Left_Min; j <= Right_Max; j++)	// 边界坐标 1到79
	{
		if (!White_Flag)	// 先查找左边界
		{
                  if(data[i][j] > BlackThres)
			{
				Left_Line = j;	// 记录当前j值为本行左边界
				White_Flag = 1;	// 左边界已找到，必有右边界

				continue;	// 结束本次循环，进入下次循环
			}
		}
		else
		{
                  if(data[i][j] < BlackThres)
			{
				Right_Line = j-1;//记录当前j值为本行右边界
				break;	// 左右边界都找到，结束循环
			}
		}
	}

	if (!White_Flag)	// 未找到左右边界
	{
		return 0;
	}
	else
	{
		*Mid = (Right_Line + Left_Line) / 2;
		return (Right_Line - Left_Line);
	}
}
/*
*	从右侧开始搜索边界，返回赛道宽度
*
*	说明：本函数仅仅作为探测赛道使用，仅返回赛道宽度，不保存边界数据
*/
int16 Traversal_Right(int16 i,uint8 (*data)[COL_W], int16 *Mid, int16 Left_Min, int16 Right_Max)
{
	int16 j, White_Flag = 0;
	int16 Left_Line = Left_Min, Right_Line = Right_Max;

	for (j = Right_Max; j >= Left_Min; j--)	// 边界坐标 1到79
	{
		if (!White_Flag)	// 先查找右边界
		{
			if(data[i][j] > BlackThres)	// 检测到白点
			{
				Right_Line = j;	// 记录当前j值为本行右边界
				White_Flag = 1;	// 右边界已找到，必有左边界

				continue;	// 结束本次循环，进入下次循环
			}
		}
		else
		{
			if(data[i][j] < BlackThres)	//检测黑点
			{
				Left_Line = j+1;	//记录当前j值为本行左边界

				break;	// 左右边界都找到，结束循环
			}
		}
	}
	if (!White_Flag)	// 未找到左右边界
	{
		return 0;
	}
	else
	{
		*Mid = (Right_Line + Left_Line) / 2;

		return (Right_Line - Left_Line);
	}
}
/*
*	从左侧开始搜索边界，保存赛道边界，返回1成功 0失败
*
*	说明：本函数使用后将保存边界数据
*/
int16 Traversal_Left_Line(int16 i,uint8 (*data)[COL_W], int16 *Left_Line, int16 *Right_Line)
{
	int16 j, White_Flag = 0;

	Left_Line[i] = 1;
	Right_Line[i] = 93;

	for (j = 1; j < 94; j++)	// 边界坐标 1到79
	{
		if (!White_Flag)	// 先查找左边界
		{
			if(data[i][j] > BlackThres)	// 检测到白点
			{
				Left_Line[i] = j;	// 记录当前j值为本行左边界
				White_Flag = 1;		// 左边界已找到，必有右边界

				continue;	// 结束本次循环，进入下次循环
			}
		}
		else
		{
			if(data[i][j] < BlackThres)	//检测黑点
			{
				Right_Line[i] = j-1;//记录当前j值为本行右边界

				break;	// 左右边界都找到，结束循环
			}
		}
	}
	if (White_Flag)
	{
		Left_Add_Line[i]  = Left_Line[i];
		Right_Add_Line[i] = Right_Line[i];
		Width_Real[i]     = Right_Line[i] - Left_Line[i];
		Width_Add[i]      = Width_Real[i];
	}

	return White_Flag;	// 返回搜索结果
}

/*
*	从右侧开始搜索边界，保存赛道边界，返回1成功 0失败
*
*	说明：本函数使用后将保存边界数据
*/
int16 Traversal_Right_Line(int16 i,uint8 (*data)[COL_W], int16 *Left_Line, int16 *Right_Line)
{
	int16 j, White_Flag = 0;

	Left_Line[i] = 1;
	Right_Line[i] = COL_W-1;

	for (j = COL_W-1; j > 0; j--)	// 边界坐标 1到79
	{
		if (!White_Flag)	// 先查找右边界
		{
			if(data[i][j] > BlackThres)	// 检测到白点
			{
				Right_Line[i] = j;	// 记录当前j值为本行右边界
				White_Flag = 1;		// 右边界已找到，必有左边界

				continue;	// 结束本次循环，进入下次循环
			}
		}
		else
		{
			if(data[i][j] < BlackThres)	//检测黑点
			{
				Left_Line[i] = j+1;//记录当前j值为本行左边界

				break;	// 左右边界都找到，结束循环
			}
		}
	}
	if (White_Flag)
	{
		Left_Add_Line[i] = Left_Line[i];
		Right_Add_Line[i] = Right_Line[i];
		Width_Real[i] = Right_Line[i] - Left_Line[i];
		Width_Add[i] = Width_Real[i];
	}
	return White_Flag;	// 返回搜索结果
}
/*
*	第一行特殊处理
*
*	说明：先使用第60行中点作为第59行(第一行)搜线起始位置，成功搜索到左右边界后
将第59行中点赋值给第60行便于下一帧图像使用。如果第60行中点在本行为黑点，再
分别使用左遍历和右遍历的方法搜索边界，以赛道宽度较大的结果作为第59行边界，
若仍然搜索不到边界或数据异常认为出界，出界返回0
*/
int16 First_Line_Handle(uint8 (*data)[COL_W])
{
	int16 i;	                        // 控制行
	int16 Weight_Left, Weight_Right;	// 左右赛道宽度
	int16 Mid_Left, Mid_Right; 
	Image_Para_Init();
	i = 59;


        if((data[i][Mid_Line[61]] < BlackThres) || (data[i][Mid_Line[61]-2] < BlackThres) || (data[i][Mid_Line[61]+2] < BlackThres) || (data[i][Mid_Line[61]+4] < BlackThres) || (data[i][Mid_Line[61]+6] < BlackThres))
	{
         
            Weight_Left = Traversal_Left(i, data, &Mid_Left, 1, COL_W-1);	// 从左侧搜索边界
            Weight_Right = Traversal_Right(i, data, &Mid_Right, 1, COL_W-1);// 从右侧搜索边界
            if (Weight_Left >= Weight_Right && Weight_Left)	// 左赛道宽度大于右赛道宽度且不为0
            {
              Traversal_Left_Line(i, data, Left_Line, Right_Line);	// 使用左遍历获取赛道边界
            }
            else if (Weight_Left < Weight_Right && Weight_Right)
            {
              Traversal_Right_Line(i, data, Left_Line, Right_Line);	// 使用右遍历获取赛道边界
            }
            else	// 说明没查到
            {
              return 0;
            }
	}
	else
	{
          Traversal_Mid_Line(i, data, Mid_Line[i+2], 1, COL_W-1, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);	// 从前一行中点向两边扫描
	}

	Left_Line[61] = Left_Line[59];
	Right_Line[61] = Right_Line[59];
	Left_Add_Line[61] = Left_Add_Line[59];
	Right_Add_Line[61] = Right_Add_Line[59];   //将上一行数值赋值给下一行使用
        
	if (Left_Add_Flag[59] && Right_Add_Flag[59])
	{
		Mid_Line[59] = Mid_Line[61];
	}
	else
	{
		Mid_Line[59] = (Right_Line[59] + Left_Line[59]) / 2;
		Mid_Line[61] = Mid_Line[59];	 // 更新第60行虚拟中点，便于下一帧图像使用
	}
	Width_Real[61] = Width_Real[59];
	Width_Add[61] = Width_Add[59];
	Width_Min = Width_Add[59];

	return 1;
}
/*
*	补线修复
*
*	说明：有始有终才使用，直接使用两点斜率进行补线
*/
/*
*	补线修复
*
*	说明：有始有终才使用，直接使用两点斜率进行补线
*/
void Line_Repair(uint8 Start, uint8 Stop,uint8 (*data)[COL_W], int16 *Line, int16 *Line_Add, int16 *Add_Flag, int16 Mode)
{
	float res;
	int16 i;	// 控制行
	float Ka, Kb;

	if ((Mode == 1) && (Right_Add_Start <= Stop) && Start <= 53)	// 左边界补线
	{
		if (Start <= 57)
		{
			Start +=2;
		}
		for (i = Start+2; i >= Stop+2;)
		{
			i -= 2;
			Line_Add[i] = range_protect(Right_Add_Line[i] - Width_Add[i+2], 1, COL_W-1);
			Width_Add[i] = Width_Add[i+2];
		}
	}
	if ((Mode == 2) && (Left_Add_Start <= Stop) && Start <= 53)	// 右边界补线
	{
		if (Start <= 57)
		{
			Start +=2;
		}
		for (i = Start+2; i >= Stop+2;)
		{
			i -= 2;
			Line_Add[i] = range_protect(Left_Add_Line[i] + Width_Add[i+2], 1, COL_W-1);
			Width_Add[i] = Width_Add[i+2];
		}
	}
	else
	{
		if (Stop)	// 有始有终
		{
			if ((Right_Add_Stop >= MIDVALUE && Left_Add_Stop >= MIDVALUE) || (Right_Add_Stop >= MIDVALUE && Left_Add_Start <= Right_Add_Stop) || (Left_Add_Stop >= MIDVALUE && Right_Add_Start <= Left_Add_Stop))	// 只有较少行需要补线，不计算斜率，直接竖直向下补线
			{
				for (i = Stop-2; i <= 57; )
				{
					i += 2;
					Line_Add[i] = Line_Add[Stop];
				}
			}
			else	// 将起始行和结束行计算斜率补线
			{
				if (Start <= 57)
				{
					Start +=2;
				}
				Ka = 1.0*(Line_Add[Start] - Line_Add[Stop]) / (Start - Stop);
				Kb = 1.0*Line_Add[Start] - (Ka * Start);

				for (i = Stop; i <= Start; )
				{
					i += 2;
					res = i * Ka + Kb;
					Line_Add[i] = range_protect((int32)res, 1, COL_W-1);
				}
			}
		}
	}
}
/*
*	补线修复
*
*	说明：有始有终才使用，直接使用两点斜率进行补线
*/
/*
*	补线修复
*
*	说明：有始有终才使用，直接使用两点斜率进行补线
*/
void Line_Repair_island(uint8 Start, uint8 Stop,uint8 (*data)[COL_W], int16 *Line, int16 *Line_Add, int16 *Add_Flag, int16 Mode)
{
	float res;
	int16 i;	// 控制行
	float Ka, Kb;

	if ((Mode == 1) && (Right_Add_Start <= Stop) && Start <= 53)	// 左边界补线
	{
		if (Start <= 57)
		{
			Start +=2;
		}
		for (i = Start+2; i >= Stop+2;)
		{
			i -= 2;
			Line_Add[i] = range_protect(Right_Add_Line[i] - Width_Add[i+2], 1, COL_W-1);
			//Width_Add[i] = Width_Add[i+2]-3;
		}
	}
	if ((Mode == 2) && (Left_Add_Start <= Stop) && Start <= 53)	// 右边界补线
	{
		if (Start <= 57)
		{
			Start +=2;
		}
		for (i = Start+2; i >= Stop+2;)
		{
			i -= 2;
			Line_Add[i] = range_protect(Left_Add_Line[i] + Width_Add[i+2], 1, COL_W-1);
			//Width_Add[i] = Width_Add[i+2]-3;
		}
	}
	else
	{
		if (Stop)	// 有始有终
		{
			if ((Right_Add_Stop >= MIDVALUE && Left_Add_Stop >= MIDVALUE) || (Right_Add_Stop >= MIDVALUE && Left_Add_Start <= Right_Add_Stop) || (Left_Add_Stop >= MIDVALUE && Right_Add_Start <= Left_Add_Stop))	// 只有较少行需要补线，不计算斜率，直接竖直向下补线
			{
				for (i = Stop-2; i <= 57; )
				{
					i += 2;
					Line_Add[i] = Line_Add[Stop];
				}
			}
			else	// 将起始行和结束行计算斜率补线
			{
				if (Start <= 57)
				{
					Start +=2;
				}
				Ka = 1.0*(Line_Add[Start] - Line_Add[Stop]) / (Start - Stop);
				Kb = 1.0*Line_Add[Start] - (Ka * Start);

				for (i = Stop; i <= Start; )
				{
					i += 2;
					res = i * Ka + Kb;
					Line_Add[i] = range_protect((int32)res, 1, COL_W-1);
				}
			}
		}
	}
}
void Mid_Line_Repair(uint8 count,uint8 (*data)[COL_W])//中线修复
{
    //	int res;
	uint8 i,j;	// 控制行
        Left_Max = 0;
	Right_Min = COL_W-1;
        int diu_count[5]={0};
	for (i = 59; i >= count;i-=2)
	{
		Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算赛道中点
		Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];		// 计算赛道宽度
          if (i>21)
          {
              if (Left_Add_Line[i] > Left_Max)
               {
                  Left_Max = Left_Add_Line[i];
               }
              if (Right_Add_Line[i] < Right_Min)
              {
                 Right_Min = Right_Add_Line[i];
              }
          }

  
	}   
        
	Mid_Line[61] = Mid_Line[59];
}
/*
*	图像处理算法
*
*	说明：处理普通图像，包括十字、障碍
*/
/**********************************会车变量差值数组*******************************/
//int left_chazhi_value[16] = {0},right_chazhi_value[16] = {0};
//int left_chazhichazhi[15] = {0},right_chazhichazhi[15] = {0};
void Image_Handle(uint8 (*data)[COL_W])
{
    int16 i;	// 控制行
    uint8 j;
    int16 res,temps;	// 用于结果状态判断


    uint8 left_right_guai_flag = 0;

    Line_Count = 0;	// 赛道行数复位
    Starting_Line_Times=0;
    Starting_Line_Time=0;
    Starting_Line_Flag=0;

   
  
    Left_Hazard_Flag = 0;	// 复位左右障碍物标志位
    Right_Hazard_Flag = 0;
    Hazard_Flag=0;

    Left_Add_Start = 0;		// 复位补线起始行坐标
    Right_Add_Start = 0;
    Left_Add_Stop = 0;
    Right_Add_Stop = 0;
    for (i = 59; i >= 19;i-=2)
    {
        Right_Line[i] = 93;
        Left_Line[i] = 1;	// 给定边界初始值
        Left_Add_Flag[i] = 1;	// 初始化补线标志位
        Right_Add_Flag[i] = 1;
    }
    /***************************** 第一行特殊处理 *****************************/
	res = First_Line_Handle(data);
	if (res == 0)
	{
		Out_Side = 1;	// 丢线
		return;
	}
	Out_Side = 0;
	Line_Count = 59;
    /*************************** 第一行特殊处理结束 ***************************/
    for (i = 59; i >= 19;)	// 仅处理前40行图像，隔行后仅处理20行数据
    {
        i -= 2;	// 隔行处理，减小单片机负荷
        if(data[i][Mid_Line[i+2]] < (BlackThres+5)&&data[i][Mid_Line[i+2]-2] < (BlackThres+5)&&data[i][Mid_Line[i+2]+2] < (BlackThres+5))//前2行中点在本行为黑点，可能是赛道结束，也可能是环路
        {
          i += 2;
          break;
        }
        else	// 使用前2行中点向两边扫描边界
        {
              Traversal_Mid_Line(i, data, Mid_Line[i+2], 1,COL_W-1, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
        }

        /**************************** 补线检测开始 ****************************/
        if (Width_Real[i] > Width_Min+1)//&&(Left_Add_Flag[i]||Right_Add_Flag[i]))	// 赛道宽度变宽，可能是十字或环路
        {
            if (Left_Add_Line[i] <=  Left_Add_Line[i+2])
		{
			if (!Left_Add_Flag[i])
			{
				Left_Add_Flag[i] = 1;	// 强制认定为需要补线
			}
		}
            
		if (Right_Add_Line[i] >= Right_Add_Line[i+2])
		{
			if (!Right_Add_Flag[i])
			{
				Right_Add_Flag[i] = 1;	// 强制认定为需要补线
			}
		}

		if (Left_Add_Flag[i] || Right_Add_Flag[i])
		{
			if (Left_Add_Stop || Right_Add_Stop)
			{
				break;
			}
		}
        }
        /*************************** 第一轮补线开始 ***************************/
	if (Left_Add_Flag[i])	// 左侧需要补线
	{
		if (i >= 55)	// 前三行补线不算
		{
			if (!Left_Add_Start)
			{
				Left_Add_Start = i;	// 记录补线开始行
				Left_Ka = 0;
				Left_Kb = Left_Add_Line[i];
			}
			Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 使用前一帧图像左边界斜率补线
		}
		else
		{
			if (!Left_Add_Start)	// 之前没有补线
			{
				Left_Add_Start = i;	// 记录左侧补线开始行
				Curve_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, Left_Add_Flag, 1);	// 使用两点法拟合直线
			}
			Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 补线完成
		}
	}
	else
	{
		if (Left_Add_Start)	// 已经开始补线
		{
			if (!Left_Add_Stop && !Left_Add_Flag[i+2] && !Left_Add_Flag[i+4])
			{
				if (Left_Add_Line[i] >= Left_Add_Line[i+2] && Left_Add_Line[i+2] >= Left_Add_Line[i+4])
				{
					Left_Add_Stop = i+4;	// 记录左侧补线结束行
                                        if(Ramp_Flag == 0) Line_Repair(Left_Add_Start, Left_Add_Stop, data, Left_Line, Left_Add_Line, Left_Add_Flag, 1);
              //                        Left_Add_Start = 0;
               //                        Left_Add_Stop = 0;
				}
			}
		}
	}
//2019年10月23日 13:33:10
	if (Right_Add_Flag[i])	// 右侧需要补线
	{
		if (i >= 55)	// 前三行补线不算
		{
			if (!Right_Add_Start)
			{
				Right_Add_Start = i;	// 记录补线开始行
				Right_Ka = 0;
				Right_Kb = Right_Add_Line[i];
			}
			Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 使用前一帧图像右边界斜率补线
		}
		else
		{
			if (!Right_Add_Start)	// 之前没有补线
			{
				Right_Add_Start = i;	// 记录右侧补线开始行
				Curve_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, Right_Add_Flag, 2);	// 使用两点法拟合直线
			}
			Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 补线完成
		}
	}
	else
	{
		if (Right_Add_Start)	// 已经开始补线
		{
			if (!Right_Add_Stop && !Right_Add_Flag[i+2] && !Right_Add_Flag[i+4])
			{
				if (Right_Line[i] <= Right_Line[i+2] && Right_Line[i+2] <= Right_Line[i+4])
				{
					Right_Add_Stop = i+4;	// 记录右侧补线结束行
                                        if(Ramp_Flag == 0) Line_Repair(Right_Add_Start, Right_Add_Stop, data, Right_Line, Right_Add_Line, Right_Add_Flag, 2);
				}
			}
		}
	}
		/*************************** 第一轮补线结束 ***************************/
                Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
		if ((Left_Add_Flag[i] && Right_Add_Flag[i]) || (!Left_Add_Flag[i] && !Right_Add_Flag[i]))
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算中线
		}
		else
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = Mid_Line[i+2];
		}
		if (Width_Add[i] < Width_Min)
		{
			Width_Min = Width_Add[i];	// 更新最小赛道宽度
		}

                Line_Count = i;
                  
                if(Left_Add_Stop&&Right_Add_Stop)    //补线结束后又要补线这不补
                {
                    if(Left_Add_Flag[i]||Right_Add_Flag[i])
                    {
                        break;
                    }
                }
    }

    

    /*************************** 环岛检测开始 ***************************/
    if(fand_ring_time == 0)
    {
      if(gpio_get(BM1)==1)
      {
       fand_left_ring();//左环岛检测
       fand_right_ring();//右环岛检测
      }
    }
    /*************************** 环岛检测结束 ***************************/

    /*************************** 中线修复开始 ***************************/
    Mid_Line_Repair(Line_Count, data);
    /*************************** 中线修复结束 ***************************/
      if(Ramp_Continue_Flag)  //开启坡道检测
    { 
        Ramp_Check();
    }   
    
    /*************************** 环岛结束后中线特殊处理 *****************/
    if(island_left_mid_add_flag)//主要用出环岛后沿另一边走一段时间
    {
        for(int mid_count = 21; mid_count <= 59; mid_count+=2)
        {
               Mid_Line[mid_count] = Right_Line[mid_count]-Island_Width_Add[mid_count]/2+2;
        }
    }
    if(island_right_mid_add_flag)
    {
        for(int mid_count = 21; mid_count <= 59; mid_count+=2)
        {
                Mid_Line[mid_count] = Left_Line[mid_count]+Island_Width_Add[mid_count]/2-2;
        }
    }
    /****************************** 环岛结束后中线特殊处理结束 ******************************/
 
   
}

/*
*	加权平均
*
*	说明：权重是乱凑的，效果不好
*/

float Dynamic_Point_Weight(int8 Num)
{
    uint8 Line_Num=0; 
    uint8 i, Point_Mid=0;
    float Point=0;
    uint8 Count = 0;
    int8   Point_id;
    int32 Sum = 0, Weight_Count = 0;
    uint8 tep=0,L_Min=MIDVALUE,R_Max=MIDVALUE;
    if (Out_Side || Line_Count >= 57)	//出界或者摄像头图像异常
	{
        //        Line_Num++;
		Point  = Last_Point;
        // Speed=0;
        // if(Annulus_Check && Line_Num>=1)
        //  {
        //    Stop_Flag = 1;
        //  }
	}
    /********************************起跑线停车控制*******************************/
    if(Starting_Line_Flag)
    {
        // Line_Num++;
        Point  = Last_Point;
        // Speed=0;
        if(Annulus_Check)
        {
            Stop_Flag = 1;
        }
    }
    /**********************起跑线停车控制********************/
	else
	{
        if (Line_Count <= 20)
        {
            Line_Count = 21;
        }

        Count = 60-Line_Count;

        for (i = 59; i >= Line_Count-Num;i-=2)		//使用加权平均
	    {
              Point_id = i-Num;
              if (Point_id>59)Point_id=59;
	      Sum += Mid_Line[Point_id] * Weight[59-i];
	      Weight_Count += Weight[59-i];
	    }
        Point = range_protect_point(1.0*Sum / Weight_Count, 2, 92);//可以不限服保护

        Last_Point = Point;

        /***** 使用最远行数据和目标点作为前瞻 *****/
        //if (Line_Count >= 27)
        //{
        //	Point_Mid = Mid_Line[31];
        //}
        //else
        //{
        //	Point_Mid = Mid_Line[Line_Count+2];
        //}
        Point_Mid = Mid_Line[Line_Count+10];

	}

      Foresight = 0.8 * Error_Transform(Point_Mid, MIDVALUE)	
        + 0.2 * Error_Transform((uint8)Point,  MIDVALUE);             //使用最远行偏差和加权偏差确定前瞻


    Last_Foresight=Foresight;

	return Point;
}
/****************** 新算法 ******************/

/*
*	加权平均(差速)
*
*	说明：权重是乱凑的，效果不好  该函数适用于C车后轮差速 DF车请略过
*/
uint8  Motor_Point_Weight(int8 Num)
{
	uint8 i ,Point, Point_Mid;
    uint8 Count=0;
    int8   Point_id;
	int32 Sum = 0, Weight_Count = 0;
	uint8 tep=0,L_Min=MIDVALUE,R_Max=MIDVALUE;

    if (Line_Count <= 15)
    {
        Line_Count = 15;
    }
    //        for (i = 61; i >= Line_Count+2; )		//使用加权平均
    //		{
    //			i -= 2;
    //			Sum += Mid_Line[i] * Weight[59-i];
    //			Weight_Count += Weight[59-i];
    //		}
    //		Point = range_protect(Sum / Weight_Count, 2, 78);
    Count = 60-Line_Count;
    if(Count <= Num)
    {

    }
    for (i = 59; i >= Line_Count-Num;i-=2)		//使用加权平均
    {
        Point_id = i-Num;
        if (Point_id>59)Point_id=59;
        Sum += Mid_Line[Point_id] * Weight[59-i];
        Weight_Count += Weight[59-i];
    }
    Point = range_protect(Sum / Weight_Count, 2, 92);


    //        for (i = 61; i >= Line_Count+2; )		//使用加权平均
    //		{
    //			i -= 2;
    //			Sum += Mid_Line[i] * Weight[59-i];
    //			Weight_Count += Weight[59-i];
    //		}
    ////////////////////////
    Point = range_protect(Point, 2, 92);
    Last_Point = Point;

    Point_Mid = Mid_Line[Line_Count+2];
    
    return Point;
}

int count_flag_num = 0;


uint16 Left_Error_Sum = 0;
uint16 Right_Error_Sum = 0;
void fand_left_ring()
{
    int i=0,k=0;
    int count_flag = 0;
    int8 Left_Error[4] = {0};    //左环岛宽度检测误差
    //uint16 Left_Error_Sum = 0;
    Left_Error_Sum = 0;
    //static uint8 left_ring_time = 0;
    diu_hang_num=0;
    for(i=45;i>=25;i-=2)
    {
        if(Left_Line[i]==1&&Left_Line[i+2]==1&&Left_Line[i+4]==1&&Left_Line[i+6]==1)//有五行丢线并记录下丢线行的赛道宽度并
        {
            Left_Error[0] = Width_Real[i]-Width_Max[i];   // 实际赛道宽度-最大宽度？
            Left_Error[1] = Width_Real[i+2]-Width_Max[i+2];
            Left_Error[2] = Width_Real[i+4]-Width_Max[i+4];
            Left_Error[3] = Width_Real[i+6]-Width_Max[i+6];
            diu_hang_num=i;
            break;
        }
    }
    for(k = 0;k<4;k++)
        Left_Error_Sum = Left_Error_Sum + Left_Error[k];//赛道宽度求和
    if(diu_hang_num>23)
    {
        if(Right_Line[51] != 93 && Right_Line[37] != 93 && Right_Line[25] != 93 && Right_Line[25] != Right_Line[51] &&  Left_Line[57] != 1 &&  Left_Line[55] != 1)//
        {
            Curve3_Fitting(&Right_Ka, &Right_Kb, 51,37, Right_Line, 2);//环岛检测函数
            for(int aaaaa = 25; aaaaa <= 57; aaaaa += 2){
                Is_Right_Line[aaaaa] = Calculate_Add(aaaaa, Right_Ka, Right_Kb);//使用两点法计算拟合出的补线坐标
                if(Right_Line[aaaaa] <= (Is_Right_Line[aaaaa]+2) && Right_Line[aaaaa] >= (Is_Right_Line[aaaaa]-1))
                {
                    if(++count_flag == 15 && Left_Error_Sum >=60  && Left_Error_Sum <=130 &&BlackEndM>=53&&Right_Error_Sum==0&&R_lost<=10&&R_straight>45)//&&fabs(Error-LastError)<=10
                    {
                        island_flag = 1;
                        island_left_mid_add_flag = 1;//检测环岛后保持中线
                       
                       
                          
                    }
                }
            }
        }
    }
}


void fand_right_ring()
{
    int i=0,k=0;
    int count_flag = 0;
    int8 Right_Error[4] = {0};    //右环岛宽度检测误差
//    uint16 Right_Error_Sum = 0;
    Right_Error_Sum = 0;
    static uint8 right_ring_time = 0;
    diu_hang_num=0;
    for(i=45;i>=25;i-=2)
    {
        if(Right_Line[i]==COL_W-1&&Right_Line[i+2]==COL_W-1&&Right_Line[i+4]==COL_W-1&&Right_Line[i+6]==COL_W-1)//有五行丢线 同 左环岛
        {
            Right_Error[0] = Width_Real[i]-Width_Max[i];
            Right_Error[1] = Width_Real[i+2]-Width_Max[i+2];
            Right_Error[2] = Width_Real[i+4]-Width_Max[i+4];
            Right_Error[3] = Width_Real[i+6]-Width_Max[i+6];
            diu_hang_num=i;
            break;
        }
    }
    for(k = 0;k<4;k++)
        Right_Error_Sum = Right_Error_Sum + Right_Error[k];
    if(diu_hang_num>23)
    {
        if(Left_Line[51] != 1 && Left_Line[37] != 1 && Left_Line[25] != 1 && Left_Line[51] != Left_Line[25]&&  Right_Line[57] != 93 &&  Right_Line[55] != 93)
        {
            Curve3_Fitting(&Left_Ka, &Left_Kb, 51,37, Left_Line, 1);
            for(int aaaaa = 25; aaaaa <= 57; aaaaa += 2){
                Is_Left_Line[aaaaa] = Calculate_Add(aaaaa, Left_Ka, Left_Kb);
                if(Left_Line[aaaaa] <= (Is_Left_Line[aaaaa]+1) && Left_Line[aaaaa] >= (Is_Left_Line[aaaaa]-2))
                {
                    if(++count_flag == 15 && Right_Error_Sum >= 60 && Right_Error_Sum <= 130&&Left_Error_Sum==0&&BlackEndM>=53&&L_lost<=10&&L_straight>45)     //&&fabs(Error-LastError)<=10
                    {
                          island_flag = 2;
                          island_right_mid_add_flag = 1;
                    }
                }
            }
        }
    }
}
//左环岛处理
void Left_island_Handle(uint8 (*data)[COL_W])
{
    int16 i;	      // 控制行
    int16 res;	      // 用于结果状态判断
    Line_Count = 0;   // 赛道行数复位

    Left_Add_Start = 0;		// 复位补线起始行坐标
    Right_Add_Start = 0;
    Left_Add_Stop = 0;
    Right_Add_Stop = 0;

    for (i = 59; i >= 19;)
    {
        i -= 2;
        Left_Add_Flag[i] = 1;	// 初始化补线标志位
        Right_Add_Flag[i] = 1;
        Right_Line[i] = 93;
        Left_Line[i] = 1;	// 给定边界初始值
    }
    /***************************** 第一行特殊处理 *****************************/
	res = First_Line_Handle(data);
	Out_Side = 0;
	Line_Count = 59;
    /*************************** 第一行特殊处理结束 ***************************/
    for (i = 59; i >= 19;)	// 仅处理前40行图像，隔行后仅处理20行数据
    {
        i -= 2;	// 隔行处理，减小单片机负荷
        if(data[i][Mid_Line[i+2]] < BlackThres)//前2行中点在本行为黑点，可能是赛道结束，也可能是环路
        {
            i += 2;
	    break;
        }
        else	// 使用前2行中点向两边扫描边界
	{
	    Traversal_Mid_Line(i, data, Mid_Line[i+2], 1,COL_W-1, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
	}

        /**************************** 补线检测开始 ****************************/
        if (Width_Real[i] > Width_Min+1&&(Left_Add_Flag[i]||Right_Add_Flag[i]))//)	// 赛道宽度变宽，可能是十字或环路
        {
            if (Left_Add_Line[i] <=  Left_Add_Line[i+2])
	       {
		  if (!Left_Add_Flag[i])
		     {
			Left_Add_Flag[i] = 1;	// 强制认定为需要补线
		     }
	        }
	    if (Right_Add_Line[i] >= Right_Add_Line[i+2])
		{
		  if (!Right_Add_Flag[i])
		     {
			Right_Add_Flag[i] = 1;	// 强制认定为需要补线
	             }
		}

	    if (Left_Add_Flag[i] || Right_Add_Flag[i])
		{
		  if (Left_Add_Stop || Right_Add_Stop)
	             {
			break;
		     }
		}
        }
        /*************************** 第一轮补线开始 ***************************/
	if (Left_Add_Flag[i])	// 左侧需要补线
	{
			if (i >= 55)	// 前三行补线不算
			{
				if (!Left_Add_Start)
				{
					Left_Add_Start = i;	// 记录补线开始行
					Left_Ka = 0;
					Left_Kb = Left_Add_Line[i];
				}
                                
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 使用前一帧图像左边界斜率补线
			}
			else
			{
				if (!Left_Add_Start)	// 之前没有补线
				{
					Left_Add_Start = i;	// 记录左侧补线开始行
					Curve_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, Left_Add_Flag, 1);	// 使用两点法拟合直线
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 补线完成
			}
		}
		else
		{
			if (Left_Add_Start)	// 已经开始补线
			{
				if (!Left_Add_Stop && !Left_Add_Flag[i+2])
				{
					if (Left_Add_Line[i] >= Left_Add_Line[i+2])
					{
						Left_Add_Stop = i;	// 记录左侧补线结束行
					}
				}
			}
		}

		if (Right_Add_Flag[i])	// 右侧需要补线
		{
			if (i >= 55)	// 前三行补线不算
			{
				if (!Right_Add_Start)
				{
					Right_Add_Start = i;	// 记录补线开始行
					Right_Ka = 0;
					Right_Kb = Right_Add_Line[i];
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 使用前一帧图像右边界斜率补线
			}
			else
			{
				if (!Right_Add_Start)	// 之前没有补线
				{
					Right_Add_Start = i;	// 记录右侧补线开始行
					Curve_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, Right_Add_Flag, 2);	// 使用两点法拟合直线
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 补线完成
			}
		}
		else
		{
			if (Right_Add_Start)	// 已经开始补线
			{
				if (!Right_Add_Stop && !Right_Add_Flag[i+2])
				{
					if (Right_Line[i] <= Right_Line[i+2])
					{
						Right_Add_Stop = i;	// 记录右侧补线结束行
					}
				}
			}
		}
		/*************************** 第一轮补线结束 ***************************/
                Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
		if ((Left_Add_Flag[i] && Right_Add_Flag[i]) || (!Left_Add_Flag[i] && !Right_Add_Flag[i]))
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2 -2;	// 计算中线
		}
		else
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] =Mid_Line[i+2];
		}
		if (Width_Add[i] < Width_Min)
		{
                  Width_Min = Width_Add[i];	// 更新最小赛道宽度
		}
		Line_Count = i;	// 记录成功识别到的赛道行数
                if(Line_Count <= 31)Line_Count = 31;
                if(Left_Add_Stop&&Right_Add_Stop)    //补线结束后又要补线这不补
                {
                  if(Left_Add_Flag[i]||Right_Add_Flag[i])
                  {
                    break;
                  }
                }
    }
///////////////////////////////////////////////////////////////
    uint8 count_flag = 0;
    if(island_flag == 1)
    {
        if(is_diu_flag == 0)
        {
            int diu_flag_num = 0;
            for(i=59;i>=49;i-=2)
            {
                if(Left_Line[i] == 1)
                    if(++diu_flag_num == 5)
                        is_diu_flag = 1;
            }
        }
        if(is_diu_flag==1)
        {
            for(i=55;i>=25;i-=2)//29//20
            {
                if((Left_Line[i+2]<=Left_Line[i]&&Left_Line[i]<=Left_Line[i-2]&&Left_Line[i-2]>=Left_Line[i-4]&&Left_Line[i-4]>Left_Line[i-6])&&(Left_Line[i]!=1&&Left_Line[i-2]!=1&&Left_Line[i-4]!=1&&Left_Line[i-6]!=1&&Left_Line[i+2]!=1))//有五行   增  减
                {
                    tuqi_point_hang=i;//前后小中间大的点行数
                    break;
                }
            }
            if(ring_big_less_choose != 1)
            {
               if(tuqi_point_hang == 25||tuqi_point_hang == 27||tuqi_point_hang == 29||tuqi_point_hang == 31 || tuqi_point_hang == 33|| tuqi_point_hang == 35|| tuqi_point_hang == 37)
                {
//                    int diu_flag_num = 0;
//                    for(i=59;i>=47;i-=2)//区分大小环岛
//                    {
//                        if(Left_Line[i] == 1)
//                            if(++diu_flag_num == 3)
//                                ring_big_less_choose = 1;  //小环岛
//                    }
//                    if(ring_big_less_choose != 1)
//                        ring_big_less_choose = 2;//大环岛
                  ring_big_less_choose = 1;  //小环
                }
            }
            if(ring_big_less_choose == 1)
            {
                if(tuqi_point_hang >= 35)//小环岛开始补线点//47
                {
//                  if(Left_Line[59]<=3&&Left_Line[57]<=3&&Left_Line[55]<=3&&Left_Line[53]<=3&&Left_Line[51]<=1)
//                  { 
                    is_diu_flag = 2;
                    island_left_mid_add_flag = 0;
//                  }
                }
            }
//            else if(ring_big_less_choose == 2)
//            {
//                if(tuqi_point_hang >= 35)//大环岛开始补线点 
//                {
//                if(Left_Line[59]<=3&&Left_Line[57]<=3&&Left_Line[55]<=3&&Left_Line[53]<=3&&Left_Line[51]<=1)
//                  {  is_diu_flag = 2;
//                    island_left_mid_add_flag = 0;
//                  }
//                }
//                      
//            }

        }
        
        if( is_diu_flag==2)
        {    
            //***********************************************************************************//find_left_point
            uint8 image_tiaobian_flag = 0;
            uint8 find_point_1y = 0;
            for(i=59;i>=21;i--)//i=59;i>=21;i--
            {
               
                if(image_tiaobian_flag == 2)
                    break;
                image_tiaobian_flag = 0;
                for(int j = 15;j <=Right_Line[i] ;j++)//找进环岛拐点  15  
                {
                    if(!image_tiaobian_flag)
                    {
                        if(image_myself[i][j-1] - image_myself[i][j] > 15&&image_myself[i][j-1] > (BlackThres-8))
                        {
                            image_tiaobian_flag = 1;
                            find_point_1y = j;
                           // flaaaaag=find_point_1y; /////
                        }
                    }
                    else
                    {
                     // flaaaaag=2; /////
                        if(image_myself[i][j] - image_myself[i][j-1] > 15 && image_myself[i][j] > (BlackThres-8)&& image_myself[i-2][j] < (BlackThres+10))//+1
                        {
                            if(j - find_point_1y < 15)
                            {
                                if(i%2==0)i=i-1;
                                find_left_point_x = i;//i
                                find_left_point_y = j;//j
                                image_tiaobian_flag = 2;
                                break;
                            }
                            else
                                break;
                        }
                       
                    }
                }
            }
            if(find_left_point_x)//找到进环岛拐点后
            {
                Line_Count = find_left_point_x-2;
                Right_Line[find_left_point_x] = find_left_point_y;
                Curve2_Fitting(&Right_Ka, &Right_Kb, 59,find_left_point_x, Right_Line, 2,round_left_value);/////////////30  20
            }
            if(Right_Ka >= 0)//对斜率限幅 并在找不到或错误后保留上一次的值
            {
                Last_Right_Ka = Right_Ka;
                Last_Right_Kb = Right_Kb;
            }
            else
            {
                Right_Ka = Last_Right_Ka;
                Right_Kb = Last_Right_Kb-10;//
            }
            for(int j=59;j>=21;j-=2)//超出的find_left_point_x       纠正中线，方便下次搜线，搜线用中线
            {
                if(Line_Count >= 27)
                Right_Add_Line[j]= Calculate_Add(j, Right_Ka, Right_Kb) - (Line_Count - 27);
                else 
                Right_Add_Line[j] = Calculate_Add(j, Right_Ka, Right_Kb);
           
                if(Right_Add_Line[j] < Left_Add_Line[j])
                {
                    Left_Add_Line[j] = 1;
                }
            }
            for(int j=find_left_point_x;j>=31;j-=2)//找进环岛二次补线点
            {
                if((Right_Line[j] < find_left_point_y)&&(Right_Line[j-2] < Right_Line[j])&&(Right_Line[j-4] < Right_Line[j-2])&&(Right_Line[j-6] < Right_Line[j-4]))
                {
                    is_diu_flag = 3;
                    zhaoguidian_diu_flag_num = j;
                    break;
                }
            }

            float g_fSlope0 = 0;
            float g_fSlope1 = 0;
            float g_fSlope2 = 0;
            float g_fSlope3 = 0;
            float g_fSlope4 = 0;
            g_fSlope0 = 1.0*(Right_Line[49] - Right_Line[45])/4;
            g_fSlope1 = 1.0*(Right_Line[47] - Right_Line[43])/4;
            g_fSlope2 = 1.0*(Right_Line[45] - Right_Line[41])/4;
            g_fSlope3 = 1.0*(Right_Line[43] - Right_Line[39])/4;
            g_fSlope4 = 1.0*(Right_Line[41] - Right_Line[37])/4;

            if((g_fSlope0 <= 1.5) && (g_fSlope1 <= 1.5) && (g_fSlope2 <= 1.5) && (g_fSlope3 <= 1.5) && (g_fSlope4 <= 1.5)&& (g_fSlope0 >= 1.0) && (g_fSlope1 >= 1.0) && (g_fSlope2 >= 1.0) && (g_fSlope3 >= 1.0) && (g_fSlope4 >= 1.0))//Right_Line[55] == (COL_W-1) && Right_Line[51] == (COL_W-1) && Right_Line[49] == (COL_W-1))//已经进入环岛 并找到清标志的条件 下一步准备找出环岛拐点
            {
              if(BlackEndM<=50)
              {
                is_diu_flag = 4;
                tuqi_point_hang = 0;
              }
            }
        }
        if(is_diu_flag == 3)
        {
            for(int j=59;j>=Line_Count-2;j-=2)//再次找进环岛补线点
            {
                if(Right_Line[j] < 80&&image_myself[j][Right_Line[j]+7]>(BlackThres-5)&&image_myself[j][Right_Line[j]+8] > (BlackThres-5))
                {
                    zhaoguidian_diu_flag_num = j;
                    break;
                }
            }
            for(int j=zhaoguidian_diu_flag_num;j>=21;j-=2)//找到后 修改有效行
            {
                if(Right_Line[j] > Right_Line[zhaoguidian_diu_flag_num])
                {
                    Line_Count = j+2;
                    break;
                }
            }
            Curve2_Fitting(&Right_Ka, &Right_Kb, 59,zhaoguidian_diu_flag_num, Right_Line, 2,27);
            if(Right_Ka >= 0)
            {
                Last_Right_Ka = Right_Ka;
                Last_Right_Kb = Right_Kb;
            }
            else
            {
                Right_Ka = Last_Right_Ka;
                Right_Kb = Last_Right_Kb;//-10
            }
            for(i = 59; i >= zhaoguidian_diu_flag_num; i -= 2)//二次进环岛补线
                Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);
            for(i = Line_Count; i >= 21; i -= 2)//纠正中线，方便下次搜线，搜线用中线
                Right_Add_Line[i] = Right_Line[zhaoguidian_diu_flag_num];
            
            float g_fSlope0 = 0;
            float g_fSlope1 = 0;
            float g_fSlope2 = 0;
            float g_fSlope3 = 0;
            float g_fSlope4 = 0;
            g_fSlope0 = 1.0*(Right_Line[49] - Right_Line[45])/4;
            g_fSlope1 = 1.0*(Right_Line[47] - Right_Line[43])/4;
            g_fSlope2 = 1.0*(Right_Line[45] - Right_Line[41])/4;
            g_fSlope3 = 1.0*(Right_Line[43] - Right_Line[39])/4;
            g_fSlope4 = 1.0*(Right_Line[41] - Right_Line[37])/4;

            if((g_fSlope0 <= 1.5) && (g_fSlope1 <= 1.5) && (g_fSlope2 <= 1.5) && (g_fSlope3 <= 1.5) && (g_fSlope4 <= 1.5)&& (g_fSlope0 >= 1.0) && (g_fSlope1 >= 1.0) && (g_fSlope2 >= 1.0) && (g_fSlope3 >= 1.0) && (g_fSlope4 >= 1.0))//Right_Line[55] == (COL_W-1) && Right_Line[51] == (COL_W-1) && Right_Line[49] == (COL_W-1))//已经进入环岛 并找到清标志的条件 下一步准备找出环岛拐点
            {
                is_diu_flag = 4;
                tuqi_point_hang = 0;
            }

        }

        if(is_diu_flag == 4)
        {
            out_ring_num = 0;
            //***********************************************************************************//find_left_point
            uint8 image_tiaobian_flag = 0;
            uint8 out_find_point_1x = 0;
            i = 55;
            for(int j=33;j<=Right_Line[i]+1;j+=1)//33//找出环岛拐点   竖着搜线 确定列 一行一行判断///+1
            {
                i-=2;
                if(i<=39) i=39;//39
                if(image_tiaobian_flag == 2)
                    break;
                image_tiaobian_flag = 0;
                for(int k = 55;k >= i;k-=2)
                {
                    if(!image_tiaobian_flag)
                    {
                        if(image_myself[k][j] - image_myself[k-1][j] > 15 && image_myself[k][j] > (BlackThres))
                        {
                            image_tiaobian_flag = 1;
                            out_find_point_1x = k;
                         
                        }
                    }
                    else
                    {
                     // flaaaaag=1;
                       
                         if((Right_Line[k-4]>Right_Line[k])&&(Right_Line[k-4]!=93))//||((Right_Line[k]<=Right_Line[k-2])&&(Right_Line[k-2]<90)))//(Right_Line[k-2]>Right_Line[k])
                         //(image_myself[k-1][j] - image_myself[k][j] > 15 && image_myself[k-1][j] > (BlackThres))//(image_myself[k-1][j] - image_myself[k][j] > 15 && image_myself[k-1][j-2] > (BlackThres) && image_myself[k][j] < (BlackThres))
                        {
                          flaaaaag=1;
                            if(out_find_point_1x - k <= 30 && out_find_point_1x - k >= 1)//15//3
                            {
                                out_ring_num = k;
                                out_ring_flag = 1;
                                image_tiaobian_flag = 2;
                                
                                break;
                            }
                            else
                                break;
                        }
                    }
                }
            }
        }
        if(out_ring_flag == 1)//找出环岛条件
        {
            if(Right_Line[55] != 93 && Right_Line[29] != 93)//55--29
            {
                Curve3_Fitting(&Right_Ka, &Right_Kb, 55,29, Right_Line, 2);//55，29
                for(int aaaaa = 29; aaaaa <= 57; aaaaa += 2)//29//57
                {
                    Outring_Right_Line[aaaaa] = Calculate_Add(aaaaa, Right_Ka, Right_Kb);
                    if(Right_Line[aaaaa] <= (Outring_Right_Line[aaaaa]+2) && Right_Line[aaaaa] >= (Outring_Right_Line[aaaaa]-2))//2
                    {
                        if(++count_flag >= 13)  //出环岛--13 10
                        {
                            island_left_mid_add_flag = 1;
                            count_flag = 0;
                            is_diu_flag = 0;
                            out_ring_flag = 0;
                            find_left_point_x = 20;//key_show_PID[9];//20;
                            find_left_point_y = 40;//key_show_PID[10];//40;
                            island_flag = 0;
                            //flaaaaag=1;
                            if(ring_big_less_choose == 1)//小环岛出环岛后延时
                              fand_ring_time =30 ;
//                            else if(ring_big_less_choose == 2)//打环岛出环岛后延时
//                              fand_ring_time = 200;
                            ring_big_less_choose = 0;
                            break;
                        }
                    }
                }
            }
            if(out_ring_num != 0)//找到出环岛拐点后进行补线
            {
                Curve2_Fitting(&Right_Ka, &Right_Kb, out_ring_num+6,out_ring_num+4, Right_Line, 2,-2);
                ring_out_Right_ka = Right_Ka;
                ring_out_Right_kb = Right_Kb;
                for(i = out_ring_num; i >= 21; i -= 2)
                    Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);///0/5
            }
            else//找不到就用上次用进行补线
            {
                for(i = 59; i >= 21; i -= 2)
                {
                    Right_Add_Line[i] = Calculate_Add(i, ring_out_Right_ka, ring_out_Right_kb);
                }
            }

        }
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*************************** 第二轮补线修复结束 ***************************/

	/****************************** 中线修复开始 ******************************/
    if(is_diu_flag == 4)//进入环岛后 所有右线靠里一点
    {
        if(Line_Count>=27)
        for(i = 59; i >= Line_Count; i -= 2)
          Right_Add_Line[i] -= (Line_Count-27);
    }
    for(i = Line_Count;i>21;i-=2)//求中线
    {
	Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2 ;
    }
    Mid_Line_Repair(Line_Count, data);
    if(island_left_mid_add_flag)       //进环岛时用假数组中线
    {
        for(int mid_count = 21; mid_count <= 59; mid_count+=2)
        {
          Mid_Line[mid_count] = Right_Line[mid_count]-Island_Width_Add[mid_count]/2;//-4
        }
    }
    
}

//右环岛处理
void Right_island_Handle(uint8 (*data)[COL_W])
{
        int16 i;	// 控制行
	int16 res;	// 用于结果状态判断
	Line_Count = 0;	// 赛道行数复位

        Left_Add_Start = 0;		// 复位补线起始行坐标
	Right_Add_Start = 0;
	Left_Add_Stop = 0;
	Right_Add_Stop = 0;

    for (i = 59; i >= 19;)
    {
        i -= 2;
        Left_Add_Flag[i] = 1;	// 初始化补线标志位
        Right_Add_Flag[i] = 1;
        Right_Line[i] = 93;
        Left_Line[i] = 1;	// 给定边界初始值
    }

    /***************************** 第一行特殊处理 *****************************/
	res = First_Line_Handle(data);
        Out_Side = 0;
	Line_Count = 59;
    /*************************** 第一行特殊处理结束 ***************************/
    for (i = 59; i >= 19;)	// 仅处理前40行图像，隔行后仅处理20行数据
    {
        i -= 2;	// 隔行处理，减小单片机负荷
        if(data[i][Mid_Line[i+2]] < BlackThres)//前2行中点在本行为黑点，可能是赛道结束，也可能是环路
        {
            i += 2;
			break;
        }
        else	// 使用前2行中点向两边扫描边界
		{
			Traversal_Mid_Line(i, data, Mid_Line[i+2], 1,COL_W-1, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
		}

        /**************************** 补线检测开始 ****************************/
        if (Width_Real[i] > Width_Min+1&&(Left_Add_Flag[i]||Right_Add_Flag[i]))	// 赛道宽度变宽，可能是十字或环路
        {
            if (Left_Add_Line[i] <=  Left_Add_Line[i+2])
			{
				if (!Left_Add_Flag[i])
				{
					Left_Add_Flag[i] = 1;	// 强制认定为需要补线
				}
			}
			if (Right_Add_Line[i] >= Right_Add_Line[i+2])
			{
				if (!Right_Add_Flag[i])
				{
					Right_Add_Flag[i] = 1;	// 强制认定为需要补线
				}
			}

			if (Left_Add_Flag[i] || Right_Add_Flag[i])
			{
				if (Left_Add_Stop || Right_Add_Stop)
				{
                                       break;
				}
			}
        }
        /*************************** 第一轮补线开始 ***************************/
		if (Left_Add_Flag[i])	// 左侧需要补线
		{
			if (i >= 55)	// 前三行补线不算
			{
				if (!Left_Add_Start)
				{
					Left_Add_Start = i;	// 记录补线开始行
					Left_Ka = 0;
					Left_Kb = Left_Add_Line[i];
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 使用前一帧图像左边界斜率补线
			}
			else
			{
				if (!Left_Add_Start)	// 之前没有补线
				{
					Left_Add_Start = i;	// 记录左侧补线开始行
					Curve_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, Left_Add_Flag, 1);	// 使用两点法拟合直线
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 补线完成
			}
		}
		else
		{
			if (Left_Add_Start)	// 已经开始补线
			{
				if (!Left_Add_Stop && !Left_Add_Flag[i+2])
				{
					if (Left_Add_Line[i] >= Left_Add_Line[i+2])
					{
						Left_Add_Stop = i;	// 记录左侧补线结束行
					}
				}
			}
		}

		if (Right_Add_Flag[i])	// 右侧需要补线
		{
			if (i >= 55)	// 前三行补线不算
			{
				if (!Right_Add_Start)
				{
					Right_Add_Start = i;	// 记录补线开始行
					Right_Ka = 0;
					Right_Kb = Right_Add_Line[i];
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 使用前一帧图像右边界斜率补线
			}
			else
			{
				if (!Right_Add_Start)	// 之前没有补线
				{
					Right_Add_Start = i;	// 记录右侧补线开始行
					Curve_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, Right_Add_Flag, 2);	// 使用两点法拟合直线
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 补线完成
			}
		}
		else
		{
			if (Right_Add_Start)	// 已经开始补线
			{
				if (!Right_Add_Stop && !Right_Add_Flag[i+2])
				{
					if (Right_Line[i] <= Right_Line[i+2])
					{
						Right_Add_Stop = i;	// 记录右侧补线结束行//%-2
					}
				}
			}
		}
		/*************************** 第一轮补线结束 ***************************/
                Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
		if ((Left_Add_Flag[i] && Right_Add_Flag[i]) || (!Left_Add_Flag[i] && !Right_Add_Flag[i]))
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算中线
		}
		else
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = Mid_Line[i+2];
		}
		if (Width_Add[i] < Width_Min)
		{
			Width_Min = Width_Add[i];	// 更新最小赛道宽度
		}
		Line_Count = i;	// 记录成功识别到的赛道行数
                if(Line_Count <= 31)Line_Count = 31;
                if(Left_Add_Stop&&Right_Add_Stop)    //补线结束后又要补线这不补
                {
                  if(Left_Add_Flag[i]||Right_Add_Flag[i])
                  {
                      break;
                  }
                }
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////
   uint8 count_flag = 0;
    if(island_flag == 2)
    {
      if(is_diu_flag == 0)
      {
        int diu_flag_num = 0;
        for(i=59;i>=49;i-=2)
        {
          if(Right_Line[i] == 93)
            if(++diu_flag_num == 5)
              is_diu_flag = 1;//判断连续丢线五行
        }
      }
      if(is_diu_flag == 1)
      {
        for(i=55;i>=25;i-=2)
        {
          if((Right_Line[i+2]>=Right_Line[i]&&Right_Line[i]>=Right_Line[i-2]&&Right_Line[i-2]<=Right_Line[i-4]&&Right_Line[i-4]<Right_Line[i-6])&&(Right_Line[i]!=(COL_W-1)&&Right_Line[i-2]!=(COL_W-1)&&Right_Line[i-4]!=(COL_W-1)&&Right_Line[i-6]!=(COL_W-1)&&Right_Line[i+2]!=(COL_W-1)))//有五行丢线   增  减
          {
            tuqi_point_hang=i;//前后小中间大的点行数
            break;
          }
        }
        if(ring_big_less_choose != 1)
        {
          if(tuqi_point_hang == 25||tuqi_point_hang == 27||tuqi_point_hang == 29||tuqi_point_hang == 31 || tuqi_point_hang == 33|| tuqi_point_hang == 35|| tuqi_point_hang == 37)
          {
//            int diu_flag_num = 0;
//            for(i=59;i>=47;i-=2)
//            {
//              if(Right_Line[i] == 93)
//                if(++diu_flag_num == 3)
//                  ring_big_less_choose = 1;
//            }
//            if(ring_big_less_choose != 1)
//              ring_big_less_choose = 2;
            ring_big_less_choose = 1;  //小环
          }
        }
        if(ring_big_less_choose == 1)
        {
          if(tuqi_point_hang>=35)
          {
//            if(Right_Line[59]>=91&&Right_Line[57]==93&&Right_Line[55]==93)//&&Right_Add_Line[53]>=91&&Right_Add_Line[51]>=91)
//            {
              is_diu_flag = 2;//找到突起点后的连续5行丢线  下一步准备补线
              island_right_mid_add_flag = 0;
//            }
          }
        }
//        if(ring_big_less_choose == 2)
//        {
//          if(tuqi_point_hang>=35)
//          {
//             if(Right_Line[59]>=91&&Right_Line[57]>=91&&Right_Line[55]>=91)//&&Right_Add_Line[53]>=91&&Right_Add_Line[51]>=91)
//             {
//               is_diu_flag = 2;//找到突起点后的连续5行丢线  下一步准备补线
//             island_right_mid_add_flag = 0;
//             }
//          }
//        }
        }
        if(is_diu_flag == 2)
        {
        for(int j=59;j>=21;j-=2) //先使用找到的拐点来补线//////////
            {
                Left_Add_Line[j] = round_right_value;
            }
//////////            //***********************************************************************************//find_left_point
//////////            uint8 image_tiaobian_flag = 0;
//////////            uint8 find_point_1y = 0;
//////////            for(i=59;i>=21;i--)
//////////            {
//////////                if(image_tiaobian_flag == 2)
//////////                    break;
//////////                image_tiaobian_flag = 0;
//////////                for(int j = 78;j >= Left_Line[i];j--)
//////////                {
//////////                    if(!image_tiaobian_flag)
//////////                    {
//////////                        if(image_myself[i][j] - image_myself[i][j-1] > 15 && image_myself[i][j] > (BlackThres-8))
//////////                        {
//////////                            image_tiaobian_flag = 1;
//////////                            find_point_1y = j;
//////////                            // flaaaaag=1; /////
//////////                        } 
//////////                    }
//////////                    else
//////////                    {
//////////                        if(image_myself[i][j-1] - image_myself[i][j] > 15 && image_myself[i][j-1] > (BlackThres-8) && image_myself[i-2][j] < (BlackThres+10))
//////////                        {
//////////                            if(find_point_1y - j < 15)
//////////                            {
//////////                                if(i%2==0)i=i-1;
//////////                                find_right_point_x = i;
//////////                                find_right_point_y = j;
//////////                          //     flaaaaag=2; /////
//////////                                image_tiaobian_flag = 2;
//////////                                break;
//////////                            }
//////////                            else
//////////                                break;
//////////                        }
//////////                    }
//////////                }
//////////            }
//////////            if(find_right_point_x)
//////////            {
//////////              Line_Count = find_right_point_x-2;
//////////              Left_Line[find_right_point_x] = find_right_point_y;
//////////              Curve2_Fitting(&Left_Ka, &Left_Kb, 59,find_right_point_x, Left_Line, 1,round_right_value);////////////20
//////////            }
//////////            if(Left_Ka <= 0)
//////////            {
//////////                Last_Left_Ka = Left_Ka;
//////////                Last_Left_Kb = Left_Kb;
//////////            }
//////////            else
//////////            {
//////////                Left_Ka = Last_Left_Ka;
//////////                Left_Kb = Last_Left_Kb+10;//
//////////            }
//////////            for(int j=59;j>=21;j-=2) //先使用找到的拐点来补线//////////
//////////            {
//////////                if(Line_Count >= 27)
//////////                Left_Add_Line[j] = Calculate_Add(j, Left_Ka, Left_Kb) + (Line_Count-27);
//////////                else
//////////                Left_Add_Line[j] = Calculate_Add(j, Left_Ka, Left_Kb);
//////////                
//////////                if(Left_Add_Line[j] > Right_Add_Line[j])
//////////                {
//////////                    Right_Add_Line[j] = 93;
//////////                }
//////////            }
//////////            for(int j=find_right_point_x;j>=31;j-=2)
//////////            {
//////////
//////////                if((Left_Line[j] > find_right_point_y)&&(Left_Line[j-2] > Left_Line[j])&&(Left_Line[j-4] > Left_Line[j-2])&&(Left_Line[j-6] >Left_Line[j-4]))
//////////                {
//////////                    is_diu_flag = 3;
//////////                    zhaoguidian_diu_flag_num = j;
//////////                    break;
//////////                }
//////////            }
            /***************************************************************************************************/
            float g_fSlope0 = 0;
            float g_fSlope1 = 0;
            float g_fSlope2 = 0;
            float g_fSlope3 = 0;
            float g_fSlope4 = 0;
            g_fSlope0 = 1.0*(Left_Line[45] - Left_Line[49])/4;
            g_fSlope1 = 1.0*(Left_Line[43] - Left_Line[47])/4;
            g_fSlope2 = 1.0*(Left_Line[41] - Left_Line[45])/4;
            g_fSlope3 = 1.0*(Left_Line[39] - Left_Line[43])/4;
            g_fSlope4 = 1.0*(Left_Line[37] - Left_Line[41])/4;

            if((g_fSlope0 <= 1.5) && (g_fSlope1 <= 1.5) && (g_fSlope2 <= 1.5) && (g_fSlope3 <= 1.5) && (g_fSlope4 <= 1.5)&& (g_fSlope0 >= 1.0) && (g_fSlope1 >= 1.0) && (g_fSlope2 >= 1.0) && (g_fSlope3 >= 1.0) && (g_fSlope4 >= 1.0))//Right_Line[55] == (COL_W-1) && Right_Line[51] == (COL_W-1) && Right_Line[49] == (COL_W-1))//已经进入环岛 并找到清标志的条件 下一步准备找出环岛拐点
            {
              if(BlackEndM<=50)
              {
                is_diu_flag = 4;
                tuqi_point_hang = 0;
              }
            }
        }
        if(is_diu_flag == 3)
        {
            for(int j=59;j>=Line_Count-2;j-=2)
            {
                if((Left_Line[j] > 13)&&image_myself[j][Left_Line[j]-7]>(BlackThres-5)&&image_myself[j][Left_Line[j]-8] > (BlackThres-5))
                {
                    zhaoguidian_diu_flag_num = j;//找到已经进入环岛的左线点
                    break;
                }
            }
            for(int j=zhaoguidian_diu_flag_num;j>=21;j-=2)
            {
                if(Left_Line[j] < Left_Line[zhaoguidian_diu_flag_num])
                {
                    Line_Count = j+2;
                    break;
                }
            }
            Curve2_Fitting(&Left_Ka, &Left_Kb, 59,zhaoguidian_diu_flag_num, Left_Line, 1,27);
            if(Left_Ka <= 0)
            {
                Last_Left_Ka = Left_Ka;
                Last_Left_Kb = Left_Kb;
            }
            else
            {
                Left_Ka = Last_Left_Ka;
                Left_Kb = Last_Left_Kb+10;//
            }
            for(i = 59; i >= zhaoguidian_diu_flag_num; i -= 2)
                Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);//用已经进入环岛的点进行补线
            for(i = Line_Count; i >= 21; i -= 2)//纠正中线，方便下次搜线，搜线用中线
                Left_Add_Line[i] = Left_Line[zhaoguidian_diu_flag_num];
            /***************************************************************************************************/
            float g_fSlope0 = 0;
            float g_fSlope1 = 0;
            float g_fSlope2 = 0;
            float g_fSlope3 = 0;
            float g_fSlope4 = 0;
            g_fSlope0 = 1.0*(Left_Line[45] - Left_Line[49])/4;
            g_fSlope1 = 1.0*(Left_Line[43] - Left_Line[47])/4;
            g_fSlope2 = 1.0*(Left_Line[41] - Left_Line[45])/4;
            g_fSlope3 = 1.0*(Left_Line[39] - Left_Line[43])/4;
            g_fSlope4 = 1.0*(Left_Line[37] - Left_Line[41])/4;

            if((g_fSlope0 <= 1.5) && (g_fSlope1 <= 1.5) && (g_fSlope2 <= 1.5) && (g_fSlope3 <= 1.5) && (g_fSlope4 <= 1.5)&& (g_fSlope0 >= 1.0) && (g_fSlope1 >= 1.0) && (g_fSlope2 >= 1.0) && (g_fSlope3 >= 1.0) && (g_fSlope4 >= 1.0))//Right_Line[55] == (COL_W-1) && Right_Line[51] == (COL_W-1) && Right_Line[49] == (COL_W-1))//已经进入环岛 并找到清标志的条件 下一步准备找出环岛拐点
            {
              if(BlackEndM<=50)
              {
                is_diu_flag = 4;
                tuqi_point_hang = 0;
              }
            }
        }
        if(is_diu_flag == 4)//开始找出环岛拐点
        {
            out_ring_num = 0;
            //***********************************************************************************//find_left_point
            uint8 image_tiaobian_flag = 0;
            uint8 out_find_point_1x = 0;
            i = 55;
            for(int j=80;j>=Left_Line[i]-1;j-=1)/////////////////
            {
                i-=2;
                if(i<=39) i=39;
                if(image_tiaobian_flag == 2)
                    break;
                image_tiaobian_flag = 0;
                for(int k = 55;k >= i;k-=2)
                {
                    if(!image_tiaobian_flag)
                    {
                        if(image_myself[k][j] - image_myself[k-1][j] > 15 && image_myself[k][j] > (BlackThres))//&& image_myself[k-1][j] < BlackThres  //>
                        {
                            image_tiaobian_flag = 1;
                           
                            out_find_point_1x = k;
                        }
                    }
                    else
                    {
                       if((Left_Line[k-4]<Left_Line[k])&&(Left_Line[k-4]!=1))// if(image_myself[k-1][j] - image_myself[k][j] > 15 && image_myself[k-1][j+2] > (BlackThres) && image_myself[k][j] < (BlackThres))//(image_myself[k][j] - image_myself[k-1][j] > 15 && image_myself[k-1][j-2] > (BlackThres) && image_myself[k][j] < BlackThres)
                        {
                            if(out_find_point_1x - k <= 30 && out_find_point_1x - k >= -3)//
                            {
                                out_ring_num = k;
                                out_ring_flag = 1;
                                image_tiaobian_flag = 2;
                                break;
                            }
                            else
                                break;
                        }
                    }
                }
            }
        }
       if(out_ring_flag== 1)
       {
            if(Left_Line[55] != 1 && Left_Line[29] != 1)//
            {
                Curve3_Fitting(&Left_Ka, &Left_Kb, 55,29, Left_Line, 1);
                for(int aaaaa = 29; aaaaa <= 57; aaaaa += 2)//找 出环岛的标志
                {
                    Outring_Left_Line[aaaaa] = Calculate_Add(aaaaa, Left_Ka, Left_Kb);
                    if(Left_Line[aaaaa] <= (Outring_Left_Line[aaaaa]+2) && Left_Line[aaaaa] >= (Outring_Left_Line[aaaaa]-2))///< >
                    {
                        if(++count_flag >= 13)  ////出环岛
                        {
                            island_right_mid_add_flag = 1;
                            count_flag = 0;
                            is_diu_flag = 0;
                            out_ring_flag = 0;
                            island_flag = 0;
                            find_right_point_x = 29;
                            find_right_point_y = 54;
                            if(ring_big_less_choose == 1)
                              fand_ring_time = 30;
//                            else if(ring_big_less_choose == 2)
//                              fand_ring_time = 100;
                            ring_big_less_choose = 0;
                            break;
                        }
                    }
                }
            }
           if(out_ring_num != 0)//找到出环岛拐点 开始补线
           {
                Curve2_Fitting(&Left_Ka, &Left_Kb, out_ring_num+6,out_ring_num+4, Left_Line, 1,-2);
                ring_out_left_ka = Left_Ka;
                ring_out_left_kb = Left_Kb;
                for(i = out_ring_num; i >= 21; i -= 2)
                    Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);
            }
            else//找到后又丢失 强制补线 打角
            {
                for(i = 59; i >= 21; i -= 2)
                {
                    Left_Add_Line[i] = Calculate_Add(i, ring_out_left_ka, ring_out_left_kb);
                }
            }

        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*************************** 第二轮补线修复结束 ***************************/

	/****************************** 中线修复开始 ******************************/
    if(is_diu_flag == 4)
    {
        if(Line_Count>=27)
        for(i = 59; i >= Line_Count; i -= 2)
          Left_Add_Line[i] += (Line_Count-27);///////左边
    }
    for(i=Line_Count;i>21;i-=2)
    {
	Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2 ;
    }
    Mid_Line_Repair(Line_Count, data);
    if(island_right_mid_add_flag)
    {
        for(int mid_count = 21; mid_count <= 59; mid_count+=2)
        {
                Mid_Line[mid_count] = Left_Line[mid_count]+Island_Width_Add[mid_count]/2;
        }
    }
}


void image_processing(uint8 (*data)[COL_W])
{
    switch(island_flag)
    {
      case 0:  Image_Handle(data);   
               break;
      case 1:  Left_island_Handle(data);       
               break;
      case 2:  Right_island_Handle(data);       
               break;
    }
}


/**************************** 坡道检测开始 ****************************/
void Ramp_Check(void)
{
    uint8 i,j,Ramp_Count,line_Count;
    //uint16
    Ramp_Error_Sum=0;
    Ramp_Count = 0;
//    line_Count = 0;
    i = 0;

    for(j=51;j>=31;)
    {
      j -= 2;
      if(!Left_Add_Flag[j]&&!Right_Add_Flag[j])
      {
        if (Left_Line[j] >= Left_Line[j+2]&&Right_Line[j] <= Right_Line[j+2]&&Left_Line[j]>Left_Line[59]&&Right_Line[j]<Right_Line[59])	//与前一行的边界实线比较
        {
            Ramp_Error[i]=Width_Real[j]-Width_Max[j];
            if(Ramp_Error[i]>=40)
              Ramp_Error[i]=0;
            else if(Ramp_Error[i]<=0)
              Ramp_Error[i]=0;
            if(Width_Real[j]>Width_Max[j])
              Ramp_Count++;
        }
      }
      Ramp_Error_Sum+=Ramp_Error[i];
      i++;
    }
    //Mid_Line_K = 1.0*(Mid_Line[59]-Mid_Line[21])/38;
    if(Ramp_Count>=5&&Ramp_Error_Sum>=40&&Ramp_Error[8]>=(Ramp_Error[0]+3)&&(BlackEndM>=55))// 8 98  11 220  9     you  zuida  18  11dian  he 111
      //9 100 8 2 +3 55
  {
      if(island_flag==0)
      { 
        Ramp_Flag = 66;
        after_ramp_flag=1;
        Ramp_Time_Count = 30;
        Ramp_Continue_Flag=0;
      }
   }
    else
    {
       // Ramp_Flag = 0;
    }
     
}
/**************************** 坡道检测结束 ****************************/


/**************************** 摄像头校正 ****************************/
int16 Camera_error = 0;
void Camera_Check(void)
{
  uint8 i = 0;
  Camera_error = 0;
  for(i=55;i>=45;i-=2)
    Camera_error += Width_Real[i]-Width_Max[i];
}


/******************************起跑线搜线方式************************************************///
void Hahaha(int16 i,uint8 (*data)[COL_W], int16 Mid, int16 Left_Min, int16 Right_Max, int16 *Left_Line, int16 *Right_Line, int16 *Left_Add_Line, int16 *Right_Add_Line)
{
  int16 j;
  Left_Add_Flag[i] = 1;	// 初始化补线标志位
  Right_Add_Flag[i] = 1;

  Right_Line[i] = Right_Max;
  Left_Line[i] = Left_Min;	// 给定边界初始值

  for (j = Mid; j >= Left_Min+1; j--)	// 以第一行中点为起点向左查找边界
  {
    if((data[i][j] - data[i][j-1]) > EdgeThres)// && (data[i][j-1] - data[i][j-2]) > EdgeThres)
    {
      if(j > 7 && data[i][j-1] < (BlackThres) && data[i][j-2] < (BlackThres) && data[i][j-3] < (BlackThres) && data[i][j-4] < (BlackThres) && data[i][j-5] < (BlackThres) && data[i][j-6] < (BlackThres)) //比 j-1 小 8 个点全是黑的
      //只有47能进这个！
      {
        Left_Add_Flag[i] = 0;	//左边界不需要补线，清除标志位
        Left_Line[i] = j;	    //记录当前j值为本行实际左边界
        Left_Add_Line[i] = j;	//记录实际左边界为补线左边界
        break;
      }
    }
  }
  for (j = Mid; j <= Right_Max-1; j++)	// 以前一行中点为起点向右查找右边界
  {
    if((data[i][j-1] - data[i][j+1]) > EdgeThres)// && (data[i][j] - data[i][j+2]) > EdgeThres)
    {
      if(j < 88 && data[i][j+1] < (BlackThres) && data[i][j+2] < (BlackThres) && data[i][j+3] < (BlackThres) && data[i][j+4] < (BlackThres) && data[i][j+5] < (BlackThres) && data[i][j+6] < (BlackThres))
      {
        Right_Add_Flag[i] = 0;		//右边界不需要补线，清除标志位
        Right_Line[i] = j;	//记录当前j值为本行右边界
        Right_Add_Line[i] = j;	// 记录实际右边界为补线左边界
        break;
      }
    }
  }

  if(Left_Add_Flag[i])	// 左边界需要补线
  {
    if (i >= 55)	// 前6行
    {
      Left_Add_Line[i] = Left_Line[59];	// 使用底行数据   ????底行特别容易丢线
    }
    else                     ///
    {
      Left_Add_Line[i] = Left_Add_Line[i+2];	// 使用前2行左边界作为本行左边界
    }
  }
  if(Right_Add_Flag[i])	// 右边界需要补线
  {
    if (i >= 55)	// 前6行
    {
      Right_Add_Line[i] = Right_Line[59];	// 使用底行数据   ????底行特别容易丢线
    }
    else         //////
    {
      Right_Add_Line[i] = Right_Add_Line[i+2];	// 使用前2行右边界作为本行右边界
    }
  }
  Width_Real[i] = Right_Line[i] - Left_Line[i];			// 计算实际赛道宽度
  Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 计算补线赛道宽度
}

int16 Xixixi(int16 i,uint8 (*data)[COL_W], int16 *Left_Line, int16 *Right_Line) //右线不对
{
  int16 j, White_Flag = 0;

	Left_Line[i] = 1;
	Right_Line[i] = 93;

	for (j = 1; j < 94; j++)	// 边界坐标 1到79
	{
		if (!White_Flag)	// 先查找左边界
		{
			if(data[i][j] > BlackThres-15)	// 检测到白点
			{
				Left_Line[i] = j;	// 记录当前j值为本行左边界
				White_Flag = 1;		// 左边界已找到，必有右边界

				continue;	// 结束本次循环，进入下次循环
			}
		}
		else
		{
                    if((data[i][j-1] - data[i][j+1]) > EdgeThres)
                    {
                      if(j < 85 && data[i][j+1] < (BlackThres) && data[i][j+2] < (BlackThres) && data[i][j+3] < (BlackThres) && data[i][j+4] < (BlackThres) && data[i][j+6] < (BlackThres) && data[i][j+7] < (BlackThres))
                      {
                        //Right_Add_Flag[i] = 0;		//右边界不需要补线，清除标志位
                        Right_Line[i] = j;	//记录当前j值为本行右边界
                        //Right_Add_Line[i] = j;	// 记录实际右边界为补线左边界
                        break;
                      }
                    }
		}
	}
	if (White_Flag)
	{
		Left_Add_Line[i] = Left_Line[i];
		Right_Add_Line[i] = Right_Line[i];
		Width_Real[i] = Right_Line[i] - Left_Line[i];
		Width_Add[i] = Width_Real[i];
	}

	return White_Flag;	// 返回搜索结果
}
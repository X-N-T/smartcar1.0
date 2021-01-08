#ifndef _CAMERA_DEAL_h
#define _CAMERA_DEAL_h

#define MIDVALUE       47                                             ////////////////////////////
/*****************************�궨������******************************/
#define WHIETROW        10             //ȫ�����ж���
#define IMAGEERROE      3     //3.4      //ͼ��ɼ�ƫ��

#define ROW_H        60
#define COL_W        94
/***************************ö�ٶ�������*****************************/

/************************ȫ�ֱ�����������****************************/
extern uint8 image_myself[ROW_H][COL_W];
extern uint8 image_garage[60][COL_W];
extern uint8  BlackThres;   //�ڰ���ֵ
extern int ring_big_less_choose;

//extern int16 Is_Right_Line[62],Is_Left_Line[62];
extern int16 Left_Add_Line[62], Right_Add_Line[62];		// ���ұ߽粹������
extern int16 Left_Line[62], Right_Line[62], Mid_Line[62];	// ԭʼ���ұ߽�����

//extern int16 Left_ramp_Line[62], Right_ramp_Line[62], Mid_ramp_line[62];	
//extern int16 Left_ramp_Add_Line[62], Right_ramp_Add_Line[62];	

extern int16 Width_Real[62];
extern int16  EdgeThres; //��ֵ
/****************************    ����ʶ�����    ****************************/
extern uint8   Game_End;
extern uint8   Stop_Flag;
extern uint8   Go_Flag;
extern uint8   zhidao_Flag;
extern uint8   Annulus_Check;
extern uint8   Obstacle_Check;
extern uint8   Out_Side;


extern int16 Line_Count;	// ��¼�ɹ�ʶ�𵽵���������
extern int16 Foresight;
extern int16 Last_Foresight;


extern float   LastError;


extern uint16 flaaaaag; 
extern uint16 flaaaaag1; 

extern uint16 Left_Error_Sum;
extern uint16 Right_Error_Sum;

extern int16 Camera_error ;



int32 range_protect(int32 duty, int32 min, int32 max);//�޷�����
float range_protect_point(float duty, float min, float max);//�޷�����
void Image_Get(void);
void yuzhiget(void);
void Track_Type(void);
void Stright();
int16 DeviationCalculation(int16 h);
float SlopeCalculation(int16 *Line, int16 upper, int16 lower);
void ADD_Cross_Black();
void Image_Handle(uint8 (*data)[COL_W]);
float Dynamic_Point_Weight(int8 Num);
uint8  Motor_Point_Weight(int8 Num);

extern int fand_ring_time;  //��Բ��֮��1s���ڼ��Բ��
extern int16 Left_Add_Start, Right_Add_Start;	// ���Ҳ�����ʼ������
extern int16 Left_Add_Stop, Right_Add_Stop;	// ���Ҳ��߽���������
extern int tuqi_point_hang,out_ring_point_hang;
extern int is_diu_flag;
//extern uint8 image_tiaobian_flag;/////////////////////////////////////////
extern uint8 find_left_point_x,find_left_point_y ;
extern uint8 find_right_point_x ,find_right_point_y;//55
extern int out_ring_num;
extern int island_flag;  //��־Ϊ0���ǻ���   ��־Ϊ1���󻷵�   ��־Ϊ2���һ���
extern int island_left_mid_add_flag,island_right_mid_add_flag;
extern int out_ring_flag;
extern uint8 Train_off_flag;

/****************************�µ����*************************************/
extern uint8 Ramp_Flag;
extern uint8 after_ramp_flag;
extern uint16 Ramp_Time_Count;
extern int8 Ramp_Error[11]; 
extern uint16 Ramp_Error_Sum;
extern uint16 Ramp_Time;
extern uint16 Ramp_Delay;
extern uint8 Ramp_Continue_Flag; 

void fand_left_ring();
void fand_right_ring();
void fand_left_ring_w();
void fand_right_ring_w();
void image_processing(uint8 (*data)[COL_W]);
void Train_test();
void Train_test_zhi();
void GetVal(void);
void Ramp_Check(void);
void Camera_Check(void);

void Traversal_Mid_Line(int16 i,uint8 (*data)[COL_W], int16 Mid, int16 Left_Min, int16 Right_Max, int16 *Left_Line, int16 *Right_Line, int16 *Left_Add_Line, int16 *Right_Add_Line);


void Hahaha(int16 i,uint8 (*data)[COL_W], int16 Mid, int16 Left_Min, int16 Right_Max, int16 *Left_Line, int16 *Right_Line, int16 *Left_Add_Line, int16 *Right_Add_Line);
int16 Xixixi(int16 i,uint8 (*data)[COL_W], int16 *Left_Line, int16 *Right_Line); //���߲���



#endif
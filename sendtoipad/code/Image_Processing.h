#ifndef _IMAGE_PROCESSING_H
#define _IMAGE_PROCESSING_H

#include "headfile.h"
#include "math.h"
#define col     94
#define row     60


extern uint8   sharp_font, sharp_left ;//sharp


extern uint8 img[row][col]   ;//////////////////

//extern uint8   image_threshold  ;

extern int     Mid_line[row+1]  ;
extern int     R_verge[row+1]   ;
extern int     L_verge[row+1]   ;
extern int     WidthReal[row+1]  ;
extern uint8   Width[row+1]     ;
extern uint8   L_lost           ;
extern uint8   R_lost           ;
extern uint8   B_lost           ;


extern uint8   L_straight       ;
extern uint8   R_straight       ;

extern int     LeftLoseStart    ;//��¼��߶��ߵĿ�ʼ��
extern int     RightLoseStart   ;//��¼�ұ߱߶��ߵĿ�ʼ��
extern int     WhiteStart       ;
extern int     LastLine         ;
extern float   AvaliableLines   ;
extern uint8   BlackEndM        ;
extern uint8   BlackEndL        ;
extern uint8   BlackEndR        ;
extern uint8   BlackEndMax      ;
extern uint8   DropRow          ;
extern uint8   midpoint         ;
extern uint8   CrossFlag        ;//ʮ�ֱ�־0
extern uint8   CrossFlag1       ;//ʮ�ֱ�־1
extern uint8   ending_flag      ; //�յ���
extern uint8   garage_enter     ; //���
extern uint8   dark_flag        ;
extern uint8   s                ;
extern uint8   straight         ; //��ֱ��
//extern uint8   obstacles_find_flag ; 

extern int     k_left           ;
extern int     k_right          ;
extern uint8   lin[2]           ;
extern uint8   hei              ;
extern uint8   flag             ;
extern uint8   round_flag       ;
extern uint8   garage_direction ;
extern uint8   start_fl         ;
uint8 otsuThreshold(uint8 *image, uint16 _col, uint16 _row);
void Image_Binarization(void);
void Image_Denoising(void);
void GetBlackEndParam(); //��ȡ���߽�ֹ��
void Get_centerline();
void maruhandan();
void On(void);
extern int regression(int Pick_table[],int startline,int endline);//����б�ʺ���

extern void round_flag1(void);
extern void Image_Processing(void);

#endif
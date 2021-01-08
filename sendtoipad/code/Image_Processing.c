#include "Image_Processing.h"



uint8   sharp_font, sharp_left  ;//sharp
uint8   garage_enter            = 0 ;

//uint8   image_threshold = 0     ;
uint8   Threshold =110          ;       /**********************************155********************************/
int     Mid_line[row+1] ;
int     R_verge[row+1]  ;
int     L_verge[row+1]  ;
int     WidthReal[row+1]  ;
uint8   Width[row+1] = { 
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
uint8   midpoint        = col/2 ;
uint8   L_lost          = 0     ;
uint8   R_lost          = 0     ;
uint8   B_lost          = 0     ;

uint8   L_straight      = 0     ;
uint8   R_straight      = 0     ;

int     LeftLoseStart   = 0     ;//��¼��߶��ߵĿ�ʼ��
int     RightLoseStart  = 0     ;//��¼�ұ߱߶��ߵĿ�ʼ��
//int     WhiteStart      = 0     ;
int     LastLine        = 0     ;
float   AvaliableLines  = 0     ;
uint8   dark_flag       = 0     ; //��ǰһƬ��
//out
//uint8   ending_flag     = 0     ; //�յ���
uint8   s               = 0     ;
//uint8   obstacles_find_flag     = 0 ;

/*********GetBlackEndParam**********/
uint8   BlackEndMR      = 0     ;
uint8   BlackEndML      = 0     ;
uint8   BlackEndLL      = 0     ;
uint8   BlackEndRR      = 0     ;
uint8   BlackEndL       = 0     ;
uint8   BlackEndM       = 0     ;
uint8   BlackEndR       = 0     ;
uint8   BlackEndMaxMax  = 0     ;
uint8   BlackEndMax     = 0     ;
uint8   DropRow         = 0     ;

uint8   CrossFlag1       = 0    ;//ʮ�ֱ�־1
uint8   CrossFlag        = 0    ;//ʮ�ֱ�־0

uint8   lin[2] = {0}, hei = 0   ;
uint8   flag            = 0     ;
uint8   round_flag      = 0     ;

//uint32 use_time;
uint8 img[row][col]   = {0}     ;
uint8 otsuThreshold(uint8 *image, uint16 _col, uint16 _row)
{
   #define GrayScale 256
    uint16 width = _col;
    uint16 height = _row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8 threshold = 0;
    uint8* data = image;  //ָ���������ݵ�ָ��
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }
    uint32 gray_sum=0;
    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //����ǰ�ĵ������ֵ��Ϊ����������±�
            gray_sum+=(int)data[i * width + j];       //�Ҷ�ֵ�ܺ�
        }
    }
    //����ÿ������ֵ�ĵ�������ͼ���еı���
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }
    //�����Ҷȼ�[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {
                w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
                u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ�ĵ�ı��� *�Ҷ�ֵ
               w1=1-w0;
               u1tmp=gray_sum/pixelSum-u0tmp;
                u0 = u0tmp / w0;              //����ƽ���Ҷ�
                u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
                u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
                deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
                if (deltaTmp > deltaMax)
                {
                    deltaMax = deltaTmp;
                    threshold = j;
                }
                if (deltaTmp < deltaMax)
                {
                break;
                }
         }
    return threshold;
}

void Image_Binarization(void)
{
  uint8 i,j;
/********����ƫ��Ƭ�޷�***********/
    //uart_getchar(uart5,&Threshold);
  //if(BlackThres<Threshold)
    //BlackThres=Threshold;
 
//   for(i=30;i<150;i++)
//    {
//      for(j=0;j<188;j++)
//      {
//       image_display[i-30][j]=mt9v03x_csi_image[i][j];
//      }
//    }
//    
    for(i=0;i<60;i++)
    {
      for(j=0;j<col;j++)
      {
        if(image_myself[i][j]>(BlackThres))//-8
          img[i][j]= 0xff;//img[(uint8)(0.5*i)][(uint8)(0.5*j)]=0xff;
        else
          img[i][j]= 0x00;//img[(uint8)(0.5*i)][(uint8)(0.5*j)]=0x00;
      }
    }
}

void Image_Denoising(void)
{
  uint8 i,j;
  for(i=0;i<row-1;i++)
  {
    for(j=0;j<col-1;j++)
    {
      if(img[i][j]&&!img[i][j+1]&&!img[i][j-1])
        img[i][j]=0x00;
      if(!img[i][j]&&img[i][j+1]&&img[i][j-1])
        img[i][j]=0xff;
    }
  }
}

//void Get_centerline()//����
//{
//  uint8 i       = 0;
//  uint8 j       = 0;
//  uint8 jj      = 0;
//  uint8 white_dots      = 0;
//  uint8 WhiteNum        = 0;
//  
//  L_lost        = 0;//��������
//  R_lost        = 0;
//  B_lost        = 0;
//  WhiteNum      = 0;
//  
//
//
//  Mid_line[row] = col/2 ;//��60��
//  L_verge[row]  = 0     ;
//  R_verge[row]  = col   ;
//  Width[row]    = col   ;
////  WidthReal[row]=col   ;
//  for(i=0;i<row+1;i++)//����ֵ
//  {
//      R_verge[i] = col  ;
//      L_verge[i] = 0    ;
//      Mid_line[i]= col/2;// col/2;//47
//  }
//
//  for(j=0;j<col;j++)//�����ж�
//  {
//      if(img[row-2][j]==0xff||img[row-1][j]==0xff)
//        white_dots++;
//  }
//  if(white_dots<12)
//    dark_flag=1;
//
//  for(i=row-1;i>=row-10;i--)//������ǰ10�У�ȫ��ɨ��
//  {
/////left
//    if(i==row-1) 
//      j=midpoint;
//    else 
//      j=Mid_line[i+1];
//    if(j<=2)
//      j=2;
//    while(j>=3)
//    {
//      if(img[i][j]==0xff&&img[i][j-1]==0x00&&img[i][j-2]==0x00)//��������
//      {
//        L_verge[i]=j;//�Ҳ�������ԭֵ
//        break;
//      }
//      j--;
//    }
/////right
//    if(i==row-1) 
//      j=midpoint;
//    else
//      j=Mid_line[i+1];
//    if(j>=col-3) 
//      j=col-3;
//    while(j<=col-3)
//    {
//      if(img[i][j]==0xff&&img[i][j+1]==0x00&&img[i][j+2]==0x00)
//      {
//        R_verge[i]=j;
//        break;
//      }
//      j++;
//    }
/////both
//    if(L_verge[i]!=0&&R_verge[i]!=col)
//      Mid_line[i]=(L_verge[i]+R_verge[i])/2;
/////lose right
//    else if(L_verge[i]!=0&&R_verge[i]==col)
//    {
//      R_lost++;
//      if((R_verge[i]-L_verge[i])>=(R_verge[i+1]-L_verge[i+1]+1))//ͻ��
//        Mid_line[i]=Mid_line[i+1];
//      else
//        Mid_line[i]=L_verge[i]+Width[i]/2;//(L_verge[i]+col)/2;//////////////////////////////////////////////////////////////Mid_line[i]=L_verge[i]+Width[i]/2;
//     }
/////lose left
//    else if(L_verge[i]==0&&R_verge[i]!=col)
//    {
//      L_lost++;////////////////////////////////////////////////////////////////////////////////////////////
//      if((R_verge[i]-L_verge[i])>=(R_verge[i+1]-L_verge[i+1]+1))//ͻ��
//        Mid_line[i]=Mid_line[i+1];
//      else
//        Mid_line[i]=R_verge[i]-Width[i]/2;//R_verge[i]/2;/////////////////////////////////////////////////////////////Mid_line[i]=R_verge[i]-Width[i]/2;
//     }
/////lose both
//    else if(L_verge[i]==0&&R_verge[i]==col)
//    {
//      B_lost++;
//      if(i==row-1)
//        Mid_line[i]=midpoint;
//      else
//        Mid_line[i]=Mid_line[i+1];
//    }
/////
//    if(Mid_line[row-1]>=84)
//      midpoint=89;
//    else if(Mid_line[row-1]<=10)
//      midpoint=5;
//    else
//      midpoint=Mid_line[row-1];//��¼��59�е�����ֵ///////////////////�޸�
//  }
//
//  for(i=row-11;i>0;i--)///ʣ����
//  {
//    if(L_verge[i+1]!=0&&R_verge[i+1]!=col)///��һ�����߶��ҵ�           ����ɨ��
//    {
//        j=((L_verge[i+1]+10)>=col-2)?col-2:(L_verge[i+1]+10);///����߽�
//        jj=((L_verge[i+1]-5)<=1)?1:(L_verge[i+1]-5);
//        while(j>=jj)
//        {
//            if(img[i][j]==0xff&&img[i][j-1]==0x00)
//            {
//                L_verge[i]=j;
//                break;
//            }
//            j--;
//        }
//
//        j=((R_verge[i+1]-10)<=1)?1:(R_verge[i+1]-10); ///���ұ߽�
//        jj=((R_verge[i+1]+5)>=col-2)?col-2:(R_verge[i+1]+5);
//        while(j<=jj)
//        {
//            if(img[i][j]==0xff&&img[i][j+1]==0x00)
//            {
//                R_verge[i]=j;
//                break;
//            }
//            j++;
//        }
//    }
//
//
//    else if(L_verge[i+1]!=0&&R_verge[i+1]==col)///��һ��ֻ�ҵ���߽�
//    {
//        j=((L_verge[i+1]+10)>=col-2)?col-2:(L_verge[i+1]+10);///��߽��ñ���ɨ��
//        jj=((L_verge[i+1]-5)<=1)?1:(L_verge[i+1]-5);
//        while(j>=jj)
//        {
//            if(img[i][j]==0xff&&img[i][j-1]==0x00)
//            {
//                L_verge[i] = j;
//                break;
//            }
//         j--;
//      }
//      
//      j=Mid_line[i+1];///��һ�ж����ұ߽���ȫ��ɨ��///////////////////�޸�
//      if(j>=col-2)
//        j=col-2;
//      while(j<=col-2)
//        {
//            if(img[i][j]==0xff&&img[i][j+1]==0x00)
//            {
//               R_verge[i]=j;
//               break;
//            }
//         j++;
//        }
//    }
//
//
//    else if(L_verge[i+1]==0&&R_verge[i+1]!=col)///��һ��ֻ�ҵ��ұ߽�
//    {
//        j=((R_verge[i+1]-10)<=1)?1:(R_verge[i+1]-10);///��Ե׷�����ұ߽�
//        jj=((R_verge[i+1]+5)>=col-2)?col-2:(R_verge[i+1]+5);
//        while(j<=jj)
//        {
//            if(img[i][j]==0xff&&img[i][j+1]==0x00)
//            {
//                R_verge[i]=j;
//                break;
//            }
//        j++;
//        }
//        j=Mid_line[i+1];//ȫ��ɨ������߽�
//        if(j<=2)
//         j=2;
//        while(j>=1)
//        {
//            if(img[i][j]==0xff&&img[i][j-1]==0x00)
//            {
//                L_verge[i]=j;
//                break;
//            }
//        j--;
//        }
//    }
//
//
//    else if(L_verge[i+1]==0&&R_verge[i+1]==col)///��һ��û�ҵ��߽磬������ʮ�ֻ��߻���
//    {
//      j=Mid_line[i+1];///��ȫ������߽�
//        while(j>=1)
//        {
//            if(img[i][j]==0xff&&img[i][j-1]==0x00)
//            {
//                L_verge[i]=j;
//                break;
//            }
//            j--;
//        }
//        j=Mid_line[i+1];///ȫ�����ұ߽�
//        while(j<=col-2)
//        {
//            if(img[i][j]==0xff&&img[i][j+1]==0x00)
//            {
//                R_verge[i]=j;
//                break;
//            }
//            j++;
//        }
//    }
//
//    if((R_verge[i]-L_verge[i])>=(R_verge[i+1]-L_verge[i+1]+1))///����
//      Mid_line[i]=Mid_line[i+1];///����һ��
//    else
//    {
//            if(L_verge[i]!=0&&R_verge[i]!=col)///find both
//            {
//                Mid_line[i]=(L_verge[i]+R_verge[i])/2;
////                ///��б��ʮ�ֽ��о���
////                if(Mid_line[i]-Mid_line[i+1]>8&&((abs(L_verge[i]-L_verge[i+1]>3)||abs(R_verge[i]-R_verge[i+1]>3)))&&i>=30)//��������ͻ��
////                {
////                  uint8 ii=i;
////                  while(1)
////                  {
////                    Mid_line[ii+1]=Mid_line[ii]-1;
////                    ii++;
////                    if(ii>=55||(Mid_line[ii]-Mid_line[ii+1])<=1)
////                      break;
////                  }
////                }
////                if((Mid_line[i+1]-Mid_line[i]>8)&&((abs(L_verge[i]-L_verge[i+1]>3)||abs(R_verge[i]-R_verge[i+2]>3)))&&i>=30)
////                {
////                  uint8 ii=i;
////                  while(1)
////                  {
////                    Mid_line[ii+1]=Mid_line[ii]+1;
////                    ii++;
////                    if(ii>=55||(Mid_line[ii+1]-Mid_line[ii])<=1)
////                      break;
////                  }
////                }
//            }
//            else if(L_verge[i]!=0&&R_verge[i]==col)///find left
//            {
//              R_lost++;
//              if(L_verge[i+1]!=0)
//                Mid_line[i]=Mid_line[i+1]+L_verge[i]-L_verge[i+1];
//              else
//                Mid_line[i]=L_verge[i]+Width[i]/2;
//            }
//            else if(L_verge[i]==0&&R_verge[i]!=col)///find right
//            {
//              L_lost++;
//              if(R_verge[i+1]!=col)
//                Mid_line[i] = Mid_line[i+1]+R_verge[i]-R_verge[i+1];
//              else
//                Mid_line[i] = R_verge[i]-Width[i]/2;
//            }
//            else if(L_verge[i]==0&&R_verge[i]==col)///���߶���
//           {
//             WhiteNum++;
//             B_lost++;
//             if(WhiteNum == 1)
//             {
//               WhiteStart = i;
//             } 
//             Mid_line[i]=Mid_line[i+1];
//           }
//       }
//    if(i==0)
//    {
//      AvaliableLines = row;
//      LastLine = 0;
//      break;
//    }
//    uint16 m = Mid_line[i];
//    if(m<5)
//    {
//      m = 5;
//    }
//    if(m>89)
//    {
//      m = 89;
//    }
//    if((L_verge[i]!=0 && L_verge[i]>=79)||(R_verge[i]!=col && R_verge[i]<15)||(i>=1)&&(img[i-1][m]== 0x00)) //���һ��              
//    {
//      LastLine = i;//���һ�У���̬ǰհ
//      AvaliableLines = row - i;//��Ч����
//      break;
//    }
//  }
////  for(i=row-1;i>0;i--)
////    WidthReal[i] = R_verge[i] - L_verge[i];
//  
//
//}
//

void Get_centerline()//����
{
  uint8 i       = 0;
  uint8 j       = 0;
  uint8 jj      = 0;
  uint8 white_dots      = 0;
  uint8 WhiteNum        = 0;
  
  L_lost        = 0;//��������
  R_lost        = 0;
  B_lost        = 0;
  WhiteNum      = 0;
  
  L_straight    = 0;
  R_straight    = 0;

  Mid_line[row] = col/2 ;//��60��
  L_verge[row]  = 0     ;
  R_verge[row]  = col   ;
  Width[row]    = col   ;
  WidthReal[row]= col   ;
  for(i=0;i<row+1;i++)//����ֵ
  {
      R_verge[i] = col  ;
      L_verge[i] = 0    ;
      Mid_line[i]= col/2;// col/2;//47
  }

  for(j=0;j<col;j++)//�����ж�
  {
      if(img[row-2][j]==0xff||img[row-1][j]==0xff)
        white_dots++;
  }
  if(white_dots<12)
    dark_flag=1;

  for(i=row-1;i>=row-10;i--)//������ǰ10�У�ȫ��ɨ��
  {
///left
    if(i==row-1) 
      j=midpoint;
    else 
      j=Mid_line[i+1];
    if(j<=2)
      j=2;
    while(j>=3)
    {
        if(((image_myself[i][j+1] - image_myself[i][j-1]) > EdgeThres) && ((image_myself[i][j] - image_myself[i][j-2]) > EdgeThres) && (image_myself[i][j-1] < BlackThres+20))
      {
        L_verge[i]=j;//�Ҳ�������ԭֵ
        break;
      }
      j--;
    }
///right
    if(i==row-1) 
      j=midpoint;
    else
      j=Mid_line[i+1];
    if(j>=col-3) 
      j=col-3;
    while(j<=col-3)
    {
     if(((image_myself[i][j-1] - image_myself[i][j+1]) > EdgeThres) && ((image_myself[i][j] - image_myself[i][j+2]) > EdgeThres) && (image_myself[i][j+1] < BlackThres+20))	
      {
        R_verge[i]=j+1;
        break;
      }
      j++;
    }
///both
    if(L_verge[i]!=0&&R_verge[i]!=col)
      Mid_line[i]=(L_verge[i]+R_verge[i])/2;
///lose right
    else if(L_verge[i]!=0&&R_verge[i]==col)
    {
      R_lost++;
      if((R_verge[i]-L_verge[i])>(R_verge[i+1]-L_verge[i+1]+1))//ͻ��
        Mid_line[i]=Mid_line[i+1];
      else
        Mid_line[i]=(L_verge[i]+col)/2;//L_verge[i]+Width[i]/2;//(L_verge[i]+col)/2;//////////////////////////////////////////////////////////////Mid_line[i]=L_verge[i]+Width[i]/2;
     }
///lose left
    else if(L_verge[i]==0&&R_verge[i]!=col)
    {
      L_lost++;////////////////////////////////////////////////////////////////////////////////////////////
      if((R_verge[i]-L_verge[i])>=(R_verge[i+1]-L_verge[i+1]+1))//ͻ��
        Mid_line[i]=Mid_line[i+1];
      else
        Mid_line[i]=R_verge[i]/2;//R_verge[i]-Width[i]/2;//R_verge[i]/2;/////////////////////////////////////////////////////////////Mid_line[i]=R_verge[i]-Width[i]/2;
     }
///lose both
    else if(L_verge[i]==0&&R_verge[i]==col)
    {
      B_lost++;
      if(i==row-1)
        Mid_line[i]=midpoint;
      else
        Mid_line[i]=Mid_line[i+1];
    }
///
    if(Mid_line[row-1]>=84)
      midpoint=89;
    else if(Mid_line[row-1]<=10)
      midpoint=5;
    else
      midpoint=Mid_line[row-1];//��¼��59�е�����ֵ///////////////////�޸�
  }

  for(i=row-11;i>0;i--)///ʣ����
  {
    if(L_verge[i+1]!=0&&R_verge[i+1]!=col)///��һ�����߶��ҵ�           ����ɨ��
    {
        j=((L_verge[i+1]+10)>=col-2)?col-2:(L_verge[i+1]+10);///����߽�
        jj=((L_verge[i+1]-5)<=1)?1:(L_verge[i+1]-5);
        while(j>=jj)
        {
            if(image_myself[i][j+1]-image_myself[i][j-1]>EdgeThres&&(image_myself[i][j-1] < BlackThres+20))
            {
                L_verge[i]=j;
                break;
            }
            j--;
        }

        j=((R_verge[i+1]-10)<=1)?1:(R_verge[i+1]-10); ///���ұ߽�
        jj=((R_verge[i+1]+5)>=col-2)?col-2:(R_verge[i+1]+5);
        while(j<=jj)
        {
            if(image_myself[i][j-1]-image_myself[i][j+1]>EdgeThres&&(image_myself[i][j+1] < BlackThres+20))
            {
                R_verge[i]=j;
                break;
            }
            j++;
        }
    }


    else if(L_verge[i+1]!=0&&R_verge[i+1]==col)///��һ��ֻ�ҵ���߽�
    {
        j=((L_verge[i+1]+10)>=col-2)?col-2:(L_verge[i+1]+10);///��߽��ñ���ɨ��
        jj=((L_verge[i+1]-5)<=1)?1:(L_verge[i+1]-5);
        while(j>=jj)
        {
           if(image_myself[i][j+1]-image_myself[i][j-1]>EdgeThres&&(image_myself[i][j-1] < BlackThres+20))
            {
                L_verge[i] = j;
                break;
            }
         j--;
      }
      
      j=Mid_line[i+1];///��һ�ж����ұ߽���ȫ��ɨ��///////////////////�޸�
      if(j>=col-2)
        j=col-2;
      while(j<=col-2)
        {
           if(image_myself[i][j-1]-image_myself[i][j+1]>EdgeThres&&(image_myself[i][j+1] < BlackThres+20))
            {
               R_verge[i]=j;
               break;
            }
         j++;
        }
    }


    else if(L_verge[i+1]==0&&R_verge[i+1]!=col)///��һ��ֻ�ҵ��ұ߽�
    {
        j=((R_verge[i+1]-10)<=1)?1:(R_verge[i+1]-10);///��Ե׷�����ұ߽�
        jj=((R_verge[i+1]+5)>=col-2)?col-2:(R_verge[i+1]+5);
        while(j<=jj)
        {
            if(image_myself[i][j-1]-image_myself[i][j+1]>EdgeThres&&(image_myself[i][j+1] < BlackThres+20))
            {
                R_verge[i]=j;
                break;
            }
        j++;
        }
        j=Mid_line[i+1];//ȫ��ɨ������߽�
        if(j<=2)
         j=2;
        while(j>=1)
        {
          if(image_myself[i][j+1]-image_myself[i][j-1]>EdgeThres&&(image_myself[i][j-1] < BlackThres+20))
            {
                L_verge[i]=j;
                break;
            }
        j--;
        }
    }


    else if(L_verge[i+1]==0&&R_verge[i+1]==col)///��һ��û�ҵ��߽磬������ʮ�ֻ��߻���
    {
      j=Mid_line[i+1];///��ȫ������߽�
        while(j>=1)
        {
            if(image_myself[i][j+1]-image_myself[i][j-1]>EdgeThres&&(image_myself[i][j-1] < BlackThres+20))
            {
                L_verge[i]=j;
                break;
            }
            j--;
        }
        j=Mid_line[i+1];///ȫ�����ұ߽�
        while(j<=col-2)
        {
             if(image_myself[i][j-1]-image_myself[i][j+1]>EdgeThres&&(image_myself[i][j+1] < BlackThres+20))
            {
                R_verge[i]=j;
                break;
            }
            j++;
        }
    }

    if((R_verge[i]-L_verge[i])>(R_verge[i+1]-L_verge[i+1]+1))///����
      Mid_line[i]=Mid_line[i+1];///����һ��
    else
    {
            if(L_verge[i]!=0&&R_verge[i]!=col)///find both
            {
                Mid_line[i]=(L_verge[i]+R_verge[i])/2;
            }
            else if(L_verge[i]!=0&&R_verge[i]==col)///find left
            {
              R_lost++;
              if(L_verge[i+1]!=0)
                Mid_line[i]=Mid_line[i+1]+L_verge[i]-L_verge[i+1];
              else
                Mid_line[i]=L_verge[i]+Width[i]/2;
            }
            else if(L_verge[i]==0&&R_verge[i]!=col)///find right
            {
              L_lost++;
              if(R_verge[i+1]!=col)
                Mid_line[i] = Mid_line[i+1]+R_verge[i]-R_verge[i+1];
              else
                Mid_line[i] = R_verge[i]-Width[i]/2;
            }
            else if(L_verge[i]==0&&R_verge[i]==col)///���߶���
           {
             WhiteNum++;
             B_lost++;
             Mid_line[i]=Mid_line[i+1];
           }
       }
  }
  
  for(i=row-1;i>0;i--)
    WidthReal[i] = R_verge[i] - L_verge[i];
  
      for (i = 60; i > 2 ; i-- )
    {
         if( R_verge[i] >= R_verge[i - 2]&&R_verge[i] - R_verge[i - 2] < 5&&R_verge[i]!=94)
            R_straight++;
         if( L_verge[i] <= L_verge[i - 2]&&L_verge[i - 2] - L_verge[i] < 5&&L_verge[i]!=0 )
            L_straight++;
     }
}

//��ȡͼ�������
//ѡȡ���У���ͼ��ײ�����ɨ��
//�ڲ�����
void GetBlackEndParam()//��ȡ���߽�ֹ��
{
  uint8 LEndFlag  = 0;//��־λ
  uint8 MEndFlag  = 0;
  uint8 REndFlag  = 0;
  uint8 MREndFlag = 0;
  uint8 MLEndFlag = 0;
  uint8 LLEndFlag = 0;
  uint8 RREndFlag = 0;
  
  int i=0;

  BlackEndMR   = 0;//����
  BlackEndML   = 0;
  BlackEndLL   = 0;
  BlackEndRR   = 0;
  BlackEndL    = 0;
  BlackEndM    = 0;
  BlackEndR    = 0;
  for (i = row-1; i >= 3 ; i--)
  {
	if(img[i][col/2] == 0xff && !MEndFlag )//!MEndFlag=1 //40
        {
          BlackEndM++;//�к��߽�����
        }
	else if(i > 1 && img[i-1][col/2] == 0x00 && img[i-2][col/2] == 0x00)//���������Ǻ�ɫ        
        {
          MEndFlag = 1;
        }
	if(img[i][23] == 0xff && !LEndFlag )//20
        {
          BlackEndL++;//����߽�����
        }
	else if(i > 1 && img[i-1][23] == 0x00 && img[i-2][23] == 0x00)
        {
          LEndFlag = 1;
        }
	if(img[i][71] == 0xff && !REndFlag )//60
        {
          BlackEndR++;//�Һ��߽�����
	}
	else if(i > 1 && img[i-1][71] == 0x00 && img[i-2][71] == 0x00)
        {
          REndFlag = 1;
        }
        if(img[i][44] == 0xff && !MLEndFlag )
        {
          BlackEndML++;//
        }
	else if(i > 1 && img[i-1][35] == 0x00 && img[i-2][35] == 0x00)
        {
          MLEndFlag = 1;
        }
	if(img[i][50] == 0xff && !MREndFlag )
        {
          BlackEndMR++;//
        }
	else if(i > 1 && img[i-1][59] == 0x00 && img[i-2][59] == 0x00)
        {
          MREndFlag = 1;
	}
        if(img[i][11] == 0xff && !LLEndFlag )
        {
          BlackEndLL++;
	}
	else if(i > 1 && img[i-1][11] == 0x00 && img[i-2][11] == 0x00)
        {
          LLEndFlag = 1;
	}
        if(img[i][83] == 0xff && !RREndFlag )
        {
          BlackEndRR++;
	}
	else if(i > 1 && img[i-1][83] == 0x00 && img[i-2][83] == 0x00)
        {
          RREndFlag = 1;
	}
   }            

        BlackEndMax =MAX(BlackEndL,BlackEndM);//ȡ��ֵ
        BlackEndMax =MAX(BlackEndMax,BlackEndR);
        BlackEndMaxMax =MAX(BlackEndMax,BlackEndMR);
        BlackEndMaxMax =MAX(BlackEndMax,BlackEndML);
        BlackEndMaxMax =MAX(BlackEndMax,BlackEndLL);
        BlackEndMaxMax =MAX(BlackEndMax,BlackEndRR);
        if(BlackEndMaxMax>=60)
        {
            BlackEndMaxMax=58;
        }
        DropRow=row-BlackEndMaxMax;//�ⶥ������         
 }

void FindInflectionPoint(void)
{
  
 int8 i=0;
 int8 j=0;

 float mid_Sum = 0.0 ;
  //��������
 uint8 Left_Black[8]={0};
 uint8 Right_Black[8]={94,94,94,94,94,94,94,94};
 uint8 Mid_Black[8]={47,47,47,47,47,47,47,47};
 uint8 RightInflectionPointRow=0;//�ұߵĹյ��������
 uint8 RightInflectionPointCol=0;//�ұߵĹյ��������
 uint8 LeftInflectionPointRow=0;//��ߵĹյ��������
 uint8 LeftInflectionPointCol=0;//��ߵĹյ��������
 uint8 RightInflectionPointFlag=0;//�ҵ���յ�ı�־λ
 uint8 LeftInflectionPointFlag=0;//�ҵ��ҹյ�ı�־λ
 
 uint8 StartRow=0;
 uint8 StartCol=47;
 uint8 EndRow=0;
 uint8 EndCol=47;


 
 
  //�ҹյ�
  
  for(i=59;i>=35;i--)//����ɨ��̫Զ�����������
  {
     if(R_verge[i+1]!=col&&(R_verge[i+1]>R_verge[i-1])&&(R_verge[i]>R_verge[i-2])) //�������в�����
     {
     
      if((R_verge[i-2]-R_verge[i-7]<-10)||(R_verge[i-1]-R_verge[i-6]<-10)||(R_verge[i-3]-R_verge[i-8]<-10)||(R_verge[i-4]-R_verge[i-9]<-10))//�ҵ��ұ����йյ�
       {
         
           RightInflectionPointRow=i;//��¼�յ����
           RightInflectionPointCol=R_verge[i];//��¼�յ����
           RightInflectionPointFlag=1;//����ҵ��ҹյ�
           break;//�˳�for
       }
      
       }
  }
  
    //��յ�
   for(i=59;i>35;i--) 
   {
     if(L_verge[i+1]!=0&&(L_verge[i+1]<L_verge[i-1])&&(L_verge[i]<L_verge[i-2]))
     {
     
      if((L_verge[i-2]-L_verge[i-7]>10)||(L_verge[i-1]-L_verge[i-6]>10)||(L_verge[i-3]-L_verge[i-8]>10)||(L_verge[i-4]-L_verge[i-9]>10))//�ҵ��յ�
       {
           LeftInflectionPointRow=i;//��¼�ùյ����
           LeftInflectionPointCol=L_verge[i];//��¼�ùյ����
           LeftInflectionPointFlag=1;//����ҵ���յ�
           break;//�ҵ��˳�
       }
       }
   }  
 if(LeftInflectionPointFlag&&RightInflectionPointFlag)//ͬʱ�ҵ������յ�
  {
    CrossFlag1=1;
 
    
//     StartCol=(int)((LeftInflectionPointCol+RightInflectionPointCol)/2);// ȡ���ҹյ��������ƽ��ֵ
//    
//     StartRow=(int)((LeftInflectionPointRow+RightInflectionPointRow)/2);//ȡ���ҹյ���������ƽ��ֵ
//    
//    for(i=8;i>2;i--)//����ͼ���ÿһ�ж�����ɨ��
//    {
//      for(j=47;j>27;j--)//���м������������Ѱ�Һڵ�
//      {
//        if(img[i][j]==0xff&&img[i][j-1]==0x00)
//        {
//          Left_Black[i]=j;//�ҵ���ߺڵ�
//            break;
//        }
//        else
//          Left_Black[i]=0;
//      }
//      for(j=47;j<67;j++)//���м����ұ�������Ѱ�Һڵ�
//      {
//        if(img[i][j]==0xff&&img[i][j+1]==0x00)
//        {
//          Right_Black[i]=j;//�ҵ��ұߺڵ�
//            break;
//        }
//        else
//          Right_Black[i]=94;
//      }
//      Mid_Black[i]=(Right_Black[i]-Left_Black[i])/2;
//    }
//    for(i=8;i>2;i--)
//      mid_Sum += Mid_Black[i];
//    
//    mid_Sum=mid_Sum/6;
//    
//    EndRow=4;
//    EndCol=(int)(mid_Sum);
//    
//    for(i=StartRow;i>EndRow;i--)  
//    Mid_line[i]=(EndCol+StartCol)/2;
   
  }
     if(CrossFlag1==1&&img[58][2]==0xff&&img[58][92]==0xff)
      CrossFlag1=0;
}

//��ʮ�ֽ���ʶ��ͽ������⴦��
void NormalCrossConduct(void)
{
  uint8 i=0;
    uint8 j=0;

  //CrossFlag=0;//����
  //CrossFlag1=0;

  FindInflectionPoint();
  
  if(CrossFlag1==1&&((B_lost>=18)&&(abs(L_lost-R_lost)<10)&&(BlackEndMaxMax>=50)))//����ͬʱ����
  {
    CrossFlag1=2;//���ʮ��
  }                         
//  for(i=58;i>30;i--)
//        if(WidthReal[i]>80)
//          j++;
//  
//  if(j<=5)
//    CrossFlag1=0;
//  if(CrossFlag1==2)
//  {
//    
//    for(i=58;i>20;i--)
//        Mid_line[i]=47;
//    
//    
////    if((BlackEndM>=BlackEndL)&&(BlackEndM>BlackEndMR))//�м�������////(abs(Mid_line[1]-47)!=0)&&(abs(Mid_line[1]-47)<=6)///(BlackEndM>=BlackEndL)&&(BlackEndM>=BlackEndR)
////    {
////      for(i=58;i>20;i--)
////        Mid_line[i]=col/2;//ֱ��
////    }
////    else if((BlackEndML>BlackEndM)&&(BlackEndML>BlackEndMR))//������߰׵���࣬����ƫ���ˣ�������///Mid_line[1]-47>6
////    {
////      for(i=58;i>20;i--)
////        Mid_line[i]=42;
////    }
////    else if((BlackEndMR>BlackEndM)&&(BlackEndMR>BlackEndML))//�����ұ߰׵���࣬����ƫ���ˣ�������
////    {
////      for(i=58;i>20;i--)
////        Mid_line[i]=52;
////    }
//  }
}

void Wave_filtering(void)//��������
{
  uint8 k;
  for(k=row-4;k>3;k--)
  {
    if(abs(Mid_line[k]-Mid_line[k-3])>5||abs(Mid_line[k]-Mid_line[k+3])>5)
      Mid_line[k]=(Mid_line[k-3]+Mid_line[k+3]+Mid_line[k-2]+Mid_line[k+2])/4;
  }
}

//void Ending(void)//////////////////////�޸�
//{
//  uint8 i=0;
//  uint8 j=0;
//  uint8 jump_point=0;
//  for(i=row-16;i<row-8;i++)
//  {
//    for(j=0;j<col-1;j++)
//    {
//      if((img[i][j]==0xff&&img[i][j+1]==0x00)||(img[i][j]==0x00&&img[i][j+1]==0xff))
//        jump_point++;
//    }
//  }
//    if(jump_point>=45&&s>12)
//    {
//      ending_flag=2;
//    }
//    else if(jump_point>=45)
//    {
//      ending_flag=1;
//    }
//}



uint8 ending_flag = 0;///==2�յ���
extern uint8 garage_direction = 0;///1Ϊ���,2λ�ұ�
int right_garage_left_turn_down[2]={0,0};
int right_garage_right_turn_up[2]={0,0};///ǰһ���������꣬��һ����������
int left_garage_right_turn_down[2];
int left_garage_left_turn_up[2];///ǰһ���������꣬��һ����������


///������
uint8 max_black=0;
void check_starting_line(uint8 start_point, uint8 end_point)
{
  uint8 i=0;
  uint8 j=0;
  uint8 jump_point[10]={0};
  for(i=start_point;i<end_point;i++)
  {
    for(j=15;j<col-15;j++)
    {
      if((img[i][j]==0xff&&img[i][j+1]==0x00)||(img[i][j]==0x00&&img[i][j+1]==0xff))
      {
        jump_point[i-start_point]++;
      }
    }
  }
  
    max_black = jump_point[0];
    for(i = 0;i < end_point-start_point; i++)
  {
  if(jump_point[i] > max_black)
    {
      max_black = jump_point[i];
    }
  }
  
    if(max_black>=10&&s>1)////12//14
    {
      ending_flag=2;
    }
//    else if(ending_flag=2&&max_black<4)
//    {
//      ending_flag=1;
//    }
}

void Start_line_search(uint8 (*data)[COL_W])
{
	int i,j;
	int tiao_point=0;
	int R_white_point=0,L_white_point=0;
	for(i=35;i<38;i++) //40 43
	{
		for(j=20;j<74;j++)
		{
			if(abs(data[i][j+1]-data[i][j])>EdgeThres)
			{
				tiao_point++;
			}
		}
	}

	if(tiao_point>=40)
        {
          for(j=30;j<45;j++)
          {
            if(data[j][10]>(BlackThres-5))
              L_white_point++;
            if(data[j][84]>(BlackThres-5))
              R_white_point++;
          }
          
          if((R_white_point>=10&&start_fl==2)||(L_white_point>=10&&start_fl==1))
                ending_flag=2;

        }
}


///[�ҳ���]
//int jicun;
//float k_1;
//float b_1;




void check_guaidian()
{
    ///���������¹յ�
    uint8 y1 = 0;
    uint8 x1 = 0;
    uint8 x = 0;
    uint8 y = 0;
        
    int jicun;
    float k_1;
    float b_1;
    /// 1��	�����Ϊ�ף������ң�������5�У���ֱ��Ϊ�ڣ���ֹ��̬��ԭ���³�ƫ���·��ߣ�
//    if (img[row-1][0] != 0)   //��Ϊ��,��Ϊ��
//    {
//        for (y = row-2; y < row-7; y--)
//        {
//            if (img[y][0] == 0)
//            {
//                y1 = y;
//                break;
//            }
//        }
//    }
//    else
//    {
//        y1 = row-1;
//    }
//    ///2��	������ȷ���ĵ�y1�п�ʼ���ұ�ɨ��ɨ���ĵ�һ����Ϊ�ڵĵ���Ϊ���¹յ㡣
//
//    for (x = 0;x<col-1;x++)
//    {
//        if (img[y1][x] != 0)
//        {
//            x1 = x;
//            break;
//        }
//    }
    //��ֵ����
    right_garage_left_turn_down[0] = 59;//y1
    right_garage_left_turn_down[1] = 0;//x1
    ///Ȼ�������Ϲյ�

//    if (img[row-1][col-1] == 0 && img[row-2][col-1] == 0 && img[row-3][col-1] == 0 && img[row-4][col-1] == 0)
//    {
//        for (y = row-1; y > 25; y--)///
//        {
//            if (img[y][col-1] != 0)//Ϊ��
//            {
//                y1 = y;
//                break;
//            }
//
//        }
//        for (y = y1; y > 25; y--)
//        {
//            if (img[y][col-1] == 0)
//            {
//                y1 = y;
//                break;
//            }
//
//        }
//    }
//    else
//    {
        for (y = row-8; y > 10; y--)
        {
            if (img[y+5][col-1] == 255&&img[y][col-1] == 255&&img[y-1][col-1] == 0)
            {
                y1 = y;
                break;
            }
        }
//    }
    ///2��	������ȷ���ĵ�y1�п�ʼ�����ɨ����¼��ɨ���ĵ�һ��Ϊ�׵ĵ��������
  if(y1)
    for (x = col-1; x > 4; x--)
    {
        if (img[y1-1][x] == 0&&img[y1-1][x-1] == 255)//&&img[y1-1][x-4] == 255)
        {
            x1 = x;
            break;
        }
    }
    uint8 y_zhengque = 0;
    ///3��	����[y2, y2 + 2],ѡ��6���������������󣩵��������Լ���ʱ�������꣬��Ϊ���Ϲյ�
    uint8 Y = 0;
    Y = y1 - 6;
    Y = Y>=0 ? Y : 0;
    for (y = y1-1 ; y >= Y; y--)
    {
        uint8 X = 0;
        for (x = col-1; x > 0; x--)
        {
            if (img[y][x] == 0&&img[y][x-1] == 255)
            {
                X = x;
                break;
            }
        }
        if (X <= x1)
        {
            x1 = X;
            y_zhengque = y;
        }
    }
    y1 = y_zhengque;
    //��ֵ����
    right_garage_right_turn_up[0] = y1;
    right_garage_right_turn_up[1] = x1;

    

    //���߳���
    
      
    uint8 j = 0;
    int delta = right_garage_left_turn_down[0] - right_garage_right_turn_up[0];
    if (delta == 0) delta = 1;
    k_1 = (right_garage_right_turn_up[1] - right_garage_left_turn_down[1]) * 1.0/ delta;
    b_1 = right_garage_left_turn_down[1];//0
    for (j = right_garage_left_turn_down[0]; j >= right_garage_right_turn_up[0]; j--)
    {
        jicun = (int)(col - k_1 * j);
        if (jicun >= col-1) 
          jicun = col-1;
        else if (jicun <= 0) 
          jicun = 0;
        
        L_verge[j] = (int8)jicun;
        Mid_line[j]=L_verge[j]+col/2;
//        if(Mid_line[j]<47)
//          Mid_line[j]=col-Mid_line[j];
    }
}
///[�󳵿�]
void check_guaidian1()
{
  //pwm_duty(PWM4_MODULE2_CHA_C30, Servo_pwm_mid+50);
}


void Garage_enter(void)
{
  if(SUM_PulseFTM>3500)//((j_L==0&&i_R==0)||SUM_PulseFTM>3800&&BlackThres<90)//
    garage_enter=1;
}

//  uint16 i=0,j=0,k=0,l=58,r=58;
//  for(i=58;i>25;i--)//������������
//  {
//    if(L_verge[i]>L_verge[l])
//      l=i;
//    if(img[i][L_verge[i]+3]!=255)
//      break;
//  }
//  for(j=58;j>25;j--)//�ұ�����С����
//  {
//    if(R_verge[j]<R_verge[r])
//      r=j;
//    if(img[j][R_verge[j]-3]!=255)
//      break;
//  }
//  if(i!=25&&j!=25&&abs(r-l)<=8)
//  {
//    if(r>l)
//    {
//      k=r;
//      r=l;
//      l=k;
//    }//r��Сl���
//    for(i=r-4,k=0;i>r-6;i--)//����ȫ��
//      for(j=0;j<col;j++)
//        if(img[i][j]==0)
//          k++;
//    if(k>100)
//    {
//      for(i=l,j=0;i<l+8;i++)
//        if(L_verge[i]!=0&&R_verge[i]!=col)
//          j++;
//      
//      if(j==8)
//        garage_enter=1;
//    }
//  }


void Ending(void)
{
  uint8 j=0;
 // check_starting_line(row-18, row-8);//
  Start_line_search(image_myself);
//  check_garage_direction(row-16, row-8);///.///.//
//  if(garage_direction==1)
//    //check_guaidian1();
//  else if(garage_direction==2)
//  {
//  if(ending_flag==2)
//    check_guaidian();
//    for (j = row-1; j >= 1 ; j-- )
//      if(Mid_line[j]<47)
//        Mid_line[j]=col-Mid_line[j];
//  }
 if(ending_flag==2)
  Garage_enter();
}


//L_lost==0
//R_lost==0
//B_lost==0
//endmax

void Image_Processing()
{
  BlackThres = otsuThreshold(image_myself[0],94,60);//image_threshold = (uint8)BlackThres;//
  
  Image_Binarization();
  //Image_Denoising();
  GetBlackEndParam(); //��ȡ���߽�ֹ��
  Get_centerline();
  
  //NormalCrossConduct();//���������У�����������������־��//ʮ��ɨ��
  //FindInflectionPoint();
  if(hw_flag!=1&&hw_flag!=2)
    Ending();
}

 
/*******************************************************************************
�������ƣ�regression���ع飩
��������: ��С���˷���б��
������
*******************************************************************************/
int regression(int Pick_table[],int startline,int endline)//����б�ʺ���//Pick_table[]
{
    int num=0,i;
    int sumX=0,sumY=0,avrX=0,avrY=0;
    int B_up1=0,B_up2=0;
    int B_up=0,B_down=0;
    int slope=0;
    for(i=startline;i<=endline;i++)
    {
         if(Pick_table[i]) 
         {
            num++;
            sumX+=i;
            sumY+=Pick_table[i];
         }
    }
    avrX=sumX/num;
    avrY=sumY/num;
    for(i=startline;i<=endline;i++)
    {
	         if(Pick_table[i]) 
	         { 
	            B_up1=(int)Pick_table[i]-(int)avrY;
	            B_up2=i-avrX;
	            B_up+=(int)B_up1*(int)B_up2;
	            //B_up=B_up/100*100;
	            B_down+=(int)(i-avrX)*(int)(i-avrX);
	         }
   }
   if(B_down==0) slope=0;
   else slope=B_up*10/B_down;
   return slope;
}
/****************************************
����������ж�
����: �ж�Բ���ĵ�һ�����
������
**************************************/
void round_flag1(void)//�жϵ�һ�����
{
  uint8 i;
  flag=0;
  
  if(round_flag==0)
    for(i=59;i>35;i--)//��Բ��
    {
      findlin_ri(i);
      if(lin[0]!=0&&lin[1]!=0)
      {
        hei=lin[1]-lin[0];
       if(hei>=7)
       {
         findlin_ri(i-1);
         if(lin[0]!=0&&lin[1]!=0)
         {
             round_flag=1;break;
             
          } 
        }
       }
     }
  
  if(round_flag==0)
    for(i=59;i>35;i--)//��Բ��
    {
      findlin_le(i);
      if(lin[0]!=0&&lin[1]!=0)
      {
        hei=lin[0]-lin[1];
        if(hei>=7)
        { 
          findlin_le(i-1);
           if(lin[0]!=0&&lin[1]!=0)
           {
             if(hei>(lin[0]-lin[1])&&(lin[0]-lin[1])>=5)
              {       
                
                
                  round_flag=11;break;
                
              }
           }
         }
       }
     }
}

  
  


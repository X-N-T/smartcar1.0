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

int     LeftLoseStart   = 0     ;//记录左边丢线的开始行
int     RightLoseStart  = 0     ;//记录右边边丢线的开始行
//int     WhiteStart      = 0     ;
int     LastLine        = 0     ;
float   AvaliableLines  = 0     ;
uint8   dark_flag       = 0     ; //眼前一片黑
//out
//uint8   ending_flag     = 0     ; //终点线
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

uint8   CrossFlag1       = 0    ;//十字标志1
uint8   CrossFlag        = 0    ;//十字标志0

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
    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }
    uint32 gray_sum=0;
    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
        }
    }
    //计算每个像素值的点在整幅图像中的比例
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }
    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {
                w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
                u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值
               w1=1-w0;
               u1tmp=gray_sum/pixelSum-u0tmp;
                u0 = u0tmp / w0;              //背景平均灰度
                u1 = u1tmp / w1;              //前景平均灰度
                u = u0tmp + u1tmp;            //全局平均灰度
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
/********不加偏振片限幅***********/
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

//void Get_centerline()//中线
//{
//  uint8 i       = 0;
//  uint8 j       = 0;
//  uint8 jj      = 0;
//  uint8 white_dots      = 0;
//  uint8 WhiteNum        = 0;
//  
//  L_lost        = 0;//变量清零
//  R_lost        = 0;
//  B_lost        = 0;
//  WhiteNum      = 0;
//  
//
//
//  Mid_line[row] = col/2 ;//第60行
//  L_verge[row]  = 0     ;
//  R_verge[row]  = col   ;
//  Width[row]    = col   ;
////  WidthReal[row]=col   ;
//  for(i=0;i<row+1;i++)//赋初值
//  {
//      R_verge[i] = col  ;
//      L_verge[i] = 0    ;
//      Mid_line[i]= col/2;// col/2;//47
//  }
//
//  for(j=0;j<col;j++)//出界判断
//  {
//      if(img[row-2][j]==0xff||img[row-1][j]==0xff)
//        white_dots++;
//  }
//  if(white_dots<12)
//    dark_flag=1;
//
//  for(i=row-1;i>=row-10;i--)//首先找前10行，全行扫描
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
//      if(img[i][j]==0xff&&img[i][j-1]==0x00&&img[i][j-2]==0x00)//从右向左
//      {
//        L_verge[i]=j;//找不到保持原值
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
//      if((R_verge[i]-L_verge[i])>=(R_verge[i+1]-L_verge[i+1]+1))//突变
//        Mid_line[i]=Mid_line[i+1];
//      else
//        Mid_line[i]=L_verge[i]+Width[i]/2;//(L_verge[i]+col)/2;//////////////////////////////////////////////////////////////Mid_line[i]=L_verge[i]+Width[i]/2;
//     }
/////lose left
//    else if(L_verge[i]==0&&R_verge[i]!=col)
//    {
//      L_lost++;////////////////////////////////////////////////////////////////////////////////////////////
//      if((R_verge[i]-L_verge[i])>=(R_verge[i+1]-L_verge[i+1]+1))//突变
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
//      midpoint=Mid_line[row-1];//记录第59行的中线值///////////////////修改
//  }
//
//  for(i=row-11;i>0;i--)///剩余行
//  {
//    if(L_verge[i+1]!=0&&R_verge[i+1]!=col)///上一行两边都找到           边沿扫描
//    {
//        j=((L_verge[i+1]+10)>=col-2)?col-2:(L_verge[i+1]+10);///找左边界
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
//        j=((R_verge[i+1]-10)<=1)?1:(R_verge[i+1]-10); ///找右边界
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
//    else if(L_verge[i+1]!=0&&R_verge[i+1]==col)///上一行只找到左边界
//    {
//        j=((L_verge[i+1]+10)>=col-2)?col-2:(L_verge[i+1]+10);///左边界用边沿扫描
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
//      j=Mid_line[i+1];///上一行丢了右边界用全行扫描///////////////////修改
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
//    else if(L_verge[i+1]==0&&R_verge[i+1]!=col)///上一行只找到右边界
//    {
//        j=((R_verge[i+1]-10)<=1)?1:(R_verge[i+1]-10);///边缘追踪找右边界
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
//        j=Mid_line[i+1];//全行扫描找左边界
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
//    else if(L_verge[i+1]==0&&R_verge[i+1]==col)///上一行没找到边界，可能是十字或者环形
//    {
//      j=Mid_line[i+1];///找全行找左边界
//        while(j>=1)
//        {
//            if(img[i][j]==0xff&&img[i][j-1]==0x00)
//            {
//                L_verge[i]=j;
//                break;
//            }
//            j--;
//        }
//        j=Mid_line[i+1];///全行找右边界
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
//    if((R_verge[i]-L_verge[i])>=(R_verge[i+1]-L_verge[i+1]+1))///畸变
//      Mid_line[i]=Mid_line[i+1];///用上一行
//    else
//    {
//            if(L_verge[i]!=0&&R_verge[i]!=col)///find both
//            {
//                Mid_line[i]=(L_verge[i]+R_verge[i])/2;
////                ///对斜出十字进行纠正
////                if(Mid_line[i]-Mid_line[i+1]>8&&((abs(L_verge[i]-L_verge[i+1]>3)||abs(R_verge[i]-R_verge[i+1]>3)))&&i>=30)//中线向右突变
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
//            else if(L_verge[i]==0&&R_verge[i]==col)///两边丢线
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
//    if((L_verge[i]!=0 && L_verge[i]>=79)||(R_verge[i]!=col && R_verge[i]<15)||(i>=1)&&(img[i-1][m]== 0x00)) //最后一行              
//    {
//      LastLine = i;//最后一行，动态前瞻
//      AvaliableLines = row - i;//有效行数
//      break;
//    }
//  }
////  for(i=row-1;i>0;i--)
////    WidthReal[i] = R_verge[i] - L_verge[i];
//  
//
//}
//

void Get_centerline()//中线
{
  uint8 i       = 0;
  uint8 j       = 0;
  uint8 jj      = 0;
  uint8 white_dots      = 0;
  uint8 WhiteNum        = 0;
  
  L_lost        = 0;//变量清零
  R_lost        = 0;
  B_lost        = 0;
  WhiteNum      = 0;
  
  L_straight    = 0;
  R_straight    = 0;

  Mid_line[row] = col/2 ;//第60行
  L_verge[row]  = 0     ;
  R_verge[row]  = col   ;
  Width[row]    = col   ;
  WidthReal[row]= col   ;
  for(i=0;i<row+1;i++)//赋初值
  {
      R_verge[i] = col  ;
      L_verge[i] = 0    ;
      Mid_line[i]= col/2;// col/2;//47
  }

  for(j=0;j<col;j++)//出界判断
  {
      if(img[row-2][j]==0xff||img[row-1][j]==0xff)
        white_dots++;
  }
  if(white_dots<12)
    dark_flag=1;

  for(i=row-1;i>=row-10;i--)//首先找前10行，全行扫描
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
        L_verge[i]=j;//找不到保持原值
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
      if((R_verge[i]-L_verge[i])>(R_verge[i+1]-L_verge[i+1]+1))//突变
        Mid_line[i]=Mid_line[i+1];
      else
        Mid_line[i]=(L_verge[i]+col)/2;//L_verge[i]+Width[i]/2;//(L_verge[i]+col)/2;//////////////////////////////////////////////////////////////Mid_line[i]=L_verge[i]+Width[i]/2;
     }
///lose left
    else if(L_verge[i]==0&&R_verge[i]!=col)
    {
      L_lost++;////////////////////////////////////////////////////////////////////////////////////////////
      if((R_verge[i]-L_verge[i])>=(R_verge[i+1]-L_verge[i+1]+1))//突变
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
      midpoint=Mid_line[row-1];//记录第59行的中线值///////////////////修改
  }

  for(i=row-11;i>0;i--)///剩余行
  {
    if(L_verge[i+1]!=0&&R_verge[i+1]!=col)///上一行两边都找到           边沿扫描
    {
        j=((L_verge[i+1]+10)>=col-2)?col-2:(L_verge[i+1]+10);///找左边界
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

        j=((R_verge[i+1]-10)<=1)?1:(R_verge[i+1]-10); ///找右边界
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


    else if(L_verge[i+1]!=0&&R_verge[i+1]==col)///上一行只找到左边界
    {
        j=((L_verge[i+1]+10)>=col-2)?col-2:(L_verge[i+1]+10);///左边界用边沿扫描
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
      
      j=Mid_line[i+1];///上一行丢了右边界用全行扫描///////////////////修改
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


    else if(L_verge[i+1]==0&&R_verge[i+1]!=col)///上一行只找到右边界
    {
        j=((R_verge[i+1]-10)<=1)?1:(R_verge[i+1]-10);///边缘追踪找右边界
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
        j=Mid_line[i+1];//全行扫描找左边界
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


    else if(L_verge[i+1]==0&&R_verge[i+1]==col)///上一行没找到边界，可能是十字或者环形
    {
      j=Mid_line[i+1];///找全行找左边界
        while(j>=1)
        {
            if(image_myself[i][j+1]-image_myself[i][j-1]>EdgeThres&&(image_myself[i][j-1] < BlackThres+20))
            {
                L_verge[i]=j;
                break;
            }
            j--;
        }
        j=Mid_line[i+1];///全行找右边界
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

    if((R_verge[i]-L_verge[i])>(R_verge[i+1]-L_verge[i+1]+1))///畸变
      Mid_line[i]=Mid_line[i+1];///用上一行
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
            else if(L_verge[i]==0&&R_verge[i]==col)///两边丢线
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

//提取图像的特征
//选取几列，从图像底部往上扫描
//内部调用
void GetBlackEndParam()//获取黑线截止行
{
  uint8 LEndFlag  = 0;//标志位
  uint8 MEndFlag  = 0;
  uint8 REndFlag  = 0;
  uint8 MREndFlag = 0;
  uint8 MLEndFlag = 0;
  uint8 LLEndFlag = 0;
  uint8 RREndFlag = 0;
  
  int i=0;

  BlackEndMR   = 0;//清零
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
          BlackEndM++;//中黑线截至行
        }
	else if(i > 1 && img[i-1][col/2] == 0x00 && img[i-2][col/2] == 0x00)//连续两行是黑色        
        {
          MEndFlag = 1;
        }
	if(img[i][23] == 0xff && !LEndFlag )//20
        {
          BlackEndL++;//左黑线截至行
        }
	else if(i > 1 && img[i-1][23] == 0x00 && img[i-2][23] == 0x00)
        {
          LEndFlag = 1;
        }
	if(img[i][71] == 0xff && !REndFlag )//60
        {
          BlackEndR++;//右黑线截至行
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

        BlackEndMax =MAX(BlackEndL,BlackEndM);//取大值
        BlackEndMax =MAX(BlackEndMax,BlackEndR);
        BlackEndMaxMax =MAX(BlackEndMax,BlackEndMR);
        BlackEndMaxMax =MAX(BlackEndMax,BlackEndML);
        BlackEndMaxMax =MAX(BlackEndMax,BlackEndLL);
        BlackEndMaxMax =MAX(BlackEndMax,BlackEndRR);
        if(BlackEndMaxMax>=60)
        {
            BlackEndMaxMax=58;
        }
        DropRow=row-BlackEndMaxMax;//封顶的行数         
 }

void FindInflectionPoint(void)
{
  
 int8 i=0;
 int8 j=0;

 float mid_Sum = 0.0 ;
  //变量清零
 uint8 Left_Black[8]={0};
 uint8 Right_Black[8]={94,94,94,94,94,94,94,94};
 uint8 Mid_Black[8]={47,47,47,47,47,47,47,47};
 uint8 RightInflectionPointRow=0;//右边的拐点的行坐标
 uint8 RightInflectionPointCol=0;//右边的拐点的列坐标
 uint8 LeftInflectionPointRow=0;//左边的拐点的行坐标
 uint8 LeftInflectionPointCol=0;//左边的拐点的列坐标
 uint8 RightInflectionPointFlag=0;//找到左拐点的标志位
 uint8 LeftInflectionPointFlag=0;//找到右拐点的标志位
 
 uint8 StartRow=0;
 uint8 StartCol=47;
 uint8 EndRow=0;
 uint8 EndCol=47;


 
 
  //右拐点
  
  for(i=59;i>=35;i--)//不能扫描太远，否则会误判
  {
     if(R_verge[i+1]!=col&&(R_verge[i+1]>R_verge[i-1])&&(R_verge[i]>R_verge[i-2])) //连续三行不丢线
     {
     
      if((R_verge[i-2]-R_verge[i-7]<-10)||(R_verge[i-1]-R_verge[i-6]<-10)||(R_verge[i-3]-R_verge[i-8]<-10)||(R_verge[i-4]-R_verge[i-9]<-10))//找到右边线有拐点
       {
         
           RightInflectionPointRow=i;//记录拐点的行
           RightInflectionPointCol=R_verge[i];//记录拐点的列
           RightInflectionPointFlag=1;//标记找到右拐点
           break;//退出for
       }
      
       }
  }
  
    //左拐点
   for(i=59;i>35;i--) 
   {
     if(L_verge[i+1]!=0&&(L_verge[i+1]<L_verge[i-1])&&(L_verge[i]<L_verge[i-2]))
     {
     
      if((L_verge[i-2]-L_verge[i-7]>10)||(L_verge[i-1]-L_verge[i-6]>10)||(L_verge[i-3]-L_verge[i-8]>10)||(L_verge[i-4]-L_verge[i-9]>10))//找到拐点
       {
           LeftInflectionPointRow=i;//记录该拐点的行
           LeftInflectionPointCol=L_verge[i];//记录该拐点的列
           LeftInflectionPointFlag=1;//标记找到左拐点
           break;//找到退出
       }
       }
   }  
 if(LeftInflectionPointFlag&&RightInflectionPointFlag)//同时找到两个拐点
  {
    CrossFlag1=1;
 
    
//     StartCol=(int)((LeftInflectionPointCol+RightInflectionPointCol)/2);// 取左右拐点的列坐标平均值
//    
//     StartRow=(int)((LeftInflectionPointRow+RightInflectionPointRow)/2);//取左右拐点的行坐标的平均值
//    
//    for(i=8;i>2;i--)//整幅图像的每一行都进行扫描
//    {
//      for(j=47;j>27;j--)//从中间向左边搜索，寻找黑点
//      {
//        if(img[i][j]==0xff&&img[i][j-1]==0x00)
//        {
//          Left_Black[i]=j;//找到左边黑点
//            break;
//        }
//        else
//          Left_Black[i]=0;
//      }
//      for(j=47;j<67;j++)//从中间向右边搜索，寻找黑点
//      {
//        if(img[i][j]==0xff&&img[i][j+1]==0x00)
//        {
//          Right_Black[i]=j;//找到右边黑点
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

//对十字进行识别和进行特殊处理
void NormalCrossConduct(void)
{
  uint8 i=0;
    uint8 j=0;

  //CrossFlag=0;//清零
  //CrossFlag1=0;

  FindInflectionPoint();
  
  if(CrossFlag1==1&&((B_lost>=18)&&(abs(L_lost-R_lost)<10)&&(BlackEndMaxMax>=50)))//左右同时丢线
  {
    CrossFlag1=2;//标记十字
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
////    if((BlackEndM>=BlackEndL)&&(BlackEndM>BlackEndMR))//中间白线最多////(abs(Mid_line[1]-47)!=0)&&(abs(Mid_line[1]-47)<=6)///(BlackEndM>=BlackEndL)&&(BlackEndM>=BlackEndR)
////    {
////      for(i=58;i>20;i--)
////        Mid_line[i]=col/2;//直走
////    }
////    else if((BlackEndML>BlackEndM)&&(BlackEndML>BlackEndMR))//看到左边白的最多，车体偏右了，往左跑///Mid_line[1]-47>6
////    {
////      for(i=58;i>20;i--)
////        Mid_line[i]=42;
////    }
////    else if((BlackEndMR>BlackEndM)&&(BlackEndMR>BlackEndML))//看到右边白的最多，车体偏左了，往右跑
////    {
////      for(i=58;i>20;i--)
////        Mid_line[i]=52;
////    }
//  }
}

void Wave_filtering(void)//中线滤线
{
  uint8 k;
  for(k=row-4;k>3;k--)
  {
    if(abs(Mid_line[k]-Mid_line[k-3])>5||abs(Mid_line[k]-Mid_line[k+3])>5)
      Mid_line[k]=(Mid_line[k-3]+Mid_line[k+3]+Mid_line[k-2]+Mid_line[k+2])/4;
  }
}

//void Ending(void)//////////////////////修改
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



uint8 ending_flag = 0;///==2终点线
extern uint8 garage_direction = 0;///1为左边,2位右边
int right_garage_left_turn_down[2]={0,0};
int right_garage_right_turn_up[2]={0,0};///前一个是行坐标，后一个是列坐标
int left_garage_right_turn_down[2];
int left_garage_left_turn_up[2];///前一个是行坐标，后一个是列坐标


///起跑线
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


///[右车库]
//int jicun;
//float k_1;
//float b_1;




void check_guaidian()
{
    ///首先找左下拐点
    uint8 y1 = 0;
    uint8 x1 = 0;
    uint8 x = 0;
    uint8 y = 0;
        
    int jicun;
    float k_1;
    float b_1;
    /// 1、	如果点为白，往上找（不超过5行），直到为黑（防止姿态的原因导致车偏向道路左边）
//    if (img[row-1][0] != 0)   //不为黑,即为白
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
//    ///2、	从上面确定的第y1行开始往右边扫，扫到的第一个不为黑的点视为左下拐点。
//
//    for (x = 0;x<col-1;x++)
//    {
//        if (img[y1][x] != 0)
//        {
//            x1 = x;
//            break;
//        }
//    }
    //赋值操作
    right_garage_left_turn_down[0] = 59;//y1
    right_garage_left_turn_down[1] = 0;//x1
    ///然后找右上拐点

//    if (img[row-1][col-1] == 0 && img[row-2][col-1] == 0 && img[row-3][col-1] == 0 && img[row-4][col-1] == 0)
//    {
//        for (y = row-1; y > 25; y--)///
//        {
//            if (img[y][col-1] != 0)//为白
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
    ///2、	从上面确定的第y1行开始往左边扫，记录下扫到的第一个为白的点的列坐标
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
    ///3、	遍历[y2, y2 + 2],选出6行中列坐标最大（最靠左）的列坐标以及那时的行坐标，作为左上拐点
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
    //赋值操作
    right_garage_right_turn_up[0] = y1;
    right_garage_right_turn_up[1] = x1;

    

    //补线程序
    
      
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
///[左车库]
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
//  for(i=58;i>25;i--)//左边线最大行数
//  {
//    if(L_verge[i]>L_verge[l])
//      l=i;
//    if(img[i][L_verge[i]+3]!=255)
//      break;
//  }
//  for(j=58;j>25;j--)//右边线最小行数
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
//    }//r最小l最大
//    for(i=r-4,k=0;i>r-6;i--)//两行全黑
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
  GetBlackEndParam(); //获取黑线截止行
  Get_centerline();
  
  //NormalCrossConduct();//出环岛误判，加条件环岛结束标志开//十字扫描
  //FindInflectionPoint();
  if(hw_flag!=1&&hw_flag!=2)
    Ending();
}

 
/*******************************************************************************
函数名称：regression（回归）
函数功能: 最小二乘法求斜率
参数：
*******************************************************************************/
int regression(int Pick_table[],int startline,int endline)//计算斜率函数//Pick_table[]
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
函数：尖角判断
功能: 判断圆环的第一个尖角
参数：
**************************************/
void round_flag1(void)//判断第一个尖角
{
  uint8 i;
  flag=0;
  
  if(round_flag==0)
    for(i=59;i>35;i--)//右圆环
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
    for(i=59;i>35;i--)//左圆环
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

  
  


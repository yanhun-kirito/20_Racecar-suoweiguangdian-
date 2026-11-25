#include "image.h"
#include <string.h>
#include "math.h"

uint8 LeftBlack[ROW], RightBlack[ROW], MiddleLine[ROW]; 


uint8 left_Point[31];//记录左引导线寻到点的y坐标
uint8 right_Point[31];//记录右引导线寻到点的y坐标
uint8 up_Point[15];//记录上引导线寻到的x坐标
uint8 LongLeft, LongRight;//左右有边界行数


float left_err;
float right_err;


uint8 lostCounter = 0;       // 连续丢失边界的行计数器
_Bool trackLostFlag = false; // 断路标志
#define MAX_LOST_ROWS 40      // 允许连续丢失的最大行数
#define xunlie 20
#define ABS_INT(a, b) ( ((int)(a) > (int)(b)) ? ((int)(a) - (int)(b)) : ((int)(b) - (int)(a)) )
uint8 WhiteRow[ROW], WhiteCol[COL];                     //存储图像横、纵方向白点数
uint8 forwardLook[COL];                                 //前瞻行像素点反转
uint8 Up_White[6];                                         //上引导线白点数
uint8 WhiteSum;                                         //前瞻行白点数
uint8 Col_WhiteSum;
uint8 LastRight, LastLeft;                              //历史左右边界
uint8 Right = COL - 1, Left = 0;                      //当前左右边界
uint8 Width[ROW];
uint8 Cross_WhiteCol[COL]={0};
uint8 Cross_row;
uint8 dump_point;                       //行黑白跳变点
_Bool left_Flag = ZF_FALSE;
_Bool right_Flag= ZF_FALSE; 
_Bool up_Flag = ZF_FALSE;
_Bool down_Flag = ZF_FALSE;
//统计行白点数
static void Row_white(void)
{
    uint8 c = 0;

    WhiteSum = 0;
    
      for(c = 0; c < COL; c++)
      {
        forwardLook[c] = Bmp[35][COL - 1 - c];
        //统计前瞻行白点数
        if(forwardLook[c])
            WhiteSum++;
      }
    
}
static void CountCol_White(void)
{
    uint8 r, c;
    //列白点数组清0
        Col_WhiteSum = 0;
        for(int a = 0;a<=1;a++)//20-80
        for(r = 5; r < 25; r++)
        {
          if(a==0) c = 30;
          if(a==1) c = 70;
            if(Bmp[r][c]) 
            Col_WhiteSum++;
        }
}


//找行跳变点
static void Count_point(void)
{ 
    const uint8 col_max = COL;
    uint8 c;
    dump_point = 0;
    uint8 key;
    for (uint8_t i = 0; i < 5; i++) 
    {
      int8 row;
            // 获取实际行号
            switch(i) {
                case 0: row = DataInfo.forwardLook4; break;
                case 1: row = (DataInfo.forwardLook4+1); break;
                case 2: row = (DataInfo.forwardLook4+2); break;
                case 3: row = (DataInfo.forwardLook4+3); break;
                case 4: row = (DataInfo.forwardLook4+4); break;
                default: row = DataInfo.forwardLook0; break; 
            }
        for(c = col_max; c > 0; c--)//前瞻行1的跳变点//80-20
      {
         if(Bmp[row][c]==0 && Bmp[row][c+1]== 1)//把c记录下来
        {
          dump_point++;
          key = c-5;
          break;
        }
      }
          for(c = key; c>0;c--)//key-20
      {
        if(Bmp[row][c]==0 && Bmp[row][c+1]== 1)
            {
                dump_point++;
                break;
            }
      }
    }
  }
//-------------------------------------------------------------------------------------------------------------------
//  @brief      引导线提取函数
//  @explain    1. 从左右两侧按列检测外侧引导线（从下往上）
//              2. 在图像顶部区域（40~35 行）水平扫描，统计连续出现的黑白跳变行数
//-------------------------------------------------------------------------------------------------------------------

void Col_border(void)
{
  uint8 left_num = 0,right_num = 0,up_num = 0;
  uint8 r,c;

//  uint8 size1=5,size2=5,size3=5,size4=5;//size表示可找到的行
//  int16 left1,left2,right1,right2;
  uint8 k1 = 1, k2 = 1;
  const uint8 col_max = COL;
  const uint8 row_end = 50;
  //寻找左外引导线,
  for(c = col_max-1; c >= col_max-1-xunlie; c--)
  {//80-70  中值50 
    for(r = 0; r <= row_end + 9; r++){
    if(Bmp[r][c] == 0 && Bmp[r+1][c] == 1){
        left_num++;
        break;
    }
       if(left_num>3) left_Flag = true;
       else left_Flag = false; 
    }
  }

  //寻找右外引导线
    for(c = 1; c <= 1+xunlie; c++)//20-30 中值50
  {
    for(r = 0; r <= row_end+9; r++) {
    if(Bmp[r][c] == 0 && Bmp[r+1][c] == 1) {
        right_num++;
        break;
    }
        if(right_num>3) right_Flag = true;
        else right_Flag = false; 
    }
  }
   //寻上引导线
    for(r = row_end+5;r<=row_end+15;r++)//55-65
    {
      for(c = col_max - 15; c > 15; c--)//85-15
      {
        if(Bmp[r][c]==0 && Bmp[r][c+1]== 1)
        {
          up_num++;
          break;
        }
        if(up_num>2) up_Flag = true;
        else up_Flag = false; 
      } 
    }
    //存边界 用来判断直角，圆环
  
    for(c = col_max-10; c >= col_max-20-xunlie; c--)
    {//  中值50 90-60
       left_Point[k1] = 0;
    for(r = 0; r <= row_end+5; r++) {//50 
    if(Bmp[r][c] == 0 && Bmp[r+1][c] == 1) {
        left_Point[k1] = r; // 记录左上边界,记录的是左上边界的
        break;
    }
    }
    k1++;
  }
  //寻找右外引导线
    for(c = 10; c <= 20+xunlie; c++)//10-40  中值50
  {
    right_Point[k2] = 0;
    for(r = 0; r <= row_end+5; r++) {//50
    if(Bmp[r][c] == 0 && Bmp[r+1][c] == 1) {
       right_Point[k2] = r; // 记录右上边界
        break;
    }
   }
    k2++;
 }

     Count_point();//统计跳变点
     Row_white();//统计白点数
     CountCol_White();
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      边缘提取函数
//-------------------------------------------------------------------------------------------------------------------
//  @explain    1、由于摄像头倒装，以下函数的左边界为实际中的左边界，即倒装后图像的右边界
//              2、此函数注释中叙述的左边界均为倒装图像的右边界
//-------------------------------------------------------------------------------------------------------------------
void Prospect_Border(void)
{
    uint8 r, c;
    _Bool leftFlag = false, rightFlag = false;
    uint8 pLeft, pRight;
    const uint8 col_max = COL;
    const uint8 row_end = 50;
    static uint8 lostCounter = 0;  // 连续丢失计数器
//    uint8 leftRecord = 0, rightRecord = 0;      //记录非归边左右边界值，用于矫正左右边界突变情况

    LongLeft = 0;
    LongRight = 0;
    trackLostFlag = false;
    memset(LeftBlack, col_max - 1, sizeof(LeftBlack));
    memset(RightBlack, 0, sizeof(RightBlack));
        for(r = 0; r < row_end; r++)
    {
           if(!leftFlag)
            {
            for(c = col_max; c > 0; c--)//找第一个左边界
                {
                if(Bmp[r][c] == 0 && Bmp[r][c-1] == 1)
                    {
                    LeftBlack[r] = c;
                    pLeft = LeftBlack[r];//每找到一次就更新一次
                    pLeft = (pLeft > 89) ? 89 : ((pLeft < 10) ? 10 : pLeft);
                    leftFlag = true;//找到了边界
                    break;
                    }
                    else if(c == 1)
                    {
                    LeftBlack[r] = col_max - 1;//直接归边，寻找下一含
                    }
                }//找到第一个边界后，从第一个边界列数的俩边左右10来巡线；
            }
            else
            {
                for(c = pLeft+10;c > pLeft-10;c--)
                {
                    if(Bmp[r+1][c] == 0 && Bmp[r+1][c-1] == 1)
                    {
                    LeftBlack[r] = c;
                    pLeft = LeftBlack[r];//每找到一次就更新一次
                    pLeft = (pLeft > 89) ? 89 : ((pLeft < 10) ? 10 : pLeft);
                    break;
                    }
                    else if(c == pLeft-9)
                    leftFlag = false;//找不到边界，重新全局搜索
                }
            }
            if(!rightFlag)
            {
                for(c = 1; c < col_max; c++)//20-80
              {
                if(Bmp[r][c] == 0 && Bmp[r][c+1] == 1)
                {
                    RightBlack[r] = c;
                    pRight = RightBlack[r];
                    pRight = (pRight > 89) ? 89 : ((pRight < 10) ? 10 : pRight);
                    rightFlag = true;
                    break;
                }
                if(c == col_max - 1) 
                RightBlack[r] = 0;
              }
            }
            else
            {
                    for(c = pRight-10;c < pRight+10;c++)//10-30
                {
                    if(Bmp[r+1][c] == 0 && Bmp[r+1][c+1] == 1)
                    {
                    RightBlack[r] = c;
                    pRight = RightBlack[r];//每找到一次就更新一次
                    pRight = (pRight > 89) ? 89 : ((pRight < 10) ? 10 : pRight);
                    break;
                    }
                    else if(c == pRight+9)//29
                    rightFlag = false;//找不到边界，重新全局搜索
                }
            }

        
        //------------------------ 边界突变校正 ------------------------//
       //前一行边界归边以及本行另一边界归边，不适用上述两种检测方法，采取以下方法
        if(r > 0)
        {
            //若本行右边界大于有记录的历史左边界，则视为出错（正常为左边界大于右边界）
            if((LeftBlack[r] != COL - 1 && RightBlack[r] == 0) || ( RightBlack[r] != 0 && LeftBlack[r] == COL - 1) ) 
            {
                  if(LeftBlack[r] != COL - 1)
                  {
                      RightBlack[r] = LeftBlack[r] - Width[r-1];
                  }
                  if(RightBlack[r] != 0)
                  {
                      LeftBlack[r] = RightBlack[r] + Width[r-1];
                  }
            }
            //------------------------ 丢失检测与断路处理 ------------------------//
            _Bool leftLost  = (LeftBlack[r] == col_max - 1);
            _Bool rightLost = (RightBlack[r] == 0);

            if(leftLost || rightLost)
            {
                //记录有效边界行
           
                lostCounter++;
                if(lostCounter >= MAX_LOST_ROWS)
                {
                    trackLostFlag = true;
                    lostCounter = 0;
                    break;
                }
            }
            else
            {
                lostCounter = 0;
            }
        
            if(!leftLost)
            {
                LongLeft++;
            }
            if(!rightLost)
            {
                LongRight++;
            }

        

        //------------------------ 赛道宽度 ------------------------//
        Width[r] = LeftBlack[r] - RightBlack[r];
        }
    }
    left_err=LongLeft;//(float)abs(LongLeft-50);
    right_err=LongRight;//-(float)abs(LongRight-50); 
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      合成中线函数
//-------------------------------------------------------------------------------------------------------------------
void Fitting_MiddleLine(void)
{
    uint8 r;

    //左边界长度大于右边界长度
    if(LongLeft > LongRight) 
    {
        for(r = 0; r < LongRight; r++)
        {
            MiddleLine[r] = (LeftBlack[r] + RightBlack[r]) >> 1;
        }
        for(r = LongRight; r < LongLeft;r++)
        {
            MiddleLine[r] = LeftBlack[r] >> 1;
        }
    }
    //右边界长度大于左边界长度
    else if(LongLeft < LongRight)
    {
        for(r = 0; r < LongLeft; r++)
        {
            MiddleLine[r] = (LeftBlack[r] + RightBlack[r]) >> 1;
        }
        for(r = LongLeft; r < LongRight; r++)
        {
            MiddleLine[r] = (RightBlack[r] + COL - 1) >> 1;
        }
    }
    //左右边界长度相等
    else
    {
        for(r = 0; r < LongLeft; r++)
        {
            MiddleLine[r] = (LeftBlack[r] + RightBlack[r]) >> 1;
        }
    }

    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      统计列白点数函数
//-------------------------------------------------------------------------------------------------------------------
//  @explain    按列扫描图像，统计连续白点数
//-------------------------------------------------------------------------------------------------------------------
void CountColWhite(void)
{
    uint8 r, c;
    //列白点数组清0
    memset(WhiteCol, 0, sizeof(WhiteCol));

    for(c = 0; c < COL; c++)
    {
        for(r = 0; r < ROW; r++)
        {
            if(Bmp[r][c])       WhiteCol[c]++;
            else                break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      统计行白点数函数
//-------------------------------------------------------------------------------------------------------------------
//  @explain    按行扫描图像，统计连续白点数
//-------------------------------------------------------------------------------------------------------------------
void CountRowWhite(void)
{
    uint8 r, c;
    uint8 *p = NULL;

    //行白点数组清0
    memset(WhiteRow, 0, sizeof(WhiteRow));

    for(r = 21; r < 26; r++)
    {
        p = &Bmp[r][0];
        for(c = MID; c > 0; c--)
        {
            if(*(p + c) == 1)       WhiteRow[r]++;
            else                break;
        }
        for(c = MID + 1; c < COL; c++)
        {
            if(*(p + c) == 1)       WhiteRow[r]++;
            else                break;
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      统计列白点数函数1
//-------------------------------------------------------------------------------------------------------------------
void Cross_CountColWhite(void)
{
    uint8 c,r;

    memset(Cross_WhiteCol, 0, sizeof(Cross_WhiteCol));

    for(c=0;c<119;c++)
    {
        for(r=20;r<ROW-3;r++)
        {
            if(Bmp[r][c]==1)
            {
                Cross_WhiteCol[c]++;
                if(Bmp[r][c]==1 && Bmp[r+1][c]==0 && Bmp[r+2][c]==0 && Bmp[r+3][c]==0)
                {
                    break;
                }
            }
        }
        for(r=19;r>3;r--)
        {
            if(Bmp[r][c]==1)// && Bmp[r+1][c]==0 && Bmp[r+2][c]==0 && Bmp[r+3][c]==0
            {
                Cross_WhiteCol[c]++;
                if(Bmp[r][c]==1 && Bmp[r-1][c]==0 && Bmp[r-2][c]==0 && Bmp[r-3][c]==0)
                {
                    break;
                }
            }
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      前瞻行左右调转函数
//-------------------------------------------------------------------------------------------------------------------
static void forwardMove(void)
{
    uint8 c = 0;

    WhiteSum = 0;

    for(c = 0; c < COL; c++)
    {
        forwardLook[c] = Bmp[DataInfo.forwardLook0][c];

        //统计前瞻行白点数
        if(forwardLook[c])
            WhiteSum++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      前瞻边缘提取函数
//-------------------------------------------------------------------------------------------------------------------
//  @explain    由于forwardLook_move() 函数将前瞻行调转了，此函数中前瞻的左边界是实际的左边界
//-------------------------------------------------------------------------------------------------------------------
void Prospect_forwardLook(void)
{
    LastLeft = Left;
    LastRight = Right;

    //前瞻行左右调转
    forwardMove();

    //前瞻行边界大小调转
    Left = 119-LeftBlack[DataInfo.forwardLook0];
    Right = 119- RightBlack[DataInfo.forwardLook0];
    
        //////检查纠正
#if 0
        if(Left > Right) //两个判断并非程序写错，可以屏蔽掉其中一个跑一下看现象
        {
            if(fabs(Left - LastLeft) > fabs(Right - LastRight))
            {
                Left = 0;
            }
            else
            {
                Right = COL - 1;
            }
        }
#endif
}


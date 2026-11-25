#include "parameter.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      参数初始化
//-------------------------------------------------------------------------------------------------------------------
void DefaultInit(void)
{
    //-----CarInfo相关参数初始化

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      浮点型变量限幅
//-------------------------------------------------------------------------------------------------------------------
float ValueLimitfloat(float value, float max, float min)
{
    if(value > max) value = max;
    else if(value < min) value = min;
    return value;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      整型变量限幅
//-------------------------------------------------------------------------------------------------------------------
int16 ValueLimitint16(int16 value, int16 max, int16 min)
{
    if(value > max) value = max;
    else if(value < min) value = min;
    return value;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      整型变量限幅
//-------------------------------------------------------------------------------------------------------------------
int8 ValueLimitint8(int8 value, int8 max, int8 min)
{
    if(value > max) value = max;
    if(value < min) value = min;
    return value;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      增幅限幅
//-------------------------------------------------------------------------------------------------------------------
float ChangeLimit(float value, float lastvalue, float increaseLimit, float decreaseLimit)
{
    if((value - lastvalue) > increaseLimit)
        value = lastvalue + increaseLimit;  //增加限幅
    else if((value - lastvalue) < decreaseLimit)
        value = lastvalue + decreaseLimit;  //减速限幅
    return value;
}

int ChangeLimitint(int value, int lastvalue, int increaseLimit, int decreaseLimit)
{
    if((value - lastvalue) > increaseLimit)
        value = lastvalue + increaseLimit;  //增加限幅
    else if((value - lastvalue) < decreaseLimit)
        value = lastvalue + decreaseLimit;  //减速限幅
    return value;
}

/**@brief    限幅函数
-- @param    int16_t x 需要限幅的值
-- @param    int a 最大值
-- @param    int b 最小值
-- @return   x 输出
-- @author   none
**/
int16_t Limit_a_b(int16_t x, int a, int b)
{
    if (x < a) x = a;
    if (x > b) x = b;
    return x;
}

/**@brief    求x,y中的最小值
-- @param    int16_t x x值
-- @param    int16_t y y值
-- @return   返回两值中的最小值
-- @author   none
-- @date    
**/
int16_t Limit1(int16_t x, int16_t y)
{
    if (x > y)             return y;
    else if (x < -y)       return -y;
    else                return x;
}
//判断value值是否在（a,b）区间，在的话返回真
bool check_size(int a, int b) 
{
  int value=0;
  for(int i=a;i<b;i++)
  {
    if(Width[a]>=50)
      value++;
  }  
  if(value>=6)
    return true;
   else
    return false;
}
    
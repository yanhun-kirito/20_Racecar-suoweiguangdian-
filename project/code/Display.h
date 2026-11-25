#ifndef ALOG_INC_DISPLAY_H
#define ALOG_INC_DISPLAY_H

#include "zf_common_headfile.h"
//-------------------------------------------------------------------------------------------------------------------
//  @brief      使用宏定义对参数进行自加自减
//  @param      Parameter       需要写入的参数
//  @param      Increase        需要加减的数值
//  @param      type            加减参数的类型（只能为int或float）
//  Sample usage:               ParameterIncrease(*Page0Para[Channel], Precision[PrecisionSelect] ,int);
//-------------------------------------------------------------------------------------------------------------------
#define     ParameterIncrease(Parameter, Increase, type)    Parameter += (type)(Increase)
#define     ParameterDecrease(Parameter, Decrease, type)    Parameter -= (type)(Decrease)

//单页最大行数
#define     MAX_ROW     7
//按键
#define     UP_PIN        P07_2
#define     DOWN_PIN      P07_0
#define     LEFT_PIN      P07_1
#define     RIGHT_PIN     P07_4
#define     SET_PIN       P07_3

#define     UP      gpio_get_level(UP_PIN)
#define     DOWN    gpio_get_level(DOWN_PIN)
#define     LEFT    gpio_get_level(LEFT_PIN)
#define     RIGHT   gpio_get_level(RIGHT_PIN)
#define     SET     gpio_get_level(SET_PIN)

//参数相关数据结构体
struct ParameterType
{
    char ParaName[MAX_ROW + 1][18];     //参数名

    void *ParaOne[MAX_ROW];             //参数地址
    void *ParaTwo[MAX_ROW];
};

typedef struct 
{
    uint8 Explore_Flag;             //探索模式标志
    uint8 Navigation_Flag;          //导航模式标志
    uint8 Debug_Flag;               //调试模式标志
    uint8 Test_Flag;                //测试模式标志
    uint8 Updata_Flag;              //更新数据标志
}MenuInfomation;

extern int8 Page,Choose_List,Channel,PrecisionSelect;
extern _Bool Is_Increase, Is_Decrease,Para_Mode;
extern _Bool Start_Flag;
extern _Bool Start;
extern uint16 CountStartTime;                                       //发车延时时间
extern uint8 StopTime;                                             //反转停车时间
extern MenuInfomation MenuInfo;


//void IPS114_display(void);
void Key_Scan(void);
void Key_Text(void);

#endif 

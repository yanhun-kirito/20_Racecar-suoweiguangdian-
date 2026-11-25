#ifndef ALOG_INC_FLASH_SAVE_H
#define ALOG_INC_FLASH_SAVE_H

#include "zf_common_headfile.h"
#define FLASH_SECTION_INDEX    (0)

// 存储数据用的扇区

//最大页数
#define     Max_Sector          8

//页号（按照变量类型定义页号）
#define     Sector_uint8        0
#define     Sector_int8         1
#define     Sector_uint16       2
#define     Sector_int16        3
#define     Sector_uint32       4
#define     Sector_int32        5
#define     Sector_float        6
//#define     Sector_Logger       7
typedef struct 
{

    //弯道PD
    float BigCurP;
    float SmallCurP;
    float BigCurD;
    float SmallCurD;



    float traP;//差速Pfl

    //摄像头速度
    uint8 image_straightSpeed;     //直道速度
    uint8 image_curSpeed;          //弯道速度
    uint8 image_traditionSpeed;    //普通速度
    uint8 goSpeed;                 //出库速度
    uint8 Circle_Distance;
    
    //元素速度
    uint8 circle_Speed;//圆环速度
    uint8 break_Speed;//断路速度
    uint8 turn_Speed;//转弯速度
    uint8 cross_Speed;//十字速度
    
    //元素打角
    float turnAngle;//转弯打角
    float breakAngle;//断路打角
    float circleAngle;//圆环打角
    
    //元素定距
    float crossDis;     //十字定距
    float circleDis;    //环岛定距  
    float breakDis;     //断路定距  

    //电磁速度
    uint8 ad_straightSpeed;     //直道速度
    uint8 ad_curSpeed;          //弯道速度
    uint8 ad_traditionSpeed;    //普通速度

    //第二页
    uint8 forwardLook0;      //前瞻行0
    uint8 forwardLook1;      //前瞻行1
    uint8 forwardLook2;      //前瞻行2
    uint8 forwardLook3;      //前瞻行3
    uint8 forwardLook4;      //前瞻行4
    uint8 GOtime;           //圈数         1:一圈   2:两圈

}DataInfomationType;

extern DataInfomationType DataInfo;

void FlashData_Init(void);
void Save_All_Data(void);
void Read_All_Data(void);
#endif 

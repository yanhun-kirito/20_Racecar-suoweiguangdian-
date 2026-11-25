#ifndef ALGO_INC_ROADRECOGNITION_H_
#define ALGO_INC_ROADRECOGNITION_H_
#include "zf_common_headfile.h"
typedef struct
{
    float crossDis;     //十字定距
    float circleDis;    //环岛定距
    float circleDis1;    //环岛定距1
    float stopDis;       //停车线定距离
    float stopDis1;     //停车线定距离1
    float left_circle_Angle; //圆环打角
    float right_circle_Angle;
    float breakDis;     //断路定距
    float breakDis1;     //断路定距1
    float breakAngle;   //导航区打角
    float leftAngle;    //左直角弯打角
    float rightAngle;    //左直角弯打角

    uint8 stopcar;               //停车标志位
    uint8 zebra;                 //斑马线标志位
    uint8 cross;                 //十字标志位
    uint8 left_circle;           //左圆环标志位
    uint8 right_circle;          //右圆环标志位
    uint8 turn_left;             //左直角弯
    uint8 turn_right;            //右直角弯
    uint8 break_road;            //断路标志位
    uint8 protect;               //保护标志位
    uint8 normal_flag;
}RoadFlagType;

extern RoadFlagType RoadFlag;
extern float Slope;
extern uint8 Right_Close;
void Stopcar(void);
void turn_check(void);
void Cross_check(void);
void circular_check(void);
void break_Road(void);
void normal_check(void);

#endif

#ifndef ALOG_INC_SPEED_CONTROL_H
#define ALOG_INC_SPEED_CONTROL_H

//关于速度控制的东东

#include "zf_common_headfile.h"


#define DELTA_T 0.005                  //定时器采样周期
#define I_TERM_LIMIT 110               //积分
#define ONE_METER  36000                //定义一米的计数值
#define COURSE_STORAGE_SIZE 4000

typedef struct
{
        float Total_Distance;           //平均行驶距离
        float Car_velocity;             //平均行驶速度
        
        int8 dirOffset;                //摄像头转向偏差
        int8 lastDirOffset;             //摄像头历史转向偏差

        float Yaw;                       //偏航角
        float YawRate;                  //偏航速率
        float Pitch;
        float Roll;                     //横滚角
          
        float straight_radius;
        int L_encoder;                   //左轮编码器读数
        int R_encoder;                   //右轮编码器读数
  
        float L_current_velocity;
        float R_current_velocity;
        //转向pid控制
        //转向pid控制
        float w_kp1;
        float w_kp2;
        float w_ki;
        float w_kd;
        float last_diff;                //上一个diff
        
        //速度类别
        float target_velocity;         //目标速度 
        float max_velocity;            //最大速度
        float min_velocity;            //最小速度
        float vl_kp;                   //左轮p
        float vr_kp;                   //右轮p
        float vl_ki;                   //左轮i
        float vr_ki;                   //右轮i
        float v_kd;
        float vl_kf;                   //左轮速度前馈系数
        float vr_kf;                   //右轮速度前馈系数
        float diff_factor;              // 差速转换系数
        uint16 cross_speed;              //十字速度
        uint16 base_speed;              // 基准速度
        uint16 circur_speed;             //圆环速度

        uint16 turn_speed;                //直角速度
        //定距和定角度
        float cross_dis;                //十字定距
        float circur_dis;               //圆环定距
        float circur_Angle;             //圆环打角
        float left_Angle;               //直角打角
        float right_Angle;
        
        
        //角度环pid
        float target_angle;             //目标角度
        float last_angle_offset;        //上一次的角度偏差
        float angle_offset;             //角度偏差

        float a_kp;           // 角度环p
        float a_kp2;
        float a_kd;           // 角度环d
        float a_kd2;           //角度换d2
        
        //断路
        uint8 break_dir;   //断路方向（左一右2）左正右负
        float break_dis;                //断路定距
        float break_Angle;              //断路打角
        int16 break_speed;              //断路速度
}CarInfomationType;

extern CarInfomationType CarInfo;
extern _Bool i_reset_flag;

extern uint8 cross_nums;
extern uint8 circle_nums;
extern uint8 break_nums;
extern uint8 turn_nums;
extern float angle_integral;
extern int16 SL_output;
extern int16 SR_output;
extern float target_left;
extern float target_right;
extern float term;
extern float angular_output;
extern float Vl_FF;
extern float Vr_FF;

void Speed_Get(void);
void Speed_Control(void);
void calcdistance(void);
//void Create_Velocity_Table(void);

#endif

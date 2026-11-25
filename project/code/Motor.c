#include "Motor.h"

uint8 Restart_flag=0;
int16 L_PWM, R_PWM;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM输出配置函数
//-------------------------------------------------------------------------------------------------------------------
void Set_Duty(int L_PWM, int R_PWM)
{

    if(L_PWM > 0)
    {
        gpio_set_level(L_Motor_Dire,0);
    }
    else
    {
        L_PWM *=(-1);
        gpio_set_level(L_Motor_Dire,1);
    }

    if(R_PWM > 0)
    {
        gpio_set_level(R_Motor_Dire,0);
    }
    else
    {
        R_PWM *=(-1);
        gpio_set_level(R_Motor_Dire,1);
    }
    pwm_set_duty(R_Motor_PWM, R_PWM);
    pwm_set_duty(L_Motor_PWM, L_PWM);


}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      电机控制函数
//-------------------------------------------------------------------------------------------------------------------
void Mottor_Control(void)
{
//    if(Start_Flag == true)
//    {
        L_PWM = SL_output;// + Vl_FF;
        R_PWM = SR_output;// + Vr_FF;
//    }
//    else
//    {
//        L_PWM = 0;
//        R_PWM = 0;
//    }
    //斑马线刹车
//    if(RoadFlag.zebra == 2 )//&& RoadFlag.zebradis >= 0.02
//    {
//        L_PWM =-660;//-50//MOTOR_MIN
//        R_PWM =-660;
//    }
//    //丢线保护与定距停车
//    if(RoadFlag.protect)
//    {
//        L_PWM = 0;
//        R_PWM = 0;
//    }
    if(L_PWM >=MAX_DRIVE_MOTOR_DUTY) L_PWM = MAX_DRIVE_MOTOR_DUTY;
    else if(L_PWM <= -MAX_DRIVE_MOTOR_DUTY) L_PWM  = -MAX_DRIVE_MOTOR_DUTY;
    if(R_PWM >= MAX_DRIVE_MOTOR_DUTY) R_PWM = MAX_DRIVE_MOTOR_DUTY;
    else if(R_PWM <= -MAX_DRIVE_MOTOR_DUTY) R_PWM  = -MAX_DRIVE_MOTOR_DUTY;
    
    Set_Duty(L_PWM, R_PWM);
 
}

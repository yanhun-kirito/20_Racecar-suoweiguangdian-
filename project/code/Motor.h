#ifndef BOARD_INC_MOTOR_H
#define BOARD_INC_MOTOR_H

#include "zf_common_headfile.h"

/*定义电机引脚*/
#define L_Motor_PWM        TCPWM_CH11_P05_2
#define L_Motor_Dire       P05_3

#define R_Motor_PWM        TCPWM_CH09_P05_0
#define R_Motor_Dire       P05_1


#define MAX_DRIVE_MOTOR_DUTY  6000    //40%

extern int16 L_PWM, R_PWM;

void Set_Duty(int L_PWM, int R_PWM);
void Mottor_Control(void);

#endif



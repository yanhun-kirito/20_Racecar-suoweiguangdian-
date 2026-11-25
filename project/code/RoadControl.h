#ifndef ALGO_INC_ROADCONTROL_H_
#define ALGO_INC_ROADCONTROL_H_
#include "zf_common_headfile.h"

extern int8 reset_yaw_left ; // ÷±Ω«÷√0
extern int8 reset_yaw_right ; // ÷±Ω«÷√0
void Stop_Cnotrol(void);
void cross_contror(void);
void turncontrol(void);
void circularcontrol(void);
void breakcontrol(void);
void normal_control(void);
#endif
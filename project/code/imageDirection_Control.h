/*
 * Direction_Control.h
 *
 *  Created on: 2023年4月17日
 *      Author: hzw
 */

#ifndef ALGO_INC_IMAGEDIRECTION_CONTROL_H_
#define ALGO_INC_IMAGEDIRECTION_CONTROL_H_

#include "zf_common_headfile.h"

#define     MIDDLE                  59             //图像中值

#define     OFFSET_MAX              90              //偏差极限值

#define     CURVE_OFFSET            30              //急弯、普通弯偏差分界

//extern float direP1 , direD1; //转向PD
extern uint8 curvetime;
extern float OutCurvature;
extern float ExpectCurvature;
extern float turn_PWM;

void imageDirectionControl(void);
void Road_check(void);
#endif /* ALGO_INC_IMAGEDIRECTION_CONTROL_H_ */

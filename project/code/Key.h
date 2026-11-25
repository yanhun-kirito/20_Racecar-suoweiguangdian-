#ifndef BOARD_INC_KEY_H
#define BOARD_INC_KEY_H

#include "zf_common_headfile.h"
//蜂鸣器
#define     BUZZY_PIN       P06_0
#define   PERIODMS     5           //采样周期（毫秒）

extern uint32 Buzzer_Stop;

void BuzzerMs(uint32 time);


#endif 

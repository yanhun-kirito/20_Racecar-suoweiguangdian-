#ifndef ALGO_INC_PARAMETER_H_
#define ALGO_INC_PARAMETER_H_

#include "zf_common_headfile.h"

void DefaultInit(void);

int ChangeLimitint(int value, int lastvalue, int increaseLimit, int decreaseLimit);
int16 ValueLimitint16(int16 value, int16 max, int16 min);
float ValueLimitfloat(float value, float max, float min);
float ChangeLimit(float value, float lastvalue, float increaseLimit, float decreaseLimit);
int8 ValueLimitint8(int8 value, int8 max, int8 min);
int16_t Limit_a_b(int16_t x, int a, int b);
int16_t Limit1(int16_t x, int16_t y);
bool check_size(int a, int b)  ;
#endif /* ALGO_INC_PARAMETER_H_ */

#ifndef ALOG_INC_ANGLECALCULATE_H_
#define ALOG_INC_ANGLECALCULATE_H_

#include "zf_common_headfile.h"
#include "Attitude_Calculation.h"
#include "zf_device_icm20602.h"

//一阶低通滤波权重
#define new_weight           0.75f
#define old_weight           0.25f

void Attitude_Geting(void);

#endif /* BOARD_INC_ANGLECALCULATE_H_ */

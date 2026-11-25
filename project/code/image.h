#ifndef ALGO_INC_IMAGE_H_
#define ALGO_INC_IMAGE_H_

#include "zf_common_headfile.h"
#define COL     120
#define ROW     60
#define MID     (COL >> 1)

#define ENDROW 15

extern uint8 LeftBlack[ROW], RightBlack[ROW], MiddleLine[ROW]; //左右边界、中线
extern uint8 LongLeft, LongRight;                              //左右有边界行数

extern float left_err;
extern float right_err;

extern uint8 WhiteRow[ROW], WhiteCol[COL];                     //存储图像纵方向白点数
extern uint8 WhiteSum;                                         //前瞻行白点数
extern uint8 Col_WhiteSum;                                      //列白点数
extern uint8 LastRight, LastLeft;                              //历史左右边界
extern uint8 Right, Left;                                      //当前左右边界
extern uint8 left_Point[31];
extern uint8 right_Point[31];
extern uint8 Width[ROW];
extern uint8 Cross_WhiteCol[COL];
extern uint8 dump_point; 
extern _Bool left_Flag;
extern _Bool right_Flag;
extern _Bool up_Flag;
extern _Bool trackLostFlag;


void Prospect_Border(void);
void Fitting_MiddleLine(void);
void CountColWhite(void);
void CountRowWhite(void);
void Prospect_forwardLook(void);
void Cross_CountColWhite(void);
void Col_border(void);
void UpRowWhite(void);
#endif /* ALGO_INC_IMAGE_H_ */

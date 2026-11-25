#ifndef ALGO_INC_BINARY_IMAGE_H_
#define ALGO_INC_BINARY_IMAGE_H_

#include "zf_device_mt9v03x.h"

typedef struct
{
    uint8 threshold;        //二值化阈值
    uint8 lastThreshold;    //上一个二值化阈值
}ImageInfomationType;

extern ImageInfomationType ImageInfo;

extern uint8 Bmp[MT9V03X_H][MT9V03X_W];               //保存二值化图像      
extern uint8 rotate_image[MT9V03X_H][MT9V03X_W];

void Binary_image(uint8 *image, uint8 *bmp, uint8 height, uint8 width, uint8 threshold);

uint8 Iterative_Threshold_Opt(uint8 *image, uint16 total_pixels, uint8 min_max_dell) ;
void rotateImage180(uint8 image[][MT9V03X_W], uint8 result[][MT9V03X_W], int r, int c);

#endif /* ALGO_INC_BINARY_IMAGE_H_ */

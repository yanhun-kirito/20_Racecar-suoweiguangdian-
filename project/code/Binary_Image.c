#include "Binary_Image.h"

ImageInfomationType ImageInfo;

uint8 Bmp[MT9V03X_H][MT9V03X_W];    //存放二值化图像
uint8 rotate_image[MT9V03X_H][MT9V03X_W];


/*
图像旋转180°
因为代码是以摄像头上下镜像为基础的的，所以需要旋转180°，才能得到正确的图像
*/
void rotateImage180(uint8 image[][MT9V03X_W], uint8 result[][MT9V03X_W], int r, int c) {
    // 180°旋转是将元素位置按对角线对称，逐个像素交换位置
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            result[r - i - 1][c - j - 1] = image[i][j];
        }
    }
}





//-------------------------------------------------------------------------------------------------------------------
//  @brief    terative_Threshold(        //迭代阈值法
//        uint8 *image,                  //源图像
//        uint16 total_pixels,           //图像总点数
//        uint8 min_max_dell             //是否删除最大最小灰度值
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 Iterative_Threshold_Opt(uint8 *image, uint16 total_pixels, uint8 min_max_dell) 
{
    uint16 pixel_Count[256] = {0};
    uint8 predict_Threshold = 0, threshold = 0;
    static uint8 s_threshold = 0;
    float alpha = 0.8; // 前景权重系数（0.5~0.8）

    // 1. 直方图统计（不修改total_pixels）
    uint16 actual_valid = 0;
    for (uint16 i = 0; i < total_pixels; i++) 
    {
        uint8 pixel = image[i];
        if (pixel != 0 && pixel != 255) 
        {
            pixel_Count[pixel]++;
            actual_valid++;
        }
    }

    // 2. 清除极端灰度（不依赖valid_Count）
    if (min_max_dell) 
    {
        for (int i = 0; i < 5; i++) pixel_Count[i] = 0;
        for (int i = 251; i < 256; i++) pixel_Count[i] = 0;
        // 移除孤岛检测逻辑
    }

    // 3. 计算有效灰度范围
    uint8 minGray = 0, maxGray = 255;
    while (minGray < 255 && pixel_Count[minGray] == 0) minGray++;
    while (maxGray > 0 && pixel_Count[maxGray] == 0) maxGray--;

    // 4. 计算总灰度和
    uint32 total_sum = 0;
    for (int i = minGray; i <= maxGray; i++) 
    {
        total_sum += pixel_Count[i] * i;
    }

    // 5. 动态初始阈值
    predict_Threshold = (s_threshold >= minGray && s_threshold <= maxGray) 
                        ? s_threshold : (minGray + maxGray) / 2;

    // 6. 加权迭代（偏向高亮区域）
    for (uint8 t = 0; t < 20; t++) 
    { 
        uint32 sum0 = 0, sum1 = 0;
        uint16 w0 = 0, w1 = 0;

        for (int i = minGray; i <= maxGray; i++) 
        {
            if (i < predict_Threshold) 
            {
                w0 += pixel_Count[i];
                sum0 += pixel_Count[i] * i;
            } else 
            {
                w1 += pixel_Count[i];
                sum1 += pixel_Count[i] * i;
            }
        }

        float u0 = (w0 > 0) ? (sum0 / (float)w0) : 0;
        float u1 = (w1 > 0) ? (sum1 / (float)w1) * alpha : 0; // 加权前景
        predict_Threshold = (uint8)((u0 + u1) / (1 + alpha));            //新预测阈值

        if (abs(predict_Threshold - threshold) <= 1)
            break;                         //比较阈值，合适就返回
        else if(abs(predict_Threshold-s_threshold)>=10)
            threshold = s_threshold;
        else if(predict_Threshold < 50 || s_threshold < 50)    
            threshold = 50;//32
        else
            threshold = predict_Threshold;  //记录阈值用于下一次比较
    }


    s_threshold = predict_Threshold;
    return threshold;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      图像二值化函数
//  @param      *image      源图像地址
//  @param      *bmp        目标图像
//  @param      height      图像高度
//  @param      width       图像宽度
//  @param      threshold   阈值
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void Binary_image(uint8 *image, uint8 *bmp, uint8 height, uint8 width, uint8 threshold)
{
    uint8_t *img_ptr = image;
    uint8_t *bmp_ptr = bmp;
    uint16_t total_pixels = (uint16_t)height * width;

    // 按连续内存逐像素处理
    while (total_pixels--) {
        *bmp_ptr++ = (*img_ptr++ > threshold) ? 1 : 0;
    }
}

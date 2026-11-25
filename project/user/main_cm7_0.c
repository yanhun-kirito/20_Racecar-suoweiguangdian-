#include "zf_common_headfile.h"
#include "image.h"
#include "LCD.h"
#include "Motor.h"

#define VOLTAGE_PORT    ADC0_CH21_P07_5 // 定义检测端口
// 图像备份数组，在发送前将图像备份再进行发送，这样可以避免图像出现撕裂的问题
//uint8 image_copy[MT9V03X_H][MT9V03X_W];

int main(void)
{
    clock_init(SYSTEM_CLOCK_250M); 	// 时钟配置及系统初始化<务必保留>
    debug_init();                       // 调试串口信息初始化
    Peripheral_Init();
    /*FLASH_INTT*/
    flash_init();               //flash初始化
    FlashData_Init();           //flash参数初始化  
    pit_ms_init(PIT_CH2, 5.5); 
    uint16 valtage_adc;
    float valtage_now;
    adc_init(VOLTAGE_PORT, ADC_12BIT);   
    //pit_ms_init(PIT_CH1, );
//    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy[0], MT9V03X_W, MT9V03X_H);
//    seekfree_assistant_camera_boundary_config(XY_BOUNDARY, BOUNDARY_NUM, xy_x1_boundary, xy_x2_boundary, xy_x3_boundary, xy_y1_boundary, xy_y2_boundary, xy_y3_boundary);
    while(true)
    {    
      
      //图像采集完成进行图像处理
        if(mt9v03x_finish_flag == 1)
        {
            
             //图像部分
            mt9v03x_finish_flag = 0;  //清除图像采集完成标志位  
            rotateImage180(mt9v03x_image, rotate_image,MT9V03X_H, MT9V03X_W);
            //阈值计算            
            ImageInfo.threshold = Iterative_Threshold_Opt(mt9v03x_image[0], MT9V03X_H * MT9V03X_W , 1); 
            //二值化
            Binary_image( rotate_image[0],Bmp[0],MT9V03X_H,MT9V03X_W,ImageInfo.threshold); 
            
            //处理图像         
            Col_border();                //引导线提取函数
            Prospect_Border();           //提取边界 
            Fitting_MiddleLine();               //合成中线
//            if(Start_Flag == true)
//            {
            Road_check();
            imageDirectionControl(); //算出diroffset的  
//            }          
//            // 在发送前将图像备份再进行发送，这样可以避免图像出现撕裂的问题
//            memcpy(image_copy[0], rotate_image[0], MT9V03X_IMAGE_SIZE);
//
//            // 发送图像
//            seekfree_assistant_camera_send();
        }
        valtage_adc = adc_convert(VOLTAGE_PORT);//电源电压检测
        valtage_now = 36.3 * valtage_adc / 4096;
        ips200_show_string(130,150,"voltage:");        //电源电压显示
        ips200_show_float(200, 150, valtage_now, 2, 2);
        OLED_SHOW();
      
        // 此处编写需要循环执行的代码
    }
}

// **************************** 代码区域 ****************************

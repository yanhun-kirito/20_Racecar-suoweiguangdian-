#include "init.h"
CarInfomationType CarInfo;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      外设初始化函数
//-------------------------------------------------------------------------------------------------------------------

void Peripheral_Init()
{
    /*显示屏*/
    lcd_init();
    /*按键*/
    key_init(5);
    /*编码器*/
    encoder_quad_init(TC_CH27_ENCODER,TC_CH27_ENCODER_CH1_P19_2, TC_CH27_ENCODER_CH2_P19_3);  //右轮编码器，Encoder2
    encoder_quad_init(TC_CH58_ENCODER,TC_CH58_ENCODER_CH1_P17_3, TC_CH58_ENCODER_CH2_P17_4);  //左轮编码器，Encoder1

    /* 电机 */
    pwm_init(L_Motor_PWM,17000, 0);
    pwm_init(R_Motor_PWM, 17000, 0);
//    pwm_init(WS_Motor_PWM, 50 , 0);
    gpio_init(L_Motor_Dire,   GPO, 0, GPO_PUSH_PULL);
    gpio_init(R_Motor_Dire, GPO, 0, GPO_PUSH_PULL);

//    /* 按键 */
//    gpio_init(UP_PIN,   GPI, 1, GPI_PULL_UP);
//    gpio_init(DOWN_PIN, GPI, 1, GPI_PULL_UP);
//    gpio_init(LEFT_PIN, GPI, 1, GPI_PULL_UP);
//    gpio_init(RIGHT_PIN,GPI, 1, GPI_PULL_UP);
//    gpio_init(SET_PIN,  GPI, 1, GPI_PULL_UP);

    /*蜂鸣器*/
    gpio_init(BUZZY_PIN, GPO, 0, GPO_PUSH_PULL);

    /*显示屏*/
//    ips114_init();
//    ips114_clear();
    
    if(imu660ra_init())
    {
//        ips114_show_string(0, 16, "imu963ra ERROR.");
        while(1);
    }
    
    /*摄像头*/
    if(mt9v03x_init())
    {
//         ips114_show_string(0, 16, "mt9v03x ERROR.");
         while(1);
    }
    
 
    /*无线转串口*/
//    if(wireless_uart_init())
//    {
//         ips114_show_string(0, 16, "wireless_uart ERROR.");
//         while(1); 
//    }
    
    /*FLASH_INTT*/
    flash_init();               //flash初始化
    FlashData_Init();           //flash参数初始化  

    /* 定时器中断 */
//    pit_us_init(PIT_CH0, 100);          //设置中断时间为0.1ms
//    pit_ms_init(PIT_CH1, 1);            //设置中断时间为1ms   
    pit_ms_init(PIT_CH2, 5);            //设置中断时间为5ms    //5ms进入一次中断处理运动决策（速度控制，电机，编码器等）
 


}
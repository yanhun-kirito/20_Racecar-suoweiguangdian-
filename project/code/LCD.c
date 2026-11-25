#include "LCD.h"

//IPS200_TYPE_SPI
//IPS200_TYPE_PARALLEL8
void lcd_init()
{
      ips200_set_color(RGB565_WHITE, RGB565_BLACK);     //字体和背景颜色
       ips200_set_dir(IPS200_PORTAIT);                  //屏幕方向初始化
       ips200_init(IPS200_TYPE_PARALLEL8);              //初始化
       ips200_set_font(IPS200_6X8_FONT);                //字体大小
}

////屏幕画边线
void Draw_LineLine(void)
{
   for(uint8 r = 0; r < 50; r++)
    {
//        ips200_draw_point(LeftBlack[r], r, RGB565_YELLOW);
        ips200_draw_point(MiddleLine[r], r, RGB565_RED);
        //ips200_draw_point(RightBlack[r], r, RGB565_BLUE);
    }
}


//lcd显示
void OLED_SHOW(void)
{
//    h = (a+b)/2;

      ips200_show_int(130,0 , LeftBlack[20],2);
      ips200_show_int(160,0 , RightBlack[20],2);
      ips200_show_int(190,0 , MiddleLine[20],2);
      ips200_show_int(130,10 , Width[5],2);
      ips200_show_int(150,10 , left_Flag,2);
      ips200_show_int(160,10 , right_Flag,2);
//      ips200_show_int(170,10 , Right_Close,2);
      ips200_show_int(180,10 , Col_WhiteSum,2);
      ips200_show_int(200,10 , RoadFlag.break_road,2);
      ips200_show_int(220,10 , dump_point,2);
//     ips200_show_string(0,180,"Err:");
//    
//    ips200_show_int(50,150,rec_data,5);  //左编码器读值
    
//    ips200_show_string(0,130,"Yuzhi:");             //阈值
//    ips200_show_string(0,140,"Boundary:");          //边界
     ips200_show_string(0,150,"Speed:");              //编码器速度
     ips200_show_string(0,180,"T_Left:");             //目标左速度
     ips200_show_string(0,190,"T_Right:");             //目标右速度
     ips200_show_string(100,180,"A:");             //角度换补偿速度
     ips200_show_string(100,190,"T:");             //转向环补偿速度
     
//    ips200_show_string(0,200,"Zhuan:");             //转向

//    //ips200_show_string(0,190,"Motor:");           //电机
    
     ips200_show_string(0,210,"Gyro:");              //陀螺仪xyz角
     ips200_show_string(0,230,"V_angular:");              //陀螺仪xyz角
     ips200_show_string(0,240,"angle_Off:");              //角度偏差
     ips200_show_string(0,250,"Err:");
     ips200_show_string(0,270,"ZuoPWM:");            //左轮PWM
     ips200_show_string(0,280,"YouPWM:");            //右轮PWM
     ips200_show_string(0,290,"Total_Dis:");         //平均距离累加
     ips200_show_string(130,230,"V_kp:");             //速度环kp
     ips200_show_string(130,240,"V_ki:");             //速度环ki
     ips200_show_string(130,250,"w_kp:");             //转向环kp
     ips200_show_string(130,260,"w_ki:");             //转向环kp
     ips200_show_string(130,270,"a_kp:");             //方向环kp
     ips200_show_string(130,280,"a_kd:");             //方向环kd
     ips200_show_string(130,300,"R_A:");            //RoadAngle
     ips200_show_string(130,160,"threshold:");        //曝光
     
//    ips200_show_int(80,130,image.Threshold,3);
//    
//    ips200_show_int(80,140,image.Left_Line[41],3);
//    ips200_show_int(110,140,image.MID_Line[41],3);
//    ips200_show_int(140,140,image.Right_Line[41],3);
//
//    ips200_show_int(90,150,image.Left_Line[44],3);
//    ips200_show_int(110,150,image.MID_Line[44],3);
//    ips200_show_int(140,150,image.Right_Line[44],3);
//
//    ips200_show_int(90,160,image.Left_Line[47],3);
//    ips200_show_int(110,160,image.MID_Line[47],3);
//    ips200_show_int(140,160,image.Right_Line[47],3);
//    
//    ips200_show_int(100,180,image.Left_Line[32],3);
//    ips200_show_int(120,180,image.MID_Line[32],3);
//    ips200_show_int(140,180,image.Right_Line[32],3);
    
    //速度显示
      ips200_show_float(50,150,CarInfo.L_current_velocity,3,1);  //左编码器实际速度
      ips200_show_float(90,150,CarInfo.Car_velocity,3,1);  //编码器均速
      ips200_show_float(90,160,CarInfo.turn_speed,3,1);  //转向速度
      ips200_show_float(50,160,CarInfo.R_current_velocity,3, 1);  //右编码器实际速度
      ips200_show_float(50,180,target_left,3, 1);  //左编码器目标速度
      ips200_show_float(50,190,target_right,3, 1);  //右编码器目标速度
      
      ips200_show_float(120,180,angular_output,3, 1);  //角度换补偿速度
      ips200_show_float(120,190,term,3, 1);            //转向环补偿速度
     //角度显示
      ips200_show_float(40, 210, CarInfo.Roll, 3,1);//翻滚角 俯仰角 偏航角
      ips200_show_float(80, 210, CarInfo.Pitch, 3,1);
      ips200_show_float(120, 210, CarInfo.Yaw, 3,1);
      ips200_show_float(80, 230, CarInfo.YawRate, 3,1);//Z轴角速度
      ips200_show_float(80, 240, CarInfo.angle_offset, 3,1);//角度偏差
      ips200_show_float(50,250,CarInfo.dirOffset,4,2);  //转向误差
      ips200_show_int(160,300,RoadFlag.leftAngle,2); //RoadFlag.leftAngle
      ips200_show_int(190,300,RoadFlag.rightAngle,2); //RoadFlag.rightAngle
      //各环输出
      ips200_show_int(50,270,L_PWM,5);  //左轮pwm
      ips200_show_int(50,280,R_PWM,5);  //右轮pwm
      /*边界显示*/
      ips200_show_int(150,290,LongLeft,5);  
      ips200_show_int(180,290,LongRight,5);  
      
      ips200_show_float(50,300,Slope,5,2);  //斜率
       ips200_show_int(0,300,up_Flag,2);  
      //距离显示
      ips200_show_float(70, 290, CarInfo.Total_Distance, 3,1);//平均距离
      //参数显示
      ips200_show_int(160,230,CarInfo.vl_kp,3);  //速度环kp
      ips200_show_int(200,230,CarInfo.vr_kp,3);  
      
      ips200_show_float(160, 240, CarInfo.vl_ki, 2,2);//速度环ki
      ips200_show_float(200,240,CarInfo.vr_ki,2,2);  //
      
      ips200_show_float(160,250,CarInfo.w_kp1,2,2);  //转向环kp
      ips200_show_float(190,250,CarInfo.w_kp2,2,3);
      ips200_show_float(160,260,CarInfo.w_ki,2,2);
      ips200_show_float(190,260,CarInfo.w_kd,2,2);
      
      ips200_show_float(160,270,CarInfo.a_kp,2,2);
      ips200_show_float(195,270,CarInfo.a_kp2,1,4);
      ips200_show_float(160,280,CarInfo.a_kd,2,3);
      ips200_show_float(200,280,CarInfo.a_kd2,2,2);
//       //直角弯标志位
      ips200_show_string(0,  100, "TURN_L/R:");
      ips200_show_uint(60, 100, RoadFlag.turn_left,1);
      ips200_show_uint(80, 100, RoadFlag.turn_right,1);
//
//      //圆环
      ips200_show_string(0,  120, "left_circle:");
      ips200_show_uint(80, 120, RoadFlag.left_circle, 1);
//      
      ips200_show_string(0,  130, "right_circle:");
      ips200_show_uint(90, 130, RoadFlag.right_circle, 1);
//      
//      //十字
      ips200_show_string(130,  20, "CROSS:");
      ips200_show_uint(170, 20, RoadFlag.cross,1);
      ips200_show_string(130,  30, "crossDis:");
      ips200_show_float(190, 30, RoadFlag.crossDis, 2, 2);
//      //断路
      ips200_show_string(130,  50, "break_Road:");
      ips200_show_uint(210, 50, RoadFlag.break_road,1);
      ips200_show_string(130,  60, "b_Dis:");
      ips200_show_float(170, 60, RoadFlag.breakDis, 2, 2);
      ips200_show_string(130,  70, "b_Angle:");
      ips200_show_float(190, 70, RoadFlag.breakAngle, 2, 2);
//      
//      //圆环打角和距离
      ips200_show_string(130,  90, "C_Dis:");
      ips200_show_float(170, 90, RoadFlag.circleDis , 2, 2);
      ips200_show_string(130,  100, "L/R_Angle:");
      ips200_show_float(190, 100, RoadFlag.left_circle_Angle, 3, 2);
      ips200_show_float(190, 110, RoadFlag.right_circle_Angle, 3, 2);
      
      //脱离控制显示
      ips200_show_string(130,  130, "Normal:");
      ips200_show_uint(200, 130, RoadFlag.normal_flag, 1);
      ips200_show_string(130,  140, "N_Dis:");
      ips200_show_float(170, 140, RoadFlag.stopDis, 1, 2);
      
      ips200_show_string(160,  180, "Stop:");
      ips200_show_uint(200, 180, RoadFlag.stopcar, 1);
      //曝光显示
      ips200_show_int(200,160,ImageInfo.threshold,5); 
//      //电源电压显示
//      ips200_show_string(130,160,"voltage:");        
//      ips200_show_float(200, 160, valtage_now, 2, 2);
//      ips200_show_float(200, 170, Extra, 1, 2);
//    ips200_show_int(50,210,f,5);  //开环左轮pwm
//    ips200_show_int(50,220,g,5);  //开环右轮pwm
//    
//    ips200_show_int(50,240,(int)z,5);     // 添加显式转换
//    ips200_show_int(50,260,(int)Z_360,5); // 添加显式转换
//    ips200_show_int(50,280,(int)k,5);      //目标速度
//    ips200_show_int(50,300,(int)n,5);     //最终速度
//    ips200_show_float(120,280,tp, 4, 2);      //转向环kp
//    ips200_show_float(120,300,td, 4, 2);     //转向环kd
    
    
//    //圆环判断
//    ips200_show_int(80,240,image.yuan_s,1);   //圆环判断开始
//    ips200_show_int(100,240,image.yuan_eL,1); //圆环出环
//    ips200_show_int(110,240,image.yuan_eR,1); //圆环出环
//    ips200_show_int(120,240,image.MID_L,1);   //左环
//    ips200_show_int(140,240,image.MID_R,1);   //右环
    
    //ips200_show_int(80,180,x,5);  //镜头误差(m7_1核心）
    

   ips200_show_gray_image(0, 0, rotate_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 130);//ImageInfo.threshold);
//      ips200_show_gray_image(0, 0,(uint8 *)mt9v03x_image, 120, 80, 240, 120, 0);
   // ips114 displayimage032_zoomB_reverse((uint8 *)mt9v03x_image,120, 80,240, 120);//显示灰度图像像
//   Draw_LineLine();
}


#include "RoadControl.h"

int8 reset_yaw_left = 0 ; // 直角置0
int8 reset_yaw_right = 0 ; // 直角置0
int8 reset_yaw_leftcircul = 0; // 圆环角度置0
int8 reset_yaw_rightcircul = 0; // 圆环角度置0
int8 reset_break_yaw = 0; //断路角度置0


void Stop_Cnotrol(void)
{
    if(RoadFlag.stopcar == 1)
    {
        CarInfo.dirOffset = 0;
        CarInfo.Yaw = 0;
    }
}
//十字控制
//1.识别到十字后，用前瞻行2与之后五行计算offset
//2,识别到十字后,当白点数大于阈值，此时中线为图像中线，当白点数恢复正常，正常扫线
void cross_contror(void)
{   
 //     uint8 size = 2;
      int16 total_sum = 0;
      int8 row,row1;
      row = DataInfo.forwardLook0;
      row1 = DataInfo.forwardLook2;
                 
      total_sum = (MiddleLine[row] + MiddleLine[row1]);
      if(total_sum == 0)
      {
        CarInfo.dirOffset = 0;
      }
      else
      {
        CarInfo.dirOffset = (total_sum / 2) - MIDDLE;
      }
      // 更新历史偏移量
      CarInfo.lastDirOffset = CarInfo.dirOffset; 

//  if(RoadFlag.crossDis > CarInfo.cross_dis && RoadFlag.cross == 2)//定距出十字
//  {
//      RoadFlag.cross = 0;
//  }
}


//转直角弯控制
//分左转弯，右转弯，识别到给定一个匀速的差速，再进行定角度（45°）
void turncontrol(void)//识别到直角后，角度清零
{
  //左直角弯
  if(RoadFlag.turn_left == 2)
  {
     if (!reset_yaw_left) 
     {
         CarInfo.Yaw = 0;
         CarInfo.YawRate = 0;
         reset_yaw_left = 1; // 角度已经置零
         CarInfo.target_angle = CarInfo.left_Angle + 5;                  //目标角度set
//         RoadFlag.leftAngle = 31;
//         system_delay_ms(50);
     } 
      
//      CarInfo.dirOffset = left_err;
//      CarInfo.dirOffset = ValueLimitint8(CarInfo.dirOffset, 60, -60);
//      //限幅
//      if(CarInfo.dirOffset < 4 && CarInfo.dirOffset > -4) CarInfo.dirOffset = 0;
//      CarInfo.lastDirOffset = CarInfo.dirOffset;//给到转向环
     
    
    
//     if(RoadFlag.leftAngle > CarInfo.left_Angle && reset_yaw_left)
//     {  
//         CarInfo.target_angle = 0;
//         CarInfo.Yaw = 0;
//         reset_yaw_left = 0;
//         RoadFlag.leftAngle = 0;
//         CarInfo.angle_offset = 0;
////         RoadFlag.turn_left = 0;     
//     }
  }
    //右直角弯
  if(RoadFlag.turn_right == 2)
  {
    if (!reset_yaw_right  ) {
         CarInfo.Yaw = 0;
         CarInfo.YawRate = 0;
         reset_yaw_right = 1; // 角度已经置零
         CarInfo.target_angle = CarInfo.right_Angle - 5;
//         RoadFlag.rightAngle = -31;
//         system_delay_ms(50);
     } 
//      CarInfo.dirOffset = right_err;
//      CarInfo.dirOffset = ValueLimitint8(CarInfo.dirOffset, 60, -60);
//      //限幅
//      if(CarInfo.dirOffset < 4 && CarInfo.dirOffset > -4) CarInfo.dirOffset = 0;
//      CarInfo.lastDirOffset = CarInfo.dirOffset;//给到转向环
//    if(RoadFlag.rightAngle < CarInfo.right_Angle &&  reset_yaw_right)
//    {
//      
//       CarInfo.target_angle = 0;
//       CarInfo.angle_offset = 0;
//       CarInfo.Yaw = 0;
//       reset_yaw_right = 0;
////       RoadFlag.turn_right = 0;
//       RoadFlag.rightAngle = 0;
//    }
   
  }

    //     int16 total_sum = 0;
    //     // 遍历四个前瞻行
    //     for (uint8 i = 0; i < 4; i++) 
    //     {
    //         int8 row;
    //         // 获取实际行号
    //         switch(i) {
    //             case 0: row = 12; break;
    //             case 1: row = 14; break;
    //             case 2: row = 16; break;
    //             case 3: row = 17; break;
    //             default: row = DataInfo.forwardLook0; break; 
    //         }           
    //             total_sum += MiddleLine[row]; 
    //     // 突变处理（限制最大变化量）
    //     if(abs(CarInfo.dirOffset - CarInfo.lastDirOffset) > 45) {
    //         CarInfo.dirOffset = CarInfo.lastDirOffset;
    //     }
    //             // 计算最终偏移量（四行均值）
    //     CarInfo.dirOffset = (total_sum / 4) - MIDDLE;
    //     CarInfo.dirOffset = ValueLimitint8(CarInfo.dirOffset, 60, -60);
    //     //限幅
    //     if(CarInfo.dirOffset < 4 && CarInfo.dirOffset > -4) CarInfo.dirOffset = 0;
    //     CarInfo.lastDirOffset = CarInfo.dirOffset;//给到转向环
    // }


}

void circularcontrol(void)
{
  //右圆环
  if(RoadFlag.right_circle == 2)
  {
       if (!reset_yaw_rightcircul  ) {
        CarInfo.Yaw = 0;
        CarInfo.YawRate = 0;
        reset_yaw_rightcircul = 1; // 角度已经置零
    }

     if(RoadFlag.right_circle_Angle > CarInfo.circur_Angle)
    {
      reset_yaw_rightcircul = 0;
      RoadFlag.right_circle = 3;
      RoadFlag.normal_flag = 1;
    }
   }
  
  //左圆环
  if(RoadFlag.left_circle == 2)
  {
       if (!reset_yaw_leftcircul  ) {
        CarInfo.Yaw = 0;
        CarInfo.YawRate = 0;
        reset_yaw_leftcircul = 1; // 角度已经置零
    }
   if(RoadFlag.left_circle_Angle > CarInfo.circur_Angle)
    {
      reset_yaw_left = 0;
      RoadFlag.left_circle = 3;
      RoadFlag.normal_flag = 1;
    }
    
  }      

}


//断路控制
//识别到断路后，给差速，定距，定角度
void breakcontrol(void)
{
    if(!reset_break_yaw  ) {
        CarInfo.Yaw = 0;
        reset_break_yaw = 1; // 角度已经置零
        CarInfo.dirOffset = 0;
    }
    if(RoadFlag.breakDis > CarInfo.break_dis && reset_break_yaw){
      reset_break_yaw = 0;
      RoadFlag.break_road = 0;
      RoadFlag.normal_flag = 1;
    }
//    
/*    RoadFlag.breakAngle
    RoadFlag.breakDis */
}


void normal_control(void)
{
  if(RoadFlag.stopDis > 0.7 ){
    RoadFlag.normal_flag = 0;
  }
     
}
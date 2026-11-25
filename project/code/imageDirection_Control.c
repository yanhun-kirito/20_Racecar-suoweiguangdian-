/*
 * ImageDirection.c
 *
 *  Created on: 2023年4月17日
 *      Author: hzw
 */

#include "imageDirection_Control.h"
#include "parameter.h"

#define MAX_CHANGE     25   // 最大允许单帧变化量


float OutCurvature = 0.0;
float turn_PWM;
int weights[6]={25,20,10,10,30,55};//前瞻权重




void Road_check(void)
{
//  void Stopcar();
  turn_check();
  Cross_check();
  circular_check();
  break_Road();
  normal_check();
}

void imageDirectionControl(void)
{  
    static int16 MidlineHistory[4] = {0}; // 存储四个前瞻行的中线
    
   // 直角弯控制
   if(RoadFlag.turn_left == 2 || RoadFlag.turn_right == 2) 
   {
        turncontrol();
        
   }
    // 十字控制
    else if(RoadFlag.cross == 1) {
       RoadFlag.right_circle = 0;
       RoadFlag.left_circle = 0;
       cross_contror();
    }
//    // 圆环控制
   else if(RoadFlag.left_circle == 2||RoadFlag.right_circle==2) {
        if(RoadFlag.left_circle == 2)
        {
            int8 size = 4;
            int16 total_sum = 0;
            uint8 valid_count = 0;
            
            int row =DataInfo.forwardLook1;
            for (uint8 i = 0; i < 4; i++) 
            {
                if(LeftBlack[row]<RightBlack[row])
                  LeftBlack[row]=RightBlack[row]+Width[row];
                if(Width[row] >25)
                {
                  size--;
                }
                else
                {
                total_sum += LeftBlack[row]+10;
                }
                row+=5;
            }
            CarInfo.dirOffset = ((total_sum / size) - MIDDLE)*0.90;//49是图像中值

//        for (uint8 i = 0; i < 4; i++) 
//        {
//                  int8 row;
//                  // 获取实际行号
//                  switch(i) {
//                      case 0: row = 23; break;
//                      case 1: row = 30; break;
//                      case 2: row = 34; break;
//                      case 3: row = 38; break;
//                      default: row = 10; break; 
//                  }
//                if (Width[row] > 26) 
//            {
//                // 使用历史值参与计算
//                total_sum += MidlineHistory[i]*weights[i];
//                valid_count += weights[i];
//            }
//            else 
//            {
//                // 更新历史值并参与计算
//                MidlineHistory[i] = MiddleLine[row];
//                total_sum += MiddleLine[row]*weights[i];
//                valid_count += weights[i];
//            }
//        }
//    
//         // 计算最终偏移量（四行均值）
//         CarInfo.dirOffset = (total_sum / valid_count) - MIDDLE;
//        if((dump_point >= 5&&dump_point <= 8)&&Width[25] > 25)
//        {
//          CarInfo.dirOffset = 59;
//        }
            CarInfo.dirOffset = ValueLimitint8(CarInfo.dirOffset, 60, -60);
            if(CarInfo.dirOffset<=-56)
            {
              CarInfo.dirOffset = -CarInfo.dirOffset;
            }
      // 更新历史偏移量
            CarInfo.lastDirOffset = CarInfo.dirOffset;
        }
        
        if(RoadFlag.right_circle == 2)
        {
            int8 size = 4;
            int16 total_sum = 0;
            uint8 valid_count = 0;
            
            int row =DataInfo.forwardLook1;
            for (uint8 i = 0; i < 4; i++) 
            {
                if(Width[row] > 25)
                {
                  size--;
                }
                else
                {
                total_sum += RightBlack[row]-10;
                }
                row+=5;
            }
            CarInfo.dirOffset = ((total_sum / size) - MIDDLE)*0.9;//49是图像中值
            CarInfo.dirOffset = ValueLimitint8(CarInfo.dirOffset, 60, -60);
      // 更新历史偏移量
            CarInfo.lastDirOffset = CarInfo.dirOffset;
         
        }
      circularcontrol();
    }
    // 断路控制
    else if(RoadFlag.break_road == 1) {
        breakcontrol();
    }
   
   else if(RoadFlag.stopcar == 1)
   {
      Stop_Cnotrol();
   }
//    // 停车控制
//    else if(RoadFlag.stopcar) {
//        stopcontrol();
//    }
    // 正常转向（包含虚线、折线等处理）
    else 
    {
 //       CarInfo.target_angle = 0;
        int16 total_sum = 0;
        uint8 valid_count = 0;
        // 遍历四个前瞻行
        for (uint8 i = 0; i < 4; i++) 
        {
            int8 row;
            // 获取实际行号
            switch(i) {
                case 0: row = 8; break;
                case 1: row = 15; break;
                case 2: row = 20; break;
                case 3: row = 35; break;
                case 4: row = 38; break;
                case 5: row = 40; break;
                default: row = 10; break; 
            }
            // 有效性判断（宽度大于20视为无效）
            if (Width[row] > 20) 
            {
                // 使用历史值参与计算
                total_sum += MidlineHistory[i]*weights[i];
                valid_count += weights[i];
            }
            else 
            {
                // 更新历史值并参与计算
                MidlineHistory[i] = MiddleLine[row];
                total_sum += MiddleLine[row]*weights[i];
                valid_count += weights[i];
            }
                // 计算最终偏移量（四行均值）
        CarInfo.dirOffset = (total_sum / valid_count) - MIDDLE;
        CarInfo.dirOffset = ValueLimitint8(CarInfo.dirOffset, 60, -60);

        // 突变处理（限制最大变化量）
        if(abs(CarInfo.dirOffset - CarInfo.lastDirOffset) > 45) 
        {
            CarInfo.dirOffset = CarInfo.lastDirOffset;
        }
        // 应用一阶低通滤波：Y(n) = αX(n) + (1-α)Y(n-1)
        const float alpha = 0.7; // 滤波系数
        CarInfo.dirOffset = alpha * CarInfo.dirOffset + (1 - alpha) * CarInfo.lastDirOffset;
        //限幅
        if(CarInfo.dirOffset < 4 && CarInfo.dirOffset > -4) CarInfo.dirOffset = 0;
        CarInfo.lastDirOffset = CarInfo.dirOffset;//给到转向环
    }


        // int16 total_sum = 0;
        // uint8 valid_count = 0;
        // // 遍历四个前瞻行
        // for (uint8 i = 0; i < 4; i++) 
        // {
        //     int8 row;
        //     // 获取实际行号
        //     switch(i) {
        //         case 0: row = DataInfo.forwardLook0; break;
        //         case 1: row = DataInfo.forwardLook1; break;
        //         case 2: row = DataInfo.forwardLook2; break;
        //         case 3: row = DataInfo.forwardLook3; break;
        //         default: row = DataInfo.forwardLook0; break; 
        //     }
        //     // 有效性判断（宽度大于20视为无效）
        //     if (Width[row] > 20) 
        //     {
        //         // 使用历史值参与计算
        //         total_sum += MidlineHistory[i] * weights[i];
        //         valid_count += weights[i];
        //     }
        //     else 
        //     {
        //         // 更新历史值并参与计算
        //         MidlineHistory[i] = MiddleLine[row]; // 更新历史值
            
        //         // 加权累加
        //         total_sum += MiddleLine[row] * weights[i];
        //         valid_count += weights[i];
        //     }
        // }
        //     if (valid_count > 0) {
        //     int8 avg_midline = total_sum / valid_count;
        //     int8 new_offset = avg_midline - MIDDLE;
            
        //     // 动态限幅（根据车速调整）
        //     int8 max_offset = ValueLimitint8(new_offset, 60, -60);
        //     new_offset = ValueLimitint8(new_offset, max_offset, -max_offset);
            
        //     // 平滑过渡处理（低通滤波 + 变化率限制）
        //     int16 delta = new_offset - CarInfo.lastDirOffset;
        //     if (abs(delta) > MAX_CHANGE) {
        //         new_offset = CarInfo.lastDirOffset + (delta > 0 ? MAX_CHANGE : -MAX_CHANGE);
        //     }
        //     // 应用一阶低通滤波：Y(n) = αX(n) + (1-α)Y(n-1)
        //     const float alpha = 0.7; // 滤波系数
        //     CarInfo.dirOffset = alpha * new_offset + (1 - alpha) * CarInfo.lastDirOffset;
            
        //     //限幅
        //     if(CarInfo.dirOffset < 3 && CarInfo.dirOffset > -3) CarInfo.dirOffset = 0;
        //     CarInfo.lastDirOffset = CarInfo.dirOffset;//给到转向环
    // }
}

}
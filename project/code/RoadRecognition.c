#include "RoadRecognition.h"

RoadFlagType RoadFlag;

_Bool StopCar = false;
uint8 ZebraTime = 0;
uint8 crosstime=0, ramptime=0, curvetime=0, LongStraightTime=0, intermtime=0, breaktime=0;
uint8 sum1=0;
uint8 SpaceDisNum,zebraNum;
uint8 count3=0;
float len = 0;
float Slope = 0;
uint8 Right_Close=0;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      起跑线线识别
//-------------------------------------------------------------------------------------------------------------------
//当停车标志位积累到2时，停车
void Stopcar(void)
{
  if(!RoadFlag.right_circle && !RoadFlag.left_circle && !RoadFlag.break_road && up_Flag && Col_WhiteSum > 12 && !RoadFlag.cross && check_size(5,25))
  {
//  if(RoadFlag.stopcar == 0)//连续五行白点数大于阈值
//  {
       RoadFlag.stopcar = 1;
  }
  else
  {
      RoadFlag.stopcar = 0;
  }
 
//  if(RoadFlag.stopcar == 1  && RoadFlag.stopDis1 >= 0.1)
//  {
//       RoadFlag.stopcar = 2;  
//  }
//  if(RoadFlag.stopcar == 2 && RoadFlag.stopDis1 >= 0.1)
//  {
//       RoadFlag.stopcar = 3; 
//  }
//  }
}

//直角弯识别
void turn_check(void)
{

  
  Right_Close=0;
  for(uint8 i=0;i<9;i++)
  {
    if(Width[i]>=16 && left_Flag != right_Flag&&LongLeft<=12&&Width[17]<=50)
    {
      Right_Close=1;
      break;
    }
  }
  Slope=(7.0)/(MiddleLine[8]-MiddleLine[1]);
  if(Slope>90)
    Slope=90;
  else if(Slope<-90)
    Slope=-90;
  uint8 Right_check = 0;
  Right_check=(Slope>0.00)||(Slope<-0.00)?1:0;
  if(Right_Close==1)
    Right_check=1;

  //右直角弯 
  if(!RoadFlag.right_circle && !RoadFlag.left_circle && !RoadFlag.break_road)
  {
  if(!left_Flag && right_Flag && !up_Flag && RoadFlag.turn_right == 0 && !RoadFlag.turn_left&&Right_check == 1)     //左侧无引导线，右侧有引导线，前瞻行白点数大于阈值,offseet很小（防止折线干扰）

  RoadFlag.turn_right = 1;     //右直角弯置1 
  if(!left_Flag && right_Flag && !up_Flag && right_Point[20] <= 49 && right_Point[21] <= 49 && !RoadFlag.turn_left && RoadFlag.turn_right != 2&&Right_check == 1)     //offset小 
  RoadFlag.turn_right = 2;
  if((RoadFlag.turn_right == 2||RoadFlag.turn_right == 1) && up_Flag||Right_check == 0)
  {
    RoadFlag.turn_right = 0;
    CarInfo.angle_offset = 0;
//    SL_output-=400;
//    SR_output-=200;
    reset_yaw_right = 0;
    
//    RoadFlag.rightAngle = -31;
//    CarInfo.target_angle = 0;
  }
  //左直角弯   
  if(left_Flag && !right_Flag && !up_Flag && RoadFlag.turn_left == 0 && !RoadFlag.turn_right&&Right_check == 1 )     //右侧无引导线，左侧有引导线，前瞻行白点数大于阈值
  RoadFlag.turn_left = 1;      //左直角弯置1
  if(left_Flag && !right_Flag && !up_Flag && left_Point[20] <= 49 && left_Point[21] <= 49 && !RoadFlag.turn_right && RoadFlag.turn_left != 2 &&Right_check == 1)
  RoadFlag.turn_left = 2;
 
  if((RoadFlag.turn_left == 2||RoadFlag.turn_left == 1) && up_Flag||Right_check == 0)
  {
    RoadFlag.turn_left = 0;
    CarInfo.angle_offset = 0;
//    SL_output-=200;
//    SR_output-=400;
    reset_yaw_left = 0;
//    RoadFlag.leftAngle = 31;
//    CarInfo.target_angle = 0;
  }
  }

}


//十字检查,先检测到十字，标志位置1，然后十字定距，再识别出十字,标志位置2
void Cross_check(void)
{
  if(left_Flag && right_Flag && up_Flag && !RoadFlag.break_road&& left_Point[10]< 40 && right_Point[10] < 40 && WhiteSum>40)//左侧有引导线，右侧有引导线，上方有引导线，某行白点数大于100，用于定十字识别位置
  {
      RoadFlag.cross = 1;//置一后，在控制里定距，然后退出十字
      RoadFlag.left_circle = 0;
      RoadFlag.right_circle= 0;
      
  } 
  if(RoadFlag.crossDis > CarInfo.cross_dis)
  {
      RoadFlag.cross = 0;
      RoadFlag.normal_flag = 1;
  }
}
//环岛识别
void circular_check(void)//左侧无引导线，右侧有引导线，上方有引导线，右侧边界在递减，则位右圆环，确认圆环切入行,进入圆环，陀螺仪定角330后，即出圆环，
{
  //右环岛
  if(!RoadFlag.left_circle && !RoadFlag.break_road && !RoadFlag.cross)//边界宽度大于阈值
  {  

      if(!left_Flag && up_Flag && right_Flag && RoadFlag.right_circle==0  && dump_point == 10&&Width[25]<Width[4])//跳变点是俩个，识别到圆环
      {
        RoadFlag.right_circle = 1;
      }
      if(RoadFlag.right_circle == 1 && dump_point == 5)
      {
        RoadFlag.right_circle = 2;
      }
//      if(RoadFlag.circleDis1 > CarInfo.circur_dis && RoadFlag.right_circle == 3)
//      {
//         RoadFlag.right_circle = 0;
//      }
      if(RoadFlag.right_circle == 3)
      {
         RoadFlag.right_circle = 0;
         RoadFlag.circleDis = 0;
//         RoadFlag.normal_flag = 1;
      }
  }
  //左圆环
  if(!RoadFlag.right_circle && !RoadFlag.break_road && !RoadFlag.cross)//边界宽度大于阈值
  {
      if(left_Flag && !right_Flag && up_Flag && RoadFlag.left_circle==0  && dump_point == 10&&Width[25]<Width[4])
      {
        RoadFlag.left_circle = 1;//提前识别圆环，
      }
      if(RoadFlag.left_circle == 1 && dump_point == 5)
      { 
        RoadFlag.left_circle = 2;
      }
//      if(RoadFlag.circleDis1 > CarInfo.circur_dis && RoadFlag.left_circle == 3)
//      {
//        RoadFlag.left_circle = 0;
//      }
       if(RoadFlag.left_circle == 3)
      {
         RoadFlag.left_circle = 0;
         RoadFlag.circleDis = 0;
         
      }
  }
}

//断路识别
void break_Road(void)//左侧无引导线，右侧无引导线，连续五行无边界
{
  if(!left_Flag && !right_Flag && trackLostFlag && !up_Flag && RoadFlag.break_road != 2 && !RoadFlag.turn_right && !RoadFlag.turn_left && !RoadFlag.left_circle && !RoadFlag.right_circle && LongRight == 0 && LongLeft == 0)//上下左右找不到，而且连续丢变
  {
      RoadFlag.break_road = 1;
      RoadFlag.cross = 0;
  }
//  if(RoadFlag.break_road == 1 && RoadFlag.breakDis > CarInfo.break_dis)//大于定距进入转向
//  { 
//      RoadFlag.break_road = 0;
//  }

}

void normal_check(void)
{
    if(RoadFlag.normal_flag == 1)
    {
        RoadFlag.left_circle = 0;
        RoadFlag.right_circle = 0;
        RoadFlag.break_road = 0;
    }
    normal_control();
}









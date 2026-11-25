#include "Speed_control.h"
#include "math.h"

#define BUZZER_PIN              (P19_4)                                         // 定义主板蜂鸣器引脚
#define LED1                    (P19_0)


//对车车的控制函数
//static _Bool reset_i = false;
float term = 0, target_angular = 0,angular_output= 0;                   //定义总的转向,角度环输出,              //角速度环输出.
float ap=0,ai=0,ad=0;   //角度环临时变量
float w_p1,w_p2,w_i,w_d; //转向环临时变量
int16 SL_output = 0, SR_output = 0;
int16 IL_offset = 0, IR_offset = 0, LD_offset = 0, RD_offset = 0;
int16 velocity_integral = 0;
float target_left,target_right, target_left_last, target_right_last;
uint8 cross_nums,circle_nums,break_nums,turn_nums;
float a_L = 0, a_R = 0; //加速度
float Vl_FF = 0, Vr_FF = 0;//速度前馈
//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器读取函数
//-------------------------------------------------------------------------------------------------------------------
void Speed_Get(void)
{
    float car_velocity = 0;                     //定义车的速度
    float dis = 0.0;                            //定义距离
    float l_dis,r_dis;                          //左右的距离

    CarInfo.R_encoder =  encoder_get_count(TC_CH27_ENCODER);      //读取右轮编码器数值
    CarInfo.L_encoder =  -encoder_get_count(TC_CH58_ENCODER);    //读取左轮编码器数值

    encoder_clear_count(TC_CH07_ENCODER);                      //清除右轮编码器数值
    encoder_clear_count(TC_CH58_ENCODER);                      //清除左轮编码器数值
    
    
    //速度计算
    CarInfo.L_current_velocity = (((float)CarInfo.L_encoder) /ONE_METER /DELTA_T*100);          //左边的速度    计算速度，看看惯性导航时要不要进行速度融合
    CarInfo.R_current_velocity = (((float)CarInfo.R_encoder) /ONE_METER /DELTA_T*100);          //右边的速度    速度的单位是 m/s*1000
    car_velocity = (CarInfo.L_current_velocity + CarInfo.R_current_velocity)/2 ;                         //平均速度
    //差速限制
//    if(CarInfo.L_current_velocity - CarInfo.R_current_velocity > 120) CarInfo.L_current_velocity = 120;
//    if(CarInfo.R_current_velocity - CarInfo.L_current_velocity > 120) CarInfo.R_current_velocity = 120;
    //动态目标速度

//    CarInfo.base_speed = 45 - CarInfo.dirOffset* 0.2;
    
    //距离计算
    l_dis = (float)CarInfo.L_encoder / ONE_METER;
    r_dis = (float)CarInfo.R_encoder / ONE_METER;
    dis = (l_dis + r_dis) / 2;
    
    CarInfo.Total_Distance += dis;              //平局距离累加
    CarInfo.Car_velocity = car_velocity;        //更新速度值
    
    //打印调参
//    printf("%f, %f, %f, %f\n",CarInfo.L_current_velocity,   CarInfo.R_current_velocity,target_left, target_right );

//    printf("%d\n",CarInfo.dirOffset);
//    printf("%f\n",CarInfo.YawRate);
    //十字行走距离
    if(RoadFlag.cross == 1)          RoadFlag.crossDis += dis;//进入标志位2，记录十字距离
    else                        RoadFlag.crossDis = 0;
                         
    //断路行走距离
    if(RoadFlag.break_road == 1)          RoadFlag.breakDis += dis;
    else                             RoadFlag.breakDis = 0;
    
//    //断路打角
//    if(RoadFlag.break_road == 1)          RoadFlag.breakAngle = CarInfo.Yaw;//正
//    else                             RoadFlag.breakAngle = 0;

    //圆环行走距离
    if(RoadFlag.left_circle == 2)          RoadFlag.circleDis += dis;
    else if(RoadFlag.right_circle == 2)   RoadFlag.circleDis += dis;
    else                                    RoadFlag.circleDis = 0;
    //圆环打角 左正右负
    if(RoadFlag.right_circle == 2)          RoadFlag.right_circle_Angle = CarInfo.Yaw;
    else                                     RoadFlag.right_circle_Angle = 0;
    
    if(RoadFlag.left_circle == 2)           RoadFlag.left_circle_Angle = -CarInfo.Yaw;
    else                                     RoadFlag.left_circle_Angle = 0;
//    //直角打角
//    if(RoadFlag.turn_left == 2)    RoadFlag.leftAngle = - CarInfo.Yaw ;
//    else                           RoadFlag.leftAngle = 0;
//    
//    if(RoadFlag.turn_right == 2)    RoadFlag.rightAngle = -CarInfo.Yaw ;
////    else                           RoadFlag.rightAngle = 0;
    
    //脱离控制
    if(RoadFlag.normal_flag == 1) {
        RoadFlag.stopDis += dis;
    } 
    else{
        RoadFlag.stopDis = 0;
    }
      
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      速度控制函数          (包含转向控制)

//  @brief      双pid转向串级速度控制
//-------------------------------------------------------------------------------------------------------------------
void Speed_Control(void)
{
     gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);                        // 初始化 BUZZER_PIN 输出 默认低电平 推挽输出模式
     gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);          // 初始化 LED1 输出 默认高电平 推挽输出模式
     if(RoadFlag.turn_left == 2 || RoadFlag.turn_right == 2)//如果是直角，给pid赋值
    {
      //速度环积分置0    
      //角度环pid
      if(RoadFlag.turn_left == 2)
      {   
          gpio_set_level(BUZZER_PIN, GPIO_HIGH);
          CarInfo.angle_offset = CarInfo.target_angle - CarInfo.Yaw;//角度偏差
          float p_gyro = CarInfo.a_kp * CarInfo.angle_offset;
          float p2_gyro = CarInfo.a_kp2 * CarInfo.angle_offset * fabs(CarInfo.angle_offset);
          float d_gyro = CarInfo.a_kd * CarInfo.YawRate;// 微分项
          float d2_gyro = CarInfo.a_kd2 * (CarInfo.angle_offset - CarInfo.last_angle_offset);
//          float L_extra = (50 - RoadFlag.left_circle) * 1.9;
          CarInfo.last_angle_offset = CarInfo.angle_offset;//更新角度偏差
          angular_output = p_gyro + p2_gyro + d_gyro + d2_gyro;
          if(angular_output > 100) angular_output = 100;
          if(angular_output < -100) angular_output = -100;
        
         target_left = CarInfo.turn_speed - angular_output;// - L_extra;        //目标速度（内）
         target_right = CarInfo.turn_speed + angular_output;//  + L_extra  ;       //目标速度（外）
//         if(target_left < 0) target_left = 0;
         
      }
      if(RoadFlag.turn_right == 2)
      { 
        gpio_set_level(BUZZER_PIN, GPIO_HIGH);
        CarInfo.angle_offset = CarInfo.target_angle - CarInfo.Yaw;//角度偏差
        float p_gyro = CarInfo.a_kp * CarInfo.angle_offset;
        float p2_gyro = CarInfo.a_kp2 * CarInfo.angle_offset * fabs(CarInfo.angle_offset);
        float d_gyro = CarInfo.a_kd * CarInfo.YawRate;// 微分项
        float d2_gyro = CarInfo.a_kd2 * (CarInfo.angle_offset - CarInfo.last_angle_offset);
//        float R_extra = (50 - RoadFlag.right_circle) * 1.9;
        CarInfo.last_angle_offset = CarInfo.angle_offset;//更新角度偏差
        
        angular_output = p_gyro + p2_gyro + d_gyro + d2_gyro;
        if(angular_output > 100) angular_output = 100;
        if(angular_output < -100) angular_output = -100;
       
       target_left = CarInfo.turn_speed - angular_output;// + R_extra;        //目标速度（外）
       target_right = CarInfo.turn_speed + angular_output;//  - R_extra;       //目标速度（内）
//       if(target_right < 0) target_right = 0;
       
      }
     
    }
//    /***停车出界保护***/
//    else if(ImageInfo.threshold < 105)
//    {
//        target_left = 0;
//        target_right = 0;
//    }
  
//     else if(RoadFlag.break_road == 2)//进入控制，定角度，定距
//     {
//      CarInfo.angle_offset = CarInfo.target_angle - CarInfo.Yaw;//角度偏差
//      float p_gyro = CarInfo.a_kp * CarInfo.angle_offset;
//      float d_gyro = CarInfo.a_kd * CarInfo.YawRate;// 微分项
//      
//       velocity_integral+= CarInfo.angle_offset;
//       
//      if(velocity_integral > 100) velocity_integral = 100;
//      if(velocity_integral< -100) velocity_integral = -100;
//
//      angular_output = p_gyro + d_gyro;
//        target_left = CarInfo.break_speed - angular_output;        //100-50
//        target_right = CarInfo.break_speed + angular_output;       //100+50
//      
//      CarInfo.last_angle_offset =  CarInfo.angle_offset;
//     }
//     else if(RoadFlag.stopcar == 2)
//     {
//       target_left = 0;
//       target_right = 0;
////       Start = false;
////       Start_Flag = false;
//     }
   else   //不是直角,圆环,断路
   {
    float P_term =  CarInfo.w_kp1 * CarInfo.dirOffset;
    float P2_term = CarInfo.w_kp2 * CarInfo.dirOffset * abs(CarInfo.dirOffset); // 非线性项
    float err = CarInfo.dirOffset - CarInfo.lastDirOffset;
    static float err_last;
    float P3_term = CarInfo.w_kd * (0.3 * err + 0.7 * err_last);//一阶低通滤波
    err_last = err;
//    float P3_term = CarInfo.w_kd * ( CarInfo.dirOffset - CarInfo.lastDirOffset);//
    float gndk = CarInfo.w_ki * CarInfo.YawRate;
        // 更新历史误差
    CarInfo.lastDirOffset = CarInfo.dirOffset;
    /******差速比计算*******/
    // 合成转向量
    term = P_term + P2_term  + P3_term + gndk;
//    if(term > 90) term = 90;
//    if(term < -90) term = -90; 
    if(abs(CarInfo.dirOffset)>6) gpio_set_level(LED1 , 1);
//    if(target_left < -5) target_left = -5;
//    if(target_right < -5) target_right = -5;
    
//    // ===== 计算差速比 =====//
//    float raw_delta = 0.0f;
//    // 避免除0错误（当速度很低时）
//    if (CarInfo.base_speed > MIN_SPEED) {
//       raw_delta = term / CarInfo.base_speed; // 核心转换公式：δ = 转向环输出  / 基础速度
//    }
//    else {
//        raw_delta = term / 5;              // 低速时直接使用原始值（保证转向能力）
//    }
//    //限幅
//    if(raw_delta > MAX_DELTA) raw_delta = MAX_DELTA;
//    if(raw_delta < MIN_DELTA) raw_delta = MIN_DELTA;
    target_left = CarInfo.base_speed - term;//* (1 - raw_delta); 
    target_right = CarInfo.base_speed + term;//* (1 + raw_delta);       //目标速度
//    if(target_left<0) target_left = 0;
//    if(target_right<0) target_right = 0;
     
    }
    //速度前馈计算
    a_L = (target_left- target_left_last) / DELTA_T;            //左轮前馈加速度
    a_R = (target_right - target_right_last) / DELTA_T;         //右轮前馈加速度
    target_left_last = target_left;                             //更新左轮目标速度
    target_right_last = target_right;                           //更新右轮目标速度
    Vl_FF = CarInfo.vl_kf * a_L;                          //左轮速度前馈计算（Speed FeedForward）
    Vr_FF = CarInfo.vr_kf * a_R;                          //右轮速度前馈计算
    //左轮速度PID控制 (增量式)
    static int16 L_offset_L_Last = 0 , L_offset_Last = 0, L_offset = 0;
    static float Lki_adjusted = 0; 
//    static float Lkp_adjusted = 0;
    L_offset_L_Last = L_offset_Last;
    L_offset_Last = L_offset;
    L_offset = (int16)(target_left - CarInfo.L_current_velocity);
    IL_offset = L_offset;
    LD_offset = L_offset - 2 * L_offset_Last+ L_offset_L_Last;
    
    if(IL_offset >= I_TERM_LIMIT) IL_offset = I_TERM_LIMIT;
    else if(IL_offset <= -I_TERM_LIMIT) IL_offset = -I_TERM_LIMIT;
    if(abs(L_offset) > 8){
      Lki_adjusted = CarInfo.vl_ki * 0.1;
    }
    else{
      Lki_adjusted = CarInfo.vl_ki;
    }
    if ( (target_left > 0 && target_left_last <= 0) || (target_left < 0 && target_left_last >= 0) ) {
      Lki_adjusted = CarInfo.vl_ki * 0.2; // 目标方向变了！积分器清零！
//      target_left_last = target_left;
//      Lkp_adjusted = CarInfo.vl_kp * 0.5;
    }
    else{
      Lki_adjusted = CarInfo.vl_ki;
//      target_left_last = target_left;
//      Lkp_adjusted = CarInfo.vl_kp;
    }
    target_left_last = target_left; // 记录这次的目标速度，下次比较用
    SL_output += (int16)(CarInfo.vl_kp * (L_offset - L_offset_Last) + Lki_adjusted * L_offset + CarInfo.v_kd * LD_offset);
    
    //右轮速度PID控制（增量式）
    static int16 R_offset = 0, R_offset_Last = 0 , R_offset_L_Last = 0;
    static float Rki_adjusted = 0;
//    static float Rkp_adjusted = 0;
    R_offset_L_Last = R_offset_Last;
    R_offset_Last =  R_offset;
    R_offset = (int16)(target_right - CarInfo.R_current_velocity);
    IR_offset = R_offset;
    RD_offset = R_offset - 2 * R_offset_Last+ R_offset_L_Last;
    
    if(IR_offset >= I_TERM_LIMIT) IR_offset = I_TERM_LIMIT;
    else if(IR_offset <= -I_TERM_LIMIT) IR_offset = -I_TERM_LIMIT;
    
    if(abs(R_offset) > 8){
      Rki_adjusted = CarInfo.vr_ki * 0.1;
    }
    else{
      Rki_adjusted = CarInfo.vr_ki;
    }
    if ( (target_right > 0 && target_right_last <= 0) || (target_right < 0 && target_right_last >= 0) ) {
      Rki_adjusted = CarInfo.vr_ki * 0.2; // 目标方向变了！积分器清零！
//      target_right_last = target_right;
//      Rkp_adjusted = CarInfo.vr_kp * 0.5;
    }
    else{
      Rki_adjusted = CarInfo.vr_ki;
//      target_right_last = target_right;
//      Rkp_adjusted = CarInfo.vr_kp;
    }
    SR_output += (int16)(CarInfo.vr_kp * (R_offset - R_offset_Last) + Rki_adjusted * R_offset + CarInfo.v_kd * RD_offset);
    //输出限幅
//    if(target_left< 50 || target_right < 50) Vl_FF = Vr_FF = 0;
    if(SL_output > 4000 ) SL_output = 4000;
    if(SL_output < -3000 ) SL_output = -3000;
    if(SR_output > 4000 ) SR_output = 4000;
    if(SR_output < -3000) SR_output = -3000;
    if(Vl_FF > 1500) Vl_FF = 1500;
    if(Vl_FF < -1500) Vl_FF = -1500;
    if(Vr_FF > 1500) Vr_FF = 1500;
    if(Vr_FF < -1500) Vr_FF = -1500;
}



float linear_function(float a, int16_t r, int16_t r_shift, float b)
{
    return a * (r - r_shift) + b;
}









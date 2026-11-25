#include "AngleCalculate.h"
#include "zf_device_ips114.h"

uint8 IsMagnet;
_Bool IsGyroOffsetReset = true;

ICM20602Datatypedef    ICM20602;
AttitudeDatatypedef    GyroOffset;
int32 temp;                                      //记录当前偏航角，用于特殊元素记录打角
//-------------------------------------------------------------------------------------------------------------------
//  @brief      陀螺仪零飘初始化
//-------------------------------------------------------------------------------------------------------------------
static void GyroOffset_init(void)
{
    static uint16 Count = 0;
 
    if(Count == 0)
    {
//        GyroOffset.Xdata = 0;
//        GyroOffset.Ydata = 0;
        GyroOffset.Zdata = 0;
    }

    if(Count == 1000)
    {
//        GyroOffset.Xdata *= 0.001;
//        GyroOffset.Ydata *= 0.001;
        GyroOffset.Zdata *= 0.001;
        IsGyroOffsetReset = ZF_FALSE;
        Count = 0;
    }
    else
    {
        imu660ra_get_acc();
        imu660ra_get_gyro();

//        GyroOffset.Xdata += imu963ra_gyro_x;
//        GyroOffset.Ydata += imu963ra_gyro_x;
        GyroOffset.Zdata += imu660ra_gyro_z;
        Count++;
    }
    
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      车体角度姿态采集
//-------------------------------------------------------------------------------------------------------------------
void Attitude_Geting(void)
{
    if(IsGyroOffsetReset == ZF_TRUE)
    {
        //零偏矫正过程中不进行姿态结算，并一定要保证姿态模块静止!!
        GyroOffset_init();
        return;
    }
    imu660ra_get_gyro();
    imu660ra_get_acc();
    //对加速度使用一阶低通滤波
    ICM20602.ACCX = ((float)imu660ra_acc_transition(imu660ra_acc_x)) * new_weight + ICM20602.Last_ACCX * old_weight;
    ICM20602.ACCY = ((float)imu660ra_acc_transition(imu660ra_acc_y)) * new_weight + ICM20602.Last_ACCY * old_weight;
    ICM20602.ACCZ = ((float)imu660ra_acc_transition(imu660ra_acc_z)) * new_weight + ICM20602.Last_ACCZ * old_weight;

    //记录上次的加速度
    ICM20602.Last_ACCX = ICM20602.ACCX;
    ICM20602.Last_ACCY = ICM20602.ACCY;
    ICM20602.Last_ACCZ = ICM20602.ACCZ;

//    ICM20602.GYROX = ((float)imu963ra_gyro_x - GyroOffset.Xdata) * 0.069930069 * 3.1415926535898 / 180;
//    ICM20602.GYROY = ((float)imu963ra_gyro_y - GyroOffset.Ydata) * 0.069930069* 3.1415926535898 / 180;
    ICM20602.GYROZ = ((float)imu660ra_gyro_z - GyroOffset.Zdata) /16.4f * 3.1415926535898 / 180;//角速度，单位是弧度***7.1

    CarInfo.YawRate = ((float)imu660ra_gyro_z - GyroOffset.Zdata) /16.4f;//角速度，单位是°
    //姿态解算    
    if(fabs(CarInfo.YawRate) < 0.2)   CarInfo.YawRate = 0;
    IMU_AHRSupdate_noMagnetic(&ICM20602);

    //四元数转欧拉角
    IMU_quaterToEulerianAngles();
 
    //用户调用
//    CarInfo.Pitch = -EulerAngle.Pitch; //俯仰角
//    CarInfo.YawRate = ICM20602.GYROZ * 180 / PI; //偏航角加速度
    
    
    
    //因为存在温漂，偏航角速度会在-3到3的范围内漂移，这里对其温漂作限制

    CarInfo.YawRate = ICM20602.GYROZ * 180 / 3.1415926535898; //偏航角速度
    if(fabs(CarInfo.YawRate) < 3)   CarInfo.YawRate = 0;
    CarInfo.Yaw += (CarInfo.YawRate * DELTA_T); //积分   
//    if(CarInfo.Yaw > 360 )CarInfo.Yaw = 360;
//    if(CarInfo.Yaw < -360 )CarInfo.Yaw = -360;
//    Kalman_getAngle(Yaw,icm20602_gyro_x , 0.01);//卡尔曼滤波计算角度
//    Kalman_getAngle(EulerAngle.Yaw,icm20602_gyro_z , 0.01);
    //转为角度    
  
//     CarInfo.Yaw = kalman.angle;
//    CarInfo.Yaw = -EulerAngle.Yaw;;
    
//    ADInfo.Yaw = CarInfo.Yaw;

}


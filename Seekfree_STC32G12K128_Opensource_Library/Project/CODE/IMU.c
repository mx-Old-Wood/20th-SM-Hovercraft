#include "headfile.h"
#include <math.h>

#define sampleFreq	200.0f			// 采样频率hz
#define twoKpDef	(2.0f*0.5f)	// 2*比例增益
#define twoKiDef	(2.0f*0.0f)	// 2 * 积分增益
#define PI                  (3.1415926535898)

//---------------------------------------------------------------------------------------------------
// 变量定义
offset IMU_offset;
volatile float twoKp = twoKpDef;											// 2 * 比例增益 (Kp)
volatile float twoKi = twoKiDef;											// 2 * 积分增益(Ki)
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;					// quaternion of sensor frame relative to auxiliary frame
volatile float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;	// integral error terms scaled by Ki
volatile float Yaw = 0,Roll = 0,Pitch = 0;
volatile float ax = 0,ay = 0,az = 0,gx = 0,gy = 0,gz = 0,mx = 0,my = 0,mz =0;
float yaw_l=0;
//---------------------------------------------------------------------------------------------------
// 函数声明

float invSqrt(float x);
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
float  gyro_z =0;
//====================================================================================================
// 功能

float gyro_Offset_flag=0;
void IMU_offset_init()               //去零飘
{
    uint16 i = 0;
    IMU_offset.axdata = 0;
    IMU_offset.aydata = 0;
    IMU_offset.azdata = 0;
    IMU_offset.gxdata = 0;
    IMU_offset.gydata = 0;
    IMU_offset.gzdata = 0;
//	delay_ms(500);

    for (i=0;i < 200;i++)
    {
        imu660ra_get_acc();            
        imu660ra_get_gyro();
        
        IMU_offset.gxdata += imu660ra_gyro_transition(imu660ra_gyro_x);
        IMU_offset.gydata += imu660ra_gyro_transition(imu660ra_gyro_y);
        IMU_offset.gzdata += imu660ra_gyro_transition(imu660ra_gyro_z);
        IMU_offset.axdata += imu660ra_acc_transition(imu660ra_acc_x);
        IMU_offset.aydata += imu660ra_acc_transition(imu660ra_acc_y);
        IMU_offset.azdata += imu660ra_acc_transition(imu660ra_acc_z);

        
       	delay_ms(5);
    }
    //得到平均零飘值
    IMU_offset.axdata /= 1000.0;//乘以0.001为了均值
    IMU_offset.aydata /= 1000.0;
    IMU_offset.azdata /= 1000.0;
    IMU_offset.gxdata /= 1000.0;//乘以0.001为了均值
    IMU_offset.gydata /= 1000.0;
    IMU_offset.gzdata /= 1000.0;

   gyro_Offset_flag=1;
}
//获取数据
void IMU_GetValues(void)
{
        imu660ra_get_acc();
        imu660ra_get_gyro();
        
        ax= imu660ra_acc_transition(imu660ra_acc_x); 
        ay= imu660ra_acc_transition(imu660ra_acc_y); 
        az= imu660ra_acc_transition(imu660ra_acc_z);
        
        gx = imu660ra_gyro_transition(imu660ra_gyro_x) ;
        gy = imu660ra_gyro_transition(imu660ra_gyro_y) ;     
        gz = imu660ra_gyro_transition(imu660ra_gyro_z) ;
        

}

void IMU_YAW_integral(void)
{
    IMU_GetValues();
    //陀螺仪获取到数据与零漂做差
    if(gx - IMU_offset.gxdata <1&&gx - IMU_offset.gxdata > -1)
    { gx = IMU_offset.gxdata;}
    if(gy - IMU_offset.gydata < 1&&gy - IMU_offset.gydata > -1)
    { gy = IMU_offset.gydata;}
    if(gz - IMU_offset.gzdata < 1&&gz - IMU_offset.gzdata > -1)
    {
        gz = IMU_offset.gzdata;
    }
    if(ax - IMU_offset.axdata < 1&&ax - IMU_offset.axdata > -1)
    { ax = IMU_offset.axdata;}
    if(ay - IMU_offset.aydata < 1&&ay - IMU_offset.aydata > -1)
    { ay = IMU_offset.aydata;}
    if(az - IMU_offset.azdata < 1&&az - IMU_offset.azdata > -1)
    {az = IMU_offset.azdata;}
    

   gyro_z = (gz-IMU_offset.gzdata ) * PI / 180.0f /14.3f;
   MahonyAHRSupdateIMU(-(gx - IMU_offset.gxdata), gy - IMU_offset.gydata, -(gz- IMU_offset.gzdata), -(ax - IMU_offset.axdata), ay - IMU_offset.aydata, -(az-IMU_offset.azdata));
    
}



void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az) 
{
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;
    
        gx *= 0.0174533;
        gy *= 0.0174533;
        gz *= 0.0174533;
    
	//仅当加速度计测量有效时才计算反馈（避免加速度计归一化中的nan
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
		// 归一化加速度计测量
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;        

		// 估算重力方向和垂直于磁通量的矢量
		halfvx = q1 * q3 - q0 * q2;
		halfvy = q0 * q1 + q2 * q3;
		halfvz = q0 * q0 - 0.5f + q3 * q3;
	
		// 误差是估算重力方向和测量重力的方向的叉乘之和
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		// Compute and apply integral feedback if enabled
		if(twoKi > 0.0f) {
			integralFBx += twoKi * halfex * (1.0f / sampleFreq);	// 被Ki缩放的积分误差
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);
			gx += integralFBx;	// 应用积分反馈
			gy += integralFBy;
			gz += integralFBz;
		}
		else {
			integralFBx = 0.0f;	// 防止积分饱和
			integralFBy = 0.0f;
			integralFBz = 0.0f;
		}

		// 应有比例反馈
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}
    // 积分四元数变化率(进)
	gx *= (0.5f * (1.0f / sampleFreq));		// 预乘四元数
	gy *= (0.5f * (1.0f / sampleFreq));
	gz *= (0.5f * (1.0f / sampleFreq));
	qa = q0;
	qb = q1;
	qc = q2;
	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx); 
	
	// 正常化四元数
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
//     Pitch =simple_asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch ,转换为度数
     Yaw=-my_atan2(2*q1*q2+2*q0*q3,-q3*q3-q2*q2+q0*q0+q1*q1)*57.295779;//四元数获取偏航角
	 yaw_union();
//        Yaw=-Yaw;
//            Yaw = Yaw + 360;
        if(Yaw - yaw_l > 1 || Yaw - yaw_l  < -1)
            yaw_l = Yaw;
        else
            Yaw = yaw_l;
}


float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

void IMUangle(void)
{
      Pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch ,转换为度数
      Roll =my_atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
      Yaw=-my_atan2(2 * q1 * q2 + 2 * q0 * q3, -q3*q3 - q2 *q2 + q0* q0 + q1*q1)* 57.3; // yaw
      if(Yaw < 0)
      {
          Yaw  = Yaw + 360.0;
      }
      
//      
}

void yaw_union(void)             //顺时针为正
{
	if(Yaw<0)
	{
		Yaw = Yaw + 360;
	}
	if(Yaw>360)
	{
		Yaw = Yaw - 360;
	}
}


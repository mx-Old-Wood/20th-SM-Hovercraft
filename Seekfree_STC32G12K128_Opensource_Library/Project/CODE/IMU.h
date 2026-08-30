#ifndef _IMU_H_
#define _IMU_H_

//----------------------------------------------------------------------------------------------------
//变量声明
#include "headfile.h"
typedef struct
{
    float axdata;
    float aydata;
    float azdata;
    float gxdata;
    float gydata;
    float gzdata;
    float mxdata;
    float mydata;
    float mzdata;
}offset;                           //x,y,z轴的角速度偏移量
extern offset IMU_offset;
extern volatile float twoKp;			// 2 * proportional gain (Kp)
extern volatile float twoKi;			
extern volatile float q0, q1, q2, q3;	
extern volatile float Yaw,Roll,Pitch;
extern volatile float ax ,ay ,az ,gx ,gy ,gz ,mx ,my ,mz ;


void IMU_offset_init(void);
void IMU_GetValues(void);
extern void IMU_YAW_integral(void);
extern void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
void IMUangle(void);
void yaw_union(void);       
extern int16 cnt;
extern float gyro_Offset_flag;
extern float gyro_z;
#endif


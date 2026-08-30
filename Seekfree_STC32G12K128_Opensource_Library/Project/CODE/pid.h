#ifndef __PID_H_
#define __PID_H_

typedef struct 
{
	float Kp;
	float Ki;
	float Kd;	
	float error;               //偏差值
    float Last_Error;          //上一次误差	
    float OutputMax;           //位置式PID输出最大值，用于限幅
	float OutputMin; 
  	float output;
    float increment;              //定义增量
    float Limit_increment;        //积分限幅
}PID_Pos;

typedef struct 
{
    float Kp;                        //P参数
    float Ki;                        //I参数
    float Kd;                        //D参数
    float Err;                      //偏差值
    float Err_Last;                 //定义上上个偏差值
    float Err_Next;                 //定义上一个偏差值
    float Out;                      //电机输出
	float OutputMax;                //位置式PID输出最大值，用于限幅
	float OutputMin; 
    float increment;                //定义增量
    float Limit_increment;          //增量限幅
    float proportion;               //PID比例项
    float integration;              //PID积分项
    float differential;             //PID微分项
}Pid_Inc;


/*********************************函数声明***********************************/
float PID_pos(PID_Pos* pid,float present, float aim);   
float Pid_Increse(Pid_Inc *pid,float present, float aim);
float limit_value(float value,float positive_value,float negative_value);
void  PID_init(void);
void PID_Type_Init(uint8 Type);
void PID_Parameter_Set(void);


/*******************************全局变量声明*********************************/
extern PID_Pos left_Angle;
extern PID_Pos right_Angle;
extern PID_Pos left_rate;
extern PID_Pos right_rate;
extern PID_Pos ccd_left;
extern PID_Pos ccd_right;
extern Pid_Inc speed;
extern float xdata PID_Parameter[7];


#endif

#include "headfile.h"

///////暂时考虑使用位置式PID   采用PD控制
///////角度环输出期望角速度给角速度环

PID_Pos left_Angle;
PID_Pos right_Angle;
PID_Pos left_rate;
PID_Pos right_rate;
PID_Pos ccd_left;
PID_Pos ccd_right;
Pid_Inc speed;
float xdata PID_Parameter[7];

void PID_init(void)
{
	speed.Kp = 28;
	speed.Ki = 1;
	speed.Kd = 0;
	speed.OutputMax = 220;                  
	speed.OutputMin = 0;
	speed.increment = 0.0;                  //定义增量
    speed.Limit_increment = 50;             //积分限幅
	speed.Err = 0.0;                        //偏差值
    speed.Err_Last = 0.0;                   //定义上上个偏差值
    speed.Err_Next = 0.0;                   //定义上一个偏差值
//	speed.error = 0.0;                      //偏差值
//	speed.Last_Error = 0.0;                 //上一次误差	
	
	left_Angle.Kp = 1.1;
	left_Angle.Ki = 50;
	left_Angle.Kd = 20;	
	left_Angle.OutputMax = 1000;
	left_Angle.OutputMin = -1000;
	left_Angle.error = 0.0;                 //偏差值
	left_Angle.Last_Error = 0.0;            //上一次误差	
	left_Angle.output = 0.0;
	left_Angle.increment = 0.0;             //定义增量
    left_Angle.Limit_increment = 0.0;       //积分限幅
	
	left_rate.Kp = 3.8;
	left_rate.Ki = 10;
	left_rate.Kd = 20;
	left_rate.OutputMax = 100;
    left_rate.OutputMin = -100;  
	left_rate.error = 0.0;                  //偏差值
	left_rate.Last_Error = 0.0;             //上一次误差	
	left_rate.output = 0.0;
	left_rate.increment = 0.0;              //定义增量+
	left_rate.Limit_increment = 0.0;        //积分限幅

	right_Angle.Kp = 1.1;
	right_Angle.Ki = 50;
	right_Angle.Kd = 20;
	right_Angle.OutputMax = 1000;
	right_Angle.OutputMin = -1000;
	right_Angle.error = 0.0;                 //偏差值
	right_Angle.Last_Error = 0.0;            //上一次误差	
	right_Angle.output = 0.0;
	right_Angle.increment = 0.0;             //定义增量
	right_Angle.Limit_increment = 0.0;       //积分限幅
	
	right_rate.Kp = 3.8;
	right_rate.Ki = 10;
	right_rate.Kd = 20;
	right_rate.OutputMax = 100; 
    right_rate.OutputMin = -100;
	right_rate.error = 0.0;                  //偏差值
	right_rate.Last_Error = 0.0;             //上一次误差	
	right_rate.output = 0.0;
	right_rate.increment = 0.0;              //定义增量
	right_rate.Limit_increment = 0.0;        //积分限幅


/********************ccd***************************/	

	ccd_left.Kp = 3.3;
	ccd_left.Ki = 10;
	ccd_left.Kd = 20;
	ccd_left.OutputMax = 100; 
    ccd_left.OutputMin = -100;
	ccd_left.error = 0.0;                  //偏差值
	ccd_left.Last_Error = 0.0;             //上一次误差	
	ccd_left.output = 0.0;
	ccd_left.increment = 0.0;              //定义增量
	ccd_left.Limit_increment = 0.0;        //积分限幅
	
	ccd_right.Kp = 3.3;
	ccd_right.Ki = 10;
	ccd_right.Kd = 20;
	ccd_right.OutputMax = 100; 
    ccd_right.OutputMin = -100;
	ccd_right.error = 0.0;                  //偏差值
	ccd_right.Last_Error = 0.0;             //上一次误差	
	ccd_right.output = 0.0;
	ccd_right.increment = 0.0;              //定义增量
	ccd_right.Limit_increment = 0.0;        //积分限幅
}

void PID_Type_Init(uint8 Type)
{
	if (Type == 1)                           //科目一参数
	{
		set_speed = set_speed_t;
		
		speed.Kp = 28;
		speed.Ki = 1;
		speed.Kd = 0;
		speed.OutputMax = 365;                  //  200    150
	    speed.OutputMin = 0;
	    speed.increment = 0.0;                  //定义增量
        speed.Limit_increment = 100;            //积分限幅
		
		left_Angle.Kp = 1.05;
		left_Angle.Ki = 50;
		left_Angle.Kd = 20;
			
		left_rate.Kp = 2.9;
		left_rate.Ki = 10;
		left_rate.Kd = 20;
		left_rate.OutputMax = 800;
		left_rate.OutputMin = -800;  
			
		right_Angle.Kp = 1.05;
		right_Angle.Ki = 50;
		right_Angle.Kd = 20;
			
		right_rate.Kp = 2.9;
		right_rate.Ki = 10;
		right_rate.Kd = 20;
		right_rate.OutputMax = 800; 
		right_rate.OutputMin = -800;
		
	}
	else if (Type == 2)                      //科目二参数
	{
		set_speed = set_speed_t;

		speed.Kp = 28;
		speed.Ki = 1;
		speed.Kd = 0;
		speed.OutputMax = 250;                  //  200    150
	    speed.OutputMin = 0;
	    speed.increment = 0.0;                  //定义增量
        speed.Limit_increment = 50;             //积分限幅
		
	}
	else if (Type == 3)                      //科目三参数
	{
		set_speed = set_speed_t;
		
		speed.Kp = 28;
		speed.Ki = 1;
		speed.Kd = 0;
		speed.OutputMax = 200;                  //  200    150
	    speed.OutputMin = 0;
	    speed.increment = 0.0;                  //定义增量
        speed.Limit_increment = 50;             //积分限幅
		
		left_Angle.Kp = 1.05;
		left_Angle.Ki = 50;
		left_Angle.Kd = 20;
		
		left_rate.Kp = 3.3;
		left_rate.Ki = 10;
		left_rate.Kd = 20;
		left_rate.OutputMax = 800;
	    left_rate.OutputMin = -800;  
		
		right_Angle.Kp = 1.05;
		right_Angle.Ki = 50;
		right_Angle.Kd = 20;
		
		right_rate.Kp = 3.3;
		right_rate.Ki = 10;
		right_rate.Kd = 20;
		right_rate.OutputMax = 800; 
	    right_rate.OutputMin = -800;
		
		ccd_left.Kp = 3.3;
		ccd_left.Ki = 10;
		ccd_left.Kd = 20;
		ccd_left.OutputMax = 100; 
		ccd_left.OutputMin = -100;
		ccd_left.error = 0.0;                  //偏差值
		ccd_left.Last_Error = 0.0;             //上一次误差	
		ccd_left.output = 0.0;
		ccd_left.increment = 0.0;              //定义增量
		ccd_left.Limit_increment = 0.0;        //积分限幅
		
		ccd_right.Kp = 3.3;
		ccd_right.Ki = 10;
		ccd_right.Kd = 20;
		ccd_right.OutputMax = 100; 
		ccd_right.OutputMin = -100;
		ccd_right.error = 0.0;                  //偏差值
		ccd_right.Last_Error = 0.0;             //上一次误差	
		ccd_right.output = 0.0;
		ccd_right.increment = 0.0;              //定义增量
		ccd_right.Limit_increment = 0.0;        //积分限幅
	}
	
	PID_Parameter[0] = left_Angle.Kp;
	PID_Parameter[1] = left_Angle.Ki;
	PID_Parameter[2] = left_Angle.Kd;
	
	PID_Parameter[3] = right_Angle.Kp;
	PID_Parameter[4] = right_Angle.Ki;
	PID_Parameter[5] = right_Angle.Kd;
}

void PID_Parameter_Set(void)
{
	left_Angle.Kp = PID_Parameter[0];
	left_Angle.Ki = PID_Parameter[1];
	left_Angle.Kd = PID_Parameter[2];
	
	right_Angle.Kp = PID_Parameter[3];
	right_Angle.Ki = PID_Parameter[4];
	right_Angle.Kd = PID_Parameter[5];
}

float PID_pos(PID_Pos* pid,float present, float aim)      //////位置式
{

	pid->error = aim - present;

	pid->increment += pid->error;

    pid->increment = limit_value(pid->increment,pid->Limit_increment,(-pid->Limit_increment));

    pid->Last_Error= pid->error;

	pid->output =  pid->Kp * pid->error + pid->Ki * pid->increment + (pid->error - pid->Last_Error) * pid->Kd;
	
	pid->output = pid->output > pid->OutputMax ? pid->OutputMax : pid->output;      

	pid->output = pid->output < pid->OutputMin ? pid->OutputMin : pid->output; 
	
	return pid->output;
	
}

float Pid_Increse(Pid_Inc *pid,float present, float aim)
{
    pid->Err = aim - present;

    pid->proportion = pid->Kp * (pid->Err - pid->Err_Next);

    pid->integration = pid->Ki * pid->Err;

    pid->differential = pid->Kd * (pid->Err - 2*pid->Err_Next + pid->Err_Last);

    pid->increment = pid->proportion + pid->integration + pid->differential;

    pid->increment = limit_value(pid->increment,pid->Limit_increment,(-pid->Limit_increment));

    pid->Out += pid->increment;
	
	pid->Out = pid->Out > pid->OutputMax ? pid->OutputMax : pid->Out;      

	pid->Out = pid->Out < pid->OutputMin ? pid->OutputMin : pid->Out; 

    pid->Err_Last = pid->Err_Next;

    pid->Err_Next = pid->Err;
	
	return pid->Out;
}

float limit_value(float value,float positive_value,float negative_value)
{
    if(value>positive_value)
    {
        value=positive_value;
    }
    if(value<negative_value)
    {
        value=negative_value;
    }
    return value;
}

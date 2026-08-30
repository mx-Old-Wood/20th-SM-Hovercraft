#include "headfile.h"

float angle_differ;
float  set_ratel,actual_rate;
float  set_rater;
float  set_speed = 0.0, set_speed_t = 6.0;       //科目一: 3.5m/s(过高)  科目二: 1m/s
float  speedout;
uint8 xdata  ccd_flag = 0;           
uint8 xdata  ccd_number = 7;
uint8 xdata  T_number;
float duty_back_left_pre;
float duty_back_right_pre; 


void angle_union(void)
{
  if(angle_differ>180)
  {
    angle_differ = angle_differ - 360;
  }
  else if(angle_differ<-180)                           
  {
    angle_differ = 360 + angle_differ;
  }
	if(Yaw > 360)
	{
		Yaw = Yaw - 360;
	}
}

void fan_control(void)
{
	duty_down = 995;
 //	speedout = (Pid_Increse(&speed,speed_act,set_speed)+625);                //控速
/***********串级*****************/
//	angle_differ = azimuth - direction;
//	angle_differ = azimuth - Yaw;
//	angle_union();                                          //角度规划
//	
//	if (Type == 1)
//	{
//		angle_differ  = angle_differ > 60 ? 60 : angle_differ;      
//		angle_differ  = angle_differ < (-60) ? (-60) : angle_differ; 
//	}
//	if (Type == 2)
//	{
//		angle_differ  = angle_differ > 70 ? 70 : angle_differ;      
//		angle_differ  = angle_differ < (-70) ? (-70) : angle_differ; 
//	}
//	if (Type == 3)
//	{
//		angle_differ  = angle_differ > 70 ? 70 : angle_differ;      
//		angle_differ  = angle_differ < (-70) ? (-70) : angle_differ; 
//	}
//	
//	set_ratel = -PID_pos(&left_Angle,angle_differ,0);
//	set_rater = -PID_pos(&right_Angle,angle_differ,0);  //角度环输出set_rate
	
	actual_rate = imu660ra_gyro_transition(imu660ra_gyro_z);
	
	duty_back_left_pre = PID_pos(&left_rate,actual_rate,set_ratel);
	duty_back_right_pre = -PID_pos(&right_rate,actual_rate,set_rater);
	
	duty_back_left  = (duty_back_left_pre+speedout);
	duty_back_right = (duty_back_right_pre+speedout); //角速度环输出pwm
	
	duty_back_left  = duty_back_left > 970 ? 970 : duty_back_left;      
	duty_back_left  = duty_back_left < 530 ? 530 : duty_back_left; 
	
	duty_back_right  = duty_back_right > 970 ? 970 : duty_back_right;      
	duty_back_right  = duty_back_right < 530 ? 530 : duty_back_right;
	
	speedout  = speedout > 950 ? 950 : speedout;
	speedout  = speedout < 530 ? 530 : speedout;

	pwm_set();
/*************************************/
}

void ccd_fan_control(void)
{
	duty_down = 995;
	speedout = (Pid_Increse(&speed,speed_act,set_speed)+625);                //控速
/********************************************************************************************/
	duty_back_left  = (PID_pos(&ccd_left,Ccd.Error,0)+speedout);
	duty_back_right = (-PID_pos(&ccd_right,Ccd.Error,0)+speedout);           //ccd环输出pwm
	
	duty_back_left  = duty_back_left > 950 ? 950 : duty_back_left;      
	duty_back_left  = duty_back_left < 550 ? 550 : duty_back_left; 
	
	duty_back_right  = duty_back_right > 950 ? 950 : duty_back_right;      
	duty_back_right  = duty_back_right < 550 ? 550 : duty_back_right;

	pwm_set();
}

void mode(uint8 Type)
{
	
/****************************************mode 1*********************************/ 
	if (Type == 1)
	{	//向前直行
		if(array_point[2*number] != 0 && array_point[2*number+1] != 0 && distance < 6.0)   //0.48
		{
			number += 1;
		}
		if(array_point[2*number] == 0 && array_point[2*number+1] == 0)
		{
			go_flag = 0;
		//增加pid参数置0
		}
		if(number == 3)
		{
			left_Angle.Kp = 3.5;
			left_Angle.Ki = 50;
			left_Angle.Kd = 20;
			
			left_rate.Kp = 3.5;
			left_rate.Ki = 10;
			left_rate.Kd = 20;
			left_rate.OutputMax = 800;
			left_rate.OutputMin = -800;  
			
			right_Angle.Kp = 3.5;
			right_Angle.Ki = 50;
			right_Angle.Kd = 20;
			
			right_rate.Kp = 3.5;
			right_rate.Ki = 10;
			right_rate.Kd = 20;
			right_rate.OutputMax = 800; 
			right_rate.OutputMin = -800;
		}
		else if(number == 2 &&(distance > 2.0 && distance < 6.0))
		{
			set_speed = 0.5;        //0.5
			
			left_Angle.Kp = 1.05;
			left_Angle.Ki = 50;
			left_Angle.Kd = 20;
			
			left_rate.Kp = 1.05;
			left_rate.Ki = 10;
			left_rate.Kd = 20;
			left_rate.OutputMax = 800;
			left_rate.OutputMin = -800;  
			
			right_Angle.Kp = 1.05;
			right_Angle.Ki = 50;
			right_Angle.Kd = 20;
			
			right_rate.Kp = 1.05;
			right_rate.Ki = 10;
			right_rate.Kd = 20;
			right_rate.OutputMax = 800; 
			right_rate.OutputMin = -800;
		} 
		else
		{
			left_Angle.Kp = 1.05;
			left_Angle.Ki = 50;
			left_Angle.Kd = 20;
			
			left_rate.Kp = 1.05;
			left_rate.Ki = 10;
			left_rate.Kd = 20;
			left_rate.OutputMax = 800;
			left_rate.OutputMin = -800;  
			
			right_Angle.Kp = 1.05;
			right_Angle.Ki = 50;
			right_Angle.Kd = 20;
			
			right_rate.Kp = 1.05;
			right_rate.Ki = 10;
			right_rate.Kd = 20;
			right_rate.OutputMax = 800; 
			right_rate.OutputMin = -800;
		}
	} 	
/****************************************mode 2*********************************/
	else if (Type == 2)
	{	 
		if(array_point[2*number] != 0 && array_point[2*number+1] != 0 && distance < 1.8)   //0.48
		{
			number += 1;
			if (number == 10)
			{
				array_point[2 * number] = array_point[2 * 5];
				array_point[2 * number + 1] = array_point[2 * 5 + 1];
			}
			else if (number == 11)
			{
				array_point[2 * number] = array_point[2 * 4];
				array_point[2 * number + 1] = array_point[2 * 4 + 1];
			}
			else if (number == 12)
			{
				array_point[2 * number] = array_point[2 * 3];
				array_point[2 * number + 1] = array_point[2 * 3 + 1];
			}
		}
		else if(array_point[2*number] == 0 && array_point[2*number+1] == 0)
		{
			go_flag = 0;
		}
		if((number == 2 && distance > 4.0)||(number == 9 && distance > 4.0))
		{
			set_speed = set_speed_t + 3;
			
			left_Angle.Kp = 1.05;
			left_Angle.Ki = 50;
			left_Angle.Kd = 20;
			
			left_rate.Kp = 1.05;
			left_rate.Ki = 10;
			left_rate.Kd = 20;
			left_rate.OutputMax = 800;
			left_rate.OutputMin = -800;  
			
			right_Angle.Kp = 1.05;
			right_Angle.Ki = 50;
			right_Angle.Kd = 20;
			
			right_rate.Kp = 1.05;
			right_rate.Ki = 10;
			right_rate.Kd = 20;
			right_rate.OutputMax = 800; 
			right_rate.OutputMin = -800;

		} 
		else if((number == 7) || (number == 14))
		{
			
			set_speed = set_speed_t + 9;
			
			left_Angle.Kp = 1.05;
			left_Angle.Ki = 50;
			left_Angle.Kd = 20;
			
			left_rate.Kp = 1.05;
			left_rate.Ki = 10;
			left_rate.Kd = 20;
			left_rate.OutputMax = 800;
			left_rate.OutputMin = -800;  
			
			right_Angle.Kp = 1.05;
			right_Angle.Ki = 50;
			right_Angle.Kd = 20;
			
			right_rate.Kp = 1.05;
			right_rate.Ki = 10;
			right_rate.Kd = 20;
			right_rate.OutputMax = 800; 
			right_rate.OutputMin = -800;

		} 
//		else if((number == 6 && distance > 11.0) || (number == 12 && distance > 11.0))
//		{
//			set_speed = set_speed_t - 1 ;
//			
//			left_Angle.Kp = 3.5;
//			left_Angle.Ki = 50;
//			left_Angle.Kd = 20;
//			
//			left_rate.Kp = 2.5;
//			left_rate.Ki = 10;
//			left_rate.Kd = 20;
//			left_rate.OutputMax = 800;
//			left_rate.OutputMin = -800;  
//			
//			right_Angle.Kp = 3.5;
//			right_Angle.Ki = 50;
//			right_Angle.Kd = 20;
//			
//			right_rate.Kp = 2.5;
//			right_rate.Ki = 10;
//			right_rate.Kd = 20;
//			right_rate.OutputMax = 800; 
//			right_rate.OutputMin = -800;
//		} 
		else if(number == 2 &&(distance > 2.0 && distance < 4.0) || (number == 8 &&(distance > 2.0 && distance < 4.0)))
		{
			set_speed = 0.2;        //0.5
			
			left_Angle.Kp = 1.05;
			left_Angle.Ki = 50;
			left_Angle.Kd = 20;
			
			left_rate.Kp = 1.05;
			left_rate.Ki = 10;
			left_rate.Kd = 20;
			left_rate.OutputMax = 800;
			left_rate.OutputMin = -800;  
			
			right_Angle.Kp = 1.05;
			right_Angle.Ki = 50;
			right_Angle.Kd = 20;
			
			right_rate.Kp = 1.05;
			right_rate.Ki = 10;
			right_rate.Kd = 20;
			right_rate.OutputMax = 800; 
			right_rate.OutputMin = -800;
		} 
		else
		{
			set_speed = 2.5;
			
			left_Angle.Kp = 3.5;
			left_Angle.Ki = 50;
			left_Angle.Kd = 20;
			
			left_rate.Kp = 2.0;    //2.0
			left_rate.Ki = 10;
			left_rate.Kd = 50;    //20
			left_rate.OutputMax = 800;
			left_rate.OutputMin = -800;  
			
			right_Angle.Kp = 3.5;
			right_Angle.Ki = 50;
			right_Angle.Kd = 20;
			
			right_rate.Kp = 2.0;  //2.0
			right_rate.Ki = 10;
			right_rate.Kd = 50;   //20
			right_rate.OutputMax = 800;   
			right_rate.OutputMin = -800;
		} 
	}
/****************************************mode 3*********************************/
	else if (Type == 3)
	{
		if(array_point[2*number] != 0 && array_point[2*number+1] != 0 && distance < 1.8)   //0.48  
		{
			number += 1;
		}
		if(array_point[2*number] == 0 && array_point[2*number+1] == 0)
		{
			go_flag = 0;
//			DisableGlobalIRQ();
//			fan_stop();
			//增加pid参数置0
		}
		if(number == ccd_number - 1 || number == ccd_number - 2)
		{
			set_speed = 1.9;
			left_Angle.Kp = 1.05;
			left_Angle.Ki = 50;
			left_Angle.Kd = 20;
			
			left_rate.Kp = 2.7;
			left_rate.Ki = 10;
			left_rate.Kd = 20;
			left_rate.OutputMax = 800;
			left_rate.OutputMin = -800;  
			
			right_Angle.Kp = 1.05;
			right_Angle.Ki = 50;
			right_Angle.Kd = 20;
			
			right_rate.Kp = 2.7;
			right_rate.Ki = 10;
			right_rate.Kd = 20;
			right_rate.OutputMax = 800; 
			right_rate.OutputMin = -800;
			
		}
		if(number == ccd_number)
		{
			ccd_flag = 1;
			set_speed = 1.9;
			
		}
		if(number == ccd_number + 1)
		{
			ccd_flag = 0;
			set_speed = set_speed_t;
		}
		if(number == 7)
		{
			set_speed = 2.7;
			left_Angle.Kp = 3.5;
			left_Angle.Ki = 50;
			left_Angle.Kd = 20;
			
			left_rate.Kp = 2.0;
			left_rate.Ki = 10;
			left_rate.Kd = 20;
			left_rate.OutputMax = 800;
			left_rate.OutputMin = -800;  
			
			right_Angle.Kp = 3.5;
			right_Angle.Ki = 50;
			right_Angle.Kd = 20;
			
			right_rate.Kp = 2.0;
			right_rate.Ki = 10;
			right_rate.Kd = 20;
			right_rate.OutputMax = 800; 
			right_rate.OutputMin = -800;
		}
		else
		{
			left_Angle.Kp = 1.05;
			left_Angle.Ki = 50;
			left_Angle.Kd = 20;
			
			left_rate.Kp = 1.5;
			left_rate.Ki = 10;
			left_rate.Kd = 20;
			left_rate.OutputMax = 800;
			left_rate.OutputMin = -800;  
			
			right_Angle.Kp = 1.05;
			right_Angle.Ki = 50;
			right_Angle.Kd = 20;
			
			right_rate.Kp = 1.8;
			right_rate.Ki = 10;
			right_rate.Kd = 20;
			right_rate.OutputMax = 800; 
			right_rate.OutputMin = -800;
			
		}
	}
}

#include "headfile.h"

//PWM1 PWM2 PWM3 PWM4
//6.0  6.2  6.4  6.6

//测试代码
//uint16 duty_down = 800;
//uint16 duty_back_left = 800;
//uint16 duty_back_right = 800;  



uint16 duty_down = 500;
uint16 duty_back_left = 500;
uint16 duty_back_right = 500;   //设置初始转速为0


void pwm_all(void)
{
	pwm_init(PWMA_CH1P_P60,50,duty_down);
	pwm_init(PWMA_CH2P_P62,50,duty_down);
	pwm_init(PWMA_CH3P_P64,50,duty_back_left);
	pwm_init(PWMA_CH4P_P66,50,duty_back_right);
}	


void pwm_set(void)
{
	pwm_duty(PWMA_CH1P_P60,duty_down);
	pwm_duty(PWMA_CH2P_P62,duty_down);
	pwm_duty(PWMA_CH3P_P64,duty_back_left);
	pwm_duty(PWMA_CH4P_P66,duty_back_right);	
}



void fan_stop(void)                //停止转速
{
	duty_down = 500;
	duty_back_left = 500;
	duty_back_right = 500;
	pwm_set();
}

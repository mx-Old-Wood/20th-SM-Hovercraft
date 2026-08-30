#ifndef __MOTOR_H_
#define __MOTOR_H_

/*********************************函数声明***********************************/
void pwm_all(void);
void fan_stop(void);
void pwm_set(void);

/*******************************全局变量声明*********************************/
extern uint16 duty_down;
extern uint16 duty_back_left;
extern uint16 duty_back_right;   //设置初始转速为0
extern uint8 closed_loop_flag_1;
extern uint8 closed_loop_flag_2;
extern uint8 closed_loop_flag_3; 

#endif

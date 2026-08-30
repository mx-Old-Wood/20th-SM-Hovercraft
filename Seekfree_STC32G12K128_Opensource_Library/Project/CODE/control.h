#ifndef __CONTROL_H_
#define __CONTROL_H_

/*********************************函数声明***********************************/
void fan_control(void);
void ccd_fan_control(void);
void mode(uint8 Type);
void angle_union(void);


/*******************************全局变量声明*********************************/
extern float  angle_differ;
extern float  set_ratel,actual_rate;
extern float  set_speed, set_speed_t;
extern float  speedout;
extern uint8  xdata  ccd_flag;
extern uint8  xdata  ccd_number;
extern float  duty_back_left_pre;
extern float  duty_back_right_pre; 
extern float  set_rater;

#endif

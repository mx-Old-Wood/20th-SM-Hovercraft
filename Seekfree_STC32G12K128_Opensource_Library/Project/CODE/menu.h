#ifndef __MENU_H_
#define __MENU_H_

struct THE_KEY
{
	char status;      //开关当前检测状态
	char last_status; //开关上一次检测状态
};

typedef enum         //开关标志位
{
	enter_flag  = 1, //确认/增加标志位
	reduce_flag = 2, //减少标志位
	down_flag   = 3, //向下标志位
	up_flag     = 4, //向上标志位
};

#define Debug   0
#define Racing  1

#define North   0
#define South   1
#define East    2
#define West    3

/*********************************函数声明***********************************/
void UI(uint8 Type);
void menu(void);
void Cursor(void);	
void Aim_control(uint8 upper_limit, int8 lower_limit, uint8 key_flag);


/*********************************IO口定义***********************************/
//P3^6 回车  P4^6向下  P4^5向右  P3^7向左
//P3^4 中  P3^5 右边 P5^0 P5^1 RST 向下

sbit enter  = P7^0;
sbit reduce = P7^1;
sbit down   = P7^2;
sbit up     = P7^3;

//sbit enter  = P3^4;
//sbit reduce = P5^1;
//sbit down   = P3^5;
//sbit up     = P5^0;	

/*******************************全局变量声明*********************************/
extern int8 xdata UI_state;
extern uint8 go_flag ;
extern uint8 Type;
extern double Towards;
extern uint8 Cone_Nunber;

#endif

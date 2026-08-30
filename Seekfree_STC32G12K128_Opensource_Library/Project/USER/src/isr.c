///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2020,逐飞科技
// * All rights reserved.
// * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
// *
// * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
// * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
// *
// * @file       		isr
// * @company	   		成都逐飞科技有限公司
// * @author     		逐飞科技(QQ790875685)
// * @version    		查看doc内version文件 版本说明
// * @Software 			MDK FOR C251 V5.60
// * @Target core		STC32G12K128
// * @Taobao   			https://seekfree.taobao.com/
// * @date       		2020-4-14
// ********************************************************************************************************************/
#include "headfile.h"

double head_distance;
//float angle_front_differ;
float xdata pulse;
float speed_act;
//UART1中断
void UART1_Isr() interrupt 4
{
    uint8 res;
	static uint8 dwon_count;
    if(UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if(UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
        res = SBUF;
        //程序自动下载
        if(res == 0x7F)
        {
            if(dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
    }
}

//UART2中断
void UART2_Isr() interrupt 8
{
    if(UART2_GET_TX_FLAG)
	{
        UART2_CLEAR_TX_FLAG;
		busy[2] = 0;
	}
    if(UART2_GET_RX_FLAG)
	{
        UART2_CLEAR_RX_FLAG;
		//接收数据寄存器为：S2BUF
		gps_uart_callback(S2BUF);
	}
}


//UART3中断
void UART3_Isr() interrupt 17
{
    if(UART3_GET_TX_FLAG)
	{
        UART3_CLEAR_TX_FLAG;
		busy[3] = 0;
	}
    if(UART3_GET_RX_FLAG)
	{
        UART3_CLEAR_RX_FLAG;
		//接收数据寄存器为：S3BUF

	}
}


//UART4中断
void UART4_Isr() interrupt 18
{
    if(UART4_GET_TX_FLAG)
	{
        UART4_CLEAR_TX_FLAG;
		busy[4] = 0;
	}
    if(UART4_GET_RX_FLAG)
	{
        UART4_CLEAR_RX_FLAG;

		//接收数据寄存器为：S4BUF;
		if(wireless_type == WIRELESS_SI24R1)
        {
            wireless_uart_callback(S4BUF);           //无线转串口回调函数
        }
	}
}

#define LED P52
void INT0_Isr() interrupt 0
{
	LED = 0;	//点亮LED
}
void INT1_Isr() interrupt 2
{

}
void INT2_Isr() interrupt 10
{
	INT2_CLEAR_FLAG;  //清除中断标志
}
void INT3_Isr() interrupt 11
{
	INT3_CLEAR_FLAG;  //清除中断标志
}

void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;  //清除中断标志
}

void TM0_Isr() interrupt 1
{
    GPS_data_t();
	if(gyro_Offset_flag == 1)
	{
		IMU_YAW_integral();
	}
    if(go_flag == 1)
	{
		if(differ_flag == 0)
		{
			GPS_differ();
			differ_flag = 1;
		}
		if(ccd_flag == 0 && differ_flag == 1)
		{
			fan_control();
		}
		else if(ccd_flag == 1&& differ_flag == 1)
		{
			ccd_fan_control();
		}
	}
	else if (go_flag == 0)
	{
		fan_stop();
	}
	
}
void TM1_Isr() interrupt 3
{
	pulse = ctimer_count_read(CTIM3_P04);
	ctimer_count_clean(CTIM3_P04);
	
    if(0 == P53)    
	{
		pulse = -pulse;
    }	
	speed_act = 200 * PI * 0.035 * pulse / 1024;
	speedout = (Pid_Increse(&speed,speed_act,set_speed)+625);  
	
	angle_differ = azimuth - Yaw;
	angle_union();                                          //角度规划
	
	if (Type == 1)
	{
		angle_differ  = angle_differ > 60 ? 60 : angle_differ;      
		angle_differ  = angle_differ < (-60) ? (-60) : angle_differ; 
	}
	if (Type == 2)
	{
		angle_differ  = angle_differ > 70 ? 70 : angle_differ;      
		angle_differ  = angle_differ < (-70) ? (-70) : angle_differ; 
	}
	if (Type == 3)
	{
		angle_differ  = angle_differ > 70 ? 70 : angle_differ;      
		angle_differ  = angle_differ < (-70) ? (-70) : angle_differ; 
	}
	
	set_ratel = -PID_pos(&left_Angle,angle_differ,0);
	set_rater = -PID_pos(&right_Angle,angle_differ,0);  //角度环输出set_rate
	
}
void TM2_Isr() interrupt 12
{
	TIM2_CLEAR_FLAG;  //清除中断标志
//	if(go_flag == 1)
//	{
//		if(differ_flag == 0)
//		{
//			GPS_differ();
//			differ_flag = 1;
//		}
//		if(ccd_flag == 0 && differ_flag == 1)
//		{
//			fan_control();
//		}
//		else if(ccd_flag == 1&& differ_flag == 1)
//		{
//			ccd_fan_control();
//		}
//	}
//	else if (go_flag == 0)
//	{
//		fan_stop();
//	}

}
void TM3_Isr() interrupt 19
{
	TIM3_CLEAR_FLAG; //清除中断标志
	
}

void TM4_Isr() interrupt 20
{
	TIM4_CLEAR_FLAG; //清除中断标志
	ccd_collect();	 //CCD采集数据

}

//void  INT0_Isr()  interrupt 0;
//void  TM0_Isr()   interrupt 1;
//void  INT1_Isr()  interrupt 2;
//void  TM1_Isr()   interrupt 3;
//void  UART1_Isr() interrupt 4;
//void  ADC_Isr()   interrupt 5;
//void  LVD_Isr()   interrupt 6;
//void  PCA_Isr()   interrupt 7;
//void  UART2_Isr() interrupt 8;
//void  SPI_Isr()   interrupt 9;
//void  INT2_Isr()  interrupt 10;
//void  INT3_Isr()  interrupt 11;
//void  TM2_Isr()   interrupt 12;
//void  INT4_Isr()  interrupt 16;
//void  UART3_Isr() interrupt 17;
//void  UART4_Isr() interrupt 18;
//void  TM3_Isr()   interrupt 19;
//void  TM4_Isr()   interrupt 20;
//void  CMP_Isr()   interrupt 21;
//void  I2C_Isr()   interrupt 24;
//void  USB_Isr()   interrupt 25;
//void  PWM1_Isr()  interrupt 26;
//void  PWM2_Isr()  interrupt 27;
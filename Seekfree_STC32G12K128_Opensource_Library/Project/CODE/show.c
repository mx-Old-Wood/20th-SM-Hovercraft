#include "headfile.h"

int8 xdata number = 1;
//int8 xdata number = 3;

void test_show()
{
//	lcd_showstr(20, 0, "angle");
//	lcd_showfloat(60, 0, azimuth, 3, 6);
//	lcd_showfloat(60, 1, Get_gps_angle(), 3, 6);
	lcd_showstr(20, 2, "dis");
	lcd_showfloat(60, 2, distance, 3, 6);
	lcd_showfloat(60, 3, Get_gps_distance()/50, 3, 6);
	
	
//	lcd_showfloat(60, 3, diff_x, 3, 6);
//	lcd_showfloat(60, 4, diff_y, 3, 6);
//	lcd_showstr(20, 2, "la_deg");
//	lcd_showuint16(60, 2, gps_tau1201.latitude_degree);
//	lcd_showuint16(60, 3, latitude_degree);
//	lcd_showstr(20, 4, "la_cen");
//	lcd_showuint16(60, 4, gps_tau1201.latitude_cent);
//	lcd_showstr(20, 5, "lo_deg:");
//	lcd_showuint16(60, 5, gps_tau1201.longitude_degree);
//	lcd_showuint16(60, 6, longitude_degree);
//	lcd_showstr(20, 7, "lo_cen");
//	lcd_showuint16(60, 7, gps_tau1201.longitude_cent);
//	lcd_showstr(20, 2, "la_min");
//	lcd_showfloat(60, 2, gps_tau1201.latitude_minute, 3, 6);
//	lcd_showfloat(60, 3, latitude_minute, 3, 6);
//	lcd_showstr(20, 5, "lo_min");
//	lcd_showfloat(60, 5, gps_tau1201.longitude_minute, 3, 6);
}



void all_init(void)
{
//	NVIC_SetPriority(TIMER1_IRQn, 3);
	NVIC_SetPriority(UART2_IRQn, 3);
	wireless_uart_init();
	imu660ra_init();
	IMU_offset_init();
	lcd_init();
	gps_init();
	iap_init();
	pwm_all();
	PID_init();
	Ccd_Type_Init(Normal);
	ctimer_count_init(CTIM3_P04);
	EnableGlobalIRQ();
//	pit_timer_ms(TIM_2, 3);
	pit_timer_ms(TIM_0, 5);
	pit_timer_ms(TIM_1, 10);
}

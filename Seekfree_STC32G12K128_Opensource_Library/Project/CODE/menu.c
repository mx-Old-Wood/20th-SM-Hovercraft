#include "headfile.h"

struct THE_KEY Keys[4]; 

int8 xdata arrow = 1;                      //初始光标位置
int8 xdata arrow_t = 1;
int8 xdata CCD_state = 0;                  //CCD图象显示标志位
uint8 xdata menu_flage = 0;                //菜单标志位
uint8 xdata show_flag = 0;                 //单次显示标志位
int8 xdata UI_state = 0;                   //UI显示标志位
int8 xdata Towards_Number = 0;
uint8 xdata T1[4] = "OFF", T2[6] = "North", T3[7] = "Debug ";
uint8 Cone_Nunber = 4;                     //锥桶数量
double Towards = 0;
uint8 go_flag = 0;                         //发车标志位
uint8 Type = 1;                            //模式

/**
  *  @brief  在相应的菜单中显示光标
  *  @param  无
  *  @retval 无
  *  @memo   在进入Car Go!!!时不显示光标
  **/

void Cursor(void)		                        //光标
{  
	if (menu_flage == 0)
	{
		lcd_showstr(2, arrow, "->");     //在参数前面显示->
	}
	if (menu_flage == 1)
	{
		lcd_showstr(2, arrow, "->");
	}
	if (menu_flage == 2)
	{
		lcd_showstr(2, arrow, "->");
	}
	if (menu_flage == 3)
	{
		lcd_showstr(2, arrow, "->");
	}
	if (menu_flage == 4)
	{
		lcd_showstr(2, arrow, "->");
	}
	if (menu_flage == 5)
	{
		lcd_showstr(2, arrow, "->");
	}
	
}
	
/**
  *  @brief  获取按键的状态
  *  @param  无
  *  @retval 0
  *  @memo   无
  **/

uint8 key_status_get(void)                        //获取按键的状态
{
	Keys[0].last_status = Keys[0].status;
	Keys[1].last_status = Keys[1].status;
	Keys[2].last_status = Keys[2].status;
	Keys[3].last_status = Keys[3].status;
	
	Keys[0].status = enter;              //确认/增加
	Keys[1].status = reduce;             //减少
	Keys[2].status = down;               //下移
	Keys[3].status = up;                 //上移
	
	if (Keys[0].status && !Keys[0].last_status)
		return (enter_flag);
	if (Keys[1].status && !Keys[1].last_status)
		return (reduce_flag);
	if (Keys[2].status && !Keys[2].last_status)
		return (down_flag);
	if (Keys[3].status && !Keys[3].last_status)
		return (up_flag);
	return 0;
}

/**
  *  @brief  控制光标的移动
  *  @param  光标上限值、光标下限值、按键返回值
  *  @retval 无
  *  @memo   该函数只能控制光标的移动
  **/

void Aim_control(uint8 upper_limit, int8 lower_limit, uint8 key_flag)
{
	if (key_flag == down_flag)                 //向下 down
	{
		lcd_showstr(2, arrow, "  ");    //需要将当前行的光标隐藏，不然当arrow更新之后，出现两行显示光标
		arrow += 1;
		if (arrow > upper_limit)
			arrow = lower_limit;
	}
	else if (key_flag == up_flag)               //向上 up
	{
		lcd_showstr(2, arrow, "  ");
		arrow -= 1;
		if (arrow < lower_limit)
			arrow = upper_limit;
	}
}

void Clear_Display(uint8 line, uint8 start, uint8 end)
{
	uint8 xdata i;
	for (i = start; i <= end; i++)
		lcd_showstr(line, i, "                    ");
}

void data_show(void)
{
	if (show_flag == 0)
	{
		lcd_showstr(20, 0, "Go");
		lcd_showstr(75, 0, "A:");
		lcd_showstr(20, 1, "PL:");
		lcd_showstr(90, 1, "PR:");
		lcd_showstr(110, 2, "F:");
		lcd_showstr(20, 3, "LA_D:");
		lcd_showstr(20, 4, "LO_D:");
		lcd_showstr(20, 7, "PN:");
		lcd_showstr(75, 7, "D:");
		show_flag = 1;
	}
	lcd_showfloat(90, 0, azimuth, 3, 3);
	lcd_showuint16(35, 1, duty_back_left);
	lcd_showuint16(105, 1, duty_back_right);
	lcd_showuint8(120, 2, go_flag);
	lcd_showfloat(40, 2, Yaw, 3, 3);
	lcd_showfloat(80, 3, duty_back_left_pre, 3, 6);
	lcd_showfloat(80, 4, duty_back_right_pre, 3, 6);
	lcd_showfloat(80, 5, speedout, 3, 6);
    lcd_showuint8(35, 7, number);
    lcd_showfloat(90, 7, distance, 3, 3);
	Show_Ccd_Image(CCD_state);
}

/**
  *  @brief  显示GPS存储的点
  *  @param  无
  *  @retval 无
  *  @memo   设定存储点数为20个
  **/

void Show_The_GPS_Points(int8 Page)
{
	if (show_flag == 0)
	{
		lcd_showstr(20, 0, "Back");
		lcd_showstr(20, 1, "Data_Gather/Clean");
		lcd_showstr(20, 2, "Data_Read");
		lcd_showstr(20, 3, "P:");
		lcd_showstr(20, 4, "Dot");
		lcd_showstr(50, 4, "Lat:");
		lcd_showstr(50, 5, "Lot:");
		lcd_showstr(20, 6, "Dot");
		lcd_showstr(50, 6, "Lat:");
		lcd_showstr(50, 7, "Lot:");
		show_flag = 1;
	}
	lcd_showuint8(90, 2, Type);
	lcd_showuint8(30, 3, Page);
	lcd_showuint8(10, 5, (uint8)(2 * Page - 1));
	lcd_showfloat(80, 4, array_point[4 * Page - 2], 3, 6);
	lcd_showfloat(80, 5, array_point[4 * Page - 1], 3, 6);
	lcd_showuint8(10, 7, (uint8)(2 * Page));
	lcd_showfloat(80, 6, array_point[4 * Page], 3, 6);
	lcd_showfloat(80, 7, array_point[4 * Page + 1], 3, 6);
}

void Show_The_PID_Parameter(void)
{
	if (show_flag == 0)
	{
		lcd_showstr(20, 0, "Back");
		lcd_showstr(80, 0, "Angle");
		lcd_showstr(20, 1, "L_P:");
		lcd_showstr(20, 2, "L_I:");
		lcd_showstr(20, 3, "L_D:");
		lcd_showstr(20, 4, "R_P:");
		lcd_showstr(20, 5, "R_I:");
		lcd_showstr(20, 6, "R_D:");
		show_flag = 1;
	}
		lcd_showfloat(50, 1, PID_Parameter[0], 2, 2);
		lcd_showfloat(50, 2, PID_Parameter[1], 2, 2);
		lcd_showfloat(50, 3, PID_Parameter[2], 2, 2);
		lcd_showfloat(50, 4, PID_Parameter[3], 2, 2);
		lcd_showfloat(50, 5, PID_Parameter[4], 2, 2);
		lcd_showfloat(50, 6, PID_Parameter[5], 2, 2);
} 

/**
  *  @brief  主菜单,选择相应子菜单
  *  @param  无
  *  @retval 无
  *  @memo   在菜单编写的过程中需注意case结尾后的break,否则不一定能达到相应要求
  **/

void menu0(void)                               //一级菜单
{
	uint8 xdata key_flag = key_status_get();
	arrow_t = arrow;
	if (show_flag == 0)
	{
		lcd_showstr(20, 0, "Car Go!!!");
		lcd_showstr(20, 1, "Mode_Switch");
		lcd_showstr(20, 2, "Angle_Rings_Para");
		lcd_showstr(20, 3, "GPS_Data");
		lcd_showstr(20, 4, "Element");
		show_flag = 1;
	}
	Aim_control(4, 0, key_flag);
	if (key_flag == enter_flag)              //确定 enter
	{
		switch(arrow)
		{
			case 0:
				lcd_clear(WHITE);
				menu_flage = 1;
				show_flag = 0;
				break;
			case 1:
				lcd_clear(WHITE);
				menu_flage = 2;
				show_flag = 0;
				break;
			case 2:
				lcd_clear(WHITE);
				menu_flage = 3;
				show_flag = 0;
				break;  //要先清屏再跳转，不然会出现页面重叠的情况
			case 3:
				lcd_clear(WHITE);
				menu_flage = 4;
				show_flag = 0;
				break;
			case 4:
				lcd_clear(WHITE);
				menu_flage = 5;
				show_flag = 0;
				break;
			case 5:
				lcd_clear(WHITE);
				menu_flage = 6;
				show_flag = 0;
				break;
		}
		arrow = 1;
	}
}

/**
  *  @brief  一级子菜单,启动界面
  *  @param  无
  *  @retval 无
  *  @memo   在go处按下enter进行发车
  **/

void menu1_0(void)
{
	uint8 xdata key_flag = key_status_get();
	data_show();
//	test_show();
	Aim_control(7, 0, key_flag);
	if(key_flag == enter_flag)
	{
		switch(arrow)
		{
			case 0:
				go_flag = 1;
				break;
			case 1:         //测试用
				go_flag = 0;
				break;
			default:
				break;
		}
	}
	else if(key_flag == reduce_flag && go_flag == 0)
	{
		switch(arrow)
		{
			case 0:
				lcd_clear(WHITE);
				menu_flage = 0;
				arrow = arrow_t;
				show_flag = 0;
				break;
			default:
				break;
		}
	}
	if (go_flag == 1)
		mode(Type);	
	else if (go_flag == 0)
		fan_stop();
}

/**
  *  @brief  一级子菜单,模式切换
  *  @param  无
  *  @retval 无
  *  @memo   在Current_Mode处按下enter或reduce改变其数值
  **/

void menu1_1(void)
{
	uint8 xdata key_flag = key_status_get();
	if (show_flag == 0)
	{
		lcd_showstr(20, 0, "Back");
		lcd_showstr(20, 1, "Current_Mode:");
		lcd_showstr(20, 2, "Set_Speed:");
		lcd_showstr(20, 3, "Ccd_Image:");
		lcd_showstr(20, 4, "Towards:");
		lcd_showstr(20, 5, "UI_Show:");
		show_flag = 1;
	}
	lcd_showuint8(120, 1, Type);
	lcd_showfloat(120, 2, set_speed, 2, 1);
	lcd_showstr(130, 3, T1);
	lcd_showstr(110, 4, T2);
	lcd_showstr(110, 5, T3);
	Aim_control(5, 0, key_flag);
	if(key_flag == enter_flag)              //增加 enter
	{
		switch(arrow)
		{
			case 0:
				lcd_clear(WHITE);
				menu_flage = 0;
				arrow = arrow_t;
				show_flag = 0;
				break;
			case 1:
				Type++;
				if (Type > 3)
					Type = 1;
				if (Type == 1)
					set_speed_t = 6.0;
				else if (Type == 2)
					set_speed_t = 3.3;
				else if (Type == 3)   
					set_speed_t = 5.0;
				break;
			case 2:
				set_speed_t += 0.1;
				break;
			case 3:
				CCD_state++;
				if (CCD_state > 1)
					CCD_state = 0;
				if (CCD_state == 0)
					strcpy(T1, "OFF");
				else if (CCD_state == 1)
					strcpy(T1, "ON ");
				break;
			case 4:
				Towards_Number++;
				if (Towards_Number > 3)
					Towards_Number = 0;
				if (Towards_Number == North)
				{
					Towards = 0;
					strcpy(T2, "North");
				}
				else if (Towards_Number == South)
				{
					Towards = -180;
					strcpy(T2, "South");
				}
				else if (Towards_Number == East)
				{
					Towards = -90;
					strcpy(T2, "East ");
				}
				else if (Towards_Number == West)
				{
					Towards = 90;
					strcpy(T2, "West ");
				}
				break;
			case 5:
				UI_state++;
				if (UI_state > 1)
					UI_state = 0;
				if (UI_state == 0)
					strcpy(T3, "Debug ");
				else if (UI_state == 1)
					strcpy(T3, "Racing");
				break;
			default:
				break;
		}
	}
	else if (key_flag == reduce_flag)       //减少 reduce
	{
		switch(arrow)
		{
			case 1:
				Type--;
				if (Type < 1) 
					Type = 3;
				if (Type == 1)
					set_speed_t = 6.0;
				else if (Type == 2)
					set_speed_t = 3.3;
				else if (Type == 3)
					set_speed_t = 5.0;
				break;
			case 2:
				set_speed_t -= 0.1;
			    break;
			case 3:
				CCD_state--;
				if (CCD_state < 0)
					CCD_state = 1;
				if (CCD_state == 0)
					strcpy(T1, "OFF");
				else if (CCD_state == 1)
					strcpy(T1, "ON ");
				break;
			case 4:
				Towards_Number--;
				if (Towards_Number < 0)
					Towards_Number = 3;
				if (Towards_Number == North)
				{
					Towards = 0;
					strcpy(T2, "North");
				}
				else if (Towards_Number == South)
				{
					Towards = -180;
					strcpy(T2, "South");
				}
				else if (Towards_Number == East)
				{
					Towards = -90;
					strcpy(T2, "East ");
				}
				else if (Towards_Number == West)
				{
					Towards = 90;
					strcpy(T2, "West ");
				}
				break;
			case 5:
					UI_state--;
				if (UI_state < 0)
					UI_state = 1;
				if (UI_state == 0)
					strcpy(T3, "Debug ");
				else if (UI_state == 1)
					strcpy(T3, "Racing");
				break;
			default:
				break;
		}
	}
	PID_Type_Init(Type);
}

/**
  *  @brief  一级子菜单,角度环PID参数
  *  @param  无
  *  @retval 无
  *  @memo   在对应参量处按下enter或reduce改变其数值,在back处按下enter返回主菜单
  **/

void menu1_2(void)                              //二级菜单:角度环PID参数1-1
{
	uint8 xdata key_flag = key_status_get();
	Show_The_PID_Parameter();
	Aim_control(6, 0, key_flag);
	if(key_flag == enter_flag)              //增加 enter
	{
		switch(arrow)
		{
			case 0:
				lcd_clear(WHITE);
				menu_flage = 0;
				arrow = arrow_t;
				show_flag = 0;
				break;
			case 1:
				PID_Parameter[0] += 0.1;
				break;
			case 2:
				PID_Parameter[1] += 0.05;
				break;
			case 3:
				PID_Parameter[2] += 0.01;
				break;
			case 4:
				PID_Parameter[3] += 0.1;
				break;
			case 5:
				PID_Parameter[4] += 0.05;
				break;
			case 6:
				PID_Parameter[5] += 0.01;
				break;
			default:
				break;
		}
	}
	else if (key_flag == reduce_flag)       //减少 reduce
	{
		switch(arrow)
		{
			case 1:
				PID_Parameter[0] -= 0.1;
				break;
			case 2:
				PID_Parameter[1] -= 0.05;
				break;
			case 3:
				PID_Parameter[2] -= 0.01;
				break;
			case 4:
				PID_Parameter[3] -= 0.1;
				break;
			case 5:
				PID_Parameter[4] -= 0.05;
				break;
			case 6:
				PID_Parameter[5] -= 0.01;
				break;
			default:
				break;
		}
	}
	PID_Parameter_Set();
}

/**
  *  @brief  一级子菜单,GPS数据记录以及实时GPS参数显示
  *  @param  无
  *  @retval 无
  *  @memo   在Data_Gather/Clean按下enter记录数据,按下reduce清除数据,在P按下enter向下翻页,按下reduce向上翻页
  **/

void menu1_3(void)                              //二级菜单:GPS数据记录
{
	static int8 xdata GPS_Page = 1, Page_flag = 0;
	uint8 xdata key_flag = key_status_get();
	Show_The_GPS_Points(GPS_Page);
	Aim_control(3, 0, key_flag);
	if(key_flag == enter_flag)              //确定 enter
	{
		switch(arrow)
		{
			case 0:
				lcd_clear(WHITE);
				menu_flage = 0;
				arrow = arrow_t;
				show_flag = 0;
				break;
			case 1:
//				GPS_record(write);
				GPS_record_t(write, Type);
				Page_flag++;
				if (array_point[2] == 0 && array_point[3] == 0)
				{
					Page_flag = 0;
					GPS_Page = 1;
				}
				else if (Page_flag > 2 && GPS_Page < 10)
				{
					Page_flag = 1;
					GPS_Page++;
				}
				break;
			case 2:
//				GPS_record(read);
				GPS_record_t(read, Type);
				break;
			case 3:
				GPS_Page++;
				if (GPS_Page > 10)
					GPS_Page = 1;
				break;
			default:
				break;
		}
	}
	else if (key_flag == reduce_flag)
	{
		switch(arrow)
		{
			case 1:
//				GPS_record(clean);
				GPS_record_t(clean, Type);
				Page_flag = 0;
				GPS_Page = 1;
				break;
			case 2:
				Type++;
				if (Type > 3)
					Type = 1;
				break;
			case 3:
				GPS_Page--;
				if (GPS_Page < 1)
					GPS_Page = 10;
				break;
			default:
				break;
		}
	}
}

/**
  *  @brief  一级子菜单,元素设置
  *  @param  无
  *  @retval 无
  *  @memo   无
  **/

void menu1_4(void)
{
	uint8 xdata key_flag = key_status_get();
	if (show_flag == 0)
	{
		lcd_showstr(20, 0, "Back");
		lcd_showstr(20, 1, "Cone_Barrel:");
		lcd_showstr(20, 2, "Ccd_Number:");
		show_flag = 1;
	}
	lcd_showuint8(120, 1, Cone_Nunber);
	lcd_showuint8(120, 2, ccd_number);
	Aim_control(2, 0, key_flag);
	if(key_flag == enter_flag)              //增加 enter
	{
		switch(arrow)
		{
			case 0:
				lcd_clear(WHITE);
				menu_flage = 0;
				arrow = arrow_t;
				show_flag = 0;
				break;
			case 1:
				Cone_Nunber++;
				if (Cone_Nunber > 8)
					Cone_Nunber = 2;
				break;
			case 2:
				ccd_number++;
				if (ccd_number > 10)
					ccd_number = 1;
				break;
			default:
				break;
		}
	}
	else if (key_flag == reduce_flag)       //减少 reduce
	{
		switch(arrow)
		{
			case 1:
				Cone_Nunber--;
				if (Cone_Nunber < 2)
					Cone_Nunber = 8;
				break;
			case 2:
				ccd_number--;
				if (ccd_number < 1)
					ccd_number = 10;
				break;
			default:
				break;
		}
	}
}

/**
  *	 @brief  菜单集成模块,在此处完成相应菜单的切换
  *  @param  无
  *  @retval 无
  *  @memo   无
**/

void menu(void)			//菜单
{
	switch(menu_flage)
	{
		case 0:
			menu0();
			break;
		case 1:
			menu1_0();
			break;
		case 2:
			menu1_1();
			break;
		case 3:
			menu1_2();
			break;
		case 4:
			menu1_3();
			break;
		case 5:
			menu1_4();
			break;
		default:
			menu0();
			menu_flage = 0;
			break;
	}
}

/**
  *  @brief  UI显示
  *  @param  无
  *  @retval 无
  *  @memo   无
  **/

void UI(uint8 Type)		  	//UI
{
	if (Type == Debug)
	{
		menu();
		Cursor();
	}
	else if (Type == Racing)
	{
		if (go_flag == 0)
		{
			menu();
			Cursor();
		}
		else if (go_flag == 1)
		{
			mode(Type);
			if (show_flag == 1)
			{
				lcd_clear(BLACK);
				show_flag = 0;
			}
		}
	}
}

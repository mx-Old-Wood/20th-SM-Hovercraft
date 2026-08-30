#include "headfile.h"

double       array_point[41] = {0};
double       lat_differ = 0,lot_differ = 0;
unsigned int add_w = 0x00, add_r = 0x00;
uint8        Number_GPS = 2;
uint8        differ_flag = 0;
double xdata average_lat,average_lot;
double distance;
double azimuth;
//double distance = 6.0;
//double azimuth = 1.0;
float direction;
float diff_x_deg, diff_x_min, diff_y_deg, diff_y_min;
float diff_x, diff_y;
float distance_math, angle_math;
float latitude_degree, latitude_minute, longitude_degree, longitude_minute;

double my_atan2(double y, double x)
{
    if (x == 0)                    // 处理x = 0的情况
	{
        if (y > 0)
            return PI / 2;         // y轴正半轴
        else if (y < 0)
            return -PI / 2;        // y轴负半轴
        else
            return 0;              // 原点
    }
	else
	{
                              
		double theta = atan(y / x);// 计算基础角度        
		if (x > 0)                 // 处理x ≠ 0的情况
			return theta;          // 第一象限和第四象限
		else                       // 处理x < 0的情况
		{
			if (y >= 0)
				return theta + PI; // 第二象限或x轴负半轴
			else
				return theta - PI; // 第三象限
		}
  }
}

/**
  *  @brief  GPS点求平均
  *  @param  无
  *  @retval 无
  *  @memo   使定点更稳定
  **/

void GPS_data_average(void)
{
	uint8  xdata i;
	average_lat = 0;
	average_lot = 0;
	for(i = 0; i < 50; i++)
	{
		average_lat = average_lat+gps_tau1201.latitude;
		average_lot = average_lot+gps_tau1201.longitude;
		delay_ms(5);
	}
	
	average_lat = average_lat/50;
	average_lot = average_lot/50;
}

/**
  *  @brief  GPS数据向eeprom读取或存储
  *  @param  存储或读取, 存储或读取的地址, 存储数据
  *  @retval 无
  *  @memo   无
  **/

void GPS_eeprom_RW(uint8 operation, uint16 address, double GPS_Data)
{
	char xdata bytes[20];
	if (operation == write)
	{
		sprintf((unsigned char*)bytes, "%.15f ", GPS_Data);
		iap_write_bytes(address, (uint8*)bytes, 20);
	}
	else if (operation == read)
	{
		iap_read_bytes(address, (uint8*)bytes, 20);
		array_point[Number_GPS] = atof(bytes);
		Number_GPS++;
	}
}

/**
  *  @brief  GPS存点
  *  @param  写入或清除
  *  @retval 无
  *  @memo   由于某些奇怪的问题,对于存储数组下标从2开始
  **/

void GPS_record(uint8 operation)
{
	uint8 xdata Number_GPS_t = Number_GPS;
	uint8 xdata T = 0;
	if (operation == read)
	{
		for (Number_GPS = 2, add_r = 0x00; T < 40; add_r += 20, T++)
			GPS_eeprom_RW(read, add_r, 0);
		Number_GPS = Number_GPS_t;
	}
	else if (operation == write)
	{
		GPS_data_average();
		GPS_eeprom_RW(write, add_w, average_lat);                  //写入lat
		GPS_eeprom_RW(read, add_w, 0);                             //读出lat
		add_w += 20;
		GPS_eeprom_RW(write, add_w, average_lot);                  //写入lot
		GPS_eeprom_RW(read, add_w, 0);                             //读出lot
		add_w += 20;
		if (add_w >= (0x00 + 40 * 20))
			add_w = 0x00;
	}
	else if (operation == clean)
	{
		memset(array_point, '\0', sizeof(array_point));
		array_point[23] = '\0';
		iap_erase_page(0x20);
		add_w = 0x00;
		add_r = 0x00;
		Number_GPS = 2;
	}
}

void GPS_record_t(uint8 operation, uint8 GPS_Type)
{
	static uint8 xdata T1 = 0, T2 = 0, T3 = 0;
	uint8 xdata Number_GPS_t = Number_GPS;
	uint8 xdata T = 0;
	if (operation == read)
	{
		memset(array_point, '\0', sizeof(array_point));
		if (GPS_Type == 1)
		{
			for (Number_GPS = 2, add_r = 0x00; T < 16; add_r += 20, T++)
				GPS_eeprom_RW(read, add_r, 0);
			Number_GPS = Number_GPS_t;
		}
		else if (GPS_Type == 2)
		{
			for (Number_GPS = 2, add_r = 0x200; T < 40; add_r += 20, T++)
				GPS_eeprom_RW(read, add_r, 0);
			Number_GPS = Number_GPS_t;
		}
		else if (GPS_Type == 3)
		{
			for (Number_GPS = 2, add_r = 0x800; T < 32; add_r += 20, T++)
				GPS_eeprom_RW(read, add_r, 0);
			Number_GPS = Number_GPS_t;
		}
	}
	else if (operation == write)
	{
		GPS_data_average();
		if (GPS_Type == 1)
		{
			if (T1 == 0)
			{
				memset(array_point, '\0', sizeof(array_point));
				Number_GPS = 2;
				add_w = 0x00;
				T1 = 1;
				T2 = 0;
				T3 = 0;
			}
			if (add_w > (0x1FF))
				add_w = 0x00;
		}
		else if (GPS_Type == 2)
		{
			if (T2 == 0)
			{
				memset(array_point, '\0', sizeof(array_point));
				Number_GPS = 2;
				add_w = 0x200;
				T1 = 0;
				T2 = 1;
				T3 = 0;
			}
			if (add_w >= (0x7FF))
				add_w = 0x200;
		}
		else if (GPS_Type == 3)
		{
			if (T3 == 0)
			{
				memset(array_point, '\0', sizeof(array_point));
				Number_GPS = 2;
				add_w = 0x800;
				T1 = 0;
				T2 = 0;
				T3 = 1;
			}
			
			if (add_w >= (0xBFF))
				add_w = 0x800;
		}
		GPS_eeprom_RW(write, add_w, average_lat);                  //写入lat
		GPS_eeprom_RW(read, add_w, 0);                             //读出lat
		add_w += 20;
		GPS_eeprom_RW(write, add_w, average_lot);                  //写入lot
		GPS_eeprom_RW(read, add_w, 0);                             //读出lot
		add_w += 20;
	}
	else if (operation == clean)
	{
		memset(array_point, '\0', sizeof(array_point));
		Number_GPS = 2;
		if (GPS_Type == 1)
		{
			iap_erase_page(0x20);
			add_w = 0x00;
			add_r = 0x00;
		}
		else if (GPS_Type == 2)
		{
			iap_erase_page(0x220);
			iap_erase_page(0x420);
			iap_erase_page(0x620);
			add_w = 0x200;
			add_r = 0x200;
		}
		else if (GPS_Type == 3)
		{
			iap_erase_page(0x820);
			iap_erase_page(0xA20);
			add_w = 0x800;
			add_r = 0x800;
		}
	}
}

// 输入为a点的经纬度（x经度，y纬度），b点的经纬度    
void Set_gps_point(float ax_deg, double ax_min, float ay_deg, double ay_min,
				   float bx_deg, float bx_min, float by_deg, float by_min)
{
	diff_x_deg = bx_deg - ax_deg;
	diff_x_min = bx_min - ax_min;
	diff_y_deg = by_deg - ay_deg;
	diff_y_min = by_min - ay_min;
	diff_x = diff_x_deg + diff_x_min; // 合并差分值
	diff_y = diff_y_deg + diff_y_min; // 合并差分值

	diff_x *= 787;//换算cm单位
	diff_y *= 1113;//换算cm单位

	distance_math = sqrt(diff_x * diff_x + diff_y * diff_y) * 100;
	angle_math = my_atan2(diff_y, diff_x) * 57.29577951308232f; // 弧度转度;
	if (angle_math < 0)
		angle_math += 360.0f; // 修正为0~360
//	if(angle_math > 360)
//		angle_math -= 360.0f;// 修正为0~360
}

float Get_gps_angle(void)
{
	if (angle_math != 0)
		angle_math = 360 - angle_math;
	return angle_math;
}

float Get_gps_distance(void)
{
	return distance_math;
}

/**
  *  @brief  GPS数据处理
  *  @param  无
  *  @retval 无
  *  @memo   转换距离,两点之间的角,以及方位角,对于两点之间的角其会受到发车方向的影响
  **/

void GPS_data(void)
{
	azimuth  = get_two_points_azimuth(array_point[2 * number], array_point[2 * number + 1], gps_tau1201.latitude, gps_tau1201.longitude) - 90;//天台跑
	if (azimuth < 0)
		azimuth += 360;
	distance = get_two_points_distance(array_point[2 * number], array_point[2 * number + 1], gps_tau1201.latitude, gps_tau1201.longitude);
}

void GPS_data_t(void)
{
	double array_temp[2];
	array_temp[0] = array_point[2 * number] + lat_differ;
	array_temp[1] = array_point[2 * number + 1] + lot_differ;
//	latitude_degree = (uint16)array_point[2 * number];
//	latitude_minute = (array_point[2 * number] - (uint16)array_point[2 * number]) * 60;
//	longitude_degree = (uint16)array_point[2 * number + 1];
//	longitude_minute = (array_point[2 * number + 1] - (uint16)array_point[2 * number + 1]) * 60;
//	Set_gps_point(longitude_degree, longitude_minute, latitude_degree, latitude_minute, gps_tau1201.longitude_degree,
//				  gps_tau1201.longitude_minute, gps_tau1201.latitude_degree, gps_tau1201.latitude_minute);
//	distance = Get_gps_distance() / 50 ;
////	azimuth  = Get_gps_angle();
////	distance = Get_gps_distance() / 50 - head_distance;
////	azimuth  = get_two_points_azimuth(gps_tau1201.latitude, gps_tau1201.longitude,array_point[2 * number], array_point[2 * number + 1]) + 90;//天台跑
//	azimuth  = get_two_points_azimuth(gps_tau1201.latitude, gps_tau1201.longitude,array_point[2 * number], array_point[2 * number + 1]) ;//天台跑
////	if (azimuth < 0)
////		azimuth += 360;
////	if (azimuth > 360)
////		azimuth -= 360;
	latitude_degree = (uint16)array_temp[0];
	latitude_minute = (array_temp[0] - (uint16)array_temp[0]) * 60;
	longitude_degree = (uint16)array_temp[1];
	longitude_minute = (array_temp[1] - (uint16)array_temp[1]) * 60;
	Set_gps_point(longitude_degree, longitude_minute, latitude_degree, latitude_minute, gps_tau1201.longitude_degree,
				  gps_tau1201.longitude_minute, gps_tau1201.latitude_degree, gps_tau1201.latitude_minute);
	distance = Get_gps_distance() / 50 ;
//	azimuth  = Get_gps_angle();
//	distance = Get_gps_distance() / 50 - head_distance;
//	azimuth  = get_two_points_azimuth(gps_tau1201.latitude, gps_tau1201.longitude,array_point[2 * number], array_point[2 * number + 1]) + 90;//天台跑
	azimuth  = get_two_points_azimuth(gps_tau1201.latitude, gps_tau1201.longitude,array_temp[0], array_temp[1]) + Towards;     //正北跑
//	azimuth  = get_two_points_azimuth(gps_tau1201.latitude, gps_tau1201.longitude,array_temp[0], array_temp[1]) + 90 ;//天台跑
	if (azimuth < 0)
		azimuth += 360;
	else if (azimuth > 360)
		azimuth -= 360;
}


//void GPS_parse(void)
//{
//  if (gps_tau1201_flag == 1)
//	 {
//		 gps_tau1201_flag = 0;
//		 if (gps_data_parse() == 0)
//		  {
//			 P52 = !P52;
//		  }
//	 }
//}

void GPS_differ(void)                                    //GPS误差矫正
{
	lat_differ = gps_tau1201.latitude - array_point[2];
	lot_differ = gps_tau1201.longitude - array_point[3];
}


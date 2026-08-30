#ifndef __GPS_H_
#define __GPS_H_

/*********************************函数声明***********************************/
void GPS_eeprom_RW(uint8 operation, uint16 address, double GPS_Data);
void GPS_record(uint8 operation);
void GPS_record_t(uint8 operation, uint8 GPS_Type);
void GPS_parse(void);
void GPS_data_average(void);
void Set_gps_point(float ax_deg, double ax_min, float ay_deg, double ay_min,float bx_deg, float bx_min, float by_deg, float by_min);
float Get_gps_distance(void);
float Get_gps_angle(void);
void GPS_data(void);
void GPS_data_t(void);
void GPS_differ(void);
double my_atan2(double y, double x);

/******************************************************/
#define clean 2
#define write 1
#define read  0

/*******************************全局变量声明*********************************/
extern double lat;               //经度
extern double lot;               //纬度
extern double array_point[41];   //经纬度存储数据
extern double lat_differ,lot_differ;
extern double distance;
extern double azimuth;
extern float  direction;
extern uint8  differ_flag;

#endif

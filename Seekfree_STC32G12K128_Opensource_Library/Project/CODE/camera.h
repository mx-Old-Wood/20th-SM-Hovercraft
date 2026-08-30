#ifndef __CAMERA_H_
#define __CAMERA_H_

typedef struct
{
	uint8 Binarization[128];
	int16 Left_Border, Right_Border;    //左右边界
	int16 Left_Border_Last, Right_Border_Last;   //左右上一次边界
	float Midline, Midline_last;                 //中线，上一次中线
	uint16 Max, Min, Threshold;                  //阈值
	uint16 Black_Threshold;
	uint16 Width,Width_Last;
	float  Error;
	float  Image_Midline;
}CCD;

void Ccd_Type_Init(uint8 Type);
void Ccd_Sent_image(void);
void Get_Threshold(void);
void Ccd_Midline(void);
void Show_Ccd_Image(uint8 state);

extern CCD xdata Ccd;


#define Normal   0
#define OFF      0
#define NO       1

#endif

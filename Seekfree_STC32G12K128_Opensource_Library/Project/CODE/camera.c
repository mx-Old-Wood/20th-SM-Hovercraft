#include "headfile.h"

uint8 xdata y1_boundary[128];
uint8 xdata y2_boundary[128];
uint8 xdata y3_boundary[128];
uint8 xdata x1_boundary[128];
uint8 xdata x2_boundary[128];
CCD xdata Ccd;

uint32 seekfree_assistant_transfer_callback   (const uint8 *buff, uint32 length)
{
	uart_putbuff(DEBUG_UART, buff, length);
//	uart_putbuff(WIRELESS_UART_INDEX, buff, length);
	return 0;
}

void Ccd_Type_Init(uint8 Type)
{
	if(Type == Normal)
	{
		ccd_init();
	}
	else if (Type == Debug)
	{
		ccd_init();
		// 设置函数指针
		seekfree_assistant_transfer = seekfree_assistant_transfer_callback;
		seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, NULL, 128, 256);
		// 设置一个边线用于显示CCD波形
		seekfree_assistant_camera_boundary_config(Y_BOUNDARY, 128, NULL, NULL ,NULL, y1_boundary, y2_boundary, y3_boundary);
		// 边线3用于显示中线
		memset(y3_boundary, 0x80, sizeof(y3_boundary));
	}
}

void Ccd_Sent_image(void)
{
	uint16 xdata j;
	if(tsl1401_finish_flag)
    {
		tsl1401_finish_flag = 0;
        for(j = 0; j < 128; j++)
		{
			// 获取CCD数据，并按分辨率进行压缩
			switch(AD_RESOLUTION)
			{
				case ADC_8BIT:
				{
					y1_boundary[j] = (uint8)(128 - ccd_data_ch1[j] / 2);
					y2_boundary[j] = (uint8)(256 - Ccd.Binarization[j] / 2);
					break;
				}
				case ADC_10BIT:
				{
					y1_boundary[j] = (uint8)(128 - ccd_data_ch1[j] / 8);
					y2_boundary[j] = (uint8)(256 - (Ccd.Binarization[j] + 1) * 70 / 8);
					break;
				}
				case ADC_12BIT:
				{
					y1_boundary[j] = (uint8)(128 - ccd_data_ch1[j] / 32);
					y2_boundary[j] = (uint8)(256 - (Ccd.Binarization[j] + 1) * 70 / 32);
					break;
				}
			}
		}
            // 发送图像
		seekfree_assistant_camera_send();
	}
}

void Ccd_Binarization(void)
{
	uint16 Ccd_Max = 0, Ccd_Min = ccd_data_ch1[0], Threshold = 0;
	uint8 xdata i;
	for (i = 0; i < 128; i++)
	{
		if (ccd_data_ch1[i] > Ccd_Max)
			Ccd_Max = ccd_data_ch1[i];
		if (ccd_data_ch1[i] < Ccd_Min)
			Ccd_Min = ccd_data_ch1[i];
	}
	
	Ccd.Max = Ccd_Max;
	Ccd.Min = Ccd_Min;
	Threshold = Ccd_Min + 0.8 * (Ccd_Max - Ccd_Min);
	Ccd.Threshold = Threshold;
	if (Threshold < 0.72 * Ccd.Black_Threshold)
		Threshold = Ccd.Black_Threshold;
	for (i = 0; i < 128; i++)
	{
		if (ccd_data_ch1[i] >= Threshold)
			Ccd.Binarization[i] = 1;
		else
			Ccd.Binarization[i] = 0;
	}
}

int i, y, start_point[14] = {5, -5, 10, -10, 15, -15, 20, -20, 25, -25, 30, -30, 35, -35};

void Ccd_Midline(void)
{
	Ccd.Left_Border_Last = Ccd.Left_Border;
	Ccd.Right_Border_Last = Ccd.Right_Border;
	
	Ccd.Left_Border = 4;
	Ccd.Right_Border = 123;
	
	y = 64;
	
	if(Ccd.Binarization[y] != 1)
	{
		for(i = 0; i < 14; i++)
		{
			if(Ccd.Binarization[y + start_point[i]] != 0)
			{
				y = y + start_point[i];
				break;
			}
		}
	}
	
	for(i = y ;i<123;i++)                     //从边界右扫描
	{
		if(Ccd.Binarization[i] == 0)
		{
			if(Ccd.Binarization[i + 1] == 0 && Ccd.Binarization[i + 2] == 0 && Ccd.Binarization[i + 3] == 0 && Ccd.Binarization[i + 4] == 0 && Ccd.Binarization[i + 5] == 0 && Ccd.Binarization[i + 6] == 0 && Ccd.Binarization[i + 7] == 0 )
			{
				Ccd.Right_Border = i;
				break;
			}
		}
	}
	
	for(i = y;i>4;i--)                         //从边界左扫描
	{
		if(Ccd.Binarization[i] == 0)
		{
			if(Ccd.Binarization[i - 1] == 0 && Ccd.Binarization[i - 2] == 0 && Ccd.Binarization[i - 3] == 0 && Ccd.Binarization[i - 4] == 0 && Ccd.Binarization[i - 5] == 0 && Ccd.Binarization[i - 6] == 0 && Ccd.Binarization[i - 7] == 0 )
			{
				Ccd.Left_Border = i;
				break;
			}
		}
	}
	
	Ccd.Width_Last = Ccd.Width;
	Ccd.Width = Ccd.Right_Border - Ccd.Left_Border;
	
	Ccd.Midline_last = Ccd.Midline;
	Ccd.Midline = (float)(Ccd.Left_Border + Ccd.Right_Border) / 2;
	
	Ccd.Image_Midline = 64;
	
	Ccd.Error = Ccd.Image_Midline - Ccd.Midline;
	
	if(abs(Ccd.Error) <= 1)
	{
	  Ccd.Error = 0;
	}
	
}

void Get_Threshold(void)
{
	Ccd_Binarization();
	Ccd.Black_Threshold = Ccd.Threshold;
}

void Show_Ccd_Image(uint8 state)
{
	uint8 xdata i;
	if (state == NO)
	{
		for(i = 0; i < 128; i++)
		{
			if(Ccd.Binarization[i] == 1)
			{
				lcd_drawpoint(i, 230, WHITE);
				lcd_drawpoint(i, 220, BLACK);
				lcd_drawpoint(i, 229, WHITE);
				lcd_drawpoint(i, 221, BLACK);
			}
			else
			{
				lcd_drawpoint(i, 230, GREEN);
				lcd_drawpoint(i, 220, WHITE);
				lcd_drawpoint(i, 229, GREEN);
				lcd_drawpoint(i, 221, WHITE);
			}
		}
			//绘制中线//
		lcd_drawpoint(Ccd.Midline, 230, RED);
		lcd_drawpoint(Ccd.Midline + 1, 230, RED);
		lcd_drawpoint(Ccd.Midline - 1, 230, RED);
		for (i = 225; i < 235; i++)
			lcd_drawpoint(Ccd.Image_Midline, i, BLUE);
	}
}

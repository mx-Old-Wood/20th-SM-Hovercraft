#include "headfile.h"

int8 xdata tail[4] = {0x00, 0x00, 0x80, 0x7f};     //VOFA接收所需的尾帧
char xdata receive_data[10] = {0};
char xdata temp[10] = {'!', '!', '!', '!', '!', '!', '!', '!', '!', '\0'};
float xdata receive_vofa[3] = {0};
char xdata Rx_flag = 0;
float xdata VOFA_RData[4] = {0, 1, 2, 3};
float xdata sent_data[4] = {0, 0, 0, 0};


typedef union         //联合体
{
    float f;
    char c[4];
} FLOAT_CONV;

/**
  *  @brief  将浮点数由大端存储改为小端存储或是将浮点数由小端存储改为大端存储
  *  @param  需转换的浮点数
  *  @retval 转换后的浮点数
  *  @memo   无
  **/

float BLEndianFloat(float fValue)
{
    FLOAT_CONV d1, d2;
    d1.f = fValue;
    d2.c[0] = d1.c[3];
    d2.c[1] = d1.c[2];
    d2.c[2] = d1.c[1];
    d2.c[3] = d1.c[0];
    return d2.f;
}

/**
  *  @brief  将32位的数据由大端存储转换为小端存储或由小端存储转换为大端存储
  *  @param  需转换的32位数
  *  @retval 转换后的32位数
  *  @memo   无
  **/

uint32 swap_endian_32(uint32 x)
{
    return ((x & 0x000000FFU) << 24) |  
           ((x & 0x0000FF00U) << 8)  |  
           ((x & 0x00FF0000U) >> 8)  |  
           ((x & 0xFF000000U) >> 24);
}

/**
  *  @brief  将浮点数由大端存储改为小端存储或是将浮点数由小端存储改为大端存储
  *  @param  需转换的浮点数
  *  @retval 转换后的浮点数
  *  @memo   无
  **/

float swap_endian_float(float x) 
{
    uint32 xdata tmp = *(uint32*)&x;
    tmp = swap_endian_32(tmp);
    return *(float*)&tmp;
}

/**
  *  @brief  串口接收数据
  *  @param  无
  *  @retval 无
  *  @memo   该代码为半成品,稳定性差,仍需改进,仅做测试,实际使用请使用蓝牙或是无线模块的接收函数(已更换)
  **/

void uart_receive(void)
{
	static uint8 xdata i = 0;
	if (Rx_flag == 0)
	{
		receive_data[i++] = S4BUF;
		if (i == 10)
			i = 0;
		if (receive_data[0] != '#')
		{
			Rx_flag = 1;
			i = 0;
			receive_data[11] = 0;
			receive_data[12] = 0;
		}
		else if (receive_data[i] == '!' && receive_data[0] == '!')
		{
			Rx_flag = 1;
			i = 0;
			receive_data[11] = 0;
		}
	}
}

/**
  *  @brief  无线串口发送数据
  *  @param  需要发送的数据地址, 需要发送的长度
  *  @retval 无
  *  @memo   无
  **/

void wireless_send_data(const uint8 *buff, uint32 length)
{
	uart_putbuff(WIRELESS_UART_INDEX, buff, length);
}

/**
  *  @brief  向VOFA上位机软件发送相应数据(justfloat协议)
  *  @param  无
  *  @retval 无
  *  @memo   由于STC32为51内核,浮点数存储为大端存储,而VOFA需接收小端存储数据,需调换发送数据的字节数,才能正常使用VOFA上位机
  *          (目前采用TTL进行测试,之后将发送函数改为无线或蓝牙模块相应发送函数, 注:已修改)
  **/

void sent_vofa_para(void)
{
	VOFA_RData[0] = sent_data[0];
	VOFA_RData[0] = swap_endian_float(VOFA_RData[0]);
	VOFA_RData[1] = sent_data[1];
	VOFA_RData[1] = swap_endian_float(VOFA_RData[1]);
	VOFA_RData[2] = sent_data[2];
	VOFA_RData[2] = swap_endian_float(VOFA_RData[2]);
	VOFA_RData[3] = sent_data[3];
	VOFA_RData[3] = swap_endian_float(VOFA_RData[3]);
	wireless_send_data((const uint8*)VOFA_RData, sizeof(VOFA_RData));
	wireless_send_data((const uint8*)tail, sizeof(tail)); 
}

/**
  *  @brief  转换上位机发送参数
  *  @param  无
  *  @retval 完成转换的参数
  *  @memo   使用上位机发送参数时,参数发送间隔慢些,防止因发送过快导致单片机接收不过来而产生的数据丢失,或是参数发送卡住
  *          上位机发送数据格式为#~=%.3f!#(如#P=%.3f!#, #I=%.3f!#, #D=%.3f!#),使用时请将上位机中控件slider绑定响应
  *          命令再进行调参,由于串口发送函数的问题,该函数的准确性有待验证
  **/

float receive_vofa_para(void)
{
	uint8 xdata i = 3, j = 0;
	uint8 xdata dat[8];
	if (receive_data[0] == '#' && receive_data[1] == 'P' && receive_data[2] == '=')
	{
		for (i = 3, j = 0; receive_data[i] != '!' && i <  9; i++, j++)
		{
			dat[j] = receive_data[i];
		}
		receive_vofa[0] = atof(dat);
		strcpy(receive_data, temp);
		return receive_vofa[0];
	}
	else if (receive_data[0] == '#' && receive_data[1] == 'I' && receive_data[2] == '=')
	{
		for (i = 3, j = 0; receive_data[i] != '!' && i <  9; i++, j++)
		{
			dat[j] = receive_data[i];
		}
		receive_vofa[1] = atof(dat);
		strcpy(receive_data, temp);
		return receive_vofa[1];
	}
	else if (receive_data[0] == '#' && receive_data[1] == 'D' && receive_data[2] == '=')
	{
		for (i = 3, j = 0; receive_data[i] != '!' && i <  9; i++, j++)
		{
			dat[j] = receive_data[i];
		}
		receive_vofa[2] = atof(dat);
		strcpy(receive_data, temp);
		return receive_vofa[2];
	}
	else if (receive_data[0] == '#' && receive_data[1] == 'F' && receive_data[5] == '=')
	{
		if (receive_data[6] == '0')
		{
			go_flag = 0;
			fan_stop();
		}
		else if (receive_data[6] == '1')
			go_flag = 1;
		strcpy(receive_data, temp);
	}
	else if (receive_data[0] == '0')
	{
		for (i = 0; i < 9; i++)
		{
			if (receive_data[i] == '0')
				go_flag = 0;
		}
		strcpy(receive_data, temp);
	}
	else if (receive_data[0] == '1')
	{
		for (i = 0; i < 9; i++)
		{
			if (receive_data[i] == '1')
				go_flag = 1;
		}
		strcpy(receive_data, temp);
	}
	else
	{
		strcpy(receive_data, temp);
//		memset(receive_data, '!', sizeof(receive_data));
//		receive_data[9] = '\0';
//		wireless_uart_read_buff(receive_data, 9);
	}
	return 0;
}

/**
  *  @brief  VOFA集成函数
  *  @param  发送数据1, 发送数据2, 发送数据3, 发送数据4
  *  @retval 无
  *  @memo   无
  **/

void VOFA(float data1, float data2, float data3, float data4)
{
	receive_vofa_para();
	sent_vofa_para();
	sent_data[0] = data1;
	sent_data[1] = data2;
	sent_data[2] = data3;
	sent_data[3] = data4;
}

void key_control()
{
	if(P03 == 1)
	{
		go_flag = 1;
	}
	else if(P02 == 1)
	{
		go_flag = 0;
	}
}

#ifndef __VOFA_H__
#define __VOFA_H__

/*********************************函数声明***********************************/
void sent_vofa_para(void);
void uart_receive(void);
float receive_vofa_para(void);
void VOFA(float data1, float data2, float data3, float data4);
void key_control();
void LoRa_Init(uint16 bound);

/*******************************全局变量声明*********************************/
extern float xdata sent_data[4];
extern char xdata receive_data[10];
extern float xdata receive_vofa[3];

#endif

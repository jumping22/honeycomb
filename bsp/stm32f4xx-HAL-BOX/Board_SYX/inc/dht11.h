#ifndef __DHT11_H
#define __DHT11_H


#include "stm32f4xx_hal.h"
////////////////////////////////////////////////////////////////////////////////// 	


void DHT11_IO_OUT(void);
void DHT11_IO_IN(void);
   	
uint8_t DHT11_Init(void);//初始化DHT11
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);//读取温湿度
uint8_t DHT11_Read_Byte(void);//读出一个字节
uint8_t DHT11_Read_Bit(void);//读出一个位
uint8_t DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11  
#endif

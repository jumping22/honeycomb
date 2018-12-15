#ifndef _MAX7219_H_
#define _MAX7219_H_

#include "stm32f4xx_hal.h"

//#include "sys.h"
//#include "delay.h"

//#define DZHP1_DIN 63 //E10 //MAX7219
//#define DZHP1_DS  64 //E11
//#define DZHP1_CLK 65 //E12

//¶¨ÒåMax7219¶Ë¿Ú

#define Max7219_pinCLK_H		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET) 
#define Max7219_pinCLK_L  	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET)

#define Max7219_pinCS_H		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET) 
#define Max7219_pinCS_L  	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_RESET)

#define Max7219_pinDIN_H		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET) 
#define Max7219_pinDIN_L  	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET)


void Write_Max7219_byte(unsigned char DATA); 
void Write_Max7219(unsigned char address,unsigned char dat);
void Init_MAX7219(void);
extern void max7219_test(void);

#endif

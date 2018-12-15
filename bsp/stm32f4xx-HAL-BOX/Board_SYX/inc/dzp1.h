#ifndef _DZP1_H_
#define _DZP1_H_

#include "stm32f4xx_hal.h"


//¶¨ÒåMax7219¶Ë¿Ú

#define DZP1_pinCLK_H		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET) 
#define DZP1_pinCLK_L  	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET)

#define DZP1_pinCS_H		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET) 
#define DZP1_pinCS_L  	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET)

#define DZP1_pinDIN_H		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET) 
#define DZP1_pinDIN_L  	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET)


void Write_DZP1_byte(unsigned char DATA); 
void Write_DZP1(unsigned char address,unsigned char dat);
void Init_DZP1(void);
extern void DZP1_test(void);

#endif

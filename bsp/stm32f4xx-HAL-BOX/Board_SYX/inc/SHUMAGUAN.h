#ifndef _SHUMAGUAN_H
#define _SHUMAGUAN_H

#include "stm32f4xx_hal.h"

#define hc595_clk_h()	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET)	//RCK
#define hc595_clk_l()	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET)

#define hc595_sclk_h()	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET)	//SCK
#define hc595_sclk_l()	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET)

#define hc595_din_h()	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET)
#define hc595_din_l() 	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET)



void SMG_init(void);

void SMG_display(void);
void SMG_OUT(uint8_t data);
void data_OUT(int data);


#endif


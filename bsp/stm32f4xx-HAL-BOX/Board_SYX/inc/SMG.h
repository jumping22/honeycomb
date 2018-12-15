#ifndef _SMG_H
#define _SMG_H

#include "stm32f4xx_hal.h"


//DIN			PB2	
//STCLK		PB0
//SHCLK		PB1
void SMG_init(void);
void SMG_out(uint8_t data);
void SMG_Display(uint16_t addr,uint16_t count);
void SMG_Display_on(void);
void SMG_Display_off(void);
uint8_t SMG_get_state(void);
int SMG_show(uint16_t count);



#define SMG_DIN_SET	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET)
#define SMG_DIN_CLR	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET)

#define SMG_STCLK_SET	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)
#define SMG_STCLK_CLR	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)

#define SMG_SHCLK_SET	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)
#define SMG_SHCLK_CLR	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)

#endif


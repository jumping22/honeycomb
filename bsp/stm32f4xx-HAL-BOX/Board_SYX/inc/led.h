#ifndef _LED_H
#define _LED_H

#include "stm32f4xx_hal.h"
#include "timer.h"

void Din_1(void);
void Din_0(void);
void Send_8bits(uint8_t dat);
void Send_2811_24bits(uint8_t GData,uint8_t RData,uint8_t BData);
void rst();
void led_show(void);


void MX_TIM2_Init(uint16_t arr,uint16_t psc);
void TIM_GPIO_INIT(void);
void USER_PWM_SetDutyRatio(TIM_HandleTypeDef *htim,uint32_t Channel,uint8_t value);

#endif


#ifndef _TIMER_H
#define _TIMER_H
#include "stm32f4xx_hal.h"

void TIM3_PWM_Init(uint16_t arr,uint16_t psc);
void TIM_SetTIM3Compare1(uint32_t compare);

void TIM4_PWM_Init(uint16_t arr,uint16_t psc);
void TIM_SetTIM4Compare1(uint32_t compare);
void TIM3_Set_freq(uint16_t arr);
void TIM4_Set_freq(uint16_t arr);

extern TIM_HandleTypeDef htim2;
void MX_TIM2_Init(void);

#endif


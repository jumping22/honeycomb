#include "led.h"

TIM_HandleTypeDef htim2;


void Din_1(void)
{
	USER_PWM_SetDutyRatio(&htim2, TIM_CHANNEL_2, 26);
}
void Din_0(void)
{
	USER_PWM_SetDutyRatio(&htim2, TIM_CHANNEL_2, 13);
}


void Send_8bits(uint8_t dat) 
{   
		uint8_t i; 
		Din_0();
		for(i=0;i<8;i++)   
		{ 
			if(dat & 0x80)//1,for "1",H:0.8us,L:0.45us;
			{      
				Din_1();				
			} 		
			else 	//0 ,for "0",H:0.4us,L:	0.85us			
			{ 
			 Din_0();					
			}
		   dat=dat<<1; 
	 }
}

void Send_2811_24bits(uint8_t GData,uint8_t RData,uint8_t BData)
{   
	Send_8bits(GData);  
	Send_8bits(RData);  
	Send_8bits(BData);
} 
 

void rst() 
{ 
	 __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 0);	
	 HAL_Delay (1);
}	

void led_show(void)
{
	
	uint8_t i=0;
	MX_TIM2_Init(211,0);
	TIM_GPIO_INIT();
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	  
	
	 while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	//	UserLED_Toggle();
		
			  rst();
//				for(i=0;i<36;i++)
//					{							  
							Send_2811_24bits(255,0,0);
//					}
//				HAL_Delay(1000);
//				rst();
//				for(i=0;i<36;i++)
//					{
//							Send_2811_24bits(0,255,0);
//					}
//				HAL_Delay(1000);
//				rst();
//				for(i=0;i<36;i++)
//					{
//							Send_2811_24bits(0,0,255);
//					}
//				HAL_Delay(3000);
  }
	
}


TIM_HandleTypeDef TIM2_Handler;      	//定时器句柄 
TIM_OC_InitTypeDef TIM2_CH2Handler;

void MX_TIM2_Init(uint16_t arr,uint16_t psc)
{
		TIM2_Handler.Instance=TIM2;          	//定时器3
    TIM2_Handler.Init.Prescaler=psc;       //定时器分频
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM2_Handler.Init.Period=arr;          //自动重装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM2_Handler);       //初始化PWM
    
    TIM2_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM2_CH2Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM2_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH2Handler,TIM_CHANNEL_2);//配置TIM2通道2
	
    HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_2);//开启PWM通道2

}

void TIM_GPIO_INIT(void)
{

  /* USER CODE BEGIN TIM2_MspPostInit 0 */
	  GPIO_InitTypeDef GPIO_InitStruct;
		__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /* USER CODE END TIM2_MspPostInit 0 */
  
    /**TIM2 GPIO Configuration    
    PB3     ------> TIM2_CH2 
    */
 

  /* USER CODE BEGIN TIM2_MspPostInit 1 */

  /* USER CODE END TIM2_MspPostInit 1 */

}

void USER_PWM_SetDutyRatio(TIM_HandleTypeDef *htim,uint32_t Channel,uint8_t value)
{
	TIM_OC_InitTypeDef sConfigOC;
	
	uint32_t period=htim->Init.Period+1;
	uint32_t pluse=(value * period)/100;
	
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = pluse;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, Channel);
	HAL_TIM_PWM_Start(htim, Channel);   
}

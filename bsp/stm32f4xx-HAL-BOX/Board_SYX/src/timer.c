#include "timer.h"
#include "SMG.h"


TIM_HandleTypeDef TIM3_Handler;      	//定时器句柄 
TIM_OC_InitTypeDef TIM3_CH1Handler;

TIM_HandleTypeDef TIM4_Handler;      	//定时器句柄 
TIM_OC_InitTypeDef TIM4_CH2Handler;

TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 


void TIM3_PWM_Init(uint16_t arr,uint16_t psc)
{  
    TIM3_Handler.Instance=TIM3;          	//定时器3
    TIM3_Handler.Init.Prescaler=psc;       //定时器分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM3_Handler.Init.Period=arr;          //自动重装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       //初始化PWM
    
    TIM3_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM3_CH1Handler.Pulse=0;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM3_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH1Handler,TIM_CHANNEL_1);//配置TIM3通道1
	
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_1);//开启PWM通道1
}

void TIM4_PWM_Init(uint16_t arr,uint16_t psc)
{  
    TIM4_Handler.Instance=TIM4;          	//定时器4
    TIM4_Handler.Init.Prescaler=psc;       //定时器分频
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM4_Handler.Init.Period=arr;          //自动重装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM4_Handler);       //初始化PWM
    
    TIM4_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM4_CH2Handler.Pulse=arr;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM4_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH2Handler,TIM_CHANNEL_2);//配置TIM4通道2
	
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_2);//开启PWM通道1
}


//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
	__HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
	
		__HAL_RCC_TIM4_CLK_ENABLE();			//使能定时器4
	__HAL_RCC_GPIOD_CLK_ENABLE();			//开启GPIOD时钟
	
	GPIO_Initure.Pin=GPIO_PIN_6;           	//PC6
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	GPIO_Initure.Alternate= GPIO_AF2_TIM3;	//PC6复用为TIM3_CH1
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_13;           	//PD13
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	GPIO_Initure.Alternate= GPIO_AF2_TIM4;	//PD13复用为TIM4_CH2
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
}

//设置TIM通道4的占空比
//compare:比较值
void TIM_SetTIM3Compare1(uint32_t compare)
{
	TIM3->CCR1=compare; 
}

void TIM_SetTIM4Compare1(uint32_t compare)
{
	TIM4->CCR2=compare; 
}

void TIM3_Set_freq(uint16_t arr)
{
	TIM3->ARR=arr;
	TIM3->CCR1=(uint16_t)(arr);
}

void TIM4_Set_freq(uint16_t arr)
{
	TIM4->ARR=arr;
	TIM4->CCR2=(uint16_t)(arr);
}



void MX_TIM2_Init(void)
{
    TIM2_Handler.Instance=TIM2;                          //通用定时器3
    TIM2_Handler.Init.Prescaler=84-1;                     //分频系数
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=5000-1;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE  

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
  
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM3中断   
	
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}

 
//定时器3中断服务函数
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Handler);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	 if (htim == (&TIM2_Handler))

	 {

		 SMG_show(45);
		//HAL_Delay(1000);
	 }

}


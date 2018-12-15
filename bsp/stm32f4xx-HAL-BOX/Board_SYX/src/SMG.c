#include "SMG.h"


static const uint8_t SMG_OF[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x73,0x40,0x39,0x5e,0x79,0x00,0x40};
static const uint8_t SMG_COD[]={0xfe,0xfd,0xfb,0xf7};
//static uint8_t SMG[4];
static uint8_t SMG_count;
static uint8_t SMG_state=0;


extern void HAL_Delay_us(__IO uint32_t delay_us);



void SMG_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  /* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}

void SMG_out(uint8_t data)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(data&0x80){
			SMG_DIN_SET;
		}
		else{
			SMG_DIN_CLR;
		}
		data <<=1;
	SMG_SHCLK_CLR;
	//HAL_Delay(1);
	SMG_SHCLK_SET;
	}	

	
}

void SMG_Display(uint16_t addr,uint16_t count)
{
	SMG_out(SMG_OF[count]);
	HAL_Delay(4);
	SMG_out(SMG_COD[addr-1]);
	//HAL_Delay(1);
	SMG_STCLK_CLR;
	//HAL_Delay(1);
	SMG_STCLK_SET;
	//HAL_Delay(1);
}

void SMG_Display_on(void)
{
	SMG_count =1;
}

void SMG_Display_off(void)
{
	uint8_t i;
	SMG_count = 0;
	for(i=4;i>0;i--)
	{
		SMG_out(0x00);
		SMG_out(0xff&(~(1<<(i-1))));
		//HAL_Delay(1);
		SMG_STCLK_CLR;
		//HAL_Delay(1);
		SMG_STCLK_SET;
		//HAL_Delay(1);
	}
	
}

uint8_t SMG_get_state(void)
{
	return SMG_state;
	
}



int SMG_show(uint16_t count)
{
	if(count>=200)
	{
		return 0;
	}
	uint8_t led[4];
	uint8_t display_count;

	
	  //确定要显示的数字位数
  if(count>=10000)display_count=5;
  if(count<10000 && count>=1000)display_count=4;
  if(count<1000 && count>=100)display_count=3;
  if(count<100 && count>=10)display_count=2;
  if(count<10 )display_count=1;

  led[3] = count/1000;
  led[2] = (count%1000)/100;
  led[1] = (count%100)/10;
  led[0] = (count%10);
	
	rt_enter_critical();
	
	switch(display_count)
	{
		case 1: {
			SMG_Display(3,led[0]);
		}break;
		
		case 2: {
			SMG_Display(3,led[0]);
			HAL_Delay_us(7000);
			SMG_Display(2,led[1]);

		}break;
		
		case 3: {
			SMG_Display(3,led[0]);
			HAL_Delay_us(5000);
			SMG_Display(2,led[1]);
			HAL_Delay_us(5000);
			SMG_Display(1,led[2]);
			//	HAL_Delay_us(500);
		}break;		

		default:
			break;
		
	}
	
	rt_exit_critical();
}


















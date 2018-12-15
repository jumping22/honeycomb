#include "SHUMAGUAN.h"


void SMG_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	/*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
//	hc595_clk_h();
//	hc595_din_h();
}
 
uint8_t LED[8];
unsigned char  LED_0F[] = 
{// 0         1          2           3        4         5          6           7        8         9          A           b        C    d          E    F    -
        0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};



void SMG_OUT(uint8_t data)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(data&0x80)
			{
				hc595_din_h();
				HAL_Delay_us(10);
			}
		else
			{
				hc595_din_l();
				HAL_Delay_us(10);
			}
	}
	
	data<<=1;
	HAL_Delay_us(10);
	hc595_sclk_l();
	HAL_Delay_us(10);
	hc595_sclk_h();
	HAL_Delay_us(10);
}
 
 
void SMG_display(void)
{
	uint8_t *p;
	uint8_t i;
	
	SMG_OUT(0xf9);
	HAL_Delay_us(10);
	SMG_OUT(0x07);
	HAL_Delay_us(10);
	hc595_clk_l();
	HAL_Delay_us(10);
	hc595_clk_h();
	HAL_Delay_us(10);
	
//	//显示第一位
//	p = LED_0F + LED[0];
//	i = *p;
//	SMG_OUT(i);
//	SMG_OUT(0x01);
//	
//	hc595_clk_l();
//	hc595_clk_h();

//	//显示第二位
//	p = LED_0F + LED[1];
//	i = *p;
//	SMG_OUT(i);
//	SMG_OUT(0x02);
//	
//	hc595_clk_l();
//	hc595_clk_h();
	
	//显示第三位
//	p = LED_0F + LED[2];
//	i = *p;
//	SMG_OUT(i);
//	SMG_OUT(0x03);
//	
//	hc595_clk_l();
//	hc595_clk_h();

//	//显示第四位
//	p = LED_0F + LED[3];
//	i = *p;
//	SMG_OUT(i);
//	SMG_OUT(0x04);
//	
//	hc595_clk_l();
//	hc595_clk_h();
//	
//	//显示第五位
//	p = LED_0F + LED[4];
//	i = *p;
//	SMG_OUT(i);
//	SMG_OUT(0x05);
//	
//	hc595_clk_l();
//	hc595_clk_h();

//	//显示第六位
//	p = LED_0F + LED[5];
//	i = *p;
//	SMG_OUT(i);
//	SMG_OUT(0x06);
//	
//	hc595_clk_l();
//	hc595_clk_h();
//	
//	//显示第七位
//	p = LED_0F + LED[6];
//	i = *p;
//	SMG_OUT(i);
//	SMG_OUT(0x07);
//	
//	hc595_clk_l();
//	hc595_clk_h();

//	//显示第八位
//	p = LED_0F + LED[7];
//	i = *p;
//	SMG_OUT(i);
//	SMG_OUT(0x08);
//	
//	hc595_clk_l();
//	hc595_clk_h();
		
}



void data_OUT(int data)
{
        LED[7] = 0;
        LED[6] = 0;
        LED[5] = data % 1000000 / 100000;
        LED[4] = data % 100000 / 10000;
//        
        LED[3] = data % 10000 / 1000;
        LED[2] = data % 1000 / 100;
        LED[1] = data % 100 / 10;
        LED[0] = data % 10;
}









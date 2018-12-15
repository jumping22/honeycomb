#include "dzp1.h"

void DZP1_test(void);

unsigned char disp2[38][8]={
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0
{0x10,0x18,0x14,0x10,0x10,0x10,0x10,0x10},//1
{0x7E,0x2,0x2,0x7E,0x40,0x40,0x40,0x7E},//2
{0x3E,0x2,0x2,0x3E,0x2,0x2,0x3E,0x0},//3
{0x8,0x18,0x28,0x48,0xFE,0x8,0x8,0x8},//4
{0x3C,0x20,0x20,0x3C,0x4,0x4,0x3C,0x0},//5
{0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x0},//6
{0x3E,0x22,0x4,0x8,0x8,0x8,0x8,0x8},//7
{0x0,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E},//8
{0x3E,0x22,0x22,0x3E,0x2,0x2,0x2,0x3E},//9
{0x8,0x14,0x22,0x3E,0x22,0x22,0x22,0x22},//A
{0x3C,0x22,0x22,0x3E,0x22,0x22,0x3C,0x0},//B
{0x3C,0x40,0x40,0x40,0x40,0x40,0x3C,0x0},//C
{0x7C,0x42,0x42,0x42,0x42,0x42,0x7C,0x0},//D
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C},//E
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40},//F
{0x3C,0x40,0x40,0x40,0x40,0x44,0x44,0x3C},//G
{0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44},//H
{0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C},//I
{0x3C,0x8,0x8,0x8,0x8,0x8,0x48,0x30},//J
{0x0,0x24,0x28,0x30,0x20,0x30,0x28,0x24},//K
{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C},//L
{0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81},//M
{0x0,0x42,0x62,0x52,0x4A,0x46,0x42,0x0},//N
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//O
{0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20},//P
{0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D},//Q
{0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21},//R
{0x0,0x1E,0x20,0x20,0x3E,0x2,0x2,0x3C},//S
{0x0,0x3E,0x8,0x8,0x8,0x8,0x8,0x8},//T
{0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C},//U
{0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},//V
{0x0,0x49,0x49,0x49,0x49,0x2A,0x1C,0x0},//W
{0x0,0x41,0x22,0x14,0x8,0x14,0x22,0x41},//X
{0x41,0x22,0x14,0x8,0x8,0x8,0x8,0x8},//Y
{0x0,0x7F,0x2,0x4,0x8,0x10,0x20,0x7F},//Z
{0x0c,0x12,0x22,0x44,0x22,0x12,0x0c,0x00},
};

//--------------------------------------------
//���ܣ���MAX7219(U3)д���ֽ�
//��ڲ�����DATA 
//���ڲ�������
//˵����
void Write_DZP1_byte(unsigned char DATA)         
{
	unsigned char i;    
	DZP1_pinCS_L;	
	HAL_Delay_us(2);	
	for(i=8;i>=1;i--)
	{		  
			DZP1_pinCLK_L;
			HAL_Delay_us(2);
			//Max7219_pinDIN=DATA>>7;
			if((DATA>>7)==0)
			{
				DZP1_pinDIN_L;
			}
			else
			{
				DZP1_pinDIN_H;
			}
			HAL_Delay_us(2);
			DATA=DATA<<1;
			DZP1_pinCLK_H;	
			HAL_Delay_us(2);
	}                                 
}
//-------------------------------------------
//���ܣ���MAX7219д������
//��ڲ�����address��dat
//���ڲ�������
//˵����
void Write_DZP1(unsigned char address,unsigned char dat)
{ 
   DZP1_pinCS_L;
	 Write_DZP1_byte(address);           //д���ַ��������ܱ��
	 HAL_Delay_us(10);
   Write_DZP1_byte(dat);               //д�����ݣ����������ʾ����
	 HAL_Delay_us(10);	
	 DZP1_pinCS_H;                        
}


//void Max7219(int argc, char** argv)
//{
//  uint32_t add = 0;
//	uint32_t dat = 0;
//  rt_kprintf("argv[0]: %s\n", argv[0]);
//  if (argc > 1)
//    rt_kprintf("argv[1]: %s\n", argv[1]);
//  
//  add = atol(argv[1]);
//	dat = atol(argv[2]);
//  
//  Write_Max7219(add, dat);
//  
//}
//MSH_CMD_EXPORT(Max7219, test max7219);


void Init_DZP1(void)
{
	 GPIO_InitTypeDef GPIO_Initure;
	 __HAL_RCC_GPIOE_CLK_ENABLE();			
	 GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4; 
	 GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
  	
	 GPIO_Initure.Pull=GPIO_PULLUP;         
	 GPIO_Initure.Speed=GPIO_SPEED_HIGH;    
	 HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
	 HAL_Delay(200);
	
	 Write_DZP1(0x09, 0x00);       //���뷽ʽ��BCD��
	 Write_DZP1(0x0a, 0x03);       //���� 
	 Write_DZP1(0x0b, 0x06);       //ɨ����ޣ�8���������ʾ
	 Write_DZP1(0x0c, 0x01);       //����ģʽ��0����ͨģʽ��1
	 Write_DZP1(0x0f, 0x00);       //��ʾ���ԣ�1�����Խ�����������ʾ��0
}
void DZP1_test(void)
{
//	 unsigned char i,j;
	 HAL_Delay(50);
//		Init_DZP1();
	
	 //while(1)
//	 {

//		for(j=0;j<38;j++)
//		{
//		 for(i=1;i<9;i++)
//			Write_Max7219(i,disp1[j][8-i]);
//			HAL_Delay(1000);
//		} 
//	 }
		 Write_DZP1(1,0x1<<1);
		 Write_DZP1(2,0x2<<1);
		 Write_DZP1(3,0x3<<1);
		 Write_DZP1(4,0x4<<1);
		 Write_DZP1(5,0x5<<1);

		 
		 
	 
}
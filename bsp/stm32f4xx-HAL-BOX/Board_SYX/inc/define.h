#ifndef _DEFINE_H
#define _DEFINE_H

/*独立LED*/
//#define LED0 10 //F0
/*十字路口RGB*/
#define RGB_R 67 //E4
#define RGB_G 66 //E3
#define RGB_B 64 //E2


///*数码管*/
//#define SMG_D0 1  //E2 //74HC595
//#define SMG_D1 2  //E3
//#define SMG_D2 3  //E4
/*点整屏1*/
#define DZHP1_DIN 63 //E10 //MAX7219
#define DZHP1_DS  68 //E11
#define DZHP1_CLK 65 //E12
/*点整屏2*/
#define DZHP2_DIN 66  //E13
#define DZHP2_DS	67	//E14
#define DZHP2_CLK	68	//E15
/*4个独立按键*/
#define KEY1	50	//F12
#define KEY2	53	//F13
#define KEY3	54	//F14
#define KEY4	55	//F15
/*模拟遥感的一个按键与5向遥感*/
#define YG_KEY 26 //C0
#define YG_X   34 //A0   ADC1_IN0 
#define YG_Y   35 //A1   ADC1_IN1 
#define YG_5SW_KEY1 93  //G8
#define YG_5SW_KEY2 124 //G9
#define YG_5SW_KEY3 125 //G10 
#define YG_5SW_KEY4 126 //G11
#define YG_5SW_KEY5 127 //G12
/*声音传感器*/
#define SY_A0 36 //A2    ADC1_IN2 
#define SY_D0 85 //D14
/*人体红外*/ 
#define RTHW 76 //B15  
/*电压采集*/
#define BAT_ADC 20 //F8  ADC3_IN6 
/*旋转电位器*/
#define XZHDWQ   37  //A3 ADC1_IN3 
/*温湿度传感器*/
#define WSHD     86  //D15
/*温度*/
#define WD       90  //G5
/*光敏电阻*/
#define GMDZ     19  //F7
/*酒精传感器*/
#define JIUJING  21  //F9   //?????????????
/*马达*/
#define MADA     82  //D13 TIM4 CH2
/*蜂鸣器*/
#define FMQ      96  //C6 TIM3 CH1
/*超声波*/
#define CSB_TX  78  //D9
#define CSB_RX  77  //D8
/*SPI_FLASH*/
#define FLASH_MOSI 43 //A7
#define FLASH_MISO 42 //A6
#define FLASH_SCK 41  //A5
#define FLASH_CS 40   //A4

/*TFT屏*/
#define LCD_CS    73   	//B12
#define SPI2_SCK  69  	//B10
#define SPI2_MOSI 29  	//C3
#define LCD_DC    88  	//G3
#define LCD_RES   87 		//G2
#define LCD_BLK   89    //G4 

#define YG_X_ADC_CH   ADC_CHANNEL_0
#define YG_Y_ADC_CH   ADC_CHANNEL_1
#define SY_ADC_CH     ADC_CHANNEL_2
#define XZHDWQ_ADC_CH ADC_CHANNEL_3
#define BAT_ADC_CH    ADC_CHANNEL_6 //ADC3


#endif

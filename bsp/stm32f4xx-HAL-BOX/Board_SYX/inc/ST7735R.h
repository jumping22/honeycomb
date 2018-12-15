#ifndef _ST7735R_H
#define _ST7735R_H

#include "stm32f4xx_hal.h"
#include "spi.h"




#define X_MAX_PIXEL	        240
#define Y_MAX_PIXEL	        240

#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//»ÒÉ«0 3165 00110 001011 00101
#define GRAY1   0x8410      	//»ÒÉ«1      00000 000000 00000
#define GRAY2   0x4208      	//»ÒÉ«2  1111111111011111

#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOB
#define LCD_RES_Pin GPIO_PIN_2
#define LCD_RES_GPIO_Port GPIOG
#define LCD_DC_Pin GPIO_PIN_3
#define LCD_DC_GPIO_Port GPIOG
#define LCD_BL_Pin GPIO_PIN_4
#define LCD_BL_GPIO_Port GPIOG

#define LCD_CS_SET		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET) 
#define LCD_CS_CLR  	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)

#define LCD_RST_SET		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_SET) 
#define LCD_RST_CLR  	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_RESET)

#define LCD_DC_SET		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_SET) 
#define LCD_DC_CLR  	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_RESET)

#define LCD_BL_SET		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_SET) 
#define LCD_BL_CLR  	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_RESET)

//#define LCD_SDA_SET	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET)
//#define LCD_SDA_CLR	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET)

//#define LCD_SCL_SET	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET)
//#define LCD_SCL_CLR	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET)

void lcd_spi_gpio_init(void);
void Lcd_WriteIndex(uint8_t Index);
void Lcd_WriteData(uint8_t Data);
void LCD_WriteData_16Bit(uint16_t Data);
void LCD_WriteBuf_16(uint16_t *buf, uint32_t length);
void Lcd_WriteReg(uint8_t Index,uint8_t Data);
void LCD_WriteIndexAndData(uint16_t Index,uint16_t Data);
void Lcd_Reset(void);
void Lcd_Init(void);
void Lcd_SetRegion(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end);
void Lcd_SetCursor(uint8_t x_start,uint8_t y_start);
void Lcd_Clear(uint16_t	Color) ;
void Lcd_SetXY(uint8_t x,uint8_t y);
uint32_t mypow(uint8_t m,uint8_t n);
void LCD_show_char24(uint8_t x,uint8_t y,uint16_t color,uint8_t size,uint8_t data);




void LCD_draw_point(uint8_t x,uint8_t y,uint16_t Color);
void LCD_show_image(void);
void LCD_show_char(uint8_t x,uint8_t y,uint16_t color,uint8_t size,uint8_t data);
void LCD_show_string(uint8_t x,uint8_t y,uint16_t color,uint8_t size,const uint8_t *p);
int LCD_draw_line(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint16_t color);
int LCD_draw_circle(uint8_t x,uint8_t y,uint8_t r,uint16_t color);
int LCD_draw_rect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint16_t color,int fill);
void LCD_show_hanzi(unsigned int x,unsigned int y,unsigned char index)	;
//void LCD_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint16_t color);


void LCD_Pin_Conf(void);

#endif



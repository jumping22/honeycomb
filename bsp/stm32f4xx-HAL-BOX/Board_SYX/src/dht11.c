#include "dht11.h"

void DHT11_IO_IN(void)
{
		GPIO_InitTypeDef GPIO_Initure;
	
    GPIO_Initure.Pin=GPIO_PIN_15;           	//PD15
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  		//推挽输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化
}

void DHT11_IO_OUT(void)
{
		GPIO_InitTypeDef GPIO_Initure;
	
    GPIO_Initure.Pin=GPIO_PIN_15;           	//PD15
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化
}


//复位DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//设置为输出
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET); 	//拉低DQ
	HAL_Delay_us(20000);    	//拉低至少18ms
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET); 	//DQ=1 
	HAL_Delay_us(30);     	//主机拉高20~40us
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_IO_IN();      //设置为输出	
	while (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15)&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		HAL_Delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
	while ((!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15)) && (retry<100))//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		HAL_Delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//从DHT11读取一个位
//返回值：1/0
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15)&&retry<100)//等待变为低电平
	{
		retry++;
		HAL_Delay_us(1);
	}
	retry=0;
	while(!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15)&&retry<100)//等待变高电平
	{
		retry++;
		HAL_Delay_us(1);
	}
	HAL_Delay_us(40);//等待40us
	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15))return 1;
	else return 0;		   
}

//从DHT11读取一个字节
//返回值：读到的数据
uint8_t DHT11_Read_Byte(void)    
{        
	uint8_t i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)    
{        
 	uint8_t buf[5];
	uint8_t i;
	uint8_t result = 0;
	
	//rt_enter_critical();
	
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
		else
		{
			result = 1;
		}
	}else {
		result = 1;
	}
	
	//rt_exit_critical();
	
	return result;	    
}

//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在     	 
uint8_t DHT11_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOD_CLK_ENABLE();			//开启GPIOD时钟
	
    GPIO_Initure.Pin=GPIO_PIN_15;           	//PD15
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化
 
    DHT11_Rst();
	return DHT11_Check();
}

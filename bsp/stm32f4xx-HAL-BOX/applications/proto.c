#include "proto.h"
#include "stm32f4xx_hal.h"
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include "define.h" 
#include "ST7735R.h"
#include "CGQ.h"
#include "timer.h"
#include "max7219.h"
#include "dzp1.h"
#include "dht11.h"
#include "ds18b20.h"
#include "adc.h"
#include "GUI.h"
#include "SMG.h"
#include "CGQ.h"




#define PROTO_HEAD		0x5a
#define PROTO_TAIL		0xa5



typedef struct {
	uint8_t head;
	uint8_t length;
	uint8_t type;
	uint16_t *data;
	uint8_t sum;
	uint8_t tail;
}proto_struct;

enum {
	RGB_LED = 1,			//1
	PASSIVE_BUZZER,		//2
	QXKG,							//3
	SMG,							//4
	MOTOR,						//5
	DZP,							//6
	YG,								//7
	TFTLCD,						//8
	SOUND_SENSOR,			//9
	PIR,							//10
	DHT11,						//11
	DS18B20,					//12
	LIGHT,						//13
	JJCGQ,						//14
	ROTARY_ENCODERS,	//15
	GDKG,							//16
	DZP1,							//17
	KEY5,							//18
	Dial,							//19
	traffic_light,		//20
	GUI_clear,				//21
};



void DZP_set(uint8_t *data);
uint8_t DHT11_get(uint8_t *tem, uint8_t *hum);
uint8_t DS18B20_get(uint16_t *temp);
void DZP1_set(uint8_t *data);
void MOTOR_set(uint8_t data);
uint8_t YG_set(uint8_t *x,uint8_t *y,uint8_t *z);
uint8_t jiujing_set(uint8_t *data);
uint8_t XZDWQ_set(uint8_t *data);
uint8_t SOUNDSENSOR_set(uint8_t * data);
void Dial_set(uint8_t data);
void traffic_light_set(uint8_t color_r,uint8_t color_y,uint8_t color_g);
void GUI_set(void);



//static uint8_t temperature = 0;
//static uint8_t humidity = 0;
uint8_t value89;



#if defined(PROTO_DEBUG)
	static uint8_t test_sendbuf[20];
	static uint8_t test_length = 0;
#endif

static proto_send_cb _proto_send_func;

static uint8_t _realcmd = 0;
static uint8_t *p_realdata;
static uint8_t _realdatalength = 0;

static uint8_t _byte_length = 0;
static uint8_t _byte_recvbuf[64];
static uint8_t _byte_recvbuf_index = 0;

int Proto_data_analyze_byte(uint8_t data)
{
	static uint8_t state = 0;
	uint8_t result = 0;

	switch(state) {
		case 0: {
			if (data == PROTO_HEAD) {
				_byte_length = 0;
				_byte_recvbuf_index = 0;
				state = 1;
			} else {
				state = 0;
				//rt_kprintf(" STATE0 ERROR");
			}
		}break;

		case 1: {
			_byte_length = data;
			state = 2;
		}break;

		case 2: {
			if (data == 0x01) {
				state = 3;
			} else {
				state = 0;
				//rt_kprintf(" STATE2 ERROR");
			}
		}break;

		case 3: {
			_byte_recvbuf[_byte_recvbuf_index++] = data;
			if (_byte_recvbuf_index >= (_byte_length-5)) {
				state = 4;
				//rt_kprintf(" STATE3 SUCCESS");
			}
		}break;

		case 4: {
			if (data == 0xff) {
				state = 5;
			} else {
				state = 0;
				//rt_kprintf(" STATE4 ERROR");
			}
		}break;

		case 5: {
			if (data == PROTO_TAIL) {
				_realcmd = _byte_recvbuf[0];
				p_realdata = &_byte_recvbuf[1];
				_realdatalength = _byte_length-6;
				result = 1;
					//rt_kprintf(" STATE5 SUCCESS\n");
			}
			state = 0;
		

		}break;
	}
	return result;
}

int Proto_data_analyze(uint8_t *data, uint8_t size)
{
	uint8_t i = 0;
	uint8_t length = 0;
	uint8_t result = 0;
	
	//find proto head
	for (;i<size;) {
		if (data[i] == PROTO_HEAD) {
			length = data[i+1];
			if (data[i+length-1] == PROTO_TAIL) {
				//_sensor_cmd_deal(data[i+3], &data[i+4], length-6); // not include cmd
				_realcmd = data[i+3];
				p_realdata = &data[i+4];
				_realdatalength = length-6;
				result = 1;
				break;
			}
		}
		i++;
	}
	return result;
}

static int _send(uint8_t cmd, uint8_t *data, uint8_t size)
{
	uint8_t sendbuf[20] = {PROTO_HEAD, 0,0x81};

	sendbuf[1] = size+6;	//length
	sendbuf[3] = cmd;
	memcpy(&sendbuf[4], data, size);
	sendbuf[4+size] = 0xff;
	sendbuf[5+size] = PROTO_TAIL;

#if defined(PROTO_DEBUG)
	test_length = size + 6;
	memcpy(test_sendbuf, sendbuf, test_length);
#endif

	//uart send
	if (_proto_send_func != NULL) {
		_proto_send_func(sendbuf, size+6);
	}

	return 0;
}

			

int Proto_sensor_cmd_deal(void)
{
	uint8_t cmd = _realcmd;
	uint8_t *data = p_realdata;
	//uint8_t size = _realdatalength;

	uint8_t _data[8];
	uint8_t sendlength = 0;
	
	switch (cmd) {
		case RGB_LED: {

			rt_pin_write(RGB_R, data[0]);
			rt_pin_write(RGB_G, data[1]);
			rt_pin_write(RGB_B, data[2]);

			_data[0] = 1;
			sendlength = 1;
			
		}break;
		
		
		case PASSIVE_BUZZER: {
			int i;
			i=data[0];

			if(data[0])
			{
				TIM3_Set_freq(data[0]);
			}
			else 
			{
				TIM3_PWM_Init(500-1,84-1);
			}

			
			_data[0] = i;
			sendlength = 1;
			
			
		}break;
		
		case QXKG: {
			int i;
			i = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_6);
			_data[0] = i;
			sendlength = 1; 
		}break;
		
		
		case SMG: {

			value89 = data[0];

			_data[0] = 0;
			sendlength = 1;
		}break;
		
		
		case MOTOR: {
			int i;
			i=data[0];
			MOTOR_set(data[0]);			
			_data[0] = i;
			sendlength = 1;
		}break;
		
		case DZP: {		
			DZP_set(data);		
			_data[0] = 0;
			sendlength = 1;
		}break;
		
		case DZP1: {
			DZP1_set(data);
			_data[0] = 0;
			sendlength = 1;
		}break;
	
		case YG: {
			uint8_t x = 0;
			uint8_t y = 0;
			uint8_t z = 0;
				YG_set(&x,&y,&z);

			_data[0] = x;	//x
			_data[1] = y;	//y
			_data[2] = z; //z
			sendlength = 3;
		}break;
		
		case TFTLCD: {
		
			Lcd_Clear(WHITE);
			switch (data[0]){
				case 1:LCD_draw_line(data[1],data[2],data[3],data[4],BLACK);break;
				case 2:LCD_draw_rect(data[1],data[2],data[3],data[4],RED,data[5]);break;
				case 3:LCD_draw_circle(data[1],data[2],data[3],BLACK);break;
				
			}

			_data[0] = 0;
			sendlength = 1;			
			
		}break;
		
		case SOUND_SENSOR: {	
			uint8_t sound_a = 0;
			uint8_t sound_d = 0;
			sound_d = rt_pin_read(SY_D0);
			SOUNDSENSOR_set(&sound_a);


			_data[0] = sound_d;
			_data[1] = sound_a;
			sendlength = 2;		
		}break;
		
		case PIR: {
			uint8_t i;
			i=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);
			_data[0] = i;

			sendlength = 1;
		}break;
		
		case DHT11: {
			uint8_t tem1 = 0,hum1 = 0;

			DHT11_get(&tem1, &hum1);
			_data[0] = tem1;
			_data[1] = hum1;
			sendlength = 2;	
		}break;
		

		
		case DS18B20: {
			uint16_t tem = 0;
			DS18B20_get(&tem);
			_data[0] = (uint8_t)(tem >> 8);
			_data[1] = (uint8_t)tem;
			sendlength = 2;
		}break;
		
		case LIGHT: {
			_data[0] = rt_pin_read(GMDZ);
			sendlength = 1;
		}break;
		
		
		case JJCGQ: {
			uint8_t jiujing_value =0;
			
			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_3,GPIO_PIN_SET);//酒精传感器电源控制端
			
			jiujing_set(&jiujing_value);
			_data[0] = jiujing_value;
			sendlength = 1;
		}break;
		
		
		case ROTARY_ENCODERS: {
			
			uint8_t value = 0;
			XZDWQ_set(&value);
			_data[0] = value;
			sendlength = 1;
		}break;
		
		
		case GDKG: {
			uint8_t value = 0;
			value = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_8);
			_data[0] = value;
			sendlength = 1;
		}break;
		
		case KEY5:{

			_data[0]	= HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_9);
			_data[1]	= HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_8);
			_data[2]	= HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_10);
			_data[3]	= HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_11);
			_data[4]	= HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_12);
			sendlength = 5;
		};break;
		
		case Dial:{
			uint8_t i;
			i = data[0];
			Dial_set(data[0]);
			_data[0] = i;
			sendlength = 1;
		};break;
		
		case traffic_light:{
			traffic_light_set(data[0],data[1],data[2]);
			_data[0] = 0;
			sendlength = 1;
		};break;
		
		case GUI_clear:{
			GUI_set();
			_data[0] = 0;
			sendlength = 1;
		}break;
		
	}

	_send(cmd, _data, sendlength);

	return cmd;
}

void proto_set_sendFunc(proto_send_cb sendfunc)
{
	_proto_send_func = sendfunc;
}


void DZP_deal(void);

static uint8_t dzp_buf[5];
static uint8_t dzp_flag = 0;
void DZP_set(uint8_t *data)
{
	memcpy(dzp_buf, data, 5);
	dzp_flag = 1;
}

void DZP_deal(void)
{
	if (dzp_flag == 1) {
		dzp_flag = 0;
		Init_MAX7219(); 
		Write_Max7219(1, dzp_buf[0]<<1);
		Write_Max7219(2, dzp_buf[1]<<1);
		Write_Max7219(3, dzp_buf[2]<<1);
		Write_Max7219(4, dzp_buf[3]<<1);
		Write_Max7219(5, dzp_buf[4]<<1);
	}
}



static uint8_t dzp1_buf[5];
static uint8_t dzp1_flag = 0;
void DZP1_set(uint8_t *data)
{
	memcpy(dzp1_buf, data, 5);
	dzp1_flag = 1;
}

void DZP1_deal(void)
{
	if (dzp1_flag == 1) {
		  dzp1_flag = 0;
		  Init_DZP1();
			Write_DZP1(1,dzp1_buf[0]<<1);
			Write_DZP1(2,dzp1_buf[1]<<1);
			Write_DZP1(3,dzp1_buf[2]<<1);
			Write_DZP1(4,dzp1_buf[3]<<1);
			Write_DZP1(5,dzp1_buf[4]<<1);
	}
}

static uint8_t speed;
static uint8_t motor_flag = 0;
void MOTOR_set(uint8_t data)
{
	//memcpy(speed, data, 1);
	speed = data;
	motor_flag = 1;
}

void MOTOR_deal(void)
{
		if(motor_flag == 1){
			if(speed){
				TIM_SetTIM4Compare1(speed);
			}
			else {
				TIM4_PWM_Init(500-1,84-1);//MADA
			}
		motor_flag = 0;
	}
}

//码表显示控件stm32处理
static uint8_t value;
static uint8_t dial_flag = 0;
void Dial_set(uint8_t data)
{
	value = data;
	dial_flag = 1;
}

void Dial_deal(void)
{
	if(dial_flag == 1)
	{
		dwq_task();
		dial_flag = 0;
	}
}

uint8_t Dial_value_get(void)
{
	return value;
	
}

static uint8_t tem1 = 0;
static uint8_t hum1 = 0;

uint8_t DHT11_get(uint8_t *tem, uint8_t *hum)
{
	*tem = tem1;
	*hum = hum1;
	
	return 0;
}

void DHT11_deal(void)
{
	static uint32_t dht11_tick = 0;
	
	dht11_tick++;
	if (dht11_tick >= 200) {		//2s
		dht11_tick = 0;
		rt_enter_critical();
		//DHT11_Init();
		DHT11_Read_Data(&tem1,&hum1);

		rt_exit_critical();
	}
}


static uint16_t tem = 0;

uint8_t DS18B20_get(uint16_t *temp)
{
	*temp = tem;
	
	return 0;
}

void DS18B20_deal(void)
{
	static uint32_t ds18b20_tick = 0;
	
	ds18b20_tick++;
	if(ds18b20_tick >=200) {
		ds18b20_tick = 0;
		tem=DS18B20_Get_Temp();
	}
}


static uint8_t x = 0;
static uint8_t y = 0;
static uint8_t z = 0;
//static uint8_t yg_flag = 0;
uint8_t YG_set(uint8_t *x_value,uint8_t *y_value,uint8_t *z_value)
{
	*x_value = x;
	*y_value = y;
	*z_value = z;
	return 0;
}

void YG_deal(void)
{
		x = Get_Adc1(YG_X_ADC_CH);	//x
		y= Get_Adc1(YG_Y_ADC_CH);	//y
		z =	rt_pin_read(YG_KEY);//z
	
}

static uint8_t jiujing_value = 0;
uint8_t jiujing_set(uint8_t *data)
{
	*data = jiujing_value;
	return 0;
}

void jiujing_deal(void)
{
		
		
		jiujing_value = Get_Adc3(ADC_CHANNEL_15);
}

static uint8_t xzdwq_value = 0;
uint8_t XZDWQ_set(uint8_t *data)
{
	*data = xzdwq_value;
	return 0;
}

void XZDWQ_deal(void)
{
		xzdwq_value = Get_Adc1(XZHDWQ_ADC_CH);
}


static uint8_t sound = 0;
uint8_t SOUNDSENSOR_set(uint8_t * data)
{
	*data = sound;
	return 0;
}
void SOUNDSENSOR_deal(void)
{
	static uint32_t sound_tick = 0;
	
	sound_tick ++;
	if(sound_tick >=200){
		sound_tick = 0;
		sound = Get_Adc1(SY_ADC_CH);
	}
}
static uint8_t Clear_flag = 0;
static uint8_t r = 0;
static uint8_t yellow = 0;
static uint8_t g = 0;;
static uint8_t traffic_light_flag = 0;
void traffic_light_set(uint8_t color_r,uint8_t color_y,uint8_t color_g)
{
	r = color_r;
	yellow = color_y;
	g = color_g;

	traffic_light_flag = 1;
}

void traffic_deal(void)
{
	
	
	if(traffic_light_flag == 1)
	{
			if(Clear_flag == 1)
		{
			Clear_flag = 0;
			GUI_SetBkColor(GUI_WHITE);  
			GUI_Clear();
			GUI_SetColor(GUI_BLUE);
			GUI_FillRoundedRect(30, 80, 209, 160, 10);
		}
		
		Traffic_light(r,yellow,g);
		traffic_light_flag = 0;
	}
}


static uint8_t GUI_flag = 0;
void GUI_set(void)
{
	GUI_flag = 1;
	Clear_flag = 1;
}

void GUI_deal(void)
{
	if(GUI_flag == 1)
	{
		GUI_Init();
		GUI_SetBkColor(GUI_BLACK); 
		GUI_Clear();
		GUI_flag = 0;
	}
}



#if defined(PROTO_DEBUG)


#include "unity.h"
#include "unity_fixture.h"

uint8_t recv_RGB_LED[] = {0x5a,0x0f,1,1,1,1,1,0,0,0,1,1,1,0xff,0xa5};
uint8_t send_RGB_LED[] = {0x5a,0x07,0x81,0x01,0x01,0xff,0xa5};

uint8_t recv_PASSIVE_BUZZER[] = {0x5a,0x07,1,2,100,0xff,0xa5};
uint8_t send_PASSIVE_BUZZER[] = {0x5a,0x07,0x81,0x02,0x01,0xff,0xa5};

uint8_t recv_LED[] = {0x5a,0x07,1,3,1,0xff,0xa5};
uint8_t send_LED[] = {0x5a,0x07,0x81,0x03,0x01,0xff,0xa5};

uint8_t recv_SMG[] = {0x5a,0x08,1,4,1,111,0xff,0xa5};
uint8_t send_SMG[] = {0x5a,0x07,0x81,0x04,0x01,0xff,0xa5};

uint8_t recv_MOTOR[] = {0x5a,0x07,1,5,100,0xff,0xa5};
uint8_t send_MOTOR[] = {0x5a,0x07,0x81,0x05,0x01,0xff,0xa5};

uint8_t recv_DZP[] = {0x5a,0x0b,1,6,1,2,3,4,5,0xff,0xa5};
uint8_t send_DZP[] = {0x5a,0x07,0x81,0x06,0x01,0xff,0xa5};

uint8_t recv_YG[] = {0x5a,0x06,1,7,0xff,0xa5};
uint8_t send_YG[] = {0x5a,0x09,0x81,0x07,0,0,0,0xff,0xa5};

//uint8_t recv_TFTLCD[] = {0x5a,0x0f,1,0,1,1,1,0,0,0,1,1,1,0xff,0xa5};
uint8_t recv_SOUND_SENSOR[] = {0x5a,0x06,1,9,0xff,0xa5};
uint8_t send_SOUND_SENSOR[] = {0x5a,0x08,0x81,0x09,0,0,0xff,0xa5};

uint8_t recv_PIR[] = {0x5a,0x06,1,10,0xff,0xa5};
uint8_t send_PIR[] = {0x5a,0x07,0x81,0x0a,0,0xff,0xa5};

uint8_t recv_DHT11[] = {0x5a,0x06,1,11,0xff,0xa5};
uint8_t send_DHT11[] = {0x5a,0x08,0x81,0x0b,0,0,0xff,0xa5};

uint8_t recv_DS18B20[] = {0x5a,0x06,1,12,0xff,0xa5};
uint8_t send_DS18B20[] = {0x5a,0x07,0x81,0x0c,0x00,0xff,0xa5};

uint8_t recv_GMDZ[] = {0x5a,0x06,1,13,0xff,0xa5};
uint8_t send_GMDZ[] = {0x5a,0x07,0x81,0x0d,0x00,0xff,0xa5};

uint8_t recv_JJCGQ[] = {0x5a,0x06,1,14,0xff,0xa5};
uint8_t send_JJCGQ[] = {0x5a,0x07,0x81,0x0e,0,0xff,0xa5};

uint8_t recv_ROTARY_ENCODERS[] = {0x5a,0x06,1,15,0xff,0xa5};
uint8_t send_ROTARY_ENCODERS[] = {0x5a,0x07,0x81,0x0f,0,0xff,0xa5};

uint8_t recv_SETPPER_MOTOR[] = {0x5a,0x07,1,16,100,0xff,0xa5};
uint8_t send_SETPPER_MOTOR[] = {0x5a,0x07,0x81,0x10,1,0xff,0xa5};



int data_recv(uint8_t *data, uint8_t size)
{
	Proto_data_analyze(data, size);
	Proto_sensor_cmd_deal();
	return 0;
}

int data_recv_loop(uint8_t *data, uint8_t size)
{
	int i=0;
	for (;i<size;i++) {
		if (ProtoProto_data_analyze_byte(data[i]) ) {
			Proto_sensor_cmd_deal();
			return 1;
		}
	}
	return 0;
}


TEST_GROUP( proto_test );

TEST_SETUP( proto_test )
{
}

TEST_TEAR_DOWN( proto_test )
{
}

TEST( proto_test, recv_send)
{
//    uint8_t testSendBuf[] = {0xaa, 20, 0, 0, 1, 5,0,0,0, 1,2,3,4,5, 15, 0,0,0,0, 0x55};
//
//    TEST_ASSERT_EQUAL(20, AUE_Receive_data.pkg_size);
//    TEST_ASSERT_EQUAL(0x0100, AUE_Receive_data.cmd);
//    TEST_ASSERT_EQUAL(5, AUE_Receive_data.databuf_size);
//    TEST_ASSERT_EQUAL_MEMORY(databuf, AUE_Receive_data.databuf, 5);

//		TEST_ASSERT_EQUAL(DB_ok,  PasswordDB_GetAdminEmptyID(&id));

	data_recv(recv_RGB_LED, sizeof(recv_RGB_LED));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_RGB_LED, test_length);

	data_recv(recv_PASSIVE_BUZZER, sizeof(recv_PASSIVE_BUZZER));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_PASSIVE_BUZZER, test_length);

	data_recv(recv_LED, sizeof(recv_LED));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_LED, test_length);

	data_recv(recv_SMG, sizeof(recv_SMG));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_SMG, test_length);

	data_recv(recv_MOTOR, sizeof(recv_MOTOR));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_MOTOR, test_length);

	data_recv(recv_DZP, sizeof(recv_DZP));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_DZP, test_length);

	data_recv(recv_YG, sizeof(recv_YG));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_YG, test_length);

	data_recv(recv_SOUND_SENSOR, sizeof(recv_SOUND_SENSOR));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_SOUND_SENSOR, test_length);

	data_recv(recv_PIR, sizeof(recv_PIR));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_PIR, test_length);

	data_recv(recv_DHT11, sizeof(recv_DHT11));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_DHT11, test_length);

	data_recv(recv_DS18B20, sizeof(recv_DS18B20));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_DS18B20, test_length);

	data_recv(recv_GMDZ, sizeof(recv_GMDZ));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_GMDZ, test_length);

	data_recv(recv_JJCGQ, sizeof(recv_JJCGQ));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_JJCGQ, test_length);

	data_recv(recv_ROTARY_ENCODERS, sizeof(recv_ROTARY_ENCODERS));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_ROTARY_ENCODERS, test_length);

	data_recv(recv_SETPPER_MOTOR, sizeof(recv_SETPPER_MOTOR));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_SETPPER_MOTOR, test_length);

}

TEST( proto_test, recv_send_byte)
{

	data_recv_loop(recv_RGB_LED, sizeof(recv_RGB_LED));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_RGB_LED, test_length);

	data_recv_loop(recv_PASSIVE_BUZZER, sizeof(recv_PASSIVE_BUZZER));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_PASSIVE_BUZZER, test_length);

	data_recv_loop(recv_LED, sizeof(recv_LED));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_LED, test_length);

	data_recv_loop(recv_SMG, sizeof(recv_SMG));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_SMG, test_length);

	data_recv_loop(recv_MOTOR, sizeof(recv_MOTOR));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_MOTOR, test_length);

	data_recv_loop(recv_DZP, sizeof(recv_DZP));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_DZP, test_length);

	data_recv_loop(recv_YG, sizeof(recv_YG));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_YG, test_length);

	data_recv_loop(recv_SOUND_SENSOR, sizeof(recv_SOUND_SENSOR));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_SOUND_SENSOR, test_length);

	data_recv_loop(recv_PIR, sizeof(recv_PIR));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_PIR, test_length);

	data_recv_loop(recv_DHT11, sizeof(recv_DHT11));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_DHT11, test_length);

	data_recv_loop(recv_DS18B20, sizeof(recv_DS18B20));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_DS18B20, test_length);

	data_recv_loop(recv_GMDZ, sizeof(recv_GMDZ));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_GMDZ, test_length);

	data_recv_loop(recv_JJCGQ, sizeof(recv_JJCGQ));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_JJCGQ, test_length);

	data_recv_loop(recv_ROTARY_ENCODERS, sizeof(recv_ROTARY_ENCODERS));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_ROTARY_ENCODERS, test_length);

	data_recv_loop(recv_SETPPER_MOTOR, sizeof(recv_SETPPER_MOTOR));
	TEST_ASSERT_EQUAL_MEMORY(test_sendbuf, send_SETPPER_MOTOR, test_length);

}


TEST_GROUP_RUNNER(proto_test)
{
	RUN_TEST_CASE(proto_test, recv_send);
	RUN_TEST_CASE(proto_test, recv_send_byte);
}


void RunTests_proto(void)
{
	RUN_TEST_GROUP(proto_test);
}

#endif

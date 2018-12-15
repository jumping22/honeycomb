#include "my_exti.h"
#include <drivers/pin.h>
/*4个独立按键*/
#define KEY1	50	//F12
#define KEY2	53	//F13
#define KEY3	54	//F14
#define KEY4	55	//F15
/*模拟遥感的一个按键与5向遥感*/
#define YG_KEY 93 //G8
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
#define RTHW 67 //B15
/*光敏电阻*/
#define GMDZ     19  //F7
/*酒精传感器*/
#define JIUJING  21  //F9
/******************遥感按键******************/
void YG_KEY_callback(void *args)//回调函数
{
    char *a = args;//获取参数
    rt_kprintf("YG_KEY %s\n",a);
}

void YG_KEY_irq_thread_entry(void* parameter)
{    
    //上拉输入
    rt_pin_mode(YG_KEY, PIN_MODE_INPUT_PULLUP);
    //绑定中断，下降沿模式，回调函数名为
    rt_pin_attach_irq(YG_KEY, PIN_IRQ_MODE_FALLING, YG_KEY_callback, (void*)"YG_KEY_callback args");
    //使能中断
    rt_pin_irq_enable(YG_KEY, PIN_IRQ_ENABLE);    
}

/******************独立按键1******************/
void KEY1_callback(void *args)//回调函数
{
    char *a = args;//获取参数
				rt_kprintf("KEY1 %s\n",a);
}

void KEY1_irq_thread_entry(void* parameter)
{    
    //上拉输入
    rt_pin_mode(KEY1, PIN_MODE_INPUT_PULLUP);
    //绑定中断，下降沿模式，回调函数名为
    rt_pin_attach_irq(KEY1, PIN_IRQ_MODE_FALLING, KEY1_callback, (void*)"KEY1_callback args");
    //使能中断
    rt_pin_irq_enable(KEY1, PIN_IRQ_ENABLE); 
}
/******************独立按键2******************/
void KEY2_callback(void *args)//回调函数
{
    char *a = args;//获取参数
    rt_kprintf("KEY2 %s\n",a);
}

void KEY2_irq_thread_entry(void* parameter)
{    
    //上拉输入
    rt_pin_mode(KEY2, PIN_MODE_INPUT_PULLUP);
    //绑定中断，下降沿模式，回调函数名为
    rt_pin_attach_irq(KEY2, PIN_IRQ_MODE_FALLING, KEY2_callback, (void*)"KEY2_callback args");
    //使能中断
    rt_pin_irq_enable(KEY2, PIN_IRQ_ENABLE);    
}

/******************独立按键3******************/
void KEY3_callback(void *args)//回调函数
{
    char *a = args;//获取参数
    rt_kprintf("KEY3 %s\n",a);
}

void KEY3_irq_thread_entry(void* parameter)
{    
    //上拉输入
    rt_pin_mode(KEY3, PIN_MODE_INPUT_PULLUP);
    //绑定中断，下降沿模式，回调函数名为
    rt_pin_attach_irq(KEY3, PIN_IRQ_MODE_FALLING, KEY3_callback, (void*)"KEY3_callback args");
    //使能中断
    rt_pin_irq_enable(KEY3, PIN_IRQ_ENABLE);    
}
/******************独立按键4******************/
void KEY4_callback(void *args)//回调函数
{
    char *a = args;//获取参数
    rt_kprintf("KEY4 %s\n",a);
}

void KEY4_irq_thread_entry(void* parameter)
{    
    //上拉输入
    rt_pin_mode(KEY4, PIN_MODE_INPUT_PULLUP);
    //绑定中断，下降沿模式，回调函数名为
    rt_pin_attach_irq(KEY4, PIN_IRQ_MODE_FALLING, KEY4_callback, (void*)"KEY4_callback args");
    //使能中断
    rt_pin_irq_enable(KEY4, PIN_IRQ_ENABLE);    
}

/*******5向遥感按键1与模拟遥感共用****************/
/******************遥感按键2******************/
void YG_5SW_KEY2_callback(void *args)//回调函数
{
    char *a = args;//获取参数
    rt_kprintf("YG_5SW_KEY2 %s\n",a);
}

void YG_5SW_KEY2_irq_thread_entry(void* parameter)
{    
    //上拉输入
    rt_pin_mode(YG_5SW_KEY2, PIN_MODE_INPUT_PULLUP);
    //绑定中断，下降沿模式，回调函数名为
    rt_pin_attach_irq(YG_5SW_KEY2, PIN_IRQ_MODE_FALLING, YG_5SW_KEY2_callback, (void*)"YG_5SW_KEY2_callback args");
    //使能中断
    rt_pin_irq_enable(YG_5SW_KEY2, PIN_IRQ_ENABLE);    
}
/******************遥感按键3******************/
void YG_5SW_KEY3_callback(void *args)//回调函数
{
    char *a = args;//获取参数
    rt_kprintf("YG_5SW_KEY3 %s\n",a);
}

void YG_5SW_KEY3_irq_thread_entry(void* parameter)
{    
    //上拉输入
    rt_pin_mode(YG_5SW_KEY3, PIN_MODE_INPUT_PULLUP);
    //绑定中断，下降沿模式，回调函数名为
    rt_pin_attach_irq(YG_5SW_KEY3, PIN_IRQ_MODE_FALLING, YG_5SW_KEY3_callback, (void*)"YG_5SW_KEY2_callback args");
    //使能中断
    rt_pin_irq_enable(YG_5SW_KEY3, PIN_IRQ_ENABLE);    
}
/******************遥感按键4******************/
void YG_5SW_KEY4_callback(void *args)//回调函数
{
    char *a = args;//获取参数
    rt_kprintf("YG_5SW_KEY4 %s\n",a);
}

void YG_5SW_KEY4_irq_thread_entry(void* parameter)
{    
    //上拉输入
    rt_pin_mode(YG_5SW_KEY4, PIN_MODE_INPUT_PULLUP);
    //绑定中断，下降沿模式，回调函数名为
    rt_pin_attach_irq(YG_5SW_KEY4, PIN_IRQ_MODE_FALLING, YG_5SW_KEY4_callback, (void*)"YG_5SW_KEY4_callback args");
    //使能中断
    rt_pin_irq_enable(YG_5SW_KEY4, PIN_IRQ_ENABLE);    
}

/******************遥感按键5******************/
void YG_5SW_KEY5_callback(void *args)//回调函数
{
    char *a = args;//获取参数
    rt_kprintf("YG_5SW_KEY5 %s\n",a);
}

void YG_5SW_KEY5_irq_thread_entry(void* parameter)
{    
    //上拉输入
    rt_pin_mode(YG_5SW_KEY5, PIN_MODE_INPUT_PULLUP);
    //绑定中断，下降沿模式，回调函数名为
    rt_pin_attach_irq(YG_5SW_KEY5, PIN_IRQ_MODE_FALLING, YG_5SW_KEY5_callback, (void*)"YG_5SW_KEY5_callback args");
    //使能中断
    rt_pin_irq_enable(YG_5SW_KEY5, PIN_IRQ_ENABLE);    
}
/******************人体红外******************/
void RTHW_callback(void *args)//回调函数
{
    char *a = args;//获取参数
    rt_kprintf("RTHW %s\n",a);
}

void RTHW_irq_thread_entry(void* parameter)
{    
    //上拉输入
    rt_pin_mode(RTHW, PIN_MODE_INPUT_PULLUP);
    //绑定中断，下降沿模式，回调函数名为
    rt_pin_attach_irq(RTHW, PIN_IRQ_MODE_FALLING, RTHW_callback, (void*)"RTHW_callback args");
    //使能中断
    rt_pin_irq_enable(RTHW, PIN_IRQ_ENABLE);    
}
/******************声音传感器数字口******************/
void SY_D0_callback(void *args)//回调函数
{
    char *a = args;//获取参数
    rt_kprintf("SY_D0%s\n",a);
}

void SY_D0_irq_thread_entry(void* parameter)
{    
    //上拉输入
    rt_pin_mode(SY_D0, PIN_MODE_INPUT_PULLUP);
    //绑定中断，下降沿模式，回调函数名为
    rt_pin_attach_irq(SY_D0, PIN_IRQ_MODE_FALLING, SY_D0_callback, (void*)"SY_D0_callback args");
    //使能中断
    rt_pin_irq_enable(SY_D0, PIN_IRQ_ENABLE);  	
}
/******************光敏电阻******************/
void GMDZ_callback(void *args)//回调函数
{
    char *a = args;//获取参数
    rt_kprintf("GMDZ%s\n",a);
}

void GMDZ_irq_thread_entry(void* parameter)
{    
    //上拉输入
    rt_pin_mode(GMDZ, PIN_MODE_INPUT_PULLUP);
    //绑定中断，下降沿模式，回调函数名为
    rt_pin_attach_irq(GMDZ, PIN_IRQ_MODE_FALLING, GMDZ_callback, (void*)"GMDZ_callback args");
    //使能中断
    rt_pin_irq_enable(GMDZ, PIN_IRQ_ENABLE);  	
}

//void man_test(void)
//{    
//	/* 创建irq线程（遥感按键）*/
//    YG_KEY_LIN = rt_thread_create("YG_KEY_irq_thread_entry",
//                    YG_KEY_irq_thread_entry, 
//                    RT_NULL,
//                    1024, 
//                    4, 
//                    10);
//    if (YG_KEY_LIN != RT_NULL)
//        rt_thread_startup(YG_KEY_LIN);
//	/* 创建irq线程（独立按键1） */
//	KEY1_LIN = rt_thread_create("KEY1_irq_thread_entry",
//                    KEY1_irq_thread_entry, 
//                    RT_NULL,
//                    1024, 
//                    4, 
//                    10);
//    if (KEY1_LIN != RT_NULL)
//        rt_thread_startup(KEY1_LIN);
//	/* 创建irq线程（独立按键2） */
//	KEY2_LIN = rt_thread_create("KEY2_irq_thread_entry",
//                    KEY2_irq_thread_entry, 
//                    RT_NULL,
//                    1024, 
//                    4, 
//                    10);
//    if (KEY2_LIN != RT_NULL)
//        rt_thread_startup(KEY2_LIN);
//	/* 创建irq线程（独立按键3） */
//	KEY3_LIN = rt_thread_create("KEY3_irq_thread_entry",
//                    KEY3_irq_thread_entry, 
//                    RT_NULL,
//                    1024, 
//                    4, 
//                    10);
//    if (KEY3_LIN != RT_NULL)
//        rt_thread_startup(KEY3_LIN);
//	/* 创建irq线程（独立按键4） */
//	KEY4_LIN = rt_thread_create("KEY4_irq_thread_entry",
//                    KEY4_irq_thread_entry, 
//                    RT_NULL,
//                    1024, 
//                    4, 
//                    10);
//    if (KEY4_LIN != RT_NULL)
//        rt_thread_startup(KEY4_LIN);
//	/****5向遥感按键1与模拟遥感共用***/
//	/* 创建irq线程（5向遥感按键2） */
//	YG_5SW_KEY2_LIN = rt_thread_create("YG_5SW_KEY2_irq_thread_entry",
//                    YG_5SW_KEY2_irq_thread_entry, 
//                    RT_NULL,
//                    1024, 
//                    4, 
//                    10);
//    if (YG_5SW_KEY2_LIN != RT_NULL)
//        rt_thread_startup(YG_5SW_KEY2_LIN);	
//	/* 创建irq线程（5向遥感按键3） */
//	YG_5SW_KEY3_LIN = rt_thread_create("YG_5SW_KEY3_irq_thread_entry",
//                    YG_5SW_KEY3_irq_thread_entry, 
//                    RT_NULL,
//                    1024, 
//                    4, 
//                    10);
//    if (YG_5SW_KEY3_LIN != RT_NULL)
//        rt_thread_startup(YG_5SW_KEY3_LIN);	
//	/* 创建irq线程（5向遥感按键4） */
//	YG_5SW_KEY4_LIN = rt_thread_create("YG_5SW_KEY4_irq_thread_entry",
//                    YG_5SW_KEY4_irq_thread_entry, 
//                    RT_NULL,
//                    1024, 
//                    4, 
//                    10);
//    if (YG_5SW_KEY4_LIN != RT_NULL)
//        rt_thread_startup(YG_5SW_KEY4_LIN);	
//	/* 创建irq线程（5向遥感按键5） */
//	YG_5SW_KEY5_LIN = rt_thread_create("YG_5SW_KEY5_irq_thread_entry",
//                    YG_5SW_KEY5_irq_thread_entry, 
//                    RT_NULL,
//                    1024, 
//                    4, 
//                    10);
//    if (YG_5SW_KEY5_LIN != RT_NULL)
//        rt_thread_startup(YG_5SW_KEY5_LIN);	
//	/* 创建irq线程（人体红外） */
//	RTHW_LIN = rt_thread_create("RTHW_irq_thread_entry",
//                    RTHW_irq_thread_entry, 
//                    RT_NULL,
//                    1024, 
//                    4, 
//                    10);
//    if (RTHW_LIN != RT_NULL)
//        rt_thread_startup(RTHW_LIN);
//	/* 创建irq线程（声音传感器D0） */
//	SY_D0_LIN = rt_thread_create("SY_D0_irq_thread_entry",
//                    SY_D0_irq_thread_entry, 
//                    RT_NULL,
//                    1024, 
//                    4, 
//                    10);
//    if (SY_D0_LIN != RT_NULL)
//        rt_thread_startup(SY_D0_LIN);
//	/* 创建irq线程（光敏电阻） */
//	GMDZ_LIN = rt_thread_create("GMDZ_irq_thread_entry",
//                    GMDZ_irq_thread_entry, 
//                    RT_NULL,
//                    1024, 
//                    4, 
//                    10);
//    if (GMDZ_LIN != RT_NULL)
//        rt_thread_startup(GMDZ_LIN);		
//}

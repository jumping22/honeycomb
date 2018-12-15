/*
 * File      : main.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-07-29     Arda.Fu      first implementation
 */
#include <rtthread.h>
#include <board.h>

/* dfs filesystem:ELM filesystem init */
#include <dfs_elm.h>
/* dfs Filesystem APIs */
#include <dfs_fs.h>
#include <dfs_posix.h>

//#include <easyflash.h>
#include <rthw.h>
#include "proto.h"


//#include "my_exti.h"
#include "ST7735R.h"
#include "max7219.h"
#include "timer.h"
#include "adc.h"
#include "dht11.h"
#include "ds18b20.h"
#include "define.h"
#include "CGQ.h"
#include "dzp1.h"
#include "SMG.h"
#include "GUI.h"
#include "CGQ.h"




static struct rt_thread mega32u4_uart;
ALIGN(4)
static rt_uint8_t mega32u4_uart_thread_stack[4096];
static void mega32u4_uart_thread(void* parameter);



void proto_init(void);
void idle_hook_function(void);
void vPortSetupTimerInterrupt( void );
void vPortSuppressTicksAndSleep( uint32_t xExpectedIdleTime );

void LCD_SHOWTITLE(void);
void Light_Pin_Init(void);
void Exti_Pin_Init(void);
void Light_thread_entry(void* parameter);
uint8_t CGQ_Read(rt_base_t cgq);
void CGQ_Test(void* parameter);
void CGQ_ADC_Test(void* parameter);
void WD_R_Test(void* parameter);
void PWM_Test(void* parameter);
void LCD_Test(void* parameter);
void fmq_init(void);
void mada_gpio_init(void);
void dianji_gpio_init(void);
void led_init(void);
void YG_KEY_gpio_init(void);
void LIGHT_CLR(void);
void CGQ_gpio_init(void);

#include "stm32f4xx_hal.h"
void StandbyMode_Measure(void)
{

  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);

  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

  HAL_PWR_EnterSTANDBYMode();

}

extern int multi_button_test(void);


void YG_KEY_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
 
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	/*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
}


void led_init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
 
	__HAL_RCC_GPIOF_CLK_ENABLE();
	
	/*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	
}

void dianji_gpio_init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
 
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	/*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	 GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	 GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}




void idle_hook_function(void)
{
	struct rt_object_information *information;
	struct rt_object *object;
	struct rt_list_node *node;
	uint32_t xExpectedIdleTime = 0;
	uint32_t next_timeout = 0;
	
	/* enter critical */
	if (rt_thread_self() != RT_NULL)
			rt_enter_critical();

	/* 循环遍历 寻找Ready最短时间 或 Suspend最短时间 */
	information = rt_object_get_information(RT_Object_Class_Thread);
	RT_ASSERT(information != RT_NULL);
	for (node  = information->object_list.next;
			 node != &(information->object_list);
			 node  = node->next)
	{
		object = rt_list_entry(node, struct rt_object, list);
		
		if (RT_THREAD_READY == ((rt_thread_t)object)->stat) {
			//if (rt_strncmp(object->name, "tidle", RT_NAME_MAX) != 0) {
				//rt_kprintf("Ready name:%s, left tick:%d, state: %d\r\n", object->name, ((rt_thread_t)object)->remaining_tick, ((rt_thread_t)object)->stat);
				if (((rt_thread_t)object)->remaining_tick > 0) {
					if (xExpectedIdleTime == 0) {
						xExpectedIdleTime = ((rt_thread_t)object)->remaining_tick;
					} else {	//寻找最小
						xExpectedIdleTime = ((rt_thread_t)object)->remaining_tick < xExpectedIdleTime? ((rt_thread_t)object)->remaining_tick:xExpectedIdleTime;
					}
				}
			//}
		} else if(RT_THREAD_SUSPEND == ((rt_thread_t)object)->stat) {
			//if (rt_strncmp(object->name, "tidle", RT_NAME_MAX) != 0) {
				//rt_kprintf("Suspend name:%s, left tick:%d, state: %d\r\n", object->name, ((rt_thread_t)object)->remaining_tick, ((rt_thread_t)object)->stat);
				if (((rt_thread_t)object)->thread_timer.timeout_tick > 0) {
					if (xExpectedIdleTime == 0) {
						xExpectedIdleTime = ((rt_thread_t)object)->thread_timer.timeout_tick - rt_tick_get();
					} else {
						uint32_t _idletime = ((rt_thread_t)object)->thread_timer.timeout_tick - rt_tick_get();
						xExpectedIdleTime = _idletime < xExpectedIdleTime? _idletime:xExpectedIdleTime;
					}
				}
			//}
		}
	}

	next_timeout = rt_timer_next_timeout_tick();
	if (xExpectedIdleTime > 10) {
		//rt_kprintf("Suspend name:%s, left tick:%d, state: %d\r\n", object->name, ((rt_thread_t)object)->remaining_tick, ((rt_thread_t)object)->stat);
		//rt_kprintf("enter tick:%d\r\n", rt_tick_get());
		rt_enter_critical();
		vPortSuppressTicksAndSleep(xExpectedIdleTime);
		rt_exit_critical();
		//rt_kprintf("exit tick:%d\r\n", rt_tick_get());
	}
	
//	for (node  = information->object_list.next;
//			 node != &(information->object_list);
//			 node  = node->next)
//	{
//		object = rt_list_entry(node, struct rt_object, list);
//		
//		if (RT_THREAD_READY == ((rt_thread_t)object)->stat) {
//			if (rt_strncmp(object->name, "tidle", RT_NAME_MAX) != 0) {
//				//rt_kprintf("Ready name:%s, left tick:%d, state: %d\r\n", object->name, ((rt_thread_t)object)->remaining_tick, ((rt_thread_t)object)->stat);
//				xExpectedIdleTime = ((rt_thread_t)object)->remaining_tick;
//			}
//		} else if(RT_THREAD_SUSPEND == ((rt_thread_t)object)->stat) {
//			if (rt_strncmp(object->name, "tidle", RT_NAME_MAX) != 0) {
//				//rt_kprintf("Suspend name:%s, left tick:%d, state: %d\r\n", object->name, ((rt_thread_t)object)->remaining_tick, ((rt_thread_t)object)->stat);
//				xExpectedIdleTime = ((rt_thread_t)object)->thread_timer.timeout_tick > 0? ((rt_thread_t)object)->thread_timer.timeout_tick - rt_tick_get():0;
//				
//				if (xExpectedIdleTime > 10) {
//					//rt_kprintf("Suspend name:%s, left tick:%d, state: %d\r\n", object->name, ((rt_thread_t)object)->remaining_tick, ((rt_thread_t)object)->stat);
//					//rt_kprintf("enter tick:%d\r\n", rt_tick_get());
//					rt_enter_critical();
//					vPortSuppressTicksAndSleep(xExpectedIdleTime);
//					rt_exit_critical();
//					//rt_kprintf("exit tick:%d\r\n", rt_tick_get());
//				}
//				
//			}
//		}
//	}
	
	/* leave critical */
	if (rt_thread_self() != RT_NULL)
			rt_exit_critical();
	
}



/***********************************FreeRTOS tickless移植**************************************/
#define portNVIC_SYSTICK_CTRL_REG			( * ( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG			( * ( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG	( * ( ( volatile uint32_t * ) 0xe000e018 ) )
#define portNVIC_SYSPRI2_REG				( * ( ( volatile uint32_t * ) 0xe000ed20 ) )
/* ...then bits in the registers. */
#define portNVIC_SYSTICK_INT_BIT			( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT			( 1UL << 0UL )
#define portNVIC_SYSTICK_COUNT_FLAG_BIT		( 1UL << 16UL )
#define portNVIC_PENDSVCLEAR_BIT 			( 1UL << 27UL )
#define portNVIC_PEND_SYSTICK_CLEAR_BIT		( 1UL << 25UL )

#define portNVIC_SYSTICK_CLK_BIT	( 1UL << 2UL )

#define configCPU_CLOCK_HZ			( SystemCoreClock )
#define configSYSTICK_CLOCK_HZ		configCPU_CLOCK_HZ
#define configTICK_RATE_HZ				(RT_TICK_PER_SECOND)
#define portMAX_24_BIT_NUMBER		( 0xffffffUL )
#define portMISSED_COUNTS_FACTOR	( 45UL )				//?

static uint32_t ulTimerCountsForOneTick = 0;
static uint32_t xMaximumPossibleSuppressedTicks = 0;
static uint32_t ulStoppedTimerCompensation = 0;


void vPortSetupTimerInterrupt( void )
{
	ulTimerCountsForOneTick = ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ );		//168000
	xMaximumPossibleSuppressedTicks = portMAX_24_BIT_NUMBER / ulTimerCountsForOneTick;
	ulStoppedTimerCompensation = portMISSED_COUNTS_FACTOR / ( configCPU_CLOCK_HZ / configSYSTICK_CLOCK_HZ );

	/* Configure SysTick to interrupt at the requested rate. */
	portNVIC_SYSTICK_LOAD_REG = ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
	portNVIC_SYSTICK_CTRL_REG = ( portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT );
}

#define portSY_FULL_READ_WRITE		( 15 )

#define configPRE_SLEEP_PROCESSING                        
#define configPOST_SLEEP_PROCESSING                       


void vPortSuppressTicksAndSleep( uint32_t xExpectedIdleTime )
{
	uint32_t ulReloadValue, ulCompleteTickPeriods, ulCompletedSysTickDecrements, ulSysTickCTRL;
	uint32_t xModifiableIdleTime;

	/* Make sure the SysTick reload value does not overflow the counter. */
	if( xExpectedIdleTime > xMaximumPossibleSuppressedTicks )
	{
		xExpectedIdleTime = xMaximumPossibleSuppressedTicks;
	}

	/* Stop the SysTick momentarily.  The time the SysTick is stopped for
	is accounted for as best it can be, but using the tickless mode will
	inevitably result in some tiny drift of the time maintained by the
	kernel with respect to calendar time. */
	portNVIC_SYSTICK_CTRL_REG &= ~portNVIC_SYSTICK_ENABLE_BIT;

	/* Calculate the reload value required to wait xExpectedIdleTime
	tick periods.  -1 is used because this code will execute part way
	through one of the tick periods. */
	ulReloadValue = portNVIC_SYSTICK_CURRENT_VALUE_REG + ( ulTimerCountsForOneTick * ( xExpectedIdleTime - 1UL ) );
	if( ulReloadValue > ulStoppedTimerCompensation )
	{
		ulReloadValue -= ulStoppedTimerCompensation;
	}

	/* Enter a critical section but don't use the taskENTER_CRITICAL()
	method as that will mask interrupts that should exit sleep mode. */
	__disable_irq();
	__dsb( portSY_FULL_READ_WRITE );
	__isb( portSY_FULL_READ_WRITE );

	/* If a context switch is pending or a task is waiting for the scheduler
	to be unsuspended then abandon the low power entry. */
	if(0)	//if( eTaskConfirmSleepModeStatus() == eAbortSleep )
	{
		/* Restart from whatever is left in the count register to complete
		this tick period. */
		portNVIC_SYSTICK_LOAD_REG = portNVIC_SYSTICK_CURRENT_VALUE_REG;

		/* Restart SysTick. */
		portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;

		/* Reset the reload register to the value required for normal tick
		periods. */
		portNVIC_SYSTICK_LOAD_REG = ulTimerCountsForOneTick - 1UL;

		/* Re-enable interrupts - see comments above __disable_irq() call
		above. */
		__enable_irq();
	}
	else
	{
		/* Set the new reload value. */
		portNVIC_SYSTICK_LOAD_REG = ulReloadValue;

		/* Clear the SysTick count flag and set the count value back to
		zero. */
		portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;

		/* Restart SysTick. */
		portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;

		/* Sleep until something happens.  configPRE_SLEEP_PROCESSING() can
		set its parameter to 0 to indicate that its implementation contains
		its own wait for interrupt or wait for event instruction, and so wfi
		should not be executed again.  However, the original expected idle
		time variable must remain unmodified, so a copy is taken. */
		xModifiableIdleTime = xExpectedIdleTime;
		configPRE_SLEEP_PROCESSING(&xModifiableIdleTime);
		if( xModifiableIdleTime > 0 )
		{
			__dsb( portSY_FULL_READ_WRITE );
			__wfi();
			__isb( portSY_FULL_READ_WRITE );
		}
		configPOST_SLEEP_PROCESSING(&xExpectedIdleTime);

		/* Stop SysTick.  Again, the time the SysTick is stopped for is
		accounted for as best it can be, but using the tickless mode will
		inevitably result in some tiny drift of the time maintained by the
		kernel with respect to calendar time. */
		ulSysTickCTRL = portNVIC_SYSTICK_CTRL_REG;
		portNVIC_SYSTICK_CTRL_REG = ( ulSysTickCTRL & ~portNVIC_SYSTICK_ENABLE_BIT );

		/* Re-enable interrupts - see comments above __disable_irq() call
		above. */
		//__enable_irq();

		if( ( ulSysTickCTRL & portNVIC_SYSTICK_COUNT_FLAG_BIT ) != 0 )
		{
			uint32_t ulCalculatedLoadValue;

			/* The tick interrupt has already executed, and the SysTick
			count reloaded with ulReloadValue.  Reset the
			portNVIC_SYSTICK_LOAD_REG with whatever remains of this tick
			period. */
			ulCalculatedLoadValue = ( ulTimerCountsForOneTick - 1UL ) - ( ulReloadValue - portNVIC_SYSTICK_CURRENT_VALUE_REG );

			/* Don't allow a tiny value, or values that have somehow
			underflowed because the post sleep hook did something
			that took too long. */
			if( ( ulCalculatedLoadValue < ulStoppedTimerCompensation ) || ( ulCalculatedLoadValue > ulTimerCountsForOneTick ) )
			{
				ulCalculatedLoadValue = ( ulTimerCountsForOneTick - 1UL );
			}

			portNVIC_SYSTICK_LOAD_REG = ulCalculatedLoadValue;

			/* The tick interrupt handler will already have pended the tick
			processing in the kernel.  As the pending tick will be
			processed as soon as this function exits, the tick value
			maintained by the tick is stepped forward by one less than the
			time spent waiting. */
			ulCompleteTickPeriods = xExpectedIdleTime - 1UL;
		}
		else
		{
			/* Something other than the tick interrupt ended the sleep.
			Work out how long the sleep lasted rounded to complete tick
			periods (not the ulReload value which accounted for part
			ticks). */
			ulCompletedSysTickDecrements = ( xExpectedIdleTime * ulTimerCountsForOneTick ) - portNVIC_SYSTICK_CURRENT_VALUE_REG;

			/* How many complete tick periods passed while the processor
			was waiting? */
			ulCompleteTickPeriods = ulCompletedSysTickDecrements / ulTimerCountsForOneTick;

			/* The reload value is set to whatever fraction of a single tick
			period remains. */
			portNVIC_SYSTICK_LOAD_REG = ( ( ulCompleteTickPeriods + 1UL ) * ulTimerCountsForOneTick ) - ulCompletedSysTickDecrements;
		}

		/* Restart SysTick so it runs from portNVIC_SYSTICK_LOAD_REG
		again, then set portNVIC_SYSTICK_LOAD_REG back to its standard
		value.  The critical section is used to ensure the tick interrupt
		can only execute once in the case that the reload register is near
		zero. */
		portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;
		portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;
		rt_tick_set(rt_tick_get() + ulCompleteTickPeriods);
		portNVIC_SYSTICK_LOAD_REG = ulTimerCountsForOneTick - 1UL;
		
		__enable_irq();
	}
}
	
	

static void SYSCLKConfig_STOP(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  uint32_t pFLatency = 0;
  
  /* Get the Oscillators configuration according to the internal RCC registers */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
  
  /* After wake-up from STOP reconfigure the system clock: Enable HSE and PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    rt_kprintf("HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK\n");
  }

  /* Get the Clocks configuration according to the internal RCC registers */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
  {
    rt_kprintf("HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK\n");
  }
}




/***********************************FreeRTOS tickless移植**************************************/

#define PROTO_BUFFER_SIZE			120

static rt_device_t proto_uart;
static rt_sem_t proto_sem;
static uint8_t proto_buffer[PROTO_BUFFER_SIZE];
static rt_size_t real_length;

static void _proto_send(uint8_t *buf, uint16_t size)
{
	rt_device_write(proto_uart, 0, buf, size);
}

static rt_err_t _proto_rx_ind(rt_device_t dev, rt_size_t size)
{
	int i = 0;
//	char *p;
	real_length = rt_device_read( proto_uart, 0, proto_buffer, PROTO_BUFFER_SIZE );
	
//	for (; i<real_length; i++) {
//		rt_kprintf("%x ", proto_buffer[i]);
////		_proto_send(&proto_buffer[i],2);
////		if(proto_buffer[i]==0xa5)
////		{
//			_proto_send("5a",2);
////		}
//	}
	
	for (;i<real_length;i++) {
		//rt_kprintf("%x ", proto_buffer[i]);
		if (Proto_data_analyze_byte(proto_buffer[i]) ) {
			rt_sem_release(proto_sem);
		}
	}
	return 0;
}



void proto_init(void)
{
	rt_err_t result;
	rt_serial_t *pserial;
	
	proto_uart = rt_device_find("uart2");
	
	if( proto_uart == RT_NULL )
	{
		rt_kprintf("Init proto use uart error\n");
		return;
	}
	
	pserial = (rt_serial_t*)proto_uart;
	
	pserial->config.baud_rate = BAUD_RATE_38400;
	
	if (rt_device_open(proto_uart, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX ) != RT_EOK) {
		rt_kprintf("Init proto uart open error\n");
		return;
	}
	rt_device_set_rx_indicate(proto_uart, _proto_rx_ind);	//uart set callback
	
	rt_device_control(proto_uart, RT_DEVICE_CTRL_CONFIG, pserial );
	
	proto_sem = rt_sem_create("mega32u4_proto_sem", 0, RT_IPC_FLAG_FIFO);

	result = rt_thread_init(&mega32u4_uart,
													"mega32u4_uart",
													mega32u4_uart_thread,
													RT_NULL,
													&mega32u4_uart_thread_stack[0],
													sizeof(mega32u4_uart_thread_stack),
													9,
													10);
	if (result == RT_EOK) {
		rt_thread_startup(&mega32u4_uart);
	}
}


static void mega32u4_uart_thread(void* parameter)
{

	proto_set_sendFunc(_proto_send);	//set proto uart send function
	
	while (1)
	{
		if (rt_sem_take( proto_sem, RT_WAITING_FOREVER) == RT_EOK)	//1.5s  RT_WAITING_FOREVER
		{
			//rt_kprintf("Now tick:%d\r\n", rt_tick_get());
			Proto_sensor_cmd_deal();
		}
		rt_kprintf("Now tick:%d\r\n", rt_tick_get());
		//rt_thread_delay(10);
	}
}


void test_print(void **argv, char *argc)
{
	rt_kprintf("shangwudong\r\n");
}
MSH_CMD_EXPORT(test_print, test print shangwudong);


void Light_Pin_Init()
{		
	rt_pin_mode(RGB_R,PIN_MODE_OUTPUT);	//RGB	
	rt_pin_mode(RGB_G,PIN_MODE_OUTPUT);	
	rt_pin_mode(RGB_B,PIN_MODE_OUTPUT);
	
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14,GPIO_PIN_SET);
//	rt_pin_mode(RGB_R2,PIN_MODE_OUTPUT);	//RGB2		
//	rt_pin_mode(RGB_G2,PIN_MODE_OUTPUT);	
//	rt_pin_mode(RGB_B2,PIN_MODE_OUTPUT);
//		
//	rt_pin_mode(RGB_R3,PIN_MODE_OUTPUT);//RGB3		
//	rt_pin_mode(RGB_G3,PIN_MODE_OUTPUT);	
//	rt_pin_mode(RGB_B3,PIN_MODE_OUTPUT);
//	
//	rt_pin_mode(RGB_R4,PIN_MODE_OUTPUT);//RGB4		
//	rt_pin_mode(RGB_G4,PIN_MODE_OUTPUT);	
//	rt_pin_mode(RGB_B4,PIN_MODE_OUTPUT);
//	
//	rt_pin_mode(LED0,PIN_MODE_OUTPUT);
}



void Light_thread_entry(void* parameter) 
{
	Light_Pin_Init();
	while(1)
	{
		rt_pin_write(RGB_R, PIN_LOW);
		rt_pin_write(RGB_G, PIN_LOW);
		rt_pin_write(RGB_B, PIN_LOW);

//		rt_pin_write(LED0, PIN_LOW);
//		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET);
		rt_thread_delay(rt_tick_from_millisecond(500));
		rt_pin_write(RGB_R, PIN_HIGH);
		rt_pin_write(RGB_G, PIN_HIGH);
		rt_pin_write(RGB_B, PIN_HIGH);

//		rt_pin_write(LED0, PIN_HIGH);
//		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET);
		rt_thread_delay(rt_tick_from_millisecond(500));
	}
}

uint8_t CGQ_Read(rt_base_t cgq)
{
		
		if(rt_pin_read(cgq)==1)
		{	
			rt_thread_delay(rt_tick_from_millisecond(10));
			if(rt_pin_read(cgq)==1)
			{
				rt_kprintf("\r CGQ:%u \n",cgq);
				//while(rt_pin_read(cgq)==0);
				return 0;
			}
		}
		return 1;
}

void CGQ_Test(void* parameter)
{
	
//	rt_pin_mode(YG_5SW_KEY1,PIN_MODE_INPUT);
//	rt_pin_mode(YG_5SW_KEY2,PIN_MODE_INPUT);
//	rt_pin_mode(YG_5SW_KEY3,PIN_MODE_INPUT);
//	rt_pin_mode(YG_5SW_KEY4,PIN_MODE_INPUT);
//	rt_pin_mode(YG_5SW_KEY5,PIN_MODE_INPUT);
//	rt_pin_mode(RTHW,PIN_MODE_INPUT);
//	rt_pin_mode(SY_D0,PIN_MODE_INPUT);
//	rt_pin_mode(GMDZ,PIN_MODE_INPUT);
//	rt_pin_mode(JIUJING,PIN_MODE_INPUT);
	while(1)
	{
//		CGQ_Read(KEY1);
//		CGQ_Read(KEY2);
//		CGQ_Read(KEY3);
//		CGQ_Read(KEY4);
//		CGQ_Read(YG_5SW_KEY1);
//		CGQ_Read(YG_5SW_KEY2);
//		CGQ_Read(YG_5SW_KEY3);
//		CGQ_Read(YG_5SW_KEY4);
//		CGQ_Read(YG_5SW_KEY5);
//		CGQ_Read(SY_D0);
//		CGQ_Read(GMDZ);
//   	CGQ_Read(RTHW);
//		CGQ_Read(JIUJING);
		rt_thread_delay(1000);
	}
}



void CGQ_ADC_Test(void* parameter)
{
	MX_ADC3_Init();
	//MX_ADC1_Init();
	//jiujing_gpio_init();
	while(1)
	{
//		rt_kprintf("\r YG_X:%u \n",Get_Adc1_Average(YG_X_ADC_CH,10));//A0+
//		rt_kprintf("\r YG_Y:%u \n",Get_Adc1_Average(YG_Y_ADC_CH ,10));//A1
//		rt_kprintf("\r SY:%u \n",Get_Adc1_Average(SY_ADC_CH,10));//A2
//		rt_kprintf("\r XZHDWQ:%u \n",Get_Adc1_Average(XZHDWQ_ADC_CH,10));//A3
		rt_kprintf("\r BAT:%u \n",Get_Adc3_Average(ADC_CHANNEL_7,10));//F9
		rt_thread_delay(500);
	}
}

void WD_R_Test(void* parameter)
{
	uint8_t tem,hum;
	DS18B20_Init();
	DHT11_Init();
	while(1)
	{
		rt_thread_delay(100);
		DHT11_Read_Data(&tem,&hum);
		rt_kprintf("\r WD_DS18B20:%u \n",DS18B20_Get_Temp());
		rt_kprintf("\r WD_DHT11:%u \n",tem);
		rt_kprintf("\r WD_DHT11:%u \n",hum);
		rt_kprintf("\r\n");
	}
}


void PWM_Test(void* parameter)
{
	uint8_t dir=1;
	uint16_t led0pwmval=0;
	//TIM3_PWM_Init(500-1,84-1);//2-5KHZ  FMQ  500--200
	TIM4_PWM_Init(500-1,84-1);//MADA
	rt_kprintf("\r PWM_Test \n");
	while(1)
	{
		rt_thread_delay(10);	 	
		if(dir)led0pwmval++;				//dir==1 led0pwmval递增
		else led0pwmval--;					//dir==0 led0pwmval递减 
		if(led0pwmval>300)dir=0;			//led0pwmval到达300后，方向为递减
		if(led0pwmval==0)dir=1;				//led0pwmval递减到0后，方向改为递增
		TIM_SetTIM4Compare1(led0pwmval);	//修改比较值，修改占空比,电机调速
		//TIM3_Set_freq(200+led0pwmval);//蜂鸣器变频率
	}
}

void LCD_Test(void* parameter)
{
//		LCD_Pin_Conf();
//		//MX_SPI2_Init();
//		lcd_spi_gpio_init();
//		Lcd_Init();
//		Lcd_Clear(BLUE);
//		rt_kprintf("\r LCD_Test \n");
//	LCD_drawline(1,1,100,100,BLACK);
//	LCD_drawrect(50,50,100,100,RED,1);
//	LCD_drawcircle(50,50,50,BLACK);
//  LCD_ShowString(10,10,RED,3,16,"1234");
	while(1)
	{
//		Lcd_Clear(RED);
		rt_thread_delay(500);
//		Lcd_Clear(GREEN);
//		rt_thread_delay(500);
	}
}

void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}




int flag_dht11 = 0;
extern uint8_t value89;
uint8_t temp,temp1;
//static uint8_t tem = 0;
//static uint8_t hum = 0;





/* 线程1控制块 */
static struct rt_thread thread_sensor;
/* 线程1栈 */
ALIGN(4)
static rt_uint8_t thread_sensor_stack[4096];

static void thread_sensor_entry(void* parameter)
{
//	rt_uint32_t count = 0;

	while (1)
	{
		DZP_deal();
		DZP1_deal();
		MOTOR_deal();
		DHT11_deal();
		DS18B20_deal();
		YG_deal();
		jiujing_deal();
		XZDWQ_deal();
		SOUNDSENSOR_deal();	
		Dial_deal();
		traffic_deal();
		GUI_deal();
		rt_thread_delay(10);
	}
}

//线程2控制块
static struct rt_thread thread_emwin;
//线程2栈
ALIGN(4)
static rt_uint8_t thread_emwin_stack[4096];

static void thread_emwin_entry(void* parameter)
{

//	Traffic_light(0,1,0);
//	rt_thread_delay(10);
//	
}

//extern void MainTask(void);

int main(void)
{
	
//	uint8_t data[7];
//	uint8_t *p=data;
	
	__HAL_RCC_CRC_CLK_ENABLE();
	
	MX_ADC3_Init();
	
//	while(Get_Adc3(ADC_CHANNEL_8)>200) ;//开机
	
//	HAL_Init();
	//rt_pin_mode(44, PIN_MODE_INPUT); 
	rt_pin_mode(74, PIN_MODE_OUTPUT);
	//if (rt_pin_read(44) == PIN_HIGH)
	//{
			rt_pin_write(74, PIN_HIGH);
	//}
	
		vPortSetupTimerInterrupt();
    /* user app entry */
//		rt_components_board_init();
//		rt_console_set_device("vcom");
//		libc_system_init();
//		easyflash_init();

		/* mount sd card fat partition 1 as root directory */  
		if (dfs_mount("flash0", "/", "elm", 0, 0) == 0) {
				rt_kprintf("flash0 mount to /.\n");  
		} else {
				rt_kprintf("flash0 mount to / failed!, fatmat and try again!\n");
				/* fatmat filesystem. */
				dfs_mkfs("elm", "flash0");
				/* re-try mount. */
				if (dfs_mount("flash0", "/", "elm", 0, 0) != 0) {
						rt_kprintf("sd0 mount to / failed.\n"); 
				}
		}

		


		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);	
		
		//StandbyMode_Measure();
		

		
		Light_Pin_Init();//rgb灯初始化

			
		TIM3_PWM_Init(500-1,84-1);//2-5KHZ  FMQ  500--200
		TIM4_PWM_Init(500-1,84-1);//MADA
		
		Init_MAX7219(); //点阵屏左初始化		
		Init_DZP1();//点阵屏右初始化
		
		MX_ADC1_Init();
		YG_KEY_gpio_init();

		Sound_init();
		DS18B20_Init();
		//MX_ADC3_Init();

		SMG_init();
		DHT11_Init();
		
		RTHW_init();
		sw5_key_init();
		LIGHT_init();
		LCD_Pin_Conf();		//lcd io口初始化
		MX_SPI2_Init();
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	  Lcd_Init();
		Lcd_Clear(WHITE);
		LCD_show_image();
		mada_gpio_init();
		GD_init();
		GZ_init();
		
		MQ_CON_init();
		//HAL_GPIO_WritePin(GPIOF,GPIO_PIN_3,GPIO_PIN_SET);//酒精传感器电源控制端
		
				proto_init();

		rt_thread_idle_sethook(idle_hook_function);
		
		multi_button_test();
		

//创建线程1	
	rt_err_t result = rt_thread_init(&thread_sensor, "thread_sensor", thread_sensor_entry, RT_NULL, &thread_sensor_stack[0], sizeof(thread_sensor_stack), 5, 10);
	if (result == RT_EOK)
		rt_thread_startup(&thread_sensor);
	

//创建线程2：emwin
	rt_err_t result_emwin = rt_thread_init(&thread_emwin, "thread_emwin", thread_emwin_entry, RT_NULL,&thread_emwin_stack[0], sizeof(thread_emwin_stack), 6, 10);
	if (result_emwin == RT_EOK)
		rt_thread_startup(&thread_emwin);
	
//max7219_test();
	while(1)
	{
			
			SMG_show(value89);
			rt_thread_delay(10);

	}
}





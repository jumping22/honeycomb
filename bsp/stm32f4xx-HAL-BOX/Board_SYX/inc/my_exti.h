#ifndef CSQ_H_
#define CSQ_H_

#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include <rtthread.h>


void YG_KEY_callback(void *args);
void YG_KEY_irq_thread_entry(void* parameter);

void KEY1_callback(void *args);
void KEY1_irq_thread_entry(void* parameter);

void KEY2_callback(void *args);
void KEY2_irq_thread_entry(void* parameter);

void KEY3_callback(void *args);
void KEY3_irq_thread_entry(void* parameter);

void KEY4_callback(void *args);
void KEY4_irq_thread_entry(void* parameter);

void YG_5SW_KEY2_callback(void *args);
void YG_5SW_KEY2_irq_thread_entry(void* parameter);

void YG_5SW_KEY3_callback(void *args);
void YG_5SW_KEY3_irq_thread_entry(void* parameter);

void YG_5SW_KEY4_callback(void *args);
void YG_5SW_KEY4_irq_thread_entry(void* parameter);

void YG_5SW_KEY5_callback(void *args);
void YG_5SW_KEY5_irq_thread_entry(void* parameter);

void RTHW_callback(void *args);
void RTHW_irq_thread_entry(void* parameter);

void SY_D0_callback(void *args);
void SY_D0_irq_thread_entry(void* parameter);

void GMDZ_callback(void *args);
void GMDZ_irq_thread_entry(void* parameter);


#endif

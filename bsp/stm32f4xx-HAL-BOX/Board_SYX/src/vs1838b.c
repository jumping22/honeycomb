/*-------------------------协议-------------------------- 
开始拉低9ms,接着是一个4.5ms的高脉冲,通知器件开始传送数据了 
接着是发送4个8位二进制码,第一二个是遥控识别码(REMOTE_ID),第一个为 
正码(0),第二个为反码(255),接着两个数据是键值,第一个为正码 
第二个为反码.发送完后40ms,遥控再发送一个9ms低,2ms高的脉冲, 
表示按键的次数,出现一次则证明只按下了一次,如果出现多次,则可 
以认为是持续按下该键. 
----------------------红外解码还在有问题进入中断后读不到引导码------------------------------*/   
#include "vs1838b.h" 
#include "usart.h"
                
uint32_t hw_jsm=0;
uint8_t hw_jsbz=0;
  
    
//功能    ：检测脉冲宽度*****************************************/  
//说明    ：最长脉宽为5ms 
//返回值  : t代表脉宽为t*20us(T=1~250); *************************/ 

//uint8_t Pulse_Width_Check(void)  
//{  
//    u16 t=0;    
//    while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==Bit_SET) //VS1838将总线拉高后 
//    {      
//        t++;  
//        delay_us(20);                      
//        if(t==270)  
//        return t;           //超时溢出  
//    }  
//    return t;  
//}                


uint8_t HW_jssj()
{
	uint8_t t=0;
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)==1)//高电平进入
	{
		t++;
		HAL_Delay_us(20);//20

		if(t>=255) return t;//返回无效时间
	}

	return t;
}
       
////功能    : 中断函数*************PF10进不了中断*********************/  
////说明    ：检测是否有中断 
////返回值: 无************************************************/ 
//void EXTI15_10_IRQHandler(void)
//{
//  
//	uint8_t Tim=0,Ok=0,Data,Num=0;
//	printf("asd");
//   while(1)
//   {
//	   	if(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_10)==1)
//		{
//			 Tim=HW_jssj();//高电平时间装载

//			 if(Tim>=250) break;//遇到无效电平直接退出

//			 if(Tim>=200 && Tim<250)//检测到引导信号 4.5ms  下限必须到150 可能是48M时钟本身慢于72M的原因
//			 {
//			 	Ok=1;               //树起开始收码的大旗
//			 }
//			 else if(Tim>=60 && Tim<90)
//			 {
//			 	Data=1;//逻辑1
//			 }
//			 else if(Tim>=10 && Tim<50)
//			 {
//			 	Data=0;//逻辑0
//			 }

//			 if(Ok==1)//看到大旗
//			 {
//			 	hw_jsm<<=1;//移位准备
//				hw_jsm+=Data;//发射

//				if(Num>=32)
//				{
//					hw_jsbz=1;//战争结束 树打扫战场旗                    
//				  	break;
//				}
//			 }

//			 Num++;
//		}
//   }
//	 HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);		//调用中断处理公用函数
//}
 
void EXTI1_IRQHandler(void)
{
	uint8_t Tim=0,Ok=0,Data,Num=0;
	
	while(1)
	{
		
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)==1)
		{
					 Tim=HW_jssj();//高电平时间装载

				 if(Tim>=250)
				 {
					//printf("\r yd_fall \n");
					break;//遇到无效电平直接退出
				 }					 
				 if(Tim>=200 && Tim<250)//检测到引导信号 4.5ms  下限必须到150 可能是48M时钟本身慢于72M的原因
				 {
					Ok=1;               //树起开始收码的大旗
					 //printf("\r yd_ok\n");
				 }
				 else if(Tim>=60 && Tim<90)
				 {
					Data=1;//逻辑1
				 }
				 else if(Tim>=10 && Tim<50)
				 {
					Data=0;//逻辑0
				 }

				 if(Ok==1)//看到大旗
				 {
					hw_jsm<<=1;//移位准备
					hw_jsm+=Data;//发射

					if(Num>=32)
					{
						hw_jsbz=1;//战争结束 树打扫战场旗                    
							break;
					}
				 }

			 Num++;
		}
	}
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);	
}
//功能    : 处理红外键盘*****************************************/  
//说明    ：无 
//返回值: 键值************************************************/  
uint8_t Remote_Process(void)  
{                 
    uint8_t t1,t2;     
    t1 = ((hw_jsm >> 8)&(0xff));  //得到控制码  
    t2=(hw_jsm >> 0)&0xff;        //得到控制反码   
    hw_jsm=0;           //清除标记      
    if(t1==(uint8_t)~t2)  
        return t1;      //正码反码对比校验  
    return 0xff;   
}  

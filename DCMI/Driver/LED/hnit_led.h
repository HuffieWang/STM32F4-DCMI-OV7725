#ifndef __LED_H
#define __LED_H
#include "stm32_sys.h"

//LED�˿ڶ���
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 

void led_init(void);//��ʼ��		 				    
#endif

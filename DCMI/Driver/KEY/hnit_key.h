#ifndef __KEY_H
#define __KEY_H
#include "stm32_sys.h"
		 				
//KEY�˿ڶ���
#define KEY0 PEin(4)	// KEY0
#define KEY1 PEin(3)	// KEY1	 

void KEY_Init(void);

#endif

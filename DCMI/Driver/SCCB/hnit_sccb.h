#ifndef __SCCB_H
#define __SCCB_H

#include "stm32_sys.h"

#define SCL_H         GPIO_SetBits(GPIOC , GPIO_Pin_1) 
#define SCL_L         GPIO_ResetBits(GPIOC , GPIO_Pin_1) 
   
#define SDA_H         GPIO_SetBits(GPIOC , GPIO_Pin_2) 
#define SDA_L         GPIO_ResetBits(GPIOC , GPIO_Pin_2) 

#define SCL_read      GPIO_ReadInputDataBit(GPIOC , GPIO_Pin_1) 
#define SDA_read      GPIO_ReadInputDataBit(GPIOC , GPIO_Pin_2) 

#define ADDR_OV7725   0x42

void SCCB_GPIO_Config(void);
int SCCB_WriteByte( u16 WriteAddress , u8 SendByte);
int SCCB_ReadByte(u8* pBuffer,   u16 length,   u8 ReadAddress);

#endif 

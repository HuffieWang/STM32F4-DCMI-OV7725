#ifndef __SCCB_H
#define __SCCB_H

#include "stm32_sys.h"

#define SCL_H         GPIO_SetBits(GPIOD , GPIO_Pin_6) 
#define SCL_L         GPIO_ResetBits(GPIOD , GPIO_Pin_6) 
   
#define SDA_H         GPIO_SetBits(GPIOD , GPIO_Pin_7) 
#define SDA_L         GPIO_ResetBits(GPIOD , GPIO_Pin_7) 

#define SCL_read      GPIO_ReadInputDataBit(GPIOD , GPIO_Pin_6) 
#define SDA_read      GPIO_ReadInputDataBit(GPIOD , GPIO_Pin_7) 

#define ADDR_OV7725   0x42

void sccb_gpio_config(void);
int sccb_write_byte( u16 WriteAddress , u8 SendByte);
int sccb_read_byte(u8* pBuffer,   u16 length,   u8 ReadAddress);

#endif 

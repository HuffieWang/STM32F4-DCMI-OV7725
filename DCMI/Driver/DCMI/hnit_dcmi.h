#ifndef _DCMI_H
#define _DCMI_H
#include "stm32_sys.h"    									
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DCMI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void dcmi_config(void);
void dcmi_dma_init(u32 DMA_Memory0BaseAddr,u16 DMA_BufferSize,u32 DMA_MemoryDataSize,u32 DMA_MemoryInc);
void dcmi_start(void);
void dcmi_stop(void);

#endif






















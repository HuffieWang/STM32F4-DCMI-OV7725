#ifndef _DCMI_H
#define _DCMI_H
#include "stm32_sys.h" 



void dcmi_config(void);
void dcmi_dma_init(u32 DMA_Memory0BaseAddr, u16 DMA_BufferSize, u16 DMA_Memory0Inc);
void dcmi_start(void);
void dcmi_stop(void);

#endif






















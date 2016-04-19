/*********************** HNIT 3103 Application Team **************************
 * 文 件 名 ：main.c
 * 描    述 ：OV7725_DCMI摄像头采集图像，并在LCD液晶显示 
 * 实验平台 ：STM32F407开发板
 * 库 版 本 ：ST1.4.0
 * 时    间 ：2016.4.17
 * 作    者 ：3103创新团队
 * 修改记录 ：无
******************************************************************************/
#include "stm32_sys.h"
#include "stm32_delay.h"
#include "stm32_usart.h"
#include "hnit_led.h" 
#include "hnit_key.h"
#include "hnit_lcd.h"
#include "hnit_ov7725.h"
#include "hnit_dcmi.h"
#include "hnit_sram.h"

uint16_t image[80000] __attribute__((at(0X68000000)));
uint16_t temp = 0;
uint32_t m = 0,n = 0;
uint16_t i, test[100];

#define DMA_TRANSFER_LEN   38400
u32 dma_next_addr = 0;


/**
  * @brief  一帧图像接收完毕，LCD光标回到初始位置
  * @param  无
  * @retval 无
  */
void DCMI_IRQHandler(void)
{
    if(DCMI_GetITStatus(DCMI_IT_FRAME) == SET)//捕获到一帧图像
    {
        LED0 = ~LED0;
#ifdef  DCMI_LCD      
        LCD_SetCursor(0, 0);       // 设置光标位置                   
        lcd_write_ram_prepare();   // 开始写入GRAM  
#endif    
        
#ifdef DCMI_SRAM                    
        dcmi_stop();
               
        dma_next_addr = 0;
        LCD_SetCursor(0, 0);       // 设置光标位置                   
        lcd_write_ram_prepare();   // 开始写入GRAM 
        
        for(n = 0; n < 76800; n++)
        {
            LCD->LCD_RAM = image[n];           
           
        }
        for(n = 0; n < 76800; n++)
        {
            image[n] = 0;
        }
        dcmi_dma_init((u32)&image, DMA_TRANSFER_LEN, DMA_MemoryInc_Enable);
        dma_next_addr = (u32)&image[DMA_TRANSFER_LEN];   
        dcmi_start();  
#endif           
        DCMI_ClearITPendingBit(DCMI_IT_FRAME);//清除帧中断      
    }
} 


/**
  * @brief  DMA2_Stream1中断服务函数
  * @param  无
  * @retval 无
  */
void DMA2_Stream1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) == SET)    
    {
        LED1 = ~LED1;
        
        //DMA2_Stream1->M0AR = (u32)&image;
        
        DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
    }
}

/**
  * @brief  c程序入口
  * @param  无
  * @retval 无
  */
int main(void)
{  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    usart1_init(115200);	
    led_gpio_config();
    key_gpio_config(); 
    
    lcd_init();
    
    fsmc_sram_init();
    
    delay_ms(100);
    dcmi_config();
#ifdef DCMI_LCD      
    dcmi_dma_init((u32)&LCD->LCD_RAM, 8, DMA_MemoryInc_Disable);     
#else 
    dma_next_addr = (u32)&image[DMA_TRANSFER_LEN];
    dcmi_dma_init((u32)&image, DMA_TRANSFER_LEN, DMA_MemoryInc_Enable);    
#endif
    
    while(ov7725_init() != SUCCESS)
    {
        ;
    }
    dcmi_start(); 

    delay_ms(1000);
    while(1)
    {  
        ;
	}
}

/******************************* END OF FILE *********************************/

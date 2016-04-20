/*********************** HNIT 3103 Application Team **************************
 * 文 件 名 ：main.c
 * 描    述 ：OV7725_DCMI采集图像，并在LCD液晶显示 
 * 实验平台 ：STM32F407开发板
 * 库 版 本 ：ST1.4.0
 * 时    间 ：2016.4.20
 * 作    者 ：3103创新团队 王昱霏
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

//#define DCMI_LCD     //DCMI -> DMA -> LCD_GRAM，直接显示，完全不经过CPU
#define DCMI_SRAM    //DCMI -> DMA -> SRAM -> LCD_GRAM，数据保存至SRAM，在帧中断中显示图像

uint16_t image[80000] __attribute__((at(0X68000000)));
uint32_t n = 0;

/**
  * @brief  帧中断，一帧图像接收完毕后的中断处理函数
  * @param  无
  * @retval 无
  */
void DCMI_IRQHandler(void)
{
    if(DCMI_GetITStatus(DCMI_IT_FRAME) == SET)//捕获到一帧图像
    {   
        //如果定义为直接写GRAM
#ifdef  DCMI_LCD   
        LED0 = ~LED0;      
        lcd_set_cursor(0, 0);         // 设置光标位置                   
        lcd_write_ram_prepare();      // 开始写入GRAM  
      
        //如果定义为写SRAM
#else                  
        dcmi_stop();                  //先停止DCMI传输
        LED1 = ~LED1;       
        lcd_set_cursor(0, 0);               
        lcd_write_ram_prepare();        
        for(n = 0; n < 76800; n++)    //将SRAM中的一帧数据显示至LCD
        {
            LCD->LCD_RAM = image[n];           
           
        }
        DMA2_Stream1->M0AR = (u32)&image;     //DMA传输地址回到图像缓存的首地址
        dcmi_start();                         //重新开始DCMI传输
#endif           
        DCMI_ClearITPendingBit(DCMI_IT_FRAME);//清除帧中断      
    }
} 

/**
  * @brief  c程序入口
  * @param  无
  * @retval 异常代号
  */
int main(void)
{  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    usart1_init(115200);	
    
    led_gpio_config();
    key_gpio_config();

    //竖屏，扫描方向5：从上到下,从右到左
    fsmc_lcd_init();  
    
    //SRAM：1M byte，起始地址0X68000000
    fsmc_sram_init();   
   
    dcmi_config();
  
#ifdef DCMI_LCD  
    //LDC_GRAM地址硬件自增，所以应禁止内存地址自增
    dcmi_dma_init((u32)&LCD->LCD_RAM, 8, DMA_MemoryInc_Disable);     
#else 
    //BufferSize必须为38400，这样一次DMA搬运的总数据刚好是一帧图像
    //32(外设字节长度) * 38400(搬运次数) = 16(内存字节长度) * 320 * 240
    dcmi_dma_init((u32)&image, 38400, DMA_MemoryInc_Enable);    
#endif
    
    while(ov7725_init() != SUCCESS)
    {
        ;
    }
    for(n = 0; n < 80000; n++)
    {
        image[n] = 0;
    }   
    dcmi_start();         
    while(1)
    {  
        ;
    }
}

/******************************* END OF FILE *********************************/

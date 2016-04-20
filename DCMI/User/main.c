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
#define DCMI_SRAM    //DCMI -> DMA -> SRAM，图像保存至SRAM，用CPU读取SRAM再显示到LCD

uint16_t image0[77000] __attribute__((at(0X68000000)));                   //图像缓冲区0
uint16_t image1[77000] __attribute__((at(0X68000000 + sizeof(image0))));  //图像缓冲区1
uint16_t *p_image = image1;                                               //图像显示指针，指向空闲缓冲区
uint16_t dma_buffer_select = 0;                                           //用于切换DMA缓冲区
uint16_t dma_complete = 0;                                                //DMA接收完成标志位

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
        lcd_set_cursor(0, 0);         // 设置光标位置                   
        lcd_write_ram_prepare();      // 开始写入GRAM  
        LED0 = ~LED0;
      
        //如果定义为写SRAM
#else                  
        dcmi_stop();                  //停止DCMI传输
        dma_complete = 1;             //DMA传输完成标志置位        
        LED1 = ~LED1;       
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
    fsmc_lcd_init();   //竖屏，扫描方向5：从上到下,从右到左    
    fsmc_sram_init();  //SRAM：1M byte，起始地址0X68000000 
    dcmi_config();
  
#ifdef DCMI_LCD  
    
    //LDC_GRAM地址硬件自增，所以应禁止内存地址自增
    dcmi_dma_init((u32)&LCD->LCD_RAM, 8, DMA_MemoryInc_Disable);     
#else 
    
    //BufferSize必须为38400，这样一次DMA搬运的总数据刚好是一帧图像
    //32(外设字节长度) * 38400(搬运次数) = 16(内存字节长度) * 320 * 240
    dcmi_dma_init((u32)&image0, 38400, DMA_MemoryInc_Enable);    
#endif
    
    while(ov7725_init() != SUCCESS);
    
    for(n = 0; n < 77000; n++)
    {
        image0[n] = 0;
        image1[n] = 0;
    }   
    dcmi_start();         
    while(1)
    {  
        #ifdef DCMI_SRAM
        
        //确保图像接收完毕
        while(dma_complete == 0);
        
        dma_complete = 0;
        
        //切换图像缓冲区，重新开始传输
        dma_buffer_select = !dma_buffer_select;
        if(dma_buffer_select == 0)
        {
            p_image = image1;
            DMA2_Stream1->M0AR = (u32)&image0; 
        }
        else
        {
            p_image = image0;
            DMA2_Stream1->M0AR = (u32)&image1;            
        }
        dcmi_start();
        
        //图像传输在后台运行，现在CPU是空闲的，算法什么的都可以写在下面       
        lcd_set_cursor(0, 0);               
        lcd_write_ram_prepare(); 
        
        //将空闲缓冲区中的图片显示至LCD
        for(n = 0; n < 76800; n++)
        {
            LCD->LCD_RAM = *p_image;
            p_image++;
        }
        
        #endif
    }
}

/******************************* END OF FILE *********************************/

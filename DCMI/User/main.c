/*********************** HNIT 3103 Application Team **************************
 * 文 件 名 ：main.c
 * 描    述 ：OV7725_DCMI摄像头驱动  
 * 实验平台 ：STM32F407开发板
 * 库 版 本 ：ST1.4.0
 * 时    间 ：2016.3.19
 * 作    者 ：3103创新团队
 * 修改记录 ：无
******************************************************************************/

#include "stm32_sys.h"
#include "stm32_delay.h"
#include "stm32_usart.h"
#include "hnit_led.h" 
#include "hnit_key.h"
#include "hnit_lcd.h"

#include "hnit_sccb.h"
#include "hnit_ov7725.h"
#include "hnit_dcmi.h"

u16 imgData[80][80] = {0};
uint16_t n = 0; 
/*****************************************************************************
*	函 数 名: main
*	功    能: c程序入口
*   调用函数：无
*	形    参：无
*	返 回 值: 错误代码
******************************************************************************/	
int main(void)
{  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
    usart1_init(115200);	
    
    lcd_init();
    LCD_Scan_Dir(5);
    led_init();
    key_init();
    sccb_gpio_config();   
    
  	dcmi_config();	
    
	dcmi_dma_init((u32)&LCD->LCD_RAM, 8, DMA_MemoryDataSize_HalfWord, DMA_MemoryInc_Disable);
   
    lcd_clear(BLACK); 
    POINT_COLOR = WHITE; 
    BACK_COLOR = BLACK; 
       
	while(ov7725_init() != SUCCESS);
    
    LCD_WriteRAM_Prepare();   // 开始写入GRAM 
    dcmi_start();
    //lcd_show_image(imgData); 
    while(1)
    {    
//        delay_ms(500);
//        LED0 = ~LED0;
	}
}

/******************************* END OF FILE *********************************/

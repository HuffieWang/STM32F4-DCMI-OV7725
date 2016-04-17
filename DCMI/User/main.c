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

u16 imgData[80][80] = {0};

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
  	dcmi_config();	 
    dcmi_dma_init((u32)&LCD->LCD_RAM, 8);
  
    lcd_scan_dir(5);
    
    while(ov7725_init() != SUCCESS);
    
    delay_ms(500);
    dcmi_start();
    while(1)
    {   
        LED0 = ~LED0;
        delay_ms(500);
	}
}

/******************************* END OF FILE *********************************/

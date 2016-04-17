/*********************** HNIT 3103 Application Team **************************
 * �� �� �� ��main.c
 * ��    �� ��OV7725_DCMI����ͷ�ɼ�ͼ�񣬲���LCDҺ����ʾ 
 * ʵ��ƽ̨ ��STM32F407������
 * �� �� �� ��ST1.4.0
 * ʱ    �� ��2016.4.17
 * ��    �� ��3103�����Ŷ�
 * �޸ļ�¼ ����
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
  * @brief  c�������
  * @param  ��
  * @retval ��
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

/*********************** HNIT 3103 Application Team **************************
 * �� �� �� ��main.c
 * ��    �� ��OV7725_DCMI����ͷ����  
 * ʵ��ƽ̨ ��STM32F407������
 * �� �� �� ��ST1.4.0
 * ʱ    �� ��2016.3.19
 * ��    �� ��3103�����Ŷ�
 * �޸ļ�¼ ����
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
*	�� �� ��: main
*	��    ��: c�������
*   ���ú�������
*	��    �Σ���
*	�� �� ֵ: �������
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
    
    LCD_WriteRAM_Prepare();   // ��ʼд��GRAM 
    dcmi_start();
    //lcd_show_image(imgData); 
    while(1)
    {    
//        delay_ms(500);
//        LED0 = ~LED0;
	}
}

/******************************* END OF FILE *********************************/

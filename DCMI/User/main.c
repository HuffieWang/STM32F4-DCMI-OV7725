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
#include "hnit_ov7725.h"
#include "hnit_sccb.h"

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
    USART1_Init(115200);	
 
    LCD_Init();
    LED_Init();
    KEY_Init();
    SCCB_GPIO_Config();
    ov7725_gpio_config();
    
    LCD_Clear(BLACK); 
    POINT_COLOR = WHITE; BACK_COLOR = BLACK;   
	while(Ov7725_Init() != SUCCESS);
    
    /*
    //TODO2 ����ΪRGB565ģʽ
	OV2640_RGB565_Mode();	
    
    //TODO3 DCMI���� attention���˴���LCD�����ַ
	My_DCMI_Init();			
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,1,DMA_MemoryDataSize_HalfWord,DMA_MemoryInc_Disable);
    
    //TODO4 �ֱ�������
 	OV2640_OutSize_Set(lcddev.width,lcddev.height); 
    */
    
//  LCD_DisImg(imgData);
    while(1)
    {    
        LED0 = ~LED0;
        delay_ms(500);
	}
}

/******************************* END OF FILE *********************************/

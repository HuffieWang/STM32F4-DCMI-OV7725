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
u16 imgData[80][80] = {0};

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
	Delay_Init(168);
    USART1_Init(115200);	
	
    LCD_Init();
    LED_Init();
    KEY_Init();
    SCCB_GPIO_Config();    
    LCD_Clear(BLACK); 
    POINT_COLOR = WHITE; BACK_COLOR = BLACK; 
      
//	while(Ov7725_Init() != SUCCESS);
   
//    LCD_DisImg(imgData);  
    while(1)
    {
        LED0 = ~LED0;
        Delay_ms(500);
	}
}

/******************************* END OF FILE *********************************/

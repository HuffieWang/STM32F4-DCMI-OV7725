/*********************** HNIT 3103 Application Team **************************
 * �� �� �� ��main.c
 * ��    �� ��Һ����ʾ    
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
	u32 t = 0;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_Init(168);
    USART1_Init(115200);		
    LCD_Init();
    LED_Init();
    KEY_Init();
    
    LCD_Clear(BLACK);   
    POINT_COLOR = BLACK;	
    BACK_COLOR = WHITE;
    
    while(KEY0 == 1);
    
    imgData[0][0] = 1;
    imgData[79][0] = 1;
    imgData[0][79] = 1;
    imgData[79][79] = 1;
    
    LCD_DisImg(imgData);
    LCD_DisNum(50,50,66666);

    while(1)
    {
        LED0 = ~LED0;
		Delay_ms(500);
		t++;
	}
}

/******************************* END OF FILE *********************************/

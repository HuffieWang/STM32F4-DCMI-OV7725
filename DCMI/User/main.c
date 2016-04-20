/*********************** HNIT 3103 Application Team **************************
 * �� �� �� ��main.c
 * ��    �� ��OV7725_DCMI�ɼ�ͼ�񣬲���LCDҺ����ʾ 
 * ʵ��ƽ̨ ��STM32F407������
 * �� �� �� ��ST1.4.0
 * ʱ    �� ��2016.4.20
 * ��    �� ��3103�����Ŷ� ������
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
#include "hnit_sram.h"

//#define DCMI_LCD     //DCMI -> DMA -> LCD_GRAM��ֱ����ʾ����ȫ������CPU
#define DCMI_SRAM    //DCMI -> DMA -> SRAM -> LCD_GRAM�����ݱ�����SRAM����֡�ж�����ʾͼ��

uint16_t image[80000] __attribute__((at(0X68000000)));
uint32_t n = 0;

/**
  * @brief  ֡�жϣ�һ֡ͼ�������Ϻ���жϴ�����
  * @param  ��
  * @retval ��
  */
void DCMI_IRQHandler(void)
{
    if(DCMI_GetITStatus(DCMI_IT_FRAME) == SET)//����һ֡ͼ��
    {   
        //�������Ϊֱ��дGRAM
#ifdef  DCMI_LCD   
        LED0 = ~LED0;      
        lcd_set_cursor(0, 0);         // ���ù��λ��                   
        lcd_write_ram_prepare();      // ��ʼд��GRAM  
      
        //�������ΪдSRAM
#else                  
        dcmi_stop();                  //��ֹͣDCMI����
        LED1 = ~LED1;       
        lcd_set_cursor(0, 0);               
        lcd_write_ram_prepare();        
        for(n = 0; n < 76800; n++)    //��SRAM�е�һ֡������ʾ��LCD
        {
            LCD->LCD_RAM = image[n];           
           
        }
        DMA2_Stream1->M0AR = (u32)&image;     //DMA�����ַ�ص�ͼ�񻺴���׵�ַ
        dcmi_start();                         //���¿�ʼDCMI����
#endif           
        DCMI_ClearITPendingBit(DCMI_IT_FRAME);//���֡�ж�      
    }
} 

/**
  * @brief  c�������
  * @param  ��
  * @retval �쳣����
  */
int main(void)
{  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    usart1_init(115200);	
    
    led_gpio_config();
    key_gpio_config();

    //������ɨ�跽��5�����ϵ���,���ҵ���
    fsmc_lcd_init();  
    
    //SRAM��1M byte����ʼ��ַ0X68000000
    fsmc_sram_init();   
   
    dcmi_config();
  
#ifdef DCMI_LCD  
    //LDC_GRAM��ַӲ������������Ӧ��ֹ�ڴ��ַ����
    dcmi_dma_init((u32)&LCD->LCD_RAM, 8, DMA_MemoryInc_Disable);     
#else 
    //BufferSize����Ϊ38400������һ��DMA���˵������ݸպ���һ֡ͼ��
    //32(�����ֽڳ���) * 38400(���˴���) = 16(�ڴ��ֽڳ���) * 320 * 240
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

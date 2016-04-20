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
#define DCMI_SRAM    //DCMI -> DMA -> SRAM��ͼ�񱣴���SRAM����CPU��ȡSRAM����ʾ��LCD

uint16_t image0[77000] __attribute__((at(0X68000000)));                   //ͼ�񻺳���0
uint16_t image1[77000] __attribute__((at(0X68000000 + sizeof(image0))));  //ͼ�񻺳���1
uint16_t *p_image = image1;                                               //ͼ����ʾָ�룬ָ����л�����
uint16_t dma_buffer_select = 0;                                           //�����л�DMA������
uint16_t dma_complete = 0;                                                //DMA������ɱ�־λ

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
        lcd_set_cursor(0, 0);         // ���ù��λ��                   
        lcd_write_ram_prepare();      // ��ʼд��GRAM  
        LED0 = ~LED0;
      
        //�������ΪдSRAM
#else                  
        dcmi_stop();                  //ֹͣDCMI����
        dma_complete = 1;             //DMA������ɱ�־��λ        
        LED1 = ~LED1;       
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
    fsmc_lcd_init();   //������ɨ�跽��5�����ϵ���,���ҵ���    
    fsmc_sram_init();  //SRAM��1M byte����ʼ��ַ0X68000000 
    dcmi_config();
  
#ifdef DCMI_LCD  
    
    //LDC_GRAM��ַӲ������������Ӧ��ֹ�ڴ��ַ����
    dcmi_dma_init((u32)&LCD->LCD_RAM, 8, DMA_MemoryInc_Disable);     
#else 
    
    //BufferSize����Ϊ38400������һ��DMA���˵������ݸպ���һ֡ͼ��
    //32(�����ֽڳ���) * 38400(���˴���) = 16(�ڴ��ֽڳ���) * 320 * 240
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
        
        //ȷ��ͼ��������
        while(dma_complete == 0);
        
        dma_complete = 0;
        
        //�л�ͼ�񻺳��������¿�ʼ����
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
        
        //ͼ�����ں�̨���У�����CPU�ǿ��еģ��㷨ʲô�Ķ�����д������       
        lcd_set_cursor(0, 0);               
        lcd_write_ram_prepare(); 
        
        //�����л������е�ͼƬ��ʾ��LCD
        for(n = 0; n < 76800; n++)
        {
            LCD->LCD_RAM = *p_image;
            p_image++;
        }
        
        #endif
    }
}

/******************************* END OF FILE *********************************/

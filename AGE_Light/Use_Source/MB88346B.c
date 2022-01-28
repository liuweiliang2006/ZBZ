#include "main.h"
#include "MyInclude.h"


/******************************************************************
�������ƣ�
��������:
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void MB88346B_Gpio_Init(void)
{
    
	  GPIO_InitTypeDef  GPIO_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOE, ENABLE);
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
	  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
   
    GPIO_InitStruct.GPIO_Pin  =  GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
	  
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
	
    MB88346BSDI_H;
    MB88346BCLK_H;

}

/**************************************************************************
��������:void SetDAC7558(float dataf,uint16_t ch,uint32_t command)
��������:��λDAC7558
�������:float32 dataf
�������:
����ֵ��:
ע������:
���ʱ��:2021��7��23��15:50:34
����������ʷ��¼:
����ʱ��:
**************************************************************************/
void SetMB88346BSingle(uint16_t data,uint16_t ChanNo)
{
    uint16_t itemp;
    volatile uint16_t buff;
    volatile uint16_t bNo,bit,channel;
    
	  bNo=ChanNo/8;
	  bit=((ChanNo%8));
	  
	  switch(bit)
		{
		  case 0:channel=0x08;break;
			case 1:channel=0x04;break;
			case 2:channel=0x0C;break;
			case 3:channel=0x02;break;
			case 4:channel=0x0A;break;
			case 5:channel=0x06;break;
			case 6:channel=0x0E;break;
			case 7:channel=0x01;break;
			default:channel=0;break;
		}
    buff = channel;
	  buff =buff << 8;
    buff |= (data);

    DA_LoadData(bNo,1);
    DELAY_Ms(2);

    DA_LoadData(bNo,0);
	
    MB88346BCLK_L;   //CLK=1
    for(itemp=0;itemp<12;itemp++)
    {	  
        if(buff & 0x800)
        {
            MB88346BSDI_H;
        }else
        {
            MB88346BSDI_L;
        }        
        MB88346BCLK_L;   //CLK=0
        DELAY_Ms(2);
				MB88346BCLK_H;   //CLK=1
        DELAY_Ms(2);
        buff<<=1;
    }
    DA_LoadData(bNo,1);
    DELAY_Ms(2);
		
    DA_LoadData(bNo,0);
   
}

/**************************************************************************
��������:void SetDAC7558(float dataf,uint16_t ch,uint32_t command)
��������:��λDAC7558
�������:float32 dataf
�������:
����ֵ��:
ע������:
���ʱ��:2021��7��23��15:50:34
����������ʷ��¼:
����ʱ��:
**************************************************************************/
void SetMB88346BMainDA(uint16_t data,uint16_t ChanNo)
{
    uint16_t itemp;
    uint32_t buff;
    uint16_t bNo,bit;
    
	  bNo=ChanNo;
	  bit=9;
	  
    buff = bit;
	  buff =buff << 8;
    buff |= (data);

    DA_LoadData(bNo,1);
    DELAY_Ms(5);

    DA_LoadData(bNo,0);
    MB88346BCLK_L;   //CLK=1
    for(itemp=0;itemp<12;itemp++)
    {	  
        if(buff & 0x800)
        {
            MB88346BSDI_H;
        }else
        {
            MB88346BSDI_L;
        }        
        MB88346BCLK_L;   //CLK=0
        DELAY_Ms(2);
				MB88346BCLK_H;   //CLK=1
        DELAY_Ms(2);
        buff<<=1;
    }
    DA_LoadData(bNo,1);
    DELAY_Ms(2);
		
    DA_LoadData(bNo,0);
}






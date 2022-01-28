#include "main.h"
#include "MyInclude.h"

struct AGE_VAR         AGEVar;
struct SYS_CTRL        SysCtrl;
struct AGE_CTRL        AGECtrl;
struct AGE_PROG        AGEProg;
struct CAL_REGS        Calib[MAX_AGE_CHAN];

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

void App_LEDGpioCfg(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);
	
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
	  
	  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15

    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
	

    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    LED1_OFF();
    LED2_OFF();
		LEDR_OFF();
    LEDG_ON();
} 

/******************************************************************
�������ƣ�ManualModifyIP(void)//2014-1-20�޸����������
��������:�޸�IP
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void InitInnerPeripheral()
{
	 App_LEDGpioCfg();
	
	 RTC_LSE_Init();
	 
	 AGE_CtrlDrive_GpioInit();
	 CtrlEN33_64_GpioInit();
	
	 SRAM_PIN_GPIO_Config();
   FSMC_SRAM_Init();	
	 DELAY_Ms(50); 
	
	 Init_AD8688Gpio();
	 ADS8688_Reset();
	 Init_Ads8688();
	
	 MB88346B_Gpio_Init();
	 FPGAIO_T4A_ADDR = 0x01;
}
/******************************************************************
�������ƣ�ManualModifyIP(void)//2014-1-20�޸����������
��������:�޸�IP
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void InitAGESysParameter()
{
   CtrlSchState(WAIT_STATE);
	 ReadConInCurFlashBlock();
   ReadSampleInCurFlashBlock();
   ReadSampleOutCurFlashBlock();
}
/******************************************************************
�������ƣ�ManualModifyIP(void)//2014-1-20�޸����������
��������:�޸�IP
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
void InitAGESysContral()
{
    CtrlAGE_STOP_CTRL();
}


#include "main.h"
#include "MyInclude.h"

static uint16_t Switch_Data[10],LD_DA_Data;
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void AGE_CtrlDrive_GpioInit()
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOC, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOE, ENABLE);
    GPIO_StructInit(&GPIO_InitStruct);
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_15);                     
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource13, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
		
	  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
	
		CD4051_DB0_H;
		CD4051_DB1_H;
    CD4051_DB2_H;		
    CD4051_WCS1_H;
		CD4051_WCS2_H;
		CD4051_EN_H;

}
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void CtrlEN33_64_GpioInit()
{
	  uint16_t i;
    GPIO_InitTypeDef  GPIO_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOC, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOE, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOH, ENABLE);
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_15);                     
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
	
	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_15);                     
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource2, GPIO_AF_15);                     
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
		
		
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_15);                     
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
	  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
	  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_15);                     
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
		
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_15);                     
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
	  GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
	  
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_15);                     
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_15);                       //Disable JTDO/TRACESWO   AF to  AF15
	  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
		
		GPIO_PinAFConfig(GPIOH, GPIO_PinSource2, GPIO_AF_15);                      //Disable JTDI   AF to  AF15
		
    GPIO_InitStruct.GPIO_Pin  = (GPIO_Pin_8 | GPIO_Pin_9|GPIO_Pin_10 | GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15);
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
	  
	  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_3|GPIO_Pin_15 | GPIO_Pin_14| GPIO_Pin_11| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_0| GPIO_Pin_6| GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_9 | GPIO_Pin_8|GPIO_Pin_7 | GPIO_Pin_6|GPIO_Pin_12 | GPIO_Pin_11| GPIO_Pin_10| GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		 GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_3|GPIO_Pin_2 | GPIO_Pin_10|GPIO_Pin_9 | GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_5| GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOH, &GPIO_InitStruct);
		
		QDB_EN33_H;
		QDB_EN34_H;
    QDB_EN35_H;		
    QDB_EN36_H;
		QDB_EN37_H;
		QDB_EN38_H;
    QDB_EN39_H;
		QDB_EN40_H;
    QDB_EN41_H;		
    QDB_EN42_H;
		QDB_EN43_H;
		QDB_EN44_H;
		QDB_EN45_H;
		QDB_EN46_H;
    QDB_EN47_H;		
    QDB_EN48_H;
		QDB_EN49_H;
		QDB_EN50_H;
		QDB_EN51_H;
		QDB_EN52_H;
    QDB_EN53_H;		
    QDB_EN54_H;
		QDB_EN55_H;
		QDB_EN56_H;
		QDB_EN57_H;
		QDB_EN58_H;
    QDB_EN59_H;		
    QDB_EN60_H;
		QDB_EN61_H;
		QDB_EN62_H;
    QDB_EN63_H;
		QDB_EN64_H;
    QDB_ENA_H;
		QDB_ENB_H;
		
		for(i=0;i<10;i++)
		{
		  Switch_Data[i]=0;
		}
}
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void Ctrl_4051ADEN(uint16_t ChanNo)
{
   switch(ChanNo)
	 {
		 case 0:  CD4051_DB0_L;CD4051_DB1_L;CD4051_DB2_L;	
		          break;
		 case 1:  CD4051_DB0_H;CD4051_DB1_L;CD4051_DB2_L;	
		          break;
	   case 2:  CD4051_DB0_L;CD4051_DB1_H;CD4051_DB2_L;	
		          break;
		 case 3:  CD4051_DB0_H;CD4051_DB1_H;CD4051_DB2_L;	
		          break;
		 case 4:  CD4051_DB0_L;CD4051_DB1_L;CD4051_DB2_H;	
		          break;
	   case 5:  CD4051_DB0_H;CD4051_DB1_L;CD4051_DB2_H;	
		          break;
		 case 6:  CD4051_DB0_L;CD4051_DB1_H;CD4051_DB2_H;	
		          break;
	   case 7:  CD4051_DB0_H;CD4051_DB1_H;CD4051_DB2_H;	
		          break;
		 default: CD4051_DB0_H;CD4051_DB1_H;CD4051_DB2_H;	
		          break;
	 }

}
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/

void Ctrl_4051INCH(uint16_t ChanNo)
{
   switch(ChanNo)
	 {
		 case 1:  CD4051_WCS1_H;
		          CD4051_WCS2_L;
		          CD4051_EN_L;
		          break;
		 case 2:  CD4051_WCS1_L;
		          CD4051_WCS2_H;
		          CD4051_EN_L;
		          break;
	   case 3:  CD4051_WCS1_L;
		          CD4051_WCS2_L;
		          CD4051_EN_H;
		          break;
		 default: CD4051_WCS1_L;
		          CD4051_WCS2_L;
		          CD4051_EN_L;
		          break;
	 }
}
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void Ctrl_ON_Switch(uint16_t ChanNo,uint16_t flag)
{
    volatile uint16_t bNo,bit;
    volatile uint16_t csbNo=0;

    bNo=ChanNo/8;
    bit=ChanNo%8;

    switch(bNo)
    {
       case 0:csbNo=ADDR_EN_DBUS;break;
       case 1:csbNo=ADDR_CS_DBUS;break;
       case 2:csbNo=ADDR_DATA_DBUS;break;
       case 3:csbNo=ADDR_PWR_DBUS;break;
       case 4:csbNo=(ADDR_PWR_DBUS+1);break;
       case 5:csbNo=(ADDR_PWR_DBUS+2);break;
       case 6:csbNo=(ADDR_PWR_DBUS+3);break;
       case 7:csbNo=(ADDR_PWR_DBUS+4);break;
       default:break;
    }
		
		if(csbNo<5)
	  {
		   switch(bit)
			 {
					case 0:bit=1;break;
					case 1:bit=0;break;
					case 2:bit=3;break;
					case 3:bit=2;break;
					case 4:bit=5;break;
					case 5:bit=4;break;
					case 6:bit=7;break;
					case 7:bit=6;break;
					default:break;
			 }
			  if(flag==1)
				{
						Switch_Data[csbNo] &= ~(0x01 << bit);	
				}
				else
				{
						Switch_Data[csbNo] |= (0x01 << bit);		  		        
				}
				WriteDataByte(csbNo,Switch_Data[csbNo]);
		}
		else
		{
				Ctrl_EN33_64(ChanNo,flag);
		}	    
}
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void Ctrl_EN33_64(uint16_t ChanNo,uint16_t flag)
{
	
	 if(flag==0)
	 {
	     switch(ChanNo)
			 {
				 case 32:QDB_EN33_H;break;
				 case 33:QDB_EN34_H;break;
				 case 34:QDB_EN35_H;break;
				 case 35:QDB_EN36_H;break;
				 case 36:QDB_EN37_H;break;
				 case 37:QDB_EN38_H;break;
				 case 38:QDB_EN39_H;break;
				 case 39:QDB_EN40_H;break;
				 case 40:QDB_EN41_H;break;
				 case 41:QDB_EN42_H;break;
				 case 42:QDB_EN43_H;break;
				 case 43:QDB_EN44_H;break;
				 case 44:QDB_EN45_H;break;
				 case 45:QDB_EN46_H;break;
				 case 46:QDB_EN47_H;break;
				 case 47:QDB_EN48_H;break;
				 case 48:QDB_EN49_H;break;
				 case 49:QDB_EN50_H;break;
				 case 50:QDB_EN51_H;break;
				 case 51:QDB_EN52_H;break;
				 case 52:QDB_EN53_H;break;
				 case 53:QDB_EN54_H;break;
				 case 54:QDB_EN55_H;break;
				 case 55:QDB_EN56_H;break;
				 case 56:QDB_EN57_H;break;
				 case 57:QDB_EN58_H;break;
				 case 58:QDB_EN59_H;break;
				 case 59:QDB_EN60_H;break;
				 case 60:QDB_EN61_H;break;
				 case 61:QDB_EN62_H;break;
				 case 62:QDB_EN63_H;break;
				 case 63:QDB_EN64_H;break;
				 default:break; 
			 }
	 }
	 else
	 {
	     switch(ChanNo)
			 {
				 case 32:QDB_EN33_L;break;
				 case 33:QDB_EN34_L;break;
				 case 34:QDB_EN35_L;break;
				 case 35:QDB_EN36_L;break;
				 case 36:QDB_EN37_L;break;
				 case 37:QDB_EN38_L;break;
				 case 38:QDB_EN39_L;break;
				 case 39:QDB_EN40_L;break;
				 case 40:QDB_EN41_L;break;
				 case 41:QDB_EN42_L;break;
				 case 42:QDB_EN43_L;break;
				 case 43:QDB_EN44_L;break;
				 case 44:QDB_EN45_L;break;
				 case 45:QDB_EN46_L;break;
				 case 46:QDB_EN47_L;break;
				 case 47:QDB_EN48_L;break;
				 case 48:QDB_EN49_L;break;
				 case 49:QDB_EN50_L;break;
				 case 50:QDB_EN51_L;break;
				 case 51:QDB_EN52_L;break;
				 case 52:QDB_EN53_L;break;
				 case 53:QDB_EN54_L;break;
				 case 54:QDB_EN55_L;break;
				 case 55:QDB_EN56_L;break;
				 case 56:QDB_EN57_L;break;
				 case 57:QDB_EN58_L;break;
				 case 58:QDB_EN59_L;break;
				 case 59:QDB_EN60_L;break;
				 case 60:QDB_EN61_L;break;
				 case 61:QDB_EN62_L;break;
				 case 62:QDB_EN63_L;break;
				 case 63:QDB_EN64_L;break;
				 default:break; 
			 }
	 }
}	
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void DA_LoadData(uint8_t ChanNo,uint8_t flag)
{
	  uint8_t SelChan,bit;

    SelChan=ChanNo;
	
	  switch(SelChan)
	  {
				case 0:bit=1;break;
			  case 1:bit=0;break;
				case 2:bit=3;break;
				case 3:bit=2;break;
				case 4:bit=5;break;
				case 5:bit=4;break;
				case 6:bit=7;break;
				case 7:bit=6;break;
				default:break;
		}
    if(flag==0x01)
    {
        LD_DA_Data |= (0x01 << bit);
    }
    else
    {
        LD_DA_Data &= ~(0x01 << bit);
    }
		
    WriteDataByte(ADDR_IO_DBUS,LD_DA_Data);

}
	
	
	





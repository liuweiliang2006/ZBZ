#include "main.h"
#include "MyInclude.h"

volatile uint16_t watch;
const unsigned int ADS8688_CH_TABLE[8] = {0xc000,0xc400,0xc800,0xcc00,0xd000,0xd400,0xd800,0xdc00};
/******************************************************************
函数名称：Init_AD8688
函数功能：初始化8688相关引脚
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
void Init_AD8688Gpio(void)
{
	  GPIO_InitTypeDef  GPIO_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOE, ENABLE);
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_15);                      
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_15);                       
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_15);
	  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_15);
	  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_15);
	  
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_11 | GPIO_Pin_12|GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_14;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void Init_AD8688(void)
{
	AD_SYNC_H;
	AD_SCLK_H;
	AD_SDI_L;
	ADS8688_Reset();
	//第一路-第八路采样范围
	Write_AD8688(0x0B01);
	Write_AD8688(0x0D01);
	Write_AD8688(0x0F01);
	Write_AD8688(0x1101);
	Write_AD8688(0x1301);
	Write_AD8688(0x1501);
	Write_AD8688(0x1701);
	Write_AD8688(0x1901);
}
/******************************************************************
函数名称：ADS8688_Reset
函数功能：8688重置
输入参数：
输出参数：
返回值：
注意：rst引脚拉低拉高，实现芯片重置
******************************************************************/
void ADS8688_Reset(void)
{
	AD_RST_H;
	DELAY_Ms(5);
	AD_RST_L;
	DELAY_Ms(15);
	AD_RST_H;
	DELAY_Ms(50);
}
/******************************************************************
函数名称：Write_AD8688
函数功能：8688命令写入
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
void Write_AD8688(uint16_t data)
{
  	uint16_t i,j;
  	AD_SYNC_H;
  	DELAY_Ms(1);
  	AD_SYNC_L;//片选信号拉低

		for(i=0;i<16;i++)//写入16位命令
		{
			if((data << i) & 0x8000)
			{
			AD_SDI_H;//din=1
			}else
			{
			AD_SDI_L;//din=0
			}

			AD_SCLK_H;//CLK=1
			j=5;
			while(j--);
			AD_SCLK_L;//CLK =0
			j=5;
			while(j--);
		}
		for(i=0;i<16;i++)//后16个时钟为读取数据，写入命令是直接抛掉
		{
			AD_SCLK_H;//CLK=1
			j=5;
			while(j--);
			AD_SCLK_L;//CLK =0
			j=5;
			while(j--);
		}

  	AD_SYNC_H;//片选信号拉高
  	AD_SCLK_H;

}
/******************************************************************
函数名称：Read_AD8688
函数功能：读取8688管脚
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
uint16_t Read_AD8688(void)
{
	  uint16_t  i,Result=0,j;
  	AD_SYNC_H;
  	DELAY_Ms(2);
  	AD_SYNC_L;//片选信号拉低

		for(i=0;i<16;i++)//前16个时钟为写入命令，读取时直接抛掉
		{
			AD_SDI_L;//确保读取时，写入命令为0；
			AD_SCLK_H;//CLK=1
			j=5;
			while(j--);
			AD_SCLK_L;//CLK =0
			j=5;
			while(j--);
		}
		for(i=0;i<16;i++)//读取十六位数据
		{
			AD_SCLK_H;//CLK =1
			j=5;
			while(j--);
			Result<<=1;
			if(AD_SDO == 1)
			{
				Result++;
			}
			AD_SCLK_L;//CLK =0
			j=5;
			while(j--);
		}

  	AD_SYNC_H;//片选信号拉高
  	AD_SCLK_H;
	  return Result;
}

/******************************************************************
函数名称：ADS8688_READ_Register
函数功能：Samp8688;AUTORST模式扫描采集8路
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
uint16_t ADS8688_READ_Register(uint16_t data)
{
	uint16_t i,j,Result=0;

  	AD_SYNC_H;
  	DELAY_Ms(2);
  	AD_SYNC_L;//片选信号拉低
  	for(i=0;i<16;i++)//写入16位命令
	{
		if((data << i) & 0x8000)
		{
		AD_SDI_H;//din=1
		}else
		{
		AD_SDI_L;//din=0
		}
		AD_SCLK_H;//CLK=1
		j=5;
		while(j--);
		AD_SCLK_L;//CLK =0
		j=5;
		while(j--);
	}
	for(i=0;i<16;i++)//读取十六位数据
	{
		AD_SCLK_H;//CLK =1
		j=5;
		while(j--);

		Result<<=1;
		if(AD_SDO == 1)
		{
			Result++;
		}
		AD_SCLK_L;//CLK =0
		j=5;
		while(j--);
	}
  	AD_SYNC_H;//片选信号拉高
  	AD_SCLK_H;
	return Result;
}

//第二份尝试程序
uint8_t Comu_8688(uint8_t Data,uint8_t Ope)
{
	volatile uint8_t i,Buf;
	if(Ope)
	{
		Buf = Data;
		for(i=0;i<8;i++)
		{
//			Delay_Us(1);
			AD_SCLK_H;
//			Delay_Us(1);
		 	if(Buf&0x80)
				AD_SDI_H;
			else
				AD_SDI_L;
//			Delay_Us(1);
			AD_SCLK_L;
			Buf <<= 1;
		}
		return 1;
	}
	else
	{
		Buf = 0;
		for(i=0;i<8;i++)
			{
				Buf <<=1;
				if(AD_SDO)
					Buf |= 1;					
//				Delay_Us(1);
				
				AD_SCLK_H;
//				Delay_Us(1);
				AD_SCLK_L;
//				Delay_Us(1);

				
			}
	return Buf;	
	}

}

void Init_Ads8688(void)
{
	uint8_t i,j,Num;
	uint8_t Range_Info[8] = {0X01,0X01,0X01,0X01,0X01,0X01,0X01,0X01};
	uint16_t itemp;

  for(Num=0;Num<5;Num++)
	{
		AD_RST_L;
		itemp=10000;
		while(itemp--);
		AD_RST_H;
		itemp=10000;
		while(itemp--);
	//Write_Reg_8688(AD_8688_FETURE_SEL,0x2B);
		itemp=50000;
		while(itemp--);
		
		
	 j=0;
	 for(i=AD_8688_CH0_RAGE;i<=AD_8688_CH7_RAGE;i++)
	 {
		 Write_Reg_8688(i,Range_Info[j]);
		 itemp=500;
		 while(itemp--);
		 watch = Read_Reg_8688(i);
		 itemp=500;
		 while(itemp--);
		 j++;	
	 }
	 //Write_Reg_8688(AD_8688_CH0_RAGE,0x01);
 }
	
}
void Write_Reg_8688(uint8_t Add,uint8_t Data)
{
	uint8_t Buf;
	uint16_t i;
	AD_SYNC_L;	
	i=400;
	while(i--);
	Buf = (Add<<1)+1;
	Comu_8688(Buf,1);
	Comu_8688(Data,1);
	AD_SYNC_H;
}

uint8_t Read_Reg_8688(uint8_t Add)
{
	uint8_t Buf;
	uint16_t i;
	AD_SYNC_L;	
	i=400;
	while(i--);
	Buf = (Add<<1);
	Comu_8688(Buf,1);
	Comu_8688(0,0);
	Buf = Comu_8688(0,0);
	AD_SYNC_H;	
	return Buf;

}

unsigned int Read_8688(unsigned int Samp_Ch)
{
	uint8_t Buf;
	uint16_t Resault,data;
	//Init_Ads8688();
//	Delay_Us(1);
	AD_SYNC_L;
//	Delay_Us(1);
	
	data=(uint16_t)Samp_Ch;
	Buf = (uint8_t)((data>>8)&0xff);
	Comu_8688(Buf,1);
	data=(uint16_t)Samp_Ch;
	Buf = (uint8_t)((data)&0xff);
	//Buf = (uint8_t)Samp_Ch;
	Comu_8688(Buf,1);
	AD_SYNC_H;
//	Delay_Us(1);
	AD_SYNC_L;
//	Delay_Us(1);
	Comu_8688(0X00,0);
	Comu_8688(0,0);
	
	Resault = (Comu_8688(0x00,0));
	Resault <<= 8;
	Resault |= (Comu_8688(0x00,0));
	AD_SYNC_H;
	return (uint32_t)Resault;
}



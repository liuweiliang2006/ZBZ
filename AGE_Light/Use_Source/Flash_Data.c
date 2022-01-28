#include "main.h"
#include "MyInclude.h"

uint16_t ReadFlashTemp[(MAX_AGE_CHAN*MAX_CURRKB_GRADE*4)];
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
void WriteCONInCurKBToFlash(uint16_t *Data,uint16_t len)
{
	  uint16_t itemp,jtemp;

		FLASH_Write_Word(Data, CONCUR_FLASH_B,len);
		
		for(itemp=0;itemp<MAX_AGE_CHAN;itemp++)
		{
			for(jtemp=0;jtemp<MAX_CURRKB_GRADE;jtemp++)
			{
				 Calib[itemp].ConCChar[jtemp].KVal.bytes.lbyte = Data[4*jtemp+(itemp*MAX_CURRKB_GRADE*4)];
				 Calib[itemp].ConCChar[jtemp].KVal.bytes.hbyte = Data[4*jtemp+1+(itemp*MAX_CURRKB_GRADE*4)];
				 Calib[itemp].ConCChar[jtemp].BVal.bytes.lbyte = Data[4*jtemp+2+(itemp*MAX_CURRKB_GRADE*4)];
				 Calib[itemp].ConCChar[jtemp].BVal.bytes.hbyte = Data[4*jtemp+3+(itemp*MAX_CURRKB_GRADE*4)];
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
void WriteSampleInCurKBToFlash(uint16_t *Data,uint16_t len)
{
	  uint16_t itemp,jtemp;
	
		FLASH_Write_Word(Data, SAMCUR_FLASH_B,len);
		
		for(itemp=0;itemp<MAX_AGE_CHAN;itemp++)
		{
			for(jtemp=0;jtemp<MAX_CURRKB_GRADE;jtemp++)
			{
				 Calib[itemp].FeeCChar[jtemp].KVal.bytes.lbyte = Data[4*jtemp+(itemp*MAX_CURRKB_GRADE*4)];
				 Calib[itemp].FeeCChar[jtemp].KVal.bytes.hbyte = Data[4*jtemp+1+(itemp*MAX_CURRKB_GRADE*4)];
				 Calib[itemp].FeeCChar[jtemp].BVal.bytes.lbyte = Data[4*jtemp+2+(itemp*MAX_CURRKB_GRADE*4)];
				 Calib[itemp].FeeCChar[jtemp].BVal.bytes.hbyte = Data[4*jtemp+3+(itemp*MAX_CURRKB_GRADE*4)];
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
void WriteSampleOutCurKBToFlash(uint16_t *Data,uint16_t len)
{
	  uint16_t i;
	  uint16_t itemp,jtemp;

		FLASH_Write_Word(Data, OUTSAMCUR_FLASH_B,len);
		
		for(itemp=0;itemp<MAX_AGE_CHAN;itemp++)
		{
			for(jtemp=0;jtemp<MAX_CURRKB_GRADE;jtemp++)
			{
				 Calib[itemp].FeeOutCChar[jtemp].KVal.bytes.lbyte = Data[4*jtemp+(itemp*MAX_CURRKB_GRADE*4)];
				 Calib[itemp].FeeOutCChar[jtemp].KVal.bytes.hbyte = Data[4*jtemp+1+(itemp*MAX_CURRKB_GRADE*4)];
				 Calib[itemp].FeeOutCChar[jtemp].BVal.bytes.lbyte = Data[4*jtemp+2+(itemp*MAX_CURRKB_GRADE*4)];
				 Calib[itemp].FeeOutCChar[jtemp].BVal.bytes.hbyte = Data[4*jtemp+3+(itemp*MAX_CURRKB_GRADE*4)];
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
/*void WriteTempValueToFlash(uint16_t *source,uint16_t len)
{
	uint16_t *ptr,i;
	for(i=0;i<len;i++)
  {
	   *ptr++=*source++;
	}
	
	FLASH_SIM_WriteEE(ptr, TEMP_FLASH_B, len);
} */
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
void ReadConInCurFlashBlock(void)
{
		uint16_t itemp,jtemp;
		uint32_t Flash_Program_Addr=0;
	
		Flash_Program_Addr = CONCUR_FLASH_B;
		FLASH_Read_Word(&ReadFlashTemp[0], Flash_Program_Addr, (MAX_AGE_CHAN*MAX_CURRKB_GRADE*4*2));
		
		for(itemp=0;itemp<MAX_AGE_CHAN;itemp++)
		{
			for(jtemp=0;jtemp<MAX_CURRKB_GRADE;jtemp++)
			{
				Calib[itemp].ConCChar[jtemp].KVal.bytes.lbyte = ReadFlashTemp[4*jtemp+(itemp*MAX_CURRKB_GRADE*4)];
				Calib[itemp].ConCChar[jtemp].KVal.bytes.hbyte = ReadFlashTemp[4*jtemp+1+(itemp*MAX_CURRKB_GRADE*4)];
				Calib[itemp].ConCChar[jtemp].BVal.bytes.lbyte = ReadFlashTemp[4*jtemp+2+(itemp*MAX_CURRKB_GRADE*4)];
				Calib[itemp].ConCChar[jtemp].BVal.bytes.hbyte = ReadFlashTemp[4*jtemp+3+(itemp*MAX_CURRKB_GRADE*4)];
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
void ReadSampleInCurFlashBlock(void)
{
    uint16_t itemp,jtemp;
		uint32_t Flash_Program_Addr=0;
	
		Flash_Program_Addr = SAMCUR_FLASH_B;
		FLASH_Read_Word(&ReadFlashTemp[0], Flash_Program_Addr, (MAX_AGE_CHAN*MAX_CURRKB_GRADE*4*2));
	
		for(itemp=0;itemp<(MAX_AGE_CHAN*MAX_CURRKB_GRADE*4);itemp++)
		{
		   ReadFlashTemp[itemp]=(ReadFlashTemp [(itemp)] & 0xFFFF);
		}
		
		for(itemp=0;itemp<MAX_AGE_CHAN;itemp++)
		{
			for(jtemp=0;jtemp<MAX_CURRKB_GRADE;jtemp++)
			{
				Calib[itemp].FeeCChar[jtemp].KVal.bytes.lbyte  = ReadFlashTemp[4*jtemp+(itemp*MAX_CURRKB_GRADE*4)];
				Calib[itemp].FeeCChar[jtemp].KVal.bytes.hbyte  = ReadFlashTemp[4*jtemp+1+(itemp*MAX_CURRKB_GRADE*4)];
				Calib[itemp].FeeCChar[jtemp].BVal.bytes.lbyte  = ReadFlashTemp[4*jtemp+2+(itemp*MAX_CURRKB_GRADE*4)];
				Calib[itemp].FeeCChar[jtemp].BVal.bytes.hbyte  = ReadFlashTemp[4*jtemp+3+(itemp*MAX_CURRKB_GRADE*4)];
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
void ReadSampleOutCurFlashBlock(void)
{
    uint16_t itemp,jtemp,UseFlag=0;
		uint32_t Flash_Program_Addr=0;
	
		Flash_Program_Addr = OUTSAMCUR_FLASH_B;
		FLASH_Read_Word(&ReadFlashTemp[0], Flash_Program_Addr, (MAX_AGE_CHAN*MAX_CURRKB_GRADE*4*2));
	
		for(itemp=0;itemp<(MAX_AGE_CHAN*MAX_CURRKB_GRADE*4);itemp++)
		{
		   ReadFlashTemp[itemp]=(ReadFlashTemp [(itemp)] & 0xFFFF);
		}
		
		for(itemp=0;itemp<MAX_AGE_CHAN;itemp++)
		{
			for(jtemp=0;jtemp<MAX_CURRKB_GRADE;jtemp++)
			{
				Calib[itemp].FeeOutCChar[jtemp].KVal.bytes.lbyte  = ReadFlashTemp[4*jtemp+(itemp*MAX_CURRKB_GRADE*4)];
				Calib[itemp].FeeOutCChar[jtemp].KVal.bytes.hbyte  = ReadFlashTemp[4*jtemp+1+(itemp*MAX_CURRKB_GRADE*4)];
				Calib[itemp].FeeOutCChar[jtemp].BVal.bytes.lbyte  = ReadFlashTemp[4*jtemp+2+(itemp*MAX_CURRKB_GRADE*4)];
				Calib[itemp].FeeOutCChar[jtemp].BVal.bytes.hbyte  = ReadFlashTemp[4*jtemp+3+(itemp*MAX_CURRKB_GRADE*4)];
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
/*void ReadTempFlashBlock(uint16_t *pdata, uint16_t Length)
{
    uint16_t i;

    for (i = 0; i < Length; i++)
    {
        *pdata++ = *((volatile uint16_t*)(TEMP_FLASH_B+i));
    }
} */



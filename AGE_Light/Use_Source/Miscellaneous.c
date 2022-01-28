#include "main.h"
#include "MyInclude.h"

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
void ClearProgramSRAM(uint16_t *ptr,uint32_t len)
{
	uint16_t  *dst;
	uint32_t  i;

	dst = ptr;

	for(i=0;i<len;i++)
	{
		*dst++ = 0;
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
void MoveDataFromSRAMToSRAM(uint16_t *source,uint16_t *dest,uint32_t len)
{
	uint32_t  i;
	uint16_t  *src,*dst;

	src = source;
	dst = dest;

	for(i=0;i<len;i++)
	{
		*dst++ = *src++;
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
void ClearVarParam()
{
   uint16_t i;

   AGECtrl.bflag.all &= 0x0000;

   AGEVar.StepTimeCnt.data = 0;
	 AGEVar.ProtectTimeCnt.data=0;
   AGEVar.PTMmtState = MmtSTOPSta;

   for(i=0;i<MAX_AGE_CHAN;i++)
   {
		   AGEVar.PTVoltage[i].data = 0;
		   AGEVar.WARNVFormData[i]=0;
       AGEVar.IN_PTCurrent[i] = 0;
		   AGEVar.WARNINCFormData[i] = 0x00;
		   AGEVar.OUT_PTCurrent[i] = 0;       
		   AGEVar.WARNOUTCFormData[i] = 0x00;
		 
		   AGECtrl.IINUpWnCnt[i]=0;
	     AGECtrl.IINDoWnCnt[i]=0;
	
	     AGECtrl.IOUTUpWnCnt[i]=0;
	     AGECtrl.IOUTDoWnCnt[i]=0;
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
void ClearProgParam()
{
   AGEProg.ProgType = NOMAL_MODE;
	
   AGEProg.StepMode=0;
   AGEProg.Con_EndValTime_RUN.data = 0;
   SysCtrl.ChanSelL.data=0;
	 SysCtrl.ChanSelH.data=0;
	 
   AGEProg.Con_HighVoltage.data=0;
   AGEProg.Con_LowVoltage.data=0;	
	 //稳态
	 AGEProg.Con_CurrentIN_RUN=0;
	 AGEProg.Con_UPCurrentIN_RUN=0;
	 AGEProg.Con_DownCurrentIN_RUN=0;
	 AGEProg.Con_UPCurrentOUT_RUN=0;
	 AGEProg.Con_DownCurrentOUT_RUN=0;
	 AGEProg.Con_UPVolIN_RUN.data=0;
	 AGEProg.Con_DownVolIN_RUN.data=0;
	 AGEProg.Con_Power_RUN.data=0;
	 AGEProg.Con_Powerfluct_RUN.data=0;
}




 







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
void SaveSystemData(void)
{
	
	uint16_t  j=0,num=0,num1=0;
	//uint32_t  sumparity1=0,sumparity2=0;
	union DATA_BYTE Step_Time,Step_Time1;
	union DATA_BYTE SetPowerFluct;
	float time_Cnt=0,time_Cnt1=0;
	uint16_t Second,Second1,Minute,Minute1,Hour,Hour1,Day,Day1,Momth,Momth1,Year,Year1;

	if(SysCtrl.DataPack_Flag==0)
	{
	   if(AGECtrl.bflag.bit.PGRN ==1)
	   {
	       SysCtrl.DataSaveA[num++] = DATA_VAL;
	   }
	   else
	   {
	       SysCtrl.DataSaveA[num++] = DATA_NOVAL;
	   }

		 SysCtrl.DataSaveA[num++]=SysCtrl.System_DCPower.bytes.lbyte;
	   SysCtrl.DataSaveA[num++]=SysCtrl.System_DCPower.bytes.lbyte;
		 
	   Second=AGECtrl.SampTime.Second;
	   Minute=AGECtrl.SampTime.Minute;
	   Hour=AGECtrl.SampTime.Hour;
	   Day=AGECtrl.SampTime.Day;
	   Momth=AGECtrl.SampTime.Momth;
	   Year=AGECtrl.SampTime.Year;

	   SysCtrl.DataSaveA[num++] = (Second + (Minute << 8));
	   SysCtrl.DataSaveA[num++] = (Hour + (Day << 8));
	   SysCtrl.DataSaveA[num++] = (Momth+((Year%100)<<8));

	   SysCtrl.DataSaveA[num++] = (Year/100);

	   time_Cnt=(float)(AGEVar.StepTimeCnt.data);
	   Step_Time.data=(uint32_t)(time_Cnt);

	   SysCtrl.DataSaveA[num++] = Step_Time.bytes.lbyte;
	   SysCtrl.DataSaveA[num++] = Step_Time.bytes.hbyte;

	   SysCtrl.DataSaveA[num++] = AGEVar.StepState;

	   SysCtrl.DataSaveA[num++] = AGEVar.SchState;
		 
		 SysCtrl.DataSaveA[num++] = AGEVar.BoardState;
		 
		 SetPowerFluct.data=AGEProg.Con_Powerfluct_RUN.data;
		 if(AGECtrl.bflag.bit.PGRN==1)
		 {
		   if(AGEVar.StepState==CURR_CTRL)
			 {
			    SetPowerFluct.data=0;
			 }
		 }
		 else
		 {
		    SetPowerFluct.data=0;
		 }
		 SysCtrl.DataSaveA[num++]=SetPowerFluct.bytes.lbyte;
		 SysCtrl.DataSaveA[num++]=SetPowerFluct.bytes.hbyte;

     for(j=0;j<MAX_VOL_CHAN;j++)
		 {
				 SysCtrl.DataSaveA[num++] = AGEVar.PTVoltage[j].bytes.lbyte;
				 SysCtrl.DataSaveA[num++] = AGEVar.PTVoltage[j].bytes.hbyte;
		 }
		 
		 for(j=0;j<MAX_VOL_CHAN;j++)
		 {
			   SysCtrl.DataSaveA[num++] = AGEVar.WARNVFormData[j];		 
		 }

		 for(j=0;j<MAX_AGE_CHAN;j++)
		 {
				 SysCtrl.DataSaveA[num++] = AGEVar.IN_PTCurrent[j];			 
		 }
		 for(j=0;j<MAX_AGE_CHAN;j++)
		 {
		    SysCtrl.DataSaveA[num++] = AGEVar.WARNINCFormData[j];
		 }
		 
		 for(j=0;j<MAX_AGE_CHAN;j++)
		 {
				 SysCtrl.DataSaveA[num++] = AGEVar.OUT_PTCurrent[j];			 
		 }
		 
		 for(j=0;j<MAX_AGE_CHAN;j++)
		 {
		    SysCtrl.DataSaveA[num++] = AGEVar.WARNOUTCFormData[j];
		 }
		 
		// sumparity1=Crs(SysCtrl.DataSaveA,(SINGLE_PACKAGE_WORDS-2));
		// SysCtrl.DataSaveA[num++]=(sumparity1 & 0x0000ffff);
		// SysCtrl.DataSaveA[num]=(sumparity1 >> 16);

	}
	else if(SysCtrl.DataPack_Flag==1)
	{

	   if(AGECtrl.bflag.bit.PGRN ==1)
	   {
	       SysCtrl.DataSaveB[num1++] = DATA_VAL;
	   }
	   else
	   {
	       SysCtrl.DataSaveB[num1++] = DATA_NOVAL;
	   }

		 SysCtrl.DataSaveB[num1++]=SysCtrl.System_DCPower.bytes.lbyte;
	   SysCtrl.DataSaveB[num1++]=SysCtrl.System_DCPower.bytes.lbyte;
		 
	   Second1=AGECtrl.SampTime.Second;
	   Minute1=AGECtrl.SampTime.Minute;
	   Hour1=AGECtrl.SampTime.Hour;
	   Day1=AGECtrl.SampTime.Day;
	   Momth1=AGECtrl.SampTime.Momth;
	   Year1=AGECtrl.SampTime.Year;

	   SysCtrl.DataSaveB[num1++] = (Second1 + (Minute1 << 8));
	   SysCtrl.DataSaveB[num1++] = (Hour1 + (Day1 << 8));
	   SysCtrl.DataSaveB[num1++] = (Momth1+((Year1%100)<<8));

	   SysCtrl.DataSaveB[num1++] = (Year1/100);

	   time_Cnt1=(float)(AGEVar.StepTimeCnt.data);
	   Step_Time1.data=(uint32_t)(time_Cnt1);

	   SysCtrl.DataSaveB[num1++] = Step_Time1.bytes.lbyte;
	   SysCtrl.DataSaveB[num1++] = Step_Time1.bytes.hbyte;

	   SysCtrl.DataSaveB[num1++] = AGEVar.StepState;

	   SysCtrl.DataSaveB[num1++] = AGEVar.SchState;
		 
		 SysCtrl.DataSaveB[num1++] = AGEVar.BoardState;
		 
		 SetPowerFluct.data=AGEProg.Con_Powerfluct_RUN.data;
		 if(AGECtrl.bflag.bit.PGRN==1)
		 {
		   if(AGEVar.StepState==CURR_CTRL)
			 {
			    SetPowerFluct.data=0;
			 }
		 }
		 else
		 {
		    SetPowerFluct.data=0;
		 }
		 SysCtrl.DataSaveB[num1++]=SetPowerFluct.bytes.lbyte;
		 SysCtrl.DataSaveB[num1++]=SetPowerFluct.bytes.hbyte;

     for(j=0;j<MAX_VOL_CHAN;j++)
		 {
				 SysCtrl.DataSaveB[num1++] = AGEVar.PTVoltage[j].bytes.lbyte;
				 SysCtrl.DataSaveB[num1++] = AGEVar.PTVoltage[j].bytes.hbyte;
		 }
		 
		 for(j=0;j<MAX_VOL_CHAN;j++)
		 {
			   SysCtrl.DataSaveB[num1++] = AGEVar.WARNVFormData[j];		 
		 }

		 for(j=0;j<MAX_AGE_CHAN;j++)
		 {
				 SysCtrl.DataSaveB[num1++] = AGEVar.IN_PTCurrent[j];			 
		 }
		 
		 for(j=0;j<MAX_AGE_CHAN;j++)
		 {
		    SysCtrl.DataSaveB[num1++] = AGEVar.WARNINCFormData[j];
		 }
		 
		 for(j=0;j<MAX_AGE_CHAN;j++)
		 {
				 SysCtrl.DataSaveB[num1++] = AGEVar.OUT_PTCurrent[j];			 
		 }
		 
		 for(j=0;j<MAX_AGE_CHAN;j++)
		 {
		    SysCtrl.DataSaveB[num1++] = AGEVar.WARNOUTCFormData[j];
		 }
		 
		 //sumparity2=Crs(SysCtrl.DataSaveB,(SINGLE_PACKAGE_WORDS-2));
		 //SysCtrl.DataSaveB[num1++]=(sumparity2 & 0x0000ffff);
		 //SysCtrl.DataSaveB[num1]=(sumparity2 >> 16); 
	}
}





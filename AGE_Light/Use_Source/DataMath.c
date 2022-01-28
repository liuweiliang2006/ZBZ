#include "main.h"
#include "MyInclude.h"

const uint16_t CURROUT_DATA[64]={
         0,16,32,48,64,80,96,112,
         1,17,33,49,65,81,97,113,
         2,18,34,50,66,82,98,114,
         3,19,35,51,67,83,99,115,

         4,20,36,52,68,84,100,116,
         5,21,37,53,69,85,101,117,
         6,22,38,54,70,86,102,118,
         7,23,39,55,71,87,103,119,
};

const uint16_t CURRIN_DATA[64]={
         8,24,40,56,72,88,104,120,
         9,25,41,57,73,89,105,121,
         10,26,42,58,74,90,106,122,
         11,27,43,59,75,91,107,123,

         12,28,44,60,76,92,108,124,
         13,29,45,61,77,93,109,125,
         14,30,46,62,78,94,110,126,
         15,31,47,63,79,95,111,127,
}; 
const uint16_t VOL_DATA[8]={
	 0,1,2,3,4,5,6,7
};
const float VoltFeedBackRef =  1.5625;// (1.25*4096*MUL_VOL)/32768

const float CurrFeedBackRef = 0.520833;//(((1.25*4096)*MUL_CURR)/32768/30/1)

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
void CaliChannelTime()
{
  // RTC_Get();
	 CalcDeltOfTime();
   TimeCopy(0);//0表示采样时间
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
void CaliChannelCurrIN(uint16_t ChanNo)
{
   float TempVol,vol,Curr;
   uint32_t sum,Average,Tempsample[10];
   uint16_t i,Curr_Data;

   for(i=0;i<SAMPLECNT;i++)
   {
       sum= SysCtrl.Sample_temp[CURRIN_DATA[ChanNo]][i].data;

       if(sum & 0x8000)
       {
           sum-=32767;
           vol = sum;  //vol为AD采样回来的码段
           TempVol = vol*CurrFeedBackRef;
       }
       else
       {
           vol = (32767-sum);  //vol为AD采样回来的码段
           TempVol = vol*CurrFeedBackRef;
       }
       Tempsample[i]=(uint32_t)(TempVol);
   }
   Average = SortAndFilter(&Tempsample[0],SAMPLECNT);
   Curr=(float)Average;

   if(AGEProg.ProgType!=CALI_MODE)
   {
      Curr=FeedBackCalibration_i(Curr,ChanNo);
   }

   Curr_Data = (uint16_t)(Curr);
	 AGEVar.IN_PTCurrent[ChanNo]=Curr_Data;
   //AGEVar.IN_PTCurrent[ChanNo]=CurrFilter(Curr_Data,ChanNo);
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
void CaliChannelCurrOUT(uint16_t ChanNo)
{
    float TempVol,vol,Curr;
    uint32_t sum,Average,Tempsample[15];
    uint16_t i,Curr_Data,Cnt;

   for(i=0;i<SAMPLECNT;i++)
   {
		   Cnt=CURROUT_DATA[ChanNo];
       sum= SysCtrl.Sample_temp[Cnt][i].data;

       if(sum & 0x8000)
       {
           sum-=32767;
           vol = sum;  //vol为AD采样回来的码段
           TempVol = vol*CurrFeedBackRef;
       }
       else
       {
           vol = (32767-sum);  //vol为AD采样回来的码段
           TempVol = vol*CurrFeedBackRef;
       }
       Tempsample[i]=(uint32_t)(TempVol);
   }
   Average = SortAndFilter(&Tempsample[0],SAMPLECNT);
   Curr=(float)Average;

   if(AGEProg.ProgType!=CALI_MODE)
   {
      Curr=FeedBackOutCurCalibration_i(Curr,ChanNo);
   }

   Curr_Data = (uint16_t)(Curr);
	 AGEVar.OUT_PTCurrent[ChanNo]=Curr_Data;
  // AGEVar.OUT_PTCurrent[ChanNo]=CurrFilter(Curr_Data,ChanNo);

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
void CaliChannelVolPower(uint16_t ChanNo)
{
   float TempVol,vol;
   uint32_t sum,Average,Tempsample[15];
   volatile uint16_t i,Sel_ChanNO;

   Sel_ChanNO=ChanNo;

   for(i=0;i<SAMPLECNT;i++)
   {
       sum= SysCtrl.Sample_Voltemp[VOL_DATA[ChanNo]][i].data;      
       if(sum & 0x8000)
       {
           sum-=32767;
           vol = sum;  //vol为AD采样回来的码段
           TempVol = vol*VoltFeedBackRef;
           AGECtrl.bflag.bit.VCPL=0;
				   
       }
       else
       {
           vol = (32767-sum);  //vol为AD采样回来的码段
           TempVol = vol*VoltFeedBackRef;
           AGECtrl.bflag.bit.VCPL=1;
				   
       }
       Tempsample[i]=(uint32_t)(TempVol);
   }
   Average = SortAndFilter(&Tempsample[0],SAMPLECNT);
	 switch(ChanNo)
	 {
		 case 0: SysCtrl.System_VFPower.data = Average;
						 if(1 == AGECtrl.bflag.bit.VCPL)
						 {
								SysCtrl.System_VFPower.data |= 0x80000000;
						 }
						 break;
		 case 1: SysCtrl.System_VCPower.data = Average;
						 if(1 == AGECtrl.bflag.bit.VCPL)
						 {
								SysCtrl.System_VCPower.data |= 0x80000000;
						 }
						 break;
		 case 2: SysCtrl.System_VCEPower.data = Average;
						 if(1 == AGECtrl.bflag.bit.VCPL)
						 {
								SysCtrl.System_VCEPower.data |= 0x80000000;
						 }
						 break;
		 case 3:break;
		 case 4:break;
		 case 5:break;
		 case 6:break;
		 case 7:break;
	   default:break;
	 
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
void CaliChannelVol(uint16_t ChanNo)
{
	 float  Curr;

	 if(AGEVar.StepState==CURR_CTRL)
	 {
			 if(AGECtrl.Ctrl_AGE_RUN[ChanNo]==1)
			 {
					AGEVar.PTVoltage[ChanNo].data=(uint32_t)(SysCtrl.System_VFPower.data);
			 }
			 else
			 {
					AGEVar.IN_PTCurrent[ChanNo] = 0;
					AGEVar.OUT_PTCurrent[ChanNo] = 0;
					AGEVar.PTVoltage[ChanNo].data=0;
			 }	     
	 }
	 else if(AGEVar.StepState==POWER_CTRL)
	 {
			 if(AGECtrl.Ctrl_AGE_RUN[ChanNo]==1)
			 {
				 Curr=(float)(AGEVar.OUT_PTCurrent[ChanNo]/100);
				 AGEVar.PTVoltage[ChanNo].data=(uint32_t)(((SysCtrl.System_VCPower.data/10)-(Curr*25))*10);
			 }
			 else
			 {
					AGEVar.IN_PTCurrent[ChanNo] = 0;
					AGEVar.OUT_PTCurrent[ChanNo] = 0;
					AGEVar.PTVoltage[ChanNo].data=0;
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
void CaliChannelALL_Data()
{
    uint16_t i;

    for(i=0;i<MAX_AGE_CHAN;i++)
    {
        CaliChannelCurrIN(i);
    }
		
		for(i=0;i<MAX_AGE_CHAN;i++)
    {
        CaliChannelCurrOUT(i);
    }

    for(i=0;i<8;i++)
    {
        CaliChannelVolPower(i);
    }
		
		if(AGECtrl.bflag.bit.PGRN==0)
    {
       for(i=0;i<MAX_VOL_CHAN;i++)
       {
           AGEVar.PTVoltage[i].data=0;
       }
			 for(i=0;i<MAX_AGE_CHAN;i++)
			 {
					AGEVar.IN_PTCurrent[i] = 0;
				  AGEVar.OUT_PTCurrent[i] = 0;
			 }
    }
    else if(AGECtrl.bflag.bit.PGRN==1)
    {
			for(i=0;i<MAX_VOL_CHAN;i++)
			{
					CaliChannelVol(i);
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
void MathChannelData()
{
	uint16_t  FuncDelay,i;

	switch(AGEVar.SchState)
  {
			case WAIT_STATE:AGEVar.deltime_s = 0;     break;

			case READY_STATE:AGEVar.deltime_s = 0;    break;

			case RUNNING_STATE:	                      break;

			case PAUSE_STATE:AGEVar.deltime_s = 0;    break;

			case COMPLATE_STATE: AGEVar.deltime_s = 0;break;

			case WARNSTOP_STATE:AGEVar.deltime_s = 0; break;

			case STOP_STATE:    AGEVar.deltime_s = 0;  break;

			default:break;
  }

	AGEVar.StepTimeCnt.data += AGEVar.deltime_s;
	AGEVar.ProtectTimeCnt.data += AGEVar.deltime_s;
	
  CaliChannelALL_Data();
	
	if(AGECtrl.bflag.bit.PGRN == 1 && AGECtrl.bflag.bit.SFEN == 0)
	{
		FuncDelay = 5;

		if(AGEVar.ProtectTimeCnt.data >= FuncDelay)
		{
		    AGECtrl.bflag.bit.PTEN =1;
		    AGECtrl.bflag.bit.SFEN = 1;
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

void CalcDeltOfTime()
{
	uint16_t  time_1,time_2,time_sum=0;
  
	time_1 =  SysCtrl.LastSec; 
	time_2 =  SysCtrl.LastMin;
		
	if(time_1 > calendar.sec)
	{
		time_sum = 60 - time_1 + calendar.sec;
		if(calendar.min > time_2)
		{
			 time_sum += (calendar.min - time_2 - 1) * 60;
		}
		else
		{
			 time_sum += (calendar.min) * 60;
			 // error
		}
	}
	else
	{
		time_sum = calendar.sec - time_1;
		if(calendar.min >= time_2)
		{
			 time_sum += (calendar.min - time_2) * 60;
		}
		else
		{
			 time_sum += (calendar.min) * 60;
			 // error code
		}
	} 
	
	AGEVar.deltime_s=time_sum;
		
	SysCtrl.LastSec=calendar.sec;
	SysCtrl.LastMin=calendar.min;

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
void TimeCopy(uint16_t flag)
{
	if(flag == 0)
	{
	    AGECtrl.SampTime.Second  = calendar.sec;
	    AGECtrl.SampTime.Minute  = calendar.min;
	    AGECtrl.SampTime.Hour    = calendar.hour;
	    AGECtrl.SampTime.Day     = calendar.w_date;
	    AGECtrl.SampTime.Momth   = calendar.w_month;
	    AGECtrl.SampTime.Year    = calendar.w_year;
	}
	else
	{
		if(flag == 1)
		{
		    AGECtrl.ProgTime.Second  = calendar.sec;
		}
		else
		{
			if(flag == 2)
			{
			    AGECtrl.StepTime.Second  = calendar.sec;
			}
		}
	}
}


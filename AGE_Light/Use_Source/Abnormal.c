#include "main.h"
#include "MyInclude.h"

////////////////////////////////////////////////////////////////////////////////////////
//	AGE异常处理
////////////////////////////////////////////////////////////////////////////////////////
void AGEAbnormalHandle()
{
			if(AGEVar.SchState == PAUSE_STATE)
			{
				return;
			}

	    // 延迟保护参数
			if(AGECtrl.bflag.bit.SFEN == 1)// 通道用的
			{		  
					 POWERVolAbnormalHandle();
				   switch(AGEVar.StepState)
					 {
							 case CURR_CTRL :    CurrentINAbnormalHandle();
																	 break;
							 case POWER_CTRL :   VoltageAbnormalHandle();
					                         CurrentOUTAbnormalHandle();
																	 break;
							 default  :          AGEVar.PTMmtState = MmtSTOPSta;;
													         break;
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
void POWERVolAbnormalHandle()
{





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
void VoltageAbnormalHandle()
{
    uint32_t  vin,vwn;
    uint16_t  ChanNo;

    if(AGEVar.PTMmtState == MmtRUNSta)
    {
        if(AGECtrl.bflag.bit.PTEN==1)
        {
           for(ChanNo=0;ChanNo<MAX_VOL_CHAN;ChanNo++)
           {
               vin = AGEVar.PTVoltage[ChanNo].data;
               if(AGEVar.PTVoltage[ChanNo].data >= 0x80000000)
               {
                   vin = (AGEVar.PTVoltage[ChanNo].data ^ 0x7fffffff) + 1;
               }

               vwn = AGEProg.Con_UPVolIN_RUN.data;
               if(AGEProg.Con_UPVolIN_RUN.data >= 0x80000000)
               {
                   vwn = (AGEProg.Con_UPVolIN_RUN.data ^ 0x7fffffff) + 1;
               }

               if(vin >= vwn)
               {
                   AGECtrl.VUpWnCnt[ChanNo]++;

                   if(AGECtrl.VUpWnCnt[ChanNo] >= MAX_AL_CNT)
                   {
                       AGEVar.WARNVFormData[ChanNo] = HIGHVol_AL;
                       AGECtrl.VUpWnCnt[ChanNo] = 0;  
										 
											 SetMB88346BSingle(0,ChanNo);
											 Ctrl_ON_Switch(ChanNo,0);
											 AGECtrl.Ctrl_AGE_RUN[ChanNo]=0;                                   
                   }
               }
               else
               {
                   AGECtrl.VUpWnCnt[ChanNo] = 0;
               }

               vwn = AGEProg.Con_DownVolIN_RUN.data;
               if(AGEProg.Con_DownVolIN_RUN.data >= 0x80000000)
               {
                   vwn = (AGEProg.Con_DownVolIN_RUN.data ^ 0x7fffffff) + 1;
               }

               if(vin <= vwn)
               {
                   AGECtrl.VDwWnCnt[ChanNo]++;

                   if(AGECtrl.VDwWnCnt[ChanNo] >= MAX_AL_CNT)
                   {
                       AGEVar.WARNVFormData[ChanNo] = LOWVol_AL;
                       AGECtrl.VDwWnCnt[ChanNo] = 0;      
										 
                       SetMB88346BSingle(0,ChanNo);
											 Ctrl_ON_Switch(ChanNo,0);
											 AGECtrl.Ctrl_AGE_RUN[ChanNo]=0;  
                   }
               }
               else
               {
                   AGECtrl.VDwWnCnt[ChanNo] = 0;
               }
           }
        }
        else
        {
            AGECtrl.VUpWnCnt[ChanNo]=0;
            AGECtrl.VDwWnCnt[ChanNo] = 0;
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
void CurrentINAbnormalHandle()
{
    uint16_t  curin,curup,curdown;
    uint16_t ChanNo,i,Cnt=0,j;

    curup = (AGEProg.Con_UPCurrentIN_RUN & 0x7fff);
    curdown= (AGEProg.Con_DownCurrentIN_RUN & 0x7fff);
      

    if(AGEVar.PTMmtState == MmtRUNSta)
    {
       for(ChanNo=0;ChanNo<MAX_AGE_CHAN;ChanNo++)
       {
           if((AGECtrl.Ctrl_AGE_RUN[ChanNo]==1)&&(AGECtrl.bflag.bit.PTEN==1))
           {
              curin=(AGEVar.IN_PTCurrent[ChanNo] & 0x7fff);

              if(curin >= curup)
              {
                 AGECtrl.IINUpWnCnt[ChanNo]++;                                               
								 if(AGECtrl.IINUpWnCnt[ChanNo] >=(MAX_AL_CNT))
								 {
											AGEVar.WARNINCFormData[ChanNo] = HIGHCUR_AL;
											AGECtrl.IINUpWnCnt[ChanNo] = 0;
											Ctrl_ON_Switch(ChanNo,0);
											AGECtrl.Ctrl_AGE_RUN[ChanNo]=0;
								 }               
              }
              else
              {
                 AGECtrl.IINUpWnCnt[ChanNo] = 0;
              }
           }
           else
           {
               AGECtrl.IINUpWnCnt[ChanNo] = 0;
           }
      }

        for(ChanNo=0;ChanNo<MAX_AGE_CHAN;ChanNo++)
        {
            if((AGECtrl.Ctrl_AGE_RUN[ChanNo]==1)&&(AGECtrl.bflag.bit.PTEN==1))
            {
                curin=(AGEVar.IN_PTCurrent[ChanNo] & 0x7fff);
                if(curin <= curdown)
                {
                    AGECtrl.IINDoWnCnt[ChanNo]++;
                                 
									 if(AGECtrl.IINDoWnCnt[ChanNo] >= (MAX_AL_CNT))
									 {
											 AGEVar.WARNINCFormData[ChanNo] = LOWCUR_AL;
											 AGECtrl.IINDoWnCnt[ChanNo] = 0;
											 Ctrl_ON_Switch(ChanNo,0);
											 AGECtrl.Ctrl_AGE_RUN[ChanNo]=0;
									 }
                    
                }
                else
                {
                    AGECtrl.IINDoWnCnt[ChanNo] = 0;
                }
            }
            else
            {
                AGECtrl.IINDoWnCnt[ChanNo] = 0;
            }
        }
        for(i=0;i<MAX_AGE_CHAN;i++)
        {
            if(AGECtrl.Ctrl_AGE_RUN[i]==0)
            {
                Cnt++;
            }
        }

        if(Cnt>=MAX_AGE_CHAN)
        {
            for(j=0;j<MAX_AGE_CHAN;j++)
            {
                if(AGEVar.WARNINCFormData[j] ==LOWCUR_AL||AGEVar.WARNINCFormData[j] ==HIGHCUR_AL)
                {
                    CtrlSchState(WARNSTOP_STATE);
                    break;
                }
            }
        }
    }
    else
    {
        for(ChanNo=0;ChanNo<MAX_AGE_CHAN;ChanNo++)
        {
           AGECtrl.IINUpWnCnt[ChanNo] = 0;
           AGECtrl.IINDoWnCnt[ChanNo] = 0;
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
void CurrentOUTAbnormalHandle()
{
    uint16_t  curin,curup,curdown;
    uint16_t ChanNo,i,Cnt=0,j;

    curup = (AGEProg.Con_UPCurrentOUT_RUN & 0x7fff);
    curdown= (AGEProg.Con_DownCurrentOUT_RUN & 0x7fff);
      

    if(AGEVar.PTMmtState == MmtRUNSta)
    {
       for(ChanNo=0;ChanNo<MAX_AGE_CHAN;ChanNo++)
       {
           if((AGECtrl.Ctrl_AGE_RUN[ChanNo]==1)&&(AGECtrl.bflag.bit.PTEN==1))
           {
              curin=(AGEVar.OUT_PTCurrent[ChanNo] & 0x7fff);

              if(curin >= curup)
              {
                 AGECtrl.IOUTUpWnCnt[ChanNo]++;

                 if(AGECtrl.IOUTUpWnCnt[ChanNo] >=MAX_AL_CNT)
                 {                                                    
										AGEVar.WARNINCFormData[ChanNo] = HIGHCUR_AL;
										AGECtrl.IOUTUpWnCnt[ChanNo] = 0;
										Ctrl_ON_Switch(ChanNo,0);
										AGECtrl.Ctrl_AGE_RUN[ChanNo]=0;                  
                 }
              }
              else
              {
                 AGECtrl.IOUTUpWnCnt[ChanNo] = 0;
              }
           }
           else
           {
               AGECtrl.IOUTUpWnCnt[ChanNo] = 0;
           }
      }

        for(ChanNo=0;ChanNo<MAX_AGE_CHAN;ChanNo++)
        {
            if((AGECtrl.Ctrl_AGE_RUN[ChanNo]==1)&&(AGECtrl.bflag.bit.PTEN==1))
            {
                curin=(AGEVar.OUT_PTCurrent[ChanNo] & 0x7fff);
                if(curin <= curdown)
                {
                    AGECtrl.IOUTDoWnCnt[ChanNo]++;

                    if(AGECtrl.IOUTDoWnCnt[ChanNo] >= MAX_AL_CNT)
                    {                                          
											 AGEVar.WARNINCFormData[ChanNo] = LOWCUR_AL;
											 AGECtrl.IOUTDoWnCnt[ChanNo] = 0;
											 Ctrl_ON_Switch(ChanNo,0);
											 AGECtrl.Ctrl_AGE_RUN[ChanNo]=0;                      
                    }
                }
                else
                {
                    AGECtrl.IOUTDoWnCnt[ChanNo] = 0;
                }
            }
            else
            {
                AGECtrl.IOUTDoWnCnt[ChanNo] = 0;
            }
        }
        for(i=0;i<MAX_AGE_CHAN;i++)
        {
            if(AGECtrl.Ctrl_AGE_RUN[i]==0)
            {
                Cnt++;
            }
        }

        if(Cnt>=MAX_AGE_CHAN)
        {
            for(j=0;j<MAX_AGE_CHAN;j++)
            {
                if(AGEVar.WARNINCFormData[j] ==LOWCUR_AL||AGEVar.WARNINCFormData[j] ==HIGHCUR_AL)
                {
                    CtrlSchState(WARNSTOP_STATE);
                    break;
                }
            }
        }
    }
    else
    {
        for(ChanNo=0;ChanNo<MAX_AGE_CHAN;ChanNo++)
        {
           AGECtrl.IOUTUpWnCnt[ChanNo] = 0;
           AGECtrl.IOUTDoWnCnt[ChanNo] = 0;
        }

    }
} 

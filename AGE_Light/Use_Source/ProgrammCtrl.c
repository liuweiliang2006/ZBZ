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
void CtrlProgramStart()
{

		if(AGECtrl.bflag.bit.PGST == 0 || AGECtrl.bflag.bit.PGEST == 0)
		{
			return;
		}

		AGECtrl.bflag.bit.PGST = 0;
		SysCtrl.DataPack_Flag=0;

		ClearVarParam();
		
		CtrlSchState(RUNNING_STATE);

		AGEVar.StepState = AGEProg.StepMode;

		switch(AGEVar.StepState)
		{
			case CURR_CTRL :    AGEVar.PTMmtState = MmtRUNSta;  
                          CtrlAGE_CURR_CTRL();			
													break;
			case POWER_CTRL :   AGEVar.PTMmtState = MmtRUNSta;  
                          CtrlAGE_Power_CTRL();			
													break;			
			default  :	        AGEVar.PTMmtState = MmtSTOPSta;
			                    CtrlAGE_STOP_CTRL();
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
void ProgParameterCome()
{
		uint16_t  temp;

	  temp=(MAX_REC_COMSIZE/2);
	
	  AGEProg.Con_EndValTime_RUN.bytes.lbyte=SysCtrl.Ethernet.Data[temp++];
	  AGEProg.Con_EndValTime_RUN.bytes.hbyte=SysCtrl.Ethernet.Data[temp++];
		if(AGEProg.Con_EndValTime_RUN.data<=0)
		{
		   AGEProg.Con_EndValTime_RUN.data=0;
		}
		
		AGEProg.StepMode           = SysCtrl.Ethernet.Data[(temp++)];

		AGEProg.Con_HighVoltage.bytes.lbyte = SysCtrl.Ethernet.Data[(temp++)];
		AGEProg.Con_HighVoltage.bytes.hbyte = SysCtrl.Ethernet.Data[(temp++)];
	
		AGEProg.Con_LowVoltage.bytes.lbyte = SysCtrl.Ethernet.Data[(temp++)];
		AGEProg.Con_LowVoltage.bytes.hbyte = SysCtrl.Ethernet.Data[(temp++)];
		
		AGEProg.Con_CurrentIN_RUN=SysCtrl.Ethernet.Data[(temp++)];
		if(AGEProg.Con_CurrentIN_RUN<=0)
		{
		   AGEProg.Con_CurrentIN_RUN=0;
		}
		
		AGEProg.Con_UPCurrentIN_RUN=SysCtrl.Ethernet.Data[(temp++)];
		if(AGEProg.Con_UPCurrentIN_RUN<=0)
		{
		   AGEProg.Con_UPCurrentIN_RUN=0;
		}
		
		AGEProg.Con_DownCurrentIN_RUN=SysCtrl.Ethernet.Data[(temp++)];
		if(AGEProg.Con_DownCurrentIN_RUN<=0)
		{
		   AGEProg.Con_DownCurrentIN_RUN=0;
		}
		
		AGEProg.Con_UPCurrentOUT_RUN=SysCtrl.Ethernet.Data[(temp++)];
		if(AGEProg.Con_UPCurrentOUT_RUN<=0)
		{
		   AGEProg.Con_UPCurrentOUT_RUN=0;
		}
		
		AGEProg.Con_DownCurrentOUT_RUN=SysCtrl.Ethernet.Data[(temp++)];
		if(AGEProg.Con_DownCurrentOUT_RUN<=0)
		{
		   AGEProg.Con_DownCurrentOUT_RUN=0;
		}
		
		AGEProg.Con_UPVolIN_RUN.bytes.lbyte = SysCtrl.Ethernet.Data[(temp++)];
		AGEProg.Con_UPVolIN_RUN.bytes.hbyte = SysCtrl.Ethernet.Data[(temp++)];
		
		AGEProg.Con_DownVolIN_RUN.bytes.lbyte = SysCtrl.Ethernet.Data[(temp++)];
		AGEProg.Con_DownVolIN_RUN.bytes.hbyte = SysCtrl.Ethernet.Data[(temp++)];
		
		AGEProg.Con_Power_RUN.bytes.lbyte = SysCtrl.Ethernet.Data[(temp++)];
		AGEProg.Con_Power_RUN.bytes.hbyte = SysCtrl.Ethernet.Data[(temp++)];
		
		AGEProg.Con_Powerfluct_RUN.bytes.lbyte = SysCtrl.Ethernet.Data[(temp++)];
		AGEProg.Con_Powerfluct_RUN.bytes.hbyte = SysCtrl.Ethernet.Data[(temp++)];
		
		SysCtrl.ChanSelL.data=0;
		SysCtrl.ChanSelH.data=0;
		SysCtrl.ChanSelL.bytes.lbyte = SysCtrl.Ethernet.Data[temp++];
    SysCtrl.ChanSelL.bytes.hbyte = SysCtrl.Ethernet.Data[temp++];
    SysCtrl.ChanSelH.bytes.lbyte = SysCtrl.Ethernet.Data[temp++];
    SysCtrl.ChanSelH.bytes.hbyte = SysCtrl.Ethernet.Data[temp++];
		
		CtrlSchState(READY_STATE);
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
void PCCtrlAGERun()
{
    uint32_t  j = 0x00000001;
    uint16_t  i;

    if(AGECtrl.bflag.bit.PGRN == 0)
    {
       if(MAX_AGE_CHAN<=32)
       {
         for(i=0;i<MAX_AGE_CHAN;i++)
         {
             if(SysCtrl.ChanSelL.data & j)
             {
                 AGECtrl.Ctrl_AGE_RUN[i]=1;
             }
             j <<= 1;
         }
       }
       else
       {
             j = 0x00000001;
             for(i=0;i<32;i++)
             {
                 if(SysCtrl.ChanSelL.data & j)
                 {
                     AGECtrl.Ctrl_AGE_RUN[i]=1;
                 }
                 j <<= 1;
             }

             j = 0x00000001;

             for(i=32;i<MAX_AGE_CHAN;i++)
             {
                 if(SysCtrl.ChanSelH.data & j)
                 {
                     AGECtrl.Ctrl_AGE_RUN[i]=1;
                 }
                 j <<= 1;
             }
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
void PCCtrlAGESTOP()
{
    uint32_t  j = 0x01;
    uint16_t  i;

    if(MAX_AGE_CHAN<=32)
    {
      for(i=0;i<MAX_AGE_CHAN;i++)
      {
          if(SysCtrl.ChanSelL.data & j)
          {
              AGECtrl.Ctrl_AGE_RUN[i]=0;
          }
          j <<= 1;
      }
    }
    else
    {
      for(i=0;i<32;i++)
      {
          if(SysCtrl.ChanSelL.data & j)
          {
              AGECtrl.Ctrl_AGE_RUN[i]=0;
          }
          j <<= 1;
      }
      j = 0x01;
      for(i=32;i<MAX_AGE_CHAN;i++)
      {
          if(SysCtrl.ChanSelH.data & j)
          {
              AGECtrl.Ctrl_AGE_RUN[i]=0;
          }
          j <<= 1;
      }
    }

    CtrlSchState(STOP_STATE);
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
void PCCtrlAGEPause()
{
    uint32_t  j = 0x01;
    uint16_t  i;

    if(AGECtrl.bflag.bit.PGRN == 1)
    {
        if(MAX_AGE_CHAN<=32)
        {
          for(i=0;i<MAX_AGE_CHAN;i++)
          {
              if(SysCtrl.ChanSelL.data & j)
              {
                  AGECtrl.Ctrl_AGE_RUN[i]=0;
              }
              j <<= 1;
          }
        }
        else
        {
          for(i=0;i<32;i++)
          {
              if(SysCtrl.ChanSelL.data & j)
              {
                  AGECtrl.Ctrl_AGE_RUN[i]=0;
              }
              j <<= 1;
          }

          j = 0x01;

          for(i=32;i<MAX_AGE_CHAN;i++)
          {
              if(SysCtrl.ChanSelH.data & j)
              {
                  AGECtrl.Ctrl_AGE_RUN[i]=0;
              }
              j <<= 1;
          }
        }
        CtrlSchState(PAUSE_STATE);
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
void PCCtrlAGERecover()
{
    uint32_t  j = 0x01;
    uint16_t  i;

    if((AGEVar.SchState == PAUSE_STATE)&&( AGECtrl.bflag.bit.PGRN == 0))
    {
       if(MAX_AGE_CHAN<=32)
       {
         for(i=0;i<MAX_AGE_CHAN;i++)
         {
             if(SysCtrl.ChanSelL.data & j)
             {
							  if((AGEVar.WARNINCFormData[i]==0)&&(AGEVar.WARNOUTCFormData[i]==0)&&(AGEVar.WARNVFormData[i]==0))
								{
								   AGECtrl.Ctrl_AGE_RUN[i]=1;
								}
								else
								{ 
								   AGECtrl.Ctrl_AGE_RUN[i]=0;
								}
                 
             }
             j <<= 1;
         }
       }
       else
       {
         for(i=0;i<32;i++)
         {
             if(SysCtrl.ChanSelL.data & j)
             {
                if((AGEVar.WARNINCFormData[i]==0)&&(AGEVar.WARNOUTCFormData[i]==0)&&(AGEVar.WARNVFormData[i]==0))
								{
								   AGECtrl.Ctrl_AGE_RUN[i]=1;
								}
								else
								{ 
								   AGECtrl.Ctrl_AGE_RUN[i]=0;
								}
             }
             j <<= 1;
         }

         j = 0x01;

         for(i=32;i<MAX_AGE_CHAN;i++)
         {
             if(SysCtrl.ChanSelH.data & j)
             {
                 if((AGEVar.WARNINCFormData[i]==0)&&(AGEVar.WARNOUTCFormData[i]==0)&&(AGEVar.WARNVFormData[i]==0))
								{
								   AGECtrl.Ctrl_AGE_RUN[i]=1;
								}
								else
								{ 
								   AGECtrl.Ctrl_AGE_RUN[i]=0;
								}
             }
             j <<= 1;
         }
       }
			 CtrlSchState(RUNNING_STATE);
       switch(AGEVar.StepState)
       {
           case CURR_CTRL :    AGEVar.PTMmtState = MmtRUNSta;
                               CtrlAGE_CURR_CTRL();

                               break;
					 case POWER_CTRL :   AGEVar.PTMmtState = MmtRUNSta;
                               CtrlAGE_Power_CTRL();

                               break;
           default  :  AGEVar.PTMmtState = MmtSTOPSta;;
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
void CtrlSchState(uint16_t state)
{
    uint16_t i;

    AGEVar.SchState = state;

    switch(state)
    {
        case WAIT_STATE:    ClearVarParam();
                            ClearProgParam(); 
                            LEDR_OFF();
                            LEDG_ON();			
                            AGEProg.ProgType=NOMAL_MODE;
			                      for(i=0;i<MAX_MAIN_DA;i++)
			                      {
														    SetMB88346BMainDA(0,i);	
														}			                                             		
                            for(i=0;i<MAX_AGE_CHAN;i++)
                            { 
															 SetMB88346BSingle(0,i);
															 DELAY_Ms(10);
                               Ctrl_ON_Switch(i,0);
                               AGECtrl.Ctrl_AGE_RUN[i]=0;
                            }                                                                              														
                            AGEVar.ProtectTimeCnt.data=0;
                            AGEVar.StepState=WAIT_CTRL;                           
                            SysCtrl.DataPack_Flag=0;                                                                                                                                                
                            SysCtrl.sflag.bit.IPModifiedEn=0;
                            break;													
														
        case READY_STATE:   ClearVarParam();
														LEDR_OFF();
                            LEDG_ON();
                            AGEProg.ProgType=NOMAL_MODE; 
                            for(i=0;i<MAX_MAIN_DA;i++)
			                      {
														    SetMB88346BMainDA(0,i);	
														}
                            														
                            for(i=0;i<MAX_AGE_CHAN;i++)
                            {
															   SetMB88346BSingle(0,i);
															   DELAY_Ms(10);
                                 Ctrl_ON_Switch(i,0);
                            }                           
                            AGECtrl.bflag.bit.PGEST = 1;
                            break;
														
        case RUNNING_STATE: AGECtrl.bflag.bit.PGRN = 1;
                            AGECtrl.bflag.bit.PTEN =0;
                            AGECtrl.bflag.bit.SFEN = 0;                                                      
                            break;
														
        case PAUSE_STATE:   AGECtrl.bflag.bit.PGRN = 0;
                            AGECtrl.bflag.bit.PTEN =0; 
														AGEVar.ProtectTimeCnt.data=0;
														LEDR_OFF();
                            LEDG_ON();
                            for(i=0;i<MAX_MAIN_DA;i++)
			                      {
														    SetMB88346BMainDA(0,i);	
														}														
                            for(i=0;i<MAX_AGE_CHAN;i++)
                            {
																SetMB88346BSingle(0,i);
																DELAY_Ms(10);
																Ctrl_ON_Switch(i,0);
																AGECtrl.Ctrl_AGE_RUN[i]=0;
                            }                           
                            break;
														
        case COMPLATE_STATE:ClearProgParam(); 
                            LEDR_OFF();
                            LEDG_ON();														
                            AGECtrl.bflag.bit.PGRN=0;
                            AGECtrl.bflag.bit.PTEN=0;
                            AGECtrl.bflag.bit.SFEN=0;                       
                            AGEVar.PTMmtState = MmtSTOPSta; 
														AGEVar.ProtectTimeCnt.data=0;
                            for(i=0;i<MAX_MAIN_DA;i++)
			                      {
														    SetMB88346BMainDA(0,i);	
														}		
                            DELAY_Ms(10);														
                            for(i=0;i<MAX_AGE_CHAN;i++)
                            {
																SetMB88346BSingle(0,i);
															  DELAY_Ms(10);
																Ctrl_ON_Switch(i,0);
															  AGEVar.WARNVFormData[i] = 0x00;
																AGEVar.WARNINCFormData[i] = 0x00;
																AGEVar.WARNOUTCFormData[i] = 0x00;
																AGECtrl.Ctrl_AGE_RUN[i]=0;
                            }                           
                            break;
														
        case WARNSTOP_STATE:AGECtrl.bflag.bit.PGRN=0;
                            AGEVar.PTMmtState = MmtSTOPSta;
                            AGECtrl.bflag.bit.SFEN =0;
														AGEVar.ProtectTimeCnt.data=0;
														LEDR_ON();
                            LEDG_OFF();
                            for(i=0;i<MAX_MAIN_DA;i++)
			                      {
														    SetMB88346BMainDA(0,i);	
														}             
                            DELAY_Ms(10);														
                            for(i=0;i<MAX_AGE_CHAN;i++)
                            {
															 SetMB88346BSingle(0,i);
															 DELAY_Ms(10);
                               Ctrl_ON_Switch(i,0);
                               AGECtrl.Ctrl_AGE_RUN[i]=0;
                            }                           
                            break;
														
        case STOP_STATE:    ClearVarParam();
                            ClearProgParam();                           
                            AGECtrl.bflag.bit.PGRN=0;
                            AGECtrl.bflag.bit.SFEN=0;
                            for(i=0;i<MAX_MAIN_DA;i++)
			                      {
														    SetMB88346BMainDA(0,i);	
														}          
                            DELAY_Ms(10);														
                            for(i=0;i<MAX_AGE_CHAN;i++)
                            {
															 SetMB88346BSingle(0,i);
                               Ctrl_ON_Switch(i,0);
                               AGECtrl.Ctrl_AGE_RUN[i]=0;
                            }                                                      
                            break;
														
        default:            break;
    }
}





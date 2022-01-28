#include "main.h"
#include "MyInclude.h"
#include"math.h"
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
// 休眠
void CtrlAGE_STOP_CTRL()
{
    uint16_t i;
    
	  QDB_ENA_H;
		QDB_ENB_H;
	  DELAY_Ms(10);
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
//恒流
void CtrlAGE_CURR_CTRL()
{
    float curr,buff;
    uint16_t daS[MAX_AGE_CHAN]={0},i,daM;

    if(AGEProg.ProgType == NOMAL_MODE)
    {
       if(AGEProg.Con_CurrentIN_RUN > MAX_DEVICE_CURRENT)
       {
           curr = MAX_DEVICE_CURRENT;
       }
			 else
			 {
			     curr = AGEProg.Con_CurrentIN_RUN;
			 }
      			 
			 buff = ((curr+155) +CONSTHARF_DAVAL)*CONST_REFDAVAL;			 
			 daM = (uint16_t)buff;

			 for(i=0;i<MAX_AGE_CHAN;i++)
			 {
					if(AGECtrl.Ctrl_AGE_RUN[i]==1)
					{
							daS[i] = 128 + ErrorCompensation_i(curr,i);
							if(daS[i]>255)
							{
									daS[i]=255;
							}           
					}
					else
					{
							daS[i] =0;
					}
			}
			 
      if(AGECtrl.bflag.bit.PGRN==1)
      {
				 for(i=0;i<MAX_MAIN_DA;i++)
				 {
						 SetMB88346BMainDA(daM,i);	
				 }		
				 DELAY_Ms(10);
         for(i=0;i<MAX_AGE_CHAN;i++)
         {
             if(AGECtrl.Ctrl_AGE_RUN[i]==1)
             {
                 SetMB88346BSingle(daS[i],i);
                 DELAY_Ms(10);
                 Ctrl_ON_Switch(i,1);
             }
             else
             {
                 SetMB88346BSingle(0,i);
                 DELAY_Ms(10);
                 Ctrl_ON_Switch(i,0);
             }
         }
				 QDB_ENA_H;
		     QDB_ENB_L;
      }
      else
      {
				  QDB_ENA_H;
		      QDB_ENB_H;
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
          }
      }
      AGECtrl.bflag.bit.PTEN=1;
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
//恒功率
void CtrlAGE_Power_CTRL()
{
    float curr,buff,Vol_Vf,Vol_AC,Vol_Salve,Sqrt_Val;
    uint16_t daS[MAX_AGE_CHAN]={0},i,daM;

		
    if(AGEProg.ProgType == NOMAL_MODE)
    {  		 
			 /*(-B-SQRT(B*B-4*A*C))/2*A*C*/
			 Vol_Vf=(float)(SysCtrl.System_VCPower.data/10000);			 
			 Vol_AC=((float)(AGEProg.Con_Power_RUN.data/10));//4ac
			 Vol_AC=((4*25*Vol_AC)/1000);
			 Vol_Salve=(Vol_Vf*Vol_Vf-Vol_AC);
			 Sqrt_Val=sqrtf(Vol_Salve);
			 curr= (Vol_Vf - Sqrt_Val)/(2*25);
			 curr=curr*100000;//mA*100
			
       if(curr > MAX_DEVICE_CURRENT)
       {
           curr = MAX_DEVICE_CURRENT;
       }
			 
			 buff = ((curr+155) +CONSTHARF_DAVAL)*CONST_REFDAVAL;	
			 
			 daM = (uint16_t)buff;

			 for(i=0;i<MAX_AGE_CHAN;i++)
			 {
					if(AGECtrl.Ctrl_AGE_RUN[i]==1)
					{
							daS[i] = 128 + ErrorCompensation_i(curr,i);
							if(daS[i]>255)
							{
									daS[i]=255;
							}           
					}
					else
					{
							daS[i] =0;
					}
			 }

      if(AGECtrl.bflag.bit.PGRN==1)
      {
				  for(i=0;i<MAX_MAIN_DA;i++)
				  {
						SetMB88346BMainDA(daM,i);	
				  }		
          for(i=0;i<MAX_AGE_CHAN;i++)
          {
             if(AGECtrl.Ctrl_AGE_RUN[i]==1)
             {
                 SetMB88346BSingle(daS[i],i);
                 DELAY_Ms(10);
                 Ctrl_ON_Switch(i,1);
             }
             else
             {
                 SetMB88346BSingle(0,i);
                 DELAY_Ms(10);
                 Ctrl_ON_Switch(i,0);
             }
          }
					QDB_ENA_L;
		      QDB_ENB_H;
      }
      else
      {
				  QDB_ENA_H;
		      QDB_ENB_H;
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
          }
      }
      AGECtrl.bflag.bit.PTEN=1;		
    }
}  





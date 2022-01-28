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
void LimitCompareOK()
{
	uint16_t  ovflag=0;

	if(AGEVar.SchState == PAUSE_STATE)
	{
		return;
	}

	if(AGECtrl.bflag.bit.PGRN==1)
	{
	   ovflag = (LimitCompare(AGEVar.StepState));

		 if(ovflag == 0x0001)
		 {
				LimitCompareProcess();
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
uint16_t LimitCompare(uint16_t mode)
{
    uint16_t  ovflag = 0;
    uint32_t vintime,Limtime;

    vintime=AGEVar.StepTimeCnt.data;

    switch(mode)
    {    
        case CURR_CTRL :    Limtime=AGEProg.Con_EndValTime_RUN.data;
                            if(vintime>=Limtime)
                            {
                               ovflag = 1;
                            }
                            break;
			  case POWER_CTRL :   Limtime=AGEProg.Con_EndValTime_RUN.data;
                            if(vintime>=Limtime)
                            {
                               ovflag = 1;
                            }
                            break;

        default     :   ovflag = 0;break;
    }

    return ovflag;
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
void LimitCompareProcess()
{

		CtrlSchState(COMPLATE_STATE); // program  complete

		AGECtrl.bflag.bit.PGEST = 0;
}



#include "main.h"
#include "MyInclude.h"


const uint16_t SOFT_DATACALI[64]={
         0,4,8,12,16,20,24,28,
         32,36,40,44,48,52,56,60,
         64,68,72,76,80,84,88,92,
         96,100,104,108,112,116,120,124,

         1,5,9,13,17,21,25,29,
         33,37,41,45,49,53,57,61,
         65,69,73,77,81,85,89,93,
         97,101,105,109,113,117,121,125,
};

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
uint16_t CurrentCalibGrade(float data)
{
	if(data < CurrGrade1)
	{
		return 1;
	}

	if(data >= CurrGrade1 && data <= CurrGrade2)
	{
		return 2;
	}
	
	return 0;
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
float ErrorCompensation_i(float cur,uint16_t ChanNo)
{
	float  tempdata,errordata,Cur_data,kData;
	uint32_t  Kdata,Bdata;
	uint16_t  grade;
	float  ReturnDa;

  grade = CurrentCalibGrade(cur);

	if(grade == 0 )
	{
		 Kdata = 100000;
		 Bdata = 0;
	}
	else
	{
     Kdata = Calib[ChanNo].ConCChar[grade-1].KVal.data;
     Bdata = Calib[ChanNo].ConCChar[grade-1].BVal.data;
	}

	// check that the KB_data is legal
	if(Kdata > 120000 || Kdata < 80000)
	{
		Kdata = 100000;
		Bdata = 0;
	}
	else
	{

	}

	Cur_data=cur;

	if(Bdata & 0x80000000)
	{
		tempdata = Cur_data + (Bdata & 0x7fffffff) / 10000.0;
	}
	else
	{
		tempdata = Cur_data - Bdata / 10000.0;
	}
	
	kData=(Kdata / 100000.0);
	
	errordata = (float)(tempdata / kData);
	
	ReturnDa=(float)((Cur_data-errordata)*CONST_SLAVEDAVAL);
	
	return (ReturnDa);
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
uint16_t CurrCalibGrade_Fee(float data)
{
	if(data < CurrGrade1)
	{
		return 1;
	}

	if(data >= CurrGrade1 && data <= CurrGrade2)
	{
		return 2;
	}

	return 0;
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
float FeedBackCalibration_i(uint32_t data,uint16_t ChanNo)
{
	uint16_t  grade;
	float  buff,cur;
	uint32_t  Kdata,Bdata;

	grade = CurrCalibGrade_Fee(data);

	if(grade == 0)
	{
		return data;
	}

  Kdata = Calib[ChanNo].FeeCChar[grade-1].KVal.data;
  Bdata = Calib[ChanNo].FeeCChar[grade-1].BVal.data;

	// check that the KB_data is legal
	if(Kdata > 120000 || Kdata < 80000)
	{
		Kdata = 100000;
		Bdata = 0;
	}
	else
	{

	}

	cur = data;

	buff = cur * Kdata / 100000.0;

	if(Bdata & 0x80000000)
	{
		buff -= (Bdata & 0x7fffffff) / 10000.0;
	}
	else
	{
		buff += Bdata / 10000.0;
	}

	return buff;
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
uint16_t OutCurrCalibGrade_Fee(float data)
{
	if(data < CurrGrade1)
	{
		return 1;
	}

	if(data >= CurrGrade1 && data <= CurrGrade2)
	{
		return 2;
	}

	return 0;
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
float FeedBackOutCurCalibration_i(uint32_t data,uint16_t ChanNo)
{
	uint16_t  grade;
	float  buff,cur;
	uint32_t  Kdata,Bdata;

	grade = OutCurrCalibGrade_Fee(data);

	if(grade == 0)
	{
		return data;
	}

  Kdata = Calib[ChanNo].FeeOutCChar[grade-1].KVal.data;
  Bdata = Calib[ChanNo].FeeOutCChar[grade-1].BVal.data;

	// check that the KB_data is legal
	if(Kdata > 120000 || Kdata < 80000)
	{
		Kdata = 100000;
		Bdata = 0;
	}
	else
	{

	}

	cur = data;

	buff = cur * Kdata / 100000.0;

	if(Bdata & 0x80000000)
	{
		buff -= (Bdata & 0x7fffffff) / 10000.0;
	}
	else
	{
		buff += Bdata / 10000.0;
	}

	return buff;
} 







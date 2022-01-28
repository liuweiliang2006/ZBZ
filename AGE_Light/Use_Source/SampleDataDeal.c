#include "main.h"
#include "MyInclude.h"

/******************************************************************
�������ƣ�
��������:
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
uint32_t abs32(uint32_t cmpa,uint32_t cmpb)
{
    uint32_t cmp;

    if((cmpa & 0x7fffffff)>(cmpb & 0x7fffffff))
    {
        cmp = (cmpa & 0x7fffffff) -  (cmpb & 0x7fffffff);
    }
    else
    {
        cmp = (cmpb & 0x7fffffff) -  (cmpa & 0x7fffffff);
    }
    return cmp;
}
/******************************************************************
�������ƣ�
��������:
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
uint32_t SortAndFilter(uint32_t *dpoint, uint16_t num)
{
    volatile uint16_t ptemp,jtemp;
    volatile uint32_t addtemp,tempcount;
	  volatile  float returntemp; 

    //����
    for(ptemp = 0;ptemp<num;ptemp++)
    {
        for(jtemp = ptemp+1;jtemp<num;jtemp++)
        {
            if(dpoint[ptemp]>dpoint[jtemp])
            {
                tempcount = dpoint[ptemp];
                dpoint[ptemp] = dpoint[jtemp];
                dpoint[jtemp] = tempcount;
            }
        }
    }
    //�ۼ�
    addtemp = 0;
    if(num<=10)  //С��10�ľ�ȥ��������Сֵ
    {
        for(ptemp=1;ptemp<num-1;ptemp++)
        {
            addtemp += dpoint[ptemp];//���
        }			
        addtemp = (uint32_t)(addtemp>>3);
    }
    else if(num<=30)//�����30������ȥ��ǰ5�ͺ�5
    {
        for(ptemp=5;ptemp<num-5;ptemp++)
        {
            addtemp += dpoint[ptemp];//���
        }
        addtemp = addtemp/(num-10);
    }
    return(addtemp);
}
/******************************************************************
�������ƣ�
��������:
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
uint16_t SortAndFilter16(uint16_t *dpoint, uint16_t num)
{
    uint16_t ptemp,jtemp;
    uint32_t addtemp,tempcount;

    //����
    for(ptemp = 0;ptemp<num;ptemp++)
    {
        for(jtemp = ptemp+1;jtemp<num;jtemp++)
        {
            if(dpoint[ptemp]>dpoint[jtemp])
            {
                tempcount = dpoint[ptemp];
                dpoint[ptemp] = dpoint[jtemp];
                dpoint[jtemp] = tempcount;
            }
        }
    }
    //�ۼ�
    addtemp = 0;
    if(num<=10)  //С��10�ľ�ȥ��������Сֵ
    {
        for(ptemp=1;ptemp<num-1;ptemp++)
        {
            addtemp += dpoint[ptemp];//���
        }
        addtemp = addtemp/(num-2);
    }

    return(addtemp);
}
/******************************************************************
�������ƣ�
��������:
���������
���������
����ֵ��
ע�⣺
******************************************************************/
/******************************************************************
����������ʷ��¼��
******************************************************************/
uint16_t CurrFilter(uint16_t Curr,uint16_t ChanNo)
{
    uint16_t  Iflct,ConCur;
    uint16_t CurDeal;

    if(AGECtrl.Ctrl_AGE_RUN[ChanNo] == 1)
    {
			  switch(AGEVar.StepState)
				{
					case CURR_CTRL :    ConCur = AGEProg.Con_CurrentIN_RUN;
                              CurDeal= (5000);		
															break;
					case POWER_CTRL :   ConCur = AGEProg.Con_Power_RUN.data;
                              CurDeal= (5000); 		
															break;			
					default  :	        
															break;
				}
       
    
        Iflct = abs32(Curr,ConCur);

        if((Iflct<CurDeal)&&(Iflct>10))
        {
            if(Curr>ConCur)
            {
                Curr = ConCur+Iflct%5;
            }
            else
            {
                Curr = ConCur-Iflct%5;
            }
        }
    }
    return  Curr;
}









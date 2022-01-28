#include "main.h"
#include "tcpecho.h"
#include "lwip/opt.h"

#include <lwip/sockets.h>
#include "lwip/sys.h"
#include "lwip/api.h"
#include "MyInclude.h"
// 下位机软件版本号
const uint8_t  SoftVersionString[] = "AGE_64CH;1.1_20220125_HXL_ATiS";


static void Send_AGEData(void* parameter);
static void RecvProgramData(void* parameter);
static void CalibSysTime(void* parameter);
static void ControlAGEMode(void* parameter);
//static void ControlAGEChan(void* parameter);

static void ControlCaliMeasure(void* parameter);
static void ControlCaliCurrent(void* parameter);
static void ControlSampleCurrent(void* parameter);
static void RecvCONKBData(void* parameter);
static void SendCONKBData(void* parameter);
static void RecvFEEKBData(void* parameter);
static void SendFEEKBData(void* parameter);
static void SampleOutCurrent(void* parameter);
static void RecvOUTFEEKBData(void* parameter);
static void SendOUTFEEKBData(void* parameter);
static void GetSoftWareVersion(void* parameter);
static void ManualModifyIP(void* parameter);
static void OnlineFlashProgram(void* parameter);


const CMD_Type CMD_Assemble[] =	{{"GETDATA"	          ,Send_AGEData},
																 {"SENDPARA"          ,RecvProgramData},
																 {"SYSTIME"	          ,CalibSysTime},
																 {"SENDSTATE"		      ,ControlAGEMode},														
																 {"CALSETMODE"			  ,ControlCaliMeasure},
																 {"CALSETINCURR"	    ,ControlCaliCurrent},
																 {"CALGETINCURR"      ,ControlSampleCurrent},
																 
																 {"CALSETCKINCURRKB"  ,RecvCONKBData},
																 {"CALGETCKINCURRKB"  ,SendCONKBData},
																 {"CALSETHJINCURRKB"  ,RecvFEEKBData},
																 {"CALGETHJINCURRKB"  ,SendFEEKBData},
																 {"CALGETOUTCURR"		  ,SampleOutCurrent},
																 {"CALSETHJOUTCURRKB"	,RecvOUTFEEKBData},
																 {"CALGETHJOUTCURRKB"	,SendOUTFEEKBData},
																 {"GETEDITION"		    ,GetSoftWareVersion},
																 {"CHANGEIP"		      ,ManualModifyIP},
																 {"ONLINEUPDATE1"		  ,OnlineFlashProgram}}; 

static bool String_Verify(const char* String,char* a);
static uint16_t String_Size(const char* String);
																 
uint16_t  Data[(MAX_AGE_CHAN*MAX_CURRKB_GRADE*4)]={0};
/******************************************************************
函数名称：EthernetInterfaceHandler
函数功能:以太网数据传输主程序
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void EthernetInterfaceHandler(uint16_t len)
{
    uint16_t stemp,send[4] = {0,0,0,0};
    uint32_t dSum,rSum;
		
    if(SysCtrl.Ethernet.ETH_Flag == Online)
    {
        ProgramUpdata();
        SysCtrl.Ethernet.ETH_PackageLength.data = SysCtrl.Ethernet.ETH_Length.data;
        if(SysCtrl.Ethernet.Flash_Legth.data == SysCtrl.Ethernet.ETH_Length.data)
        {
            EthernetReceiveSuccess(0,&send[0]);
            SysCtrl.Ethernet.ETH_Length.data = 0;
            SysCtrl.Ethernet.ETH_Flag = FALSE;
					
            //FlashUpdate();
            
        }
        else if(0 == SysCtrl.Ethernet.ETH_PackageLength.data%65536)
        {
            EthernetReceiveSuccess(0,&send[0]);
            SysCtrl.Ethernet.ETH_PackageLength.data = 0;
        }
        else if(SysCtrl.Ethernet.ETH_Length.data < SysCtrl.Ethernet.Flash_Legth.data)
        {
//            SysCtrl.Ethernet.ETH_Flag = FALSE;
        }
        else
        {
            SysCtrl.Ethernet.ETH_Length.data=0;
            SysCtrl.Ethernet.ETH_Flag = FALSE;
            EthernetReceiveFaiAGE(0);
        }
    }
    else
    {
        SysCtrl.Ethernet.ETH_Length.data = len;
			  SysCtrl.Ethernet.ETH_ReceiveLength.byte.LBYTE = SysCtrl.Rec_Comand[(MAX_REC_COMSIZE-4)];
				SysCtrl.Ethernet.ETH_ReceiveLength.byte.ZBYTE = SysCtrl.Rec_Comand[(MAX_REC_COMSIZE-3)];
				SysCtrl.Ethernet.ETH_ReceiveLength.byte.MBYTE = SysCtrl.Rec_Comand[(MAX_REC_COMSIZE-2)];
				SysCtrl.Ethernet.ETH_ReceiveLength.byte.HBYTE = SysCtrl.Rec_Comand[(MAX_REC_COMSIZE-1)];
        SysCtrl.Ethernet.ETH_ReceiveLength.all/=2;

        if(SysCtrl.Ethernet.ETH_ReceiveLength.all>1536)
        {
            EthernetReceiveFaiAGE(0);
            return;
        }

        if(SysCtrl.Ethernet.Data[SysCtrl.Ethernet.ETH_ReceiveLength.all-1] == 0x5E5D && SysCtrl.Ethernet.Data[SysCtrl.Ethernet.ETH_ReceiveLength.all-2] == 0x5C5A)
        {
            dSum = Crs(SysCtrl.Ethernet.Data,SysCtrl.Ethernet.ETH_ReceiveLength.all-4);
            rSum = SysCtrl.Ethernet.Data[SysCtrl.Ethernet.ETH_ReceiveLength.all-4]+SysCtrl.Ethernet.Data[SysCtrl.Ethernet.ETH_ReceiveLength.all-3]*65536;
        }

        if (dSum == rSum)
        {
					  PC_DATAHandler(SysCtrl.Rec_Comand, MAX_REC_COMSIZE);       
        }
        else
        {
					  EthernetReceiveFaiAGE(0);          
        }
        for(stemp=0 ;stemp<1536; stemp++)
        {
            SysCtrl.Ethernet.Data[stemp]=0;
        }
    }

}
/******************************************************************
函数名称：EthernetReceiveSuccess
函数功能:数据接收成功标志，传输给上位机
输入参数：Command：收到的命令， ParameterLen：上传的参数的值，Send：上传的参数的首地址
输出参数：
返回值：  
注意： 上传参数长度为零时不需要上参数首地址     数据长度过大时默认恢复校验和为0 ParameterLen为0时表示无上传数据，此时Send可以为任意值
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void EthernetReceiveSuccess(uint32_t ParameterLen,uint16_t* Send1)
{
     uint16_t temp[20],num=0,i,length;
	   uint8_t  ChanData[512]={0};
	   uint8_t  ChanHeadData[50]={0};
	   uint8_t  ChanLastData[50]={0};
     uint32_t SUM;
		 uint8_t IP_TEMP=0;
	  
		 
		IP_TEMP=READSTATE_ADDR;
    temp[num++]= 0xBBAA;
    temp[num++]= 0xDDCC;
		for(i=0;i<10;i++)
		{
		   temp[num++]=(SysCtrl.Rec_Comand[(2*i+1+4)]<<8)|(SysCtrl.Rec_Comand[(2*i+4)]);
		}  
    temp[num++]= 0;
    temp[num++]= 0xC0A8;
    temp[num++]= (0x7B<<8)+IP_TEMP;
    temp[num++]= 0x3388;
    SysCtrl.TEMP.data=ParameterLen*2+(num*2)+12;//参数长度+这行之前的长度+这行之后除去命令的长度
    temp[num++]= SysCtrl.TEMP.bytes.lbyte;
    temp[num++]= SysCtrl.TEMP.bytes.hbyte;
    SUM=Crs(temp, num);
    length=num*2;	
    for(i=0;i<num;i++)
    {
        ChanHeadData[(2*i+1)]=(temp[i]>>8)&0xff;
        ChanHeadData[(2*i)]=(temp[i])&0xff;
    }
		write(SysCtrl.socket_descr,(&ChanHeadData[0]),length);
    if(0 !=ParameterLen )
    {
        if(ParameterLen<1536)
        {
            SUM+=Crs(Send1,ParameterLen);
        }
        else
        {
            SUM=0;
        }
				if(ParameterLen<=256)
				{
				  length=ParameterLen*2;
					for(i=0;i<ParameterLen;i++)
				  {
						 ChanData[(2*i+1)]=((*Send1)>>8)&0xff;
						 ChanData[(2*i)]=((*Send1++))&0xff;
				  }
					
					write(SysCtrl.socket_descr,(&ChanData[0]),length);
				}
				else if(ParameterLen<=512)
				{			   
					 for(i=0;i<256;i++)
				   {
						 ChanData[(2*i+1)]=((*Send1)>>8)&0xff;
						 ChanData[(2*i)]=((*Send1++))&0xff;
				   }				 
					 write(SysCtrl.socket_descr,(&ChanData[0]),512);
					 length=(ParameterLen-256)*2;
					 for(i=0;i<(ParameterLen-256);i++)
				   {
						 ChanData[(2*i+1)]=((*Send1)>>8)&0xff;
						 ChanData[(2*i)]=((*Send1++))&0xff;
				   }				 
					 write(SysCtrl.socket_descr,(&ChanData[0]),length);
				}				 				       
    }
    num=0;
    temp[num++]= SUM;
    temp[num++]= SUM>>16;
    temp[num++]= 0x5C5A;
    temp[num++]= 0x5E5D;
    length=num*2;
    for(i=0;i<num;i++)
    {
        ChanLastData[(2*i+1)]=(temp[i]>>8)&0xff;
        ChanLastData[(2*i)]=(temp[i])&0xff;
    }
    write(SysCtrl.socket_descr,(&ChanLastData[0]),length);
}
/******************************************************************
函数名称：EthernetReceiveFaiAGE
函数功能:数据接受失败标志，传输给上位机
输入参数：错误命令
输出参数：
返回值：  
注意： 
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void EthernetReceiveFaiAGE(uint16_t Error)
{
    uint16_t temp[50],num=0,i,length;
	  uint8_t  ChanData[50];
	  uint8_t  IP_TEMP;
    uint32_t SUM;
	  
		IP_TEMP=READSTATE_ADDR;
    temp[num++]= 0xBBAA;
    temp[num++]= 0xDDCC;
    for(i=0;i<10;i++)
		{
		   temp[num++]=(SysCtrl.Rec_Comand[(2*i+1+4)]<<8)|(SysCtrl.Rec_Comand[(2*i+4)]);//去4个开头后是命令
		}  
    temp[num++]= 0;
    temp[num++]= 0xC0A8;
    temp[num++]= (0x7B<<8)+IP_TEMP;
    temp[num++]= 0x1144;
    temp[num++]= 68;
    temp[num++]= 0;
    temp[num++]= Error;//后面改成错误码
    SUM=Crs(temp,num);
    temp[num++]=SUM;
    temp[num++]=SUM>>16;
    temp[num++]= 0x5C5A;
    temp[num++]= 0x5E5D;
    length=num*2;
    for(i=0;i<num;i++)
    {
        ChanData[(2*i+1)]=(temp[i]>>8)&0xff;
        ChanData[(2*i)]=(temp[i])&0xff;
    }
    write(SysCtrl.socket_descr,(&ChanData[0]),length);
   
}

/******************************************************************
函数名称：Crs
函数功能:校验和计算
输入参数：
输出参数：
返回值： 计算好的校验和
注意： 
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
uint32_t Crs(uint16_t*  buf,uint16_t len)
{   // 校验和 = 命令 + 数据，len按字来算，而不是按字节算
    uint32_t  i,sum=0;
    uint16_t  *ptr;

    if(len>1536)
    {
        return 0;
    }
    ptr = buf;
    for(i=0 ; i<len ; i++)
    {
        sum += ((*(ptr+i)) & 0x00ff);
        sum += ((*(ptr+i)) >> 8);
    }

    return sum;
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
static void Send_AGEData(void* parameter)
{
    if(0==SysCtrl.DataPack_Flag)
    {
        EthernetReceiveSuccess(SINGLE_PACKAGE_WORDS,&SysCtrl.DataSaveB[0]);
        SysCtrl.DataPack_Flag=1;
    }
    else if(1==SysCtrl.DataPack_Flag)
    {
        EthernetReceiveSuccess(SINGLE_PACKAGE_WORDS,&SysCtrl.DataSaveA[0]);
        SysCtrl.DataPack_Flag=0;
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
static void RecvProgramData(void* parameter)
{
    ProgParameterCome();
    EthernetReceiveSuccess(0,0);
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
static void CalibSysTime(void* parameter)
{
    uint16_t i,Adjust_Time[8],jtemp;
    uint16_t Rec_Data[4],Setyear;

    jtemp=(MAX_REC_COMSIZE/2);
    for(i=0;i<4;i++)
    {
        Rec_Data[i]=SysCtrl.Ethernet.Data[jtemp++];
    }

    Adjust_Time[0]=Rec_Data[0]&0xff;
    Adjust_Time[1]=(Rec_Data[0]>>8)&0xff;
    Adjust_Time[2]=Rec_Data[1]&0xff;
    Adjust_Time[3]=(Rec_Data[1]>>8)&0xff;
    Adjust_Time[4]=Rec_Data[2]&0xff;
    Adjust_Time[5]=(Rec_Data[2]>>8)&0xff;
    Adjust_Time[6]=(Rec_Data[3])&0xff;
    Adjust_Time[7]=(Rec_Data[3]>>8)&0xff;
		Setyear=(Adjust_Time[6]*100+Adjust_Time[5]);
    RTC_Set(Setyear, Adjust_Time[4], Adjust_Time[3], Adjust_Time[2],  Adjust_Time[1], Adjust_Time[0]);
    EthernetReceiveSuccess(0,0);
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
static void ControlAGEMode(void* parameter)
{
    uint16_t  Ctrl_State=0,jtemp;

    jtemp=(MAX_REC_COMSIZE/2);
	  
    Ctrl_State = SysCtrl.Ethernet.Data[jtemp++];
    Ctrl_State= (Ctrl_State & 0x00ff);

    switch(Ctrl_State)
    {   // start
        case 0x01 :  if(AGECtrl.bflag.bit.PGEST == 1)
                     {
                        AGECtrl.bflag.bit.PGST=1;
                        PCCtrlAGERun();
                     }
                     break;
        // pause
        case 0x02 :  PCCtrlAGEPause();
                     break;
        // recover
        case 0x03 :  PCCtrlAGERecover();
                     break;
        // stop
        case 0x04 :  PCCtrlAGESTOP();
                     break;

        default     :   break;
    }

    EthernetReceiveSuccess(0,0);
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
/*static void ControlAGEChan(void* parameter)
{
    uint16_t  jtemp;
    uint32_t  j = 0x00000001;

	  jtemp=(MAX_REC_COMSIZE/2);
	
    SysCtrl.ChanSelL.data=0;
    SysCtrl.ChanSelH.data=0;
    SysCtrl.ChanSelL.bytes.lbyte = SysCtrl.Ethernet.Data[jtemp++];
    SysCtrl.ChanSelL.bytes.hbyte = SysCtrl.Ethernet.Data[jtemp++];
    SysCtrl.ChanSelH.bytes.lbyte = SysCtrl.Ethernet.Data[jtemp++];
    SysCtrl.ChanSelH.bytes.hbyte = SysCtrl.Ethernet.Data[jtemp++];

    EthernetReceiveSuccess(0,0);
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
static void ControlCaliMeasure(void* parameter)
{
    uint16_t  jtemp;

    jtemp=(MAX_REC_COMSIZE/2);

    AGEProg.ProgType = SysCtrl.Ethernet.Data[jtemp++];

    EthernetReceiveSuccess(0,0);
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
static void ControlCaliCurrent(void* parameter)
{
    uint16_t Curr,Chan,daM=0,i,jtemp,daS[MAX_AGE_CHAN]={0};
    float buff=0;

	  jtemp=(MAX_REC_COMSIZE/2);
    Chan = SysCtrl.Ethernet.Data[jtemp++];
    Chan = Chan-1;

    Curr = SysCtrl.Ethernet.Data[jtemp++];
    Curr = (uint16_t)(Curr*MUL_CURR);

	  for(i=0;i<MAX_MAIN_DA;i++)
		{
				SetMB88346BMainDA(0,i);	
		}
    for(i=0;i<MAX_AGE_CHAN;i++)
    {
		  SetMB88346BSingle(0,i);
      Ctrl_ON_Switch(i,0);
    }

    if(AGEProg.ProgType==MEASURE_MODE)
    {
			  buff = ((Curr+198)+CONSTHARF_DAVAL)*CONST_REFDAVAL;			 
			  daM = (uint16_t)buff;
			  daS[Chan]=(uint16_t)(128+ErrorCompensation_i(Curr,Chan));
    }
    else if(AGEProg.ProgType==CALI_MODE)
    {
			   buff = (Curr +CONSTHARF_DAVAL)*CONST_REFDAVAL;				 
			   daM = (uint16_t)buff;
			   daS[Chan]=128;
    }
    
		QDB_ENA_H;
		QDB_ENB_L;
		for(i=0;i<MAX_MAIN_DA;i++)
		{
				SetMB88346BMainDA(daM,i);	
		}	
		DELAY_Ms(10);
    SetMB88346BSingle(daS[Chan],Chan);
    DELAY_Ms(10);
    Ctrl_ON_Switch(Chan,1);
    DELAY_Ms(10);

    EthernetReceiveSuccess(0,0);
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
static void ControlSampleCurrent(void* parameter)
{
    uint16_t  Chan,i,Data,jtemp;

	  jtemp=(MAX_REC_COMSIZE/2);
    Chan = SysCtrl.Ethernet.Data[jtemp++];
    Chan = Chan-1;

    Ctrl_4051INCH(SELECT_4051NO1);
	  DELAY_Ms(5);
    for(i=0;i<8;i++)
    {
       Ctrl_4051ADEN(i);
			 DELAY_Ms(5);
       SampleSingleChanData(i);      
			 Ctrl_4051ADEN(8);
    }
		Ctrl_4051INCH(SELECT_4051NO);
    DELAY_Ms(5);
		
		Ctrl_4051INCH(SELECT_4051NO2);
		DELAY_Ms(5);
    for(i=0;i<8;i++)
    {
       Ctrl_4051ADEN(i);
			 DELAY_Ms(5);
       SampAGEoubleChanData(i);       
			 Ctrl_4051ADEN(8);
    }
		Ctrl_4051INCH(SELECT_4051NO);
			
    CaliChannelCurrIN(Chan);

    Data=AGEVar.IN_PTCurrent[Chan];

    EthernetReceiveSuccess(1,&Data);
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
static void SampleOutCurrent(void* parameter)
{
    uint16_t  Chan,i,Data,jtemp;

	  jtemp=(MAX_REC_COMSIZE/2);
    Chan = SysCtrl.Ethernet.Data[jtemp++];
    Chan = Chan-1;

    Ctrl_4051INCH(SELECT_4051NO1);
	  DELAY_Ms(5);
    for(i=0;i<8;i++)
    {
       Ctrl_4051ADEN(i);
			 DELAY_Ms(5);
       SampleSingleChanData(i);      
			 Ctrl_4051ADEN(8);
    }
		Ctrl_4051INCH(SELECT_4051NO);
    DELAY_Ms(5);		
		Ctrl_4051INCH(SELECT_4051NO2);
		DELAY_Ms(5);
    for(i=0;i<8;i++)
    {
       Ctrl_4051ADEN(i);
			 DELAY_Ms(5);
       SampAGEoubleChanData(i);       
			 Ctrl_4051ADEN(8);
    }
		Ctrl_4051INCH(SELECT_4051NO);
		
    CaliChannelCurrOUT(Chan);

    Data=AGEVar.OUT_PTCurrent[Chan];

    EthernetReceiveSuccess(1,&Data);
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
static void RecvCONKBData(void* parameter)
{
    uint16_t  i,num,jtemp;
    uint16_t  command;

		jtemp=(MAX_REC_COMSIZE/2);
    command = SysCtrl.Ethernet.Data[jtemp++];

    switch(command)
    {
        case 0x00 :                                                        break;
        case 0x01 :                                                        break;
        case 0x02 :                                                        break;
        case 0x03 : num=16;
					          for(i=0;i<(MAX_AGE_CHAN*MAX_CURRKB_GRADE*4);i++)
                    {
                       Data[i]=SysCtrl.Ethernet.Data[(num++)];
                    }                  
                    WriteCONInCurKBToFlash(&Data[0],(uint16_t)(MAX_AGE_CHAN*MAX_CURRKB_GRADE*4));
                    break;
        default :   break;
    }

    EthernetReceiveSuccess(0,0);
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
static void RecvFEEKBData(void* parameter)
{
    uint16_t  i,num,jtemp;
    uint16_t  command;

    jtemp=(MAX_REC_COMSIZE/2);
    command = SysCtrl.Ethernet.Data[jtemp++];
    switch(command)
    {
        case 0 :                                                        break;
        case 2 :                                                        break;
        case 1 :                                                        break;
			  case 3 :num=16;
                for(i=0;i<(MAX_AGE_CHAN*MAX_CURRKB_GRADE*4);i++)
                {
                   Data[i]=SysCtrl.Ethernet.Data[(num++)];
                }               
                WriteSampleInCurKBToFlash(&Data[0],(uint16_t)(MAX_AGE_CHAN*MAX_CURRKB_GRADE*4));
                break;
        default : break;
    }
    EthernetReceiveSuccess(0,0);
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
static void RecvOUTFEEKBData(void* parameter)
{
    uint16_t  i,num,jtemp;
    uint16_t  command;

    jtemp=(MAX_REC_COMSIZE/2);
    command = SysCtrl.Ethernet.Data[jtemp++];
    switch(command)
    {
        case 3 :                                                        break;
        case 2 :                                                        break;
        case 1 :                                                        break;
			  case 0 :num=16;
                for(i=0;i<(MAX_AGE_CHAN*MAX_CURRKB_GRADE*4);i++)
                {
                   Data[i]=SysCtrl.Ethernet.Data[(num++)];
                }              
                WriteSampleOutCurKBToFlash(&Data[0],(uint16_t)(MAX_AGE_CHAN*MAX_CURRKB_GRADE*4)); 
                break;
        default : break;
    }
    EthernetReceiveSuccess(0,0);
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
static void SendCONKBData(void* parameter)
{
    uint16_t  itemp,jtemp;
		
		for(itemp=0;itemp<MAX_AGE_CHAN;itemp++)
		{
			for(jtemp=0;jtemp<MAX_CURRKB_GRADE;jtemp++)
			{
					Data[4*jtemp+(itemp*MAX_CURRKB_GRADE*4)] = Calib[itemp].ConCChar[jtemp].KVal.bytes.lbyte;
					Data[4*jtemp+1+(itemp*MAX_CURRKB_GRADE*4)]=Calib[itemp].ConCChar[jtemp].KVal.bytes.hbyte;
					Data[4*jtemp+2+(itemp*MAX_CURRKB_GRADE*4)]=Calib[itemp].ConCChar[jtemp].BVal.bytes.lbyte;
					Data[4*jtemp+3+(itemp*MAX_CURRKB_GRADE*4)]=Calib[itemp].ConCChar[jtemp].BVal.bytes.hbyte;
			}
		}
    
    EthernetReceiveSuccess((MAX_AGE_CHAN*MAX_CURRKB_GRADE*4),&Data[0]);
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
static void SendFEEKBData(void* parameter)
{
    uint16_t  itemp,jtemp;

		for(itemp=0;itemp<MAX_AGE_CHAN;itemp++)
		{
			for(jtemp=0;jtemp<MAX_CURRKB_GRADE;jtemp++)
			{
					Data[4*jtemp+(itemp*MAX_CURRKB_GRADE*4)] = Calib[itemp].FeeCChar[jtemp].KVal.bytes.lbyte;
					Data[4*jtemp+1+(itemp*MAX_CURRKB_GRADE*4)]=Calib[itemp].FeeCChar[jtemp].KVal.bytes.hbyte;
					Data[4*jtemp+2+(itemp*MAX_CURRKB_GRADE*4)]=Calib[itemp].FeeCChar[jtemp].BVal.bytes.lbyte;
					Data[4*jtemp+3+(itemp*MAX_CURRKB_GRADE*4)]=Calib[itemp].FeeCChar[jtemp].BVal.bytes.hbyte;
			}
		}

    EthernetReceiveSuccess((MAX_AGE_CHAN*MAX_CURRKB_GRADE*4),&Data[0]);
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
static void SendOUTFEEKBData(void* parameter)
{
    uint16_t  itemp,jtemp;

		for(itemp=0;itemp<MAX_AGE_CHAN;itemp++)
		{
			for(jtemp=0;jtemp<MAX_CURRKB_GRADE;jtemp++)
			{
					Data[4*jtemp+(itemp*MAX_CURRKB_GRADE*4)] = Calib[itemp].FeeOutCChar[jtemp].KVal.bytes.lbyte;
					Data[4*jtemp+1+(itemp*MAX_CURRKB_GRADE*4)]=Calib[itemp].FeeOutCChar[jtemp].KVal.bytes.hbyte;
					Data[4*jtemp+2+(itemp*MAX_CURRKB_GRADE*4)]=Calib[itemp].FeeOutCChar[jtemp].BVal.bytes.lbyte;
					Data[4*jtemp+3+(itemp*MAX_CURRKB_GRADE*4)]=Calib[itemp].FeeOutCChar[jtemp].BVal.bytes.hbyte;
			}
		}

    EthernetReceiveSuccess((MAX_AGE_CHAN*MAX_CURRKB_GRADE*4),&Data[0]);
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
static void GetSoftWareVersion(void* parameter)
{
    uint32_t  parity;
    uint16_t  i,j,len,ret;
    uint16_t  SoftVerSend[22];

    len = sizeof(SoftVersionString);//36
    ret = (len - 1) - (((len -1) >> 1) << 1);
    len = (len -1) >> 1;

    for(i=0,j=0;i<(len);i++)
    {
        SoftVerSend[i] = SoftVersionString[j++];
        SoftVerSend[i] += (SoftVersionString[j++] << 8);
    }
    if(ret > 0)
    {
        SoftVerSend[i] = SoftVersionString[j];
        SoftVerSend[i++] += 0x2000;
    }
    for(j=0;j<(20-len-ret);j++)
    {
        SoftVerSend[i++] = 0x2020;
    }
    parity = Crs(SoftVerSend,20);
    SoftVerSend[20] = parity;
    SoftVerSend[21] = (parity >> 16);

    EthernetReceiveSuccess(22,SoftVerSend);
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
static void ManualModifyIP(void* parameter)
{
    uint16_t jtemp;
    uint16_t  *dptr,*sptr;

	  jtemp=(MAX_REC_COMSIZE/2);
    *dptr++ = 0x56aa;
    *dptr++ = 0x2aa0;
    *dptr++ =SysCtrl.Ethernet.Data[jtemp++];
    *dptr++ =SysCtrl.Ethernet.Data[jtemp++];//ip
    *dptr++ =SysCtrl.Ethernet.Data[jtemp++];
    *dptr++ =SysCtrl.Ethernet.Data[jtemp++];//sub
    *dptr++ =SysCtrl.Ethernet.Data[jtemp++];
    *dptr++ =SysCtrl.Ethernet.Data[jtemp++];//war
    *dptr++ =SysCtrl.Ethernet.Data[jtemp++];
    *dptr++ =SysCtrl.Ethernet.Data[jtemp++];//mac
    *dptr++ =SysCtrl.Ethernet.Data[jtemp++];
    SysCtrl.sflag.bit.IPModifiedEn = 1;

    EthernetReceiveSuccess(0,0);
} 
/******************************************************************
函数名称：OnlineFlashProgram()
函数功能:程序在线下载
输入参数：
输出参数：
返回值：
注意：通过上位机直接下载程序给下位机，进行程序的更新
******************************************************************/
/******************************************************************
函数更改历史记录：第一步
******************************************************************/
static void OnlineFlashProgram(void* parameter)
{
    SysCtrl.Ethernet.ETH_Length.data=0;
    SysCtrl.Ethernet.ETH_PackageLength.data=0;
    SysCtrl.Ethernet.Flash_Legth.bytes.lbyte=SysCtrl.Ethernet.Data[15];
    SysCtrl.Ethernet.Flash_Legth.bytes.hbyte=SysCtrl.Ethernet.Data[16];
    SysCtrl.Ethernet.ETH_Flag=Online;
    
    EthernetReceiveSuccess(0,0);
} 
/******************************************************************
函数名称：ProgramUpdata
函数功能：将烧写程序放入ram中暂存
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：//第二步
******************************************************************/
void ProgramUpdata(void)
{
    uint32_t length,k=0,i;
    uint16_t *ptr;
   
    if(0==SysCtrl.Ethernet.ETH_Length.data)
    {
        ptr=(uint16_t *)(0x100000);
    }
    else
    {
        ptr=(uint16_t *)(0x100000)+(SysCtrl.Ethernet.ETH_Length.data)/2;
    }
    if (length == 0)
    {
        SysCtrl.Ethernet.ETH_Flag=FALSE;
    }
    else
    {
      for (i = 0; i < (length / 2); i++)
      {
        k++;
      }
      SysCtrl.Ethernet.ETH_Flag=Online;
    }
    SysCtrl.Ethernet.ETH_Length.data=SysCtrl.Ethernet.ETH_Length.data+(k*2);
}

/******************************************************************
函数名称：ManualModifyIP(void)//2014-1-20修改了这个函数
函数功能:修改IP
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
static bool String_Verify(const char* String,char* a)
{
		uint16_t i = 0;
		for(;String[i];i ++)
		{
			if(String[i] != a[i])
			{
				return FALSE;
			}
		}
		if(a[i] != 0)
			return FALSE;
		return TRUE;
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
static uint16_t String_Size(const char* String)
{
	uint16_t String_Count = 0;
	
	for(;String[String_Count] ;String_Count ++);
	String_Count ++;
	
	return String_Count;
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
void PC_DATAHandler(char *Sptr,uint16_t len)
{
    uint16_t i;
	  char* Command = CMD_Assemble[0].CMD_String;
    char ptr[MAX_SCI_REC];
	  uint16_t CMD_Count = 0;

    for(i=0;i<len;i++)
    {
        ptr[i]=*Sptr++;
    }
		for(i=0;i<17;i++) 
    {	
			if(String_Verify(Command,(char*)(ptr+4)) == true)
			{
					DELAY_Ms(10);
				 (*CMD_Assemble[CMD_Count].Run_Function)((void*)((uint32_t)ptr + String_Size(Command)));
			}
			CMD_Count ++;
		  Command = CMD_Assemble[CMD_Count].CMD_String;
		}
} 


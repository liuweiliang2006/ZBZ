#ifndef _SYSTEM_VAR_H
#define _SYSTEM_VAR_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "MyInclude.h"
/**************************************************************************
                                 INCLUDES
***************************************************************************/
struct UINT16_BYTES
{
			 uint16_t LBYTE:8;
			 uint16_t HBYTE:8;
};

union UINT16_TYPE
{
			uint16_t   all;
			struct UINT16_BYTES byte;
};

union DATA_BYTE
{
		uint32_t data;
		struct 
		{	
			uint16_t lbyte;
			uint16_t hbyte;
		}bytes;
};

struct UINT32_BYTES
{
		 uint16_t LBYTE:8;
	   uint16_t ZBYTE:8;
	   uint16_t MBYTE:8;
	   uint16_t HBYTE:8;
};

struct UINT32_WORDS
{
			uint16_t LWORD;
			uint16_t HWORD;
};

union UINT32_TYPE
{
			uint32_t				 all;
			struct UINT32_WORDS  word;
			struct UINT32_BYTES  byte;
};

struct TIMER_REG
{
			uint16_t  MSecond;
			uint16_t  Second;
			uint16_t  Minute;
			uint16_t  Hour;
			uint16_t  Day;
			uint16_t  Momth;
			uint16_t  Year;
};


struct SYS_CTRL_BITS
{
			uint16_t KBModifiedEn:1;
			uint16_t KBModifiedST:1;
			uint16_t IPModifiedEn:1;
			uint16_t HaveProgRun:1;  
			uint16_t PowerInitDone:1;
			uint16_t rsvd:11;
};

union SYS_CTRL_REG
{
			uint16_t			   all;
			struct SYS_CTRL_BITS    bit;
};

struct ETH_TYPE_REG
{  
    union UINT32_TYPE     ETH_ReceiveLength;
    union DATA_BYTE       ETH_Length;
    union DATA_BYTE       ETH_PackageLength;

    union DATA_BYTE       Flash_Legth;

    uint16_t       ETH_Flag;
    uint16_t       Data[1536];
} ;


struct SYS_CTRL
{
    union DATA_BYTE     ChanSelL;	// ͨ��ѡ���־L
    union DATA_BYTE     ChanSelH;  // ͨ��ѡ���־L
    uint16_t            ProgDataLength;		// �������ݳ���
    struct TIMER_REG    SysTime;  // ��ǰϵͳʱ��
    union SYS_CTRL_REG  sflag;
    union DATA_BYTE     TEMP; //�ϴ����ݳ���

    //communication
    struct ETH_TYPE_REG Ethernet;

    uint16_t  DataPack_Flag;//������

    union DATA_BYTE  System_DCPower;

    union DATA_BYTE  Sample_temp[128][SAMPLECNT];
		
		union DATA_BYTE  Sample_Voltemp[8][SAMPLECNT];

    uint16_t DataSaveA[SAVE_BUFFA_SIZE];
    uint16_t DataSaveB[SAVE_BUFFB_SIZE];
		
		union DATA_BYTE  System_VCPower;
		union DATA_BYTE  System_VFPower;
    union DATA_BYTE  System_VCEPower;
		
		int16_t socket_descr;
		
		char Rec_Comand[MAX_REC_COMSIZE];
		uint16_t Rec_Data[MAX_REC_DATASIZE];
		
		uint16_t Command;
		
		uint16_t LastSec;
		uint16_t LastMin;
		uint8_t  Ip_Temp;
};


struct AGE_VAR
{
			// ��ǰ��������״̬�������1���ŵ���2��������0
			uint16_t  PTMmtState; // present step state
			uint16_t  StepState;   // step state
			uint16_t  SchState;   // step state

			union DATA_BYTE  StepTimeCnt; // step timer counter

			union DATA_BYTE  ProtectTimeCnt; // step timer counter

			union DATA_BYTE  PTVoltage[MAX_VOL_CHAN];	// present voltage
	    uint16_t  WARNVFormData[MAX_VOL_CHAN];
	
			uint16_t  IN_PTCurrent[MAX_AGE_CHAN]; // present current			
			uint16_t  WARNINCFormData[MAX_AGE_CHAN];
			uint16_t  OUT_PTCurrent[MAX_AGE_CHAN]; // present current
	    uint16_t  WARNOUTCFormData[MAX_AGE_CHAN];

			uint16_t   deltime_s;

      uint16_t  BoardState;
	    

};



///////////////////////////////////////////////////////////////////////////////
// ��ؿ��Ʋ���
///////////////////////////////////////////////////////////////////////////////
struct AGEFLAG_BITS
{
	uint16_t FTCH:1;		// first charge // ���̵�һ�γ���־;����һ�γ��ʱ,�ñ�־��1,ѭ���Ŵ����ʱ��ʼ�ۼ�
	uint16_t PGEST:1;		// �����ļ����� program exist
	uint16_t STJP:1;		// step jump			  ;  1  jump     ;   0  none
	uint16_t PGST:1;		// program(flow) start	  ;  1  start     ;   0  none
	uint16_t PGRN:1;		// program(flow) running  ;  1  run     ;   0  stop
	uint16_t VCPL:1;  	// voltage-cable voltage polarity  ;  1  negetive     ;   0  positive
	uint16_t CCPL:1;  	// current-cable voltage polarity  ;  1  negetive     ;   0  positive
	uint16_t SFEN:1;      // safeguard delay enable          ;  1  enable     ;   0  disable
	uint16_t PTEN:1;      // step jump              ;  1  jump     ;   0  none
	uint16_t DCPL:1;
	uint16_t rsvd:6;
};

union AGEFLAG_REG
{
	uint16_t				 all;
	struct AGEFLAG_BITS bit;
};


struct AGE_CTRL
{
	union AGEFLAG_REG   bflag;

	struct TIMER_REG  SampTime;	// �������ʱ��
	struct TIMER_REG  ProgTime; // ����ʱ��
	struct TIMER_REG  StepTime; // ����ʱ��
	
	uint16_t   VUpWnCnt[MAX_VOL_CHAN];
	uint16_t   VDwWnCnt[MAX_VOL_CHAN];

	uint16_t   IINUpWnCnt[MAX_AGE_CHAN];
	uint16_t   IINDoWnCnt[MAX_AGE_CHAN];
	
	uint16_t   IOUTUpWnCnt[MAX_AGE_CHAN];
	uint16_t   IOUTDoWnCnt[MAX_AGE_CHAN];
	
	uint16_t   Ctrl_AGE_RUN[MAX_AGE_CHAN];
	

};

///////////////////////////////////////////////////////////////////////////////
// ���̲�������
///////////////////////////////////////////////////////////////////////////////

struct AGE_PROG
{
	// ��ͨ�����̲���
	uint16_t	ProgType;			//��������
  union DATA_BYTE  Con_EndValTime_RUN;
  uint16_t  StepMode;          //step mode

  union DATA_BYTE  Con_HighVoltage;
  union DATA_BYTE  Con_LowVoltage;

  uint16_t  Con_CurrentIN_RUN;
  uint16_t  Con_UPCurrentIN_RUN;
  uint16_t  Con_DownCurrentIN_RUN;
	uint16_t  Con_UPCurrentOUT_RUN;
  uint16_t  Con_DownCurrentOUT_RUN;
	
	union DATA_BYTE  Con_UPVolIN_RUN;
  union DATA_BYTE  Con_DownVolIN_RUN;
	union DATA_BYTE  Con_Power_RUN;
  union DATA_BYTE  Con_Powerfluct_RUN;

};


///////////////////////////////////////////////////////////////////////////////
// У׼KBֵ����
///////////////////////////////////////////////////////////////////////////////

struct KB_VALUE
{	
	union DATA_BYTE  KVal;
	union DATA_BYTE  BVal;
};

typedef struct CMD_Type
{
	char* CMD_String;			
	void (*Run_Function)(void* parameter);
}CMD_Type;


typedef struct CAL_REGS
{
	struct KB_VALUE  ConCChar[MAX_CURRKB_GRADE];
	struct KB_VALUE  FeeCChar[MAX_CURRKB_GRADE];
  struct KB_VALUE  FeeOutCChar[MAX_CURRKB_GRADE];
	uint16_t TempValue;
}CAL_REGS;

extern  struct AGE_VAR         AGEVar;
extern  struct SYS_CTRL        SysCtrl;
extern  struct AGE_CTRL        AGECtrl;
extern  struct AGE_PROG        AGEProg;
extern  struct CAL_REGS        Calib[MAX_AGE_CHAN];


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif

//===========================================================================
// End of file.
//===========================================================================


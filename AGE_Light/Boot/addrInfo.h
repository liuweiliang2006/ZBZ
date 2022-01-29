#ifndef _ADDRINFO_H
#define _ADDRINFO_H
#include "main.h"

//���Խӿ�
#define LOAD_APP	1
//#define DEBUG	1

/*Bootloader 16K*/
#define Bootloader_FLASHAddr 			((u32)0x08000000)	//bootloader��ʼ��ַ 140K
#define Bootloader_FLASH_End			((u32)0x08022fff)	//bootloader������ַ
/*app 180K*/	
#define APP_FLASHAddr							((u32)0x08023000)	//APP1��ʼ��ַ
#define APP_FLASH_End							((u32)0x0804ffff)	//APP1������ַ
/*������ 180K*/	
#define Cache_FLASHAddr							((u32)0x08050000)	//APP2��ʼ��ַ
#define Cache_FLASH_End							((u32)0x0807CFFF)	//APP2������ַ

#define APP_FLASH_MAX  (180*1024)

/*��־λ����� 1K*/	
#define Symbol_FLASHAddr					((u32)0x0807FC00)	//��־λ�������ʼ��ַ
#define Symbol_FLASH_End					((u32)0x0807FFFF)	//��־λ�����������ַ
#define Restart_Flag							((u32)0x0807FC00)	//������־λ���ñ�־λbootloader��APP����
#define Updata_Flag								((u32)0x08024002)	//�����Ƿ�ɹ���־λ���ñ�־λbootloader��APP����
#define StartCopy_Flag						((u32)0x08024004)	//��������־

	
/*Ԥ����366K*/	
//#define Resever_FLASHAddr					((u32)0x08024800)	//Ԥ����ʼ��ַ
//#define Resever_FLASH_End					((u32)0x0807ffff)	//Ԥ��������ַ

#define LENGTH (10*1024)

typedef enum {UPDATE = 1, NOT_UPDATE = !UPDATE} RestartFlag; //flashֻ����1���0

extern uint8_t ucReciveBuffer[520];
extern uint16_t ucSalvePackLen;
extern uint8_t g_ucBackUPApp[LENGTH];
extern uint32_t FlagAddr[3];

extern uint8_t g_ucSendFlag;
extern uint8_t g_ucCmd;
extern uint16_t g_ucFrameNumPre,g_ucFrameNumCur;
#endif

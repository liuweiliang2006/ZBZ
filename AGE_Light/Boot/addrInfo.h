#ifndef _ADDRINFO_H
#define _ADDRINFO_H
#include "main.h"

//调试接口
#define LOAD_APP	1
//#define DEBUG	1

/*Bootloader 16K*/
#define Bootloader_FLASHAddr 			((u32)0x08000000)	//bootloader起始地址 140K
#define Bootloader_FLASH_End			((u32)0x08022fff)	//bootloader结束地址
/*app1 180K*/	
#define APP1_FLASHAddr							((u32)0x08023000)	//APP1起始地址
#define APP1_FLASH_End							((u32)0x0804ffff)	//APP1结束地址
/*app2 180K*/	
#define APP2_FLASHAddr							((u32)0x08050000)	//APP2起始地址
#define APP2_FLASH_End							((u32)0x0807CFFF)	//APP2结束地址

#define APP_FLASH_MAX  (180*1024)

/*缓存备份区 64K*/	
//#define Cache_FLASHAddr						((u32)0x08014000)	//备份区起始地址
//#define Cache_FLASH_End						((u32)0x08023fff)	//备份区结束地址
/*标志位存放区 1K*/	
#define Symbol_FLASHAddr					((u32)0x0807FC00)	//标志位存放区起始地址
#define Symbol_FLASH_End					((u32)0x0807FFFF)	//标志位存放区结束地址
#define Restart_Flag							((u32)0x0807FC00)	//重启标志位，该标志位bootloader与APP共用
#define ACTIVE_Flag								((u32)0x08024002)	//升级是否成功标志位，该标志位bootloader与APP共用
#define UPDATAFLAG_Flag						((u32)0x08024004)	//允许拷贝标志

	
/*预留区366K*/	
//#define Resever_FLASHAddr					((u32)0x08024800)	//预留起始地址
//#define Resever_FLASH_End					((u32)0x0807ffff)	//预留结束地址

#define LENGTH (10*1024)

typedef enum {UPDATE = 1, NOT_UPDATE = !UPDATE} RestartFlag; //flash只能由1变成0

extern uint8_t ucReciveBuffer[520];
extern uint16_t ucSalvePackLen;
extern uint8_t g_ucBackUPApp[LENGTH];
extern uint32_t FlagAddr[3];

extern uint8_t g_ucSendFlag;
extern uint8_t g_ucCmd;
extern uint16_t g_ucFrameNumPre,g_ucFrameNumCur;
#endif

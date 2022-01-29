#ifndef _BOOT_API_H
#define _BOOT_API_H

#include "main.h"

#define PACK_FIX_LEN 40 //每包数据中除了数据字段的固定长度



void delay_100us(uint32_t count);
ErrorStatus WriteAppData(uint32_t length,char *recData);
FLASH_Status iap_write_APP_appbin(char *appbuf,uint32_t appsize,uint16_t number);
struct stResponseFrame
{
	uint8_t Head[4];
	uint8_t Cmd[20];
	uint8_t Reserve[2];
	uint8_t IPAdress[4];
	uint8_t Result[4];
	uint8_t Length[4];
	uint8_t CheckSum[4];
};

extern struct stResponseFrame response;
extern uint8_t recAllDataFlag;
#endif

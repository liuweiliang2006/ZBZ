#ifndef _BOOT_API_H
#define _BOOT_API_H

#include "main.h"

extern uint8_t head[4];
extern uint8_t tail[4];

void delay_100us(uint32_t count);
ErrorStatus WriteAppData(uint32_t length,char *recData);

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
#endif

#ifndef _BOOT_API_H
#define _BOOT_API_H

#include "main.h"

extern uint8_t head[4];
extern uint8_t tail[4];

void delay_100us(uint32_t count);
uint8_t WriteAppData(uint32_t length,char *recData);
#endif

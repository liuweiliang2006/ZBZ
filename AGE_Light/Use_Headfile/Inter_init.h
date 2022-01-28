#ifndef __INTER_INIT_H__
#define __INTER_INIT_H__

#include "main.h"
#include "MyInclude.h"

#define LED1_Port  GPIOA
#define LED1_Pin   GPIO_Pin_0
#define LED2_Port  GPIOA
#define LED2_Pin   GPIO_Pin_3

#define LEDR_Port  GPIOD
#define LEDR_Pin   GPIO_Pin_4
#define LEDG_Port  GPIOD
#define LEDG_Pin   GPIO_Pin_5

#define   LED1_ON()       GPIO_ResetBits(LED1_Port, LED1_Pin)
#define   LED1_OFF()      GPIO_SetBits(LED1_Port, LED1_Pin)
#define   LED1_TOGGLE()  (GPIO_ReadOutputDataBit(LED1_Port,LED1_Pin))?(GPIO_ResetBits(LED1_Port,LED1_Pin)):(GPIO_SetBits(LED1_Port,LED1_Pin))

#define   LED2_ON()       GPIO_ResetBits(LED2_Port, LED2_Pin)
#define   LED2_OFF()      GPIO_SetBits(LED2_Port, LED2_Pin)
#define   LED2_TOGGLE()  (GPIO_ReadOutputDataBit(LED2_Port,LED2_Pin))?(GPIO_ResetBits(LED2_Port,LED2_Pin)):(GPIO_SetBits(LED2_Port,LED2_Pin))


#define   LEDR_ON()       GPIO_ResetBits(LEDR_Port, LEDR_Pin)
#define   LEDR_OFF()      GPIO_SetBits(LEDR_Port, LEDR_Pin)
#define   LEDR_TOGGLE()  (GPIO_ReadOutputDataBit(LEDR_Port,LEDR_Pin))?(GPIO_ResetBits(LEDR_Port,LEDR_Pin)):(GPIO_SetBits(LEDR_Port,LEDR_Pin))

#define   LEDG_ON()       GPIO_ResetBits(LEDG_Port, LEDG_Pin)
#define   LEDG_OFF()      GPIO_SetBits(LEDG_Port, LEDG_Pin)
#define   LEDG_TOGGLE()  (GPIO_ReadOutputDataBit(LEDG_Port,LEDG_Pin))?(GPIO_ResetBits(LEDG_Port,LEDG_Pin)):(GPIO_SetBits(LEDG_Port,LEDG_Pin))


extern void App_LEDGpioCfg(void);
extern void InitInnerPeripheral(void);
extern void InitLedSysParameter(void);
extern void InitLedSysContral(void);
#endif





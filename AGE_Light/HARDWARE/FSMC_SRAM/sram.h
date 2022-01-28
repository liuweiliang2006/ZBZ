////////////////////////////////////////////////////////////////////////////////
/// @file    sram.h
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FIRMWARE FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////


// Define to prevent recursive inclusion
#ifndef __SRAM_H
#define __SRAM_H

// Files includes


#include "hal_device.h"
#include "stdio.h"

extern void sram_writeread(void);
extern void SRAM_PIN_GPIO_Config(void);
extern void FSMC_SRAM_Init(void);

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Example_Layer
/// @brief MM32 Example Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_RESOURCE
/// @brief MM32 Examples resource modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Constants
/// @{

#define Bank1_SRAM3_ADDR    ((uint32_t)(0x60020000))    //used NE1  PD7
//#define Bank1_SRAM3_ADDR    ((uint32_t)(0x64000000))  //used NE2  PG9
//#define Bank1_SRAM3_ADDR    ((uint32_t)(0x68000000))  //used NE3  PG10
//#define Bank1_SRAM3_ADDR    ((uint32_t)(0x6C000000))  //used NE4  PG12
//#define IS62WV51216_SIZE 0x100000  //512*16/2bits = 0x100000
//#define IS62WV51216_SIZE 0x80000  //256*16/2bits = 0x100000


//#define IS62WV51216_SIZE 0x40000  //256K  //128K*16/8bits = 0x40000 262144 = 2^18


//#define IS62WV51216_SIZE 0x4000000 //64M
//#define IS62WV51216_SIZE 0x2000000 //32M
//#define IS62WV51216_SIZE 0x1000000 //16M
//#define IS62WV51216_SIZE 0x800000  //8M
//#define IS62WV51216_SIZE 0x400000  //4M
//#define IS62WV51216_SIZE 0x200000  //2M
//#define IS62WV51216_SIZE 0x100000 //1M
//#define IS62WV51216_SIZE 0x80000  //512K
//#define IS62WV51216_SIZE 0x40000 //256K
//#define IS62WV51216_SIZE 0x32000  //200k
//#define IS62WV51216_SIZE 0x25800  //150k
//#define IS62WV51216_SIZE 0x20032   //128K+50
//#define IS62WV51216_SIZE (0x20000+1)

//#define IS62WV51216_SIZE 0x20000 //128K
//#define IS62WV51216_SIZE 0x10000 //64K
//#define IS62WV51216_SIZE 100
#define IS62WV51216_SIZE 10
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Enumeration
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief XXXX enumerate definition.
/// @anchor XXXX
////////////////////////////////////////////////////////////////////////////////

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Variables
/// @{
#ifdef _SRAM_C_
#define GLOBAL







#else
#define GLOBAL extern







#endif





#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Functions
/// @{

void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);

uint8_t SRAM_Test(void);

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////


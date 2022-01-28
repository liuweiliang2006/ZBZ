////////////////////////////////////////////////////////////////////////////////
/// @file    sram.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
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
#define _SRAM_C_

// Files includes
#include "hal_conf.h"
#include "sram.h"
#include "MyInclude.h"
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SRAM
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SRAM_Exported_Constants
/// @{


void FSMC_SRAM_Init(void)
{
    FSMC_InitTypeDef  FSMC_InitStructure;
    FSMC_NORSRAM_Bank_InitTypeDef  FSMC_BankInitStructure;

    FSMC_NORSRAM_BankStructInit(&FSMC_BankInitStructure);
    FSMC_NORSRAMStructInit(&FSMC_InitStructure);

    RCC_AHB3PeriphClockCmd(RCC_AHB3ENR_FSMC, ENABLE);                           //enable FSMCEN
#define USING96MHZ
#if defined(USINGHSI8MHZ)
    FSMC_BankInitStructure.FSMC_SMReadPipe    = 0;
    FSMC_BankInitStructure.FSMC_ReadyMode     = 0;
    FSMC_BankInitStructure.FSMC_WritePeriod   = 0x1F;
    FSMC_BankInitStructure.FSMC_WriteHoldTime = 0;
    FSMC_BankInitStructure.FSMC_AddrSetTime   = 1;
    FSMC_BankInitStructure.FSMC_ReadPeriod    = 0x2;
    FSMC_BankInitStructure.FSMC_DataWidth     = FSMC_DataWidth_8bits;
    FSMC_NORSRAM_Bank_Init(&FSMC_BankInitStructure, FSMC_NORSRAM_BANK0);
#endif
#if defined(USING96MHZ)// for 96M HSI PLL Trying
//  Only for write and read external DEVICE operations. 0: Internal FSMC 1: External DEVICE(i.e. from FSMC_NWAIT)
    FSMC_BankInitStructure.FSMC_SMReadPipe    = 0;                            // SM_read_pipe [1:0] the cycle of latch read data, namely the cycle of hready_RESP pull up
    FSMC_BankInitStructure.FSMC_ReadyMode     = 0;                            // Select whether the Hready_RESP signal comes from inside the FSMC IP or from an external DEVICE.
    FSMC_BankInitStructure.FSMC_WritePeriod   = 7;                            // Write operation cycle, configuration large continuous write will have an exception
    FSMC_BankInitStructure.FSMC_WriteHoldTime = 10;                            // The address of the write operation and the retention time of the data
    FSMC_BankInitStructure.FSMC_AddrSetTime   = 1;                            // Address setup time
    FSMC_BankInitStructure.FSMC_ReadPeriod    = 9;                            // Read operation cycle
    FSMC_BankInitStructure.FSMC_DataWidth     = FSMC_DataWidth_8bits;
    FSMC_NORSRAM_Bank_Init(&FSMC_BankInitStructure, FSMC_NORSRAM_BANK0);
#endif

    FSMC_InitStructure.FSMC_Mode = FSMC_Mode_NorFlash;
    FSMC_InitStructure.FSMC_TimingRegSelect = FSMC_TimingRegSelect_0;
    FSMC_InitStructure.FSMC_MemSize = FSMC_MemSize_64MB;
    FSMC_InitStructure.FSMC_MemType = FSMC_MemType_NorSRAM;
    FSMC_InitStructure.FSMC_AddrDataMode = FSMC_AddrDataDeMUX;
    FSMC_NORSRAMInit(&FSMC_InitStructure);
}
#define BUFFERLEN  512
u16 writebuffer[BUFFERLEN];
u16 readbbuffer[BUFFERLEN];//read back buffer


u16 compareu16data(u16* pbuffera, u16* pbufferb, u16 len)
{
    u16 i, u16a, u16b, counter = 0;
    for(i = 0; i < len; i++) {
        u16a = *pbuffera++;
        u16b = *pbufferb++;
        if( u16a != u16b) {
            counter++;
        }
    }
    return counter;
}

void sram_demo(void)
{
    vu8* p;
	  vu8* sptr;
    u32 i, counter = 0,temp;
    for(i = 0; i < BUFFERLEN; i++) {
        writebuffer[i] = 0x02;
    }
    for(i = 0; i < BUFFERLEN; i++) {
        readbbuffer[i] = 0x02;
    }
		sptr=(u8*)READSTATE_ADDR;
		temp=*sptr;
    p = (u8*)IO_CTLBUS_ADDR;
    for (i = 0x00; i < BUFFERLEN; i++) {
        //M16(Bank1_SRAM3_ADDR + i) = (u16)writebuffer[i];
        *p++ = (u8)writebuffer[i];
    }
    p = (u8*)Bank1_SRAM3_ADDR;
    for (i = 0x00; i < BUFFERLEN; i++) {
        //readbbuffer[i] =M16(Bank1_SRAM3_ADDR + i);
        readbbuffer[i] = *p++;
    }
    counter = compareu16data(&writebuffer[0], &readbbuffer[0], BUFFERLEN);
    if(counter > 0) {
//        printf("16 bit data read/write error£¡\n");
    }
    else {
//        printf("16 bit data read/write successful\n");
    }
    for(i = 0; i < BUFFERLEN; i++) {
        writebuffer[i] = i;
    }
/*    p = (u16*)Bank1_SRAM3_ADDR;
    for (i = 0x00; i < BUFFERLEN; i++) {
        //M16(Bank1_SRAM3_ADDR + i) = (u16)writebuffer[i];
        *p++ = (u16)writebuffer[i];
//        printf("Data=0x%04x\n", (u16)writebuffer[i]);
    }
    p = (u16*)Bank1_SRAM3_ADDR;
    for (i = 0x00; i < BUFFERLEN; i++) {
        //readbbuffer[i] = M16(Bank1_SRAM3_ADDR + i);
        readbbuffer[i] = *p++;
//        printf("Data=0x%04x\n", (u16)readbbuffer[i]);
    }
    counter = compareu16data(&writebuffer[0], &readbbuffer[0], BUFFERLEN);
    if(counter > 0) {
//        printf("16 bit data read/write error£¡\n");
    }
    else {
//        printf("16 bit data read/write successful\n");
    }*/
}

/// @}

/// @}

/// @}

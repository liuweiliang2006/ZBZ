#ifndef _FLASH_DATA_H
#define _FLASH_DATA_H

#include "main.h"
#define  FLASH_WORD          0x800
#define  WFLASH_INDEX        256
#define  MAX_FLASH_PAGE      40
#define  FLASH_WFLAG         0x5A5A


//2048个字节
#define  ADDR_FLASH_SECTOR_1      (u32)0x08040000
//2048个字节
#define  ADDR_FLASH_SECTOR_2      (u32)(ADDR_FLASH_SECTOR_1+FLASH_WORD)
//2048个字节
#define  ADDR_FLASH_SECTOR_3      (u32)(ADDR_FLASH_SECTOR_2+FLASH_WORD)
//2048个字节
#define  ADDR_FLASH_SECTOR_4      (u32)(ADDR_FLASH_SECTOR_3+FLASH_WORD)

//2048个字节
#define  CONCUR_FLASH_B      ADDR_FLASH_SECTOR_1

//2048个字节
#define  SAMCUR_FLASH_B      ADDR_FLASH_SECTOR_2

//2048个字节
#define  OUTSAMCUR_FLASH_B   ADDR_FLASH_SECTOR_3

//2048个字节
#define  TEMP_FLASH_B        ADDR_FLASH_SECTOR_4

//extern void WriteIPValueToFlash(uint16_t *source,uint16_t len);
extern void WriteCONInCurKBToFlash(uint16_t *Data,uint16_t len);
extern void WriteSampleInCurKBToFlash(uint16_t *Data,uint16_t len);
extern void WriteSampleOutCurKBToFlash(uint16_t *Data,uint16_t len);
//extern void WriteTempValueToFlash(uint16_t *source,uint16_t len);


//extern void ReadTempFlashBlock(uint16_t *pdata, uint16_t Length);
extern void ReadConInCurFlashBlock(void);
extern void ReadSampleInCurFlashBlock(void);
extern void ReadSampleOutCurFlashBlock(void);

#endif




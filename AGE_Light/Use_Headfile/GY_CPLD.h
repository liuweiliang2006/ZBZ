#ifndef _GY_CPLD_H
#define _GY_CPLD_H

#include "main.h" 

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))


#define EN_DBUS_ADDR        M8(0x60010000)
#define CS_DBUS_ADDR        M8(0x60020000)
#define DATA_DBUS_ADDR      M8(0x60030000)
#define PWR_CTLBUS_ADDR     M8(0x60040000)
#define IO_CTLBUS_ADDR      M8(0x60050000)
#define FPGAIO_T4A_ADDR     M8(0x60060000)
#define READSTATE_ADDR      M8(0x60080000)

#define  ADDR_EN_DBUS     1
#define  ADDR_CS_DBUS     2 
#define  ADDR_DATA_DBUS   3
#define  ADDR_PWR_DBUS    4
#define  ADDR_IO_DBUS     5


extern void WriteDataByte(uint8_t S_245No,uint8_t data);

#endif




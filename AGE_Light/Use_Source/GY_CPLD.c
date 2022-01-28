#include "main.h"
#include "MyInclude.h" 
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void WriteDataByte(uint8_t S_245No,uint8_t data)
{
	 switch(S_245No)
	 {
	   case ADDR_EN_DBUS:   EN_DBUS_ADDR = (u8)data;break;
		 case ADDR_CS_DBUS:   CS_DBUS_ADDR = (u8)data;break;
		 case ADDR_DATA_DBUS: DATA_DBUS_ADDR = (u8)data;break;
		 case ADDR_PWR_DBUS:  PWR_CTLBUS_ADDR = (u8)data;break;
		 case ADDR_IO_DBUS:   IO_CTLBUS_ADDR = (u8)data;break;
		 default:data=0;break;
	 }
}




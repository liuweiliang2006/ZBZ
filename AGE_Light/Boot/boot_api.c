
#include "boot_api.h"
#include "addrinfo.h"



struct stResponseFrame response= {
	//head
	{0xaa,0xbb,0xcc,0xDD},
	//cmd
	{0x4F,0x4E,0x4C,0x49,0x4E,0x45,0x55,0x50,0x44,0x41,0x54,0x45,0x32,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	//reserve
	{0x00,0x00},
	//IP
	{192,168,0,1},
	//result
	{0x00,0x00},
	//Length
	{0x00,0x00,0x00,0x28},
	//checksum
	{0x00,0x00,0x00,0x00}
};
uint8_t head[4] = {0xaa,0xbb,0xcc,0xDD};
uint8_t recAllDataFlag = 0;  //用于标识.BIN文件内的数据均以全部接收完成

void delay_100us(uint32_t count)
{
		uint32_t i,j;
		
		for(j=0; j<count; j++)
		{
			 for(i=0;i<660;i++);
		}
}

FLASH_Status iap_write_cache_appbin(char *appbuf,uint32_t appsize,uint16_t number)
{
	FLASH_Status status ;
	uint16_t t;
	uint16_t i=0;
	uint16_t temp;
	uint32_t fwaddr=Cache_FLASHAddr+number*1024;//当前写入的地址
	
	char *dfu=appbuf;
	
	FLASH_Unlock();
	for(t=0;t<appsize;t+=2)
	{						    
		temp=(u16)dfu[1]<<8;
		temp+=(u16)dfu[0];	  
		dfu+=2;//偏移2个字节
		status = FLASH_ProgramHalfWord(fwaddr,temp);
		if(status != FLASH_COMPLETE)
		{
			break;
		}
		fwaddr += 2;
	}
	FLASH_Lock();

	return status;	
}

FLASH_Status iap_write_APP_appbin(char *appbuf,uint32_t appsize,uint16_t number)
{
	FLASH_Status status ;
	uint16_t t;
	uint16_t i=0;
	uint16_t temp;
	uint32_t fwaddr=APP_FLASHAddr+number*1024;//当前写入的地址
	
	char *dfu=appbuf;
	
	FLASH_Unlock();
	for(t=0;t<appsize;t+=2)
	{						    
		temp=(u16)dfu[1]<<8;
		temp+=(u16)dfu[0];	  
		dfu+=2;//偏移2个字节
		status = FLASH_ProgramHalfWord(fwaddr,temp);
		if(status != FLASH_COMPLETE)
		{
			break;
		}
		fwaddr += 2;
	}
	FLASH_Lock();

	return status;	
}

ErrorStatus WriteAppData(uint32_t length,char *recData)
{
	uint32_t i =0;
	ErrorStatus result = 0;
	uint32_t checkSum = 0;
	uint32_t dataLength = 0;
	uint16_t packNum = 0;
	static uint8_t writePart = 0;
	char upgradeDATA[1024] = {0};
	FLASH_Status status ;

	
	if ((recData[0] != head[0]) || (recData[1] != head[1]) || (recData[2] != head[2]) ||(recData[3] != head[3])) 
	{
		result = ERROR;
		return result;
	}
	//CHECK SUM
	checkSum = (recData[length-8]<<24)+ (recData[length-7]<<16) + (recData[length-6]<<8) +(recData[length-5]);
	//data length
	dataLength = (recData[26]<<24)+ (recData[27]<<16) + (recData[28]<<8) +(recData[29])-PACK_FIX_LEN;
	//pack number
	packNum = (recData[30]<<8) + recData[31];
	
	uint32_t calSum = 0;
	for( i = 0; i < length-8; i++)
	{
		calSum += recData[i];
	}
	
	if(calSum != checkSum)
	{
		result = ERROR;
		return result;
	}
	
	if(packNum != 0 )
	{ //not last pack
		memcpy(upgradeDATA,&recData[32],1024);
		status =iap_write_cache_appbin(upgradeDATA,1024,writePart);
	}
	else
	{// it is last pack
		memcpy(upgradeDATA,&recData[32],dataLength);
		if(dataLength%2 != 0)
		{//最后一包数据最后一个字节，凑够补成半字
			upgradeDATA[dataLength++] = 0xff;
		}
		status= iap_write_cache_appbin(upgradeDATA,dataLength,writePart);
		if(status == FLASH_COMPLETE)
			recAllDataFlag =1;
	}
	writePart++;
	
	if(status==FLASH_COMPLETE)
	{
		result = SUCCESS;
	}
	else
	{
		result = ERROR;
	}
	
	if (result == SUCCESS)
	{
		response.Result[0] = 0x88;
		response.Result[1] = 0x33;
	}
	else
	{
		response.Result[0] = 0x44;
		response.Result[1] = 0x11;
	}
	return result;
	
}
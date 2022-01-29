/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#include "tcpecho.h"

#include "lwip/opt.h"

#if LWIP_SOCKET
#include <lwip/sockets.h>

#include "lwip/sys.h"
#include "lwip/api.h"
#include "MyInclude.h"
#include "boot_api.h"
#include "addrinfo.h"
#include "iap.h"
/*-----------------------------------------------------------------------------------*/

#define RECV_DATA         (1200)


int32_t iDataLength;

void  tcpecho_thread(void* arg)
{
	  int sock = -1, connected;
    char* recv_data;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;
		ErrorStatus result;
    int j,i;
		uint8_t resData[PACK_FIX_LEN];
		
//    printf("The local port number is%d\n\n", LOCAL_PORT);

    recv_data = (char*)pvPortMalloc(RECV_DATA);
    if (recv_data == NULL) {
//        printf("No memory\n");
        goto __exit;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
//        printf("Socket error\n");
        goto __exit;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(LOCAL_PORT);
    memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
       // printf("Unable to bind\n");
        goto __exit;
    }

    if (listen(sock, 5) == -1) {
      //  printf("Listen error\n");
        goto __exit;
    }

    while(1) {
           sin_size = sizeof(struct sockaddr_in);

           SysCtrl.socket_descr = accept(sock, (struct sockaddr*)&client_addr, &sin_size);

//           printf("new client connected from (%s, %d)\n",
 //              inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
           {
            int flag = 1;

            setsockopt(SysCtrl.socket_descr,
                       IPPROTO_TCP,     /* set option at TCP level */
                       TCP_NODELAY,     /* name of option */
                       (void*) &flag,   /* the cast is historical cruft */
                       sizeof(int));    /* length of option value */
           }

           while(1) 
				   {
            iDataLength = recv(SysCtrl.socket_descr, recv_data, RECV_DATA, 0);
						if (iDataLength <= 0)
							break;
						if(iDataLength >20)
						{
							result =WriteAppData(iDataLength,recv_data);
							memset(resData,0,PACK_FIX_LEN);
							memcpy(resData,&response,36);
							uint32_t calCheckSum = 0;
							for(uint8_t i = 0;i < 36; i++)
							{
								calCheckSum += resData[i];
							}
							resData[36] = calCheckSum & 0xff;
							resData[37] = calCheckSum & 0xff00 >> 8;
							resData[38] = calCheckSum & 0xff0000 >> 16;
							resData[39] = calCheckSum & 0xff000000 >> 24;
							
							write(SysCtrl.socket_descr, resData, PACK_FIX_LEN);
							
							if(result == SUCCESS)
							{
								continue;
							}
							else
							{
								break;
							}
						}
						
						if(recAllDataFlag)
						{//正确接收到所有数据
							//将APP区擦除,大小180K
							char ucBackUPApp[1024]={0};
							FLASH_Unlock();
							for(uint8_t i=0;i<180;i++)
							{
								FLASH_ErasePage(APP_FLASHAddr+i*1024);
							}
							FLASH_Lock();
							for(uint8_t i =0; i < 180; i++)
							{
								for(uint16_t j=0; j<1024; j++)
								{
									ucBackUPApp[j] = *(uint8_t*) (Cache_FLASHAddr+i*1024+j);
								}
								iap_write_APP_appbin(ucBackUPApp,1024,i);
								memset(ucBackUPApp,0,1024);
							}
							//整个APP更新成功，更新标志位，跳转至APP
							FLASH_Unlock();
							FLASH_ErasePage(Symbol_FLASHAddr);
							FLASH_ProgramHalfWord(FlagAddr[0],0x00); //升级标志位置1
							FLASH_ProgramHalfWord(FlagAddr[1],0x01); //升级成功标志位清零。1：成功，0：失败
							iap_load_app(APP_FLASHAddr); //建议启动了操作系统，最好软件重启跳至APP
						}

        }
        if (SysCtrl.socket_descr >= 0)
            closesocket(SysCtrl.socket_descr);

        SysCtrl.socket_descr = -1;
    }
__exit:
    if (sock >= 0) closesocket(sock);
    if (recv_data) free(recv_data);
		//失败，更新标志位，跳转APP，到此步新的APP没有刷写完成，执行老的APP
		FLASH_Unlock();
		FLASH_ErasePage(Symbol_FLASHAddr);
		FLASH_ProgramHalfWord(FlagAddr[0],0x00); //升级标志位置1
		FLASH_ProgramHalfWord(FlagAddr[1],0x00); //升级成功标志位清零。1：成功，0：失败
		iap_load_app(APP_FLASHAddr);//建议启动了操作系统，最好软件重启跳至APP
}
							
/*-----------------------------------------------------------------------------------*/
void
tcpecho_init(void)
{
    sys_thread_new("tcpecho_thread", tcpecho_thread, NULL, 512, 4);
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */

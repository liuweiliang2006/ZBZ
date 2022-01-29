////////////////////////////////////////////////////////////////////////////////
/// @file    main.c
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
#define _MAIN_C_

// Files includes
#include "main.h"
#include <lwip/opt.h>
#include <lwip/arch.h>
#include "tcpip.h"
#include "reg_rcc.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "ethernetif.h"
#include "netif/ethernet.h"
#include "lwip/def.h"
#include "lwip/stats.h"
#include "lwip/etharp.h"
#include "lwip/ip.h"
#include "lwip/snmp.h"
#include "lwip/timeouts.h"
//#include "tcpclient.h"

#include "eth_bsp_config.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "tcpecho.h"

#include "MyInclude.h"

#include "tcpecho.h"

#include "addrinfo.h"
#include "boot_api.h"
#include "drv_sys.h"
#include "iap.h"

static TaskHandle_t AppTaskCreate_Handle = NULL; // Create task handle
static TaskHandle_t Program_Task_Handle = NULL; // Program_Task handle
//static TaskHandle_t Test2_Task_Handle = NULL; // Test2_Task handle


static void AppTaskCreate(void); // Used to create tasks

static void Program_Task(void* pvParameters); // Program_Task task implementation
//static void Test2_Task(void* pvParameters); // Test2_Task task implementation

extern void TCPIP_Init(void);


extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
    uint32_t ulReturn;
    // Enter the critical section, the critical section can be nested
    ulReturn = taskENTER_CRITICAL_FROM_ISR();

    //HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
#endif  /* INCLUDE_xTaskGetSchedulerState */
        xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
    }
#endif  /* INCLUDE_xTaskGetSchedulerState */

    // Exit the critical section
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

extern xSemaphoreHandle s_xSemaphore;
void ETH_RxCpltCallback(void)
{
    //LED2_TOGGLE;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR( s_xSemaphore, &xHigherPriorityTaskWoken );
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
void ETH_IRQHandler()
{
    uint32_t ulReturn;
    // Enter the critical section, the critical section can be nested
    ulReturn = taskENTER_CRITICAL_FROM_ISR();

    if (ETH_GetDMAITStatus(ETH_DMA_IT_R)) {                         /* Frame received */

        ETH_RxCpltCallback();

        ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
    }
    else if (ETH_GetDMAITStatus(ETH_DMA_IT_T)) {                    /* Frame transmitted */
        /* Transfer complete callback */
        // callback

        /* Clear the Eth DMA Tx IT pending bits */
        ETH_DMAClearITPendingBit(ETH_DMA_IT_T);
    }

    /* Clear the interrupt flags */
    ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);

    /* ETH DMA Error */
    if(ETH_GetDMAITStatus(ETH_DMA_FLAG_AIS)) {
        /* Ethernet Error callback */
        // callback

        /* Clear the interrupt flags */
        ETH_DMAClearITPendingBit(ETH_DMA_FLAG_AIS);
    }

    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   AppTaskCreate
/// @note   Contains all task creation functions
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
static void AppTaskCreate(void)
{	
    BaseType_t xReturn = pdPASS;// Define a creation information return value, the default is pdPASS
	
    InitInnerPeripheral();
	  
	  InitAGESysParameter(); 
	
	  InitAGESysContral();
	
    TCPIP_Init();

    tcpecho_init();
		
    // Create Test1_Task task
/******************delete by william 2022*********************************	
	
//    xReturn = xTaskCreate((TaskFunction_t )Program_Task,  // Task entry function
//                          (const char*    )"Program_Task", // Task name
//                          (uint16_t       )512,    // Task stack size
//                          (void*          )NULL,   // Task entry function parameters
//                          (UBaseType_t    )1,      // The priority of the task
//                          (TaskHandle_t*  )&Program_Task_Handle); // Task control block pointer
//    if(pdPASS == xReturn)
**************************************************/
       // printf("Create Program_Task sucess...\r\n");


   /* xReturn = xTaskCreate((TaskFunction_t )Test2_Task,   // Task entry function
                          (const char*    )"Test2_Task", // Task name
                          (uint16_t       )512,   // Task stack size
                          (void*          )NULL, // Task entry function parameters
                          (UBaseType_t    )2,  // The priority of the task
                          (TaskHandle_t*  )&Test2_Task_Handle); // Task control block pointer
    if(pdPASS == xReturn)
     //   printf("Create Test2_Task sucess...\n\n");*/

    vTaskDelete(AppTaskCreate_Handle); //Delete AppTask Create task

    taskEXIT_CRITICAL();            //Exit the critical section
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   Test1_Task
/// @note   Test1_Task task body
/// @param
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
static void Program_Task(void* parameter)
{
    while (1) 
		{		
			 LED1_TOGGLE();
			
       LimitCompareOK(); 
			
			 CtrlProgramStart();
			 SampleChannelData();
			 CaliChannelTime();

			 MathChannelData();
			 AGEAbnormalHandle();		
			 
		   SaveSystemData();
       vTaskDelay(300); // Delay 1000 ticks
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   Test2_Task
/// @note    Test2_Task task body
/// @param
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
/*static void Test2_Task(void* parameter)
{
    while (1) {
        //LED2_TOGGLE;
//    PRINT_DEBUG("LED2_TOGGLE\n");
			 
        vTaskDelay(100); //delay 2000 ticks
    }
}*/

uint32_t FlagAddr[3]={Restart_Flag,Updata_Flag,StartCopy_Flag};
int main(void)
{
	uint16_t ucFlagVal[3]={0};
	SystemTick_Count = 0;
	
	delay_100us(1000);
	
    
	//获取标志位
	for(uint8_t i=0;i<3;i++)
	{
		ucFlagVal[i]=(*(uint32_t*)FlagAddr[i]);
	}
	
	
	if((ucFlagVal[0] & 0x01) == UPDATE )
	{
		//将备份区擦除,大小180K
		FLASH_Unlock();
		for(uint8_t i=0;i<180;i++)
		{
			FLASH_ErasePage(Cache_FLASHAddr+i*1024);
		}
		FLASH_Lock();
		//需要执行升级操作
    BaseType_t xReturn = pdPASS;// Define a creation information return value, the default is pdPASS

    xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,   // Task entry function
                          (const char*    )"AppTaskCreate", // Task name
                          (uint16_t       )512,   // Task stack size
                          (void*          )NULL, // Task entry function parameters
                          (UBaseType_t    )1,  // The priority of the task
                          (TaskHandle_t*  )&AppTaskCreate_Handle); // Task control block pointer
    // Start task scheduling
    if(pdPASS == xReturn)
        vTaskStartScheduler();
    else
        return -1;

    while(1);    // Normally will not execute here		
	}
	else
	{
//		uint32_t desAddress = 0;
//		if((ucFlagVal[1] & 0xffff) || ((ucFlagVal[1] & 0xffff)) == 0x41)
//		{
//			//如果活动分区标志位为FF或者为字符‘A’，跳转至APP1
//			desAddress = APP1_FLASHAddr;
//		}
//		else if(((ucFlagVal[1] & 0xffff)) == 0x42)
//		{
//			desAddress = APP2_FLASHAddr;
//		}
		
		iap_load_app(APP_FLASHAddr);
	}


}



/// @}

/// @}

/// @}

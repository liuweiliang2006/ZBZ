////////////////////////////////////////////////////////////////////////////////
/// @file     ethernetif.h
/// @author   AE TEAM
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE ethernetif.h EXAMPLES.
/// ////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT  MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

#ifndef __ETHERNETIF_H
#define __ETHERNETIF_H

#include "lwip/err.h"
#include "lwip/netif.h"

#define NETIF_MTU                 ( 1500 )


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
#ifdef _ETHERNETIF_C_
#define GLOBAL

#else
#define GLOBAL extern
#endif

#undef GLOBAL

#define NETIF_MTU                                     ( 1500 )

#define NETIF_IN_TASK_STACK_SIZE            ( 1024 )
#define NETIF_IN_TASK_PRIORITY            ( 3 )

#define NETIF_OUT_TASK_STACK_SIZE           ( 1024 )
#define NETIF_OUT_TASK_PRIORITY           ( 3 )


err_t ethernetif_init(struct netif* netif);

void ethernetif_input(void* pParams);
void ethernetif_output( void* argument );
void ethernetif_update_config(struct netif* netif);
void ethernetif_notify_conn_changed(struct netif* netif);

u32_t sys_jiffies(void);
u32_t sys_now(void);

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @file     eth_bsp_config.h
/// @author   AE TEAM
/// @brief    CONTAINS ALL FUNCTIONS PROTOTYPES FOR THE eth_bsp_config.h EXAMPLES.
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
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////
// Define to prevent recursive inclusion
#ifndef __ETH_BSP_CONFIG_H
#define __ETH_BSP_CONFIG_H

// Files includes
#include "types.h"
#include "mm32_device.h"
#include "hal_conf.h"

#define LAN8720_PHY_ADDRESS       ((uint16_t) 0x00)
// Ethernet Flags for EthStatus variable
#define ETH_INIT_FLAG           0x01                                            // Ethernet Init Flag
#define ETH_LINK_FLAG           0x10                                            // Ethernet Link Flag
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
#ifdef _ETH_BSP_CONFIG_C_
#define GLOBAL

#else
#define GLOBAL extern
#endif
/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Functions
/// @{

#if defined   (__CC_ARM)
__align(4)
GLOBAL ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RX_BUF_NUM];// Ethernet Rx MA Descriptor
__align(4)
GLOBAL ETH_DMADESCTypeDef  DMATxDscrTab[ETH_TX_BUF_NUM];// Ethernet Tx DMA Descriptor
__align(4)
GLOBAL uint8_t Rx_Buff[ETH_RX_BUF_NUM][ETH_RX_BUF_SIZE]; // Ethernet Receive Buffer
__align(4)
GLOBAL uint8_t Tx_Buff[ETH_TX_BUF_NUM][ETH_TX_BUF_SIZE]; // Ethernet Transmit Buffer

#elif defined ( __ICCARM__ )
#pragma data_alignment=4

#pragma location=0x2000C000
GLOBAL ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RX_BUF_NUM];
#pragma location=0x2000C080
GLOBAL ETH_DMADESCTypeDef  DMATxDscrTab[ETH_TX_BUF_NUM];
#pragma location=0x2000C100
GLOBAL uint8_t Rx_Buff[ETH_RX_BUF_NUM][ETH_RX_BUF_SIZE];
#pragma location=0x2000D8D0
GLOBAL uint8_t Tx_Buff[ETH_TX_BUF_NUM][ETH_TX_BUF_SIZE];

#elif defined (__GNUC__) // GNU Compiler 
GLOBAL ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB] __attribute__ ((aligned (4)));
GLOBAL ETH_DMADESCTypeDef  DMATxDscrTab[ETH_TXBUFNB] __attribute__ ((aligned (4)));
GLOBAL uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __attribute__ ((aligned (4)));
GLOBAL uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __attribute__ ((aligned (4)));

#elif defined  (__TASKING__)
__align(4)
GLOBAL ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB];
__align(4)
GLOBAL ETH_DMADESCTypeDef  DMATxDscrTab[ETH_TXBUFNB];
__align(4)
GLOBAL uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE];
__align(4)
GLOBAL uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE];

#endif
GLOBAL __IO uint32_t  EthStatus;

#undef GLOBAL

void ETH_BSP_Config(void);
void ETH_GPIO_Config(void);

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////

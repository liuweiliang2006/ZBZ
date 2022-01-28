////////////////////////////////////////////////////////////////////////////////
/// @file     eth_bsp_config.c
/// @author   AE TEAM
/// @brief    THIS FILE PROVIDES ALL THE eth_bsp_config.c EXAMPLE.
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

#define _ETH_BSP_CONFIG_C_

//#include "HAL_rcc.h"
//#include "HAL_gpio.h"
//#include "HAL_nvic.h"

//#include "HAL_eth.h"
#include "eth_bsp_config.h"
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DELAY
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DELAY_Exported_Functions
/// @{


void ETH_NVIC_Config(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* Enable the Ethernet global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = ETHERNET_MAC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void ETH_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // Enable GPIOs clocks
    RCC_AHBPeriphClockCmd( RCC_AHBENR_GPIOA | RCC_AHBENR_GPIOB |
                           RCC_AHBENR_GPIOC | RCC_AHBENR_GPIOD |
                           RCC_AHBENR_GPIOE, ENABLE);  // RCC_APB2ENR_SYSCFG
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFG, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                                   //C_NRST
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIOB->BSRR = 1 << 4;

    // Configure PA0, PA1, PA2, PA3 and PA7
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 |GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_11);                       //ETH_RX_CLK/REGOFF     PA1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_11);                       //ETH_MDIO              PA2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_11);                       //ETH_RX_DV             PA7--PD8

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_11);                      // ETH_TX_EN     PB11
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_11);                      // ETH_TXD0     PB12
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_11);                      // ETH_TXD1        PB13

    // Configure PC1, PC2, PC3, PC4 and PC5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_11);                       // ETH_MDC       PC1
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_11);                       // ETH_RXD0      PC4
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_11);                       // ETH_RXD1      PC5

    SYSCFG->CFGR2 |= 0X01 << 20;
}

static void ETH_MAC_DMA_Config(void)
{
    ETH_InitTypeDef ETH_InitStructure;

    // Enable ETHERNET clock
    RCC_AHBPeriphClockCmd(RCC_AHBENR_ETHMAC, ENABLE);

    // Reset ETHERNET on AHB Bus
    ETH_DeInit();//

    // Software reset
    ETH_SoftwareReset();

    // Wait for software reset
    while (ETH_GetSoftwareResetStatus());

    ETH_StructInit(&ETH_InitStructure);

    //  Fill ETH_InitStructure parametrs
    // ------------------------   MAC   ----------------------------------------
    ETH_InitStructure.ETH_AutoNegotiation =   ETH_AutoNegotiation_Enable;//ETH_AutoNegotiation_Disable;//
    ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
    ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Enable;
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
    ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Disable;

    // ------------------------   DMA   ----------------------------------------
    //  When we use the Checksum offload feature, we need to enable the Store and Forward mode:
    // the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum,
    // if the checksum is OK the DMA can handle the frame otherwise the frame is dropped

    ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
    ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
    ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

    ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
    ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
    ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
    ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Disable;
    ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
    ETH_InitStructure.ETH_RxDMABurstLength = 0;
    ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
    ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_1_1;

    EthStatus = ETH_Init(&ETH_InitStructure, LAN8720_PHY_ADDRESS);
}
void ETH_BSP_Config( void)
{
    EthStatus = 0;

    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_Config(RCC_GetHCLKFreq() / 1000); // 10ms

    ETH_GPIO_Config();

//    ETH_NVIC_Config();

    ETH_MAC_DMA_Config();

    while (!(ETH_ReadPHYRegister(LAN8720_PHY_ADDRESS, PHY_BSR) & PHY_Linked_Status));
    EthStatus |= ETH_LINK_FLAG;

}
/// @}

/// @}

/// @}

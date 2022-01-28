////////////////////////////////////////////////////////////////////////////////
/// @file    fsmc_sram.c
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
#define _FSMC_SRAM_C_

// Files includes

#include "stdio.h"
#include "hal_conf.h"
#include "delay.h"
#include "sram.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup FSMC_SRAM
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup FSMC_SRAM_Exported_Constants
/// @{



// Private function prototypes
void GPIO_Configuration(void);
////////////////////////////////////////////////////////////////////////////////
/// @brief  Io clock configuration
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_ConfigInit(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA | RCC_AHBENR_GPIOB | RCC_AHBENR_GPIOC | RCC_AHBENR_GPIOD | RCC_AHBENR_GPIOE, ENABLE);
    RCC->AHB3ENR |= 0x01;       //enable FSMCEN
    RCC->APB2ENR |= 0x01 << 14; //open sys_cfg clk
}


void SRAM_PIN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd( RCC_AHBENR_GPIOD | RCC_AHBENR_GPIOE , ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_12); //DA2
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_12); //DA3
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_12); //NOE
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_12); //NWE
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_12); //NE1
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_12); //A16
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_12); //A17
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_12); //A18
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_12); //DA0
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_12); //DA1
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_12); //A19
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_12); //A20
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_12); //DA4
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_12); //DA5
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_12); //DA6
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_12); //DA7

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5
		                           |GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13
															  |GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7|GPIO_Pin_8
		                           |GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

}

void sram_demo(void);
////////////////////////////////////////////////////////////////////////////////
/// @brief   write and read sram
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void sram_writeread(void)
{
//    RCC_ConfigInit();


    SRAM_PIN_GPIO_Config();
    FSMC_SRAM_Init();
    DELAY_Ms(50);                   // delay 50 ms
    //sram_demo();


    /*while(1) {
    }*/
}

/// @}

/// @}

/// @}

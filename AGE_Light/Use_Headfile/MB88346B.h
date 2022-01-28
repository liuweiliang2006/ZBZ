#ifndef _MB88346B_H
#define _MB88346B_H

#include "main.h"
#include "MyInclude.h" 
/**************************************************************************
                                 INCLUDES
***************************************************************************/


#define QDB_MB88346BSDI_PORT       GPIOC
#define QDB_MB88346BSDI_PIN        GPIO_Pin_2

#define QDB_MB88346BCLK_PORT       GPIOE
#define QDB_MB88346BCLK_PIN        GPIO_Pin_0


#define MB88346BSDI_H    GPIO_SetBits(QDB_MB88346BSDI_PORT,QDB_MB88346BSDI_PIN)
#define MB88346BSDI_L    GPIO_ResetBits(QDB_MB88346BSDI_PORT,QDB_MB88346BSDI_PIN)

#define MB88346BCLK_H    GPIO_SetBits(QDB_MB88346BCLK_PORT,QDB_MB88346BCLK_PIN)
#define MB88346BCLK_L    GPIO_ResetBits(QDB_MB88346BCLK_PORT,QDB_MB88346BCLK_PIN)
/**************************************************************************/
extern void MB88346B_Gpio_Init(void);
extern void SetMB88346BSingle(uint16_t data,uint16_t ChanNo);
extern void SetMB88346BMainDA(uint16_t data,uint16_t ChanNo);

#endif

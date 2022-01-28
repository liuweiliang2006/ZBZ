#ifndef _LED_CTRLDRIVE_H
#define _LED_CTRLDRIVE_H

#include "main.h"
#include "MyInclude.h" 

/**************************************************************************
                                 INCLUDES
***************************************************************************/
#define QDB_CD4051_DB0_PORT         GPIOE
#define QDB_CD4051_DB0_PIN          GPIO_Pin_2
#define QDB_CD4051_DB1_PORT         GPIOA
#define QDB_CD4051_DB1_PIN          GPIO_Pin_6
#define QDB_CD4051_DB2_PORT         GPIOA
#define QDB_CD4051_DB2_PIN          GPIO_Pin_5
#define QDB_CD4051_WCS1_PORT        GPIOA
#define QDB_CD4051_WCS1_PIN         GPIO_Pin_4
#define QDB_CD4051_WCS2_PORT        GPIOC
#define QDB_CD4051_WCS2_PIN         GPIO_Pin_13
#define QDB_CD4051_EN_PORT          GPIOB
#define QDB_CD4051_EN_PIN           GPIO_Pin_9

//64路电流采样的CD4051模拟开关控制
#define CD4051_DB0_H      GPIO_SetBits(QDB_CD4051_DB0_PORT, QDB_CD4051_DB0_PIN)
#define CD4051_DB1_H      GPIO_SetBits(QDB_CD4051_DB1_PORT, QDB_CD4051_DB1_PIN)
#define CD4051_DB2_H      GPIO_SetBits(QDB_CD4051_DB2_PORT, QDB_CD4051_DB2_PIN)
#define CD4051_WCS1_H     GPIO_SetBits(QDB_CD4051_WCS1_PORT, QDB_CD4051_WCS1_PIN)
#define CD4051_WCS2_H     GPIO_SetBits(QDB_CD4051_WCS2_PORT, QDB_CD4051_WCS2_PIN)
#define CD4051_EN_H       GPIO_SetBits(QDB_CD4051_EN_PORT, QDB_CD4051_EN_PIN)


#define CD4051_DB0_L     GPIO_ResetBits(QDB_CD4051_DB0_PORT, QDB_CD4051_DB0_PIN)
#define CD4051_DB1_L     GPIO_ResetBits(QDB_CD4051_DB1_PORT, QDB_CD4051_DB1_PIN)
#define CD4051_DB2_L     GPIO_ResetBits(QDB_CD4051_DB2_PORT, QDB_CD4051_DB2_PIN)
#define CD4051_WCS1_L    GPIO_ResetBits(QDB_CD4051_WCS1_PORT, QDB_CD4051_WCS1_PIN)
#define CD4051_WCS2_L    GPIO_ResetBits(QDB_CD4051_WCS2_PORT, QDB_CD4051_WCS2_PIN)
#define CD4051_EN_L      GPIO_ResetBits(QDB_CD4051_EN_PORT, QDB_CD4051_EN_PIN)

#define  SELECT_4051NO1       1
#define  SELECT_4051NO2       2
#define  SELECT_4051NO3       3
#define  SELECT_4051NO        0




#define QDB_EN33_PORT         GPIOB
#define QDB_EN33_PIN          GPIO_Pin_4
#define QDB_EN34_PORT         GPIOB
#define QDB_EN34_PIN          GPIO_Pin_3
#define QDB_EN35_PORT         GPIOD
#define QDB_EN35_PIN          GPIO_Pin_6
#define QDB_EN36_PORT         GPIOD
#define QDB_EN36_PIN          GPIO_Pin_3
#define QDB_EN37_PORT         GPIOD
#define QDB_EN37_PIN          GPIO_Pin_2
#define QDB_EN38_PORT         GPIOA
#define QDB_EN38_PIN          GPIO_Pin_15
#define QDB_EN39_PORT         GPIOH
#define QDB_EN39_PIN          GPIO_Pin_2
#define QDB_EN40_PORT         GPIOA
#define QDB_EN40_PIN          GPIO_Pin_12
#define QDB_EN41_PORT         GPIOA
#define QDB_EN41_PIN          GPIO_Pin_11
#define QDB_EN42_PORT         GPIOA
#define QDB_EN42_PIN          GPIO_Pin_10
#define QDB_EN43_PORT         GPIOA
#define QDB_EN43_PIN          GPIO_Pin_9 
#define QDB_EN44_PORT         GPIOA
#define QDB_EN44_PIN          GPIO_Pin_8
#define QDB_EN45_PORT         GPIOC
#define QDB_EN45_PIN          GPIO_Pin_9
#define QDB_EN46_PORT         GPIOC
#define QDB_EN46_PIN          GPIO_Pin_8
#define QDB_EN47_PORT         GPIOC
#define QDB_EN47_PIN          GPIO_Pin_7
#define QDB_EN48_PORT         GPIOC
#define QDB_EN48_PIN          GPIO_Pin_6
#define QDB_EN49_PORT         GPIOD
#define QDB_EN49_PIN          GPIO_Pin_10
#define QDB_EN50_PORT         GPIOD
#define QDB_EN50_PIN          GPIO_Pin_9
#define QDB_EN51_PORT         GPIOD
#define QDB_EN51_PIN          GPIO_Pin_8
#define QDB_EN52_PORT         GPIOB
#define QDB_EN52_PIN          GPIO_Pin_15
#define QDB_EN53_PORT         GPIOB
#define QDB_EN53_PIN          GPIO_Pin_14
#define QDB_EN54_PORT         GPIOB
#define QDB_EN54_PIN          GPIO_Pin_11
#define QDB_EN55_PORT         GPIOB
#define QDB_EN55_PIN          GPIO_Pin_1
#define QDB_EN56_PORT         GPIOB
#define QDB_EN56_PIN          GPIO_Pin_2
#define QDB_EN57_PORT         GPIOE
#define QDB_EN57_PIN          GPIO_Pin_6
#define QDB_EN58_PORT         GPIOB
#define QDB_EN58_PIN          GPIO_Pin_0
#define QDB_EN59_PORT         GPIOC
#define QDB_EN59_PIN          GPIO_Pin_12
#define QDB_EN60_PORT         GPIOE
#define QDB_EN60_PIN          GPIO_Pin_5
#define QDB_EN61_PORT         GPIOC
#define QDB_EN61_PIN          GPIO_Pin_11
#define QDB_EN62_PORT         GPIOC
#define QDB_EN62_PIN          GPIO_Pin_10
#define QDB_EN63_PORT         GPIOB
#define QDB_EN63_PIN          GPIO_Pin_6
#define QDB_EN64_PORT         GPIOB
#define QDB_EN64_PIN          GPIO_Pin_5

#define QDB_ENA_PORT         GPIOC
#define QDB_ENA_PIN          GPIO_Pin_3
#define QDB_ENB_PORT         GPIOE
#define QDB_ENB_PIN          GPIO_Pin_1

//64路电流采样的CD4051模拟开关控制
#define QDB_EN33_H     GPIO_SetBits(QDB_EN33_PORT, QDB_EN33_PIN)
#define QDB_EN34_H     GPIO_SetBits(QDB_EN34_PORT, QDB_EN34_PIN)
#define QDB_EN35_H     GPIO_SetBits(QDB_EN35_PORT, QDB_EN35_PIN)
#define QDB_EN36_H     GPIO_SetBits(QDB_EN36_PORT, QDB_EN36_PIN)
#define QDB_EN37_H     GPIO_SetBits(QDB_EN37_PORT, QDB_EN37_PIN)
#define QDB_EN38_H     GPIO_SetBits(QDB_EN38_PORT, QDB_EN38_PIN)
#define QDB_EN39_H     GPIO_SetBits(QDB_EN39_PORT, QDB_EN39_PIN)
#define QDB_EN40_H     GPIO_SetBits(QDB_EN40_PORT, QDB_EN40_PIN)
#define QDB_EN41_H     GPIO_SetBits(QDB_EN41_PORT, QDB_EN41_PIN)
#define QDB_EN42_H     GPIO_SetBits(QDB_EN42_PORT, QDB_EN42_PIN)
#define QDB_EN43_H     GPIO_SetBits(QDB_EN43_PORT, QDB_EN43_PIN)
#define QDB_EN44_H     GPIO_SetBits(QDB_EN44_PORT, QDB_EN44_PIN)
#define QDB_EN45_H     GPIO_SetBits(QDB_EN45_PORT, QDB_EN45_PIN)
#define QDB_EN46_H     GPIO_SetBits(QDB_EN46_PORT, QDB_EN46_PIN)
#define QDB_EN47_H     GPIO_SetBits(QDB_EN47_PORT, QDB_EN47_PIN)
#define QDB_EN48_H     GPIO_SetBits(QDB_EN48_PORT, QDB_EN48_PIN)
#define QDB_EN49_H     GPIO_SetBits(QDB_EN49_PORT, QDB_EN49_PIN)
#define QDB_EN50_H     GPIO_SetBits(QDB_EN50_PORT, QDB_EN50_PIN)
#define QDB_EN51_H     GPIO_SetBits(QDB_EN51_PORT, QDB_EN51_PIN)
#define QDB_EN52_H     GPIO_SetBits(QDB_EN52_PORT, QDB_EN52_PIN)
#define QDB_EN53_H     GPIO_SetBits(QDB_EN53_PORT, QDB_EN53_PIN)
#define QDB_EN54_H     GPIO_SetBits(QDB_EN54_PORT, QDB_EN54_PIN)
#define QDB_EN55_H     GPIO_SetBits(QDB_EN55_PORT, QDB_EN55_PIN)
#define QDB_EN56_H     GPIO_SetBits(QDB_EN56_PORT, QDB_EN56_PIN)
#define QDB_EN57_H     GPIO_SetBits(QDB_EN57_PORT, QDB_EN57_PIN)
#define QDB_EN58_H     GPIO_SetBits(QDB_EN58_PORT, QDB_EN58_PIN)
#define QDB_EN59_H     GPIO_SetBits(QDB_EN59_PORT, QDB_EN59_PIN)
#define QDB_EN60_H     GPIO_SetBits(QDB_EN60_PORT, QDB_EN60_PIN)
#define QDB_EN61_H     GPIO_SetBits(QDB_EN61_PORT, QDB_EN61_PIN)
#define QDB_EN62_H     GPIO_SetBits(QDB_EN62_PORT, QDB_EN62_PIN)
#define QDB_EN63_H     GPIO_SetBits(QDB_EN63_PORT, QDB_EN63_PIN)
#define QDB_EN64_H     GPIO_SetBits(QDB_EN64_PORT, QDB_EN64_PIN)
#define QDB_ENA_H      GPIO_SetBits(QDB_ENA_PORT, QDB_ENA_PIN)
#define QDB_ENB_H      GPIO_SetBits(QDB_ENB_PORT, QDB_ENB_PIN)

#define QDB_EN33_L     GPIO_ResetBits(QDB_EN33_PORT, QDB_EN33_PIN)
#define QDB_EN34_L     GPIO_ResetBits(QDB_EN34_PORT, QDB_EN34_PIN)
#define QDB_EN35_L     GPIO_ResetBits(QDB_EN35_PORT, QDB_EN35_PIN)
#define QDB_EN36_L     GPIO_ResetBits(QDB_EN36_PORT, QDB_EN36_PIN)
#define QDB_EN37_L     GPIO_ResetBits(QDB_EN37_PORT, QDB_EN37_PIN)
#define QDB_EN38_L     GPIO_ResetBits(QDB_EN38_PORT, QDB_EN38_PIN)
#define QDB_EN39_L     GPIO_ResetBits(QDB_EN39_PORT, QDB_EN39_PIN)
#define QDB_EN40_L     GPIO_ResetBits(QDB_EN40_PORT, QDB_EN40_PIN)
#define QDB_EN41_L     GPIO_ResetBits(QDB_EN41_PORT, QDB_EN41_PIN)
#define QDB_EN42_L     GPIO_ResetBits(QDB_EN42_PORT, QDB_EN42_PIN)
#define QDB_EN43_L     GPIO_ResetBits(QDB_EN43_PORT, QDB_EN43_PIN)
#define QDB_EN44_L     GPIO_ResetBits(QDB_EN44_PORT, QDB_EN44_PIN)
#define QDB_EN45_L     GPIO_ResetBits(QDB_EN45_PORT, QDB_EN45_PIN)
#define QDB_EN46_L     GPIO_ResetBits(QDB_EN46_PORT, QDB_EN46_PIN)
#define QDB_EN47_L     GPIO_ResetBits(QDB_EN47_PORT, QDB_EN47_PIN)
#define QDB_EN48_L     GPIO_ResetBits(QDB_EN48_PORT, QDB_EN48_PIN)
#define QDB_EN49_L     GPIO_ResetBits(QDB_EN49_PORT, QDB_EN49_PIN)
#define QDB_EN50_L     GPIO_ResetBits(QDB_EN50_PORT, QDB_EN50_PIN)
#define QDB_EN51_L     GPIO_ResetBits(QDB_EN51_PORT, QDB_EN51_PIN)
#define QDB_EN52_L     GPIO_ResetBits(QDB_EN52_PORT, QDB_EN52_PIN)
#define QDB_EN53_L     GPIO_ResetBits(QDB_EN53_PORT, QDB_EN53_PIN)
#define QDB_EN54_L     GPIO_ResetBits(QDB_EN54_PORT, QDB_EN54_PIN)
#define QDB_EN55_L     GPIO_ResetBits(QDB_EN55_PORT, QDB_EN55_PIN)
#define QDB_EN56_L     GPIO_ResetBits(QDB_EN56_PORT, QDB_EN56_PIN)
#define QDB_EN57_L     GPIO_ResetBits(QDB_EN57_PORT, QDB_EN57_PIN)
#define QDB_EN58_L     GPIO_ResetBits(QDB_EN58_PORT, QDB_EN58_PIN)
#define QDB_EN59_L     GPIO_ResetBits(QDB_EN59_PORT, QDB_EN59_PIN)
#define QDB_EN60_L     GPIO_ResetBits(QDB_EN60_PORT, QDB_EN60_PIN)
#define QDB_EN61_L     GPIO_ResetBits(QDB_EN61_PORT, QDB_EN61_PIN)
#define QDB_EN62_L     GPIO_ResetBits(QDB_EN62_PORT, QDB_EN62_PIN)
#define QDB_EN63_L     GPIO_ResetBits(QDB_EN63_PORT, QDB_EN63_PIN)
#define QDB_EN64_L     GPIO_ResetBits(QDB_EN64_PORT, QDB_EN64_PIN)
#define QDB_ENA_L      GPIO_ResetBits(QDB_ENA_PORT, QDB_ENA_PIN)
#define QDB_ENB_L      GPIO_ResetBits(QDB_ENB_PORT, QDB_ENB_PIN)


extern void AGE_CtrlDrive_GpioInit(void);
extern void CtrlEN33_64_GpioInit(void);
extern void Ctrl_4051ADEN(uint16_t ChanNo);
extern void Ctrl_4051INCH(uint16_t ChanNo);
extern void Ctrl_ON_Switch(uint16_t ChanNo,uint16_t flag);
extern void Ctrl_EN33_64(uint16_t ChanNo,uint16_t flag);
extern void DA_LoadData(uint8_t ChanNo,uint8_t flag);

#endif

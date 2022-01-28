#ifndef _SYSTEM_CONST_H
#define _SYSTEM_CONST_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
                                 INCLUDES
***************************************************************************/
#define MAX_AGE_CHAN               64
#define MAX_VOL_CHAN               64
#define MAX_MAIN_DA                8
	
#define MAX_CURRKB_GRADE	         2	

//按两个字节为一个长度计算

#define BIT0                       0x01
#define BIT1                       0x02
#define BIT2                       0x04
#define BIT3                       0x08
#define BIT4                       0x10
#define BIT5                       0x20
#define BIT6                       0x40
#define BIT7                       0x80


#define MUL_CURR                   100
#define MUL_VOL                    10	
	
#define MAX_SCI_REC                100

#define MEASURE_MODE               0
#define CALI_MODE                  1
#define NOMAL_MODE                 2

#define DATA_VAL                   0x55AA
#define DATA_NOVAL                 0x0000

#define MAX_DEVICE_CURRENT         (uint16_t)(80*MUL_CURR)

#define SAMPLECNT                  10

#define CurrGrade1                 (uint16_t)(15*MUL_CURR)
#define CurrGrade2                 (uint16_t)(82*MUL_CURR)

#ifndef Online
#define Online                     2
#endif

/* void type */
#ifndef NULL
#define NULL                      (void*)0
#endif

#define MmtRUNSta	                1 // SCH RUN state
#define MmtSTOPSta                0

//RUN STATE
#define WAIT_STATE                0x00
#define READY_STATE               0x01
#define RUNNING_STATE             0x02
#define PAUSE_STATE               0x03
#define TESTING_STATE             0x04
#define TESTFINISH_STATE          0x05
#define COMPLATE_STATE            0x06
#define WARNSTOP_STATE            0x07
#define STOP_STATE                0x0B

//SCH_MODE
#define WAIT_CTRL                 0x00
#define CURR_CTRL                 0x01
#define POWER_CTRL                0x02


//PROTECT STATE
#define NOMAL_AL                  0x00
#define HIGHCUR_AL                0x05
#define LOWCUR_AL                 0x06
#define HIGHTEMP_AL               0x03
#define LOWTEMP_AL                0x04

#define HIGHVol_AL                0x07
#define LOWVol_AL                 0x08
#define SETDATA_AL                0x07

//POWER PROTECT STATE
#define POWERNOMAL_AL             0x00
#define POWERFORM_AL              0x01
#define POWEROUT_AL               0x02

//ABNOMAL CNT
#define MAX_AL_CNT                3
#define LEGAL_HEAD_LEN            1024


//DA系数
#define CONST_REFDAVAL             0.0153 //(255*30)/(2500*MUL_CURR*2)*1
#define CONSTHARF_DAVAL           1.089324618736383//(250000/(30*255*30*1))

#define CONST_SLAVEDAVAL          0.918//(30*30*255*1)/2500*MUL_CURR

#ifdef __cplusplus
}
#endif 



#endif

#ifndef _ADS8588S_H
#define _ADS8588S_H

#include "main.h"
#include "MyInclude.h" 

/**************************************************************************
                                 INCLUDES
***************************************************************************/

#define QDB_ADS8588SRESET_PORT       GpioPortA
#define QDB_ADS8588SRESET_PIN        GpioPin1

#define QDB_ADS8588SCLK_PORT       GpioPortA
#define QDB_ADS8588SCLK_PIN        GpioPin2

#define QDB_ADS8588SCS_PORT       GpioPortA
#define QDB_ADS8588SCS_PIN        GpioPin3

#define QDB_ADS8588SCONV_PORT       GpioPortA
#define QDB_ADS8588SCONV_PIN        GpioPin0

#define QDB_ADS8588SDOA_PORT       GpioPortA
#define QDB_ADS8588SDOA_PIN        GpioPin7

#define QDB_ADS8588SDOB_PORT       GpioPortA
#define QDB_ADS8588SDOB_PIN        GpioPin6

    

#define ADS8588SRESET_HIGH    1
#define ADS8588SRESET_LOW     0

#define ADS8588SCLK_HIGH      1
#define ADS8588SCLK_LOW       0

#define ADS8588SCS_HIGH       1
#define ADS8588SCS_LOW        0

#define ADS8588SCONV_HIGH     1
#define ADS8588SCONV_LOW      0

#define ADS8588SDOA            1
#define ADS8588SDOB            0

/**************************************************************************/

extern void ADS8588S_GPIO_OUTInit(void);
extern void ADS8588S_GPIO_INInit(void);
extern void Init_ADS8588S(void);
extern void ADS8588SReset(void);
extern void ADS8588SReadOneSample(uint16_t *iRdData);

#endif

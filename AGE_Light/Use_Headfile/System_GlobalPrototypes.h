#ifndef _SYSTEM_GLOBALPROTOTYPES_H
#define _SYSTEM_GLOBALPROTOTYPES_H


#include "main.h"
#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
                                 INCLUDES
*************   **************************************************************/


/*********************************************************
初始化相关控制
*********************************************************/
extern void InitAGESysParameter(void);
extern void InitAGESysContral(void);
extern void InitInnerPeripheral(void);

/*********************************************************
 限制比较函数
*********************************************************/
extern void LimitCompareOK(void);
extern void LimitCompareProcess(void);
extern uint16_t LimitCompare(uint16_t mode);

/*********************************************************
AGE异常处理函数
*********************************************************/
extern void AGEAbnormalHandle(void);
extern void POWERVolAbnormalHandle(void);
extern void VoltageAbnormalHandle(void);
extern void CurrentINAbnormalHandle(void);
extern void CurrentOUTAbnormalHandle(void);
/*********************************************************
 flow contral proto
*********************************************************/
extern void CtrlProgramStart(void);
extern void ProgParameterCome(void);
/*********************************************************
 数据计算，保存函数
*********************************************************/
extern void MathChannelData(void);
extern void SaveSystemData(void);
extern uint32_t Crs(uint16_t* buf,uint16_t len);
/*********************************************************
 Ethenet communication
*********************************************************/
extern void PCCtrlAGERun(void);
extern void PCCtrlAGESTOP(void);
extern void PCCtrlAGEPause(void);
extern void PCCtrlAGERecover(void);

extern void PC_DATAHandler(char *Sptr,uint16_t len);

/***************************************************************
 DS1390函数声明
****************************************************************/
extern void CaliChannelTime(void);
extern void CalcDeltOfTime(void);
extern void TimeCopy(uint16_t flag);
/***************************************************************
    采样函数声明
****************************************************************/
extern void SampleChannelData(void);
extern void SampleSingleChanData(uint16_t ChanNo);
extern void SampAGEoubleChanData(uint16_t ChanNo);
extern void SampAGEVolChanData(uint16_t ChanNo);

extern uint16_t SortAndFilter16(uint16_t *dpoint, uint16_t num);
extern uint32_t SortAndFilter(uint32_t *dpoint, uint16_t num);
extern uint32_t abs32(uint32_t cmpa,uint32_t cmpb);
extern uint16_t CurrFilter(uint16_t Curr,uint16_t ChanNo);
extern void CaliDCpowerVol(void);
extern void CaliChannelVolPower(uint16_t ChanNo);
extern void CaliChannelCurrIN(uint16_t ChanNo);
extern void CaliChannelCurrOUT(uint16_t ChanNo);
extern void CaliChannelVol(uint16_t ChanNo);
extern void CaliChannelALL_Data(void);

/***************************************************************
    电池充放电模式控制函数
****************************************************************/
extern void CtrlAGE_STOP_CTRL(void);
extern void CtrlAGE_CURR_CTRL(void);
extern void CtrlAGE_Power_CTRL(void);
extern void CtrlSchState(uint16_t state);

/***************************************************************
    校准函数
****************************************************************/
extern uint16_t CurrentCalibGrade(float32_t data);
extern float ErrorCompensation_i(float32_t cur,uint16_t ChanNo);
extern uint16_t CurrCalibGrade_Fee(float32_t data);
extern float32_t FeedBackCalibration_i(uint32_t data,uint16_t ChanNo);
extern uint16_t OutCurrCalibGrade_Fee(float data);
extern float FeedBackOutCurCalibration_i(uint32_t data,uint16_t ChanNo);
/*********************************************************
 Miscellaneous
*********************************************************/
extern void ClearProgramSRAM(uint16_t *ptr,uint32_t len);
extern void MoveDataFromSRAMToSRAM(uint16_t *source,uint16_t *dest,uint32_t len);
extern void ClearVarParam(void);
extern void ClearProgParam(void);
/*********************************************************
 ETH
*********************************************************/
extern void EthernetReceiveSuccess(uint32_t ParameterLen,uint16_t* Send);
extern void EthernetReceiveFaiAGE(uint16_t Error);
extern void EthernetInterfaceHandler(uint16_t len);
extern void write_word(uint16_t*  buf,uint16_t len);
extern void ProgramUpdata(void);//2
extern void FlashUpdate(void);//3

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  

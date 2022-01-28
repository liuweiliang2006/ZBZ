#include "main.h"
#include "MyInclude.h"

extern const unsigned int ADS8688_CH_TABLE[8];
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值： 
注意： 
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void SampleChannelData()
{
    uint16_t i;	
	  
	  Ctrl_4051INCH(SELECT_4051NO1);
	  DELAY_Ms(5);
    for(i=0;i<8;i++)
    {			 			
       Ctrl_4051ADEN(i);			
			 DELAY_Ms(5);
       SampleSingleChanData(i);      
			 Ctrl_4051ADEN(8);		 
    }
		Ctrl_4051INCH(SELECT_4051NO);
		
		Ctrl_4051INCH(SELECT_4051NO2);
		DELAY_Ms(5);
    for(i=0;i<8;i++)
    {
       Ctrl_4051ADEN(i);
			 DELAY_Ms(5);
       SampAGEoubleChanData(i);       
			 Ctrl_4051ADEN(8);
			 
    }
		Ctrl_4051INCH(SELECT_4051NO);

		Ctrl_4051INCH(SELECT_4051NO3);
		DELAY_Ms(5);
		for(i=0;i<3;i++)
    {
       Ctrl_4051ADEN(i);
       SampAGEVolChanData(i);
       DELAY_Ms(5);
			 Ctrl_4051ADEN(8);
    }
    Ctrl_4051INCH(SELECT_4051NO);
}
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值： 
注意： 
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void SampleSingleChanData(uint16_t ChanNo)
{
    volatile  uint32_t   temp[8][SAMPLECNT];
    uint16_t   i,itemp;
    uint16_t  SelChan=0;

    SelChan=ChanNo;
    
    for(i=0; i<8; i++)
    {
         Read_8688(ADS8688_CH_TABLE[i]);
    }
    for(i=0;i<SAMPLECNT;i++)
    {
        for(itemp=0;itemp<8;itemp++)
        {				  
            temp[itemp][i]  = Read_8688(ADS8688_CH_TABLE[itemp]);
        }
    }
		switch(SelChan)	
		{
			case 0:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[0][i].data=temp[7][i];
								SysCtrl.Sample_temp[1][i].data=temp[6][i];
								SysCtrl.Sample_temp[2][i].data=temp[5][i];
								SysCtrl.Sample_temp[3][i].data=temp[4][i];
								SysCtrl.Sample_temp[4][i].data=temp[3][i];
								SysCtrl.Sample_temp[5][i].data=temp[2][i];
								SysCtrl.Sample_temp[6][i].data=temp[1][i];
								SysCtrl.Sample_temp[7][i].data=temp[0][i];
						}
						break;
			case 1:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[8][i].data=temp[7][i];
								SysCtrl.Sample_temp[9][i].data=temp[6][i];
								SysCtrl.Sample_temp[10][i].data=temp[5][i];
								SysCtrl.Sample_temp[11][i].data=temp[4][i];
								SysCtrl.Sample_temp[12][i].data=temp[3][i];
								SysCtrl.Sample_temp[13][i].data=temp[2][i];
								SysCtrl.Sample_temp[14][i].data=temp[1][i];
								SysCtrl.Sample_temp[15][i].data=temp[0][i];
						}break;
			case 2:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[16][i].data=temp[7][i];
								SysCtrl.Sample_temp[17][i].data=temp[6][i];
								SysCtrl.Sample_temp[18][i].data=temp[5][i];
								SysCtrl.Sample_temp[19][i].data=temp[4][i];
								SysCtrl.Sample_temp[20][i].data=temp[3][i];
								SysCtrl.Sample_temp[21][i].data=temp[2][i];
								SysCtrl.Sample_temp[22][i].data=temp[1][i];
								SysCtrl.Sample_temp[23][i].data=temp[0][i];
						}break;
			case 3:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[24][i].data=temp[7][i];
								SysCtrl.Sample_temp[25][i].data=temp[6][i];
								SysCtrl.Sample_temp[26][i].data=temp[5][i];
								SysCtrl.Sample_temp[27][i].data=temp[4][i];
								SysCtrl.Sample_temp[28][i].data=temp[3][i];
								SysCtrl.Sample_temp[29][i].data=temp[2][i];
								SysCtrl.Sample_temp[30][i].data=temp[1][i];
								SysCtrl.Sample_temp[31][i].data=temp[0][i];
						}break;
			case 4:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[32][i].data=temp[7][i];
								SysCtrl.Sample_temp[33][i].data=temp[6][i];
								SysCtrl.Sample_temp[34][i].data=temp[5][i];
								SysCtrl.Sample_temp[35][i].data=temp[4][i];
								SysCtrl.Sample_temp[36][i].data=temp[3][i];
								SysCtrl.Sample_temp[37][i].data=temp[2][i];
								SysCtrl.Sample_temp[38][i].data=temp[1][i];
								SysCtrl.Sample_temp[39][i].data=temp[0][i];
						}break;
				case 5:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[40][i].data=temp[7][i];
								SysCtrl.Sample_temp[41][i].data=temp[6][i];
								SysCtrl.Sample_temp[42][i].data=temp[5][i];
								SysCtrl.Sample_temp[43][i].data=temp[4][i];
								SysCtrl.Sample_temp[44][i].data=temp[3][i];
								SysCtrl.Sample_temp[45][i].data=temp[2][i];
								SysCtrl.Sample_temp[46][i].data=temp[1][i];
								SysCtrl.Sample_temp[47][i].data=temp[0][i];
						}break;
			case 6:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[48][i].data=temp[7][i];
								SysCtrl.Sample_temp[49][i].data=temp[6][i];
								SysCtrl.Sample_temp[50][i].data=temp[5][i];
								SysCtrl.Sample_temp[51][i].data=temp[4][i];
								SysCtrl.Sample_temp[52][i].data=temp[3][i];
								SysCtrl.Sample_temp[53][i].data=temp[2][i];
								SysCtrl.Sample_temp[54][i].data=temp[1][i];
								SysCtrl.Sample_temp[55][i].data=temp[0][i];
						}break;		
			case 7:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[56][i].data=temp[7][i];
								SysCtrl.Sample_temp[57][i].data=temp[6][i];
								SysCtrl.Sample_temp[58][i].data=temp[5][i];
								SysCtrl.Sample_temp[59][i].data=temp[4][i];
								SysCtrl.Sample_temp[60][i].data=temp[3][i];
								SysCtrl.Sample_temp[61][i].data=temp[2][i];
								SysCtrl.Sample_temp[62][i].data=temp[1][i];
								SysCtrl.Sample_temp[63][i].data=temp[0][i];
						}break;	
			default:break;										
		}
}
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值： 
注意： 
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void SampAGEoubleChanData(uint16_t ChanNo)
{
    uint32_t   temp[8][SAMPLECNT];
    uint16_t   i,itemp;
    uint16_t  SelChan=0;

    SelChan=ChanNo;

    for(i=0; i<8; i++)
    {
         Read_8688(ADS8688_CH_TABLE[i]);
    }
    for(i=0;i<SAMPLECNT;i++)
    {
        for(itemp=0;itemp<8;itemp++)
        {
            temp[itemp][i]  = Read_8688(ADS8688_CH_TABLE[itemp]);
        }
    }
    switch(SelChan)	
		{
			case 0:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[64][i].data=temp[7][i];
								SysCtrl.Sample_temp[65][i].data=temp[6][i];
								SysCtrl.Sample_temp[66][i].data=temp[5][i];
								SysCtrl.Sample_temp[67][i].data=temp[4][i];
								SysCtrl.Sample_temp[68][i].data=temp[3][i];
								SysCtrl.Sample_temp[69][i].data=temp[2][i];
								SysCtrl.Sample_temp[70][i].data=temp[1][i];
								SysCtrl.Sample_temp[71][i].data=temp[0][i];
						}
						break;
			case 1:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[72][i].data=temp[7][i];
								SysCtrl.Sample_temp[73][i].data=temp[6][i];
								SysCtrl.Sample_temp[74][i].data=temp[5][i];
								SysCtrl.Sample_temp[75][i].data=temp[4][i];
								SysCtrl.Sample_temp[76][i].data=temp[3][i];
								SysCtrl.Sample_temp[77][i].data=temp[2][i];
								SysCtrl.Sample_temp[78][i].data=temp[1][i];
								SysCtrl.Sample_temp[79][i].data=temp[0][i];
						}break;
			case 2:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[80][i].data=temp[7][i];
								SysCtrl.Sample_temp[81][i].data=temp[6][i];
								SysCtrl.Sample_temp[82][i].data=temp[5][i];
								SysCtrl.Sample_temp[83][i].data=temp[4][i];
								SysCtrl.Sample_temp[84][i].data=temp[3][i];
								SysCtrl.Sample_temp[85][i].data=temp[2][i];
								SysCtrl.Sample_temp[86][i].data=temp[1][i];
								SysCtrl.Sample_temp[87][i].data=temp[0][i];
						}break;
			case 3:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[88][i].data=temp[7][i];
								SysCtrl.Sample_temp[89][i].data=temp[6][i];
								SysCtrl.Sample_temp[90][i].data=temp[5][i];
								SysCtrl.Sample_temp[91][i].data=temp[4][i];
								SysCtrl.Sample_temp[92][i].data=temp[3][i];
								SysCtrl.Sample_temp[93][i].data=temp[2][i];
								SysCtrl.Sample_temp[94][i].data=temp[1][i];
								SysCtrl.Sample_temp[95][i].data=temp[0][i];
						}break;
			case 4:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[96][i].data=temp[7][i];
								SysCtrl.Sample_temp[97][i].data=temp[6][i];
								SysCtrl.Sample_temp[98][i].data=temp[5][i];
								SysCtrl.Sample_temp[99][i].data=temp[4][i];
								SysCtrl.Sample_temp[100][i].data=temp[3][i];
								SysCtrl.Sample_temp[101][i].data=temp[2][i];
								SysCtrl.Sample_temp[102][i].data=temp[1][i];
								SysCtrl.Sample_temp[103][i].data=temp[0][i];
						}break;
				case 5:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[104][i].data=temp[7][i];
								SysCtrl.Sample_temp[105][i].data=temp[6][i];
								SysCtrl.Sample_temp[106][i].data=temp[5][i];
								SysCtrl.Sample_temp[107][i].data=temp[4][i];
								SysCtrl.Sample_temp[108][i].data=temp[3][i];
								SysCtrl.Sample_temp[109][i].data=temp[2][i];
								SysCtrl.Sample_temp[110][i].data=temp[1][i];
								SysCtrl.Sample_temp[111][i].data=temp[0][i];
						}break;
			case 6:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[112][i].data=temp[7][i];
								SysCtrl.Sample_temp[113][i].data=temp[6][i];
								SysCtrl.Sample_temp[114][i].data=temp[5][i];
								SysCtrl.Sample_temp[115][i].data=temp[4][i];
								SysCtrl.Sample_temp[116][i].data=temp[3][i];
								SysCtrl.Sample_temp[117][i].data=temp[2][i];
								SysCtrl.Sample_temp[118][i].data=temp[1][i];
								SysCtrl.Sample_temp[119][i].data=temp[0][i];
						}break;		
			case 7:for(i=0;i<SAMPLECNT;i++)
						{
								SysCtrl.Sample_temp[120][i].data=temp[7][i];
								SysCtrl.Sample_temp[121][i].data=temp[6][i];
								SysCtrl.Sample_temp[122][i].data=temp[5][i];
								SysCtrl.Sample_temp[123][i].data=temp[4][i];
								SysCtrl.Sample_temp[124][i].data=temp[3][i];
								SysCtrl.Sample_temp[125][i].data=temp[2][i];
								SysCtrl.Sample_temp[126][i].data=temp[1][i];
								SysCtrl.Sample_temp[127][i].data=temp[0][i];
						}break;	
			default:break;				
						
		} 
}
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值： 
注意： 
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void SampAGEVolChanData(uint16_t ChanNo)
{
    uint32_t   temp[8][SAMPLECNT];
    uint16_t   i,itemp;

    for(i=0; i<8; i++)
    {
         Read_8688(ADS8688_CH_TABLE[i]);
    }
    for(i=0;i<SAMPLECNT;i++)
    {
        for(itemp=0;itemp<8;itemp++)
        {
            temp[itemp][i]  = Read_8688(ADS8688_CH_TABLE[itemp]);
        }
    }
    for(i=0;i<SAMPLECNT;i++)
    {
        SysCtrl.Sample_Voltemp[ChanNo][i].data=temp[7][i];
    } 
} 





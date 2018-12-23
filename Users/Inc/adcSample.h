
#ifndef  __ADCSAMPLE_H
#define  __ADCSAMPLE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
   
typedef enum
{
    FullPower = 0,                              /* Boost电压满额. */
    ReductionPower = 1,                         /* Boost电压降额. */
    UnderVoltage = 2,                           /* Boost电压欠压. */
    OverVoltage = 3,                            /* Boost电压过压. */
}boostVoltStatusDef_t;

typedef struct
{
    boostVoltStatusDef_t inputSta;              /* Boost输入电压状态. */
    float inputVolt;                            /* Boost输入电压. */
    
    boostVoltStatusDef_t outputSta;             /* Boost输出电压状态. */
    float outputVolt;                           /* Boost输出电压. */
}boostVoltParaDef_t;


typedef struct
{
    int8_t tempResult;
    float  resistance;
}ntcSensorParaDef_t;



   
#define  ADC_SAMPLE_RAWDATABUF_SIZE     (10)
   
   
void adcTempChannelInit_LL(void);
void adcBoostInputVoltChannelInit_LL(void);
void adcBoostOutputVoltChannelInit_LL(void);

void adcSampleRawdataBuff_Write(uint16_t data);
void configADConvertCompleteMutexFlag(bool val);
void adcSampleConvertScan(void);
uint16_t adcMovingFilter(uint16_t* pRawData, uint8_t len);

void adcSampleTriggerScan(void);
void adcSampleGetResult(void);

#ifdef __cplusplus
}
#endif

#endif

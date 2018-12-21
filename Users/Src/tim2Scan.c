
#include "tim2Scan.h"
#include "adcSample.h"


/*
 * @函数功能：TIMER2定时器用于实时扫描某些任务初始化
 * @函数参数：无
 * @返回值：无
 */
void tim2ScanInit_LL(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
    
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_128, (625 - 1));               /* 16MHz / (128 * 625) = 0.2KHz = 5ms. */
    TIM2_ARRPreloadConfig(DISABLE);
    TIM2_UpdateDisableConfig(DISABLE);
    TIM2_UpdateRequestConfig(TIM2_UPDATESOURCE_REGULAR);
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    TIM2_SetCounter(0);
    TIM2_Cmd(ENABLE);
}

/*
 * @函数功能：
 * @函数参数：无
 * @返回值：无
 */
void tim2UpdateCallback(void)
{
    if (TIM2_GetITStatus(TIM2_IT_UPDATE) != RESET)
    {
        TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
        
        adcSampleTriggerScan();
    }
}


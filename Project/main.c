/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief   This file contains the main function for the ADC2 Continuous Conversion example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "clock.h"
#include "tim4tick.h"
#include "ledlight.h"
#include "usart.h"
#include "tim1PwmCtrl.h"
#include "comm.h"
#include "voltRegulate.h"
#include "ioctrl.h"
#include "adcSample.h"
#include "tim2Scan.h"


/*
 * @函数功能：系统上电启动后的主函数
 * @函数参数：无
 * @返回值：无
 */
void main(void)
{
    systemClockInit_LL();
    ioCtrlInit_LL();
    systemTimTickInit_LL();
    ledLightInit_LL();
    usartConfig_LL();
    timer1PwmControlInit_LL();
    tim2ScanInit_LL();
    adcBoostInputVoltChannelInit_LL();
    
    enableInterrupts();
    
	while (1)
	{
        adcSampleGetResult();
        commSendCtrlInfo();
        commReceivedFrameParsing();
    }
}


 
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

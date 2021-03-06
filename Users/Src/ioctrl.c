
#include "ioctrl.h"
#include "comm.h"
#include "adcSample.h"
#include "tim4tick.h"


static bool boostOutputEnableStatus  = FALSE;		/* Boost输出使能状态. FALSE,输出关闭;TRUE,输出开启; */
static bool llcOutputEnableStatus    = FALSE;		/* 后级LLC输出使能状态. FALSE,输出关闭;TRUE,输出开启; */
static bool relayStatus              = FALSE;		/* 继电器输出状态.FALSE,关闭态;TRUE,开启态; */


/*
 * @函数功能：获取LLC输出使能状态.
 * @函数参数：无
 * @返回值：TRUE, 后级LLC输出使能打开; FALSE, 后级LLC输出使能关闭;
 */
bool getLLCOutputEnableStatus(void)
{
    return (llcOutputEnableStatus);
}

/*
 * @函数功能：改写LLC输出使能状态.
 * @函数参数：wdata, 待改写的状态值.可取值TRUE或者FALSE.
 * @返回值：无
 */

void configLLCOutputEnableStatus(bool wdata)
{
	llcOutputEnableStatus = wdata;
}

/*
 * @函数功能：获取当前Boost输出使能状态.
 * @函数参数：无
 * @返回值：TRUE, Boost输出使能打开; FALSE, Boost输出使能关闭;
 */
bool getBoostOutputEnableStatus(void)
{
	return (boostOutputEnableStatus);
}

/*
 * @函数功能：改写Boost输出使能状态.
 * @函数参数：wdata, 待改写的状态值.可取值TRUE或者FALSE.
 * @返回值：无
 */
void configBoostOutputEnbaleStatus(bool wdata)
{
	boostOutputEnableStatus = wdata;
}

/*
 * @函数功能：获取当前继电器输出工作状态.
 * @函数参数：无
 * @返回值：TRUE, 继电器处于开启状态; FALSE, 继电器处于关闭状态.
 */
bool getSystemRelayOutputStatus(void)
{
	return (relayStatus);
}

/*
 * @函数功能：改写继电器输出状态.
 * @函数参数：wdata, 待改写的状态值.可取值TRUE或者FALSE.
 * @返回值：无
 */
void configSystemRelayOutputStatus(bool wdata)
{
	relayStatus = wdata;
}

/*
 * @函数功能：IO输入输出端口初始化
 * @函数参数：无
 * @返回值：无
 */
void ioCtrlInit_LL(void)
{
    GPIO_Init(RELAY_CTRL_PORT, RELAY_CTRL_PIN, GPIO_MODE_OUT_PP_LOW_FAST);        /* 上电默认关闭继电器输出. */
    GPIO_Init(ANOLOG_CTRL_PORT, ANOLOG_CTRL_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);     /* 上电默认禁止模拟主控芯片UC3843使能. */
	GPIO_Init(PFCVS22_CTRL_PORT, PFCVS22_CTRL_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);	  /* 上电默认关断后级PFC输出,需要在接收到开机指令后才打开. */
}

/*
 * @函数功能：继电器输出打开
 * @函数参数：无
 * @返回值：无
 */
void ioCtrlRelayOpen_LL(void)
{
	if (relayStatus == FALSE)
	{
    	GPIO_WriteHigh(RELAY_CTRL_PORT, RELAY_CTRL_PIN);
		relayStatus = TRUE;
	}
}

/*
 * @函数功能：继电器输出关闭
 * @函数参数：无
 * @返回值：无
 */
void ioCtrlRelayClose_LL(void)
{
	if (relayStatus == TRUE)
	{
    	GPIO_WriteLow(RELAY_CTRL_PORT, RELAY_CTRL_PIN);
		relayStatus = FALSE;
	}
}

/*
 * @函数功能：Boost输出驱动芯片使能打开.
 * @函数参数：无
 * @返回值：无
 */
void ioCtrlBoostOutputEnable_LL(void)
{
	if (boostOutputEnableStatus == FALSE)
	{
    	GPIO_WriteLow(ANOLOG_CTRL_PORT, ANOLOG_CTRL_PIN);
		boostOutputEnableStatus = TRUE;
	}
}

/*
 * @函数功能：Boost输出驱动芯片使能关闭.
 * @函数参数：无
 * @返回值：无
 */
void ioCtrlBoostOutputDisable_LL(void)
{
	if (boostOutputEnableStatus == TRUE)
	{
    	GPIO_WriteHigh(ANOLOG_CTRL_PORT, ANOLOG_CTRL_PIN);
		boostOutputEnableStatus = FALSE;
	}
}

/*
 * @函数功能：后级LLC驱动芯片NCP1397AC使能打开.
 * @函数参数：无
 * @返回值：无
 */
void ioCtrlLLCOutputEnable_LL(void)
{
	if (llcOutputEnableStatus == FALSE)
	{
		GPIO_WriteLow(PFCVS22_CTRL_PORT, PFCVS22_CTRL_PIN);
		llcOutputEnableStatus = TRUE;
	}
}

/*
 * @函数功能：后级LLC驱动芯片NCP1397AC使能关闭.
 * @函数参数：无
 * @返回值：无
 */
void ioCtrlLLCOutputDisable_LL(void)
{
	if (llcOutputEnableStatus == TRUE)
	{
		GPIO_WriteHigh(PFCVS22_CTRL_PORT, PFCVS22_CTRL_PIN);
		llcOutputEnableStatus = FALSE;
	}
}

/*
 * @函数功能：Boost输出和后级LLC输出实时检测控制.
 * @函数参数：无
 * @返回值：无
 */
void ioCtrlProcessing(void)
{
	SystemInfoParaDef_t *pInfoPar = getSystemInfoParaPtr();
	
	if (getSystemSwitchingChangeStatus() == TRUE)					/* 系统开关机状态有变化吗? */
	{
		if (getSystemCurrentSwitchingStatus() == TRUE)				/* 是开机态. */
		{
			ioCtrlBoostOutputEnable_LL();							/* 开启boost输出. */

			if (pInfoPar->outputSta == FullPower)					/* 等待,直到检测到boost输出电压在正常范围后才输出后级LLC. */
			{
				ioCtrlLLCOutputEnable_LL();							/* 开启后级LLC输出. */

				configSystemSwitchingChangeStatus(FALSE);			/* 系统开关机状态变化标志清除,等待直到下一次有新的变化. */
			}
		}
		else														/* 是关机态. */
		{
			ioCtrlLLCOutputDisable_LL();							/* 先关闭后级LLC输出. */

			systemDelayms(100);										/* 延时等待100ms. */

			ioCtrlBoostOutputDisable_LL();							/* 再关闭前级boost输出. */

			configSystemSwitchingChangeStatus(FALSE);				/* 系统开关机状态变化标志清除,等待直到下一次有新的变化. */
		}
	}
	else															/* 开关机状态无变化,需要实时检测母线电压是否正常,若异常需要关闭LLC输出. */
	{
		if (getSystemCurrentSwitchingStatus() == TRUE)				/* 是开机态. */
		{
			if (pInfoPar->outputSta != FullPower)
			{
				ioCtrlLLCOutputDisable_LL();						/* 关闭后级LLC输出. */
			}
		}
	}
}




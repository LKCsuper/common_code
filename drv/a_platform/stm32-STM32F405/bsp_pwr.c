/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-10-30 13:29:44
 * @LastEditors: lkc
 * @LastEditTime: 2024-01-08 16:21:01
 * @detail: 
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "rtthread.h"

#include "common.h"
  
#include "pwr.h"
#include "gpio.h"
  
#include "bsp_gpio.h"
#include "app_log.h"

#include "app_flash.h"

#include "includes.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 外设上下电
 * @detail: 
 * @param {PWR_STATE_E} enPwr
 * @return {*}
 * @author: lkc
 */
void PWR_Extern(PWR_STATE_E enPwr)
{
    BUZZER_OFF();               /* 默认上电下电先关喇叭 */
    
    switch(enPwr)
    {   
        case PWR_WAKE:
        case PWR_ON:
        {
            if (TRUE == gpstParamState->isPower)
            {
                PWR_NANO_ON();
                PWR_RSV_ON();
                PWR_CAMERA_ON();
                PWR_LASER_ON();
                PWR_LIGHT_ON();
                HWT_PWR_EN_EN();
            }
            break;
        }
        case PWR_SLEEP:
        case PWR_OFF:
        {
            PWR_NANO_OFF();
            PWR_RSV_OFF();
            //PWR_CAMERA_OFF();
            PWR_LASER_OFF();
            PWR_LIGHT_OFF();

            RTKBtoRTKH_data.msg2.flash_flag = 0;
            RTKBtoRTKH_data.msg3.flash_flag = 0;
            break;
        }
        default: break;
    }

    return;
}

/**
 * @description: 板外部电源上下电
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void PWR_Module(PWR_STATE_E enPwr)
{
    switch (enPwr)
    {
        case PWR_ON:
        {
            LORA_RST_H();
            LORA_PWR_0N();
            OS_DELAY(200);
            /* 复位rst */
            LORA_RST_L();
            OS_DELAY(200);
            LORA_RST_H();

            /* 电源开关 */
            if (TRUE == gpstParamState->isPower)
            {
                /* 拉低模块的RESET引脚来重启模块,推荐使用PWRKEY开关机 RESET 复位时间推荐 2.5s
                */
                GPS_RST_H();
                GPS_PWR_ON();
                HWT_PWR_EN_EN();

                /* rtk电源开关 如果处理rtk差分信息,才给4g上电 */
                if (TRUE == gpstParamState->isRtkPro)
                {
                    G4_RST_EN();
                    OS_DELAY(100);
                    G4_RST_DIS();
                    //G4_PWRKEY_EN();
                    //G4_PWR_EN_EN();
                }
            }
            break;
        }
        case PWR_WAKE:
        {
            /* 电源开关 */
            if (TRUE == gpstParamState->isPower)
            {
                /* 拉低模块的RESET引脚来重启模块,推荐使用PWRKEY开关机RESET 复位时间推荐 2.5s */
                //GPS_RST_H();
                //GPS_PWR_ON();

                GPS_RST_L();                        /* 开始复位 */

                HWT_PWR_EN_EN();

                /* 4g 由gpstParamState->isRtkPro开关 */
            }
            break;
        }
        case PWR_SLEEP:
        case PWR_OFF:
        {
            /* 中断关闭以及中断 */
            // USART_ITConfig(UART5, USART_IT_IDLE , DISABLE);
            // USART_Cmd(UART5, DISABLE);          /* 4G */

            // USART_ITConfig(USART2, USART_IT_IDLE , DISABLE);
            // USART_Cmd(USART2, DISABLE);         /* lora */

            USART_ITConfig(USART1, USART_IT_IDLE , DISABLE);
            USART_Cmd(USART1, DISABLE);         /* GPS */

            USART_ITConfig(UART4, USART_IT_IDLE , DISABLE);
            USART_Cmd(UART4,  DISABLE);         /* HWT */

            USART_ITConfig(USART6, USART_IT_IDLE , DISABLE);
            USART_Cmd(USART6, DISABLE);         /* NANO */

            /* 模组 4g hwt gps,只开启lora */
            //G4_PWRKEY_DIS();
            //G4_PWR_EN_DIS();
            //HWT_PWR_EN_DIS();
            GPS_PWR_OFF();
            break;
        }
    default:
        break;
    }

    return;
}

/**
 * @description: 电源控制
 * @detail: 
 * @param {PWR_STATE_E} enPwr
 * @return {*}
 * @author: lkc
 */
void PWR_RTK(PWR_STATE_E enPwr)
{
    switch (enPwr)
    {
        case PWR_ON:
        {
			G4_PWR_EN_EN();

            /* 4g 有起不来的可能 */
            G4_RST_EN();
            OS_DELAY(100);
            G4_RST_DIS();
            APP_LED_Ctrl(LED_RTK);
            NOTICE("RTK Enable\r\n");


            /* 开启定时器检测 */

            break;
        }
        case PWR_OFF:
        {
            TASK_GPS_SDK_Destory();                             /* 删除sdk任务 */

            /* 4g 参数初始化 */
            FLASH_StateG4Default();

            /* gps 参数 */
            gpstGpsState->isRecvRtk = FALSE;                    /* 默认没有收到差分消息 */
            gpstGpsState->isSendRtk = TRUE;                     

            /* 4g下电 */
            G4_PWR_EN_DIS();

            /* 4g obj参数恢复默认 */
            G4_Device_Deinit();
            G4_Urc_Init();
            gpstG4State->ipdSize = 0;       /* 清除标志,注意理论上所有的static都要恢复，目前注意到这一个变量有影响 */
            if(TRUE == gpstParamCtrl->isPowerControl) APP_LED_Ctrl(LED_DEV_ONLINE);
            NOTICE("RTK Disable\r\n");
            break;
        }
        default:
            break;
    }

    return;
}
#ifdef __cplusplus
}
#endif



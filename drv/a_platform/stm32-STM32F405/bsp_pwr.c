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
 * @description: �������µ�
 * @detail: 
 * @param {PWR_STATE_E} enPwr
 * @return {*}
 * @author: lkc
 */
void PWR_Extern(PWR_STATE_E enPwr)
{
    BUZZER_OFF();               /* Ĭ���ϵ��µ��ȹ����� */
    
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
 * @description: ���ⲿ��Դ���µ�
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
            /* ��λrst */
            LORA_RST_L();
            OS_DELAY(200);
            LORA_RST_H();

            /* ��Դ���� */
            if (TRUE == gpstParamState->isPower)
            {
                /* ����ģ���RESET����������ģ��,�Ƽ�ʹ��PWRKEY���ػ� RESET ��λʱ���Ƽ� 2.5s
                */
                GPS_RST_H();
                GPS_PWR_ON();
                HWT_PWR_EN_EN();

                /* rtk��Դ���� �������rtk�����Ϣ,�Ÿ�4g�ϵ� */
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
            /* ��Դ���� */
            if (TRUE == gpstParamState->isPower)
            {
                /* ����ģ���RESET����������ģ��,�Ƽ�ʹ��PWRKEY���ػ�RESET ��λʱ���Ƽ� 2.5s */
                //GPS_RST_H();
                //GPS_PWR_ON();

                GPS_RST_L();                        /* ��ʼ��λ */

                HWT_PWR_EN_EN();

                /* 4g ��gpstParamState->isRtkPro���� */
            }
            break;
        }
        case PWR_SLEEP:
        case PWR_OFF:
        {
            /* �жϹر��Լ��ж� */
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

            /* ģ�� 4g hwt gps,ֻ����lora */
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
 * @description: ��Դ����
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

            /* 4g �������Ŀ��� */
            G4_RST_EN();
            OS_DELAY(100);
            G4_RST_DIS();
            APP_LED_Ctrl(LED_RTK);
            NOTICE("RTK Enable\r\n");


            /* ������ʱ����� */

            break;
        }
        case PWR_OFF:
        {
            TASK_GPS_SDK_Destory();                             /* ɾ��sdk���� */

            /* 4g ������ʼ�� */
            FLASH_StateG4Default();

            /* gps ���� */
            gpstGpsState->isRecvRtk = FALSE;                    /* Ĭ��û���յ������Ϣ */
            gpstGpsState->isSendRtk = TRUE;                     

            /* 4g�µ� */
            G4_PWR_EN_DIS();

            /* 4g obj�����ָ�Ĭ�� */
            G4_Device_Deinit();
            G4_Urc_Init();
            gpstG4State->ipdSize = 0;       /* �����־,ע�����������е�static��Ҫ�ָ���Ŀǰע�⵽��һ��������Ӱ�� */
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



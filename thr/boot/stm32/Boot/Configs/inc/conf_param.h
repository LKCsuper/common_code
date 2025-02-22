/*
 * @Description: ????
 * @detail: 
 * @Version: 1.0
 * @Date: 2023-05-24 08:37:49
 * @LastEditTime: 2024-09-06 09:52:32
 * @LastEditors: Please set LastEditors
 */
#ifndef __CONF_PARAM_H__
#define __CONF_PARAM_H__
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* ???????????? */
#define VERSION_APP                                     (0x01U)
#define VERSION_HARDWARE                                (0x01U)

/* ?????? */
#define MASTAR                                          (0U)

// TODO ???????????????

/* Private typedef -----------------------------------------------------------*/
/* ???? */
typedef struct tagGlobaParam
{
    IAP_PARAM_S stIap;
    UCHAR ucRes[4];
}GLOBAL_PARAM_S;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern GLOBAL_PARAM_S   gstGlobalParam;
extern GLOBAL_PARAM_S   *gpstGlobalParam;
extern IAP_PARAM_S      *gpstIapParam;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
VOID Conf_Param_Init(VOID);

// TODO ?????????

#endif

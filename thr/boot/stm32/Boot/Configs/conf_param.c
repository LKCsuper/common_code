/*
 * @Description: 参数配置以及初始化
 * @detail: 
 * @Version: 1.0
 * @Date: 2023-05-24 08:37:49
 * @LastEditTime: 2023-10-13 11:38:20
 * @LastEditors: lkc
 */
#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GLOBAL_PARAM_S gstGlobalParam;
GLOBAL_PARAM_S *gpstGlobalParam = NULL;

/* 全局子变量 */
IAP_PARAM_S     *gpstIapParam = NULL;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: IAP初始化
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID Conf_Param_IapInit(VOID)
{
    memset(gpstIapParam, 0, sizeof(IAP_PARAM_S));

    gpstIapParam->ulBlud = 115200;
    gpstIapParam->bEnterBoot = FALSE;
    gpstIapParam->bExit = FALSE;

    return;
}

/**
 * @description: 参数恢复默认
 * @detail: 
 * @return {*}
 * @author: lkc
 */
STATIC VOID Conf_Param_Deinit(VOID)
{
    Conf_Param_IapInit();

    return;
}

/**
 * @description: 参数初始化
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID Conf_Param_Init(VOID)
{
    gpstGlobalParam = &gstGlobalParam;
    /* 初始化全局变量 */
    gpstIapParam   = &gpstGlobalParam->stIap;

    /* 清空全局值 */
    memset(gpstGlobalParam, 0, sizeof(GLOBAL_PARAM_S));

    Conf_Param_Deinit();

    return;
}

#ifdef __cplusplus
}
#endif

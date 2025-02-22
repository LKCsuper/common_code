/*
 * @Description: iap参数配置
 * @detail: 
 * @Version: 1.0
 * @Date: 2023-06-08 09:38:45
 * @LastEditTime: 2023-12-04 11:33:26
 * @LastEditors: lkc
 */
#ifndef __CONF_PARAM_IAP_H__
#define __CONF_PARAM_IAP_H__
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_PAGE_SIZE                                                     (0x400U)
/* Flash 起???地址 */
#define STORE_START                                                         (0x08000000U)
/* 默???擦除大??4kb */
#define STORE_ERASE_SIZE                                                    (FLASH_PAGE_SIZE)
/* Flash 大小 */
#define STORE_SIZE                                                          (0x20000U)
#define STORE_END                                                           (STORE_START + STORE_SIZE)
/* 暂时默???最后一页为测试地址 */
#define STORE_TEST_SIZE                                                     (STORE_END - STORE_ERASE_SIZE)
/* Private typedef -----------------------------------------------------------*/
typedef struct tagIapParam
{
    /* 板级信息 */
    ULONG ulBlud;                   /* 串口波特?? */
    ULONG ulWdgtTim;                /* 看门狗???位时间 */            

    /* iap信息 */
    BOOL bEnterBoot;                /* ??否进??boot */
    BOOL bExit;                    /* 是否退出shell */

    BOOL bYomdemUpdate;             /* ymodem升级 */

    UCHAR ucTmp[1100];
}IAP_PARAM_S;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#endif

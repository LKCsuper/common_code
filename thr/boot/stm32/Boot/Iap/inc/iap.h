/*
 * @Description: iap升级
 * @detail: 注意 flash划分的时候一定要4kb对齐
 * @Version: 1.0
 * @Date: 2023-06-08 10:39:19
 * @LastEditTime: 2023-10-13 10:21:51
 * @LastEditors: lkc
 */
#ifndef __IAP_H__
#define __IAP_H__
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UPDATE_FLAG_ADR                                 (0x0806FFFFU)                           /* 升级地址 */
#define UPDATE_FLAG                                     (0x12345678U)                           /* 升级标志 */

#define BOOT_SIZE                                       (ULONG)(16 * 1024)                      /* boot大小 */
#define BOOT_START_ADDR                                 (FLASH_BASE)                            /* boot开始大小 */
#define BOOT_END_ADDR                                   (BOOT_START_ADDR + BOOT_SIZE)           /* boot结束地址 */

#define APP_SIZE                                        (ULONG)(368 * 1024)                     /* app大小 */
#define APP_START_ADDR                                  (BOOT_END_ADDR)                         /* app开始地址 */
#define APP_END_ADDR                                    (APP_START_ADDR + APP_SIZE)

#define APP_OFFSET                                      (APP_START_ADDR - BOOT_START_ADDR)      /* app偏移 */

#define APP_BAK_SIZE                                    (ULONG)(512 * 1024)                     /* 备份区大小 */
#define APP_BAK_START_ADDR                              (APP_END_ADDR)                          /* 备份区起始地址 */
#define APP_BAK_END_ADDR                                (APP_BAK_START_ADDR + APP_BAK_SIZE)     /* 备份区结束地址 */

#define CONFIG_SIZE                                     (128 * 1024)                            /* 配置大小 */
#define CONFIG_START_ADDR                               (APP_BAK_END_ADDR)                      /* 配置起始大小 */
#define CONFIG_END_ADDR                                 (CONFIG_START_ADDR + CONFIG_SIZE)       /* 配置结束地址 */
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
VOID Iap_BootDelay(VOID);
VOID Iap_Init(VOID);
#endif

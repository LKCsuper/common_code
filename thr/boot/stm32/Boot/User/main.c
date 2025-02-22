/*
 * @Description: 主函数
 * @detail: 
 * @Version: 1.0
 * @Date: 2023-05-24 08:37:49
 * @LastEditTime: 2023-12-04 11:35:58
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
#define BOOT_VERSION                                                        (0x01U)     /* Boot版本 */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*!
    \brief      显示基本信息
    \param[in]  none
    \param[out] none
    \retval     none
*/
STATIC VOID Main_Show(VOID)
{
    /* 编译时间 版本*/
    PRINTF_BLUE("Boot Start, Hardware Init Success\r\n");
    PRINTF_BLUE("Complie Time: %s %s\r\n", __DATE__, __TIME__);
    PRINTF_BLUE("Version: %s\r\n", TO_STR(BOOT_VERSION));
    PRINTF_BLUE(" ____   ____   ____ _______ \r\n");
    PRINTF_BLUE("|  _ \\ / __ \\ / __ \\__   __|\r\n");
    PRINTF_BLUE("| |_) | |  | | |  | | | |   \r\n");
    PRINTF_BLUE("|  _ <| |  | | |  | | | |   \r\n");
    PRINTF_BLUE("| |_) | |__| | |__| | | |  \r\n");
    PRINTF_BLUE("|____/ \\____/ \\____/  |_| \r\n");

    return;
}

/**
 * @description: 主函数
 * @detail: 
 * @return {*}
 * @author: lkc
 */
int main(VOID)
{
    /* shell 初始化 */
    Shell_Init();

    /* bsp外设初始化 */
    Bsp_Init();

    /* 参数初始化 */
    Conf_Param_Init();

    /* 显示板级信息 */
    Main_Show();

    /* 判断是否收到b，收到b进入boot */
    Iap_BootDelay();
    
    /* 如果有升级标志，并且备份区有app */
    if (YMODEM_FLAG == Ymodem_Flash_Get_Flag() && (REG32(APP_BAK_START_ADDR) != REG32(APP_BAK_START_ADDR + 4)))
    {
        ULONG ulWriteSize = REG32(YMODEM_FLAG_SIZE);
        gpstYmodemParma->ulHasWrite = ulWriteSize;
        /* 校验crc */
        USHORT usCrc16 = Common_CRC16((uint8_t *)(uint32_t)(APP_BAK_START_ADDR), gpstYmodemParma->ulHasWrite);
        YMODEM_INFO("Update Crc[%d] [%d]\r\n", usCrc16, REG16(YMODEM_FLAG_CRC));
        YMODEM_INFO("Update Start.Size[%d]\r\n", ulWriteSize);

        /* 文件大小限制,crc校验 */
        if ((gpstYmodemParma->ulHasWrite > 64 * 1024) && (REG16(YMODEM_FLAG_CRC) == usCrc16))
        {
            // TODO 应该是写多少擦多少
            Ymodem_Flash_Erase(YMODEM_UPDATE_ADDR, YMODEM_UPDATE_SIZE);
            YMODEM_INFO("Update Erase App\r\n");
            /* 拷贝备份区到app */
            Ymodem_Flash_CopyApp();
            YMODEM_NOTICE("Update Success\r\n");
        }
        else    /* 防止出错,规定最小 */
        {
            Ymodem_Flash_Erase(YMODEM_BACK_ADDRESS, YMODEM_BACK_SIZE);
        }
        /* 清除ymodem升级标志,如果擦除整个app区，所以也会被擦除 */
        /* Ymodem_Flash_Set_Flag(false); */
    } 

    /* 如果备份区有数据,并且升级标志没有,擦除备份区,此操作保证每一次app中操作备份区为空 */
    if (YMODEM_FLAG != Ymodem_Flash_Get_Flag() && (REG32(APP_BAK_START_ADDR) != REG32(APP_BAK_START_ADDR + 4)))
    {
        YMODEM_INFO("Erase Back App\r\n");
        /* 擦除备份区 */
        Ymodem_Flash_Erase(YMODEM_BACK_ADDRESS, YMODEM_BACK_SIZE);
    }

    /* (1) 按b进入命令行 (2) 判断app bak区是否有app,没有的话直接进入 */
    if (TRUE == gpstIapParam->bEnterBoot || (REG32(APP_START_ADDR) == REG32(APP_START_ADDR+4)))
    {
        YMODEM_INFO("Enter Cmd Mode. \r\n");
        /* 进入命令行 */
        Shell_Process();
    }

    if (REG32(APP_START_ADDR) != REG32(APP_START_ADDR + 4))    /* 如果app区存在app */
    {
        /* 喂狗，避免跳转打断 */
        Ymodem_Wdgt_Reload();
        /* app跳转 */
        Ymodem_Jump_App();
    }
    else    /* 如果app区没有文件 */
    {
        Bsp_Misc_Reset();
    }
}
#ifdef __cplusplus
}
#endif

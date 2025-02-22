/*
 * @Description: 
 * @Version: 2.0
 * @Author: lkc
 * @Date: 2022-12-24 17:02:40
 * @LastEditors: lkc
 * @LastEditTime: 2023-11-29 14:51:15
 */
#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "ymodem_includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 串口初始化
 * @detail: 
 * @param {uint32_t} ulBound 波特率
 * @return {*}
 * @author: lkc
 */
WEAK void Ymodem_Uart_Init(uint32_t ulBound)
{
    YMODEM_WARNING("You have to make sure you don't need this function [ %s ] \r\n", __func__);
    return;
}

/**
 * @description: 输出到终端一个字符
 * @detail:
 * @param {uint8_t} c 输出字符
 * @return {*}
 * @author: lkc
 */
WEAK void Ymodem_PutChar(uint8_t c)
{
    YMODEM_WARNING("You have to make sure you don't need this function [ %s ] \r\n", __func__);
    return;
}

/**
 * @description: 获取终端字符
 * @detail:
 * @param {uint8_t} *key
 * @return {*}
 * @author: lkc
 */
WEAK uint32_t Ymodem_GetChar(uint8_t *key)
{
    YMODEM_WARNING("You have to make sure you don't need this function [ %s ] \r\n", __func__);
    return 0;
}

/**
 * @brief  flash初始化
 * @param  None
 * @retval None
 */
WEAK void Ymodem_Flash_Init(void)
{
    YMODEM_WARNING("You have to make sure you don't need this function [ %s ] \r\n", __func__);
    return;
}

/**
 * @brief  将数据写入flash中(data are 32-bit aligned).
 * @note   After writing data buffer, the flash content is checked.
 * @param  FlashAddress: start address for writing data buffer
 * @param  Data: pointer on data buffer
 * @return YMODEM_SUCCESS 写成功
 *         YMODEM_ERROR 写失败
 */
WEAK uint32_t Ymodem_Flash_Write(uint32_t FlashAddress ,uint32_t Data)
{
    YMODEM_WARNING("You have to make sure you don't need this function [ %s ] \r\n", __func__);

    if (FLASH_COMPLETE == FLASH_ProgramWord(FlashAddress, Data))
    {
        return YMODEM_SUCCESS;
    }

    return YMODEM_SUCCESS;
}

/**
 * @description: 升级标志
 * @detail: 主要是ymodem升级到备份区之后然后置位,关于这个升级标志存放位置，可以放在升级app_bak 最后4个字节
 * 这样既不影响程序运行，当擦除升级标志的时候正好一起将app_bak擦掉
 * @param {bool} isSet 是否置位
 * @return {*}
 * @author: lkc
 */
WEAK void Ymodem_Flash_Set_Flag(bool isSet)
{
    YMODEM_WARNING("You have to make sure you don't need this function [ %s ] \r\n", __func__);

    /* 写一??4字节的数?? */

    return;
}

/**
 * @description: 升级标志
 * @detail: 主要是ymodem升级到备份区之后然后置位
 * @param {bool} isSet 是否置位
 * @return {*}
 * @author: lkc
 */
WEAK uint32_t Ymodem_Flash_Get_Flag(void)
{
    YMODEM_WARNING("You have to make sure you don't need this function [ %s ] \r\n", __func__);

    /* 读取一??四字节的数据 */

    return 0;
}

/**
 * @brief  写app
 * @param  Data: 数据
 * @param  DataLength：写数据长度
 * @retval write DataLen
 */
WEAK uint32_t Ymodem_Flash_Write_App(uint8_t *Data, uint32_t DataLength)
{
    YMODEM_WARNING("You have to make sure you don't need this function [ %s ] \r\n", __func__);

    uint32_t i = 0;
    STATIC __IO ULONG FlashAddress = YMODEM_BACK_ADDRESS;

    for (i = 0; (i < DataLength) && (FlashAddress <= (YMODEM_BACK_END_ADDRESS - 4)); i++)
    {
        /* 电压范围2.7v到3.6v,操作才能完成写一个word */
        /* 如果??4字节数据成功 */
        if (YMODEM_SUCCESS == Ymodem_Flash_Write(FlashAddress, *(uint32_t *)(Data + i)))
        {
            /* 写入后读出数据 判断数据是不是相等 */
            if (*(uint32_t *)FlashAddress != *(uint32_t *)(Data + i))
            {
                /* Flash content doesn't match SRAM content */
                return (2);
            }
            /* Increment FLASH destination address */
            FlashAddress += 4;
        }
        else
        {
            /* Error occurred while writing data in Flash memory */
            return (1);
        }
    }
    
    return 0;
}

/**
 * @description: 看門狗喂狗
 * @detail: 
 * @return {*}
 * @author: lkc
 */
WEAK void Ymodem_Wdgt_Reload(VOID)
{
    YMODEM_WARNING("You have to make sure you don't need this function [ %s ] \r\n", __func__);
    return;
}

/**
 * @description: 跳转app
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void Ymodem_Jump_App(void)
{
    Bsp_Misc_Jump(YMODEM_UPDATE_ADDR);

    return;
}

extern unsigned char file_name[FILE_NAME_LENGTH];
extern unsigned char file_size[FILE_SIZE_LENGTH];
/**
 * @description: 显示ymodem信息
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void Ymodem_ShowFileInfo(void)
{
	YMODEM_PRINTF("you maybe receive success!\r\n");
	YMODEM_PRINTF("file name[%s], file size[%s]\r\n", file_name, file_size);

	return;
}

/**
 * @description: 消息显示
 * @detail:
 * @return {*}
 * @author: lkc
 */
void Ymodem_Info(void)
{
    YMODEM_PRINTF("\r\n========================================================\r\n");
    YMODEM_PRINTF("\r\nTime: [ %s ] [ %s ]\r\n", __DATE__, __TIME__);
    YMODEM_PRINTF("Attention: Do not print during ymodem transfer\r\n");

    return;
}

/**
 * @description: debug 信息
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID Ymodem_Debug(VOID)
{
    ULONG i = 0;
    YMODEM_PRINTF("YMODEM FLASH ADDR:[%lx] \r\n", gpstYmodemParma->ulErrAddr);
    YMODEM_PRINTF("YMODEM FLASH ErrAddrData:[%lx] \r\n", gpstYmodemParma->ulErrAddrData);
    YMODEM_PRINTF("YMODEM FLASH ulCurData:[%lx] \r\n", gpstYmodemParma->ulCurData);

    YMODEM_PRINTF("\r\n");

    /* 读出flash前100位,查看flash是否写成功 */
    for (i = YMODEM_BACK_ADDRESS; i < (YMODEM_BACK_ADDRESS + 1024); i++)
    {
        YMODEM_PRINTF("[%02x] ", REG8(i));
        Ymodem_Wdgt_Reload();
    }
    /* 查看当前包数据 */

    return;
}
#ifdef __cplusplus
}
#endif

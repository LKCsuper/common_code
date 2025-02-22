/*
 * @Description: iap升级
 * @detail: 
 * @Version: 1.0
 * @Date: 2023-06-08 10:39:09
 * @LastEditTime: 2023-11-29 15:06:58
 * @LastEditors: lkc
 */
#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
#define IAP_DELAY_TIM_NUM                                               (0x100000U)       /* 轮询次数,用于延时 */
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 判断bootdelay
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID Iap_BootDelay(VOID)
{
    ULONG i = IAP_DELAY_TIM_NUM;
    UCHAR ulRet = 0;
    UCHAR ucData = 0;

    /* 喂狗 */
    Bsp_Misc_IWdgtReload();
    /* 提示用户 */
    YMODEM_PRINTF("Please Input 'b', Enter Boot\r\n");
    /* 喂狗 */
    Bsp_Misc_IWdgtReload();
    while (i--)
    {
        /* 从fifo中取出一个数据 */
        ulRet = fifo_read(&ringbufRxFifo, &ucData, 1); 
        
        /* 读到数据 */
        if (ulRet > 0)
        {
            YMODEM_PRINTF("%c\r\n", ucData);
            /* 进入boot */
            if ('b' == ucData)
            {
                gpstIapParam->bEnterBoot = TRUE;
                break;
            }
            else if ('\r' == ucData)
            {
                return;
            }
        }

        /* 倒数计数 */
        if ((0 != (i >> 16)) && (0 == (i << 16)))
        {
            YMODEM_PRINTF("\r");
            YMODEM_PRINTF("Bootdelay [%ld]", (i>>16));
        }
    }

    /* 喂狗 */
    Bsp_Misc_IWdgtReload();

    /* 提示是否进入app还是boot */
    if (FALSE == gpstIapParam->bEnterBoot)
    {
        YMODEM_PRINTF("\r\nEnter App Success\r\n");
    }
    else
    {
        YMODEM_PRINTF("\r\nEnter Boot Success\r\n");
    }

    return;
}

/*=========================================YMODEM WEAK ===================================================*/
/**
 * @description: 串口初始化
 * @detail: 
 * @param {uint32_t} ulBound
 * @return {*}
 * @author: lkc
 */
void Ymodem_Uart_Init(uint32_t ulBound)
{
    Bsp_USART_Init(115200);
    return;
}

/**
 * @description: 发送字符
 * @detail: 
 * @param {uint8_t} c
 * @return {*}
 * @author: lkc
 */
void Ymodem_PutChar(uint8_t c)
{
    Bsp_PutChar(c);
    return;
}

/**
 * @description: 获取字符
 * @detail: 
 * @param {uint8_t} *key
 * @return {*}
 * @author: lkc
 */
uint32_t Ymodem_GetChar(uint8_t *key)
{
    ULONG ulGetState = YMODEM_SUCCESS;

    ulGetState = Bsp_GetChar(key);

    return ulGetState;
}

/**
 * @description: ymodemflash初始化
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void Ymodem_Flash_Init(void)
{
    Bsp_FLASH_Init();
    return;
}

/**
 * @description: ymodem写
 * @detail: 
 * @param {uint32_t} FlashAddress
 * @param {uint32_t} Data
 * @return {*}
 * @author: lkc
 */
uint32_t Ymodem_Flash_Write(uint32_t FlashAddress ,uint32_t Data)
{
    Bsp_Flash_Program(FlashAddress, (UCHAR *)&Data, 8);

    return YMODEM_SUCCESS;
}

/**
 * @description: 设置标志位
 * @detail: 
 * @param {bool} isSet
 * @return {*}
 * @author: lkc
 */
void Ymodem_Flash_Set_Flag(bool isSet)
{
    FLASH_Unlock();
    if (isSet)
    {
        Bsp_Flash_Program32(YMODEM_FLAG_ADDR, YMODEM_FLAG);
        /* size */
        Bsp_Flash_Program32(YMODEM_FLAG_SIZE, gpstYmodemParma->ulHasWrite);
        /* crc */
        USHORT usCrc16 = Common_CRC16((uint8_t *)(uint32_t)(APP_BAK_START_ADDR), gpstYmodemParma->ulHasWrite);
        Bsp_Flash_Program32(YMODEM_FLAG_CRC, usCrc16);
    }
    else
    {
        Bsp_Flash_Program32(YMODEM_FLAG_ADDR, 0xffffffff);
    }
    FLASH_Lock();

    return;
}

/**
 * @description: 获取标志位
 * @detail: 
 * @return {*}
 * @author: lkc
 */
uint32_t Ymodem_Flash_Get_Flag(void) 
{
    return REG32(YMODEM_FLAG_ADDR);
}

/**
 * @description: ymodem擦除
 * @detail: 
 * @param {ULONG} ulAddr
 * @param {ULONG} ulSize
 * @return {*}
 * @author: lkc
 */
void Ymodem_Flash_Erase(ULONG ulAddr, ULONG ulSize)
{
    /* 检测，每当4kb才能擦写 */
    ULONG i = 0;
    ULONG ulEraseEnd = ADRESS_ALIGN_UP(ulSize, 4);        /* 4字节对齐 */

    Bsp_Flash_Erase(ulAddr, ulEraseEnd);

    /* 检测擦除是否成功 */
    for (i = YMODEM_UPDATE_ADDR; i < (ulEraseEnd); i += 4)
    {
        /* 一次比较4个字节 */
        if (0xFFFFFFFF != REG32(i))
        {
            YMODEM_ERR("Flash Erase Err. Addr[%lx] [%lx]\r\n", i, REG32(i));
        }
    }

    return;
}

/**
 * @description: 写app
 * @detail: 
 * @param {uint8_t} *Data
 * @param {uint32_t} DataLength
 * @return {*}
 * @author: lkc
 */
uint32_t Ymodem_Flash_Write_App(uint8_t *Data, uint32_t DataLength)
{
    ULONG i = 0;
    /* 一次写8字节 */
    ULONG ulWirteNum = 4;
    ULONG ulWriteEnd = YMODEM_BACK_END_ADDRESS;

    /* 关中断 */
    FLASH_Unlock();
    __set_PRIMASK(1);

    gpstYmodemParma->ulHasWrite += DataLength;

    /* 一次性写256或1k数据 */
    for (i = 0; (i < DataLength) && (gpstYmodemParma->ulWriteAddr <= (ulWriteEnd - 8)); i += ulWirteNum)
    {
        /* 如果写ulWirteNum字节数据成功 */
        if (YMODEM_SUCCESS == Bsp_Flash_Program(gpstYmodemParma->ulWriteAddr, (UCHAR *)(Data), ulWirteNum))
        {
            /* 写入后???出数据 判断数据???不是相等 */
            if (REG32(gpstYmodemParma->ulWriteAddr) != *(uint32_t *)(Data))
            {
                gpstYmodemParma->ulErrAddr = gpstYmodemParma->ulWriteAddr;
                gpstYmodemParma->ulErrAddrData = REG32(gpstYmodemParma->ulWriteAddr);
                gpstYmodemParma->ulCurData = *(uint32_t *)(Data);
                /* Flash content doesn't match SRAM content */
                YMODEM_ERR("Wirte After Read Addr[%lx] Err\n", gpstYmodemParma->ulWriteAddr);
                return YMODEM_ERROR;
            }
            /* Increment FLASH destination address */
            gpstYmodemParma->ulWriteAddr += ulWirteNum;
            Data += ulWirteNum;
        }
        else
        {
            /* 写flash错误 */
            return YMODEM_ERROR;
        }
    }

    /* 开中断 */
    __set_PRIMASK(0);
    FLASH_Lock();

    return YMODEM_SUCCESS;
}

/**
 * @description: 拷贝app
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void Ymodem_Flash_CopyApp(VOID)
{
    /* 4字节对齐 */
    uint32_t updataSize = ADRESS_ALIGN_UP(gpstYmodemParma->ulHasWrite, 4);
    uint32_t copyAddr = YMODEM_BACK_ADDRESS;                                    /* 拷贝地址 */
    uint32_t updateAddr = YMODEM_UPDATE_ADDR;                                   /* 更新地址 */
    uint32_t updateEndAddr = YMODEM_UPDATE_ADDR + updataSize;                   /* 更新结束地址 */
    ULONG ulWirteNum = 4;                                                       /* 一次性写多大 */
    ULONG i = 0;

    /* 关中断 */
    FLASH_Unlock();
    __set_PRIMASK(1);

    for (i = updateAddr; i < updateEndAddr; i += ulWirteNum, copyAddr += ulWirteNum)
    {
        Bsp_Flash_Program32(i, REG32(copyAddr));
    }
    
    /* 开中断 */
    __set_PRIMASK(0);
    FLASH_Lock();

    return;
}

/**
 * @description: 看门狗复位
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void Ymodem_Wdgt_Reload(VOID)
{
    Bsp_Misc_IWdgtReload();
    return;
}
/*=========================================YMODEM===================================================*/
#ifdef __cplusplus
}
#endif

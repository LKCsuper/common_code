/*
 * @Description: 
 * @Version: 2.0
 * @Author: lkc
 * @Date: 2022-11-28 19:28:49
 * @LastEditors: lkc
 * @LastEditTime: 2023-11-29 15:09:48
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __YMODEM_CONFIG_H_
#define __YMODEM_CONFIG_H_
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* flash分区 boot app appbak data */
#define     YMODEM_BACK_SIZE                                        APP_BAK_SIZE
#define     YMODEM_BACK_ADDRESS                                     APP_BAK_START_ADDR
#define     YMODEM_BACK_END_ADDRESS                                 APP_BAK_END_ADDR

#define     YMODEM_UPDATE_SIZE                                      APP_SIZE
#define     YMODEM_UPDATE_ADDR                                      APP_START_ADDR
#define     YMODEM_UPDATE_END_ADDR                                  APP_END_ADDR

#define     YMODEM_OFFSET                                           APP_OFFSET

#define     YMODEM_SLEEP_MS(ms)                                     do{Bsp_Delay_MS(ms);}while(0);
/* 标志位默认写在app区的最后一位,这就要求擦除的话就全部擦除 */
#define     YMODEM_FLAG_ADDR                                        (YMODEM_UPDATE_END_ADDR - 4)
/* 升级大小 */
#define     YMODEM_FLAG_SIZE                                        (YMODEM_FLAG_ADDR - 4)
/* 升级crc */
#define     YMODEM_FLAG_CRC                                         (YMODEM_FLAG_SIZE - 4)

/* flash升级标志 */
#define     YMODEM_FLAG                                             (UPDATE_FLAG)
/* 0擦除实际大小 1擦除整个备份区 */
#define     YMODEM_ERASE_MIN                                        (0)

/* 跳转函数位置 */
typedef void (*pFunction)(void);

/* 延时时间,用于不停的发c */
#define     YMODEM_DELAY_MS                 (0)
/* ymodem 发送c的???数 */
#define     YMODEM_C_NUM                    (30)
/* 超时时间，while?????? */
#define     YMODEM_TIMEOUT                  (0x100000)
/* ymodem打印 */
#define     YMODEM_DEBUG                    (1)
/* 文件名字字???串长度 */
#define     FILE_NAME_LENGTH                (32)
/* 文件大小字???串长度 */
#define     FILE_SIZE_LENGTH                (16)

/* 调试打印 */
#if YMODEM_DEBUG
#define YMODEM_PRINTF(format, ...)          PRINTF(format, ##__VA_ARGS__)
#define YMODEM_NOTICE(format, ...)          NOTICE(format, ##__VA_ARGS__)
#define YMODEM_ERR(format, ...)             ERR(format, ##__VA_ARGS__)
#define YMODEM_INFO(format, ...)            INFO(format, ##__VA_ARGS__)
#define YMODEM_WARNING(format, ...)         WARNING(format, ##__VA_ARGS__)
#else
#define YMODEM_PRINTF(format, ...)  
#define YMODEM_ERR(format, ...)
#define YMODEM_INFO(format, ...)
#define YMODEM_WARNING(format, ...)
#endif

/* 弱定??? */
#ifdef __CC_ARM /* ARM Compiler */
#define WEAK __weak
#elif defined(__IAR_SYSTEMS_ICC__) /* for IAR Compiler */
#define WEAK __weak
#elif defined(__GNUC__) /* GNU GCC Compiler */
#define WEAK __attribute__((weak))
#elif defined(__ADSPBLACKFIN__) /* for VisualDSP++ Compiler */
#define WEAK __attribute__((weak))
#elif defined(_MSC_VER)
#define WEAK
#elif defined(__TI_COMPILER_VERSION__)
#define WEAK
#else
#error not supported tool chain
#endif

/* ymodem 结果 */
typedef enum tagYmodemRes
{
    YMODEM_ABORT_T = -6,        /* 传输出现err,主动终???ca ca */
    YMODEM_NO_ACK = -5,         /* 发c??,没有ack回??? */
    YMODEM_FLASH_ERR = -4,      /* flash 擦写失败 */
    YMODEM_OVER_SIZE = -3,      /* 文件大小超大 */
    YMODEM_ABORT = -2,          /* ??发送者终??,然后主动发ca */
    YMODEM_ERR = -1,            /* 错????? 空帧 */
    YMODEM_DATA_SUCCESS = 0,            /* 数据处理??*/
}YMODEM_RES_E;

/* ymodem参数 */
typedef struct tagYmodemParam
{
    ULONG ulPackSize;                   /* 传输协???数据包大?? */
    UCHAR ucEotNum;                     /* 传输到最??,应???是两??? */
    UCHAR ucCaNum;                      /* 终???标??,一????上位机发?? */
    ULONG ulSohNum;                     /* 256 package num*/
    ULONG ulStxNum;                     /* 1k package num */

    UCHAR ucErrNum;                     /* 错???次数 */
    ULONG ulPackNum;                    /* 数据包个?? */
    ULONG ulFileSize;                  /* 文件大小 */

    UCHAR ucFileDone;
    UCHAR ucSessionDone;
    UCHAR ucSessionBegin;

    ULONG ulWriteAddr;
    ULONG ulHasWrite;                   /* 已经写了多少 */
    ULONG ulErrAddrData;
    ULONG ulCurData;
    ULONG ulErrAddr;

    UCHAR ucIsUpdate;                   /* 是否ymodem升级 */
}YMODEM_PARAM_S;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern YMODEM_PARAM_S gstYmodemParma;
extern YMODEM_PARAM_S *gpstYmodemParma;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#if 0
WEAK void Ymodem_Uart_Init(uint32_t ulBound);
WEAK uint32_t Ymodem_GetChar(uint8_t *key);
WEAK void Ymodem_PutChar(uint8_t c);
WEAK void Ymodem_Flash_Set_Flag(bool isSet);
WEAK uint32_t Ymodem_Flash_Get_Flag(void);
WEAK void Ymodem_Flash_Init(void);
WEAK uint32_t Ymodem_Flash_Write(uint32_t FlashAddress ,uint32_t Data);
WEAK uint32_t Ymodem_Flash_Write_App(uint32_t *Data, uint32_t DataLength);
WEAK void Ymodem_Wdgt_Reload(VOID);

void Ymodem_Info(void);
void Ymodem_Jump_App(void);
void Ymodem_Debug(void);
#endif
#ifdef __cplusplus
};
#endif
#endif  /* __YMODEM_H_ */
/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

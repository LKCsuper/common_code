/*
 * @Description: Shell
 * @detail: 
 * @Version: 1.0
 * @Date: 2023-06-08 10:08:18
 * @LastEditTime: 2023-11-29 15:36:14
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
#define PROMPT_IMAGE                                                    "Image=>"
#define PROMPT_BOOT                                                     "Boot=>"
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UCHAR gaucPrompt[10] = {0};                                         /* 提示 */
_fifo_t ringbufRxFifo;
_fifo_t ringbufTxFifo;
uint8_t gaucFifoRxBuf[RINGBUFF_RX_LEN];
uint8_t gaucFifoTxBuf[RINGBUFF_RX_LEN];

/* 命令 回调 help */
cmd_t gastCmd[] =
{
    {"image",       "\r\nhelp\r\n", 0, Shell_Image},
    {"download",    "help\r\n", 0, Shell_Image_Download},
    {"reset",       "help\r\n", 0, Shell_Reset},
    {"bootz",       "help\r\n", 0, Shell_Bootz},
    {"debug",       "help\r\n", 0, Shell_Debug},
    {"exit",        "help\r\n", 0, Shell_Exit},
    {"help",        "help\r\n", 0, Shel_List},
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 切换image
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void Shell_Image(void)
{
    /* 切换Image权限 */
    memcpy(gaucPrompt, PROMPT_IMAGE, sizeof(PROMPT_IMAGE));

    return;
}

/**
 * @description: 退出命令行
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void Shell_Exit(void)
{
    gpstIapParam->bExit = TRUE;

    return;
}

/**
 * @description: 开始下载程序
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID Shell_Image_Download(VOID)
{
    LONG lRet = 0;

    /* 匹配权限 */
    if (0 != strncmp((const char *)gaucPrompt, PROMPT_IMAGE, sizeof(PROMPT_IMAGE)))
    {
        YMODEM_PRINTF("\r\nYou Have No Permission \r\n");
        return;
    }

    YMODEM_PRINTF("\r\nDownload Start.\r\n");

    /* 关闭串口中断 */
    //NVIC_DisableIRQ(USART3_IRQn);

    /* ymodem升级,拷贝到制定区域 */
    lRet = Ymodem_Receive(NULL);

    /* 开启串口中断 */
    //NVIC_EnableIRQ(USART3_IRQn);
    Ymodem_Wdgt_Reload();

    if (lRet > 0)
    {
        Ymodem_ShowFileInfo();
        /* 代表ymodem升级完成 写入flash标志 */
        Ymodem_Flash_Set_Flag(TRUE);
        gpstIapParam->bYomdemUpdate = TRUE;
        YMODEM_NOTICE("Ymodem Update Success\r\n");
        /* 设备重启 */
        Bsp_Misc_Reset();
    }
    else if (lRet == ERROR_NO_ACK)
    {
        YMODEM_PRINTF("\r\nYMODEM: Ymodem is not in use \r\n");
    }

    return;
}

/**
 * @description: 重启
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID Shell_Reset(VOID)
{
    /* 切换Image权限 */
    Bsp_Misc_Reset();

    return;
}

/**
 * @description: 跳转app
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID Shell_Bootz(VOID)
{
    /* 跳转 */
    Bsp_Misc_Jump(YMODEM_UPDATE_ADDR);

    return;
}

/**
 * @description: 跳转app
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID Shell_Debug(VOID)
{
    /* 显示调试信息 */
    Ymodem_Debug();

    return;
}

/**
 * @description: 列出命令信息
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID Shel_List(VOID)
{
    for (int i = 0; i < sizeof(gastCmd) / sizeof(cmd_t); i++) {
        printf("%s -->%s\n", gastCmd[i].cmd, gastCmd[i].cmd_mess);
    }

    return;
}

/**
 * @description: Shell 处理
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID Shell_Process(VOID)
{
    UCHAR ucData = 0;
    ULONG ulDataLen = 0;
    UCHAR aucData[RINGBUFF_RX_LEN];

    /* 终端提示用户 */
    memcpy(gaucPrompt, PROMPT_BOOT, sizeof(PROMPT_BOOT));
    /* 打印默认到终端 */
    YMODEM_PRINTF("%s", gaucPrompt);
    cmd_init(gastCmd, sizeof(gastCmd) / sizeof(cmd_t));

    while (1)
    {
        if (SUCCESS == fifo_read(&ringbufRxFifo, &ucData, 1))
        {
            if ((ASCALL_BACK_SPACE == ucData))
            {
                if (ulDataLen > 0)
                {
                    ulDataLen--;
                    aucData[ulDataLen] = '\0';
                    YMODEM_PRINTF(OUTPUT_BACK_SPACE);
                }
                continue;
            }

            /* 如果有回车，上层回车是\r */
            if ('\r' == ucData)
            {
                aucData[ulDataLen] = '\0';
                /* 处理命令 */
                cmd_parsing((char *)aucData);
                memset(aucData, 0, sizeof(aucData));
                ulDataLen = 0;
                YMODEM_PRINTF("\r\n%s", gaucPrompt);
                continue;
            }

            /* 回显,并且保存 */
            YMODEM_PRINTF("%c", ucData);
            aucData[ulDataLen] = ucData;
            ulDataLen++;
        }

        if (TRUE == gpstIapParam->bExit)
        {
            return;
        }

        /* 喂狗 */
        Ymodem_Wdgt_Reload();
    }
}

/**
 * @description: shell 初始化
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID Shell_Init(VOID)
{
    fifo_register(&ringbufRxFifo, gaucFifoRxBuf, sizeof(gaucFifoRxBuf), NULL, NULL);
    fifo_register(&ringbufTxFifo, gaucFifoTxBuf, sizeof(gaucFifoTxBuf), NULL, NULL);
    return;
}
#ifdef __cplusplus
}
#endif

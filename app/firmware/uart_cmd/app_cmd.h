/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
#ifndef __APP_CMD_H__
#define __APP_CMD_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* 退格键 */
#define ASCALL_BACK_SPACE (0x08U)
#define OUTPUT_BACK_SPACE ("\b \b")
#define ASCALL_ENTER      ('\r')
#define PROMPT_USER       "ENV=>"
#define SHELL_RX_LEN      (256)
#define FIFO_RX_BUF_SIZE  256
#define FIFO_RX1_BUF_SIZE  256
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern _fifo_t stRxFifo;
extern UCHAR aucRx2FifoBuf[FIFO_RX_BUF_SIZE];
extern UCHAR aucRx1FifoBuf[FIFO_RX_BUF_SIZE];
extern fifo_t stFifo;
extern fifo_t stFifo1;
extern cmd_t gastCmd[]; 
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
VOID Cmd_Pro(VOID);
VOID CMD_Init(VOID);
VOID Shell_Reset(VOID);
VOID Shell_SetVoltage(VOID);
VOID Shell_SetTmper(VOID);
VOID Shell_List(VOID);
VOID Shell_Show(VOID);
VOID Shell_SetTrigger(VOID);
VOID Shell_ResetParam(VOID);
VOID Shell_TmperEn(VOID);
VOID Shell_Log(VOID);
VOID Shell_LogInit(VOID);
VOID SHELL_ConfigGet(VOID);
VOID SHELL_ConfigSet(VOID);
VOID SHELL_TimSet(VOID);
VOID SHELL_UsrAdd(VOID);
VOID SHELL_UsrLogin(VOID);
VOID SHELL_UsrOut(VOID);
VOID SHELL_GenRandom(VOID);
VOID SHELL_ShowPub(VOID);
VOID SHELL_Boot(VOID);
#ifdef __cplusplus
}
#endif
#endif

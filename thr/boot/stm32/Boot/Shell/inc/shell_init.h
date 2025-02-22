/*
 * @Description: Shell
 * @detail: 
 * @Version: 1.0
 * @Date: 2023-06-08 10:08:30
 * @LastEditTime: 2023-10-13 15:57:02
 * @LastEditors: lkc
 */
#ifndef _SHELL_INIT_H_
#define _SHELL_INIT_H_
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* 退格键 */
#define ASCALL_BACK_SPACE                                                   (0x08U)
#define OUTPUT_BACK_SPACE                                                   ("\b \b")
#define ASCALL_ENTER                                                        ('\r')
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern _fifo_t ringbufRxFifo;
extern _fifo_t ringbufTxFifo;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
VOID Shell_Process(VOID);
VOID Shell_Init(VOID);
VOID Shel_List(VOID);
void Shell_Image(void);
VOID Shell_Image_Download(VOID);
VOID Shell_Reset(VOID);
VOID Shell_Bootz(VOID);
VOID Shell_Debug(VOID);
VOID Shell_Exit(VOID);
#endif

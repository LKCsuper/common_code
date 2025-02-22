/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
/*
 * @Description: 
 * @Date: 2022-11-28 19:28:49
 * @LastEditTime: 2023-10-13 15:52:43
 * @FilePath: \YMODEM\ThirtyParty\ymodem\ymodem.h
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __YMODEM_H_
#define __YMODEM_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum tagYmodemFlag
{
	YMODEM_RESET = 0,
	YMODEM_SET = !YMODEM_RESET
}YMODEM_FLAG_E;

typedef enum tagYmodemState
{
	YMODEM_DISABLE = 0,
	YMODEM_ENABLE = !YMODEM_DISABLE
}YMODEM_STATE_E;

typedef enum
{
	YMODEM_ERROR = 0,
	YMODEM_SUCCESS = !YMODEM_ERROR
}
YMODEM_ERROR_STATUS_E;

/* Private define ------------------------------------------------------------*/
#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define SOH                     (0x01)  /* start of 128-byte data packet */
#define STX                     (0x02)  /* start of 1024-byte data packet */
#define EOT                     (0x04)  /* end of transmission */
#define ACK                     (0x06)  /* acknowledge */
#define NAK                     (0x15)  /* negative acknowledge */
#define CA                      (0x18)  /* two of these in succession aborts transfer */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC */
#define end1                    (0x62) 
#define end2                    (0x72) 

#define END						('O')

#define ABORT1                  (0x41)  /* 'A' == 0x41, abort by user */
#define ABORT2                  (0x61)  /* 'a' == 0x61, abort by user */

#define MAX_ERRORS              (5)

#define ERROR_NO_ACK            (-5)	/* 规定次数ymodem没有回应,返回没有回应错误 */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int32_t Ymodem_Receive(uint8_t*);
uint8_t Ymodem_Transmit(uint8_t*, const  uint8_t*, uint32_t);
int32_t Receive_Byte(uint8_t* c, uint32_t timeout);
uint32_t Send_Byte(uint8_t c);
/*===============================WEAK============================================*/
void Ymodem_Uart_Init(uint32_t ulBound);
void Ymodem_PutChar(uint8_t c);
uint32_t Ymodem_GetChar(uint8_t *key);
void Ymodem_Flash_Init(void);
uint32_t Ymodem_Flash_Write(uint32_t FlashAddress ,uint32_t Data);
void Ymodem_Flash_Set_Flag(bool isSet);
uint32_t Ymodem_Flash_Get_Flag(void);
void Ymodem_Flash_Erase(ULONG ulAddr, ULONG ulSize);
uint32_t Ymodem_Flash_Write_App(uint8_t *Data, uint32_t DataLength);
void Ymodem_Flash_CopyApp(void);
void Ymodem_Wdgt_Reload(void);
void Ymodem_Jump_App(void);
/*===============================WEAK============================================*/
void Ymodem_ShowFileInfo(void);
void Ymodem_Info(void);
VOID Ymodem_Debug(VOID);
#ifdef __cplusplus
};
#endif
#endif  /* __YMODEM_H_ */

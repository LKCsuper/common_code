/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void rt_hw_console_output(const char *str)
{
    rt_enter_critical();

    while (*str!='\0')
    {
        if (*str=='\n')
        {
            USART_SendData(USART3, '\r');
            while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
        }

        USART_SendData(USART3, *str++);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    }
    rt_exit_critical();
}

int fputc(int ch, FILE *f)
{
    rt_enter_critical();
    char cStr = (char)ch; 

    if (cStr == '\n')  
    {
        USART_SendData(USART3, (uint8_t)'\r');
        while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TC));
    }
    USART_SendData(USART3, (uint8_t)cStr);
    while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TC));
    rt_exit_critical();
    return ch;
}

#include <LowLevelIOInterface.h>
#include "Board.h"
#pragma module_name = "?__write"
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    return size;

    /*
     * This means that we should flush internal buffers.  Since we
     * don't we just return.  (Remember, "handle" == -1 means that all
     * handles should be flushed.)
    */
    if (buffer == 0)
    {
        return 0;
    }

    /* This template only writes to "standard out" and "standard err",
    * for all other file handles it returns failure. */
    if (handle != _LLIO_STDOUT && handle != _LLIO_STDERR)
    {
        return _LLIO_ERROR;
    }

    rt_enter_critical();
    for(int i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')  
        {
            USART_SendData(USART3, (uint8_t)'\r');
            while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TC));
        }
        USART_SendData(USART3, (uint8_t)buffer[i]);
        while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TC));
    }
    rt_exit_critical();
    return size;
}

// IAR
/**
 * @description: 重定向(IAR)
 * @detail: 
 * @param {int} handle
 * @param {unsigned char *} buffer
 * @param {size_t} size
 * @return {*}
 * @author: lkc
 */
size_t __write(int handle, const unsigned char * buffer, size_t size)
{  
	for (int i =0; i < size; i++)
	{
	    USART_SendData(EVAL_COM1, (u8)buffer[i]);
        while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);
	}
	
	return size;
}
#ifdef __cplusplus
}
#endif
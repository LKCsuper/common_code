/*
 * @Description: 杂项外设
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-10-12 10:06:27
 * @LastEditors: lkc
 * @LastEditTime: 2023-10-13 15:45:24
 * @detail: 
 */
#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  SYS_CLOCK                                              SystemCoreClock
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
volatile uint32_t time_delay; // 延时时间，注意定义为全局变量

/**
 * @description: 延时ms
 * @event: 
 * @param {volatile unsigned long} nms
 * @return {*}
 */
void Bsp_Delay_MS(volatile uint32_t nms)
{
    if (SysTick_Config(SYS_CLOCK / 1000))
    {
        while (1);
    }
    time_delay = nms; // 读取定时时间
    while (time_delay);
    SysTick->CTRL = 0x00; // 关闭计数
    SysTick->VAL = 0X00;  // 清空计数
}

/**
 * @description: 延时us
 * @detail description: 
 * @param {volatile uint32_t} nus
 * @return {*}
 */
void Bsp_Delay_US(volatile uint32_t nus)
{
    if (SysTick_Config(SYS_CLOCK / 1000000))
    {
        while (1);
    }
    time_delay = nus; // 读取定时时间
    while (time_delay){ };
    SysTick->CTRL = 0x00; // 关闭计数
    SysTick->VAL = 0X00;  // 清空计数
}

/**
 * @description: systick中断
 * @detail description: 实现变量递减
 * @return {*}
 */
void SysTick_Handler(void)
{
    if (time_delay)
        time_delay--;
}

/**
 * @description: 看门狗初始化
 * @detail description: 
 * @return {*}
 */
void Bsp_Misc_Iwdg(UCHAR prv ,USHORT rlv)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(prv);
    IWDG_SetReload(rlv);
    IWDG_ReloadCounter();
    IWDG_Enable(); 

    return;
}

/**
 * @description: 看门狗喂??
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void Bsp_Misc_IWdgtReload(VOID)
{   
    IWDG_ReloadCounter();
    return;
}

/**
 * @description: 设???重??
 * @detail: 
 * @return {*}
 * @author: lkc
 */
void Bsp_Misc_Reset(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}

/**
 * @description: 跳转
 * @detail: 
 * @param {IN ULONG} ulAddr
 * @return {*}
 * @author: lkc
 */
pFunction Jump_To_Application;
uint32_t JumpAddress;
VOID Bsp_Misc_Jump(IN ULONG ulAddr)
{
    YMODEM_PRINTF("YMODEM : Start Jump to App.[%lx] \r\n", ulAddr);

    JumpAddress = *(__IO uint32_t *)(ulAddr + 4);
    /* Jump to user application */
    Jump_To_Application = (pFunction)JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t *)ulAddr);

    /* 关闭所有中断，避免跳转期间被打断 */
    /* !NOTICE 注意看门狗，就算跳转app也不能关，除非重新上电 */
    __disable_irq();
    /* !NOTICE重新设置向量表,因为RTT上电不会先进入自己的main */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, YMODEM_OFFSET);
    Jump_To_Application();

    YMODEM_ERR("Jump Err.\r\n");

    return;
}
#ifdef __cplusplus
};
#endif

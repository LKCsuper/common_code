/*
 * @Description:
 * @detail:
 * @Version: 1.0
 * @Date: 2023-06-28 14:00:27
 * @LastEditTime: 2024-08-05 14:28:59
 * @LastEditors: Please set LastEditors
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro --------------------------------- ----------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: gps数据 uart1 -> dma
 * @detail: p
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Dma0_GpsInit(VOID)
{
    dma_parameter_struct dma_init_struct; /* DMA配置参数结构体 */
    rcu_periph_clock_enable(GPS_DMA_CLK); /* 使能 DMA0 时钟 */

    /* USART0 DMA 接收配置*/
    dma_deinit(GPS_DMA, GPS_DMA_CH);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;    /* 外设到内存 */
    dma_init_struct.memory_addr = (uint32_t)gaucGpsDmaBuf;   /* 设置内存接收基地址 */
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE; /* 内存地址递增 */
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;    /* 8位内存数据 */
    dma_init_struct.number = sizeof(gaucGpsDmaBuf);
    dma_init_struct.periph_addr = ((uint32_t)(GPS_COM + 0x00000004U)); /* 外设基地址,USART数据寄存器地址 */
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;          /* 外设地址不递增 */
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;          /* 8位外设数据 */
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;                /* 最高DMA通道优先级 */
    dma_init(GPS_DMA, GPS_DMA_CH, &dma_init_struct);                   /* 按照结构体的配置初始化DMA */
    dma_circulation_disable(GPS_DMA, GPS_DMA_CH);                      /* 关闭DMA循环模式 */
    dma_memory_to_memory_disable(GPS_DMA, GPS_DMA_CH);                 /* DMA内存到内存模式不开启 */
    dma_channel_enable(GPS_DMA, GPS_DMA_CH);                           /* 使能DMA传输 */

    usart_dma_receive_config(GPS_COM, USART_RECEIVE_DMA_ENABLE); /* USART0 DMA接收模式开启 */

    PRINTF("Bsp Dma Init\r\n");

    return;
}

/**
 * @description: gps数据 uart3 -> dma
 * @detail: p
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Dma0_L716Init(VOID)
{
    dma_parameter_struct dma_init_struct;  /* DMA配置参数结构体 */
    rcu_periph_clock_enable(L716_DMA_CLK); /* 使能 DMA0 时钟 */

    /* USART0 DMA 接收配置*/
    dma_deinit(L716_DMA, L716_DMA_CH);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;    /* 外设到内存 */
    dma_init_struct.memory_addr = (uint32_t)gaucL716DmaBuf;  /* 设置内存接收基地址 */
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE; /* 内存地址递增 */
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;    /* 8位内存数据 */
    dma_init_struct.number = sizeof(gaucL716DmaBuf);
    dma_init_struct.periph_addr = ((uint32_t)(L716_COM + 0x00000004U)); /* 外设基地址,USART数据寄存器地址 */
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;           /* 外设地址不递增 */
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;           /* 8位外设数据 */
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;                 /* 最高DMA通道优先级 */
    dma_init(L716_DMA, L716_DMA_CH, &dma_init_struct);                  /* 按照结构体的配置初始化DMA */
    dma_circulation_disable(L716_DMA, L716_DMA_CH);                     /* 关闭DMA循环模式 */
    dma_memory_to_memory_disable(L716_DMA, L716_DMA_CH);                /* DMA内存到内存模式不开启 */
    dma_channel_enable(L716_DMA, L716_DMA_CH);                          /* 使能DMA传输 */

    usart_dma_receive_config(L716_COM, USART_RECEIVE_DMA_ENABLE); /* USART0 DMA接收模式开启 */

    PRINTF("Bsp Dma0 L716 Init\r\n");

    return;
}

/**
 * @description: 核心板数据 uart3 -> dma
 * @detail: p
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Dma1_MbInit(VOID)
{
    dma_parameter_struct dma_init_struct; /* DMA配置参数结构体 */
    rcu_periph_clock_enable(MB_DMA_CLK);  /* 使能 DMA0 时钟 */

    /* USART0 DMA 接收配置*/
    dma_deinit(MB_DMA, MB_DMA_CH);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;    /* 外设到内存 */
    dma_init_struct.memory_addr = (uint32_t)gaucMbDmaBuf;    /* 设置内存接收基地址 */
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE; /* 内存地址递增 */
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;    /* 8位内存数据 */
    dma_init_struct.number = sizeof(gaucMbDmaBuf);
    dma_init_struct.periph_addr = ((uint32_t)(MB_COM + 0x00000004U)); /* 外设基地址,USART数据寄存器地址 */
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;         /* 外设地址不递增 */
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;         /* 8位外设数据 */
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;               /* 最高DMA通道优先级 */
    dma_init(MB_DMA, MB_DMA_CH, &dma_init_struct);                    /* 按照结构体的配置初始化DMA */
    dma_circulation_disable(MB_DMA, MB_DMA_CH);                       /* 关闭DMA循环模式 */
    dma_memory_to_memory_disable(MB_DMA, MB_DMA_CH);                  /* DMA内存到内存模式不开启 */
    dma_channel_enable(MB_DMA, MB_DMA_CH);                            /* 使能DMA传输 */

    usart_dma_receive_config(MB_COM, USART_RECEIVE_DMA_ENABLE); /* USART0 DMA接收模式开启 */

    PRINTF("Bsp Dma1 Mb Init\r\n");

    return;
}

/**
 * @description: Dma初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
int Bsp_Dma_Init(VOID)
{
    Bsp_Dma0_GpsInit();
    Bsp_Dma0_L716Init();
    Bsp_Dma1_MbInit();

    return 0;
}

INIT_BOARD_EXPORT(Bsp_Dma_Init);
#ifdef __cplusplus
}
#endif

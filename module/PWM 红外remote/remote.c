/**
 ****************************************************************************************************
 * @file        remote.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-27
 * @brief       红外遥控解码 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211027
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/REMOTE/remote.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"


TIM_HandleTypeDef g_tim1_handle;      /* 定时器4句柄 */
TIM_IC_InitTypeDef tim_ic_init_handle;

/**
 * @brief       红外遥控初始化
 *   @note      设置IO以及定时器的输入捕获
 * @param       无
 * @retval      无
 */
void remote_init(void)
{
    g_tim1_handle.Instance = REMOTE_IN_TIMX;                    /* 通用定时器1 */
    g_tim1_handle.Init.Prescaler = 168 - 1;                     /* 预分频器,1M的计数频率,1us加1 */
    g_tim1_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* 向上计数器 */
    g_tim1_handle.Init.Period = 10000;                          /* 自动装载值 */
    HAL_TIM_IC_Init(&g_tim1_handle);
    
    /* 初始化TIM4输入捕获参数 */
    tim_ic_init_handle.ICPolarity = TIM_ICPOLARITY_RISING;      /* 上升沿捕获 */
    tim_ic_init_handle.ICSelection = TIM_ICSELECTION_DIRECTTI;  /* 映射到TI1上 */
    tim_ic_init_handle.ICFilter = 0x03;                         /* IC1F=0003 8个定时器时钟周期滤波 */
    HAL_TIM_IC_ConfigChannel(&g_tim1_handle, &tim_ic_init_handle, REMOTE_IN_TIMX_CHY);      /* 配置TIM1通道1 */
    HAL_TIM_IC_Start_IT(&g_tim1_handle, REMOTE_IN_TIMX_CHY);    /* 开始捕获TIM的通道值 */
    __HAL_TIM_ENABLE_IT(&g_tim1_handle, TIM_IT_UPDATE);         /* 使能更新中断 */
}

/**
 * @brief       定时器4底层驱动，时钟使能，引脚配置
 * @param       htim:定时器句柄
 * @note        此函数会被HAL_TIM_IC_Init()调用
 * @retval      无
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == REMOTE_IN_TIMX)
    {
        GPIO_InitTypeDef gpio_init_struct;
        
        REMOTE_IN_GPIO_CLK_ENABLE();                            /* 红外接入引脚GPIO时钟使能 */
        REMOTE_IN_TIMX_CHY_CLK_ENABLE();                        /* 定时器时钟使能 */
        
        gpio_init_struct.Pin = REMOTE_IN_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 复用输入 */
        gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
        gpio_init_struct.Alternate = REMOTE_IN_GPIO_AF;         /* PA8 复用为TIM1_CH1 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
        HAL_GPIO_Init(REMOTE_IN_GPIO_PORT, &gpio_init_struct);  /* 初始化定时器通道引脚 */

        HAL_NVIC_SetPriority(REMOTE_IN_TIMX_IRQn, 0, 3);        /* 设置中断优先级，抢占优先级1，子优先级3 */
        HAL_NVIC_EnableIRQ(REMOTE_IN_TIMX_IRQn);                /* 开启ITM1中断 */
        
        /* 如果是通用定时器 ,不需要设置CC中断 */
        HAL_NVIC_SetPriority(REMOTE_IN_TIMX_CC_IRQn, 0, 2);     /* 设置中断优先级，抢占优先级1，子优先级2 */
        HAL_NVIC_EnableIRQ(REMOTE_IN_TIMX_CC_IRQn);             /* 开启ITM1中断 */
    }
}

/* 遥控器接收状态
 * [7]  : 收到了引导码标志
 * [6]  : 得到了一个按键的所有信息
 * [5]  : 保留
 * [4]  : 标记上升沿是否已经被捕获
 * [3:0]: 溢出计时器
 */
uint8_t g_remote_sta = 0;
uint32_t g_remote_data = 0; /* 红外接收到的数据 */
uint8_t g_remote_cnt = 0;   /* 按键按下的次数 */

/**
 * @brief       定时器溢出中断服务函数
 * @param       无
 * @retval      无
 */
void REMOTE_IN_TIMX_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim1_handle); /* 定时器共用处理函数 */
}

/**
 * @brief       定时器输入捕获中断服务函数
 * @param       无
 * @retval      无
 */
void REMOTE_IN_TIMX_CC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim1_handle); /* 定时器共用处理函数 */
}

/**
 * @brief       定时器溢出中断服务函数
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == REMOTE_IN_TIMX)
    {
        if (g_remote_sta & 0x80)                /* 上次有数据被接收到了 */
        {
            g_remote_sta &= ~0x10;              /* 取消上升沿已经被捕获标记 */

            if ((g_remote_sta & 0x0F) == 0x00)
            {
                g_remote_sta |= 1 << 6;         /* 标记已经完成一次按键的键值信息采集 */
            }
            
            if ((g_remote_sta & 0x0F) < 14)
            {
                g_remote_sta++;
            }
            else
            {
                g_remote_sta &= ~(1 << 7);      /* 清空引导标识 */
                g_remote_sta &= 0xF0;           /* 清空计数器 */
            }
        }
    }
}

/**
 * @brief       定时器输入捕获中断回调函数
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == REMOTE_IN_TIMX)
    {
        uint16_t dval;  /* 下降沿时计数器的值 */
        
        if (RDATA)      /* 上升沿捕获 */
        {
            __HAL_TIM_SET_CAPTUREPOLARITY(&g_tim1_handle, REMOTE_IN_TIMX_CHY, TIM_INPUTCHANNELPOLARITY_FALLING);    /* CC4P=1 设置为下降沿捕获 */
            __HAL_TIM_SET_COUNTER(&g_tim1_handle, 0);       /* 清空定时器值 */
            g_remote_sta |= 0x10;                           /* 标记上升沿已经被捕获 */
        }
        else           /* 下降沿捕获 */
        {
            dval=HAL_TIM_ReadCapturedValue(&g_tim1_handle, REMOTE_IN_TIMX_CHY);         /* 读取CCR1也可以清CC1IF标志位 */
            __HAL_TIM_SET_CAPTUREPOLARITY(&g_tim1_handle, REMOTE_IN_TIMX_CHY, TIM_INPUTCHANNELPOLARITY_RISING);     /* 配置TIM4通道4上升沿捕获 */

            if (g_remote_sta & 0x10)                        /* 完成一次高电平捕获 */
            {
                if (g_remote_sta & 0x80)                    /* 接收到了引导码 */
                {

                    if (dval > 300 && dval < 800)           /* 560为标准值,560us */
                    {
                        g_remote_data >>= 1;                /* 右移一位 */
                        g_remote_data &= ~(0x80000000);     /* 接收到0 */
                    }
                    else if (dval > 1400 && dval < 1800)    /* 1680为标准值,1680us */
                    {
                        g_remote_data >>= 1;                /* 右移一位 */
                        g_remote_data |= 0x80000000;        /* 接收到1 */
                    }
                    else if (dval > 2000 && dval < 3000)    /* 得到按键键值增加的信息 2250为标准值2.25ms */
                    {
                        g_remote_cnt++;                     /* 按键次数增加1次 */
                        g_remote_sta &= 0xF0;               /* 清空计时器 */
                    }
                }
                else if (dval > 4200 && dval < 4700)        /* 4500为标准值4.5ms */
                {
                    g_remote_sta |= 1 << 7;                 /* 标记成功接收到了引导码 */
                    g_remote_cnt = 0;                       /* 清除按键次数计数器 */
                }
            }

            g_remote_sta &= ~(1 << 4);
        }
    }
}

/**
 * @brief       处理红外按键(类似按键扫描)
 * @param       无
 * @retval      0   , 没有任何按键按下
 *              其他, 按下的按键键值
 */
uint8_t remote_scan(void)
{
    static uint8_t sta = 0;
    uint8_t t1, t2;

    if (g_remote_sta & (1 << 6))                        /* 得到一个按键的所有信息了 */
    {
        t1 = g_remote_data;                             /* 得到地址码 */
        t2 = (g_remote_data >> 8) & 0xff;               /* 得到地址反码 */

        if ((t1 == (uint8_t)~t2) && t1 == REMOTE_ID)    /* 检验遥控识别码(ID)及地址 */
        {
            t1 = (g_remote_data >> 16) & 0xff;
            t2 = (g_remote_data >> 24) & 0xff;

            if (t1 == (uint8_t)~t2)
            {
                sta = t1;                               /* 键值正确 */
            }
        }

        if ((sta == 0) || ((g_remote_sta & 0x80) == 0)) /* 按键数据错误/遥控已经没有按下了 */
        {
            g_remote_sta &= ~(1 << 6);                  /* 清除接收到有效按键标识 */
            g_remote_cnt = 0;                           /* 清除按键次数计数器 */
        }
    }

    return sta;
}

// /**
//  * @brief       红外遥控测试
//  * @param       无
//  * @retval      无
//  */
// void remote_test(void)
// {
//     printf("\r\n/******************************* 5，红外遥控器测试 *******************************/\r\n");
//     printf("/********* 注意：按下红外遥控器，串口输出对应键值，当按下Power将退出测试 *********/\r\n");
//     printf("/****************************** NEXT: POWER ******************************/\r\n\r\n");
//     uint8_t key , old_key = 0;
//     char *str = 0;
//     remote_init();
        
//     while(1)
//     {   
//         /* 测试红外 */
//         key = remote_scan();
//         switch (key)
//         {
//             case 0:
//                 str = "ERROR";
//                 break;

//             case 69:
//                 str = "POWER";
//                 break;

//             case 70:
//                 str = "UP";
//                 break;

//             case 64:
//                 str = "PLAY";
//                 break;

//             case 71:
//                 str = "ALIENTEK";
//                 break;

//             case 67:
//                 str = "RIGHT";
//                 break;

//             case 68:
//                 str = "LEFT";
//                 break;

//             case 7:
//                 str = "VOL-";
//                 break;

//             case 21:
//                 str = "DOWN";
//                 break;

//             case 9:
//                 str = "VOL+";
//                 break;

//             case 22:
//                 str = "1";
//                 break;

//             case 25:
//                 str = "2";
//                 break;

//             case 13:
//                 str = "3";
//                 break;

//             case 12:
//                 str = "4";
//                 break;

//             case 24:
//                 str = "5";
//                 break;

//             case 94:
//                 str = "6";
//                 break;

//             case 8:
//                 str = "7";
//                 break;

//             case 28:
//                 str = "8";
//                 break;

//             case 90:
//                 str = "9";
//                 break;

//             case 66:
//                 str = "0";
//                 break;

//             case 74:
//                 str = "DELETE";
//                 break;
//         }
//         if(old_key != key)
//         {
//             printf("%s\r\n",str);
//             old_key = key;
//         }
//         if (key == 69) break;                       /* 当按下电容触摸按键就退出 */
//     }

// }

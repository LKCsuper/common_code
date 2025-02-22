/*
 * @Description: 学习Rt-Thread 而来
 * @Version: 2.0
 * @Date: 2023-07-17 11:33:08
 * @LastEditors: Please set LastEditors
 * @detail:
 *
 * Components Initialization will initialize some driver and components as following
 * order:
 * rti_start         --> 0
 * BOARD_EXPORT      --> 1
 * rti_board_end     --> 1.end
 *
 * DEVICE_EXPORT     --> 2
 * COMPONENT_EXPORT  --> 3
 * FS_EXPORT         --> 4
 * ENV_EXPORT        --> 5
 * APP_EXPORT        --> 6
 *
 * rti_end           --> 6.end
 *
 * These automatically initialization, the driver or component initial function must
 * be defined with:
 * INIT_BOARD_EXPORT(fn);
 * INIT_DEVICE_EXPORT(fn);
 * ...
 * INIT_APP_EXPORT(fn);
 * etc.
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "common_includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* board init routines will be called in board_init() function */
#define MODULE_BOARD_EXPORT(fn)     MODULE_EXPORT(fn, "1")
/* pre/device/component/env/app init routines will be called in init_thread */
/* components pre-initialization (pure software initilization) */
#define MODULE_PREV_EXPORT(fn)      MODULE_EXPORT(fn, "2")
/* device initialization */
#define MODULE_DEVICE_EXPORT(fn)    MODULE_EXPORT(fn, "3")
/* components initialization (dfs, lwip, ...) */
#define MODULE_COMPONENT_EXPORT(fn) MODULE_EXPORT(fn, "4")
/* environment initialization (mount disk, ...) */
#define MODULE_ENV_EXPORT(fn)       MODULE_EXPORT(fn, "5")
/* appliation initialization (rtgui application etc ...) */
#define MODULE_APP_EXPORT(fn)       MODULE_EXPORT(fn, "6")

/* 外设初始化开始 */
static int rti_start(void)
{
    return 0;
}
MODULE_EXPORT(rti_start, "0");

/* 外设初始化结束 */
static int rti_board_start(void)
{
    return 0;
}
MODULE_EXPORT(rti_board_start, "0.end");


static int rti_board_end(void)
{
    return 0;
}
MODULE_EXPORT(rti_board_end, "1.end");

static int rti_end(void)
{
    return 0;
}
MODULE_EXPORT(rti_end, "6.end");

/* 外设初始化结束 */
static int test(void)
{
    return 0;
}
MODULE_BOARD_EXPORT(test);

void MODULE_Components_Board_Init(void)
{
#if MODULE_DEBUG_INIT
    int result;
    const struct rt_module_desc *desc;
    PRINTF_BLUE("\r\n==================================================================\r\n");
    for (desc = &__rt_module_desc_rti_board_start; desc < &__rt_module_desc_rti_board_end; desc++) {
        PRINTF_BLUE("initialize [%s]", desc->fn_name);
        result = desc->fn();
        PRINTF_BLUE(":%d done\n", result);
    }
    PRINTF_BLUE("==================================================================\r\n");
#else
    volatile const init_fn_t *fn_ptr;

    /*  分析组成
        __rt_module_rti_board_start       ==》 __rt_module_##fn
        __rt_module_rti_board_end
    */
    for (fn_ptr = &__rt_module_rti_board_start; fn_ptr < &__rt_module_rti_board_end; fn_ptr++) {
        (*fn_ptr)();
    }
#endif
}
#ifdef __cplusplus
}
#endif

/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-07-19 16:59:35
 * @detail:
 */
#ifndef _COMMON_H_
#define _COMMON_H_
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
/* Private typedef -----------------------------------------------------------*/
typedef unsigned char boolean;    /* Boolean value type. */
typedef unsigned long int uint32; /* Unsigned 32 bit value */
typedef unsigned short uint16;    /* Unsigned 16 bit value */
typedef unsigned char uint8;      /* Unsigned 8 bit value */
typedef signed long int int32;    /* Signed 32 bit value */
typedef signed short int16;       /* Signed 16 bit value */
typedef signed char int8;         /* Signed 8 bit value */

typedef enum {
    FALSE = 0,
    TRUE = !FALSE
} tagJuder;

#ifndef SET
typedef enum {
    RESET = 0,
    SET = !RESET
} FlagStatus, ITStatus;
#endif
#ifndef ENABLE
typedef enum {
    DISABLE = 0,
    ENABLE = !DISABLE
} FunctionalState;
#endif
#ifndef SUCCESS
typedef enum {
    ERR = 0,
    SUCCESS = !ERR
} ErrorStatus;
#endif
/* Private define ------------------------------------------------------------*/
#define VOID   void
#define UCHAR  unsigned char
#define CHAR   char
#define LONG   int32_t
#define ULONG  unsigned int
#define STATIC static
#define SHORT  int16_t
#define USHORT uint16_t
#define BOOL   bool
#define FLOAT  float

/* IN 表示输入 OUT表示输出参数 */
#define IN
#define OUT

/* 未使用 */
#define UNUSED(x)  (void)(x)
#define UNUSED_FUN __attribute__((__unused__))

/* FLOAT 转换 UNSIGNED INT */
#define FLOAT_TO_INT(x) *(unsigned int *)(&x);

/* 第几位 */
#define BIT(x)                       ((uint32_t)((uint32_t)0x01U << (x)))
/* 取好几位 */
#define BITS(start, end)             ((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (31U - (uint32_t)(end))))
/* 获取某个寄存器范围 */
#define GET_BITS(regval, start, end) (((regval) & BITS((start), (end))) >> (start))
/* 设置多位 */
#define SET_BITS(REG, BIT)           ((REG) |= (BIT))
/* 清除多位 */
#define CLEAR_BITS(REG, BIT)         ((REG) &= ~(BIT))
/* 读多位 */
#define READ_BITS(REG, BIT)          ((REG) & (BIT))
/* 设置位 */
#define SET_BIT(data, i)             ((data) |= (1 << (i)))
/* 清空位 */
#define CLEAR_BIT(data, i)           ((data) &= (~(1 << (i))))
/* 翻转位 */
#define FLIP_BIT(data, i)            ((data) ^= (1 << (i)))
/* 读取位 */
#define READ_BIT(data, i)            ((data) >> (i) & 1)
/* 检查位 */
#define CHECK_BIT(data, i)           ((data) & (1 << (i)))

/* 取地址数据 */
#define REG32(addr) (*(volatile uint32_t *)(uint32_t)(addr))
#define REG16(addr) (*(volatile uint16_t *)(uint32_t)(addr))
#define REG8(addr)  (*(volatile uint8_t *)(uint32_t)(addr))

/* 清空寄存器 */
#define CLEAR_REG(REG)                      ((REG) = (0x0))
/* 写入寄存器 */
#define WRITE_REG(REG, VAL)                 ((REG) = (VAL))
/* 读寄存器 */
#define READ_REG(REG)                       ((REG))
/* 修改寄存器位 */
#define MODIFY_REG(REG, CLEARMASK, SETMASK) WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

/* 得到一个字的高位和低位 */
#define WORD_LO(xxx) ((byte)((word)(xxx) & 255))
#define WORD_HI(xxx) ((byte)((word)(xxx) >> 8))

/*得到指定地址上的一个字节或字*/
#define MEM_B(x) (*((byte *)(x)))
#define MEM_W(x) (*((word *)(x)))

/* 最大值最小值 */
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
/* 绝对值 */
#define ABS(x)    (((x) > 0) ? (x) : (0 - (x)))


/* 得到fiele在结构体的偏移量 */
#define STRUCT_OFFSET(s, m)      ((size_t)(&((s *)0)->m))
/* 得到一个结构体中field所占用的字节数 */
#define STRUCT_FSIZ(type, field) sizeof(((type *)0)->field)

/* 转换为字符串 */
#define STR(str)         #str
/* 数字转换为字符串 */
#define TO_STR(str)      STR(str)
/* 拼接字符串 */
#define STR_CONCAT(a, b) a##b

/* 检测字节对齐,字节向上对齐 字节向下对齐 */
#define CHECK_ALIGN_ADRESS(x, a)   (((x) + (a) - 1) & ~(a - 1))
#define ADRESS_ALIGN_DOWN(a, size) (a & (~(size - 1)))
#define ADRESS_ALIGN_UP(a, size)   ((a + size - 1) & (~(size - 1)))
/* 除法的向上取整*/
#define CEIL(num, deno)            (((num) + (deno) - 1) / (deno))
/* 除法的向下取整 */
#define FLOOR(num, deno)           ((num) / (deno))

/* 超时函数 */
#define TIME_OUT() Common_Time_Out()

/* 获取枚举个数 */
#define GET_EN_NUM(x)    (x - 1)
#define ARRARY_LENGTH(a) (sizeof(a) / sizeof((a)[0]))

/* 乘法 */
#define SQUARE(x)                   ((x)*(x))

#define htol(x)                     ((uint32)(((x)&0xff000000)>>24)|((x)&0x00ff0000)<<8|((x)&0x0000ff00)<<24|((x)&0x000000ff)<<16)

/* 用来超时执行 */
// #define IS_TIME_OUT(x) while((x)) {if(TIME_OUT())break;}
#define IS_TIME_OUT(start, timeout) (rt_tick_get() - (start) > (timeout))
/* 翻转字节序 */
#define TO_ENDIAN(result, len)      format_endian_oneself((UCHAR *)result, len, len)
/* 翻转字节序 */
#define SWAP_DATA(result, len)      format_endian_oneself((UCHAR *)result, len, len)
#define MODULE_DEBUG_INIT           1

/* 标准c库的函数 这里全部#deinfe */
#define MEMCPY(x, y, z)          rt_memcpy(x, y, z);
#define MEMSET(x, y, z)          rt_memset(x, y, z);
#define STRNCPY(x, y, z)         rt_strncpy(x, y, z)
#define STRNCMP(x, y, z)         rt_strncmp(x, y, z)
#define STRCMP(x, y)             rt_strcmp(x, y)
#define STRNLEN(x, y)            rt_strnlen(x, y)
#define STRLEN(x)                rt_strlen(x)
#define STRSTR(x, y)             rt_strstr(x, y)
#define STRDUP(x)                rt_strdup(x) // ! 注意,使用这个函数必须释放内存
#define FREE(x)                  rt_free(x)
#define MALLOC(x)                rt_malloc(x)
#define OS_DELAY(x)              rt_thread_mdelay(x)
#define SPRINTF(x, fmt, ...)     rt_sprintf(x, fmt, ##__VA_ARGS__)
#define SNPRINTF(x, y, fmt, ...) rt_snprintf(x, y, fmt, ##__VA_ARGS__)
#define ATOI(x)                  atoi(x)
#define GET_TICK()               HAL_GetTick()
/* 判断是否为真 */
#define ASSERT_EQL(x, y)                                                                                               \
    do {                                                                                                               \
        if (x == y)                                                                                                    \
            return;                                                                                                    \
    } while (0);

#define ASSERT_NULL(x)                                                                                                 \
    do {                                                                                                               \
        if (NULL == x)                                                                                                 \
            return;                                                                                                    \
    } while (0);

#define ASSERT(x, y)                                                                                                   \
    do {                                                                                                               \
        if (x != y)                                                                                                    \
            return;                                                                                                    \
    } while (0);

#define ASSERT_RETUEN(x, y, z)                                                                                         \
    do {                                                                                                               \
        if (x != y)                                                                                                    \
            return z;                                                                                                  \
    } while (0);

// 判断返回值 并且打印函数
#define CHECK_RET(x)                                                                                                   \
    do {                                                                                                               \
        LONG ret;                                                                                                      \
        ret = (x);                                                                                                     \
        if (SUCCESS != ret) {                                                                                          \
            PRINTF("[%s:%d] %s Err[%#x]\n", __FUNCTION__, __LINE__, TO_STR(x), ret);                                   \
            return;                                                                                                    \
        }                                                                                                              \
    } while (0);

#define CHECK_RETURN(x)                                                                                                \
    do {                                                                                                               \
        LONG ret;                                                                                                      \
        ret = (x);                                                                                                     \
        if (SUCCESS != ret) {                                                                                          \
            PRINTF("[%s:%d] %s Err[%#x]\n", __FUNCTION__, __LINE__, TO_STR(x), ret);                                   \
            return ret;                                                                                                \
        }                                                                                                              \
    } while (0);
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
ULONG Common_Time_Out(VOID);
CHAR *Common_Strdup(const char *str);
CHAR *Common_Strsplit(CHAR **pucStringp, const CHAR *pcDelim);
LONG Common_StrstrCnt(CHAR *pcStr, CHAR *pcSubstr);
int COMMON_StrToHex(const char *str, unsigned char *buf, unsigned int hex_nbytes, unsigned int group_nbytes, unsigned int endian)
int COMMON_HexToStr(char inArray[], unsigned int maxInSize, char outHexString[], unsigned int maxOutSize);
VOID Common_ToEdian(UCHAR *result, ULONG len, UCHAR size);
VOID Common_Swap(VOID *dest, VOID *src, ULONG len);
VOID Common_MemEndian(VOID *dest, VOID *src, ULONG len);
ULONG comresult_byte(UCHAR *src, volatile UCHAR *dst, ULONG len);
ULONG Common_JudgeSystem(void);
LONG Common_ReadWrite(UCHAR isRdWr, UCHAR *pucDatabuf, ULONG ulLen, ULONG ulMaxlen);
UCHAR COMMON_Bcd2Dec(UCHAR BCD);
UCHAR COMMON_Dec2Bcd(UCHAR DEC);
#endif // 宏结束行

/******************************************************************************
 * @brief    通用宏定义
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-02-01     Roger.luo    Initial version.
 ******************************************************************************/

#ifndef _COM_DEF_H_
#define _COM_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*匿名类型定义 -----------------------------------------------------------*/
#define ANONY_CONN(type, var, line)   type var##line
#define ANONY_DEF(type, prefix, line) ANONY_CONN(type, prefix, line)
#define ANONY_TYPE(type, prefix)      ANONY_DEF(type, prefix, __LINE__)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
// #define offsetof(type, member) ((unsigned int)&((type *)0)->member)
#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))


/* Compiler Related Definitions */
#ifdef __CC_ARM                         /* ARM Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  __attribute__((section(x)))
    #define UNUSED                      __attribute__((unused))
    #define USED                        __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define rt_inline                   static __inline
    /* module compiling */
    #ifdef RT_USING_MODULE
        #define RTT_API                 __declspec(dllimport)
    #else
        #define RTT_API                 __declspec(dllexport)
    #endif

#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  @ x
    #define UNUSED
    #define USED
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
    #define rt_inline                   static inline
    #define RTT_API

#elif defined (__GNUC__)                /* GNU GCC Compiler */
    #ifdef RT_USING_NEWLIB
        #include <stdarg.h>
    #else
		/* the version of GNU GCC must be greater than 4.x */
        typedef __builtin_va_list   __gnuc_va_list;
        typedef __gnuc_va_list      va_list;
        #define va_start(v,l)       __builtin_va_start(v,l)
        #define va_end(v)           __builtin_va_end(v)
        #define va_arg(v,l)         __builtin_va_arg(v,l)
    #endif

    #define SECTION(x)                  __attribute__((section(x)))
    #define UNUSED                      __attribute__((unused))
    #define USED                        __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define rt_inline                   static __inline
    #define RTT_API
#elif defined (__ADSPBLACKFIN__)        /* for VisualDSP++ Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  __attribute__((section(x)))
    #define UNUSED                      __attribute__((unused))
    #define USED                        __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define rt_inline                   static inline
    #define RTT_API
#elif defined (_MSC_VER)
    #include <stdarg.h>
    #define SECTION(x)
    #define UNUSED
    #define USED
    #define ALIGN(n)                    __declspec(align(n))
    #define rt_inline                   static __inline
    #define RTT_API
#elif defined (__TI_COMPILER_VERSION__)
    #include <stdarg.h>
    /* The way that TI compiler set section is different from other(at least
     * GCC and MDK) compilers. See ARM Optimizing C/C++ Compiler 5.9.3 for more
     * details. */
    #define SECTION(x)
    #define UNUSED
    #define USED
	#define PRAGMA(x)					_Pragma(#x)
    #define ALIGN(n)
    #define rt_inline                   static inline
    #define RTT_API
#else
    #error not supported tool chain
#endif

/* 根据不同的ram进行 分配 */
#define CCRAM  __attribute__((section(".ccmram")))
#define PACKED __attribute__((packed)) /* 按照实际占用字节数对齐 */
#define WEAK       __attribute__((weak))
#ifdef __cplusplus
}
#endif

#endif

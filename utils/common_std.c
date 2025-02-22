/*
 * @Description: c库
 * @Version: 1.0
 * @Author: lkc
 * @detail:
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "common_includes.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 字符串类型转换
 * @detail: 
 * @param {int} value
 * @param {char} *str
 * @param {int} base
 * @return {*}
 * @author: lkc
 */
char *COMMON_itoa(int value, char *str, int base)
{
    if (base < 2 || base > 32) {
        printf("Wrong radix!\n");
        return str;
    }
    char *ret = str;
    if (value == 0) {
        *str++ = '0';
        *str = '\0';
        return ret;
    }
    if (base == 10 && value < 0) {
        value = -value;
        *str++ = '-';
    }
    char *start = str;
    // 从右到左依次将数字的每一位存储起来
    size_t num = value;
    while (num != 0) {
        if (num % base < 10) {
            *str++ = '0' + (char)(num % base);
        } else {
            *str++ = 'a' + (char)(num % base - 10);
        }
        num /= base;
    }
    *str = '\0';
    // 倒置字符串
    for (char *left = start, *right = str - 1; left < right; left++, right--) {
        char tmp = *left;
        *left = *right;
        *right = tmp;
    }
    return ret;
}

/**
 * @description: strdup
 * @detail:
 * @param {char*} str
 * @return {*}
 * @author: lkc
 */
CHAR *Common_Strdup(const char *str)
{
    if (str == NULL) {
        return NULL;
    }

    char *strat = (char *)str;
    int len = 0;
    while (*str++ != '\0') {
        len++;
    }
    char *ret = (char *)malloc(len + 1);

    while ((*ret++ = *strat++) != '\0') {
    }

    return ret - (len + 1);
}

/**
 * @description: 数据分割，可以分割两个连续的分隔符
 * @detail:
 * @param {CHAR**} pcStringp
 * @param {CHAR*} pcDelim
 * @return {*}
 * @author: lkc
 */
CHAR *Common_Strsplit(CHAR **pucStringp, const CHAR *pcDelim)
{
    char *start = *pucStringp;
    char *p;

    p = (start != NULL) ? strpbrk(start, pcDelim) : NULL;

    if (p == NULL) {
        *pucStringp = NULL;
    } else {
        *p = '\0';
        *pucStringp = p + 1;
    }

    return start;
}

/**
 * @description: 统计字符串在另一个字符串中出现的次数
 * @detail:
 * @param {CHAR} *pcStr
 * @param {CHAR} *pcSubstr
 * @return {*}
 * @author: lkc
 */
LONG Common_StrstrCnt(CHAR *pcStr, CHAR *pcSubstr)
{
    char *srcStr = pcStr;
    int count = 0;

    do {
        srcStr = strstr(srcStr, pcSubstr);
        if (srcStr != NULL) {
            count++;
            srcStr = srcStr + strlen(pcSubstr);
        } else {
            break;
        }
    } while (*srcStr != '\0');

    return count;
}

#ifdef __cplusplus
}
#endif
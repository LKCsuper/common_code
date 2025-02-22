/*
 * @Author: wudi
 * @Date: 2018-07-04 15:46:56
 * @Last Modified by: wudi
 * @Last Modified time: 2018-07-04 15:58:14
 */
#ifndef __DFIFO_H__
#define __DFIFO_H__

#include "main.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*- for stm32 -*/
#define DFIFO_ENABLE_INT()  __enable_irq()
#define DFIFO_DISABLE_INT() __disable_irq()
/*- for stm8 -*/
// #define DFIFO_ENABLE_INT() enableInterrupts()
// #define DFIFO_DISABLE_INT() disableInterrupts()

// uin8_t fifo struct
typedef struct {
    char *name;
    uint8_t *buff;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
    uint16_t cnt;
} fifo_t;

extern bool fifo_init(fifo_t *fifo, uint8_t *buff, uint16_t size, char *name);
extern bool fifo_put(fifo_t *fifo, uint8_t chByte);
extern bool fifo_get(fifo_t *fifo, uint8_t *pchByte);
extern bool fifo_clear(fifo_t *fifo);

// 这两个函数的使用有限定条件，内部不会关闭中断，如果有更高优先级中断操作队列,或者多线程操作队列，有可能会出现错误。
extern bool fifo_put_not_isr(fifo_t *fifo, uint8_t chByte);
extern bool fifo_get_not_isr(fifo_t *fifo, uint8_t *pchByte);
extern bool fifo_clear_not_isr(fifo_t *fifo);

#endif

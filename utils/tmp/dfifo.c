/** 迪哥多个项目中使用，其他fifo好像都没有开关中断
 * @Author: wudi
 * @Date: 2018-07-04 15:46:59
 * @Last Modified by: wudi
 * @Last Modified time: 2018-07-04 15:58:31
 */
#include "dfifo.h"

#define DFIFO_DEBUG(...)
// #define DFIFO_DEBUG(...)                                                                                               \
//     do {                                                                                                               \
//         debug_printf("[dfifo] ");                                                                                       \
//         debug_printf(__VA_ARGS__);                                                                                     \
//     } while (0)

bool fifo_init(fifo_t *fifo, uint8_t *buff, uint16_t size, char *name)
{
    if (NULL == fifo || NULL == buff || NULL == name) {
        return false;
    }

    fifo->name = name;
    fifo->buff = buff;
    fifo->size = size;
    fifo->head = 0;
    fifo->tail = 0;
    fifo->cnt = 0;

    return true;
}

bool fifo_put(fifo_t *fifo, uint8_t chByte)
{
    if (NULL == fifo) {
        return false;
    }

    DFIFO_DISABLE_INT();
    if (fifo->cnt >= fifo->size) {
        DFIFO_DEBUG("%s fifo full\r\n", fifo->name);
        DFIFO_ENABLE_INT();
        return false;
    }

    fifo->buff[fifo->head] = chByte;
    fifo->cnt++;
    fifo->head++;
    if (fifo->head >= fifo->size) {
        fifo->head = 0;
    }
    DFIFO_ENABLE_INT();

    return true;
}

bool fifo_get(fifo_t *fifo, uint8_t *pchByte)
{
    if (NULL == fifo || NULL == pchByte) {
        return false;
    }

    DFIFO_DISABLE_INT();
    if (fifo->cnt == 0) {
        // DFIFO_DEBUG("%s fifo empty\r\n",fifo->name);
        DFIFO_ENABLE_INT();
        return false;
    }

    *pchByte = fifo->buff[fifo->tail];
    fifo->cnt--;
    fifo->tail++;
    if (fifo->tail >= fifo->size) {
        fifo->tail = 0;
    }
    DFIFO_ENABLE_INT();

    return true;
}

bool fifo_clear(fifo_t *fifo)
{
    if (NULL == fifo) {
        return false;
    }

    DFIFO_DISABLE_INT();
    fifo->head = 0;
    fifo->tail = 0;
    fifo->cnt = 0;
    DFIFO_ENABLE_INT();

    return true;
}

bool fifo_put_not_isr(fifo_t *fifo, uint8_t chByte)
{
    if (NULL == fifo) {
        return false;
    }

    if (fifo->cnt >= fifo->size) {
        DFIFO_DEBUG("%s fifo full\r\n", fifo->name);
        return false;
    }

    fifo->buff[fifo->head] = chByte;
    fifo->cnt++;
    fifo->head++;
    if (fifo->head >= fifo->size) {
        fifo->head = 0;
    }

    return true;
}

bool fifo_get_not_isr(fifo_t *fifo, uint8_t *pchByte)
{
    if (NULL == fifo || NULL == pchByte) {
        return false;
    }

    if (fifo->cnt == 0) {
        // DFIFO_DEBUG("%s fifo empty\r\n", fifo->name);
        return false;
    }

    *pchByte = fifo->buff[fifo->tail];
    fifo->cnt--;
    fifo->tail++;
    if (fifo->tail >= fifo->size) {
        fifo->tail = 0;
    }

    return true;
}

bool fifo_clear_not_isr(fifo_t *fifo)
{
    if (NULL == fifo) {
        return false;
    }

    fifo->head = 0;
    fifo->tail = 0;
    fifo->cnt = 0;

    return true;
}

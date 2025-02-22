/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-09-18 11:17:53
 * @LastEditors: lkc
 * @LastEditTime: 2023-10-13 08:17:59
 * @detail:
 */

#ifndef COMMON_FIFO_H
#define COMMON_FIFO_H

#include <stdbool.h>
#include <stdint.h>

#define RINGBUFF_RX_LEN (1280)

typedef void (*lock_fun)(void);

typedef struct {
    uint8_t *buf;         /* ������ */
    uint32_t buf_size;    /* ��������С */
    uint32_t occupy_size; /* ��Ч���ݴ�С */
    uint8_t *pwrite;      /* дָ�� */
    uint8_t *pread;       /* ��ָ�� */
    void (*lock)(void);   /* �������� */
    void (*unlock)(void); /* ������� */
} _fifo_t;


extern void fifo_register(_fifo_t *pfifo, uint8_t *pfifo_buf, uint32_t size, lock_fun lock, lock_fun unlock);
extern void fifo_release(_fifo_t *pfifo);
extern uint32_t fifo_write(_fifo_t *pfifo, const uint8_t *pbuf, uint32_t size);
extern uint32_t fifo_read(_fifo_t *pfifo, uint8_t *pbuf, uint32_t size);
extern uint32_t fifo_get_total_size(_fifo_t *pfifo);
uint32_t fifo_get_occupy_size(_fifo_t *pfifo);
/* tfk_li
����������������ȡ�� occupy_size
�����Ա����û�мӽ����������������ǰ�ȫ�ģ������������ĳ�static�ģ������⿪�š�
Ҫ�밲ȫ��Ҫ�ӽ�������������������Ӱ�����ܣ����Բ�����ʹ��������������
 */
// extern uint32_t fifo_get_free_size(_fifo_t *pfifo);
// extern uint32_t fifo_get_occupy_size(_fifo_t *pfifo);

#endif

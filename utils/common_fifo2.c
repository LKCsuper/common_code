// 未验证

#include <sysdep.h>
#include <string.h>
#include <fifo.h>

static uint32 __fast min(uint32 x, uint32 y)
{
    return (x < y ? x : y);
}

void fifo_init(fifo_t *pfifo, int8 *pbuffer, uint32 size)
{
    fifo_t *pl_fifo;

    pl_fifo = pfifo;

    /* in/out add from zero. */
    pl_fifo->in = 0;
    pl_fifo->out = 0;

    pl_fifo->size = size;
    pl_fifo->pbuffer = pbuffer;
}

uint32 __fast fifo_put(fifo_t *pfifo, int8 *pbuffer, uint32 length)
{
    uint32 split_len = 0;

    /**
     * We check FIFO size to select min length in avoid of fifo overflow.
     * the left fifo size = pfifo->size - pfifo->in + pfifo->out
     */
    length = min(length, pfifo->size - pfifo->in + pfifo->out);

    split_len = min(length, pfifo->size - (pfifo->in & (pfifo->size - 1)));

    /* First put the data starting from pfifo->in to buffer end */
    memcpy(pfifo->pbuffer + (pfifo->in & (pfifo->size - 1)), pbuffer, split_len);

    /* then put the rest (if any) at the beginning of the buffer */
    memcpy(pfifo->pbuffer, pbuffer + split_len, length - split_len);

    pfifo->in += length; // After 'in' increasing overflow, reverse to zero.

    return length;
}

uint32 __fast fifo_get(fifo_t *pfifo, int8 *pbuffer, uint32 length)
{
    uint32 split_len = 0;

    /**
     * We check FIFO valid data size to select min length.
     * the valid data size = pfifo->in - pfifo->out
     */
    length = min(length, pfifo->in - pfifo->out);

    split_len = min(length, pfifo->size - (pfifo->out & (pfifo->size - 1)));

    /* First put the data starting from pfifo->out to buffer end */
    memcpy(pbuffer, pfifo->pbuffer + (pfifo->out & (pfifo->size - 1)), split_len);

    /* then put the rest (if any) at the beginning of the buffer */
    memcpy(pbuffer + split_len, pfifo->pbuffer, length - split_len);

    pfifo->out += length; // After 'in' increasing overflow, reverse to zero.

    return length;
}

uint32 __fast fifo_length_get(fifo_t *pfifo)
{
    return (pfifo->in - pfifo->out);
}

void __fast fifo_reset(fifo_t *pfifo)
{
    pfifo->in = 0;
    pfifo->out = 0;
}

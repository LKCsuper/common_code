#ifndef __COMMON_FIFO2_H__
#define __COMMON_FIFO2_H__

#include <sysdep.h>

typedef struct fifo {
    int8 *pbuffer; // the pbuffer holding the data buffer address header

    /* The size must be 2^n. Otherwise,it won't work. */
    uint32 size; // the size of the allocated buffer
    uint32 in;   // data is added at offset (in % size)
    uint32 out;  // data is extracted from off. (out % size)
} fifo_t;

void fifo_init(fifo_t *pfifo, int8 *pbuffer, uint32 size);
uint32 fifo_put(fifo_t *pfifo, int8 *pbuffer, uint32 length);
uint32 fifo_get(fifo_t *pfifo, int8 *pbuffer, uint32 length);
uint32 fifo_length_get(fifo_t *pfifo);
void fifo_reset(fifo_t *pfifo);

#endif /* __FIFO_H__ */

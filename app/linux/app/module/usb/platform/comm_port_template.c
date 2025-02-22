
#include <stdio.h>

#include "../../comm.h"

typedef struct {
    void *hdl;
} comm_user_data_t;

static void *dev_open(int num)
{
    #error "dev"
    return NULL;
}

static void dev_close(void *ctx)
{
    #error "dev"
    return;
}

static int dev_xfer(void *ctx, uint8_t *pucaCmd, uint8_t *pucaInData, unsigned int uiInDataLen, uint8_t *pucaOutData, unsigned int *puiOutDataLen)
{
    #error "dev"
    return -1;
}

comm_ops_t template_ops = {
    //
    .open = dev_open,
    .close = dev_close,
    .xfer = dev_xfer,
};

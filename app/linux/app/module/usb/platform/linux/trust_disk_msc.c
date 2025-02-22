
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>

#include "../../comm.h"

#include "ZaykSkf_api.h"
#include "ZaykSkfDebug.h"

#define true  1
#define false 0

#include <scsi/sg.h>
#include <sys/ioctl.h>

static void *dev_open(int num)
{
    // not support
    return NULL;
}

static void dev_close(void *ctx)
{
    // not support
    return;
}

static int dev_xfer(void *ctx, uint8_t *pucaCmd, uint8_t *pucaInData, unsigned int uiInDataLen, uint8_t *pucaOutData, unsigned long *puiOutDataLen)
{
    // not support
    return SAR_NOTSUPPORTYETERR;
}

comm_ops_t trust_disk_msc_ops = {
    //
    .open = dev_open,
    .close = dev_close,
    .xfer = dev_xfer,
};

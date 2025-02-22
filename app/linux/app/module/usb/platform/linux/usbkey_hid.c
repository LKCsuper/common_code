
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <linux/hidraw.h>
#include <linux/input.h>

#include "../../comm.h"

#include "ZaykSkf_api.h"
#include "ZaykSkfDebug.h"

typedef struct {
    int fd;
} comm_user_data_t;

#define pVID 0x5a41
#define pPID 0x594b

static void *dev_open(int num)
{
    int fd;
    comm_user_data_t *data;
    struct sg_io_hdr *p_hdr;
    int status = 0;
    char path[20];
    int i;
    int cnt = 0;
    struct hidraw_devinfo info;

    data = (comm_user_data_t *)malloc(sizeof(comm_user_data_t));
    if (data == NULL) {
        // df_error("malloc call error\n");
        return NULL;
    }

#if 1
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/dev");
    if (dir == NULL) {
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (0 == memcmp(entry->d_name, "hidraw", strlen("hidraw"))) {
            // printf("dir : %s\n", entry->d_name);

            snprintf(path, sizeof(path), "/dev/%s", entry->d_name);
            fd = open(path, O_RDWR);
            if (fd > 0) {

                ioctl(fd, HIDIOCGRAWINFO, &info);
                if ((info.vendor == pVID) && info.product == pPID) {
                    LOGI("num: %d devnam: %s USBKEY is HID\r\n", num, entry->d_name);
                    if (++cnt == num) {
                        data->fd = fd;

                        closedir(dir);
                        return data;
                    }
                }
                close(fd);
            }
        }
    }

    closedir(dir);
#else

    for (i = 0; i < 10; i++) {
        sprintf(path, "/dev/hidraw%d", i);
        fd = open(path, O_RDWR);
        if (fd > 0) {

            ioctl(fd, HIDIOCGRAWINFO, &info);
            if ((info.vendor == pVID) && info.product == pPID) {

                data->fd = fd;
                ctx->user_data = data;

                return ctx;
            }
            close(fd);
        }
    }
#endif

    free(data);
    return NULL;
}

static void dev_close(void *ctx)
{
    comm_user_data_t *data = ctx;

    if (data == NULL) {
        return;
    }

    close(data->fd);
    free(data);

    return;
}

static int dev_xfer(void *ctx, uint8_t *pucaCmd, uint8_t *pucaInData, unsigned int uiInDataLen, uint8_t *pucaOutData, unsigned long *puiOutDataLen)
{
    comm_user_data_t *data;
    unsigned char ucaOutBuff[5200], ucaInBuff[5200];
    unsigned int uiOutDataLen;
    int ret;
    int offset;

    data = ctx;
    if (data == NULL) {
        return SAR_DEV_COMM;
    }

    // out
    memcpy(ucaOutBuff + 3, pucaCmd, 7);
    memcpy(ucaOutBuff + 3 + 7, pucaInData, uiInDataLen);
    ucaOutBuff[0] = 0xA5;
    ucaOutBuff[1] = (uiInDataLen + 3 + 7) >> 8;
    ucaOutBuff[2] = uiInDataLen + 3 + 7;
    uiOutDataLen = uiInDataLen + 3 + 7;

#if DEBUG_COMM_INOUT
    xprint_hex("out\n", 0, ucaOutBuff, uiOutDataLen);
#endif

    offset = 0;
    while (uiOutDataLen > 0) {
        ret = write(data->fd, ucaOutBuff + offset, uiOutDataLen);
        if (ret < 0) {
            return SAR_DEV_COMM;
        }
        uiOutDataLen -= ret;
        offset += ret;
    }

    // in
    ret = read(data->fd, ucaInBuff, sizeof(ucaInBuff));
    if (ret < 0) {
        return SAR_DEV_COMM;
    }

#if DEBUG_COMM_INOUT
    xprint_hex("in0\n", 0, ucaInBuff, ret);
#endif

    if (ucaInBuff[0] != 0x5A) {
        return SAR_DEV_COMM;
    }

    int iLength = (ucaInBuff[1] << 8) + ucaInBuff[2];
    iLength -= ret;

    offset = ret;
    while (iLength > 0) {
        ret = read(data->fd, ucaInBuff + offset, sizeof(ucaInBuff));
        if (ret < 0) {
            return SAR_DEV_COMM;
        }

#if DEBUG_COMM_INOUT
        xprint_hex("in1\n", 0, ucaInBuff + offset, ret);
#endif
        iLength -= ret;
        offset += ret;
    }

    iLength = (ucaInBuff[1] << 8) + ucaInBuff[2];
    memcpy(pucaOutData, &ucaInBuff[3], iLength - 2 - 3);
    ret = (ucaInBuff[iLength - 2] << 8) + ucaInBuff[iLength - 1];

    *puiOutDataLen = iLength - 2 - 3;

#if DEBUG_COMM_INOUT
    xprint_hex("in\n", 0, ucaInBuff, iLength);
#endif

    return ret;
}

comm_ops_t usbkey_hid_ops = {
    //
    .open = dev_open,
    .close = dev_close,
    .xfer = dev_xfer,
};

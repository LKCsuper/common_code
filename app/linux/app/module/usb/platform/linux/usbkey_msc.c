
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

// #define DEBUG_COMM_INOUT 1

#define SENSE_LEN    255
#define BLOCK_LEN    32
#define SCSI_TIMEOUT 20000

typedef struct {
    int fd;
    struct sg_io_hdr *p_hdr;

    unsigned char sense_buffer[SENSE_LEN];
    unsigned char data_buffer[BLOCK_LEN * 256];
} comm_user_data_t;

static void show_sense_buffer(struct sg_io_hdr *hdr)
{
    unsigned char *buffer = hdr->sbp;
    int i;
    for (i = 0; i < hdr->mx_sb_len; ++i) {
        putchar(buffer[i]);
    }
}

struct sg_io_hdr *init_io_hdr()
{

    struct sg_io_hdr *p_scsi_hdr = (struct sg_io_hdr *)malloc(sizeof(struct sg_io_hdr));
    memset(p_scsi_hdr, 0, sizeof(struct sg_io_hdr));
    if (p_scsi_hdr) {
        p_scsi_hdr->interface_id = 'S';          /* this is the only choice we have! */
        p_scsi_hdr->flags = SG_FLAG_LUN_INHIBIT; /* this would put the LUN to 2nd byte of cdb*/
    }

    return p_scsi_hdr;
}

static void destroy_io_hdr(struct sg_io_hdr *p_hdr)
{
    if (p_hdr) {
        free(p_hdr);
    }
}

static void set_xfer_data(struct sg_io_hdr *p_hdr, void *data, unsigned int length)
{
    if (p_hdr) {
        p_hdr->dxferp = data;
        p_hdr->dxfer_len = length;
    }
}

static void set_sense_data(struct sg_io_hdr *p_hdr, unsigned char *data, unsigned int length)
{
    if (p_hdr) {
        p_hdr->sbp = data;
        p_hdr->mx_sb_len = length;
    }
}

static int execute_Inquiry(int fd, int page_code, int evpd, struct sg_io_hdr *p_hdr)
{
    unsigned char cdb[6];

    /* set the cdb format */
    cdb[0] = 0x12; /*This is for Inquery*/
    cdb[1] = evpd & 1;
    cdb[2] = page_code & 0xff;
    cdb[3] = 0;
    cdb[4] = 0xff;
    cdb[5] = 0; /*For control filed, just use 0*/

    p_hdr->dxfer_direction = SG_DXFER_FROM_DEV;
    p_hdr->cmdp = cdb;
    p_hdr->cmd_len = 6;

    int ret = ioctl(fd, SG_IO, p_hdr);
    if (ret < 0) {
        printf("Sending SCSI Command failed.\n");
        close(fd);
        exit(1);
    }

    return p_hdr->status;
}

static void *dev_open(int num)
{
    comm_user_data_t *data;
    int fd;
    struct sg_io_hdr *p_hdr;
    int status = 0;
    char path[20];
    int i;
    int cnt = 0;

    data = (comm_user_data_t *)malloc(sizeof(comm_user_data_t));
    if (data == NULL) {
        return NULL;
    }

    p_hdr = init_io_hdr();
    set_xfer_data(p_hdr, data->data_buffer, BLOCK_LEN * 256);
    set_sense_data(p_hdr, data->sense_buffer, SENSE_LEN);

#if 1
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/dev");
    if (dir == NULL) {
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (0 == memcmp(entry->d_name, "sg", strlen("sg"))) {
            // printf("dir : %s\n", entry->d_name);

            snprintf(path, sizeof(path), "/dev/%s", entry->d_name);
            fd = open(path, O_RDWR);
            if (fd > 0) {
                status = execute_Inquiry(fd, 0, 0, p_hdr);
                if (status != 0) {
                    show_sense_buffer(p_hdr);
                } else {
                    unsigned char *buffer = p_hdr->dxferp;
                    if ((0 == memcmp("ZaSec", buffer + 8, 5)) && (0 == memcmp("USBKEY", buffer + 16, 6))) {
#if 0
                    show_vendor(p_hdr);
                    show_product(p_hdr);
                    show_product_rev(p_hdr);
#endif
                        if (++cnt == num) {
                            LOGI("USBKEY is SCSI");
                            data->fd = fd;
                            data->p_hdr = p_hdr;
                            return data;
                        }
                    }
                }
                close(fd);
            }
        }
    }

    closedir(dir);
#else

    for (i = 0; i < 10; i++) {
        sprintf(path, "/dev/sg%d", i);
        fd = open(path, O_RDWR);
        if (fd > 0) {
            status = execute_Inquiry(fd, 0, 0, p_hdr);
            if (status != 0) {
                show_sense_buffer(p_hdr);
            } else {
                unsigned char *buffer = p_hdr->dxferp;
                if ((0 == memcmp("ZaSec", buffer + 8, 5)) && (0 == memcmp("USBKEY", buffer + 16, 6))) {
#if 0
                    show_vendor(p_hdr);
                    show_product(p_hdr);
                    show_product_rev(p_hdr);
#endif
                    data->fd = fd;
                    data->p_hdr = p_hdr;
                    ctx->user_data = data;

                    LOGI("USBKEY is SCSI\r\n");

                    return ctx;
                }
            }
            close(fd);
        }
    }
#endif

    destroy_io_hdr(p_hdr);
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
    destroy_io_hdr(data->p_hdr);
    free(data);
    return;
}

static int dev_xfer(void *ctx, uint8_t *pucaCmd, uint8_t *pucaInData, unsigned int uiInDataLen, uint8_t *pucaOutData, unsigned long *puiOutDataLen)
{
    unsigned char cdb[16];
    comm_user_data_t *data;
    unsigned char ucaOutBuff[5200], ucaInBuff[5200];
    int ret;

    data = ctx;
    if (data == NULL) {
        return SAR_DEV_COMM;
    }

    // bot out
    memset(cdb, 0, sizeof(cdb));
    cdb[0] = 0xFE;
    cdb[1] = 0x01;
    cdb[7] = (uiInDataLen + 7) >> 8;
    cdb[8] = uiInDataLen + 7;

    data->p_hdr->dxfer_direction = SG_DXFER_TO_DEV;
    data->p_hdr->cmdp = cdb;
    data->p_hdr->cmd_len = 16;

    memcpy(ucaOutBuff + 3, pucaCmd, 7);
    memcpy(ucaOutBuff + 3 + 7, pucaInData, uiInDataLen);
    ucaOutBuff[0] = 0x12;
    ucaOutBuff[1] = (uiInDataLen + 7) >> 8;
    ucaOutBuff[2] = uiInDataLen + 7;
    data->p_hdr->dxferp = ucaOutBuff;
    data->p_hdr->dxfer_len = uiInDataLen + 3 + 7;

#if DEBUG_COMM_INOUT
    xprint_hex("bot out\n", 0, data->p_hdr->dxferp, data->p_hdr->dxfer_len);
#endif

    ret = ioctl(data->fd, SG_IO, data->p_hdr);
    if (ret < 0) {
        return SAR_DEV_COMM;
    }

    // bot in
    memset(cdb, 0, sizeof(cdb));
    cdb[0] = 0xFE;
    cdb[1] = 0x02;

    data->p_hdr->dxfer_direction = SG_DXFER_FROM_DEV;
    data->p_hdr->cmdp = cdb;
    data->p_hdr->cmd_len = 16;
    data->p_hdr->dxferp = ucaInBuff;
    data->p_hdr->dxfer_len = sizeof(ucaInBuff);

    ret = ioctl(data->fd, SG_IO, data->p_hdr);
    if (ret < 0) {
        return SAR_DEV_COMM;
    }

    if (ucaInBuff[0] != 0x12) {
        return SAR_DEV_COMM;
    }

    int iLength = (ucaInBuff[1] << 8) + ucaInBuff[2];
    *puiOutDataLen = iLength - 2;
    memcpy(pucaOutData, &ucaInBuff[3], iLength - 2);
    ret = (ucaInBuff[iLength + 3 - 2] << 8) + ucaInBuff[iLength + 3 - 1];

#if DEBUG_COMM_INOUT
    xprint_hex("bot in\n", 0, ucaInBuff, iLength + 3);
#endif

    return ret;
}

comm_ops_t usbkey_msc_ops = {
    //
    .open = dev_open,
    .close = dev_close,
    .xfer = dev_xfer,
};


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../comm.h"
#include "../../../3rd/ccm3310/linux/x86_64/dev_drive.h"

#include "ZaykSkf_api.h"
#include "ZaykSkfDebug.h"

#define INTERFACE_NAME "SECOM.SCT"

typedef struct {
    PDRIVE_METH pMeth;
    int hdl;
} comm_user_data_t;

static void *dev_open(int num)
{
    int ret = 0;
    PDRIVE_METH pMeth;
    HDEV hdl;
    char *szDevices = NULL;
    unsigned int uiDevLen;
    comm_user_data_t *data = NULL;

    if (num != 1)
        return NULL;

    data = (comm_user_data_t *)malloc(sizeof(comm_user_data_t));
    if (data == NULL) {
        return NULL;
    }

    CC_GetDriveMeth(&pMeth, 0);

    ret = pMeth->SetInterfaceName(INTERFACE_NAME);
    if (ret != 0) {
        LOGE("set interface name err, errno: 0x%08x", ret);
        return NULL;
    }
    
    ret = pMeth->EnumDevice(&szDevices, &uiDevLen, NULL);
    if (ret != 0) {
        LOGE("enum device err, errno: 0x%08x", ret);
        goto __finish;
    }

    ret = pMeth->OpenDevice(szDevices, NULL, &hdl);
    if (ret != 0) {
        LOGE("open device err, errno: 0x%08x", ret);
        goto __finish;
    }

    // TODO check is recognized dev
    
    LOGI("USBKEY is TFKEY");
    pMeth->FreeDevName(szDevices);

    data->pMeth = pMeth;
    data->hdl = hdl;
    return data;

__finish:
    pMeth->FreeDevName(szDevices);
    free(data);

    return NULL;
}

static void dev_close(void *ctx)
{
    PDRIVE_METH pMeth;
    comm_user_data_t *data = ctx;

    if (data == NULL) {
        return;
    }

    pMeth = data->pMeth;
    pMeth->CloseDevice(data->hdl);

    free(data);
    return;
}

static int dev_xfer(void *ctx, uint8_t *pucaCmd, uint8_t *pucaInData, unsigned int uiInDataLen, uint8_t *pucaOutData, unsigned long *puiOutDataLen)
{
    int ret = 0;
    unsigned int uiReadDataLen = 4096;
    unsigned char ucaInData[5200] = {0xe0, 0xe0};
    unsigned char ucaOutData[5200];
    comm_user_data_t *data = NULL;

    data = (comm_user_data_t *)ctx;
    if (data == NULL) {
        return SAR_DEV_COMM;
    }

    memcpy(ucaInData, pucaCmd, 7);
    memcpy(ucaInData + 7, pucaInData, uiInDataLen);
    uiInDataLen += 7;

#ifdef DEBUG_COMM_INOUT
    xprint_hex("send: \n", 0, ucaInData, uiInDataLen);
#endif

    ret = data->pMeth->WriteDeviceData(data->hdl, ucaInData, uiInDataLen);
    if (ret) {
        // data->pMeth->CloseDevice(data->hdl);
        return SAR_DEV_COMM;
    }

    while (TRUE) {
        ret = data->pMeth->ReadDeviceData(data->hdl, ucaOutData, &uiReadDataLen);
        if (ret) {
            if (ret == DR_RD_BUSY)
                continue;
            else
                return SAR_DEV_COMM;
        } else {
#ifdef DEBUG_COMM_INOUT
            xprint_hex("recv: \n", 0, ucaOutData, uiReadDataLen);
#endif
            *puiOutDataLen = uiReadDataLen - 2;
            memcpy(pucaOutData, ucaOutData, *puiOutDataLen);
            
            ret = (ucaOutData[uiReadDataLen - 2] << 8) + ucaOutData[uiReadDataLen - 1];
            if (ret != 0x9000) {
                if (uiReadDataLen != 2) {
#ifdef DEBUG_COMM_INOUT
                    printf("\n ZAYK ReadDevcieData err, errno: %d\n", ret);
#endif
                }
            }
            return ret;
        }
    }
    
    return 0;
}

comm_ops_t tfkey_cc3310_ops = {
    //
    .open = dev_open,
    .close = dev_close,
    .xfer = dev_xfer,
};

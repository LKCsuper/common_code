#include <windows.h>
#include <wchar.h>
#include <winioctl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../comm.h"

#include "ZaykSkf_api.h"
#include "ZaykSkfDebug.h"
#if !defined(_WIN64)
#include "../../../3rd/ccm3302/win/dev_drive.h"

#define INTERFACE_NAME "SECOM.SCT"

typedef struct {
    PDRIVE_METH pMeth;
    HDEV hdl;
} comm_user_data_t;

static void *dev_open(int num)
{
    int ret = 0;
    PDRIVE_METH pMeth;
    HDEV hdl;
    char *szDevices = NULL;
    unsigned int uiDevLen;
    comm_user_data_t *data = NULL;

    if (num != 1) {
        return NULL;
    }

    data = (comm_user_data_t *)malloc(sizeof(comm_user_data_t));
    if (data == NULL) {
        return NULL;
    }

    CC_GetDriveMeth(&pMeth, 0);

    ret = pMeth->SetInterfaceName(INTERFACE_NAME);
    if (ret != 0) {
#ifdef _DEBUG
        printf("set interface name err, errno: 0x%08x\r\n", ret);
#endif
        return NULL;
    }
    
    ret = pMeth->EnumDevice(&szDevices, &uiDevLen, NULL);
    if (ret != 0) {
#ifdef _DEBUG
        printf("enum device err, errno: 0x%08x\r\n", ret);
#endif
        goto __finish;
    }

    ret = pMeth->OpenDevice(szDevices, NULL, &hdl);
    if (ret != 0) {
#ifdef _DEBUG        
        printf("open device err, errno: 0x%08x\r\n", ret);
#endif
        goto __finish;
    }

    // TODO check is recognized dev

#ifdef _DEBUG
    printf("USBKEY is TFKEy\r\n");
#endif
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

#ifdef _DEBUG
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
            if (ret == DR_RD_BUSY) {
                continue;
            } else {
                return SAR_DEV_COMM;
            }
        } else {
#ifdef _DEBUG
            xprint_hex("recv: \n", 0, ucaOutData, uiReadDataLen);
#endif
            *puiOutDataLen = uiReadDataLen - 2;
            memcpy(pucaOutData, ucaOutData, *puiOutDataLen);
            
            ret = (ucaOutData[uiReadDataLen - 2] << 8) + ucaOutData[uiReadDataLen - 1];
            if (ret != 0x9000) {
                if (uiReadDataLen != 2) {
#ifdef _DEBUG
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
#else
comm_ops_t tfkey_cc3310_ops = {
    //
    .open = NULL,
    .close = NULL,
    .xfer = NULL,
};
#endif
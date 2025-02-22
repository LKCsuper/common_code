#include <windows.h>

#include <Cfgmgr32.h>
#include <Devpkey.h>
#include <hidsdi.h>
#include <initguid.h>
#include <setupapi.h>

#include <stdio.h>

#include "../../comm.h"

#include "ZaykSkf_api.h"
#include "ZaykSkfDebug.h"

#define MAX_DEV 256

#define pVID 0x5a41
#define pPID 0x594b

typedef struct {
    HANDLE hdl;
    int uiInReportLen;
    int uiOutReportLen;
} comm_user_data_t;

static void *dev_open(int num)
{
    HIDD_ATTRIBUTES Attributes;
    SP_DEVICE_INTERFACE_DATA devInfoData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA detailData;
    GUID HidGuid;
    int cnt = 0;
    comm_user_data_t *data;

    HANDLE DeviceHandle, hDevInfo;
    int amount = 0, i;
    int MemberIndex = 0;
    LONG Result;
    ULONG Length, Required;

    HidD_GetHidGuid(&HidGuid);
    hDevInfo = SetupDiGetClassDevs(&HidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
    devInfoData.cbSize = sizeof(devInfoData);

    for (i = 0; i < MAX_DEV; i++) {
        Result = SetupDiEnumDeviceInterfaces(hDevInfo, 0, &HidGuid, MemberIndex, &devInfoData);
        if (Result != 0) {

            Result = SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, NULL, 0, &Length, NULL);

            detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);
            detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

            Result = SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, detailData, Length, &Required, NULL);
            DeviceHandle = CreateFile(detailData->DevicePath,
                                      //   0,
                                      GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, 0, NULL);
            Attributes.Size = sizeof(Attributes);
            Result = HidD_GetAttributes(DeviceHandle, &Attributes);

            free(detailData);

            do {
                if (Attributes.VendorID == pVID && Attributes.ProductID == pPID) {
                    HIDP_CAPS Capabilities;
                    PHIDP_PREPARSED_DATA PreparsedData;
                    BOOLEAN bret;
                    NTSTATUS status;

                    bret = HidD_GetPreparsedData(DeviceHandle, &PreparsedData);
                    if (bret == FALSE) {
                        break;
                    }
                    status = HidP_GetCaps(PreparsedData, &Capabilities);
                    if (status != HIDP_STATUS_SUCCESS) {
                        break;
                    }
                    bret = HidD_FreePreparsedData(PreparsedData);
                    if (bret == FALSE) {
                        break;
                    }

#ifdef _DEBUG
                    LOGI("USBKEY is HID\r\n");
#endif
                    if (++cnt == num) {
                        data = malloc(sizeof(comm_user_data_t));

                        data->uiInReportLen = Capabilities.InputReportByteLength;
                        data->uiOutReportLen = Capabilities.OutputReportByteLength;
                        data->hdl = DeviceHandle;
                        // CloseHandle(DeviceHandle);
                        SetupDiDestroyDeviceInfoList(hDevInfo);
                        return data;
                    }
                }
            } while (0);

            CloseHandle(DeviceHandle);
            MemberIndex++;
        }
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);

    return NULL;
}

static void dev_close(void *ctx)
{
    comm_user_data_t *data = ctx;

    if (data == NULL) {
        return;
    }

    CloseHandle(data->hdl);

    free(data);
}

OVERLAPPED WriteOverlapped;
OVERLAPPED ReadOverlapped;
static int dev_xfer(void *ctx, uint8_t *pucaCmd, uint8_t *pucaInData, unsigned int uiInDataLen, uint8_t *pucaOutData, unsigned long *puiOutDataLen)
{
    comm_user_data_t *data;
    unsigned char ucaOutBuff[5200], ucaInBuff[5200];
    int uiOutDataLen;
    int offset;
    unsigned int Result;
    unsigned int LastError;
    unsigned char ucaBuff[5200];
    int cnt = 0;

    int ret;

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

#if _DEBUG
    xprint_hex("out\n", 0, ucaOutBuff, uiOutDataLen);
#endif

    offset = 0;
    while (uiOutDataLen > 0) {
        ucaBuff[0] = 0x00;
        memcpy(ucaBuff + 1, ucaOutBuff + offset, data->uiOutReportLen - 1);

#if _DEBUG
        printf("out %d\n", cnt);
        xprint_hex("", 0, ucaBuff, data->uiOutReportLen);
#endif

        Result = WriteFile(data->hdl, ucaBuff, data->uiOutReportLen, NULL, &WriteOverlapped);
        if (!Result) {
            LastError = GetLastError();
            return SAR_DEV_COMM;
        }
        uiOutDataLen -= (data->uiOutReportLen - 1);
        offset += (data->uiOutReportLen - 1);
        cnt++;
    }

    // in
    DWORD read_cnt;
    cnt = 0;
    ucaBuff[0] = 0x00;
    Result = ReadFile(data->hdl, ucaBuff, data->uiInReportLen, &read_cnt, NULL);
    if (!Result) {
        LastError = GetLastError();
        return SAR_DEV_COMM;
    }

#if _DEBUG
    printf("in %d\n", cnt);
    xprint_hex("", 0, ucaBuff, data->uiInReportLen);
#endif
    cnt++;

    memcpy(ucaInBuff, ucaBuff + 1, data->uiInReportLen - 1);

    if (ucaInBuff[0] != 0x5A) {
        return SAR_DEV_COMM;
    }

    int iLength = (ucaInBuff[1] << 8) + ucaInBuff[2];
    iLength -= data->uiInReportLen - 1;

    offset = data->uiInReportLen - 1;
    while (iLength > 0) {
        ucaBuff[0] = 0x00;
        Result = ReadFile(data->hdl, ucaBuff, data->uiInReportLen, &read_cnt, NULL);
        if (!Result) {
            LastError = GetLastError();
            return SAR_DEV_COMM;
        }

#if _DEBUG
        printf("in %d\n", cnt);
        xprint_hex("", 0, ucaBuff, data->uiInReportLen);
#endif

        memcpy(ucaInBuff + offset, ucaBuff + 1, data->uiInReportLen - 1);

        iLength -= (data->uiInReportLen - 1);
        offset += (data->uiInReportLen - 1);
        cnt++;
    }

    iLength = (ucaInBuff[1] << 8) + ucaInBuff[2];
    memcpy(pucaOutData, &ucaInBuff[3], iLength - 2 - 3);
    ret = (ucaInBuff[iLength - 2] << 8) + ucaInBuff[iLength - 1];

    *puiOutDataLen = iLength - 2 - 3;

#if _DEBUG
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

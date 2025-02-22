#include <windows.h>
#include <ntddscsi.h>
#include <setupapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <winioctl.h>

#include "../../comm.h"

#include "ZaykSkf_api.h"
#include "ZaykSkfDebug.h"

#define CDROM_M 1
//
typedef struct _SCSI_PASS_THROUGH_WITH_BUFFERS {
    SCSI_PASS_THROUGH spt;
    ULONG Filler; // realign buffers to double word boundary
    UCHAR ucSenseBuf[32];
    UCHAR ucDataBuf[512];
} SCSI_PASS_THROUGH_WITH_BUFFERS, *PSCSI_PASS_THROUGH_WITH_BUFFERS;

typedef struct _SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER {
    SCSI_PASS_THROUGH_DIRECT sptd;
    ULONG Filler; // realign buffer to double word boundary
    UCHAR ucSenseBuf[32];
} SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, *PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER;

typedef struct _USB_DEVICE_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    USHORT bcdUSB;
    UCHAR bDeviceClass;
    UCHAR bDeviceSubClass;
    UCHAR bDeviceProtocol;
    UCHAR bMaxPacketSize0;
    USHORT idVendor;
    USHORT idProduct;
    USHORT bcdDevice;
    UCHAR iManufacturer;
    UCHAR iProduct;
    UCHAR iSerialNumber;
    UCHAR bNumConfigurations;
} USB_DEVICE_DESCRIPTOR, *PUSB_DEVICE_DESCRIPTOR;

#if CDROM_M
DEFINE_GUID(CdRomClassGuid, 0x53f56308L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
#define GUID_GLOBAL CdRomClassGuid
#else
DEFINE_GUID(DiskClassGuid, 0x53f56307L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
#define GUID_GLOBAL DiskClassGuid
#endif

static int MassStorageInquiry(HANDLE fileHandle, unsigned char *buf)
{
    BOOL status = 0;
    DWORD accessMode = 0, shareMode = 0;
    PUCHAR dataBuffer = NULL;
    SCSI_PASS_THROUGH_WITH_BUFFERS sptwb;
    ULONG length = 0, errorCode = 0, returned = 0, sectorSize = 512;

    ZeroMemory(&sptwb, sizeof(SCSI_PASS_THROUGH_WITH_BUFFERS));

    sptwb.spt.Length = sizeof(SCSI_PASS_THROUGH);
    sptwb.spt.PathId = 0;
    sptwb.spt.TargetId = 1;
    sptwb.spt.Lun = 0;
    sptwb.spt.CdbLength = 6;
    sptwb.spt.SenseInfoLength = 24;
    sptwb.spt.DataIn = SCSI_IOCTL_DATA_IN;
    sptwb.spt.DataTransferLength = 0x24;
    sptwb.spt.TimeOutValue = 2;
    sptwb.spt.DataBufferOffset = offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS, ucDataBuf);
    sptwb.spt.SenseInfoOffset = offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS, ucSenseBuf);
    sptwb.spt.Cdb[0] = 0x12; // inquiry
    sptwb.spt.Cdb[4] = 36;
    length = offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS, ucDataBuf) + sptwb.spt.DataTransferLength;

    status = DeviceIoControl(fileHandle, IOCTL_SCSI_PASS_THROUGH, &sptwb, sizeof(SCSI_PASS_THROUGH), &sptwb, length, &returned, FALSE);
    memcpy(buf, sptwb.ucDataBuf, 0x24);
    return status; // if success,nonzero
}

static int GetDevicePath(int num, LPTSTR path, LPGUID pGuid)
{
    HANDLE hDev = INVALID_HANDLE_VALUE;
    HDEVINFO hardwareDeviceInfo;
    SP_INTERFACE_DEVICE_DATA deviceInfoData;
    PUSB_DEVICE_DESCRIPTOR usbDeviceInst;
    PUSB_DEVICE_DESCRIPTOR *UsbDevices = &usbDeviceInst;

    PSP_INTERFACE_DEVICE_DETAIL_DATA functionClassDeviceData = NULL;
    ULONG predictedLength = 0;
    ULONG requiredLength = 0;

    BOOL bResult = TRUE;

    hardwareDeviceInfo = SetupDiGetClassDevs(pGuid,
                                             NULL,                     // Define no enumerator (global)
                                             NULL,                     // Define no
                                             (DIGCF_PRESENT |          // Only Devices present
                                              DIGCF_INTERFACEDEVICE)); // Function class devices.

    deviceInfoData.cbSize = sizeof(SP_INTERFACE_DEVICE_DATA);

    bResult = FALSE;
    bResult = SetupDiEnumDeviceInterfaces(hardwareDeviceInfo,
                                          0, // We don't care about specific PDOs
                                          pGuid, num, &deviceInfoData);
    if (bResult) {
        bResult = SetupDiGetInterfaceDeviceDetail(hardwareDeviceInfo, &deviceInfoData,
                                                  NULL, // probing so no output buffer yet
                                                  0,    // probing so output buffer length of zero
                                                  &requiredLength,
                                                  NULL); // not interested in the specific dev-node

        predictedLength = requiredLength;

        functionClassDeviceData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(predictedLength);
        functionClassDeviceData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

        bResult = SetupDiGetInterfaceDeviceDetail(hardwareDeviceInfo, &deviceInfoData, functionClassDeviceData, predictedLength, &requiredLength, NULL);
        if (bResult) {
            // strcpy(path, functionClassDeviceData->DevicePath);
            strcpy_s(path, 254, functionClassDeviceData->DevicePath);
        }
        free(functionClassDeviceData);
    }

    SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);

    return bResult;
}

typedef struct {
    HANDLE hdl;
} comm_user_data_t;

static void *dev_open(int num)
{
    LPTSTR path;
    int i;
    HANDLE hDev;
    int bResult = FALSE;
    GUID guid;
    int rv;
    comm_user_data_t *data;
    int cnt = 0;

    STORAGE_PROPERTY_QUERY query;
    unsigned char buf[512];

    query.PropertyId = StorageDeviceProperty;
    query.QueryType = PropertyStandardQuery;

    guid = GUID_GLOBAL;

    path = (LPTSTR)malloc(256);

    for (i = 0; i < 256; i++) {
        rv = GetDevicePath(i, path, &guid);
        if (rv == 0) {
            goto __error;
        }

        hDev = CreateFile(path, GENERIC_READ | GENERIC_WRITE, 0,
                          NULL,          // no SECURITY_ATTRIBUTES structure
                          OPEN_EXISTING, // No special create flags
                          0,             // No special attributes
                          NULL);
        if (INVALID_HANDLE_VALUE == hDev) {
        }

        rv = MassStorageInquiry(hDev, buf);
        if (rv != 0) {
            if (!(memcmp(buf + 8, "ZaSec", 5) || memcmp(buf + 16, "USBKEY          ", 16) || memcmp(buf + 32, "1.01", 4))) {
#ifdef _DEBUG
                LOGI("USBKEY is SCSI\r\n");
#endif
                if (++cnt == num) {
                    data = (comm_user_data_t *)malloc(sizeof(comm_user_data_t));
                    data->hdl = hDev;
                    // CloseHandle(hDev);
                    free(path);
                    return data;
                }
            }
        }
        CloseHandle(hDev);
    }

__error:
    free(path);

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

static int dev_xfer(void *ctx, uint8_t *pucaCmd, uint8_t *pucaInData, unsigned int uiInDataLen, uint8_t *pucaOutData, unsigned long *puiOutDataLen)
{
    BOOL status;
    unsigned int len;
    unsigned char CMD_W1[] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // write cmd
    unsigned char CMD_W2[] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // read cmd
    unsigned char DataBuf[5200] = {0};
    unsigned char outDataBuf[5200] = {0};

    comm_user_data_t *data;

    data = ctx;
    if (data == NULL) {
        return SAR_DEV_COMM;
    }

    SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER sptdwb;

    ULONG length, returned;
    ZeroMemory(&sptdwb, sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER));

    sptdwb.sptd.Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
    sptdwb.sptd.PathId = 0;
    sptdwb.sptd.TargetId = 1;
    sptdwb.sptd.Lun = 0;
    sptdwb.sptd.CdbLength = 10;
    sptdwb.sptd.SenseInfoLength = 24;
    sptdwb.sptd.DataIn = SCSI_IOCTL_DATA_OUT;
    sptdwb.sptd.DataTransferLength = 3 + 7 + uiInDataLen;
    sptdwb.sptd.TimeOutValue = 1500;
    sptdwb.sptd.DataBuffer = DataBuf;
    sptdwb.sptd.SenseInfoOffset = offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, ucSenseBuf);
    sptdwb.sptd.Cdb[0] = 0xfe; // out
    memcpy(sptdwb.sptd.Cdb + 1, CMD_W1, 9);
    sptdwb.sptd.Cdb[8] = (unsigned char)(uiInDataLen + 7);
    sptdwb.sptd.Cdb[7] = (unsigned char)((uiInDataLen + 7) >> 8);

    DataBuf[0] = 0x12;
    DataBuf[1] = (unsigned char)((uiInDataLen + 7) >> 8);
    DataBuf[2] = (unsigned char)(uiInDataLen + 7);
    memcpy(DataBuf + 3, pucaCmd, 7);
    memcpy(DataBuf + 3 + 7, pucaInData, uiInDataLen);

    length = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);

#if _DEBUG
    xprint_hex("cbw out\n", 0, sptdwb.sptd.Cdb, sizeof(sptdwb.sptd.Cdb));
    xprint_hex("out\n", 0, DataBuf, uiInDataLen + 3 + 7);
#endif

    status = DeviceIoControl(data->hdl, IOCTL_SCSI_PASS_THROUGH_DIRECT, &sptdwb, sizeof(SCSI_PASS_THROUGH_DIRECT), &sptdwb, length, &returned, FALSE);

    DWORD error = GetLastError();

    if ((sptdwb.sptd.ScsiStatus == 0) && (status != 0)) {
    } else {
        return SAR_DEV_COMM;
    }

    ZeroMemory(&sptdwb, sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER));

    sptdwb.sptd.Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
    sptdwb.sptd.PathId = 0;
    sptdwb.sptd.TargetId = 1;
    sptdwb.sptd.Lun = 0;
    sptdwb.sptd.CdbLength = 10;
    sptdwb.sptd.SenseInfoLength = 24;
    sptdwb.sptd.DataIn = SCSI_IOCTL_DATA_IN;
    sptdwb.sptd.DataTransferLength = 5200; // 3+4096+2;
    sptdwb.sptd.TimeOutValue = 60;
    sptdwb.sptd.DataBuffer = outDataBuf;
    sptdwb.sptd.SenseInfoOffset = offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, ucSenseBuf);
    sptdwb.sptd.Cdb[0] = 0xfe; // in
    memcpy(sptdwb.sptd.Cdb + 1, CMD_W2, 9);

    length = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);

#if _DEBUG
    xprint_hex("cbw out\n", 0, sptdwb.sptd.Cdb, sizeof(sptdwb.sptd.Cdb));
#endif

    status = DeviceIoControl(data->hdl, IOCTL_SCSI_PASS_THROUGH_DIRECT, &sptdwb, sizeof(SCSI_PASS_THROUGH_DIRECT), &sptdwb, length, &returned, FALSE);

    if (status != 0) {

#if _DEBUG
        xprint_hex("in\n", 0, outDataBuf, sptdwb.sptd.DataTransferLength);
#endif

        len = sptdwb.sptd.DataTransferLength - 3;
        *puiOutDataLen = len - 2;
        memcpy(pucaOutData, outDataBuf + 3, len - 2);
        return outDataBuf[sptdwb.sptd.DataTransferLength - 2] * 256 + outDataBuf[sptdwb.sptd.DataTransferLength - 1];
    }

    return SAR_DEV_COMM;
}

comm_ops_t usbkey_msc_ops = {
    //
    .open = dev_open,
    .close = dev_close,
    .xfer = dev_xfer,
};

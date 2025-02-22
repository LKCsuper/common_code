#include <windows.h>
#include <iphlpapi.h>
#include <stdio.h>

#include <stdint.h>

#pragma comment(lib, "iphlpapi.lib")

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int msecond;
} DateTime;

#ifdef _WIN32
int get_current_datetime(DateTime *datetime)
{
    SYSTEMTIME stm;
    GetSystemTime(&stm);

    datetime->year = stm.wYear;
    datetime->month = stm.wMonth;
    datetime->day = stm.wDay;
    datetime->hour = stm.wHour;
    datetime->minute = stm.wMinute;
    datetime->second = stm.wSecond;
    datetime->msecond = stm.wMilliseconds;

    return 0;
}
#else
int get_current_datetime(DateTime *datetime)
{
    struct timeval stv;
    struct tm *stm;

    gettimeofday(&stv, NULL);

    stm = localtime(&stv.tv_sec);

    datetime->year = stm->tm_year + 1900;
    datetime->month = stm->tm_mon + 1;
    datetime->day = stm->tm_mday;
    datetime->hour = stm->tm_hour;
    datetime->minute = stm->tm_min;
    datetime->second = stm->tm_sec;
    datetime->msecond = stv.tv_usec / 1000;

    return 0;
}
#endif

static int demo_get_ip(uint8_t *mac)
{
    ULONG outBufLen = 0;
    DWORD dwRetVal = 0;
    PIP_ADAPTER_INFO pAdapterInfo = NULL;
    PIP_ADAPTER_INFO pAdapter = NULL;
    int cnt = 0;

    // 首先，获取需要的缓冲区大小
    if (GetAdaptersInfo(NULL, &outBufLen) == ERROR_BUFFER_OVERFLOW) {
        pAdapterInfo = (IP_ADAPTER_INFO *)malloc(outBufLen);
    }

    // 获取适配器信息
    if (pAdapterInfo == NULL) {
        printf("Memory allocation failed for IP_ADAPTER_INFO.\n");
        return 1;
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &outBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        printf("Adapter\t\t\tMAC Address\n");
        printf("------\t\t\t-----------\n");

        do {
            // 检查网卡类型，只打印实体网卡
            if (pAdapter->Type == MIB_IF_TYPE_ETHERNET) {
                printf("%s\t\t\t%02X-%02X-%02X-%02X-%02X-%02X\n", pAdapter->AdapterName, pAdapter->Address[0],
                       pAdapter->Address[1], pAdapter->Address[2], pAdapter->Address[3], pAdapter->Address[4],
                       pAdapter->Address[5]);
                if (cnt == 0) {
                    memcpy(mac, pAdapter->Address, 6);
                }
                cnt++;
            }

            pAdapter = pAdapter->Next;
        } while (pAdapter != NULL);
    } else {
        printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
    }

    if (pAdapterInfo != NULL) {
        free(pAdapterInfo);
    }

    if (cnt) {
        return 0;
    } else {
        return -1;
    }
}

int get_system_uuid(char *uuid, uint16_t len)
{
    int ret;
    uint8_t mac[6];
    DateTime dt;

    ret = demo_get_ip(mac);
    if (ret) {
        return -1;
    }

    ret = get_current_datetime(&dt);
    if (ret) {
        return -1;
    }

    snprintf(uuid, len, "%04d%02d%02d%02d%02d%02d%03d_%02X%02X%02X%02X%02X%02X", dt.year, dt.month, dt.day, dt.hour,
             dt.minute, dt.second, dt.msecond, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    // printf("uuid: %s\r\n", uuid);

    return 0;
}

#include <stdint.h>
#include <intrin.h>

int demo_get_cpuid(uint8_t *cpuid)
{
    DWORD dwInfo[4];
    char cpuid_str[0x20] = {0};

    __cpuid(dwInfo, 0x80000000);

    // 检查我们是否能够获取扩展的CPUID信息
    if (dwInfo[0] >= 0x80000004) {
        // 获取CPU序列号
        __cpuid(dwInfo, 0x80000002);
        printf("CPU ID 1: %08X %08X %08X %08X\n", dwInfo[0], dwInfo[1], dwInfo[2], dwInfo[3]);

        __cpuid(dwInfo, 0x80000003);
        printf("CPU ID 2: %08X %08X %08X %08X\n", dwInfo[0], dwInfo[1], dwInfo[2], dwInfo[3]);

        __cpuid(dwInfo, 0x80000004);
        printf("CPU ID 3: %08X %08X %08X %08X\n", dwInfo[0], dwInfo[1], dwInfo[2], dwInfo[3]);
    } else {
        printf("Extended CPUID information is not available.\n");
        return -1;
    }

    return 0;
}

#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "libhttp.h"

int http_simple_request(char *url, char *req_data, uint32_t *resp_code, uint32_t resp_head_len, char *resp_head,
                        uint32_t *resp_data_len, uint8_t *resp_data)
{
    int result = 0;
    const char *ca_file = NULL;
    const char *client_file = NULL;
    int retry_max = 3;
    int timeout = 30;
    unsigned int len;

    URL_HANDLE url_handle = NULL;
    result = url_ctx_init(&url_handle);
    char *resp_data_temp = NULL;

    result = http_parse_url(url_handle, url);

    if (0 == strncmp(url, "https://", strlen("https://"))) {
        result = https_client_build_ssl(url_handle);
        result = https_client_set_ca_file(url_handle, ca_file);
    }

    // result = https_client_set_key_file(url_handle, NULL, NULL);
    result = http_connect(url_handle, retry_max, timeout);

    // result = http_request_add_body(url_handle, "E:/Admin/Desktop/url_test.txt", 10);
    result = http_request_add_header_keepalive(url_handle, 0);

    if (req_data) {
        result = http_request_add_body(url_handle, req_data, strlen(req_data));
    }

    http_request_add_header_content_type(url_handle, "application/json");

    result = http_request_execute(url_handle, req_data == NULL ? 0 : 1, 0, NULL);

    result = url_ctx_resp_data0(url_handle, &resp_data_temp, 0);

    result = url_ctx_resp_length(url_handle, &len);
    if (resp_data_len) {
        *resp_data_len = len;
    }
    if (resp_data) {
        result = url_ctx_resp_data0(url_handle, &resp_data, 1);
    }

    // printf("====:\n%.*s\n", len, resp_data_temp);

    url_ctx_cleanup(url_handle);

    return result;
}

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// extern void debug_printf(const char *fmt, ...);
#define PRINT_HEX_PRINTF(...) printf(__VA_ARGS__)

// send@1
// 0000: 7E 25 02 00 00 7D 02 02  00 00 2B 48 54 00 47 56  |~%...}.. ..+HT.GV|
// 0010: 35 55 5C 00 00 00 00 00  00 00 00 00 00 00 00 00  |5U\..... ........|
// 0020: 00 00 00 00 00 00 00 00  00 20 01 14 16 31 00 E1  |........ . ...1..|
// 0030: 0D 02 00 0F 0A 00 F0 58  53 15 0C DA BD 05 61 7D  |.......X S.....a}|
// 0040: 02 E8 5A 7F                                       |..Z.             |
// DEC:00068 HEX:0044
void print_hex(char *title, int offset, void *in_data, int dataLen, ...)
{
    int i = 0;
    int lineLen;
    char tmp_buff[20];
    char *buf;
    int len = dataLen;
    char title_buff[64];
    unsigned char *data = in_data;

    if (title != NULL) {
        va_list arp;

        va_start(arp, dataLen);
        vsnprintf(title_buff, sizeof(title_buff), title, arp);
        va_end(arp);
        PRINT_HEX_PRINTF("%s", title_buff);
    }

    while (1) {
        buf = tmp_buff;

        lineLen = (dataLen > 16 ? 16 : dataLen);
        PRINT_HEX_PRINTF("%06X:", offset);

        for (i = 0; i < 16; i++) {
            if (i < lineLen) {
                PRINT_HEX_PRINTF(" %02X", data[i]);
                if (data[i] < 0x20 || data[i] > 0x7e) {
                    *buf++ = '.';
                } else {
                    *buf++ = data[i];
                }
            } else {
                PRINT_HEX_PRINTF("   ");
                *buf++ = ' ';
            }

            if (i == 7) {
                PRINT_HEX_PRINTF(" ");
                *buf++ = ' ';
            }
        }

        *buf++ = '\0';
        PRINT_HEX_PRINTF("  |%s|\r\n", tmp_buff);

        offset += lineLen;
        dataLen -= lineLen;
        data += lineLen;

        if (lineLen < 16 || dataLen == 0) {
            PRINT_HEX_PRINTF("DEC:%05d HEX:%04X\r\n\r\n", len, len);
            return;
        }
    }
}

// md5sum:
// 7EC00100007D02800100054854004758
void print_buff(char *title, void *buf, int dataLen, ...)
{
    int i;
    char title_buff[64];
    unsigned char *data = buf;

    if (title != NULL) {
        va_list arp;

        va_start(arp, dataLen);
        vsnprintf(title_buff, sizeof(title_buff), title, arp);
        va_end(arp);

        PRINT_HEX_PRINTF("%s", title_buff);
    }

    for (i = 0; i < dataLen; i++) {
        PRINT_HEX_PRINTF("%02X", data[i]);
    }

    PRINT_HEX_PRINTF("\r\n\r\n");
}

static const char encode_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
    'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};

static const char decode_table[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x3e, 0xff, 0xff, 0xff, 0x3f, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c,
    0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
    0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a,
    0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff,
};

int base64_encode(const char *in, int len, char *out)
{
    int i, j, s;
    char l, c;

    for (i = j = 0, s = 0, l = 0; i < len; i++) {
        c = in[i];
        switch (s) {
            case 0:
                s = 1;
                out[j++] = encode_table[(c >> 2) & 0x3f];
                break;
            case 1:
                s = 2;
                out[j++] = encode_table[((l & 0x3) << 4) | ((c >> 4) & 0xf)];
                break;
            case 2:
                s = 0;
                out[j++] = encode_table[((l & 0xf) << 2) | ((c >> 6) & 0x3)];
                out[j++] = encode_table[c & 0x3f];
                break;
            default:
                break;
        }
        l = c;
    }
    switch (s) {
        case 1:
            out[j++] = encode_table[(l & 0x3) << 4];
            out[j++] = '=';
            out[j++] = '=';
            break;
        case 2:
            out[j++] = encode_table[(l & 0xf) << 2];
            out[j++] = '=';
            break;
        default:
            break;
    }
    out[j] = '\0';
    return j;
}

int base64_decode(const char *in, int len, char *out)
{
    int i, j;
    char c;

    for (i = j = 0; i < len; i++) {
        if (in[i] == '=') {
            break;
        }
        if ((in[i] < '+') || (in[i] > 'z')) {
            return 0;
        }
        c = decode_table[(unsigned char)in[i]];
        if (c == 0xff) {
            return 0;
        }
        switch (i & 0x3) {
            case 0:
                out[j] = (c << 2) & 0xff;
                break;
            case 1:
                out[j++] |= (c >> 4) & 0x3;
                out[j] = (c & 0xF) << 4;
                break;
            case 2:
                out[j++] |= (c >> 2) & 0xf;
                out[j] = (c & 0x3) << 6;
                break;
            case 3:
                out[j++] |= c;
                break;
            default:
                break;
        }
    }
    return j;
}

void ByteToHexStr(const unsigned char *source, char *dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;

    for (i = 0; i < sourceLen; i++) {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f;

        if (highByte >= 10) {
            dest[i * 2] = highByte - 10 + 'A';
        } else {
            dest[i * 2] = highByte + '0';
        }

        if (lowByte >= 10) {
            dest[i * 2 + 1] = lowByte - 10 + 'A';
        } else {
            dest[i * 2 + 1] = lowByte + '0';
        }
    }
    return;
}

int HexStrToByte(const char *source, unsigned char *dest, unsigned int *puiDestLen)
{
    unsigned int i, k = 0;
    unsigned char HByte, LByte;
    unsigned int uiInStrLen;

    uiInStrLen = strlen(source);
    if (uiInStrLen % 2 != 0) {
        return -1;
    }

    for (i = 0; i < uiInStrLen; i = i + 2) {
        if (source[i] >= '0' && source[i] <= '9') {
            HByte = source[i] - '0';
        } else if (source[i] >= 'A' && source[i] <= 'F') {
            HByte = source[i] + 10 - 'A';
        } else if (source[i] >= 'a' && source[i] <= 'f') {
            HByte = source[i] + 10 - 'a';
        } else {
            HByte = source[i];
            return -1;
        }
        HByte = HByte << 4;
        HByte = HByte & 0xF0;
        if (source[i + 1] >= '0' && source[i + 1] <= '9') {
            LByte = source[i + 1] - '0';
        } else if (source[i + 1] >= 'A' && source[i + 1] <= 'F') {
            LByte = source[i + 1] + 10 - 'A';
        } else if (source[i + 1] >= 'a' && source[i + 1] <= 'f') {
            LByte = source[i + 1] + 10 - 'a';
        } else {
            LByte = source[i];
            return -1;
        }
        dest[k++] = HByte | LByte;
    }
    if (puiDestLen) {
        *puiDestLen = k;
    }

    return 0;
}

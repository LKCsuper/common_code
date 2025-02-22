#ifndef __TOOL_H__
#define __TOOL_H__

#include <stdint.h>

extern int get_system_uuid(char *uuid, uint16_t len);

extern int http_simple_request(char *url, char *req_data, uint32_t *resp_code, uint32_t resp_head_len, char *resp_head,
                               uint32_t *resp_data_len, uint8_t *resp_data);

/**
 * @brief 相当于 hexdump -C file.txt
 * @note
 * @param  *title:
 * @param  offset:
 * @param  *data:
 * @param  data_len:
 * @retval None
 */
extern void print_hex(char *title, int offset, void *data, int dataLen, ...);

/**
 * @brief 用于打印 md5 sha256 等密钥
 * @note
 * @param  *title:
 * @param  *data:
 * @param  data_len:
 * @retval None
 */
extern void print_buff(char *title, void *data, int dataLen, ...);

extern int base64_encode(const char *in, int len, char *out);

extern int base64_decode(const char *in, int len, char *out);

extern void ByteToHexStr(const unsigned char *source, char *dest, int sourceLen);

extern int HexStrToByte(const char *source, unsigned char *dest, unsigned int *puiDestLen);

#endif

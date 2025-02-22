/*
 * Copyright 2008-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <string.h>
#include "symm_modes.h"

int CRYPTO_bc_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                      unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                      unsigned char *out, block128_f block)
{
    size_t n;
    int ret = 0;
    int num, i;
    const unsigned char *iv = pucIv;
    unsigned char tmpBuf[16];
    unsigned char outData[16];
    unsigned char tmpIv[16];

    if (len == 0) {
        return -1;
    }

    while (len) {
        num = uiIvLen > 8 ? 16 : 8;

        for (i = 0; i < num; i++) {
            outData[i] = in[i] ^ iv[i];
        }

        ret = (*block)(hdl, uiAlgID, (unsigned char *)pucaKey, keyLen, outData, num, tmpBuf);
        if (ret != 0) {
            return ret;
        }

        for (i = 0; i < num; i++) {
            tmpIv[i] = tmpBuf[i] ^ iv[i];
        }
        iv = tmpIv;


        len -= num;
        in += num;

        memcpy(out, tmpBuf, num);
        out += num;
    }

    return 0;
}

#if 1
int CRYPTO_bc_decrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                      unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                      unsigned char *out, block128_f block)
{
    int ret = 0;
    int num, i;
    unsigned char tmpBuf[16];
    unsigned char outData[16];
    unsigned char *ivec = pucIv;
    unsigned char tmpIv[16];

    if (len == 0) {
        return -1;
    }

    while (len) {
        unsigned char c;
        num = uiIvLen > 8 ? 16 : 8;

        ret = (*block)(hdl, uiAlgID, (unsigned char *)pucaKey, keyLen, (unsigned char *)in, num, tmpBuf);
        if (ret != 0) {
            return ret;
        }

        for (i = 0; i < num; i++) {
            c = in[i];
            outData[i] = tmpBuf[i] ^ ivec[i];
            tmpIv[i] = c ^ ivec[i];
        }
        ivec = tmpIv;

        len -= num;
        in += num;

        memcpy(out, outData, num);
        out += num;
    }

    return 0;
}
#endif
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

/*
 * The input and output encrypted as though 128bit ofb mode is being used.
 * The extra state information to record how much of the 128bit block we have
 * used is contained in *num;
 */
int CRYPTO_ofbnlf_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                          unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                          unsigned char *out, block128_f block1, block128_f block2)
{
    int ret = 0;
    int num, i;
    unsigned char *iv = (unsigned char *)pucIv;
    unsigned char *tmpKey;
    unsigned int tmpKeyLen;
    unsigned char outData[16];
    unsigned char tempBuf[16];

    if (len == 0) {
        return -1;
    }

    tmpKey = (unsigned char *)pucaKey;
    tmpKeyLen = keyLen;

    while (len) {
        num = len > uiIvLen ? uiIvLen : len;

        ret = (*block1)(hdl, uiAlgID, tmpKey, tmpKeyLen, (unsigned char *)iv, uiIvLen, tempBuf);
        if (ret != 0) {
            return ret;
        }

        ret = (*block2)(hdl, uiAlgID, tempBuf, tmpKeyLen, (unsigned char *)in, num, outData);
        if (ret != 0) {
            return ret;
        }

        memcpy(out, outData, num);

        iv = tempBuf;
        uiIvLen = num;

        len -= num;
        in += num;
        out += num;
    }

    return 0;
}

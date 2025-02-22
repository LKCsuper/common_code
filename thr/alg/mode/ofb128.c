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
int CRYPTO_ofb_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                           unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                           unsigned char *out, block128_f block)
{
    int ret = 0;
    int num, i;
    const unsigned char *iv = pucIv;
    unsigned char outData[16];
    unsigned char tempBuf[16];

    if (len == 0) 
        return -1;
    
    while (len) {
        ret = (*block)(hdl, uiAlgID, (unsigned char *)pucaKey, keyLen, (unsigned char *)iv, uiIvLen, tempBuf);
        if (ret != 0) {
            return ret;
        }

        num = len > uiIvLen ? uiIvLen : len;

        for (i = 0; i < num; i++) {
            outData[i] = tempBuf[i] ^ in[i];
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

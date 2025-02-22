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
 * The input and output encrypted as though 128bit cfb mode is being used.
 * The extra state information to record how much of the 128bit block we have
 * used is contained in *num;
 */
int CRYPTO_cfb_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
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

        iv = outData;
        uiIvLen = num;

        len -= num;
        in += num;
        out += num;
    }

    return 0;
}

int CRYPTO_cfb_decrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keyLen,
                           unsigned char *pucIv, unsigned int uiIvLen, const unsigned char *in, size_t len,
                           unsigned char *out, block128_f block)
{
    int ret = 0;
    int num, i;
    unsigned char *iv = pucIv;
    unsigned char outData[16];
    unsigned char tempBuf[16];

    if (len == 0) 
        return -1;
    
    while (len) {
        unsigned char c;
        ret = (*block)(hdl, uiAlgID, (unsigned char *)pucaKey, keyLen, (unsigned char *)iv, uiIvLen, tempBuf);
        if (ret != 0) {
            return ret;
        }

        num = len > uiIvLen ? uiIvLen : len;

        for (i = 0; i < num; i++) {
            c = in[i];
            outData[i] = tempBuf[i] ^ in[i];
            iv[i] = c;
        }
        memcpy(out, outData, num);

        uiIvLen = num;

        len -= num;
        in += num;
        out += num;
    }

    return 0;
}

/*
 * This expects a single block of size nbits for both in and out. Note that
 * it corrupts any extra bits in the last byte of out
 */
static void cfbr_encrypt_block(const unsigned char *in, unsigned char *out,
                               int nbits, const void *key,
                               unsigned char ivec[16], int enc,
                               block128_f block)
{
    int n, rem, num;
    unsigned char ovec[16 * 2 + 1]; /* +1 because we dereference (but don't
                                     * use) one byte off the end */

    if (nbits <= 0 || nbits > 128)
        return;

    /* fill in the first half of the new IV with the current IV */
    memcpy(ovec, ivec, 16);
    /* construct the new IV */
    // (*block) (ivec, ivec, key);
    num = (nbits + 7) / 8;
    if (enc)                    /* encrypt the input */
        for (n = 0; n < num; ++n)
            out[n] = (ovec[16 + n] = in[n] ^ ivec[n]);
    else                        /* decrypt the input */
        for (n = 0; n < num; ++n)
            out[n] = (ovec[16 + n] = in[n]) ^ ivec[n];
    /* shift ovec left... */
    rem = nbits % 8;
    num = nbits / 8;
    if (rem == 0)
        memcpy(ivec, ovec + num, 16);
    else
        for (n = 0; n < 16; ++n)
            ivec[n] = ovec[n + num] << rem | ovec[n + num + 1] >> (8 - rem);

    /* it is not necessary to cleanse ovec, since the IV is not secret */
}

/* N.B. This expects the input to be packed, MS bit first */
void CRYPTO_cfb128_1_encrypt(const unsigned char *in, unsigned char *out,
                             size_t bits, const void *key,
                             unsigned char ivec[16], int *num,
                             int enc, block128_f block)
{
    size_t n;
    unsigned char c[1], d[1];

    for (n = 0; n < bits; ++n) {
        c[0] = (in[n / 8] & (1 << (7 - n % 8))) ? 0x80 : 0;
        cfbr_encrypt_block(c, d, 1, key, ivec, enc, block);
        out[n / 8] = (out[n / 8] & ~(1 << (unsigned int)(7 - n % 8))) |
            ((d[0] & 0x80) >> (unsigned int)(n % 8));
    }
}

void CRYPTO_cfb128_8_encrypt(const unsigned char *in, unsigned char *out,
                             size_t length, const void *key,
                             unsigned char ivec[16], int *num,
                             int enc, block128_f block)
{
    size_t n;

    for (n = 0; n < length; ++n)
        cfbr_encrypt_block(&in[n], &out[n], 8, key, ivec, enc, block);
}

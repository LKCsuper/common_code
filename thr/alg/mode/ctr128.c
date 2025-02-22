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
 * NOTE: the IV/counter CTR mode is big-endian.  The code itself is
 * endian-neutral.
 */

/* increment counter (128-bit int) by 1 */
static void ctr128_inc(unsigned char *counter)
{
    unsigned int n = 16, c = 1;

    do {
        --n;
        c += counter[n];
        counter[n] = (unsigned char)c;
        c >>= 8;
    } while (n);
}

// #if !defined(OPENSSL_SMALL_FOOTPRINT)
static void ctr128_inc_aligned(unsigned char *counter)
{
    size_t *data, c, d, n;
    const union {
        long one;
        char little;
    } is_endian = {1};

    if (is_endian.little || ((size_t)counter % sizeof(size_t)) != 0) {
        ctr128_inc(counter);
        return;
    }

    data = (size_t *)counter;
    c = 1;
    n = 16 / sizeof(size_t);
    do {
        --n;
        d = data[n] += c;
        /* did addition carry? */
        c = ((d - c) & ~d) >> (sizeof(size_t) * 8 - 1);
    } while (n);
}
// #endif

/*
 * The input encrypted as though 128bit counter mode is being used.  The
 * extra state information to record how much of the 128bit block we have
 * used is contained in *num, and the encrypted counter is kept in
 * ecount_buf.  Both *num and ecount_buf must be initialised with zeros
 * before the first call to CRYPTO_ctr128_encrypt(). This algorithm assumes
 * that the counter is in the x lower bits of the IV (ivec), and that the
 * application has full control over overflow and the rest of the IV.  This
 * implementation takes NO responsibility for checking that the counter
 * doesn't overflow into the rest of the IV when incremented.
 */
int CRYPTO_ctr128_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keylen,
                          unsigned char counter[16], const unsigned char *in, size_t len, unsigned char *out,
                          block128_f block)
{
    int n;
    int ret = 0;

    unsigned char tmp_buf[16];

    while (len) {
        ret = (*block)(hdl, uiAlgID, (unsigned char *)pucaKey, keylen, counter, 16, tmp_buf);
        if (ret != 0) {
            return ret;
        }
        ctr128_inc_aligned(counter);
        for (n = 0; n < 16; n += sizeof(size_t)) {
            *(size_t *)(out + n) = *(size_t *)(in + n) ^ *(size_t *)(tmp_buf + n);
        }
        len -= 16;
        out += 16;
        in += 16;
    }

    return 0;
}

// // counter默认为大端模式
// static void ctr_inc(unsigned char *counter, unsigned int count_len)
// {
//     unsigned int c = 1;
//     int i;

//     for (i = count_len - 1; i >= 0; i--) {
//         c += counter[i];
//         counter[i] = (unsigned char)c;
//         c >> 8;
//     }
// }

static void ctr_inc(unsigned char *counter, unsigned int count_len)
{
    unsigned int n = count_len, c = 1;

    do {
        --n;
        c += counter[n];
        counter[n] = (unsigned char)c;
        c >>= 8;
    } while (n);
}

int CRYPTO_ctr_encrypt(void *hdl, unsigned int uiAlgID, const unsigned char *pucaKey, unsigned int keylen,
                       unsigned char *counter, unsigned int counter_len, const unsigned char *in, size_t len,
                       unsigned char *out, block128_f block)
{
    int i;
    int ret = 0;
    int num;

    unsigned char tmp_buf[16];

    while (len) {
        ret = (*block)(hdl, uiAlgID, (unsigned char *)pucaKey, keylen, counter, counter_len, tmp_buf);
        if (ret != 0) {
            return ret;
        }
        num = len > counter_len ? counter_len : len;
        for (i = 0; i < num; i++) {
            out[i] = in[i] ^ tmp_buf[i];
        }

        ctr_inc(counter, counter_len);

        len -= num;
        out += num;
        in += num;
    }

    return 0;
}

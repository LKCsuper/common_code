#ifndef __ZUC_H__
#define __ZUC_H__

#ifdef __cplusplus
extern "C"
{
#endif
    typedef unsigned char u8;

    typedef unsigned int u32;

    // int ZUC_Encrypt(u8 *Key, u32 counter, u8 bearer, u8 dir, u8 *inData, u32 inDataLen, u8 *outData, u32
    // *outDataLen);

    // int ZUC_Decrypt(u8 *Key, u32 counter, u8 bearer, u8 dir, u8 *inData, u32 inDataLen, u8 *outData, u32
    // *outDataLen);

    int ZUC_EEA_Init(u8 *CK, u32 COUNT, u8 BEARER, u8 DIRECTION, u32 expectOutBits, u32 *keyStream);
    int ZUC_EEA_Crypto(u32 *keyStream, u32 expectOutBits, u32 *inData, u32 *outData);

    int ZUC_EEA_Cipher(u8 *Key, u32 counter, u8 bearer, u8 dir, u32 *inData, u32 nbits, u32 *outData);

    int ZUC_EIA_MAC(u8 *Key, u32 counter, u8 bearer, u8 dir, u32 *inData, u32 nbits, u32 *outData);

#ifdef __cplusplus
}
#endif

#endif

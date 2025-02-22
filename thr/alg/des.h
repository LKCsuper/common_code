#ifndef __DES_H__
#define __DES_H__
#ifdef __cplusplus
extern "C"
{
#endif

    extern int des_ecb_run(int flag, unsigned char *pKey, unsigned int KeyLen, unsigned char *pInData,
                       unsigned int inDataLen, unsigned char *pOutData, unsigned int *pOutDataLen);

    extern int des_cbc_run(int flag, unsigned char *pKey, unsigned int KeyLen, unsigned char *pInData,
                           unsigned int inDataLen, unsigned char *pOutData, unsigned int *pOutDataLen,
                           unsigned char *iv);

    extern int des_cfb_run(int flag, unsigned char *pKey, unsigned int KeyLen, unsigned char *pInData,
                           unsigned int inDataLen, unsigned char *pOutData, unsigned int *pOutDataLen,
                           unsigned char *pIv);

    extern int des_ofb_run(int flag, unsigned char *pKey, unsigned int KeyLen, unsigned char *pInData,
                           unsigned int inDataLen, unsigned char *pOutData, unsigned int *pOutDataLen,
                           unsigned char *pIv);

#ifdef __cplusplus
}
#endif

#endif

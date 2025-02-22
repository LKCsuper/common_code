#include <stdio.h>

#ifndef _SM4_H_
#define _SM4_H_

#ifdef __cplusplus
extern "C" {
#endif

/**@brief  ECB模式的SM4加密
 * @param[in]  pKey					密钥
 * @param[in]  KeyLen				密钥长度，16字节。
 * @param[in]  pInData				输入数据
 * @param[in]  inDataLen			输入数据长度
 * @param[out]  pOutData			输出数据
 * @param[out]  pOutDataLen			输出数据长度
 * @return  
 * @remarks  
 *  
 */
int SM4_ECB_Encrypt( unsigned char *pKey, 
                      unsigned int KeyLen, 
                      unsigned char *pInData,
                      unsigned int inDataLen,
                      unsigned char *pOutData, 
                      unsigned int *pOutDataLen);


/**@brief  ECB模式的SM4解密
 * @param[in]  pKey				密钥
 * @param[in]  KeyLen			密钥长度，16字节。
 * @param[in]  pInData			输入数据
 * @param[in]  inDataLen		输入数据长度
 * @param[out]  pOutData		输出数据
 * @param[out]  pOutDataLen		输出数据长度
 * @return  
 * @remarks  
 *  
 */
int SM4_ECB_Decrypt(  unsigned char *pKey, 
                      unsigned int KeyLen, 
                      unsigned char *pInData, 
                      unsigned int inDataLen,
                      unsigned char *pOutData,
                      unsigned int *pOutDataLen);

/**@brief  CBC模式的SM4加密
 * @param[in]  pKey				密钥
 * @param[in]  KeyLen			密钥长度，16字节。
 * @param[in]  pIV				初始向量
 * @param[in]  ivLen			初始向量，16字节。
 * @param[in]  pInData			输入数据
 * @param[in]  inDataLen		输入数据长度
 * @param[out]  pOutData		输出数据
 * @param[out]  pOutDataLen		输出数据长度
 * @return  
 * @remarks  
 *  
 */
int SM4_CBC_Encrypt( unsigned char *pKey, 
                     unsigned int KeyLen,
                     unsigned char *pIV, 
                     unsigned int ivLen,
                     unsigned char *pInData, 
                     unsigned int inDataLen,
                     unsigned char *pOutData, 
                     unsigned int *pOutDataLen);

/**@brief  CBC模式的SM4解密
 * @param[in]  pKey				密钥
 * @param[in]  KeyLen			密钥长度，16字节。
 * @param[in]  pIV				初始向量
 * @param[in]  ivLen			初始向量，16字节。
 * @param[in]  pInData			输入数据
 * @param[in]  inDataLen		输入数据长度
 * @param[out]  pOutData		输出数据
 * @param[out]  pOutDataLen		输出数据长度
 * @return  
 * @remarks  
 *  
 */
int SM4_CBC_Decrypt(unsigned char *pKey, 
                    unsigned int KeyLen, 
                    unsigned char *pIV, 
                    unsigned int ivLen,
                    unsigned char *pInData,
                    unsigned int inDataLen,
                    unsigned char *pOutData, 
                    unsigned int *pOutDataLen);


#ifdef __cplusplus
}
#endif

#endif // _SM4_H_

#ifndef __SSF33_H__
#define __SSF33_H__

extern unsigned int SSF33_crypto(unsigned int isenc, unsigned char *pucaKey, unsigned char *pucaIv,
                                 unsigned char *pucaInData, unsigned int uiInDataLen, unsigned char *pucaOutData);

#endif

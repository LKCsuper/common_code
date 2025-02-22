
#ifndef _APP_STRING_H__
#define _APP_STRING_H__

#include "string.h" 
#include "stdint.h" 

#define CMP_SAME  0

#define VALID_SUBSTR_CNT 50

typedef struct
{
		uint16_t pos_head;
		uint16_t pos_end;	
		uint16_t len;
}MsgSub_str;

typedef struct
{
		uint8_t substr_cnt;		
		MsgSub_str substr[VALID_SUBSTR_CNT];	
}MsgStringSearch_str;


/*************************************
dataIn : 0x00---0x09  0x0a/0x0A ----0x0f/0x0F				
dataOut: '0'----'9'      a    -----    f
**************************************/
char data4bits_TO_Char_lower(char data);


/*************************************
dataIn : 0x00---0x09  0x0a/0x0A ----0x0f/0x0F
dataOut: '0'----'9'      A    -----    F
**************************************/
char data4bits_TO_Char_upper(char data);


/*************************************
dataIn : '0'----'9'      A/a  ----    F/f
dataOut: 0x00---0x09     0x0A ----    0x0F
**************************************/
char CharNum_TO_NUM(char data);


/*************************************
dataIn : 0-9   10-99   100-999  -----
dataOut:  1     2 				 3		 -----	
**************************************/
uint8_t data_length_calc(uint32_t data);

/*************************************
dataIn : char[4]
dataOut: float
**************************************/
float string_to_float(unsigned char *data_array);

/*************************************
dataIn : float
dataOut: char[4]
**************************************/
unsigned char *float_to_string(float f_data);

/*************************************
dataIn  eg: "12ab"   "123456"
dataOut eg: 0x12ab   0x123456
**************************************/
uint32_t u16string_to_data(unsigned char *buf,uint8_t length);

/*************************************
dataIn  eg: "1234"   "123456"
dataOut eg:  1234     123456
**************************************/
uint32_t u10string_to_data(unsigned char *buf,uint8_t length);

/*************************************
dataIn  eg: ABCD1248... 
dataOut eg: 0xAB,0xCD,0x12,0x48...
**************************************/
uint16_t 	memcpy_TwoChar_To_OneChar(unsigned char *target_data,unsigned char *source_data,uint16_t source_length);

uint16_t 	memcpy_OneChar_To_TwoChar(unsigned char *target_data,unsigned char *source_data,uint16_t source_length);

uint16_t 	memcpy_OneChar_To_ThreeChar(unsigned char *target_data,unsigned char *source_data,uint16_t source_length);
/*************************************
dataIn  eg: "1234"   "123456"
dataOut eg:  1234     123456
??? u10string_to_data ,???????,???????????,???;
**************************************/
uint32_t numstrToNum(unsigned char*buf);

unsigned char data_to_u10string(uint32_t data,unsigned char *str);


MsgStringSearch_str GetSubString(unsigned char *mString,uint16_t mlen,uint16_t pos,char *sString,uint16_t slen);
uint8_t SearchOneString(unsigned char *mString,uint16_t mlen,char *sString,uint16_t slen);
uint16_t  SearchOneString_returnPosition(unsigned char *mString,uint16_t mlen,char *sString,uint16_t slen);
uint8_t * HexToChars_16bits(uint16_t *pHex,unsigned char *pChars,unsigned char NoPreZero);    // 16位数据宽度的HEX数转ASCII
uint8_t  StringIsNumAndLetter(unsigned char *p,uint8_t  Lenth);
uint8_t  StringIsNumAndLetter(unsigned char *p,uint8_t  Lenth);
uint8_t  StringIsNum(unsigned char *p,uint8_t  Lenth);
#endif
					
					

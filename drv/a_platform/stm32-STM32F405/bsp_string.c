/*****************************************************************************
*	模块名称： bsp_string.c                                         	
*	程序功能：                             
*	编 写 者：                                   
*	编写日期：
                    
* 说明： 
****************************************************************************/

#include "bsp_string.h"

uint8_t SearchOneString(unsigned char *mString,uint16_t mlen,char *sString,uint16_t slen)
{
		uint16_t i;
		uint8_t flag;
		
		if	(	(mString == NULL)||(mlen == 0)
				||(sString == NULL)||(slen == 0)
				||(mlen < slen))
		return 0;		
		
		for(i=0;i<(mlen-slen)+1;i++)
		{  
				flag = memcmp(&mString[i],sString,slen);
				if(flag == CMP_SAME)
				{
					return 1;
				}
		}
		return 0;
}

//查找字符串，找到后返回查找到字符串位置
//查找失败返回0，注意对返回值的判断
uint16_t  SearchOneString_returnPosition(unsigned char *mString,uint16_t mlen,char *sString,uint16_t slen)
{
		uint16_t i;
		uint8_t flag;
		
		if	(	(mString == NULL)||(mlen == 0)
				||(sString == NULL)||(slen == 0)
				||(mlen < slen))
		return 0;		
		
		for(i=0;i<(mlen-slen)+1;i++)
		{  
				flag = memcmp(&mString[i],sString,slen);
				if(flag == CMP_SAME)
				{
					return i;
				}
		}
		return 0;
}
MsgStringSearch_str GetSubString(unsigned char *mString,uint16_t mlen,uint16_t pos,char *sString,uint16_t slen)
{
		uint16_t i;
		int cmp_flag;
    MsgStringSearch_str msgSearch;  
    
		memset(&msgSearch,0,sizeof(MsgStringSearch_str));
		
		if	(	(mString == NULL)||(mlen == 0)
				||(sString == NULL)||(slen == 0)
				||(mlen<pos)||(mlen-pos < slen))
		return msgSearch;		
		
		msgSearch.substr_cnt = 0;	
		for(i=pos;i<(mlen-slen)+1;i++)
		{  
				cmp_flag = memcmp(&mString[i],sString,slen);
				if(cmp_flag == CMP_SAME)
				{
						msgSearch.substr[msgSearch.substr_cnt].pos_head=i;
						msgSearch.substr[msgSearch.substr_cnt].pos_end=i+slen;
						if(msgSearch.substr_cnt > 0)
						{
							msgSearch.substr[msgSearch.substr_cnt-1].len = msgSearch.substr[msgSearch.substr_cnt].pos_head - msgSearch.substr[msgSearch.substr_cnt-1].pos_head;
						}
						msgSearch.substr_cnt ++;	

						if(msgSearch.substr_cnt == VALID_SUBSTR_CNT)
						{
							return msgSearch;
						}
				}
				else
				{
				
				}
		}
		msgSearch.substr[msgSearch.substr_cnt-1].len = mlen - msgSearch.substr[msgSearch.substr_cnt-1].pos_head;
		return msgSearch;
}

/************************************************************************
	*   功能描述: 	4位数据转换成小写字符            
	*   函数说明:        			                                             
	*   @@@ 输入:   数据                                 
	*   @@@ 输出：  字符    
	eg: 0b1111  -> 'F'	 0b0111  -> '7'
*************************************************************************/
char data4bits_TO_Char_lower(char data)
{
		char temp;
		temp = data&0x0f;
		if(temp<10) //0123456789
		{
				temp +=0x30;
		}
		else//abcdef
		{
				temp +=0x57;
		}
		return temp;
}
/************************************************************************
	*   功能描述: 	4位数据转换成大写字符            
	*   函数说明:        			                                              
	*   @@@ 输入:   数据                                 
	*   @@@ 输出：  字符     
	eg: 0b1111  -> 'F'	 0b0111  -> '7'
*************************************************************************/
char data4bits_TO_Char_upper(char data)
{
		char temp;
		temp = data&0x0f;
		if(temp<10) //0123456789
		{
				temp +=0x30;
		}
		else//ABCDEF
		{
				temp +=0x37;
		}
		return temp;
}
/************************************************************************
	*   功能描述: 	字符数字变数字         
	*   函数说明:   '1' -> 0x01;  'a'->0x0A; 'A'->0x0A;                     
	*   @@@ 输入:   字符                                 
	*   @@@ 输出：  数字  
eg：'1' -> 0x01;  'a'->0x0A; 'A'->0x0A;  
*************************************************************************/
char CharNum_TO_NUM(char data)
{
		char temp;
		
		if((data>0x2F) && (data<0x3A))//data = 0~9;
		{
				temp = data & 0x0f;
				return temp;
		}
		else if((data>0x40)&&(data<0x47))//data = A~F;
		{
				temp = data - 0x37;
				return temp;
		}
		else if((data>0x60)&&(data<0x67))//data = a~f;
		{
				temp = data - 0x57;
				return temp;
		}
		return 0xFF;
}

/************************************************************************
	*   功能描述: 	位数获取      
	*   函数说明:   举例  数据 3546,最大到千位。所以返回数值4。             
	*   @@@ 输入:   数据源和目标数据                                 
	*   @@@ 输出：  无         
*************************************************************************/
uint8_t data_length_calc(uint32_t data)
{
		if(data < 10) return 1;
	  if(data < 100) return 2;
	  if(data < 1000) return 3;
	  if(data < 10000) return 4;
		if(data < 100000) return 5;
		if(data < 1000000) return 6;
		if(data < 10000000) return 7;
		if(data < 100000000) return 8;
		if(data < 1000000000) return 9;
		return 10;
}

/************************************************************************
	*   功能描述: 	10进制数数据获取 
	*   函数说明:   字符串提取数据。                                     
	*   @@@ 输入:   数据源和                               
	*   @@@ 输出：  目标数据  
eg：  字符串10进制数据 “3546”,提取为data=3456。 
*************************************************************************/
uint32_t u10string_to_data(unsigned char *buf,uint8_t length)
{
		uint32_t temp_data = 0;
		uint8_t i;
		unsigned char data_uchar=0;
		for(i=0;i<length;i++)
		{
				data_uchar = *(buf+i);
				if(( data_uchar > 0x2F)&& (data_uchar < 0x3A)) //0~9;
				{
						temp_data = temp_data*10 +(data_uchar & 0x0F);
				}
				else 
				{
						return temp_data;						
				}
		}
		return temp_data;
}

/************************************************************************
	*   功能描述: 	16位数数据获取 
	*   函数说明:   字符串提出数据。                                  
	*   @@@ 输入:   数据源和长度                               
	*   @@@ 输出：  目标数据      
eg：  字符串16进制数据 “3546”,提取为data=0x3456
*************************************************************************/
uint32_t u16string_to_data(unsigned char *buf,uint8_t length)
{
		uint32_t temp_data = 0;
		uint8_t i;
		unsigned char source_uchar=0;
		for(i=0;i<length;i++)
		{
				source_uchar = *(buf+i);
				if(( source_uchar > 0x2F)&& (source_uchar < 0x3A))//0~9;
				{
						temp_data = (temp_data<<4)|(source_uchar & 0x0F);
				}
				else if(( source_uchar > 0x40)&&(source_uchar < 0x47))//A~F;
				{
						temp_data = (temp_data<<4)|(source_uchar -0x37);
				}
				else if(( source_uchar > 0x60)&&(source_uchar < 0x67))//a~f;
				{
						temp_data = (temp_data<<4)|(source_uchar -0x57);
				}
				else 
				{
						return temp_data;						
				}
		}
		return temp_data;
}
/************************************************************************
	*   功能描述: 	float转字符串
	*   函数说明:                                                              
	*   @@@ 输入:   数据源和目标数据                                 
	*   @@@ 输出：  无              
*************************************************************************/
unsigned char *float_to_string(float f_data)
{
    static  unsigned char data_str[4]={0};
		uint8_t i;
    union MyFloat
    {
      unsigned char  array[4];
      float   data;
    }storef;

    storef.data = f_data;

    for(i=0;i<4;i++)
    {
      data_str[i] = storef.array[i];
    }
    return data_str;
}

/************************************************************************
	*   功能描述: 	字符串转float
	*   函数说明:                                                            
	*   @@@ 输入:   数据源和目标数据                                 
	*   @@@ 输出：  无          
  *   @@@ 状态   In use；
*************************************************************************/
float string_to_float(unsigned char *data_array)
{
	 int i=0;
   union MyFloat
    {
      unsigned char  array[4];
      float   data;
    }storef;

		 for(i=0;i<4;i++)
		{
			storef.array[i]= data_array[i];
		}

  return storef.data;
}

/************************************************************************
	*   功能描述: 	字符串转u32
	*   函数说明:                                                              
	*   @@@ 输入:   数据源和目标数据                                 
	*   @@@ 输出：  无                
*************************************************************************/
uint32_t string_to_u32(unsigned char *data_array,uint8_t length)
{
	 switch(length)
	 {
		 case 1:
				return data_array[0];
		 case 2:
				return (data_array[1]<<8)| data_array[0];
		 case 3:
				return (data_array[2]<<16) | (data_array[1]<<8)| data_array[0];
		 case 4:
				return (data_array[3]<<24) | (data_array[2]<<16) | (data_array[1]<<8)| data_array[0];
	 }
  return 0;
}
/************************************************************************
	*   功能描述: 	字符串转s32
	*   函数说明:                                                             
	*   @@@ 输入:   数据源和目标数据                                 
	*   @@@ 输出：  无    
*************************************************************************/
int32_t string_to_s32(unsigned char *data_array)
{
    int32_t temp=0;

    temp = (data_array[3]<<24) | (data_array[2]<<16) | (data_array[1]<<8)| data_array[0];

    return temp;
}

uint16_t stringTwoChar_to_sringOneChar(unsigned char *data_array,uint16_t length)
{
		unsigned char *temp;
		uint16_t i,count;
	
		if(length%2 ==1) return 0;
		
		temp = data_array;
		count = length/2;
	
		for(i=0;i<count;i++)
		{
				*temp = u16string_to_data(data_array + (i<<1),2);
				temp++;
		}
		return count;
}

/************************************************************************
	*   功能描述: 	字符串转s32
	*   函数说明:                                                          
	*   @@@ 输入:   数据源和目标数据                                 
	*   @@@ 输出：  无          
Warning Warning two bytes = one char; 
eg: ABCD1248... => 0xAB,0xCD,0x12,0x48...
*************************************************************************/

uint16_t 	memcpy_TwoChar_To_OneChar(unsigned char *target_data,unsigned char *source_data,uint16_t source_length)
{
		uint16_t  i;
		unsigned char  bytetemp;    //seed value
		uint16_t len;
		if((source_length == 0)||(source_length %2 == 1)) return 0;
	  
		len = source_length/2;
	
		for(i=0;i<len;i++)
		{
				bytetemp = u16string_to_data(source_data+i*2,2);
				*(target_data+i) = bytetemp;
		}
		return(len);
}

uint16_t 	memcpy_OneChar_To_TwoChar(unsigned char *target_data,unsigned char *source_data,uint16_t source_length)
{
		uint16_t  i;
		unsigned char  bytetemp;    //seed value
    unsigned char  datatemp;
		uint16_t len;
		if(source_length == 0) return 0;
	  
		len = source_length;//* 2;
	
		for(i=0;i<len;i++)
		{
        datatemp = *(source_data+i);
				bytetemp = data4bits_TO_Char_upper((datatemp>>4)&0x0F);
				*(target_data+2*i) = bytetemp;
        bytetemp = data4bits_TO_Char_upper(datatemp&0x0F);
				*(target_data+2*i+1) = bytetemp;
		}
		return(len);
}

uint16_t 	memcpy_OneChar_To_ThreeChar(unsigned char *target_data,unsigned char *source_data,uint16_t source_length)
{
		uint16_t  i;
		unsigned char  bytetemp;    //seed value
    unsigned char  datatemp;
		uint16_t len;
		if(source_length == 0) return 0;
	  
		len = source_length;//* 2;
	
		for(i=0;i<len;i++)
		{
        datatemp = *(source_data+i);
				bytetemp = data4bits_TO_Char_upper((datatemp>>4)&0x0F);
				*(target_data+3*i) = bytetemp;
        bytetemp = data4bits_TO_Char_upper(datatemp&0x0F);
				*(target_data+3*i+1) = bytetemp;
        *(target_data+3*i+2) = ' ';
		}
		return(len);
}

/************************************************************************
	*   功能描述: 	10进制数数据获取 
	*   函数说明:   字符串提取数据。                                     
	*   @@@ 输入:   数据源和                               
	*   @@@ 输出：  目标数据  
eg：  字符串10进制数据 “3546”,提取为data=3456。 
类似u10string_to_data，只是缺省了length，直接去非0~9的数据为结束。
*************************************************************************/
uint32_t numstrToNum(unsigned char*buf)
{
		uint8_t i;
		unsigned char temp_data[10];
		uint32_t num = 0;
		memcpy(temp_data,buf,10);
	
		for(i=0;i<10;i++)
		{
				if((temp_data[i]>0x2F)&&(temp_data[i]<0x3A))//0~9
				{
						num = num*10 + (temp_data[i]&0x0F);
				}
				else
				{
						break;
				}
		}
		return num;
}

/***********************************************************************
 
  Mac_Addr:     0xF082C0FFFE31D7C1
  Pan_id:       0x5566
  Src_addr:     0x0003
  Dest_addr:    0xFFFF

   uint32_t u16string_to_data(unsigned char *buf,uint8_t length);
******************************************************************/


/************************************************************************
	*   功能描述: 	u16 数据转字符串
	*   函数说明:                                                              
	*   @@@ 输入:   数据源和目标数据                                 
	*   @@@ 输出：  无   
           
*************************************************************************/

unsigned char *data_to_u16x0string(uint16_t data)
{
    static  unsigned char data_str[6]={0};

    data_str[0] = '0';
    data_str[1] = 'x';
    
    data_str[2] =data4bits_TO_Char_upper((char)(data>>12));
    data_str[3] =data4bits_TO_Char_upper((char)(data>>8));
    data_str[4] =data4bits_TO_Char_upper((char)(data>>4));
    data_str[5] =data4bits_TO_Char_upper((char)(data));
    
    return data_str;
}

/************************************************************************
	*   功能描述: 	u16 数据转字符串
	*   函数说明:                                                              
	*   @@@ 输入:   数据源和目标数据                                 
	*   @@@ 输出：  无              
*************************************************************************/
//};
unsigned char data_to_u10string(uint32_t data,unsigned char *str)
{
//    static  unsigned char data_str[10]={0;

    uint8_t i;
    uint32_t temp_data = data;
    uint8_t data_len = data_length_calc(data);//10进制数一共多少位    
    
    for(i=0;i<data_len;i++)
    {
       
       *(str+data_len-1-i) = temp_data%10+0x30;
       temp_data /= 10;
    } 
//    memset(view_data_str,0,10);
//    memcpy(view_data_str,data_str,data_len);
    return data_len;
}

/************************************************************************
	*   功能描述: 	hex转ascii
	*   函数说明:    
  *   *pHex:  //待转的16位数据
  *   *pChars: 转好的ASCII
  *   NoPreZero: 有效数字前0是否消隐
  *   0: 保留有效数字前0
  *   1: 消隐有效数字前0
  *  返回：
  *  指向ascii字符串后最后一个字符\0的地址+1处                                                          
	*   @@@ 输入:   数据源和目标数据                                 
	*   @@@ 输出：  无              
*************************************************************************/
uint8_t * HexToChars_16bits(uint16_t *pHex,unsigned char *pChars,unsigned char NoPreZero)    // 16位数据宽度的HEX数转ASCII
{
    uint16_t Res;
    uint16_t Diver;
    uint8_t JumpZero ;
    
    Res = *pHex ;
    Diver = 10000 ;
    JumpZero = NoPreZero; //去前面的0的功能还没调
    
    while(Diver > 1)
    {
      *pChars = (uint8_t)(Res / Diver) + 0x30;
      Res %= Diver ;
      Diver /= 10 ;
      if(JumpZero)
      {
        if(pChars[0] != '0')
        {
          JumpZero = 0 ;
          pChars++ ;
        }
      }
      else
      {
        pChars++ ;
      }
    }
    *pChars = (uint8_t)Res + 0x30;
    pChars++;
    *pChars = '\r';
    pChars++;
    *pChars = '\n';
    pChars++;
    *pChars = '\0';
    pChars++;
    return pChars;
}
/************************************************************************
	*   功能描述: 	字符串有效性判断
	*   函数说明:                                                              
	*   @@@ 输入:   数据源和目标数据                                 
	*   @@@ 输出：  无              
*************************************************************************/
uint8_t  StringIsNumAndLetter(unsigned char *p,uint8_t  Lenth)
{
  while(Lenth--)
  {        
    if( (*(p) < '0') || (*(p) > '9') )
    {
      if((*(p) < 'A') || (*(p)  > 'Z'))
      {
        if((*(p) < 'a') || (*(p)  > 'z'))
        {
          return 0;
        }
      }
    }   
    p++;
  }
  return 1;
}
/************************************************************************
	*   功能描述: 	字符串数字判别
	*   函数说明:                                                              
	*   @@@ 输入:   数据源和目标数据                                 
	*   @@@ 输出：  无              
*************************************************************************/
uint8_t  StringIsNum(unsigned char *p,uint8_t  Lenth)
{
  while(Lenth--)
  {        
      if( (*(p) < '0') || (*(p) > '9') )
          return 0;
      p++;
  }
  return 1;
}


/**
 * @file ymodem.c
 * @brief ymodem升级
 * @details 细节
 * @author 作者
 * @version 版本
 * @date 
 * @license 版权
 */
#ifdef __cplusplus
extern "C" {
#endif
 /* Includes ------------------------------------------------------------------*/
#include "ymodem_includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* 文件名字  */
unsigned char file_name[FILE_NAME_LENGTH] = {0};
unsigned char file_size[FILE_SIZE_LENGTH] = {0};
UCHAR aucPacketData[PACKET_1K_SIZE + PACKET_OVERHEAD] = {0};
YMODEM_PARAM_S gstYmodemParma;
YMODEM_PARAM_S *gpstYmodemParma = &gstYmodemParma;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @brief              接受一字节
 * @param  c: ·        接受的字??
 * @param  timeout:    超时时间
 * @retval 0: ····     收到字节
 *        -1:          超时
 */
int32_t Receive_Byte(uint8_t *c, uint32_t timeout)
{
	while (timeout-- > 0)
	{
		if (Ymodem_GetChar(c) == YMODEM_SUCCESS)
		{
			return 0;
		}
	}

	return -1;
}

/**
  * @brief      发送一个数据
  * @param  c:  一个字节
  * @retval 0:  默认返回0
  */
uint32_t Send_Byte(uint8_t c)
{
	Ymodem_PutChar(c);

	return 0;
}

/**
  * @brief      接受一个字节
  * @param      接收数据
  * @param      数据长度
  * @param      超时时间
  *     0:      结束传输
  *    -1:      ??发送者结??
  *    >0:      包长
  * @retval 0:  正常返回
  *        -1:  超时或者包错???
  *         1:  ??用户????
  */
int32_t Receive_Packet(uint8_t* data, int32_t* length, uint32_t timeout)
{
	uint16_t i, packet_size;
	uint8_t c;
	*length = 0;

	/* 喂狗 */
	Ymodem_Wdgt_Reload();

	/* ????判断有没有数?? */
	if (Receive_Byte(&c, timeout) != 0)
	{
		return -1;
	}

	/* 喂狗 */
	Ymodem_Wdgt_Reload();

	switch (c)
	{
		/* 128字节的数??或者数???? ??前xshell??持是128 */
		case SOH:
		{
			packet_size = PACKET_SIZE;
			gpstYmodemParma->ulSohNum++;
			break;
		}
		/* 1024的数?? */
		case STX:
		{
			packet_size = PACKET_1K_SIZE;
			gpstYmodemParma->ulStxNum++;
			break;
		}
		/* 传输结束 */
		case EOT:
		{
			gpstYmodemParma->ucEotNum++;
			return 0;
		}
		/* 传输终??? */
		case CA:
		{
			if ((Receive_Byte(&c, timeout) == 0) && (c == CA))
			{
				*length = -1;
				return 0;
			}
			else
			{
				return -1;
			}
		}
		case ABORT1:
		case ABORT2:
		{
			return 1;
		}
		default:
		{ 
			return -1; 
		}
	}
	*data = c;
	/* 接收数据大小128 + 5（???部和校验码??   */
	for (i = 1; i < (packet_size + PACKET_OVERHEAD); i++)
	{
		if (Receive_Byte(data + i, timeout) < 0)
		{
			return -1;
		}
	}
	/* 猜测??取反判断??否相?? */
	if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
	{
		return -1;
	}
	*length = packet_size;

	return 0;
}

/**
  * @brief  check response using the ymodem protocol
  * @param  buf: Address of the first byte
  * @retval The size of the file
  */
int32_t Ymodem_CheckResponse(uint8_t c)
{
	return 0;
}

/**
  * @brief  Prepare the first block
  * @param  timeout
  *     0: end of transmission
  * @retval None
  */
void Ymodem_PrepareIntialPacket(uint8_t* data, const uint8_t* fileName, uint32_t* length)
{
	uint16_t i, j;
	uint8_t pucFileInfo[10];

	/* Make first three packet */
	data[0] = SOH;
	data[1] = 0x00;
	data[2] = 0xff;

	/* Filename packet has valid data */
	for (i = 0; (fileName[i] != '\0') && (i < FILE_NAME_LENGTH);i++)
	{
		data[i + PACKET_HEADER] = fileName[i];
	}

	data[i + PACKET_HEADER] = 0x00;

	Int2Str(pucFileInfo, *length);
	for (j = 0, i = i + PACKET_HEADER + 1; pucFileInfo[j] != '\0'; )
	{
		data[i++] = pucFileInfo[j++];
	}

	for (j = i; j < PACKET_SIZE + PACKET_HEADER; j++)
	{
		data[j] = 0;
	}
}

/**
  * @brief  Prepare the data packet
  * @param  timeout
  *     0: end of transmission
  * @retval None
  */
void Ymodem_PreparePacket(uint8_t* SourceBuf, uint8_t* data, uint8_t pktNo, uint32_t sizeBlk)
{
	uint16_t i, size, packetSize;
	uint8_t* pucFileInfo;

	/* Make first three packet */
	packetSize = sizeBlk >= PACKET_1K_SIZE ? PACKET_1K_SIZE : PACKET_SIZE;
	size = sizeBlk < packetSize ? sizeBlk : packetSize;
	if (packetSize == PACKET_1K_SIZE)
	{
		data[0] = STX;
	}
	else
	{
		data[0] = SOH;
	}
	data[1] = pktNo;
	data[2] = (~pktNo);
	pucFileInfo = SourceBuf;

	/* Filename packet has valid data */
	for (i = PACKET_HEADER; i < size + PACKET_HEADER;i++)
	{
		data[i] = *pucFileInfo++;
	}
	if (size <= packetSize)
	{
		for (i = size + PACKET_HEADER; i < packetSize + PACKET_HEADER; i++)
		{
			data[i] = 0x1A; /* EOF (0x1A) or 0x00 */
		}
	}
}

/**
  * @brief  Transmit a data packet using the ymodem protocol
  * @param  data
  * @param  length
  * @retval None
  */
void Ymodem_SendPacket(uint8_t* data, uint16_t length)
{
	uint16_t i;
	i = 0;
	while (i < length)
	{
		Send_Byte(data[i]);
		i++;
	}
}

LONG Ymodem_Data_Process(LONG lPacketLen)
{
	UCHAR *pucData = NULL;
	UCHAR *pucFileInfo;
	ULONG i = 0;

	/* 错???标志清零 */
	gpstYmodemParma->ucErrNum = 0;
	switch (lPacketLen)
	{
		/* ??发送者终?? */
		case -1:
		{
			Send_Byte(ACK);
			return 0;
		}
		/* 结束传输 */
		case 0:
		{
			/* ??改???地?? */
			Send_Byte(NAK);
			gpstYmodemParma->ucFileDone = 1;
			/* modified 结束 */
			if (2 == gpstYmodemParma->ucEotNum)
			{
				gpstYmodemParma->ucSessionDone = 1;
			}
			break;
		}
		default:
		{
			/* 正常?? */
			if ((aucPacketData[PACKET_SEQNO_INDEX] & 0xff) != (gpstYmodemParma->ulPackNum & 0xff))
			{
				Send_Byte(NAK);
			}
			else
			{
				/* 如果是第一帧起始帧 */
				if (gpstYmodemParma->ulPackNum == 0)
				{
					/* 文件名字?? */
					if (aucPacketData[PACKET_HEADER] != 0)
					{
						/* pucFileInfo+3 因为前三??字节??首部 再就??文件名字 */
						for (i = 0, pucFileInfo = aucPacketData + PACKET_HEADER; (*pucFileInfo != 0) && (i < FILE_NAME_LENGTH);)
						{
							file_name[i++] = *pucFileInfo++;
						}
						/* 获取到的文件名字字???串 */
						file_name[i++] = '\0';

						/* 文件名字之后紧接着就是文件大小 */
						for (i = 0, pucFileInfo++; (*pucFileInfo != ' ') && (i < FILE_SIZE_LENGTH);)
						{
							file_size[i++] = *pucFileInfo++;
						}
						file_size[i++] = '\0';

						/* 将文件大小转??为整?? */
						Str2Int(file_size, (LONG *)&(gpstYmodemParma->ulFileSize));

						/* 检测???发送镜像bin文件的大?? */
						/* 如果镜像bin文件比flash?? */
						if (gpstYmodemParma->ulFileSize > (YMODEM_BACK_SIZE + 1))
						{
							/* End session */
							Send_Byte(CA);
							Send_Byte(CA);
							/*debug 文件???? flash???? 没有合适的空间 */
							return -1;
						}

						if (0 == YMODEM_ERASE_MIN)
						{
							/* 擦除flash实际大小 */
							Ymodem_Flash_Erase(YMODEM_BACK_ADDRESS, gpstYmodemParma->ulFileSize);
						}
						else
						{
							Ymodem_Flash_Erase(YMODEM_BACK_ADDRESS, YMODEM_BACK_SIZE);
						}

						Send_Byte(ACK);
						/* 收到??起???帧后立??回应 发送c */
						Send_Byte(CRC16);
					}
					/* 文件名字包为空，结束传输 */
					else
					{
						Send_Byte(ACK);
						gpstYmodemParma->ucFileDone = 1;
						gpstYmodemParma->ucSessionDone = 1;
						break;
					}
				}
				/* 数据?? */
				else
				{
					/* 计算进度 */

					/* 指向数据 */
					pucData = aucPacketData + PACKET_HEADER;

					/* 往flash里边写数?? lPacketLen/4的原因是一??字占4??字节 */
					/* flash地址 ramsource写数?? lPacketLen 写入长度*/
					if (YMODEM_SUCCESS == Ymodem_Flash_Write_App((UCHAR *)pucData, (uint16_t)lPacketLen))
					{
						Send_Byte(ACK);
					}
					else /* 当往flash写的时候，出现错误 */
					{
						/* 结束传输 */
						Send_Byte(CA);
						Send_Byte(CA);
						return -2;
					}
				}
				gpstYmodemParma->ulPackNum++;
				gpstYmodemParma->ucSessionBegin = 1;
			}
		}
	}
	return YMODEM_DATA_SUCCESS;
}

/**
  * @brief  使用ymodem接受一??文件
  * @param  buf: ??一??字节的地址.
  * @retval 文件的大??.
  */
int32_t Ymodem_Receive(uint8_t* buf)
{
	LONG lPacketLen = 0;
	uint8_t ucSendCNum = 0;
	LONG lRet = 0;

	memset(gpstYmodemParma, 0, sizeof(YMODEM_PARAM_S));
	gpstYmodemParma->ulWriteAddr = YMODEM_BACK_ADDRESS;
	gpstYmodemParma->ulHasWrite = 0;

	while(1)
	{
		switch (Receive_Packet(aucPacketData, &lPacketLen, YMODEM_TIMEOUT))
		{
			/* 如果接收成功 */
			case 0:
			{
				YMODEM_PRINTF("收到c\r\n");
				lRet = Ymodem_Data_Process(lPacketLen);
				if (lRet < 0)
				{
					return -2;
				}
				break;
			}
			case 1:
			{
				Send_Byte(CA);
				Send_Byte(CA);
				return -3;
			}
			default:	/* 这里其实??-1 */
			{
				if (gpstYmodemParma->ucSessionBegin > 0)
				{
					gpstYmodemParma->ucErrNum++;
					break;
				}
				if (gpstYmodemParma->ucErrNum > MAX_ERRORS)
				{
					Send_Byte(CA);
					Send_Byte(CA);
					return 0;
				}

				/* 每???延??500ms 发??6?? */
				if (YMODEM_C_NUM == ucSendCNum)
				{
					return ERROR_NO_ACK;
				}
				/* 延时,上边接收已延时，该???不许???再次进行延?? */
				YMODEM_SLEEP_MS(YMODEM_DELAY_MS);
				/* 看门狗喂?? */
				Ymodem_Wdgt_Reload();
				ucSendCNum++;
				Send_Byte(CRC16);
				break;
			}
		}
		if ((gpstYmodemParma->ucFileDone != 0) && (gpstYmodemParma->ucSessionDone != 0))
		{
			break;
		}
	}

	/* 再???接收到EOT之后 */
	Send_Byte(ACK);
	Send_Byte(CRC16);
	/* 最后接收结束帧 该帧其实不??????错 都已经传输完?? */
	Receive_Packet(aucPacketData, &lPacketLen, YMODEM_TIMEOUT);
	Send_Byte(ACK);
	Send_Byte(END);

	return (int32_t)gpstYmodemParma->ulFileSize;
}

/**
  * @brief  使用ymodem传输文件
  * @param  buf: Address of the first byte
	sendFileName	发送文件名??
	sizeFile	发送文件大??
  * @retval The size of the file
  */
uint8_t Ymodem_Transmit(uint8_t* buf, const uint8_t* sendFileName, uint32_t sizeFile)
{

	uint8_t aucPacketData[PACKET_1K_SIZE + PACKET_OVERHEAD];
	uint8_t filename[FILE_NAME_LENGTH];
	uint8_t* buf_ptr, tempCheckSum;
	uint16_t tempCRC;
	uint16_t blkNumber;
	uint8_t receivedC[2], CRC16_F = 0, i;
	uint32_t ulErrors, ackReceived, size = 0, pktSize;

	ulErrors = 0;
	ackReceived = 0;
	/* 拷贝文件名字 */
	for (i = 0; i < (FILE_NAME_LENGTH - 1); i++)
	{
		filename[i] = sendFileName[i];
	}
	CRC16_F = 1;

	/* 准??????一?? */
	Ymodem_PrepareIntialPacket(&aucPacketData[0], filename, &sizeFile);

	do
	{
		/* Send Packet */
		Ymodem_SendPacket(aucPacketData, PACKET_SIZE + PACKET_HEADER);

		/* Send CRC or Check Sum based on CRC16_F */
		if (CRC16_F)
		{
			tempCRC = Cal_CRC16(&aucPacketData[3], PACKET_SIZE);
			Send_Byte(tempCRC >> 8);
			Send_Byte(tempCRC & 0xFF);
		}
		else
		{
			tempCheckSum = CalChecksum(&aucPacketData[3], PACKET_SIZE);
			Send_Byte(tempCheckSum);
		}

		/* Wait for Ack and 'C' */
		if (Receive_Byte(&receivedC[0], 10000) == 0)
		{
			if (receivedC[0] == ACK)
			{
				/* Packet transferred correctly */
				ackReceived = 1;
			}
		}
		else
		{
			ulErrors++;
		}
	} while (!ackReceived && (ulErrors < 0x0A));

	if (ulErrors >= 0x0A)
	{
		return ulErrors;
	}
	buf_ptr = buf;
	size = sizeFile;
	blkNumber = 0x01;
	/* Here 1024 bytes package is used to send the packets */


	/* Resend packet if NAK  for a count of 10 else end of communication */
	while (size)
	{
		/* Prepare next packet */
		Ymodem_PreparePacket(buf_ptr, &aucPacketData[0], blkNumber, size);
		ackReceived = 0;
		receivedC[0] = 0;
		ulErrors = 0;
		do
		{
			/* Send next packet */
			if (size >= PACKET_1K_SIZE)
			{
				pktSize = PACKET_1K_SIZE;

			}
			else
			{
				pktSize = PACKET_SIZE;
			}
			Ymodem_SendPacket(aucPacketData, pktSize + PACKET_HEADER);
			/* Send CRC or Check Sum based on CRC16_F */
			/* Send CRC or Check Sum based on CRC16_F */
			if (CRC16_F)
			{
				tempCRC = Cal_CRC16(&aucPacketData[3], pktSize);
				Send_Byte(tempCRC >> 8);
				Send_Byte(tempCRC & 0xFF);
			}
			else
			{
				tempCheckSum = CalChecksum(&aucPacketData[3], pktSize);
				Send_Byte(tempCheckSum);
			}

			/* Wait for Ack */
			if ((Receive_Byte(&receivedC[0], 100000) == 0) && (receivedC[0] == ACK))
			{
				ackReceived = 1;
				if (size > pktSize)
				{
					buf_ptr += pktSize;
					size -= pktSize;
					if (blkNumber == (YMODEM_BACK_SIZE / 1024))
					{
						return 0xFF; /*  error */
					}
					else
					{
						blkNumber++;
					}
				}
				else
				{
					buf_ptr += pktSize;
					size = 0;
				}
			}
			else
			{
				ulErrors++;
			}
		} while (!ackReceived && (ulErrors < 0x0A));
		/* Resend packet if NAK  for a count of 10 else end of communication */

		if (ulErrors >= 0x0A)
		{
			return ulErrors;
		}

	}
	ackReceived = 0;
	receivedC[0] = 0x00;
	ulErrors = 0;
	do
	{
		Send_Byte(EOT);
		/* Send (EOT); */
		/* Wait for Ack */
		if ((Receive_Byte(&receivedC[0], 10000) == 0) && receivedC[0] == ACK)
		{
			ackReceived = 1;
		}
		else
		{
			ulErrors++;
		}
	} while (!ackReceived && (ulErrors < 0x0A));

	if (ulErrors >= 0x0A)
	{
		return ulErrors;
	}

	/* Last packet preparation */
	ackReceived = 0;
	receivedC[0] = 0x00;
	ulErrors = 0;

	aucPacketData[0] = SOH;
	aucPacketData[1] = 0;
	aucPacketData[2] = 0xFF;

	for (i = PACKET_HEADER; i < (PACKET_SIZE + PACKET_HEADER); i++)
	{
		aucPacketData[i] = 0x00;
	}

	do
	{
		/* Send Packet */
		Ymodem_SendPacket(aucPacketData, PACKET_SIZE + PACKET_HEADER);

		/* Send CRC or Check Sum based on CRC16_F */
		tempCRC = Cal_CRC16(&aucPacketData[3], PACKET_SIZE);
		Send_Byte(tempCRC >> 8);
		Send_Byte(tempCRC & 0xFF);

		/* Wait for Ack and 'C' */
		if (Receive_Byte(&receivedC[0], 10000) == 0)
		{
			if (receivedC[0] == ACK)
			{
				/* Packet transferred correctly */
				ackReceived = 1;
			}
		}
		else
		{
			ulErrors++;
		}
	} while (!ackReceived && (ulErrors < 0x0A));

	/* Resend packet if NAK  for a count of 10  else end of communication */
	if (ulErrors >= 0x0A)
	{
		return ulErrors;
	}

	do
	{
		Send_Byte(EOT);
		/* Send (EOT); */
		/* Wait for Ack */
		if ((Receive_Byte(&receivedC[0], 10000) == 0) && receivedC[0] == ACK)
		{
			ackReceived = 1;
		}
		else
		{
			ulErrors++;
		}
	} while (!ackReceived && (ulErrors < 0x0A));

	if (ulErrors >= 0x0A)
	{
		return ulErrors;
	}
	return 0; /* file transmitted successfully */
}

#ifdef __cplusplus
};
#endif

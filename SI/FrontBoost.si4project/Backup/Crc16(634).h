/**************************************************************************
ģ������: CRC16ģ�����
ģ�鹦��: CRCУ��������
�������: 
���ز���: 
**************************************************************************/
#ifndef __CRC16_H
#define __CRC16_H	

#include "main.h"


uint16_t crc16(uint8_t *puchMsg, uint16_t usDataLen);
uint8_t dataBufCrcCheck(uint8_t *pDataBuf, uint16_t usDataLen);



#endif



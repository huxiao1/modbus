// ModbusProtocol.cpp: implementation of the CModbusProtocol class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "StdAfx.h"
#include "ModbusProtocol.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//�����Ǽ���õ������  
/* CRC ��λ�ֽ�ֵ��  
CRC high byte */   
const UINT8 auchCRCHi[] = {   
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,   
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,   
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,   
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,   
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,   
		0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,   
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,   
		0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,   
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,   
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,   
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,   
		0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,   
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,   
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,   
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,   
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,   
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,   
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,   
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,   
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,   
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,   
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,   
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,   
		0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,   
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,   
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40   };   

/* CRC��λ�ֽ�ֵ�� 
CRC low byte */   
const UINT8 auchCRCLo[] = {   
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,   
		0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,   
		0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,   
		0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,   
		0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,   
		0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,   
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,   
		0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,   
		0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,   
		0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,   
		0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,   
		0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,   
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,   
		0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,   
		0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,   
		0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,   
		0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,   
		0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,   
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,   
		0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,   
		0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,   
		0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,   
		0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,   
		0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,   
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,   
		0x43, 0x83, 0x41, 0x81, 0x80, 0x40   };  

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModbusProtocol::CModbusProtocol()
{
	
}

CModbusProtocol::~CModbusProtocol()
{
	
}

//CRCУ�麯��
UINT16 CModbusProtocol::CRC16(UINT8 *Pushdata, UINT8 length)
{
	UINT8 uchCRCHi=0xFF;  
    UINT8 uchCRCLo=0xFF;  
    UINT8 uIndex;  
	
    while(length--)  
    {  
		uIndex   = uchCRCHi ^ *Pushdata++;  
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];  
		uchCRCLo = auchCRCLo[uIndex];  
    }  

    return (uchCRCHi<<8|uchCRCLo);  
}

//���豸��0x03���Ĵ���״̬
void CModbusProtocol::InquireDataMaster(UINT8 MachineAddr, 
		UINT16 RegStartAddr, UINT16 RegNum, UINT8 *sendBuf, UINT16 *sendNum)
{
	UINT16 CRCResult;

	sendBuf[0] = MachineAddr;
	sendBuf[1] = 0x03;
	sendBuf[2] = HIBYTE_NORMAL(RegStartAddr);
	sendBuf[3] = LOBYTE_NORMAL(RegStartAddr);
	sendBuf[4] = HIBYTE_NORMAL(RegNum);
	sendBuf[5] = LOBYTE_NORMAL(RegNum);

	CRCResult = CRC16(sendBuf, 6);

	sendBuf[6] = LOBYTE_NORMAL(CRCResult);
	sendBuf[7] = HIBYTE_NORMAL(CRCResult);

	*sendNum = 8;
}

//���豸��0x03����������豸��ȡ����Ϣ
void CModbusProtocol::InquireDataSlave(UINT8 MachineAddr, UINT16 RegStartAddr, 
				UINT16 RegNum, UINT8 *sendBuf, UINT16 sendNum, UINT16 *RegBuf)
{
	UINT16 countFor = 0;
	UINT16 CRCResult;

	sendBuf[0] = MachineAddr;
	sendBuf[1] = 0x03;
	sendBuf[2] = RegNum * 2 + 5;
	
	for (countFor = 0; countFor<RegNum; countFor++)
	{
		sendBuf[3 + countFor * 2 ] = HIBYTE_NORMAL(RegBuf[countFor]);
		sendBuf[3 + countFor * 2 + 1 ] = LOBYTE_NORMAL(RegBuf[countFor]);
	}

	CRCResult = CRC16(sendBuf, RegNum * 2 + 3);

	sendBuf[RegNum * 2 + 3 ] = LOBYTE_NORMAL(CRCResult);
	sendBuf[RegNum * 2 + 4 ] = HIBYTE_NORMAL(CRCResult);

	sendNum = RegNum * 2 + 5;
}

//�����ȡ��ַ�д��󣬷���0x83���ϴ���
void CModbusProtocol::InquireDataSlaveError(UINT8 MachineAddr, 
					UINT8 ErrorNum, UINT8 *sendBuf, UINT16 sendNum)
{
	UINT16 CRCResult;

	sendBuf[0] = MachineAddr;
	sendBuf[1] = 0x83;
	sendBuf[2] = ErrorNum;

	CRCResult = CRC16(sendBuf, 3);

	sendBuf[3] = LOBYTE_NORMAL(CRCResult);
	sendBuf[4] = HIBYTE_NORMAL(CRCResult);

	sendNum = 5;
}

//���豸��0x10�����Ӧ���豸д����
void CModbusProtocol::ControlDataSlaver(UINT8 *receiveBuf,  INT16 receiveNum, UINT8 *sendBuf, UINT16 *sendNum)
{
	UINT16 CRCResult;

	sendBuf[0] = receiveBuf[0];
	sendBuf[1] = receiveBuf[1];

	sendBuf[2] = receiveBuf[2];
	sendBuf[3] = receiveBuf[3];
	sendBuf[4] = receiveBuf[4];
	sendBuf[5] = receiveBuf[5];

	CRCResult = CRC16(sendBuf, 6);

	sendBuf[6] = LOBYTE_NORMAL(CRCResult);
	sendBuf[7] = HIBYTE_NORMAL(CRCResult);

	*sendNum = 8;
}

//���豸������д�����Ĵ�������
void CModbusProtocol::ControlDataSlaverError(UINT8 *receiveBuf, INT16 receiveNum, 
											 UINT8 *sendBuf, UINT16 *sendNum)
{
	UINT16 CRCResult;

	sendBuf[0] = receiveBuf[0];
	sendBuf[1] = 0x90;
	sendBuf[2] = 1;
	
	CRCResult = CRC16(sendBuf, 3);

	sendBuf[3] = LOBYTE_NORMAL(CRCResult);
	sendBuf[4] = HIBYTE_NORMAL(CRCResult);

	*sendNum = 5;
}

//���豸����ȡ���豸��Ȧ
void CModbusProtocol::CoilDataMaster(UINT8 MachineAddr, UINT16 RegStartAddr, UINT16 RegNum, UINT8 *sendBuf, UINT16 *sendNum)
{
	UINT16 CRCResult;

	sendBuf[0] = MachineAddr;
	sendBuf[1] = 0x01;
	sendBuf[2] = HIBYTE_NORMAL(RegStartAddr);
	sendBuf[3] = LOBYTE_NORMAL(RegStartAddr);
	sendBuf[4] = HIBYTE_NORMAL(RegNum);
	sendBuf[5] = LOBYTE_NORMAL(RegNum);

	CRCResult = CRC16(sendBuf, 6);

	sendBuf[7] = LOBYTE_NORMAL(CRCResult);
	sendBuf[6] = HIBYTE_NORMAL(CRCResult);

	*sendNum = 8;
}



//���豸�����ض���Ȧ���ݣ�����ֻ��16λ������
UINT16 CModbusProtocol::CoilSetDataSlaver( UINT8 *CoilData, UINT16 CoilStartAddr, UINT16 CoilCmd )
{
	switch(CoilStartAddr)
	{
	case 8:
		if (CoilCmd == 0xFF00)  //�����ON
		{
			CoilData[1] |= 0x01;  //����һλ��1
		}
		else
		{
			CoilData[1] &= 0xFE;   //�����off����һλ��0
		}
		break;

	case 9:                          //ʣ�°�λ��һ������
		if (CoilCmd == 0xFF00)
		{
			CoilData[1] |= 0x02;
		}
		else
		{
			CoilData[1] &= 0xFD;
		}
		break;

	case 10:
		if (CoilCmd == 0xFF00)
		{
			CoilData[1] |= 0x04;
		}
		else
		{
			CoilData[1] &= 0xFB;
		}
		break;

	case 11:
		if (CoilCmd == 0xFF00)
		{
			CoilData[1] |= 0x08;
		}
		else
		{
			CoilData[1] &= 0xF7;
		}
		break;

	case 12:
		if (CoilCmd == 0xFF00)
		{
			CoilData[1] |= 0x10;
		}
		else
		{
			CoilData[1] &= 0xEF;
		}
		break;

	case 13:
		if (CoilCmd == 0xFF00)
		{
			CoilData[1] |= 0x20;
		}
		else
		{
			CoilData[1] &= 0xDF;
		}
		break;

	case 14:
		if (CoilCmd == 0xFF00)
		{
			CoilData[1] |= 0x40;
		}
		else
		{
			CoilData[1] &= 0xBF;
		}
		break;

	case 15:
		if (CoilCmd == 0xFF00)
		{
			CoilData[1] |= 0x80;
		}
		else
		{
			CoilData[1] &= 0x7F;
		}
		break;

	default:
		break;
	}

	return 0;
}

UINT16 CModbusProtocol::CoilDataSlaver(UINT8 MachineAddr, 
	UINT8 *CoilData, UINT16 CoilStartAddr, UINT16 CoilDataNum, UINT8 *sendBuf)
{
	UINT16 CRCResult;
	UINT16 countFor;
	UINT16 CoileSendNum = 0;
	
	//�����������ݰ�
	sendBuf[0] = MachineAddr;  //���ذ���һ�ֽ�Ϊ��ַ
	sendBuf[1] = 0x01;         //���ذ��ڶ��ֽ�Ϊ����
	
	//Coil�ֽ���������������8λ���һ���ֽ�BYTE���������8С��17��Ϊ2���ֽڣ����С�ڰ�Ϊ1�ֽ�
	if ((CoilDataNum%8) == 0)  //����ܱ�8������˵���ֽ������պ�
	{
		CoileSendNum = CoilDataNum * 0.125;   //CoilDataNum/8
	}
	else  //������ܱ�������˵�����������λ���ֽ���Ҫ��һ
	{
		CoileSendNum = CoilDataNum * 0.125 + 1;   //CoilDataNum/8
	}
	
	//������Ȧռ���ֽ��������������
	sendBuf[2] = CoileSendNum;
	
    //������Ȧ���� coilData[]Ϊ������Ȧ״̬�ֽڣ�����ֻ��coilData[0]Ϊ״̬�ֽ�, coileSendNum=1
	for (countFor = 0; countFor<CoileSendNum; countFor++)
	{
		sendBuf[3+countFor] = CoilData[countFor];
	}
	
	//����CRCУ������
	CRCResult = CRC16(sendBuf, CoileSendNum + 3);
	
	sendBuf[3 + CoileSendNum] = HIBYTE_NORMAL(CRCResult);  //���������ֽ�ΪCRC
	sendBuf[4 + CoileSendNum] = LOBYTE_NORMAL(CRCResult);
	
	return 5 + CoileSendNum;   //�������ط������ݰ���С
}


UINT16 CModbusProtocol::ModbusSlaveEcho(UINT8 *receiveData, 
										INT16 receiveNum, 
										UINT8 Addr, 
									  UINT8 *CoilDatainput, 
									  UINT16 *RegDataInput, 
									  UINT8 *sendDataTemp)
{
	//CRCУ�����
	UINT16 CRCResult;
	UINT16 CRCCalc;
	//��д��ַ
	UINT16 ReadStartAddr = 0;
	UINT16 ReadNumber = 0;
	UINT16 ReceiveCMD = 0;
	//for
	UINT16 countFor = 0;

	 //��F9�����ڳ���Ӷϵ㣬�������԰�F10
	if (receiveData[0] == Addr)   //����Ǳ�����ַ�������Ӧ��Ӧ�����Ǳ�����ַ���ù��ˣ������㣬������485���߷�����
	{
		//CRC У��
		CRCResult = CRC16(receiveData, receiveNum-2);  //����������CRCУ��
		CRCCalc = MAKEINT16(receiveData[receiveNum-2], 
			receiveData[receiveNum-1]);  //�����յ�������8λ�����16λ��
		
		//�ж�CRC����Ƿ���ȷ
		//����Ӧ��������лظ�
		if (CRCResult == CRCCalc)   //�жϽ��յ�CRC�����Ƿ����������ͬ������ͬ�򷵻��㣬˵������Ҫ�ظ�
		{
			switch (receiveData[1])   //�ڶ����ֽ�Ϊ����
			{
			case 0x01:  //��Ӧ����Ȧ״̬ 0x01����
				
				//����Ȧ��ʼ��ַ
				ReadStartAddr = MAKEINT16(receiveData[2], receiveData[3]);  //�ڶ��ֽں͵����ֽ�Ϊ��ַ
				//����Ȧ���ݸ���
				ReadNumber = MAKEINT16(receiveData[4], receiveData[5]);  //�����ֽں͵����ֽ�Ϊ��Ҫ���ĸ���
				//�����������
				return CoilDataSlaver(Addr, CoilDatainput, ReadStartAddr, ReadNumber, sendDataTemp);  //��ʼ������鷵�����ݰ�
				break;
				
			case 0x05:  //ǿ����Ȧ״̬   0x05����
				//����Ȧ��ʼ��ַ
				ReadStartAddr = MAKEINT16(receiveData[2], receiveData[3]);
				//����Ȧ����
				ReceiveCMD = MAKEINT16(receiveData[4], receiveData[5]);
				//��������
				CoilSetDataSlaver(CoilDatainput, ReadStartAddr, ReceiveCMD);
				//������Ϣ��ֱ�ӽ�������Ϣ����
				for (countFor=0; countFor<receiveNum; countFor++)
				{
					sendDataTemp[countFor] = receiveData[countFor];
				}
				//�������ݸ���Ϊ�������ݸ���
				return receiveNum;
				break;
				
			default:
				break;
			}

			return 0;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

UINT16 CModbusProtocol::ControDataMaster(UINT8 MachineAddr, UINT16 RegStartAddr, 
										 UINT16 RegNum, UINT8 *sendBuf, UINT8 *ControlReg)
{
	UINT16 CRCResult;
	UINT16 countFor = 0;
	
	sendBuf[0] = MachineAddr;
	sendBuf[1] = 0x10;
	sendBuf[2] = HIBYTE_NORMAL(RegStartAddr);  //��ʼ��ַ
	sendBuf[3] = LOBYTE_NORMAL(RegStartAddr);
	sendBuf[4] = HIBYTE_NORMAL(RegNum);   //����16λ������
	sendBuf[5] = LOBYTE_NORMAL(RegNum);
	sendBuf[6] = RegNum * 2;              //�����ֽ���
	
	for (countFor=0; countFor< RegNum; countFor++)   //װ�ؼĴ�������
	{
		sendBuf[7 + countFor*2] = ControlReg[countFor*2+1];
		sendBuf[7 + countFor*2+1] = ControlReg[countFor*2];
	}
	
	CRCResult = CRC16(sendBuf, RegNum * 2 + 7);   //CRCУ��
	
	sendBuf[RegNum*2 + 7] = HIBYTE_NORMAL(CRCResult);
	sendBuf[RegNum*2 + 8] = LOBYTE_NORMAL(CRCResult);
	
	return RegNum*2+9;    //���ش�����Ҫ�������ݸ���
}

UINT16 CModbusProtocol::WriteSigleCoil(UINT8 MachineAddr, UINT16 RegStartAddr, UINT16 Command, UINT8 *sendBuf)
{
	UINT16 CRCResult;
	
	sendBuf[0] = MachineAddr;
	sendBuf[1] = 0x05;
	sendBuf[2] = HIBYTE_NORMAL(RegStartAddr);
	sendBuf[3] = LOBYTE_NORMAL(RegStartAddr);
	sendBuf[4] = HIBYTE_NORMAL(Command);
	sendBuf[5] = LOBYTE_NORMAL(Command);
	
	CRCResult = CRC16(sendBuf, 6);
	
	sendBuf[7] = LOBYTE_NORMAL(CRCResult);
	sendBuf[6] = HIBYTE_NORMAL(CRCResult);

	return 8;
}

void CModbusProtocol::CoilDataMasterInput(UINT8 MachineAddr, UINT16 RegStartAddr, UINT16 RegNum, UINT8 *sendBuf, UINT16 *sendNum)
{
	UINT16 CRCResult;
	
	sendBuf[0] = MachineAddr;
	sendBuf[1] = 0x02;
	sendBuf[2] = HIBYTE_NORMAL(RegStartAddr);
	sendBuf[3] = LOBYTE_NORMAL(RegStartAddr);
	sendBuf[4] = HIBYTE_NORMAL(RegNum);
	sendBuf[5] = LOBYTE_NORMAL(RegNum);
	
	CRCResult = CRC16(sendBuf, 6);
	
	sendBuf[7] = LOBYTE_NORMAL(CRCResult);
	sendBuf[6] = HIBYTE_NORMAL(CRCResult);
	
	*sendNum = 8;
}

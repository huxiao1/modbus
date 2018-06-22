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


//以下是计算好的数组表：  
/* CRC 高位字节值表  
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

/* CRC低位字节值表 
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

//CRC校验函数
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

//主设备：0x03读寄存器状态
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

//从设备：0x03命令，返回主设备读取的信息
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

//如果读取地址有错误，返回0x83故障代码
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

//从设备，0x10命令，响应主设备写操作
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

//从设备：返回写入多个寄存器错误
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

//主设备：读取从设备线圈
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



//从设备：返回读线圈内容，假设只有16位数字量
UINT16 CModbusProtocol::CoilSetDataSlaver( UINT8 *CoilData, UINT16 CoilStartAddr, UINT16 CoilCmd )
{
	switch(CoilStartAddr)
	{
	case 8:
		if (CoilCmd == 0xFF00)  //如果是ON
		{
			CoilData[1] |= 0x01;  //将第一位置1
		}
		else
		{
			CoilData[1] &= 0xFE;   //如果是off将第一位置0
		}
		break;

	case 9:                          //剩下八位是一个道理
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
	
	//构建返回数据包
	sendBuf[0] = MachineAddr;  //返回包第一字节为地址
	sendBuf[1] = 0x01;         //返回包第二字节为命令
	
	//Coil字节数量，返回数据8位组成一个字节BYTE，如果大于8小于17则为2个字节，如果小于八为1字节
	if ((CoilDataNum%8) == 0)  //如果能被8整除，说明字节数量刚好
	{
		CoileSendNum = CoilDataNum * 0.125;   //CoilDataNum/8
	}
	else  //如果不能被整除，说明还多出来几位，字节数要加一
	{
		CoileSendNum = CoilDataNum * 0.125 + 1;   //CoilDataNum/8
	}
	
	//发送线圈占用字节数，上面计算结果
	sendBuf[2] = CoileSendNum;
	
    //发送线圈数据 coilData[]为发送线圈状态字节，我们只有coilData[0]为状态字节, coileSendNum=1
	for (countFor = 0; countFor<CoileSendNum; countFor++)
	{
		sendBuf[3+countFor] = CoilData[countFor];
	}
	
	//发送CRC校验数据
	CRCResult = CRC16(sendBuf, CoileSendNum + 3);
	
	sendBuf[3 + CoileSendNum] = HIBYTE_NORMAL(CRCResult);  //后面两个字节为CRC
	sendBuf[4 + CoileSendNum] = LOBYTE_NORMAL(CRCResult);
	
	return 5 + CoileSendNum;   //函数返回发送数据包大小
}


UINT16 CModbusProtocol::ModbusSlaveEcho(UINT8 *receiveData, 
										INT16 receiveNum, 
										UINT8 Addr, 
									  UINT8 *CoilDatainput, 
									  UINT16 *RegDataInput, 
									  UINT8 *sendDataTemp)
{
	//CRC校验变量
	UINT16 CRCResult;
	UINT16 CRCCalc;
	//读写地址
	UINT16 ReadStartAddr = 0;
	UINT16 ReadNumber = 0;
	UINT16 ReceiveCMD = 0;
	//for
	UINT16 countFor = 0;

	 //按F9可以在程序加断点，单步调试按F10
	if (receiveData[0] == Addr)   //如果是本机地址数据则回应响应，不是本机地址则不用管了，返回零，不会往485总线发数据
	{
		//CRC 校验
		CRCResult = CRC16(receiveData, receiveNum-2);  //对输入数做CRC校验
		CRCCalc = MAKEINT16(receiveData[receiveNum-2], 
			receiveData[receiveNum-1]);  //将接收到的两个8位数组成16位数
		
		//判断CRC结果是否正确
		//对相应的命令进行回复
		if (CRCResult == CRCCalc)   //判断接收到CRC数据是否与计算结果相同，不相同则返回零，说明不需要回复
		{
			switch (receiveData[1])   //第二个字节为命令
			{
			case 0x01:  //回应读线圈状态 0x01命令
				
				//读线圈起始地址
				ReadStartAddr = MAKEINT16(receiveData[2], receiveData[3]);  //第二字节和第三字节为地址
				//读线圈数据个数
				ReadNumber = MAKEINT16(receiveData[4], receiveData[5]);  //第四字节和第五字节为需要读的个数
				//返回数据组包
				return CoilDataSlaver(Addr, CoilDatainput, ReadStartAddr, ReadNumber, sendDataTemp);  //开始解包和组返回数据包
				break;
				
			case 0x05:  //强制线圈状态   0x05命令
				//读线圈起始地址
				ReadStartAddr = MAKEINT16(receiveData[2], receiveData[3]);
				//读线圈命令
				ReceiveCMD = MAKEINT16(receiveData[4], receiveData[5]);
				//接收数据
				CoilSetDataSlaver(CoilDatainput, ReadStartAddr, ReceiveCMD);
				//返回信息，直接将接收信息返回
				for (countFor=0; countFor<receiveNum; countFor++)
				{
					sendDataTemp[countFor] = receiveData[countFor];
				}
				//发送数据个数为接收数据个数
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
	sendBuf[2] = HIBYTE_NORMAL(RegStartAddr);  //起始地址
	sendBuf[3] = LOBYTE_NORMAL(RegStartAddr);
	sendBuf[4] = HIBYTE_NORMAL(RegNum);   //发送16位数个数
	sendBuf[5] = LOBYTE_NORMAL(RegNum);
	sendBuf[6] = RegNum * 2;              //发送字节数
	
	for (countFor=0; countFor< RegNum; countFor++)   //装载寄存器数据
	{
		sendBuf[7 + countFor*2] = ControlReg[countFor*2+1];
		sendBuf[7 + countFor*2+1] = ControlReg[countFor*2];
	}
	
	CRCResult = CRC16(sendBuf, RegNum * 2 + 7);   //CRC校验
	
	sendBuf[RegNum*2 + 7] = HIBYTE_NORMAL(CRCResult);
	sendBuf[RegNum*2 + 8] = LOBYTE_NORMAL(CRCResult);
	
	return RegNum*2+9;    //返回串口需要发送数据个数
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

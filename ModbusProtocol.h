// ModbusProtocol.h: interface for the CModbusProtocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODBUSPROTOCOL_H__2DF6D936_1CED_409C_BCC0_AB67B1EE92F2__INCLUDED_)
#define AFX_MODBUSPROTOCOL_H__2DF6D936_1CED_409C_BCC0_AB67B1EE92F2__INCLUDED_

#include "StdAfx.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LOBYTE_NORMAL(w) ((char)((UINT16)(w) & 0xFFFF))
#define HIBYTE_NORMAL(w) ((char)(((UINT16)(w) >> 8) & 0xFF)) 
#define MAKEINT16(a,b) ((UINT16)((UINT8)(b) >>0 ) | ((UINT16)(UINT8)(a)) << 8)


class CModbusProtocol  
{
public:
	void CoilDataMasterInput(UINT8 MachineAddr, UINT16 RegStartAddr, UINT16 RegNum, UINT8 *sendBuf, UINT16 *sendNum);
	UINT16 WriteSigleCoil(UINT8 MachineAddr, UINT16 RegStartAddr, UINT16 Command, UINT8 *sendBuf);
	UINT16 ControDataMaster(UINT8 MachineAddr, UINT16 RegStartAddr, UINT16 RegNum, UINT8 *sendBuf, UINT8 *ControlReg);
	UINT16 CoilSetDataSlaver( UINT8 *CoilData, UINT16 CoilStartAddr, UINT16 CoilCmd );
	UINT16 ModbusSlaveEcho(UINT8 *receiveData, INT16 receiveNum, UINT8 Addr, UINT8 *CoilDatainput, UINT16 *RegDataInput, UINT8 *sendDataTemp);
	UINT16 CoilDataSlaver(UINT8 MachineAddr, UINT8 *CoilData, UINT16 CoilStartAddr, UINT16 CoilDataNum, UINT8 *sendBuf);
	void CoilDataMaster(UINT8 MachineAddr, UINT16 RegStartAddr,  UINT16 RegNum, UINT8 *sendBuf, UINT16 *sendNum);
	void ControlDataSlaverError(UINT8 *receiveBuf, INT16 receiveNum, UINT8 *sendBuf, UINT16 *sendNum);
	void ControlDataSlaver(UINT8 *receiveBuf,  INT16 receiveNum, UINT8 *sendBuf, UINT16 *sendNum);
	void InquireDataSlaveError(UINT8 MachineAddr, UINT8 ErrorNum, UINT8 *sendBuf, UINT16 sendNum);
	void InquireDataSlave(UINT8 MachineAddr, UINT16 RegStartAddr, UINT16 RegNum, UINT8 *sendBuf, UINT16 sendNum, UINT16 *RegBuf);
	void InquireDataMaster(UINT8 MachineAddr, UINT16 RegStartAddr, UINT16 RegNum,  UINT8 *sendBuf, UINT16 *sendNum);
	UINT16 CRC16(UINT8 *Pushdata, UINT8 length);
	CModbusProtocol();

	virtual ~CModbusProtocol();
};

#endif // !defined(AFX_MODBUSPROTOCOL_H__2DF6D936_1CED_409C_BCC0_AB67B1EE92F2__INCLUDED_)

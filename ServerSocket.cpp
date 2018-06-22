// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "CRC2018.h"
#include "ServerSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions

void CServerSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	unsigned char receBuf[1024];
	int receNum = 1024;

	int receLen = CSocket::Receive(receBuf, receNum);

	m_pParentWnd->PostMessage(WM_POST_TCP_MESSAGE, (WPARAM)receBuf, (LPARAM)receLen);

	CSocket::OnReceive(nErrorCode);
}

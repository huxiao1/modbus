#if !defined(AFX_LISTENSOCKET_H__15C0DC49_FD59_4726_993E_A6CBC4A9E23A__INCLUDED_)
#define AFX_LISTENSOCKET_H__15C0DC49_FD59_4726_993E_A6CBC4A9E23A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenSocket.h : header file
//

#include "ServerSocket.h"

/////////////////////////////////////////////////////////////////////////////
// CListenSocket command target

class CListenSocket : public CSocket
{
// Attributes
public:
	CWnd *m_pParentWnd;

	CServerSocket *m_ServerSocket;

// Operations
public:
	CListenSocket();
	virtual ~CListenSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENSOCKET_H__15C0DC49_FD59_4726_993E_A6CBC4A9E23A__INCLUDED_)

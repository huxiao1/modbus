#if !defined(AFX_SERVERSOCKET_H__BF1887D4_CD7C_4791_AF19_411A6301D1BC__INCLUDED_)
#define AFX_SERVERSOCKET_H__BF1887D4_CD7C_4791_AF19_411A6301D1BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSocket.h : header file
//
#define WM_POST_TCP_MESSAGE WM_USER+100

/////////////////////////////////////////////////////////////////////////////
// CServerSocket command target

class CServerSocket : public CSocket
{
// Attributes
public:
	CWnd *m_pParentWnd;


// Operations
public:
	CServerSocket();
	virtual ~CServerSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKET_H__BF1887D4_CD7C_4791_AF19_411A6301D1BC__INCLUDED_)

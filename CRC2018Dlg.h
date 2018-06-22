// CRC2018Dlg.h : header file
//
//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_CRC2018DLG_H__4EA6F8CA_5BE8_4897_9E49_3614D0D59BCF__INCLUDED_)
#define AFX_CRC2018DLG_H__4EA6F8CA_5BE8_4897_9E49_3614D0D59BCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCRC2018Dlg dialog

class CCRC2018Dlg : public CDialog
{
// Construction
public:
	void tcpReceDataProcess(UINT8 *receData, int receNum);
	void displayMessage(UINT8 *data, UINT16 num);
	void sendCommData(UINT8 *sendData, UINT16 num);
	BOOL CommOpenFlag;
	void initComm(void);
	CCRC2018Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCRC2018Dlg)
	enum { IDD = IDD_CRC2018_DIALOG };
	CMSComm	m_Comm;
	CString	m_display;
	int		m_distance;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCRC2018Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCRC2018Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButCalcCrc();
	afx_msg void OnButtOpenComm();
	afx_msg void OnButtSendData();
	afx_msg void OnOnCommMscomm1();
	afx_msg void OnButt5On();
	afx_msg void OnButt5Off();
	afx_msg void OnButtFanOn();
	afx_msg void OnButtFanOff();
	afx_msg void OnButFast();
	afx_msg void OnButSlow();
	afx_msg void OnButDistance();
	afx_msg void OnButStart();
	afx_msg void OnButStop();
	afx_msg void OnButCreateServer();
	afx_msg void OnButSerSend();
	afx_msg void OnButCliCon();
	afx_msg void OnButCliSend();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRC2018DLG_H__4EA6F8CA_5BE8_4897_9E49_3614D0D59BCF__INCLUDED_)

// CRC2018.h : main header file for the CRC2018 application
//

#if !defined(AFX_CRC2018_H__AC4141B2_934E_47A8_A81E_1608268E1C76__INCLUDED_)
#define AFX_CRC2018_H__AC4141B2_934E_47A8_A81E_1608268E1C76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCRC2018App:
// See CRC2018.cpp for the implementation of this class
//

class CCRC2018App : public CWinApp
{
public:
	CCRC2018App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCRC2018App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCRC2018App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRC2018_H__AC4141B2_934E_47A8_A81E_1608268E1C76__INCLUDED_)

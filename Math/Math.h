// Math.h : main header file for the Math application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMathApp:
// See Math.cpp for the implementation of this class
//

class CMathApp : public CWinApp
{
public:
	CMathApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMathApp theApp;
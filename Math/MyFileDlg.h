#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyToolBar.h"


// CMyFileDlg

class CMyFileDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CMyFileDlg)

public:
	CMyFileDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	virtual ~CMyFileDlg();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	CButton m_MySaveButton;
public:
	CMyToolBar m_ToolBar;
public:
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
public:
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
protected:
	CButton m_HelpButton;
};



#pragma once
#include "afxwin.h"
#include "speccombo.h"


// CMyFontDlg

class CMyFontDlg : public CFontDialog
{
	DECLARE_DYNAMIC(CMyFontDlg)

protected:
//	CComboBox m_cbFontul;

public:
	CMyFontDlg(LPLOGFONT lplfInitial = NULL,
			DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS,
			CDC* pdcPrinter = NULL,
			CWnd* pParentWnd = NULL);
#ifndef _AFX_NO_RICHEDIT_SUPPORT
	CMyFontDlg(const CHARFORMAT& charformat,
			DWORD dwFlags = CF_SCREENFONTS,
			CDC* pdcPrinter = NULL,
			CWnd* pParentWnd = NULL);
#endif
	virtual ~CMyFontDlg();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	CButton m_bHelp;
public:
	afx_msg void OnPaint();
protected:
	CEdit m_eSample;
	CFont m_SelectedFont;
public:
//	CComboBox m_cbCB4;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	INT_PTR DoModal(void);
	CSpecCombo m_Combo1;
	CSpecCombo m_Combo2;
	CSpecCombo m_Combo3;
	CSpecCombo m_Combo5;//ca 4 nu ne trebe
	RECT TextRect;
public:
	CFont* GetSelectedFont(void);
public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};



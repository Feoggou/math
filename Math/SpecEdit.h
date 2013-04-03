#pragma once


// CSpecEdit

class CSpecEdit : public CEdit
{
	DECLARE_DYNAMIC(CSpecEdit)

public:
	CSpecEdit();
	virtual ~CSpecEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
public:
	int m_ComboID;
public:
	CWnd* m_pDlg;
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};



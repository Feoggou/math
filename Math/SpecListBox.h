#pragma once


// CSpecListBox

class CSpecListBox : public CListBox
{
	DECLARE_DYNAMIC(CSpecListBox)

public:
	CSpecListBox();
	virtual ~CSpecListBox();

protected:
	DECLARE_MESSAGE_MAP()
	
//public:
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
protected:
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu); 
};



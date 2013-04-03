#pragma once
#include "specedit.h"
#include "afxwin.h"
#include "speclistbox.h"


// CSpecCombo

class CSpecCombo : public CComboBox
{
	DECLARE_DYNAMIC(CSpecCombo)

public:
	CSpecCombo();
	virtual ~CSpecCombo();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CSpecEdit m_Edit;
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg void OnCbnSelchange();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
private:
	CSpecListBox m_List;
};



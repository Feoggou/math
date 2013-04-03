#pragma once


// CCalcEdit

class CCalcEdit : public CEdit
{
	DECLARE_DYNAMIC(CCalcEdit)

public:
	CCalcEdit();
	virtual ~CCalcEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};



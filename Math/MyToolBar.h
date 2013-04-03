#pragma once


// CMyToolBar

class CMyToolBar : public CToolBarCtrl
{
	DECLARE_DYNAMIC(CMyToolBar)

public:
	CMyToolBar();
	virtual ~CMyToolBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcPaint();
};



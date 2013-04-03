#pragma once

#include "CalcEdit.h"

// CCalcDialogBar dialog

class CCalcDialogBar : public CDialogBar
{
	DECLARE_DYNAMIC(CCalcDialogBar)

public:
	CCalcDialogBar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCalcDialogBar();

// Dialog Data
	enum { IDD = IDD_DIALOGBAR_CALCUL };

protected:


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateButtonCalcul(CCmdUI* pCmdUI);
public:
	CCalcEdit* GetCalcEdit(void);
public:
	afx_msg void OnButtonCalcul();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	UINT DimEVechi;
	UINT DimFNou;
	UINT DimFVechi;
		CButton		m_ButtonCalcul;
	CCalcEdit	m_EditCalcul;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
};

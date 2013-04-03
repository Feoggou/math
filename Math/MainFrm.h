// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "afxext.h"
#include "CalcDialogBar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
public:
	CCalcDialogBar m_CalcDialogBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	DECLARE_MESSAGE_MAP()
public:
	CCalcEdit* GetCalcEdit(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	afx_msg void OnEeditCopy();
public:
	afx_msg void OnUpdateEeditCopy(CCmdUI *pCmdUI);
public:
	afx_msg void OnEeditSelectAll();
public:
	afx_msg void OnUpdateEeditSelectAll(CCmdUI *pCmdUI);
public:
	afx_msg void OnEeditCut();
public:
	afx_msg void OnUpdateEeditCut(CCmdUI *pCmdUI);
public:
//	afx_msg void OnEeditDelete();
public:
	afx_msg void OnUpdateEeditDelete(CCmdUI *pCmdUI);
public:
	afx_msg void OnEeditPaste();
public:
	afx_msg void OnUpdateEeditPaste(CCmdUI *pCmdUI);
public:
	afx_msg void OnEeditUndo();
public:
	afx_msg void OnUpdateEeditUndo(CCmdUI *pCmdUI);
public:
	afx_msg void OnPaint();
public:
//	afx_msg void OnEeditDeleteAll();
public:
//	afx_msg void OnEeditDelete();
};



// MathView.h : interface of the CMathView class
//


#pragma once

//#include "CntrItem.h"
class CMathCntrItem;

class CMathView : public CRichEditView
{
protected: // create from serialization only
	CMathView();
	DECLARE_DYNCREATE(CMathView)

// Attributes
public:
	CMathDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
//	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
//	BOOL IsEditBoxWithFocus(CWnd* pWnd);
	virtual ~CMathView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnViewOptions();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	BOOL IsEditBoxWithFocus(CWnd* pWnd);
	afx_msg void OnEditClearAll();
	afx_msg void OnUpdateEditClearAll(CCmdUI *pCmdUI);
	CFont myFont;
	afx_msg void OnFormatFont();
	virtual void OnInitialUpdate();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSave();
};

#ifndef _DEBUG  // debug version in MathView.cpp
inline CMathDoc* CMathView::GetDocument() const
   { return reinterpret_cast<CMathDoc*>(m_pDocument); }
#endif


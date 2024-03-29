// MathDoc.h : interface of the CMathDoc class
//


#pragma once

#include "afxrich.h"

class CMathDoc : public CRichEditDoc
{
protected: // create from serialization only
	CMathDoc();
	DECLARE_DYNCREATE(CMathDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

// Implementation
public:
	virtual ~CMathDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};



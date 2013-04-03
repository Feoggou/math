// CntrItem.h : interface of the CMathCntrItem class
//

#pragma once

#include "afxrich.h"

class CMathDoc;
class CMathView;

class CMathCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CMathCntrItem)

// Constructors
public:
	CMathCntrItem(REOBJECT* preo = NULL, CMathDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer

// Attributes
public:
	CMathDoc* GetDocument()
		{ return reinterpret_cast<CMathDoc*>(CRichEditCntrItem::GetDocument()); }
	CMathView* GetActiveView()
		{ return reinterpret_cast<CMathView*>(CRichEditCntrItem::GetActiveView()); }

	public:
	protected:

// Implementation
public:
	~CMathCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};


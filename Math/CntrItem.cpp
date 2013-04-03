// CntrItem.cpp : implementation of the CMathCntrItem class
//

#include "stdafx.h"
#include "Math.h"

#include "MathDoc.h"
#include "MathView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMathCntrItem implementation

IMPLEMENT_SERIAL(CMathCntrItem, CRichEditCntrItem, 0)

CMathCntrItem::CMathCntrItem(REOBJECT* preo, CMathDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
}

CMathCntrItem::~CMathCntrItem()
{
	// TODO: add cleanup code here
}


// CMathCntrItem diagnostics

#ifdef _DEBUG
void CMathCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CMathCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif


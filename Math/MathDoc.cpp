// MathDoc.cpp : implementation of the CMathDoc class
//

#include "stdafx.h"
#include "Math.h"

#include "MathDoc.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMathDoc

IMPLEMENT_DYNCREATE(CMathDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CMathDoc, CRichEditDoc)
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()


// CMathDoc construction/destruction

CMathDoc::CMathDoc()
{
	// TODO: add one-time construction code here

}

CMathDoc::~CMathDoc()
{
}

BOOL CMathDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;
	this->SetTitle(L"Neintitulat");

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

CRichEditCntrItem* CMathDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CMathCntrItem(preo, const_cast<CMathDoc*>(this));
}




// CMathDoc serialization

void CMathDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		CString s;
		this->m_bRTF = FALSE;
		char* t; int length = 0;
		this->GetView()->GetWindowTextW(s);
		length = s.GetLength();
		t = new (char[length]);
		memset(t, 0, length);
		int i;
		for (i = 0; i < s.GetLength(); i++)
		{
			t[i] = (s[i]);
/*			if (t[i]=='{') 
			{
				t[i] = 0;
				break;
			}*/
		}
		length = i+1;
		ar.Write(t, 0);
//		ar.WriteString(s);
//		ar<<s;
		delete[] t;
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// TODO: set CRichEditDoc::m_bRTF = FALSE if you are serializing as text
	CRichEditDoc::Serialize(ar);
}


// CMathDoc diagnostics

#ifdef _DEBUG
void CMathDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CMathDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG


// CMathDoc commands

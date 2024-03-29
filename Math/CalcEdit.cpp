// CalcEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Math.h"
#include "CalcEdit.h"
#include "MainFrm.h"


// CCalcEdit

IMPLEMENT_DYNAMIC(CCalcEdit, CEdit)

CCalcEdit::CCalcEdit()
{
}

CCalcEdit::~CCalcEdit()
{
}


BEGIN_MESSAGE_MAP(CCalcEdit, CEdit)
	ON_WM_CONTEXTMENU()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CCalcEdit message handlers



void CCalcEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu ContextMenu;

	if (ContextMenu.CreatePopupMenu())
	{
		CString str;
		GetWindowText(str);
		UINT grayed;

		//edit_undo
		grayed = CanUndo()==FALSE ? MF_GRAYED : 0 ;
		ContextMenu.InsertMenu(0, MF_STRING | MF_BYPOSITION, ID_EEDIT_UNDO, _T("&Anulează"));
		if (str.GetLength())//////////este scris text
		{
			int Start, End;

			GetSel(Start, End);
			str = str.Mid(Start, End-Start+1);

			ContextMenu.InsertMenu(1, MF_SEPARATOR);
			
			//edit_cut
			grayed = str.GetLength()==0 ? MF_GRAYED : 0;
			ContextMenu.InsertMenu(2, MF_STRING | MF_BYPOSITION, ID_EEDIT_CUT, _T("&Decupează"));

			//edit_copy
			grayed = str.GetLength()==0 ? MF_GRAYED : 0;
			ContextMenu.InsertMenu(3, MF_STRING | MF_BYPOSITION, ID_EDIT_COPY, _T("&Copiere"));

			ContextMenu.AppendMenu(MF_SEPARATOR);
			ContextMenu.AppendMenu(MF_STRING, ID_EEDIT_SELECT_ALL, _T("&Selecteaza tot"));
		}

		else 
		{	
			ContextMenu.InsertMenu(0, MF_STRING | MF_GRAYED, ID_EDIT_COPY, _T("&Copiere"));
			ContextMenu.AppendMenu(MF_SEPARATOR);
			ContextMenu.AppendMenu(MF_STRING | MF_GRAYED, ID_EDIT_SELECT_ALL, _T("&Selecteaza tot"));
		}

		ContextMenu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}

	else MessageBox(_T("Crearea meniului contextual esuata!"), _T("Eroare!"), MB_ICONSTOP);
}

BOOL CCalcEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	HWND hView = this->GetParentFrame()->GetActiveView()->m_hWnd;
	WPARAM wParam = MAKEWPARAM(nFlags, zDelta);
	LPARAM lParam = MAKELPARAM(pt.x, pt.y);

	return (BOOL)::SendMessageW(hView, WM_MOUSEWHEEL, wParam, lParam);
}

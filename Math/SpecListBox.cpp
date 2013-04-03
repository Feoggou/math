// SpecListBox.cpp : implementation file
//

#include "stdafx.h"
#include "Math.h"
#include "SpecListBox.h"


// CSpecListBox

IMPLEMENT_DYNAMIC(CSpecListBox, CListBox)

CSpecListBox::CSpecListBox()
{

}

CSpecListBox::~CSpecListBox()
{
}


BEGIN_MESSAGE_MAP(CSpecListBox, CListBox)
//	ON_WM_CONTEXTMENU()
	ON_WM_INITMENUPOPUP()
END_MESSAGE_MAP()



// CSpecListBox message handlers


void CSpecListBox::OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu)
{
	MessageBox(L"da!");
	pMenu->AppendMenuW(MF_BYPOSITION, ID_FILE_NEW, L"Caca");
}


/*void CSpecListBox::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CListBox::OnContextMenu(pWnd, point);
}*/

void CSpecListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	CString Text;

	if (lpDrawItemStruct->itemID==-1) return;

	this->GetText(lpDrawItemStruct->itemID,Text);
//	MessageBox(Text);

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	COLORREF TextColor = dc.GetTextColor();
	COLORREF BackColor = dc.GetBkColor();

	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
       (lpDrawItemStruct->itemState  & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	else
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, BackColor);

	dc.DrawText(Text, Text.GetLength(), &lpDrawItemStruct->rcItem, DT_LEFT|DT_SINGLELINE|DT_VCENTER);

	dc.SetTextColor(TextColor);
	dc.SetBkColor(BackColor);
	dc.Detach();

}
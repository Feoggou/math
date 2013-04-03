// SpecCombo.cpp : implementation file
//

#include "stdafx.h"
#include "Math.h"
#include "MyFontDlg.h"
#include <dlgs.h>

extern WNDPROC OldCBNProc;


// CSpecCombo

IMPLEMENT_DYNAMIC(CSpecCombo, CComboBox)

CSpecCombo::CSpecCombo()
{
}

CSpecCombo::~CSpecCombo()
{
}


BEGIN_MESSAGE_MAP(CSpecCombo, CComboBox)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_DRAWITEM()
	ON_WM_CONTEXTMENU()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &CSpecCombo::OnCbnSelchange)
END_MESSAGE_MAP()



// CSpecCombo message handlers



HBRUSH CSpecCombo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

//	WM_CTLCOLOR
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (nCtlColor==CTLCOLOR_EDIT)
	{
		if (m_Edit.GetSafeHwnd()==NULL)
		{
			m_Edit.SubclassWindow(pWnd->GetSafeHwnd());
			m_Edit.m_ComboID = GetDlgCtrlID();
			m_Edit.m_pDlg = GetParent();
//			int i = m_Edit.GetDlgCtrlID();
		}
	}

	if (nCtlColor==CTLCOLOR_LISTBOX)
	{
		if (m_List.GetSafeHwnd()==NULL)
		{
			m_List.SubclassWindow(pWnd->GetSafeHwnd());
//			int i = m_Edit.GetDlgCtrlID();
		}
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;

}

void CSpecCombo::OnDestroy()
{
	if (m_Edit.GetSafeHwnd()!=NULL)
		m_Edit.UnsubclassWindow();
	if (::IsWindow(this->m_hWnd))
		UnsubclassWindow();

	CComboBox::OnDestroy();

	// TODO: Add your message handler code here
}

void CSpecCombo::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
//	CComboBox::OnContextMenu(pWnd, point);
}


void CSpecCombo::OnCbnSelchange()
{
	// TODO: Add your control notification handler code here

	CString str;
	CFont* pFont;
	CMyFontDlg* pDlg;
	LOGFONT lf;

	if (GetDlgCtrlID()==cmb3 && GetCurSel()==-1)
		GetWindowText(str);
	else
	GetLBText(GetCurSel(), str);
	pDlg = (CMyFontDlg*)GetParent();
	pFont = pDlg->GetSelectedFont();
	pFont->GetLogFont(&lf);
	pFont->DeleteObject();
//	MessageBox(_T("SelChange"));

	switch (GetDlgCtrlID())
	{
	case cmb2: 
		{
			if (str.CompareNoCase(_T("BOLD"))==0)
			{
				lf.lfWeight = FW_BOLD;
				lf.lfItalic = FALSE;
			}
			else if (str.CompareNoCase(_T("REGULAR"))==0)
			{
				lf.lfWeight = FW_REGULAR;
				lf.lfItalic = FALSE;
			}
			else if (str.CompareNoCase(_T("ITALIC"))==0)
			{
				lf.lfWeight = FW_REGULAR;
				lf.lfItalic = TRUE;
			}
			else if (str.CompareNoCase(_T("BOLD ITALIC"))==0)
			{
				lf.lfWeight = FW_BOLD;
				lf.lfItalic = TRUE;
			}
		break;
		}

	case cmb3: 
		{
			int k = _wtoi(str);
			lf.lfHeight = -MulDiv(k, GetDeviceCaps(GetParent()->GetDC()->m_hDC, LOGPIXELSY), 72);
			break;
		}
		
	case cmb5: 
		if (str.CompareNoCase(_T("WESTERN"))==0)
			lf.lfCharSet = ANSI_CHARSET;
		else if (str.CompareNoCase(_T("HEBREW"))==0)
			lf.lfCharSet = HEBREW_CHARSET;
		else if (str.CompareNoCase(_T("ARABIC"))==0)
			lf.lfCharSet = ARABIC_CHARSET;
		else if (str.CompareNoCase(_T("GREEK"))==0)
			lf.lfCharSet = GREEK_CHARSET;
		else if (str.CompareNoCase(_T("TURKISH"))==0)
			lf.lfCharSet = TURKISH_CHARSET;
		else if (str.CompareNoCase(_T("BALTIC"))==0)
			lf.lfCharSet = BALTIC_CHARSET;
		else if (str.CompareNoCase(_T("CENTRAL EUROPEAN"))==0)
			lf.lfCharSet = ANSI_CHARSET;
		else if (str.CompareNoCase(_T("VIETNAMESE"))==0)
			lf.lfCharSet = VIETNAMESE_CHARSET;
		else if (str.CompareNoCase(_T("JAPANASE"))==0)
			lf.lfCharSet = SHIFTJIS_CHARSET;
		else if (str.CompareNoCase(_T("HANGUL"))==0)
			lf.lfCharSet = HANGUL_CHARSET;
		else if (str.CompareNoCase(_T("HANGUL(JOHAB)"))==0)
			lf.lfCharSet = JOHAB_CHARSET;
		else if (str.CompareNoCase(_T("CHINEZE_GB2312"))==0)
			lf.lfCharSet = GB2312_CHARSET;
		else if (str.CompareNoCase(_T("CHINESE_BIG5"))==0)
			lf.lfCharSet = CHINESEBIG5_CHARSET;
		else if (str.CompareNoCase(_T("THAI"))==0)
			lf.lfCharSet = THAI_CHARSET;
		else if (str.CompareNoCase(_T("MAC"))==0)
			lf.lfCharSet = MAC_CHARSET;
		else if (str.CompareNoCase(_T("SYMBOL"))==0)
			lf.lfCharSet = SYMBOL_CHARSET;
		else if (str.CompareNoCase(_T("OEM/DOS"))==0)
			lf.lfCharSet = OEM_CHARSET;
		else lf.lfCharSet = DEFAULT_CHARSET;
		break;
	}

	pFont->CreateFontIndirectW(&lf);
	GetParent()->InvalidateRect(&(pDlg->TextRect));
//	this->UpdateDialogControls
//	CallWindowProc(OldCBNProc,this->m_hWnd, WM_COMMAND, MAKEWPARAM(1000, CBN_SELCHANGE),0);
}

void CSpecCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  Add your code to draw the specified item

	//	CComboBox::OnDrawItem(nIDCtl, lpDrawItemStruct);
	
//	ASSERT(lpDrawItemStruct->CtlType == ODT_COMBOBOX);
//	LPCTSTR lpszText = (LPCTSTR) lpDrawItemStruct->itemData;
//	ASSERT(lpszText != NULL);
//	DRAWITEMSTRUCT
	CString Text;

	if (lpDrawItemStruct->itemID==-1) return;

	this->GetLBText(lpDrawItemStruct->itemID,Text);
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

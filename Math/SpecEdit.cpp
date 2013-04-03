// SpecEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Math.h"
#include "SpecEdit.h"
#include "dlgs.h"
#include "MyFontDlg.h"

// CSpecEdit

IMPLEMENT_DYNAMIC(CSpecEdit, CEdit)

CSpecEdit::CSpecEdit()
: m_ComboID(0)
, m_pDlg(NULL)
{

}

CSpecEdit::~CSpecEdit()
{
}


BEGIN_MESSAGE_MAP(CSpecEdit, CEdit)
	ON_WM_CONTEXTMENU()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CSpecEdit message handlers



void CSpecEdit::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
//	MessageBox(_T("DA"));
}

void CSpecEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	if (this->m_ComboID==cmb3)
	{
		CString str;
		LOGFONT lf;
		CFont* pFont = ((CMyFontDlg*)m_pDlg)->GetSelectedFont();
		pFont->GetLogFont(&lf);
		pFont->DeleteObject();
		
		GetWindowText(str);
		if (str.GetLength()==0)
		{
			::SendMessage(m_pDlg->GetDlgItem(cmb3)->m_hWnd, CB_SETCURSEL, 0, 0);
			GetWindowText(str);
		}

		int k = _wtoi(str);
		lf.lfHeight = -MulDiv(k, GetDeviceCaps(GetParent()->GetDC()->m_hDC, LOGPIXELSY), 72);
		pFont->CreateFontIndirectW(&lf);
		m_pDlg->InvalidateRect(&((CMyFontDlg*)m_pDlg)->TextRect);		
	}

	if (this->m_ComboID==cmb2)
	{
		CString str;
		GetWindowText(str);
		CComboBox* pComboBox = (CComboBox*)(m_pDlg->GetDlgItem(cmb2));
		if (str.GetLength()==0 || pComboBox->GetCurSel()==-1)
		{
			::SendMessageW(m_pDlg->GetDlgItem(cmb2)->m_hWnd, CB_SETCURSEL, 0, 0);
		}
	}
}

void CSpecEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (this->m_ComboID==cmb3)
	{
		if (::iswdigit(nChar) || ::iswcntrl(nChar))
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}

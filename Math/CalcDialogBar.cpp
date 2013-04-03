// CalcDialogBar.cpp : implementation file
//

#include "stdafx.h"
#include "Math.h"
#include "CalcDialogBar.h"
#include "Expresion.h"

extern CRichEditCtrl* pRichCtrl;

// CCalcDialogBar dialog

IMPLEMENT_DYNAMIC(CCalcDialogBar, CDialogBar)

CCalcDialogBar::CCalcDialogBar(CWnd* pParent /*=NULL*/)
: CDialogBar()
{
	DimEVechi = 0;
	DimFNou = 0;
	DimFVechi = 0;
}

CCalcDialogBar::~CCalcDialogBar()
{
}




BEGIN_MESSAGE_MAP(CCalcDialogBar, CDialogBar)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_CALCUL, &CCalcDialogBar::OnUpdateButtonCalcul)
	ON_BN_CLICKED(IDC_BUTTON_CALCUL, &CCalcDialogBar::OnButtonCalcul)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CCalcDialogBar message handlers

CCalcEdit* CCalcDialogBar::GetCalcEdit(void)
{
	return &m_EditCalcul;
}

void CCalcDialogBar::OnUpdateButtonCalcul(CCmdUI* pCmdUI)
{
	this->UpdateData(FALSE);

	if (::IsWindow(m_EditCalcul.m_hWnd))
	{
		pCmdUI->Enable(m_EditCalcul.GetWindowTextLength());
	}
	if (::IsWindow(m_ButtonCalcul.m_hWnd))
	{
	}
}
void CCalcDialogBar::OnButtonCalcul()
{
/*	CString str;
	CExpresion ep;

	m_EditCalcul.GetWindowText(str);
	ep.GetString(str);
	if (ep.IsValid())
		ep.CalcExpr();

	this->GetParentFrame()->GetActiveView()->SetWindowText(ep.ReturnString());

	m_EditCalcul.SetFocus();
	m_EditCalcul.SetSel(0, str.GetLength());*/

	CString str;
	CExpresion ep;

	//we get the text from the editbox
	CEdit* pEdit = (CEdit*) (GetDlgItem(IDC_EDIT_CALCUL));
	pEdit->GetWindowText(str);

	//we place the text to the expression object
	ep.GetString(str);
	//if the expression is valid, we ask for calculation:
	if (ep.CheckValid())
		ep.Calculate();

	CHARFORMAT2 chf2;
	memset(&chf2, 0, sizeof(CHARFORMAT2));
	chf2.cbSize = sizeof(CHARFORMAT2);
	chf2.dwMask = CFM_BACKCOLOR | CFM_COLOR;
	chf2.crTextColor = RGB(255,255,255);
	chf2.crBackColor = RGB(255,0,0);

	GetParentFrame()->GetActiveView()->SetWindowText(ep.ReturnString());
	pRichCtrl->SetSel(ep.GetHighBeg(), ep.GetHighEnd());
	pRichCtrl->SetSelectionCharFormat(chf2);

	CString s = ep.ReturnString();
	int poserror = s.Find(L"EROARE!");
	int err_pos1 = ep.GetErrorPos1();
	int err_pos2 = ep.GetErrorPos2();

	if (poserror > -1) 
	{
		poserror = s.ReverseFind('\n');
		err_pos1 = poserror;
		err_pos2 = ep.ReturnString().GetLength();
	}
	chf2.crTextColor = RGB(255,0,0);
	chf2.crBackColor = RGB(255,255,255);
	pRichCtrl->SetSel(err_pos1, err_pos2);
	pRichCtrl->SetSelectionCharFormat(chf2);
	pRichCtrl->SetSel(-1, -1);

	pEdit->SetFocus();
	pEdit->SetSel(0, str.GetLength());
	GetParentFrame()->GetActiveDocument()->SetModifiedFlag();
}

void CCalcDialogBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if (IsWindowVisible() && ::IsWindow(m_ButtonCalcul.m_hWnd) && ::IsWindow(m_EditCalcul.m_hWnd))
	{
		int dif(0);

		RECT rectB, rectD, rectE;
		DimFNou = cx;

		m_EditCalcul.GetWindowRect(&rectE);
		m_ButtonCalcul.GetWindowRect(&rectB);
		GetWindowRect(&rectD);

		if (rectE.right - rectE.left)
		DimEVechi = rectE.right - rectE.left;
		UINT dimy = rectE.bottom - rectE.top;

		dif = cx - DimFVechi;

		UINT posX = rectB.left + dif - rectD.left;
		UINT posY = rectB.top - rectD.top;
		UINT dimx;
		if (dif==cx) dimx = DimEVechi;
				else dimx= DimEVechi + dif;

		m_ButtonCalcul.SetWindowPos(&CWnd::wndTop, posX, posY, 0, 0, SWP_NOSIZE);
		m_EditCalcul.SetWindowPos(&CWnd::wndTop, 0, 0, dimx, dimy, SWP_NOMOVE);

		DimFVechi = DimFNou;
	}
}

int CCalcDialogBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	RECT rect;

	GetClientRect(&rect);
	DimFVechi = rect.right-rect.left;

	return 0;
}

void CCalcDialogBar::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class
	DDX_Control(pDX, IDC_BUTTON_CALCUL, m_ButtonCalcul);
	DDX_Control(pDX, IDC_EDIT_CALCUL, m_EditCalcul);

	CDialogBar::DoDataExchange(pDX);
}

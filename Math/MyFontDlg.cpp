// MyFontDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Math.h"
#include "MyFontDlg.h"
#include <dlgs.h>

static UINT_PTR CALLBACK FontDlgHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM /*lParam*/);
WNDPROC OldCBNProc;
WNDPROC OldEditProc;
WNDPROC OldListProc;

LRESULT CALLBACK NewListProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg==WM_CTLCOLOR)
	{
		MessageBox(hWnd, L"Scroll", L"Caption", 0);
	}

	if (uMsg==WM_CONTEXTMENU)
	{
		return 0;
	}

	return CallWindowProc(OldListProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg==WM_CONTEXTMENU)
	{
		return 0;
	}

	if (uMsg==WM_KILLFOCUS)
	{
		CMyFontDlg* pDlg = (CMyFontDlg*)CWnd::FromHandle(GetParent(GetParent(hWnd)));
		CComboBox* pComboBox = (CComboBox*)pDlg->GetDlgItem(cmb1);
		if (pComboBox->GetCurSel()==-1)
		{
			int i = pComboBox->SelectString(0,L"Times New Roman");
			SendMessageW(GetParent(hWnd), CB_SETCURSEL, i,0);
			LOGFONT lf;
			CFont* pFont;
			pFont = ((CMyFontDlg*)pDlg)->GetSelectedFont();
			pFont->GetLogFont(&lf);
			wcscpy_s(lf.lfFaceName, 16, L"Times New Roman");
			pFont->DeleteObject();

			pFont->CreateFontIndirectW(&lf);
			pDlg->InvalidateRect(&(((CMyFontDlg*)pDlg)->TextRect));
		}
	}

	return CallWindowProc(OldEditProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewCBNProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	HWND hWndEdit = NULL;
	HWND hWndList = NULL;
	static bool subclassed(0);
	static bool subclassedL(0);
	bool ala = false;


	if (uMsg==WM_COMMAND && HIWORD(wParam)==CBN_SELCHANGE && LOWORD(wParam)==1000)
	{
		ala = true;
		CString str;//sirul va prelua elementele string din comboboxes
		LOGFONT lf;//elementele vor fi pastrate aici
		CFont* pFont;//pointer la fontul selectat

		CMyFontDlg* pDlg = (CMyFontDlg*)CWnd::FromHandle(GetParent(hWnd));
		CComboBox* pCombo = (CComboBox*)pDlg->GetDlgItem(cmb1);
		pCombo->GetLBText(pCombo->GetCurSel(), str);

		pFont = pDlg->GetSelectedFont();
		pFont->GetLogFont(&lf);
		pFont->DeleteObject();

		for (int i=0; i<=(int)wcslen(lf.lfFaceName); i++)
			lf.lfFaceName[i] = str[i];

		pFont->CreateFontIndirectW(&lf);
		pDlg->InvalidateRect(&(pDlg->TextRect));
	}
	if (uMsg==WM_CTLCOLOREDIT)
	{
		hWndEdit = (HWND)lParam;
	}
	
	if (uMsg==WM_CTLCOLORLISTBOX)
	{
		hWndList = (HWND)lParam;
	}

	if (hWndEdit && !subclassed)
	{
		OldEditProc = (WNDPROC)SetWindowLongPtr(hWndEdit, GWL_WNDPROC,(LONG_PTR)NewEditProc); 
		subclassed = true;
	}
	if (hWndList && !subclassedL)
	{
		OldListProc = (WNDPROC)SetWindowLongPtrW(hWndList, GWL_WNDPROC, (LONG_PTR)NewListProc);
		subclassedL = true;
	}

	LRESULT lr = CallWindowProc(OldCBNProc, hWnd, uMsg, wParam, lParam);
	if (ala == true)
	{
		CString str;
		LOGFONT lf;
		CFont* pFont;
		CMyFontDlg* pDlg = (CMyFontDlg*)CWnd::FromHandle(GetParent(hWnd));
	
	//pentru bold/italic..
		CComboBox* pCombo = (CComboBox*)pDlg->GetDlgItem(cmb2);
		pCombo->GetLBText(pCombo->GetCurSel(), str);
		pFont = pDlg->GetSelectedFont();
		pFont->GetLogFont(&lf);
		pFont->DeleteObject();


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
	//pentru size

		pCombo = (CComboBox*)pDlg->GetDlgItem(cmb3);
		int CurSel = pCombo->GetCurSel();
		if (CurSel>-1)
			pCombo->GetLBText(pCombo->GetCurSel(), str);
		else pCombo->GetWindowTextW(str);

		int k = _wtoi(str);
		lf.lfHeight = -MulDiv(k, GetDeviceCaps(pDlg->GetDC()->m_hDC, LOGPIXELSY), 72);

	//pentru script
		pCombo = (CComboBox*)pDlg->GetDlgItem(cmb5);
		pCombo->GetWindowTextW(str);

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

/***********************************/
		pFont->CreateFontIndirectW(&lf);
		pDlg->InvalidateRect(&(pDlg->TextRect));
		
	}
	return lr;
}

#ifndef _WIN32_WCE // CFontDialog is not supported for Windows CE.

// CMyFontDlg

IMPLEMENT_DYNAMIC(CMyFontDlg, CFontDialog)

CMyFontDlg::CMyFontDlg(LPLOGFONT lplfInitial, DWORD dwFlags, CDC* pdcPrinter, CWnd* pParentWnd) : 
	CFontDialog(lplfInitial, dwFlags, pdcPrinter, pParentWnd)
{
}

CMyFontDlg::~CMyFontDlg()
{
}


BEGIN_MESSAGE_MAP(CMyFontDlg, CFontDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()



// CMyFontDlg message handlers


#endif // !_WIN32_WCE

BOOL CMyFontDlg::OnInitDialog()
{
	CFontDialog::OnInitDialog();

	m_cf.Flags         |= CF_ENABLEHOOK;
	m_cf.lpfnHook       = FontDlgHookProc;

	LOGFONT lf;
	GetCurrentFont(&lf);
	m_SelectedFont.CreateFontIndirectW(&lf);

	OldCBNProc = (WNDPROC)SetWindowLongPtr(GetDlgItem(cmb1)->m_hWnd, GWL_WNDPROC, (LONG_PTR)NewCBNProc);

	m_Combo2.SubclassDlgItem(cmb2, this);
	m_Combo3.SubclassDlgItem(cmb3, this);
	m_Combo5.SubclassDlgItem(cmb5, this);

	this->ModifyStyleEx(WS_EX_CONTEXTHELP, 0);

	// TODO:  Add extra initialization here

	SetWindowText(_T("Fontul"));

	SetDlgItemText(IDOK, _T("Acceptă"));
	SetDlgItemText(IDCANCEL, _T("Anulează"));

	GetDlgItem(chx1)->ShowWindow(FALSE);
	GetDlgItem(chx2)->ShowWindow(FALSE);

	GetDlgItem(grp1)->ShowWindow(FALSE);
	SetDlgItemText(grp2, _T("Exemplar"));

	SetDlgItemText(stc1, _T("Fontul:"));
	SetDlgItemText(stc2, _T("Stilul:"));
	SetDlgItemText(stc3, _T("Mărimea:"));
	GetDlgItem(stc4)->ShowWindow(FALSE);
	SetDlgItemText(stc7, _T("Scrierea:"));

	GetDlgItem(cmb4)->ShowWindow(FALSE);//cmb4=Attach

	//butonul Help
	//left&right
	RECT rect_but,r_aux;
	GetDlgItem(cmb1)->GetWindowRect(&r_aux);
	ScreenToClient(&r_aux);
	rect_but.left = r_aux.left;
	rect_but.right = r_aux.right;

	//top&bottom
	GetDlgItem(cmb5)->GetWindowRect(&r_aux);
	ScreenToClient(&r_aux);
	rect_but.top = r_aux.top;
	rect_but.bottom = r_aux.bottom;

	m_bHelp.Create(_T("Ajutor"), WS_CHILD | WS_VISIBLE, rect_but,this, IDC_BUTTON_HELP);
	m_bHelp.SetFont(GetDlgItem(IDOK)->GetFont());
	m_bHelp.GetWindowRect(&rect_but);
	ScreenToClient(&rect_but);

	//Frame-ul de la Sample
	int dimX,dimY,posY;
	GetDlgItem(grp2)->GetWindowRect(&r_aux);
	this->RedrawWindow();

	ScreenToClient(&r_aux);
	dimX = r_aux.right - rect_but.left;
	dimY = r_aux.bottom - r_aux.top;
	posY = r_aux.top;

	GetDlgItem(grp2)->SetWindowPos(NULL, rect_but.left,posY,dimX,dimY, SWP_NOZORDER);

//	CComboBoxEx:

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMyFontDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class
//	DDX_Control(pDX, cmb1, m_cbFontul);

	CFontDialog::DoDataExchange(pDX);
}

BOOL CMyFontDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFontDialog::OnNotify(wParam, lParam, pResult);
}

void CMyFontDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFontDialog::OnPaint() for painting messages

	//EditBox
	RECT rect_but;
	GetDlgItem(grp2)->GetWindowRect(&rect_but);
	ScreenToClient(&rect_but);

	
	rect_but.bottom-=10;
	rect_but.top+=15;
	rect_but.left+=10;
	rect_but.right-=10;
	TextRect = rect_but;
	dc.Rectangle(&rect_but);

	rect_but.bottom-=2;
	rect_but.top+=2;
	rect_but.left+=2;
	rect_but.right-=2;
	dc.Rectangle(&rect_but);
/*	aFont.DeleteObject();*/
		dc.SetBkColor(RGB(255,255,255));
	dc.SelectObject(m_SelectedFont);
	dc.DrawTextEx(_T("5+4log(3,6)*9/3e^rad6="), -1, &rect_but,
					DT_VCENTER | DT_CENTER | DT_SINGLELINE, NULL);

/*	LOGFONT*********seeek*/
}

int CMyFontDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFontDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// CFHookProc
static UINT_PTR CALLBACK FontDlgHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	
	if (uiMsg==WM_CONTEXTMENU)
	{
//		MessageBox(hdlg,_T("Context!"), _T("Caption"), 0);
		return TRUE;
	}

	return 0;
}

INT_PTR CMyFontDlg::DoModal(void)
{
	

	return CFontDialog::DoModal();
}

CFont* CMyFontDlg::GetSelectedFont(void)
{
	return &m_SelectedFont;
}

BOOL CMyFontDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: Add your message handler code here and/or call default
	OnHelp();

	return FALSE;
}

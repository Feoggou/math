// MyFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Math.h"
#include "MyFileDlg.h"
#include "Mesaj.h"
#include <dlgs.h>
#include <commctrl.h>
#include "Settings.h"
#include "hlp\\HTMLDefines.h"

WNDPROC OldWndProc;
extern CSettings Settings;
extern bool ShowResult;
extern int nrMaxim;
BOOL SavedBefore = FALSE;
CString OfficialName;

int ReplaceMessage(HWND hWnd)
{
	CMesaj dlg(CWnd::FromHandle(hWnd));
	return dlg.DoModal();
}

LRESULT CALLBACK NewWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg==WM_HELP)
	{
/*		MessageBox(0, L"HELP!", 0, 0);
		CWnd::FromHandle(hWnd)->SetFocus();
		CWnd::FromHandle(hWnd)->OnHelp();
		return true;*/
	}

	if (uMsg==WM_COMMAND && HIWORD(wParam)==BN_CLICKED && LOWORD(wParam)==IDC_BUTTON_SAVE)
	{
		CFileDialog* pParent = (CFileDialog*)(CWnd::FromHandle(hWnd)->GetDlgItem(0));
		CString str = pParent->GetPathName();
		CString title = pParent->GetFileTitle();

		for (int i = 0; i< title.GetLength(); i++)
			if (iswalnum(title[i])==FALSE)
			{
				MessageBox(hWnd, L"Numele fişierului nu poate conţine decât litere şi cifre", L"Nume invalid", 0);
				return 0;
			}
		
		if (str.Right(4).CompareNoCase(L".txt")!=0)
			str+=L".txt";

		CFrameWnd* pFrame = (CFrameWnd*)CWnd::FromHandle(hWnd)->GetParentFrame();
		CDocument* pDoc = pFrame->GetActiveDocument();
		
		CFile theFile;
		bool problem = true;
		bool opened = true;
		CFileException e;
		if (!theFile.Open(str, CFile::modeRead, &e))
		{
			if (e.m_cause==CFileException::fileNotFound)
			{
				opened = false;
				problem = false;
			}
		}

		if (problem && ReplaceMessage(hWnd)==IDOK || !problem)
		{
			if (opened) theFile.Close();
			SendMessage(hWnd, WM_CLOSE,0,0);

			OfficialName = str;
			theFile.Open(str, CFile::modeWrite | CFile::modeCreate);
			TCHAR path[260];
			CString s, t;
			t = theFile.GetFileTitle();
			s = theFile.GetFilePath();
			s = s.Left(s.GetLength() - t.GetLength()-1);

			wcscpy(path, s);
			Settings.SaveSettings(ShowResult, nrMaxim, path);
			CArchive archive(&theFile, CArchive::store);
			pDoc->Serialize(archive);
			archive.Close();
			theFile.Close();
			SavedBefore = TRUE;
		}
	}

	else if (uMsg==WM_COMMAND && HIWORD(wParam)==BN_CLICKED && LOWORD(wParam)==IDOK)
	{
		HWND m_hWnd = CWnd::FromHandle(hWnd)->GetDlgItem(IDC_BUTTON_SAVE)->m_hWnd;
		SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_BUTTON_SAVE, BN_CLICKED), (LPARAM)m_hWnd);
		return 0;
	}

	else if (uMsg==WM_COMMAND && HIWORD(wParam)==BN_CLICKED && LOWORD(wParam)==IDC_BUTTON_HELPF)
	{
//		MessageBox(0, L"HELP!", 0, 0);
		CWnd::FromHandle(hWnd)->GetParentFrame()->OnContextHelp();

		return 0;
	}

	switch (uMsg)
	{
	case WM_CONTEXTMENU:
		return 1;
	}

	return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
}


// CMyFileDlg

IMPLEMENT_DYNAMIC(CMyFileDlg, CFileDialog)

CMyFileDlg::CMyFileDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{

}

CMyFileDlg::~CMyFileDlg()
{
	m_MySaveButton.Detach();
//	m_MySaveButton.DestroyWindow();
}


BEGIN_MESSAGE_MAP(CMyFileDlg, CFileDialog)
END_MESSAGE_MAP()

// CMyFileDlg message handlers

BOOL CMyFileDlg::OnInitDialog()
{
	CFileDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	SetWindowContextHelpId(AFX_HIDD_FILESAVE);
	GetParent()->ModifyStyleEx(WS_EX_CONTEXTHELP, 0);

	HWND hWnd = GetParent()->m_hWnd;
	OldWndProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)NewWndProc);
	GetParent()->SetWindowTextW(L"Salvează ca");

	//pt butonul OK, il schimbam cu altul.
	RECT rect_but;
	CWnd* pWnd = GetParent()->GetDlgItem(IDOK);
	pWnd->GetWindowRect(&rect_but);
	pWnd->ShowWindow(0);
	ScreenToClient(&rect_but);
	m_MySaveButton.Create(L"Salvează", WS_VISIBLE | BS_DEFPUSHBUTTON, rect_but, GetParent(), IDC_BUTTON_SAVE);
	m_MySaveButton.SetFont(GetParent()->GetDlgItem(IDCANCEL)->GetFont(), 1);
	RECT rect_Cancel;
	GetParent()->GetDlgItem(IDCANCEL)->GetWindowRect(&rect_Cancel);
	ScreenToClient(&rect_Cancel);
	int plus = rect_Cancel.top - rect_but.top;
	rect_but.top+=plus+plus;
	rect_but.bottom+=plus+plus;

	m_HelpButton.Create(L"Ajutor", WS_VISIBLE, rect_but, GetParent(), IDC_BUTTON_HELPF);
	m_HelpButton.SetFont(GetParent()->GetDlgItem(IDCANCEL)->GetFont(), 1);

	//Celelalte butoane

	GetParent()->GetDlgItem(IDCANCEL)->SetWindowTextW(L"Anulează");
	
	GetParent()->GetDlgItem(stc2)->SetWindowTextW(L"Tipul fişierului:");
	GetParent()->GetDlgItem(stc3)->SetWindowTextW(L"Numele fişierului:");
	GetParent()->GetDlgItem(stc4)->SetWindowTextW(L"Salvează în:");


	//pentru toolbar-ul de sus ascundem butonul 3, iar pt
	//toolbar-ul din stanga schimbam numele
	CWnd* pChild = GetParent()->GetWindow(GW_CHILD);
	bool exit1 = 0;
	bool exit2 = 0;
	while (pChild)
	{
		TCHAR strName[30];
		::GetClassName(pChild->m_hWnd, strName, 30);
		if (lstrcmp(strName, L"ToolbarWindow32")==0)
		{
			if (pChild->GetDlgCtrlID()==0x04a0)//verticala
			{
				exit1 = true;
				
				m_ToolBar.SubclassWindow(pChild->m_hWnd);
			}
			if (pChild->GetDlgCtrlID()==0x0440)//orizontala
			{
				exit2 = true;

				CString s;
				CToolBarCtrl* pTool = (CToolBarCtrl*)pChild;
				TBBUTTON tb;
				pTool->GetButton(3, &tb);
				TBBUTTONINFO tbi;
				tbi.dwMask = TBIF_STATE | TBIF_BYINDEX;
				tbi.fsState = TBSTATE_HIDDEN;
				tbi.cbSize = sizeof(tbi);
				pTool->SetButtonInfo(3, &tbi);
			}
		}
		if (exit1 && exit2) break;
		pChild = pChild->GetNextWindow();
	}

	//Alte customizari

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
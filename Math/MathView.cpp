// MathView.cpp : implementation of the CMathView class
//

#include "stdafx.h"
#include "Math.h"

#include "MathDoc.h"
#include "CntrItem.h"
#include "MathView.h"

#include "MainFrm.h"
#include "Options.h"
#include "MyFileDlg.h"
#include "Settings.h"
#include "MyFontDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSettings Settings;
bool ShowResult(FALSE);
int nrMaxim(2);
CRichEditCtrl* pRichCtrl;
TCHAR path[260];
extern BOOL SavedBefore;
extern CString OfficialName;

// CMathView

IMPLEMENT_DYNCREATE(CMathView, CRichEditView)

BEGIN_MESSAGE_MAP(CMathView, CRichEditView)
ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_OPTIONS, &CMathView::OnViewOptions)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, &CMathView::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CMathView::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CMathView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CMathView::OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, &CMathView::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CMathView::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, &CMathView::OnEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, &CMathView::OnUpdateEditClearAll)
	ON_COMMAND(ID_FORMAT_FONT, &CMathView::OnFormatFont)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMathView::OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, &CMathView::OnFileSave)
END_MESSAGE_MAP()

// CMathView construction/destruction

CMathView::CMathView()
{
	// TODO: add construction code here

}

CMathView::~CMathView()
{
}

void CMathView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	CWnd* pWnd = GetFocus();
    if (  NULL == pWnd ||
          !IsEditBoxWithFocus( pWnd ) ||
          (pWnd->GetStyle() & ES_READONLY) != 0 )
    {
       pCmdUI->Enable( FALSE );
    }
    else
       pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));
}

void CMathView::OnEditUndo()
{
	CEdit* pEdit = (CEdit*)GetFocus();
    ASSERT( IsEditBoxWithFocus( pEdit) );
    ASSERT(::IsClipboardFormatAvailable(CF_TEXT));
    pEdit->Undo();
}

void CMathView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	CWnd* pWnd = GetFocus();
    if (NULL == pWnd || !IsEditBoxWithFocus( pWnd ))
    {
       pCmdUI->Enable( FALSE );
    }
    else
    {
       CEdit* pEdit = (CEdit*)pWnd;
       int nBeg, nEnd;

       pEdit->GetSel( nBeg, nEnd );
       pCmdUI->Enable( nBeg != nEnd );
    }
}

void CMathView::OnEditClearAll()
{
	this->SetWindowTextW(_T(""));
}

void CMathView::OnUpdateEditClearAll(CCmdUI *pCmdUI)
{
	CString text;
	GetWindowTextW(text);
	pCmdUI->Enable(text.GetLength());
}

void CMathView::OnFileSaveAs()
{
	// TODO: Add your command handler code here

	WCHAR filter[] = L"Fişier Text (*.txt)||";
	CMyFileDlg fdlg(0,L"TXT", L"Exerciţiu",
OFN_OVERWRITEPROMPT, filter, this);
	fdlg.m_ofn.lpstrInitialDir = path;

	fdlg.DoModal();
}

void CMathView::OnFileSave()
{
	// TODO: Add your command handler code here

	if (SavedBefore)
	{
		CFile theFile;

		theFile.Open(OfficialName, CFile::modeWrite | CFile::modeCreate);

		Settings.SaveSettings(ShowResult, nrMaxim, path);
		CArchive archive(&theFile, CArchive::store);
		CMathDoc* pDoc = (CMathDoc*)GetDocument();
		GetRichEditCtrl().SetModify();
		pDoc->Serialize(archive);
		archive.Close();
		theFile.Close();
	}
	else CMathView::OnFileSaveAs();
}

void CMathView::OnFormatFont()
{
	LOGFONT logfont;
	myFont.GetLogFont(&logfont);
	CMyFontDlg fdlg(&logfont);
	if (fdlg.DoModal()==IDOK)
	{
		myFont.DeleteObject();
		fdlg.GetCurrentFont(&logfont);
		myFont.CreateFontIndirectW(&logfont);
		SetFont(&myFont);
	}
}

BOOL CMathView::IsEditBoxWithFocus(CWnd *pWnd)
{
	ASSERT( pWnd != NULL );
    HWND hWnd = pWnd->GetSafeHwnd();
    if (hWnd == NULL)
		return FALSE;

	TCHAR szClassName[6];
    return ::GetClassName(hWnd, szClassName, 6) && _tcsicmp(szClassName, _T("Edit")) == 0;
}

BOOL CMathView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CMathView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();
	pRichCtrl = &GetRichEditCtrl();


	// Set the printing margins (720 twips = 1/2 inch)
	SetMargins(CRect(720, 720, 720, 720));

	if (myFont.m_hObject)
		myFont.DeleteObject();
	myFont.CreateFontW(21, 0, 0, 0, FW_REGULAR, 0,0,0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_ROMAN,_T("Times New Roman"));
	SetFont(&myFont);
	CMainFrame* pMain = (CMainFrame*)GetParentFrame();
	CWnd *pWnd = pMain->GetCalcEdit();
	if (::IsWindow(pWnd->m_hWnd))
		pWnd->SetWindowTextW(L"");
}

void CMathView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CRichEditView::OnDestroy();
}



// CMathView diagnostics

#ifdef _DEBUG
void CMathView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CMathView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CMathDoc* CMathView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMathDoc)));
	return (CMathDoc*)m_pDocument;
}
#endif //_DEBUG


// CMathView message handlers

void CMathView::OnSetFocus(CWnd* pOldWnd)
{
	CRichEditView::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here

	HideCaret();
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	pMainFrame->m_CalcDialogBar.GetDlgItem(IDC_EDIT_CALCUL)->SetFocus();
}

void CMathView::OnViewOptions()
{
	// TODO: Add your command handler code here
	COptions Options;
	Options.DoModal();
}

void CMathView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu ContextMenu;

	if (ContextMenu.CreatePopupMenu())
	{
		CString str;
		GetWindowText(str);
		if (str.GetLength())
		{
			str = GetRichEditCtrl().GetSelText();
			if (str.GetLength())
				ContextMenu.InsertMenu(0, MF_STRING | MF_BYPOSITION, ID_EDIT_COPY, L"&Copiere");
			else ContextMenu.InsertMenu(0, MF_STRING | MF_BYPOSITION | MF_GRAYED, ID_EDIT_COPY, L"&Copiere");
			ContextMenu.AppendMenu(MF_SEPARATOR);
			ContextMenu.AppendMenu(MF_STRING, ID_EDIT_SELECT_ALL, L"&Selecteaza tot");
		}

		else 
		{	
			ContextMenu.InsertMenu(0, MF_STRING | MF_GRAYED, ID_EDIT_COPY, L"&Copiere");
			ContextMenu.AppendMenu(MF_SEPARATOR);
			ContextMenu.AppendMenu(MF_STRING | MF_GRAYED, ID_EDIT_SELECT_ALL, L"&Selecteaza tot");
		}

		ContextMenu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}

	else MessageBox(L"Crearea meniului contextual esuata!", L"Eroare!", MB_ICONSTOP);
}

/*void CMathView::OnEditCopy2()
{
	// TODO: Add your command handler code here
	this->GetRichEditCtrl().Copy();
}

void CMathView::OnUpdateEditCopy2(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	long nBeg, nEnd;

	this->GetRichEditCtrl().GetSel(nBeg, nEnd );
	pCmdUI->Enable( nBeg != nEnd );
}*/

void CMathView::OnEditPaste()
{
	CEdit* pEdit = (CEdit*)GetFocus();
    ASSERT( IsEditBoxWithFocus( pEdit) );
    ASSERT(::IsClipboardFormatAvailable(CF_TEXT));
    pEdit->Paste();
}

//void CMathView::On()
//{
//	// TODO: Add your command handler code here
//}

void CMathView::OnEditCopy()
{
	CEdit* pEdit = (CEdit*)GetFocus();
    ASSERT( IsEditBoxWithFocus( pEdit) );
    pEdit->Copy();
}

void CMathView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	CWnd* pWnd = GetFocus();
    if (NULL == pWnd || !IsEditBoxWithFocus( pWnd ))
    {
       pCmdUI->Enable( FALSE );
    }
    else
    {
       CEdit* pEdit = (CEdit*)pWnd;
       int nBeg, nEnd;

       pEdit->GetSel( nBeg, nEnd );
       pCmdUI->Enable( nBeg != nEnd );
    }
}

//void CMathView::OnClose()
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CRichEditView::OnClose();
//	MessageBox(L"da!");
//	this->OnCommand((WPARAM)ID_APP_EXIT,0);
//}

/*void CMathView::OnAjutorDocumentatie()
{
	
	SHELLEXECUTEINFOW sei;
	memset(&sei, 0, sizeof(SHELLEXECUTEINFOW));
	sei.cbSize = sizeof(SHELLEXECUTEINFOW);
	sei.hwnd = this->m_hWnd;
	sei.lpVerb = L"open";
	sei.lpFile = L"Citeste-ma!.htm";
	sei.nShow = SW_SHOW;
	ShellExecuteExW(&sei);
}
*/
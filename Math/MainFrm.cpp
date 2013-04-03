// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Math.h"

#include "MainFrm.h"
#include "Expresion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString Last = _T("");
CString AlmLast = _T("");


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, &CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, &CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, &CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, &CFrameWnd::OnHelpFinder)
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_EEDIT_COPY, &CMainFrame::OnEeditCopy)
	ON_UPDATE_COMMAND_UI(ID_EEDIT_COPY, &CMainFrame::OnUpdateEeditCopy)
	ON_COMMAND(ID_EEDIT_SELECT_ALL, &CMainFrame::OnEeditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EEDIT_SELECT_ALL, &CMainFrame::OnUpdateEeditSelectAll)
	ON_COMMAND(ID_EEDIT_CUT, &CMainFrame::OnEeditCut)
	ON_UPDATE_COMMAND_UI(ID_EEDIT_CUT, &CMainFrame::OnUpdateEeditCut)
	ON_UPDATE_COMMAND_UI(ID_EEDIT_DELETE, &CMainFrame::OnUpdateEeditDelete)
	ON_COMMAND(ID_EEDIT_PASTE, &CMainFrame::OnEeditPaste)
	ON_UPDATE_COMMAND_UI(ID_EEDIT_PASTE, &CMainFrame::OnUpdateEeditPaste)
	ON_COMMAND(ID_EEDIT_UNDO, &CMainFrame::OnEeditUndo)
	ON_UPDATE_COMMAND_UI(ID_EEDIT_UNDO, &CMainFrame::OnUpdateEeditUndo)
	ON_WM_PAINT()
//	ON_COMMAND(ID_EEDIT_DELETE, &CMainFrame::OnEeditDelete)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_CalcDialogBar.Create(this, IDD_DIALOGBAR_CALCUL, CBRS_BOTTOM, IDD_DIALOGBAR_CALCUL))
	{
		TRACE0("Failed to create dialog bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CMenu *SysMenu = GetSystemMenu(FALSE);

	SysMenu->ModifyMenuW(0, MF_STRING | MF_BYPOSITION, SC_RESTORE, _T("&Restaureaza"));
	SysMenu->ModifyMenuW(1, MF_STRING | MF_BYPOSITION, SC_MOVE, _T("&Misca"));
	SysMenu->ModifyMenuW(2, MF_STRING | MF_BYPOSITION, SC_SIZE, _T("M&arime"));
	SysMenu->ModifyMenuW(3, MF_STRING | MF_BYPOSITION, SC_MINIMIZE, _T("Mi&nimizeaza"));
	SysMenu->ModifyMenuW(4, MF_STRING | MF_BYPOSITION, SC_MAXIMIZE, _T("Ma&ximizeaza"));
	SysMenu->ModifyMenuW(5, MF_SEPARATOR | MF_BYPOSITION, 0, _T(""));
	SysMenu->ModifyMenuW(6, MF_STRING | MF_BYPOSITION, SC_CLOSE, _T("&Inchide	Alt+F4"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers





void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x=250;
	lpMMI->ptMinTrackSize.y=200;
}

CCalcEdit* CMainFrame::GetCalcEdit(void)
{
	return m_CalcDialogBar.GetCalcEdit();
}

void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class

	CFrameWnd::DoDataExchange(pDX);
}

void CMainFrame::OnEeditCopy()
{
	CEdit* pEdit = GetCalcEdit();
	pEdit->Copy();
}

void CMainFrame::OnUpdateEeditCopy(CCmdUI *pCmdUI)
{
	CEdit* pEdit = GetCalcEdit();
	int nFirst, nLast;
	pEdit->GetSel(nFirst, nLast);
	pCmdUI->Enable(nFirst<nLast);
}

void CMainFrame::OnEeditSelectAll()
{
	CEdit* pEdit = GetCalcEdit();
	CString s;

	pEdit->GetWindowTextW(s);
	pEdit->SetSel(0, s.GetLength());
}

void CMainFrame::OnUpdateEeditSelectAll(CCmdUI *pCmdUI)
{
	CString s;
	CEdit* pEdit = GetCalcEdit();

	pEdit->GetWindowTextW(s);
	pCmdUI->Enable(s.GetLength());
}

void CMainFrame::OnEeditCut()
{
	CEdit* pEdit = GetCalcEdit();
	pEdit->Cut();
}

void CMainFrame::OnUpdateEeditCut(CCmdUI *pCmdUI)
{
	CEdit* pEdit = GetCalcEdit();
	int nFirst, nLast;
	pEdit->GetSel(nFirst, nLast);
	pCmdUI->Enable(nFirst<nLast);
}


void CMainFrame::OnUpdateEeditDelete(CCmdUI *pCmdUI)
{
	CEdit* pEdit = GetCalcEdit();
	int nFirst, nLast;
	pEdit->GetSel(nFirst, nLast);
	pCmdUI->Enable(nFirst<nLast);
}

void CMainFrame::OnEeditPaste()
{
	GetCalcEdit()->Paste();
}

void CMainFrame::OnUpdateEeditPaste(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(1);
}

void CMainFrame::OnEeditUndo()
{
	CString aux;
	GetCalcEdit()->GetWindowTextW(Last);
	aux = AlmLast;
	AlmLast = Last;
	Last = aux;

	GetCalcEdit()->Undo();
}

void CMainFrame::OnUpdateEeditUndo(CCmdUI *pCmdUI)
{
	CString s;
	GetCalcEdit()->GetWindowTextW(s);
	pCmdUI->Enable(s.GetLength() || AlmLast.GetLength());
}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFrameWnd::OnPaint() for painting messages
	this->SetWindowTextW(_T("Math"));
}
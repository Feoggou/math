// MyToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "Math.h"
#include "MyToolBar.h"


// CMyToolBar

IMPLEMENT_DYNAMIC(CMyToolBar, CToolBarCtrl)

CMyToolBar::CMyToolBar()
{

}

CMyToolBar::~CMyToolBar()
{
}


BEGIN_MESSAGE_MAP(CMyToolBar, CToolBarCtrl)
	ON_WM_NCPAINT()
END_MESSAGE_MAP()



// CMyToolBar message handlers



void CMyToolBar::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CToolBarCtrl::OnNcPaint() for painting messages
	
	TCHAR sir[5][30] =
	{L"Documentele mele recente",
	L"Suprafaţa de lucru",
	L"Documentele mele",
	L"Calculatorul meu",
	L"Reţeaua mea"};

//	TBBUTTON tb;
	TBBUTTONINFO tbi;
	tbi.dwMask = TBIF_BYINDEX | TBIF_TEXT;

	for (int i = 0; i<5; i++)
	{
//		GetButton(i, &tb);
		tbi.pszText = sir[i];
		tbi.cbSize = sizeof(tbi);
		SetButtonInfo(i, &tbi);
	}
		
}

// Options.cpp : implementation file
//

#include "stdafx.h"
#include "Math.h"
#include "Options.h"
#include "Settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool ShowResult;
extern BOOL ShowPrezentare;
extern int nrMaxim;
extern CSettings Settings;
extern TCHAR path[260];

/////////////////////////////////////////////////////////////////////////////
// COptions dialog


COptions::COptions(CWnd* pParent /*=NULL*/)
	: CDialog(COptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptions)
	m_cResult = ShowResult;
	m_eNrMaxim = nrMaxim;

	//}}AFX_DATA_INIT
}


void COptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptions)
	DDX_Check(pDX, IDC_CHK_ONLY_RESULT, m_cResult);
	DDX_Text(pDX, IDC_EDIT_NRMAXIM, m_eNrMaxim);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptions, CDialog)
	//{{AFX_MSG_MAP(COptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptions message handlers

void COptions::OnOK() 
{
	// TODO: Add extra validation here
	
	UpdateData();
	ShowResult = m_cResult;
	if (m_eNrMaxim>=0 && m_eNrMaxim<=10)
	nrMaxim = m_eNrMaxim;
	else AfxMessageBox((PCTSTR)"Trebuie sa introduci un numar cuprins intre 0 si 10");
	CDialog::OnOK();
	Settings.SaveSettings(ShowResult, nrMaxim, path);
}

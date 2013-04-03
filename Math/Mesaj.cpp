// Mesaj.cpp : implementation file
//

#include "stdafx.h"
#include "Math.h"
#include "Mesaj.h"


// CMesaj dialog

IMPLEMENT_DYNAMIC(CMesaj, CDialog)

CMesaj::CMesaj(CWnd* pParent /*=NULL*/)
	: CDialog(CMesaj::IDD, pParent)
{

}

CMesaj::~CMesaj()
{
}

void CMesaj::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMesaj, CDialog)
	ON_BN_CLICKED(IDOK, &CMesaj::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMesaj::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMesaj message handlers

void CMesaj::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CMesaj::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

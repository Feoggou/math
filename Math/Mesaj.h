#pragma once


// CMesaj dialog

class CMesaj : public CDialog
{
	DECLARE_DYNAMIC(CMesaj)

public:
	CMesaj(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMesaj();

// Dialog Data
	enum { IDD = IDD_MESSAJ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
};

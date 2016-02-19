// PortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WriteFee.h"
#include "PortDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortDlg dialog


CPortDlg::CPortDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortDlg)
	m_nPort = 0;
	//}}AFX_DATA_INIT
}


void CPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortDlg)
	DDX_CBIndex(pDX, IDC_COMBO1, m_nPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortDlg, CDialog)
	//{{AFX_MSG_MAP(CPortDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortDlg message handlers

void CPortDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}

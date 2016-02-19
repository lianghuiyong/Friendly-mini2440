// DownFile.cpp : implementation file
//

#include "stdafx.h"
#include "WriteFee.h"
#include "DownFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownFile dialog


CDownFile::CDownFile(CWnd* pParent /*=NULL*/)
	: CDialog(CDownFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownFile)
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT
}


void CDownFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownFile)
	DDX_Text(pDX, IDC_EDIT1, m_strFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownFile, CDialog)
	//{{AFX_MSG_MAP(CDownFile)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownFile message handlers

void CDownFile::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CFileDialog filedlg(TRUE, NULL, NULL,
		OFN_READONLY | OFN_FILEMUSTEXIST,
		NULL,NULL);
	filedlg.m_ofn.lpstrTitle = _T("浏览");
	if (filedlg.DoModal() == IDOK)
	{
		m_strFilePath = filedlg.GetPathName();
		UpdateData(FALSE);

	}
}



BOOL CDownFile::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch(m_param){
	case 0x1:
		SetWindowText("参数");
		break;
	case 0x2:
		SetWindowText("主费率");
		break;
	case 0x3:
		SetWindowText("备费率");
		break;
	case 0x4:
		SetWindowText("字库");
		break;
	case 0x5:
		SetWindowText("程序");
		break;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



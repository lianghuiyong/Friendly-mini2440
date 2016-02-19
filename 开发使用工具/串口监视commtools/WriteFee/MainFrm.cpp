// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "WriteFee.h"

#include "MainFrm.h"
#include "DownFile.h"
#include "serial.h"

#include "WriteFeeView.h"
#include "PortDlg.h"
#include "ProgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_PARAM,ID_ALL,OnDown)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
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

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

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

/////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnDown(UINT nID ) 
{
	// TODO: Add your command handler code here
	CPortDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		m_nPort =dlg.m_nPort;
	}
	else
		return;
	
	if(nID == ID_ALL){
		for(int i=1; i<= 5; i++) {
			Send(i);
			Sleep(3);
		}
	}
	else {
		BYTE para = nID - ID_PARAM +1;
		Send(para );
	}

}

BOOL CMainFrame::Send(BYTE para)
{
	CSerial serial;
	serial.m_nComID = m_nPort ;
	CString str;
	switch(para){
	case 0x01:
		 str = ((CWriteFeeView *)GetActiveView())->m_strParam;
		 break;
	case 0x02:
		str = ((CWriteFeeView *)GetActiveView())->m_strFee;
		break;
	case 0x03:
		str = ((CWriteFeeView *)GetActiveView())->m_strSFee;
		break;
	case 0x04:
		str = ((CWriteFeeView *)GetActiveView())->m_strFont;
		break;
	case 0x05:
		str = ((CWriteFeeView *)GetActiveView())->m_strProg;
		break;

	}
	if(str.IsEmpty())
		return FALSE;
	if(!serial.Open())
		return FALSE;;

	CProgressDlg dlg;
	dlg.Create();
	dlg.SetStatus("等待...");

	CFile file(str,CFile::modeRead);
	int len = file.GetLength();
	BYTE buf[2] = {0x57};
	buf[1] = para;
	serial.Write(buf,2);
	BYTE readbuf[2];
	char c;
	int n = 0;
	DWORD dwTicks = GetTickCount();
	while (TRUE)
	{
		if(serial.Read(&c,1)){
			readbuf[n++] = c;
			if((n == 2) && 
				(readbuf[0] == 0x57) && (readbuf[1] == para))
				break;
		
		}
		if((n >= 2) || (GetTickCount()-dwTicks > 100000)){
			serial.Close();
			AfxMessageBox("忙");
			return FALSE;

		}

	} 

	BYTE buffer[4];

	if (para != 4) {
		buffer[0] = 0;
		buffer[1] = 0;
		buffer[2] = len >> 8;
		buffer[3] = len;
	}
	else {
		buffer[0] = 0;
		buffer[1] = (BYTE)(len / 0x8000);
		buffer[2] = (BYTE)(len % 0x8000 >> 8);
		buffer[3] = (BYTE)(len % 0x8000);
	}
	serial.Write(buffer,4);
#if 0
	while (TRUE)
	{
		if(serial.Read(&c,1)){
			readbuf[n++] = c;
			if((n == 2) && 
			(readbuf[0] == 0x57) && (readbuf[1] == para)
			)
			break;
		
		}
		if((n >= 2) || (GetTickCount()-dwTicks >2000)){
			serial.Close();
			AfxMessageBox("无回应");
			return FALSE;
		}
	}
#endif
	Sleep(1);
	unsigned short crc = 0;
	char sendc;
	while(file.Read(&sendc,1)){
		crc += (BYTE)sendc;
		serial.Write(&sendc,1);
	}
	
	BYTE crcbuf[2];
	crcbuf[0] = (BYTE)(crc>>8);
	crcbuf[1] = (BYTE)(crc);

	serial.Write(&crcbuf,2);
	
	BYTE rec = (para&0x7f) | (~para &0x80);
	n = 0;
	dwTicks = GetTickCount();
	while (TRUE)
	{
		if(serial.Read(&c,1)){
			readbuf[n++] = c;

			if((n == 2) && 
			(readbuf[0] == 0x57) && (readbuf[1] == rec))
				break;
		}
		if((n >= 2) || (GetTickCount()-dwTicks > 5000)){
			serial.Close();
			AfxMessageBox("下载失败");
			return FALSE;

		}

	}
	serial.Close();
	AfxMessageBox("下载成功");
	return TRUE;
}

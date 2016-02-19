// WriteFeeView.cpp : implementation of the CWriteFeeView class
//

#include "stdafx.h"
#include "WriteFee.h"

#include "WriteFeeDoc.h"
#include "WriteFeeView.h"
#include "DownFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWriteFeeView

IMPLEMENT_DYNCREATE(CWriteFeeView, CListView )

BEGIN_MESSAGE_MAP(CWriteFeeView, CListView )
	//{{AFX_MSG_MAP(CWriteFeeView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView ::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView ::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView ::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWriteFeeView construction/destruction

CWriteFeeView::CWriteFeeView()
{
	// TODO: add construction code here

}

CWriteFeeView::~CWriteFeeView()
{
}

BOOL CWriteFeeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView ::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWriteFeeView drawing

void CWriteFeeView::OnDraw(CDC* pDC)
{
	CWriteFeeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CWriteFeeView printing

BOOL CWriteFeeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWriteFeeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWriteFeeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWriteFeeView diagnostics

#ifdef _DEBUG
void CWriteFeeView::AssertValid() const
{
	CListView ::AssertValid();
}

void CWriteFeeView::Dump(CDumpContext& dc) const
{
	CListView ::Dump(dc);
}

CWriteFeeDoc* CWriteFeeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWriteFeeDoc)));
	return (CWriteFeeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWriteFeeView message handlers

void CWriteFeeView::OnInitialUpdate() 
{
	CListView ::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}

int CWriteFeeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView ::OnCreate(lpCreateStruct) == -1)
		return -1;
	CListCtrl &List = GetListCtrl();
	List.SetTextColor(RGB(-1,-1,-1));
	List.SetTextBkColor(RGB(60,128,128));
	List.SetBkColor(RGB(60,128,128));
	List.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	List.InsertColumn(0,"类型",LVCFMT_LEFT,100);
	List.InsertColumn(1,"文件",LVCFMT_LEFT,500);
	List.ModifyStyle(GetStyle() & LVS_TYPEMASK, 0);
	List.ModifyStyle(0, LVS_REPORT);
	List.InsertItem(0,"参数");
	List.InsertItem(1,"主费率");
	List.InsertItem(2,"备费率");
	List.InsertItem(3,"汉字库");
	List.InsertItem(4,"程序");

	
	return 0;
}

void CWriteFeeView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CListCtrl &List = GetListCtrl();
	int row = List.HitTest(point,NULL);
	if(row <0 || row >4)
		return;
	CDownFile dlg;
	dlg.m_param = row+1;
	if(dlg.DoModal()==IDOK){

		List.SetItemText(row,1,dlg.m_strFilePath);
		switch(row)
		{
	
		case 0:
			m_strParam = dlg.m_strFilePath;
			break;
		case 1:
			m_strFee = dlg.m_strFilePath;
			break;
		case 2:
			m_strSFee = dlg.m_strFilePath;
			break;
		case 3:
			m_strFont = dlg.m_strFilePath;
			break;
		case 4:
			m_strProg = dlg.m_strFilePath;
			break;

		}
	}
	


	CListView ::OnLButtonDblClk(nFlags, point);
}

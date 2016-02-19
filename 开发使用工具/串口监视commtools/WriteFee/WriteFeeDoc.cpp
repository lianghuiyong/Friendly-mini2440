// WriteFeeDoc.cpp : implementation of the CWriteFeeDoc class
//

#include "stdafx.h"
#include "WriteFee.h"

#include "WriteFeeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWriteFeeDoc

IMPLEMENT_DYNCREATE(CWriteFeeDoc, CDocument)

BEGIN_MESSAGE_MAP(CWriteFeeDoc, CDocument)
	//{{AFX_MSG_MAP(CWriteFeeDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWriteFeeDoc construction/destruction

CWriteFeeDoc::CWriteFeeDoc()
{
	// TODO: add one-time construction code here

}

CWriteFeeDoc::~CWriteFeeDoc()
{
}

BOOL CWriteFeeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWriteFeeDoc serialization

void CWriteFeeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWriteFeeDoc diagnostics

#ifdef _DEBUG
void CWriteFeeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWriteFeeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWriteFeeDoc commands

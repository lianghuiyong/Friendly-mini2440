// WriteFeeView.h : interface of the CWriteFeeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITEFEEVIEW_H__E24FAE2C_C40D_11D6_AE74_5254AB3695BC__INCLUDED_)
#define AFX_WRITEFEEVIEW_H__E24FAE2C_C40D_11D6_AE74_5254AB3695BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "WriteFeeDoc.h"
#include "MainFrm.h"

class CWriteFeeView : public CListView 
{
protected: // create from serialization only
	CWriteFeeView();
	DECLARE_DYNCREATE(CWriteFeeView)

// Attributes
public:
	CWriteFeeDoc* GetDocument();

	CString m_strParam;
	CString m_strFee;
	CString m_strSFee;
	CString m_strProg;
	CString m_strFont;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWriteFeeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWriteFeeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWriteFeeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WriteFeeView.cpp
inline CWriteFeeDoc* CWriteFeeView::GetDocument()
   { return (CWriteFeeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRITEFEEVIEW_H__E24FAE2C_C40D_11D6_AE74_5254AB3695BC__INCLUDED_)

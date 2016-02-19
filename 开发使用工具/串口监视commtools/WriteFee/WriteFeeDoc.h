// WriteFeeDoc.h : interface of the CWriteFeeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITEFEEDOC_H__E24FAE2A_C40D_11D6_AE74_5254AB3695BC__INCLUDED_)
#define AFX_WRITEFEEDOC_H__E24FAE2A_C40D_11D6_AE74_5254AB3695BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWriteFeeDoc : public CDocument
{
protected: // create from serialization only
	CWriteFeeDoc();
	DECLARE_DYNCREATE(CWriteFeeDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWriteFeeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWriteFeeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWriteFeeDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRITEFEEDOC_H__E24FAE2A_C40D_11D6_AE74_5254AB3695BC__INCLUDED_)

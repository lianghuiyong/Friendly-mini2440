#if !defined(AFX_DOWNFILE_H__E24FAE35_C40D_11D6_AE74_5254AB3695BC__INCLUDED_)
#define AFX_DOWNFILE_H__E24FAE35_C40D_11D6_AE74_5254AB3695BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownFile dialog

class CDownFile : public CDialog
{
// Construction
public:
	BYTE m_param;
	CDownFile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDownFile)
	enum { IDD = IDD_DIALOG1 };
	CString	m_strFilePath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDownFile)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNFILE_H__E24FAE35_C40D_11D6_AE74_5254AB3695BC__INCLUDED_)

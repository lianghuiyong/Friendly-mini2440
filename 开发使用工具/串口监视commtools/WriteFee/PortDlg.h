#if !defined(AFX_PORTDLG_H__F8A54341_C4A0_11D6_AE74_5254AB3695BC__INCLUDED_)
#define AFX_PORTDLG_H__F8A54341_C4A0_11D6_AE74_5254AB3695BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPortDlg dialog

class CPortDlg : public CDialog
{
// Construction
public:
	CPortDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPortDlg)
	enum { IDD = IDD_DIALOG2 };
	int		m_nPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPortDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTDLG_H__F8A54341_C4A0_11D6_AE74_5254AB3695BC__INCLUDED_)

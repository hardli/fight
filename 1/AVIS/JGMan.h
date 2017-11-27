#if !defined(AFX_JGMAN_H__3B204CC6_2F96_4259_9ABE_0FD0198975B3__INCLUDED_)
#define AFX_JGMAN_H__3B204CC6_2F96_4259_9ABE_0FD0198975B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JGMan.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJGMan dialog

class CJGMan : public CDialog
{
// Construction
public:
	CJGMan(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJGMan)
	enum { IDD = IDD_LSMAN };
	CIPAddressCtrl	m_IpCtrl1;
	CIPAddressCtrl	m_IpCtrl0;
	UINT	m_Port0;
	UINT	m_Port1;
	int		m_AqcMode;
	DWORD dwIP0;
	DWORD dwIP1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJGMan)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJGMan)
	afx_msg void OnJg0conn0();
	afx_msg void OnJg0conn1();
	virtual BOOL OnInitDialog();
	afx_msg void OnComfirm();
	afx_msg void OnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JGMAN_H__3B204CC6_2F96_4259_9ABE_0FD0198975B3__INCLUDED_)

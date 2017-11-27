#if !defined(AFX_ALGOSET_H__E11DC0C7_5AF9_42A2_B60B_94F5D03864AE__INCLUDED_)
#define AFX_ALGOSET_H__E11DC0C7_5AF9_42A2_B60B_94F5D03864AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgoSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlgoSet dialog

class CAlgoSet : public CDialog
{
// Construction
public:
	CAlgoSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgoSet)
	enum { IDD = IDD_ALGSET };
	int		m_A3out;
	int		m_AxReport;
	UINT	m_EmptyFrCnt;
	UINT	m_GratHt;
	int		m_GratPos;
	UINT	m_HiFrCnt;
	UINT	m_IsLanHt;
	UINT	m_IsLanWd;
	UINT	m_JGHeight;
	UINT	m_LaneWd;
	UINT	m_LvFrCnt;
	UINT	m_LwFrCnt;
	UINT	m_MaxHt;
	UINT	m_StartX;
	UINT	m_ZeroPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgoSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlgoSet)
	afx_msg void OnAlgset();
	afx_msg void OnAlgexit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGOSET_H__E11DC0C7_5AF9_42A2_B60B_94F5D03864AE__INCLUDED_)

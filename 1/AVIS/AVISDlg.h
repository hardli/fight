// AVISDlg.h : header file
//

#if !defined(AFX_AVISDLG_H__B55CB19A_5D05_404E_947E_E181E0B539B5__INCLUDED_)
#define AFX_AVISDLG_H__B55CB19A_5D05_404E_947E_E181E0B539B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAVISDlg dialog
#include "Avi.h"

class CAVISDlg : public CDialog
{
// Construction
public:
	CAVISDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAVISDlg)
	enum { IDD = IDD_AVIS_DIALOG };
	CStatic	m_PlotName;
	CStatic	m_PlotIm;
	CListCtrl	m_AlgoList;
	CRichEditCtrl	m_LogCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAVISDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAVISDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEnplot();
	afx_msg void OnSetalgo();
	afx_msg void OnClalgo();
	afx_msg void OnDisplot();
	afx_msg void OnFilemode();
	afx_msg void OnSoftmode();
	afx_msg void OnStalgo();
	afx_msg void OnTruemode();
	
	afx_msg void OnLaserman();
	afx_msg void OnImpwv();
	afx_msg void OnCamSet();
	afx_msg void OnSimGetCard();
	afx_msg void OnEnsvdb();
	afx_msg void OnDissvdb();
	afx_msg void OnDissvwv();
	afx_msg void OnEnsvwv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void OnInitAlgoList(void);
	afx_msg void OnWriteLog(CString Tips);
	void AddPitcure(char *pFilePath,char *pFileName)  ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVISDLG_H__B55CB19A_5D05_404E_947E_E181E0B539B5__INCLUDED_)

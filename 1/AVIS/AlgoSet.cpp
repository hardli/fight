// CAlgoSet.cpp : implementation file
//

#include "stdafx.h"
#include "AVIS.h"
#include "AlgoSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlgoSet dialog


CAlgoSet::CAlgoSet(CWnd* pParent /*=NULL*/)
	: CDialog(CAlgoSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlgoSet)
	m_A3out = -1;
	m_AxReport = -1;
	m_EmptyFrCnt = 0;
	m_GratHt = 0;
	m_GratPos = -1;
	m_HiFrCnt = 0;
	m_IsLanHt = 0;
	m_IsLanWd = 0;
	m_JGHeight = 0;
	m_LaneWd = 0;
	m_LvFrCnt = 0;
	m_LwFrCnt = 0;
	m_MaxHt = 0;
	m_StartX = 0;
	m_ZeroPos = 0;
	//}}AFX_DATA_INIT
}


void CAlgoSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgoSet)
	DDX_CBIndex(pDX, IDC_A3OUT, m_A3out);
	DDX_CBIndex(pDX, IDC_AXREPORT, m_AxReport);
	DDX_Text(pDX, IDC_EMPTYFR, m_EmptyFrCnt);
	DDX_Text(pDX, IDC_GRATHT, m_GratHt);
	DDX_CBIndex(pDX, IDC_GRATPOS, m_GratPos);
	DDX_Text(pDX, IDC_HIFRAM, m_HiFrCnt);
	DDX_Text(pDX, IDC_ISLANHT, m_IsLanHt);
	DDX_Text(pDX, IDC_ISLANWD, m_IsLanWd);
	DDX_Text(pDX, IDC_JGHEIGHT, m_JGHeight);
	DDX_Text(pDX, IDC_LANEWD, m_LaneWd);
	DDX_Text(pDX, IDC_LVFRAM, m_LvFrCnt);
	DDX_Text(pDX, IDC_LWFRAM, m_LwFrCnt);
	DDX_Text(pDX, IDC_MAXHT, m_MaxHt);
	DDX_Text(pDX, IDC_STARTX, m_StartX);
	DDX_Text(pDX, IDC_ZEROPOS, m_ZeroPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlgoSet, CDialog)
	//{{AFX_MSG_MAP(CAlgoSet)
	ON_BN_CLICKED(IDC_ALGSET, OnAlgset)
	ON_BN_CLICKED(IDC_ALGEXIT, OnAlgexit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlgoSet message handlers

void CAlgoSet::OnAlgset() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	OnOK();
}

void CAlgoSet::OnAlgexit() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CAlgoSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

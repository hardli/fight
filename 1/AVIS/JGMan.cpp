// JGMan.cpp : implementation file
//

#include "stdafx.h"
#include "AVIS.h"
#include "JGMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJGMan dialog


CJGMan::CJGMan(CWnd* pParent /*=NULL*/)
	: CDialog(CJGMan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJGMan)
	m_Port0 = 0;
	m_Port1 = 0;
	m_AqcMode = -1;
	dwIP0=0;
	dwIP1=0;
	//}}AFX_DATA_INIT
}


void CJGMan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJGMan)
	DDX_Control(pDX, IDC_IP1, m_IpCtrl1);
	DDX_Control(pDX, IDC_IP0, m_IpCtrl0);
	DDX_Text(pDX, IDC_PORT0, m_Port0);
	DDX_Text(pDX, IDC_PORT1, m_Port1);
	DDX_Radio(pDX, IDC_RADIO, m_AqcMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJGMan, CDialog)
	//{{AFX_MSG_MAP(CJGMan)
	ON_BN_CLICKED(IDC_COMFIRM, OnComfirm)
	ON_BN_CLICKED(IDC_Exit, OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJGMan message handlers

void CJGMan::OnJg0conn0() 
{
	// TODO: Add your control notification handler code here
	
}

void CJGMan::OnJg0conn1() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CJGMan::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_Port0<1||m_Port1<1||dwIP0<1||dwIP1<1)
	{
		m_Port0=2110;
		m_Port1=2112;
		m_IpCtrl0.SetAddress(192,168,0,2);
		m_IpCtrl1.SetAddress(192,168,0,4);
		m_AqcMode=0;
	}
	else
	{
		m_IpCtrl0.SetAddress(dwIP0);
		m_IpCtrl1.SetAddress(dwIP1);
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJGMan::OnComfirm() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_IpCtrl0.GetAddress(dwIP0);
	m_IpCtrl1.GetAddress(dwIP1);
	OnOK();
}

void CJGMan::OnExit() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

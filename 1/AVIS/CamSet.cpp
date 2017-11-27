// CamSet.cpp : implementation file
//

#include "stdafx.h"
#include "AVIS.h"
#include "CamSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCamSet dialog


CCamSet::CCamSet(CWnd* pParent /*=NULL*/)
	: CDialog(CCamSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCamSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCamSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCamSet, CDialog)
	//{{AFX_MSG_MAP(CCamSet)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamSet message handlers

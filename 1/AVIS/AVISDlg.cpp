// AVISDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AVIS.h"
#include "AVISDlg.h"
#include "Avi_Comm.h"
#include "Avi.h"
#include "AlgoSet.h"
#include "JGMan.h"
#include "CamSet.h"
#include <afxsock.h> 

//#pragma comment(lib,"Avi.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAVISDlg dialog

CAVISDlg::CAVISDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAVISDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAVISDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAVISDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAVISDlg)
	DDX_Control(pDX, IDC_PlotName, m_PlotName);
	DDX_Control(pDX, IDC_PLOTIM, m_PlotIm);
	DDX_Control(pDX, IDC_AlgoList, m_AlgoList);
	DDX_Control(pDX, IDC_LOG, m_LogCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAVISDlg, CDialog)
	//{{AFX_MSG_MAP(CAVISDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_ENPLOT, OnEnplot)
	ON_COMMAND(ID_SETALGO, OnSetalgo)
	ON_COMMAND(IDC_CLALGO, OnClalgo)
	ON_COMMAND(IDC_DISPLOT, OnDisplot)
	ON_COMMAND(IDC_FILEMODE, OnFilemode)
	ON_COMMAND(IDC_SOFTMODE, OnSoftmode)
	ON_COMMAND(IDC_STALGO, OnStalgo)
	ON_COMMAND(IDC_TRUEMODE, OnTruemode)
	ON_COMMAND(IDC_LASERMAN, OnLaserman)
	ON_COMMAND(IDC_IMPWV, OnImpwv)
	ON_COMMAND(ID_CAMMAN, OnCamSet)
	ON_COMMAND(ID_SimGetCard, OnSimGetCard)
	ON_COMMAND(ID_ENSVDB, OnEnsvdb)
	ON_COMMAND(ID_DISSVDB, OnDissvdb)
	ON_COMMAND(ID_DISSVWV, OnDissvwv)
	ON_COMMAND(ID_ENSVWV, OnEnsvwv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAVISDlg message handlers
int	CALLBACK AlgoVehOut(VehSet Veh)
{
	int i=0;
	int RowIndx=0;
	int CowIndx=1;
	CString StrTmp="";
	CString sTmp="";
	CString  str="";
	BYTE   Cartype;

	CAVISDlg *pMainHnd=(CAVISDlg *)(AfxGetApp()->GetMainWnd());

	RowIndx=pMainHnd->m_AlgoList.GetItemCount();

	StrTmp.Format("%d",Veh.VehId);
	pMainHnd->m_AlgoList.InsertItem(RowIndx,StrTmp);//�����

	if(Veh.VPattern>=1 && Veh.VPattern<=4)
	{
		str="��";
		Cartype=Veh.VPattern;

	}
	else if(Veh.VPattern>=5 && Veh.VPattern<=9)
	{
		str="��";
		Cartype=Veh.VPattern-4;

	}
	else if(Veh.VPattern==10 || Veh.VPattern==11)
	{
	  str="��װ��";
	  Cartype=Veh.VPattern-9;
	}
	else
		;

	if(Veh.VSpeCode==201)
		StrTmp="����";
	else
	{
		StrTmp.Format("%d",Cartype);
		StrTmp=str+StrTmp;
	}
	//д����
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//����
	
	StrTmp.Format("%d",Veh.VDir);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//����

	StrTmp.Format("%d",Veh.AxleNum);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//����

	StrTmp="";
	for (i=0;i<Veh.AxleNum;i++)
	{
		sTmp.Format("%d",Veh.AxleType[i]);
		StrTmp+=sTmp;
	}
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//ԭʼ����

	StrTmp.Format("%d",Veh.TranAxleType);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//ͨ������


	StrTmp.Format("%s",Veh.Plate);
	pMainHnd->OnWriteLog("����"+StrTmp);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//����

	if (Veh.VoutFlag==1)
	{
		StrTmp="ǿ�Ƴɹ�";
	}
	else if (Veh.VoutFlag==2)
	{
		StrTmp="ǿ��ʧ��";
	}
	else
	{
		StrTmp="������β";
	}
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//��������


	SYSTEMTIME l_stime = {0};
	GetLocalTime(&l_stime);//��õ�ǰ����ʱ��

	StrTmp.Format("%4d/%02d/%02d",l_stime.wYear,l_stime.wMonth,l_stime.wDay);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//�������

	StrTmp.Format("%02d:%02d:%02d",l_stime.wHour,l_stime.wMinute,l_stime.wSecond);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//��ʾϵͳʱ��


	StrTmp.Format("%d",Veh.VJudCode);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//�ж�׼��

	StrTmp.Format("%d",Veh.VAdjCode);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//����׼��

	StrTmp.Format("%d",Veh.VSpeCode);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//ר��

	StrTmp.Format("%d",Veh.VDpHt);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//���̸�

	StrTmp.Format("%d",Veh.VehHeadHt);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//��ͷ��

	StrTmp.Format("%d",Veh.VehHeight);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//����

	StrTmp.Format("%d",Veh.Hopper);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//����

	StrTmp.Format("%d",Veh.HeadJump);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//ͷ����


	StrTmp.Format("%d",Veh.TailJump);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//β����

	StrTmp.Format("%d",Veh.Cargo);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//�ػ�

	StrTmp.Format("%d",Veh.Container);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//����

	StrTmp.Format("%d",Veh.TruckTail);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//����β

	StrTmp.Format("%d",Veh.CastGrd);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//����

	StrTmp.Format("%d",Veh.FillWin);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//��ճ���

	StrTmp.Format("%d",Veh.GlassWin);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//��������

	StrTmp.Format("%d",Veh.SideUiform);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//����һ��

	StrTmp="";
	StrTmp.Format("%s",Veh.FoldName);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//��ע


	pMainHnd->m_AlgoList.EnsureVisible(RowIndx,false);


	return 0;
}

int CALLBACK ShowPlotIm(char *pFilePath,char *pFileName)
{
	CAVISDlg *pMainHnd=(CAVISDlg *)(AfxGetApp()->GetMainWnd());
	pMainHnd->AddPitcure(pFilePath,pFileName);
	//CRect r; 
	//HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
	//pFilePath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE);
	//if( hbmp == NULL )
	//	return FALSE;

	//if (hbmp!=NULL)
	//{
	//	//pMainHnd->m_PlotIm.SetBitmap(hbmp);
	//	pMainHnd->m_PlotName.SetWindowText(pFileName);
	//}
	//CBitmap   m_bmp;
	//m_bmp.Attach( hbmp );
	//

 //

 //    DIBSECTION ds;
 //    BITMAPINFOHEADER &bminfo = ds.dsBmih;
 //    m_bmp.GetObject( sizeof(ds), &ds );
 //    int cx=bminfo.biWidth;   //�õ�ͼ����
 //    int cy=bminfo.biHeight; //�õ�ͼ��߶�
 //    /////////////////// ////////////////////////////////
 //    //�õ���ͼ��Ŀ�Ⱥ͸߶Ⱥ�,���ǾͿ��Զ�ͼ���С������Ӧ,
 //    //�������ؼ��Ĵ�С,����������ʾһ��ͼƬ
 //    ////////////////////////////////////////////////////
 //    CRect rect;
 //    pMainHnd->GetDlgItem(IDC_PLOTIM)->GetWindowRect(&rect);
 //    pMainHnd->ScreenToClient(&rect);
 //    pMainHnd->GetDlgItem(IDC_PLOTIM)->MoveWindow(rect.left,rect.top,cx,cy,true);//������С

	return ERR_SUCC;
}



void CAVISDlg::AddPitcure(char *pFilePath,char *pFileName)  
{   
	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		pFilePath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE);
	if( hbmp == NULL )
		return ;

	if (hbmp!=NULL)
	{
		//pMainHnd->m_PlotIm.SetBitmap(hbmp);
		m_PlotName.SetWindowText(pFileName);
	}
	CBitmap   m_bmp;
	m_bmp.Attach( hbmp );

	  
	//����ָ��λͼ��Դ BmpͼƬID    
     
	//��ȡ�Ի����ϵľ�� ͼƬ�ؼ�ID    
	CStatic *p=(CStatic *)GetDlgItem(IDC_PLOTIM);      
	//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ     
	p->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);     
	//��ͼƬ���õ�Picture�ؼ���    
	p->SetBitmap(m_bmp);     

	BITMAP bmpInfo;   
	m_bmp.GetBitmap(&bmpInfo);   
	CDC dcMemory;   
	CDC* pDC=GetDlgItem(IDC_PLOTIM)->GetDC();   
	dcMemory.CreateCompatibleDC(pDC);   
	CBitmap* pOldBitmap = dcMemory.SelectObject(&m_bmp);   
	CRect rect;   
	m_PlotIm.GetClientRect(&rect);  
	int nX = rect.left + (rect.Width() - bmpInfo.bmWidth) / 2;   
	int nY = rect.top + (rect.Height() - bmpInfo.bmHeight) / 2;   
	//pDC->BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY);    
	pDC->SetStretchBltMode(COLORONCOLOR);   
	pDC->StretchBlt(0, 0,rect.Width(),rect.Height(),&dcMemory,0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,SRCCOPY);   
	dcMemory.SelectObject(pOldBitmap);   
	ReleaseDC(pDC);   
}  


BOOL CAVISDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	Avi_Init();
	OnInitAlgoList();
	RegMainVReport((PFReportVh)&AlgoVehOut);
	RegMainPlotIm((PFPlotIm)&ShowPlotIm);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAVISDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAVISDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAVISDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAVISDlg::OnEnplot() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	SystemSet Sys;
	memset(&Sys,0,sizeof(SystemSet));
	Sys=GetMainSysSet();
	Sys.PlotImg=1;
	nCode=SetMainSysSet(Sys);
	SaveSysPara(NULL);
	if (nCode!=ERR_SUCC)
	{
		Logtxt.Format("������ͼʧ��,������[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="������ͼ�ɹ�";
		OnWriteLog(Logtxt);
	}
}

void CAVISDlg::OnDisplot() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	SystemSet Sys;
	memset(&Sys,0,sizeof(SystemSet));
	Sys=GetMainSysSet();
	Sys.PlotImg=0;
	nCode=SetMainSysSet(Sys);
	SaveSysPara(NULL);
	if (nCode!=ERR_SUCC)
	{
		Logtxt.Format("�رջ�ͼʧ��,������[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="�رջ�ͼ�ɹ�";
		OnWriteLog(Logtxt);
	}

}


void CAVISDlg::OnFilemode() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	SystemSet Sys;
	memset(&Sys,0,sizeof(SystemSet));
	Sys=GetMainSysSet();
	Sys.RunMode=FILESIN;
	nCode=SetMainSysSet(Sys);
	SaveSysPara(NULL);
	if (nCode!=ERR_SUCC)
	{
		Logtxt.Format("�����ļ�����ʧ��,������[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="�����ļ����гɹ�";
		OnWriteLog(Logtxt);
		nCode=StartAqcWave();
		if (nCode!=ERR_SUCC)
		{
			Logtxt.Format("�л��ļ�����ʧ��,������[%d]",nCode);
			OnWriteLog(Logtxt);			
		}
		else
		{
			Logtxt="�л��ļ����гɹ�";
			OnWriteLog(Logtxt);
		}
	}
}

void CAVISDlg::OnSoftmode() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	SystemSet Sys;
	memset(&Sys,0,sizeof(SystemSet));
	Sys=GetMainSysSet();
	Sys.RunMode=SOFTSIM;
	nCode=SetMainSysSet(Sys);
	SaveSysPara(NULL);
	if (nCode!=ERR_SUCC)
	{
		Logtxt.Format("����ģ������ʧ��,������[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="����ģ�����гɹ�";
		OnWriteLog(Logtxt);
		nCode=StartAqcWave();
		if (nCode!=ERR_SUCC)
		{
			Logtxt.Format("�л�ģ������ʧ��,������[%d]",nCode);
			OnWriteLog(Logtxt);			
		}
		else
		{
			Logtxt="�л�ģ�����гɹ�";
			OnWriteLog(Logtxt);
		}
	}	
}

void CAVISDlg::OnTruemode() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	SystemSet Sys;
	memset(&Sys,0,sizeof(SystemSet));
	Sys=GetMainSysSet();
	Sys.RunMode=LASERIN;
	nCode=SetMainSysSet(Sys);
	SaveSysPara(NULL);
	if (nCode!=ERR_SUCC)
	{
		Logtxt.Format("�����ֳ�����ʧ��,������[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="�����ֳ����гɹ�";
		OnWriteLog(Logtxt);
		nCode=StartAqcWave();
		if (nCode!=ERR_SUCC)
		{
			Logtxt.Format("�л��ֳ�����ʧ��,������[%d]",nCode);
			OnWriteLog(Logtxt);			
		}
		else
		{
			Logtxt="�л��ֳ����гɹ�";
			OnWriteLog(Logtxt);
		}
	}		
}


void CAVISDlg::OnStalgo() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	nCode=StartAviAlgo();
	if (nCode!=ERR_SUCC)
	{
		Logtxt.Format("�㷨����ʧ��,������[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="�㷨�����ɹ�";
		OnWriteLog(Logtxt);
	}
}

void CAVISDlg::OnClalgo() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	nCode=ExitEngine();
	if (nCode!=ERR_SUCC)
	{
		Logtxt.Format("�㷨�ر�ʧ��,������[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="�㷨�رճɹ�";
		OnWriteLog(Logtxt);
	}
}

void CAVISDlg::OnSetalgo() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	CAlgoSet AlgoSet;
	ModelSet Mdl;
	//ģ�Ͳ���
	memset(&Mdl,0,sizeof(Mdl));
	Mdl=GetMainMdlSet();
	AlgoSet.m_JGHeight=Mdl.JGHeight;
	AlgoSet.m_ZeroPos=Mdl.ZeroPos;
	AlgoSet.m_StartX=Mdl.StartX;
	AlgoSet.m_LaneWd=Mdl.LaneWidth;
	AlgoSet.m_IsLanHt=Mdl.IsLanHt;
	AlgoSet.m_IsLanWd=Mdl.IsLanWd;
	AlgoSet.m_MaxHt=Mdl.MaxHt;
	//��������
	VOutSet Voset;
	memset(&Voset,0,sizeof(Voset));
	Voset=GetMainVoutSet();
	AlgoSet.m_EmptyFrCnt=Voset.EmFramCnt;
	AlgoSet.m_HiFrCnt=Voset.HiFramCnt;
	AlgoSet.m_LwFrCnt=Voset.LwFramCnt;
	AlgoSet.m_LvFrCnt=Voset.LvFramCnt;
	AlgoSet.m_AxReport=Voset.AxReport;
	AlgoSet.m_A3out=Voset.A3ForOut;
	AlgoSet.m_GratHt=Voset.GratHt;
	AlgoSet.m_GratPos=Voset.GratPos;
	int nResponse = AlgoSet.DoModal();
	if (nResponse == IDOK)
	{
		Mdl.JGHeight=AlgoSet.m_JGHeight;
		Mdl.ZeroPos=AlgoSet.m_ZeroPos;
		Mdl.StartX=AlgoSet.m_StartX;
		Mdl.LaneWidth=AlgoSet.m_LaneWd;
		Mdl.IsLanHt=AlgoSet.m_IsLanHt;
		Mdl.IsLanWd=AlgoSet.m_IsLanWd;
		Mdl.MaxHt=AlgoSet.m_MaxHt;
		nCode=SetMainMdlSet(Mdl);
		SaveModelPara(NULL);
		if (nCode!=ERR_SUCC)
		{
			Logtxt.Format("ģ�Ͳ���ʧ��,������[%d]",nCode);
			OnWriteLog(Logtxt);
		}
		else
		{
			Logtxt="ģ�Ͳ������óɹ�";
			OnWriteLog(Logtxt);
		}

		Voset.EmFramCnt=AlgoSet.m_EmptyFrCnt;
		Voset.HiFramCnt=AlgoSet.m_HiFrCnt;
		Voset.LwFramCnt=AlgoSet.m_LwFrCnt;
		Voset.LvFramCnt=AlgoSet.m_LvFrCnt;
		Voset.AxReport=AlgoSet.m_AxReport;
		Voset.A3ForOut=AlgoSet.m_A3out;
		Voset.GratHt=AlgoSet.m_GratHt;
		Voset.GratPos=AlgoSet.m_GratPos;
		nCode=SetMainVoutSet(Voset);
		SaveVoutPara(NULL);
		if (nCode!=ERR_SUCC)
		{
			Logtxt.Format("��������ʧ��,������[%d]",nCode);
			OnWriteLog(Logtxt);
		}
		else
		{
			Logtxt="�����������óɹ�";
			OnWriteLog(Logtxt);
		}
		nCode=ReLoadEngine();
		if (nCode!=ERR_SUCC)
		{
			Logtxt.Format("�����㷨ʧ��,������[%d]",nCode);
			OnWriteLog(Logtxt);
		}
		else
		{
			Logtxt="�����㷨����ɹ�";
			OnWriteLog(Logtxt);
		}
	}
}


void CAVISDlg::OnWriteLog(CString Tips)
{
	CString Log="";
	m_LogCtrl.SetSel(-1, -1);
	CTime tm = CTime::GetCurrentTime();
	CString strtm= tm.Format("%H:%M:%S ");
	Log=strtm+Tips+"\r\n";
	m_LogCtrl.ReplaceSel( (LPCTSTR)(Log) );
	Avi_WriteLog(Tips.GetBuffer(Tips.GetLength()));
}

void CAVISDlg::OnLaserman() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	CJGMan JGMan;
	LaserSet Ls;
	CString sIp="";
	memset(&Ls,0,sizeof(Ls));
	Ls=GetMainLaserSet();
	JGMan.m_AqcMode=Ls.AqcMode;
	JGMan.m_Port0=Ls.Port0;
	JGMan.dwIP0=ntohl(inet_addr(Ls.IP0));
	JGMan.m_Port1=Ls.Port1;
	JGMan.dwIP1=ntohl(inet_addr(Ls.IP1));
	int nResponse = JGMan.DoModal();
	if (nResponse == IDOK)
	{
		Ls.AqcMode=JGMan.m_AqcMode;
		Ls.Port0=JGMan.m_Port0;
		IN_ADDR ia;
		ia.S_un.S_addr=htonl(JGMan.dwIP0);
		sIp=inet_ntoa(ia);
		memset(Ls.IP0,0,sizeof(Ls.IP0));
		memcpy(Ls.IP0,sIp.GetBuffer(sIp.GetLength()),sIp.GetLength());
		Ls.Port1=JGMan.m_Port1;
		ia.S_un.S_addr=htonl(JGMan.dwIP1);
		sIp=inet_ntoa(ia);
		memset(Ls.IP1,0,sizeof(Ls.IP0));
		memcpy(Ls.IP1,sIp.GetBuffer(sIp.GetLength()),sIp.GetLength());
		nCode=SetMainLaserSet(Ls);
		SaveLaserPara(NULL);
		if (nCode!=ERR_SUCC)
		{
			Logtxt.Format("���ü������ʧ��,������[%d]",nCode);
			OnWriteLog(Logtxt);
		}
		else
		{
			Logtxt="���ü�������ɹ�";
			OnWriteLog(Logtxt);
		}
	}
}

void CAVISDlg::OnImpwv() 
{
	// TODO: Add your command handler code here
	CString m_path = _T("");  
	CString Logtxt = _T("");  
	TCHAR servPath[MAX_PATH];//��������ļ���·��  
	BROWSEINFO bi;  
	LPITEMIDLIST pidl;  
	bi.hwndOwner = this->m_hWnd;  
	bi.pidlRoot = NULL;  
	bi.pszDisplayName = servPath;  
	bi.lpszTitle = _T("ѡ�����ļ�·��");  
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  
	bi.lpfn = NULL;  
	bi.lParam = NULL;  
	bi.iImage = NULL;  
	if((pidl = SHBrowseForFolder(&bi)) != NULL)  
	{  
		if(SUCCEEDED(SHGetPathFromIDList(pidl, servPath))) //�õ��ļ��е�ȫ·������Ҫ�Ļ���ֻ�ñ��ļ�����  
		{  
		  m_path.Format("%s",servPath);
		  ImportWvPath(m_path);
		  Logtxt="ѡ����·��:"+m_path;
		  OnWriteLog(Logtxt);	  
		}  
	}	
}


void CAVISDlg::OnInitAlgoList()
{
	int nIndex=0;
	m_AlgoList.InsertColumn(nIndex++,_T("�����"),LVCFMT_CENTER,80,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("����"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("����"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("����"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("ԭʼ����"),LVCFMT_CENTER,80,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("ͨ������"),LVCFMT_CENTER,80,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("���ƺ���"),LVCFMT_CENTER,120,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("��������"),LVCFMT_CENTER,80,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("�������"),LVCFMT_CENTER,100,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("���ʱ��"),LVCFMT_CENTER,100,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("�ж���"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("������"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("ר����"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("���̸�"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("��ͷ��"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("����"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("����"),LVCFMT_CENTER,50,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("ͷ����"),LVCFMT_CENTER,70,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("β����"),LVCFMT_CENTER,70,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("�ػ�"),LVCFMT_CENTER,50,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("����"),LVCFMT_CENTER,50,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("����β"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("����"),LVCFMT_CENTER,50,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("��մ�"),LVCFMT_CENTER,80,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("������"),LVCFMT_CENTER,80,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("����һ��"),LVCFMT_CENTER,80,-1);

	m_AlgoList.InsertColumn(nIndex++,_T("��ע"),LVCFMT_CENTER,120,-1);
	m_AlgoList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_SHOWSELALWAYS );
}

void CAVISDlg::OnCamSet() 
{
	// TODO: Add your command handler code here
	
	int nCode=ERR_FAIL;
	CString Logtxt="";
	CCamSet CamSet;

	int nResponse = CamSet.DoModal();
	if (nResponse == IDOK)
	{
			Logtxt.Format("CamSet,��[%d]",nCode);
			OnWriteLog(Logtxt);
	}

}

void CAVISDlg::OnSimGetCard() 
{
	// TODO: Add your command handler code here
		
	int a=0;
	int b=0;
	Model_Getinfo(&a,&b);
}

void CAVISDlg::OnEnsvdb() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	SystemSet Sys;
	memset(&Sys,0,sizeof(SystemSet));
	Sys=GetMainSysSet();
	Sys.SvDB=1;
	nCode=SetMainSysSet(Sys);
	SaveSysPara(NULL);
	if (nCode!=ERR_SUCC)
	{
		Logtxt.Format("�������ʧ��,������[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="�������ɹ�";
		OnWriteLog(Logtxt);
	}
		
}

void CAVISDlg::OnDissvdb() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	SystemSet Sys;
	memset(&Sys,0,sizeof(SystemSet));
	Sys=GetMainSysSet();
	Sys.SvDB=0;
	nCode=SetMainSysSet(Sys);
	SaveSysPara(NULL);
	if (nCode!=ERR_SUCC)
	{
		Logtxt.Format("�رմ��ʧ��,������[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="�رմ��ɹ�";
		OnWriteLog(Logtxt);
	}	
}

void CAVISDlg::OnDissvwv() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	SystemSet Sys;
	memset(&Sys,0,sizeof(SystemSet));
	Sys=GetMainSysSet();
	Sys.SvWv=0;
	nCode=SetMainSysSet(Sys);	
	nCode=PreSaveWave();
	SaveSysPara(NULL);
	if (nCode!=ERR_SUCC)
	{
		Logtxt.Format("�رմ洢����ʧ��,������[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="�رմ洢���γɹ�";
		OnWriteLog(Logtxt);
	}		
}

void CAVISDlg::OnEnsvwv() 
{
	// TODO: Add your command handler code here
	int nCode=ERR_FAIL;
	CString Logtxt="";
	SystemSet Sys;
	memset(&Sys,0,sizeof(SystemSet));
	Sys=GetMainSysSet();
	Sys.SvWv=1;
	nCode=SetMainSysSet(Sys);
	nCode=PreSaveWave();
	SaveSysPara(NULL);
	if (nCode!=ERR_SUCC)
	{
		Logtxt.Format("�����洢����ʧ��,������[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="�����洢���γɹ�";
		OnWriteLog(Logtxt);
	}			
}

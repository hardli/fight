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
	pMainHnd->m_AlgoList.InsertItem(RowIndx,StrTmp);//车序号

	if(Veh.VPattern>=1 && Veh.VPattern<=4)
	{
		str="客";
		Cartype=Veh.VPattern;

	}
	else if(Veh.VPattern>=5 && Veh.VPattern<=9)
	{
		str="货";
		Cartype=Veh.VPattern-4;

	}
	else if(Veh.VPattern==10 || Veh.VPattern==11)
	{
	  str="集装箱";
	  Cartype=Veh.VPattern-9;
	}
	else
		;

	if(Veh.VSpeCode==201)
		StrTmp="行人";
	else
	{
		StrTmp.Format("%d",Cartype);
		StrTmp=str+StrTmp;
	}
	//写入表格
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//车型
	
	StrTmp.Format("%d",Veh.VDir);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//方向

	StrTmp.Format("%d",Veh.AxleNum);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//轴数

	StrTmp="";
	for (i=0;i<Veh.AxleNum;i++)
	{
		sTmp.Format("%d",Veh.AxleType[i]);
		StrTmp+=sTmp;
	}
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//原始轴型

	StrTmp.Format("%d",Veh.TranAxleType);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//通用轴型


	StrTmp.Format("%s",Veh.Plate);
	pMainHnd->OnWriteLog("车牌"+StrTmp);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//车牌

	if (Veh.VoutFlag==1)
	{
		StrTmp="强制成功";
	}
	else if (Veh.VoutFlag==2)
	{
		StrTmp="强制失败";
	}
	else
	{
		StrTmp="正常收尾";
	}
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//出车类型


	SYSTEMTIME l_stime = {0};
	GetLocalTime(&l_stime);//获得当前本地时间

	StrTmp.Format("%4d/%02d/%02d",l_stime.wYear,l_stime.wMonth,l_stime.wDay);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//检测日期

	StrTmp.Format("%02d:%02d:%02d",l_stime.wHour,l_stime.wMinute,l_stime.wSecond);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//显示系统时间


	StrTmp.Format("%d",Veh.VJudCode);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//判定准则

	StrTmp.Format("%d",Veh.VAdjCode);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//调整准则

	StrTmp.Format("%d",Veh.VSpeCode);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//专车

	StrTmp.Format("%d",Veh.VDpHt);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//底盘高

	StrTmp.Format("%d",Veh.VehHeadHt);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//车头高

	StrTmp.Format("%d",Veh.VehHeight);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//车高

	StrTmp.Format("%d",Veh.Hopper);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//车斗

	StrTmp.Format("%d",Veh.HeadJump);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//头跳高


	StrTmp.Format("%d",Veh.TailJump);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//尾跳高

	StrTmp.Format("%d",Veh.Cargo);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//载货

	StrTmp.Format("%d",Veh.Container);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//货箱

	StrTmp.Format("%d",Veh.TruckTail);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//货车尾

	StrTmp.Format("%d",Veh.CastGrd);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//掷地

	StrTmp.Format("%d",Veh.FillWin);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//封闭车窗

	StrTmp.Format("%d",Veh.GlassWin);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//玻璃车窗

	StrTmp.Format("%d",Veh.SideUiform);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//侧面一致

	StrTmp="";
	StrTmp.Format("%s",Veh.FoldName);
	pMainHnd->m_AlgoList.SetItemText(RowIndx,CowIndx++,StrTmp);//备注


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
 //    int cx=bminfo.biWidth;   //得到图像宽度
 //    int cy=bminfo.biHeight; //得到图像高度
 //    /////////////////// ////////////////////////////////
 //    //得到了图像的宽度和高度后,我们就可以对图像大小进行适应,
 //    //即调整控件的大小,让它正好显示一张图片
 //    ////////////////////////////////////////////////////
 //    CRect rect;
 //    pMainHnd->GetDlgItem(IDC_PLOTIM)->GetWindowRect(&rect);
 //    pMainHnd->ScreenToClient(&rect);
 //    pMainHnd->GetDlgItem(IDC_PLOTIM)->MoveWindow(rect.left,rect.top,cx,cy,true);//调整大小

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

	  
	//加载指定位图资源 Bmp图片ID    
     
	//获取对话框上的句柄 图片控件ID    
	CStatic *p=(CStatic *)GetDlgItem(IDC_PLOTIM);      
	//设置静态控件窗口风格为位图居中显示     
	p->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);     
	//将图片设置到Picture控件上    
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
		Logtxt.Format("开启绘图失败,错误码[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="开启绘图成功";
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
		Logtxt.Format("关闭绘图失败,错误码[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="关闭绘图成功";
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
		Logtxt.Format("设置文件运行失败,错误码[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="设置文件运行成功";
		OnWriteLog(Logtxt);
		nCode=StartAqcWave();
		if (nCode!=ERR_SUCC)
		{
			Logtxt.Format("切换文件运行失败,错误码[%d]",nCode);
			OnWriteLog(Logtxt);			
		}
		else
		{
			Logtxt="切换文件运行成功";
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
		Logtxt.Format("设置模拟运行失败,错误码[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="设置模拟运行成功";
		OnWriteLog(Logtxt);
		nCode=StartAqcWave();
		if (nCode!=ERR_SUCC)
		{
			Logtxt.Format("切换模拟运行失败,错误码[%d]",nCode);
			OnWriteLog(Logtxt);			
		}
		else
		{
			Logtxt="切换模拟运行成功";
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
		Logtxt.Format("设置现场运行失败,错误码[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="设置现场运行成功";
		OnWriteLog(Logtxt);
		nCode=StartAqcWave();
		if (nCode!=ERR_SUCC)
		{
			Logtxt.Format("切换现场运行失败,错误码[%d]",nCode);
			OnWriteLog(Logtxt);			
		}
		else
		{
			Logtxt="切换现场运行成功";
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
		Logtxt.Format("算法启动失败,错误码[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="算法启动成功";
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
		Logtxt.Format("算法关闭失败,错误码[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="算法关闭成功";
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
	//模型参数
	memset(&Mdl,0,sizeof(Mdl));
	Mdl=GetMainMdlSet();
	AlgoSet.m_JGHeight=Mdl.JGHeight;
	AlgoSet.m_ZeroPos=Mdl.ZeroPos;
	AlgoSet.m_StartX=Mdl.StartX;
	AlgoSet.m_LaneWd=Mdl.LaneWidth;
	AlgoSet.m_IsLanHt=Mdl.IsLanHt;
	AlgoSet.m_IsLanWd=Mdl.IsLanWd;
	AlgoSet.m_MaxHt=Mdl.MaxHt;
	//出车参数
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
			Logtxt.Format("模型参数失败,错误码[%d]",nCode);
			OnWriteLog(Logtxt);
		}
		else
		{
			Logtxt="模型参数设置成功";
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
			Logtxt.Format("出车参数失败,错误码[%d]",nCode);
			OnWriteLog(Logtxt);
		}
		else
		{
			Logtxt="出车参数设置成功";
			OnWriteLog(Logtxt);
		}
		nCode=ReLoadEngine();
		if (nCode!=ERR_SUCC)
		{
			Logtxt.Format("重启算法失败,错误码[%d]",nCode);
			OnWriteLog(Logtxt);
		}
		else
		{
			Logtxt="重启算法引擎成功";
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
			Logtxt.Format("设置激光参数失败,错误码[%d]",nCode);
			OnWriteLog(Logtxt);
		}
		else
		{
			Logtxt="设置激光参数成功";
			OnWriteLog(Logtxt);
		}
	}
}

void CAVISDlg::OnImpwv() 
{
	// TODO: Add your command handler code here
	CString m_path = _T("");  
	CString Logtxt = _T("");  
	TCHAR servPath[MAX_PATH];//用来存放文件夹路径  
	BROWSEINFO bi;  
	LPITEMIDLIST pidl;  
	bi.hwndOwner = this->m_hWnd;  
	bi.pidlRoot = NULL;  
	bi.pszDisplayName = servPath;  
	bi.lpszTitle = _T("选择波形文件路径");  
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  
	bi.lpfn = NULL;  
	bi.lParam = NULL;  
	bi.iImage = NULL;  
	if((pidl = SHBrowseForFolder(&bi)) != NULL)  
	{  
		if(SUCCEEDED(SHGetPathFromIDList(pidl, servPath))) //得到文件夹的全路径，不要的话，只得本文件夹名  
		{  
		  m_path.Format("%s",servPath);
		  ImportWvPath(m_path);
		  Logtxt="选择波形路径:"+m_path;
		  OnWriteLog(Logtxt);	  
		}  
	}	
}


void CAVISDlg::OnInitAlgoList()
{
	int nIndex=0;
	m_AlgoList.InsertColumn(nIndex++,_T("车序号"),LVCFMT_CENTER,80,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("车型"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("方向"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("轴数"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("原始轴型"),LVCFMT_CENTER,80,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("通用轴型"),LVCFMT_CENTER,80,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("车牌号码"),LVCFMT_CENTER,120,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("出车类型"),LVCFMT_CENTER,80,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("检测日期"),LVCFMT_CENTER,100,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("检测时间"),LVCFMT_CENTER,100,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("判定码"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("调整码"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("专车码"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("底盘高"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("车头高"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("车高"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("车斗"),LVCFMT_CENTER,50,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("头高跳"),LVCFMT_CENTER,70,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("尾高跳"),LVCFMT_CENTER,70,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("载货"),LVCFMT_CENTER,50,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("货箱"),LVCFMT_CENTER,50,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("货车尾"),LVCFMT_CENTER,60,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("掷地"),LVCFMT_CENTER,50,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("封闭窗"),LVCFMT_CENTER,80,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("玻璃窗"),LVCFMT_CENTER,80,-1);
	m_AlgoList.InsertColumn(nIndex++,_T("侧面一致"),LVCFMT_CENTER,80,-1);

	m_AlgoList.InsertColumn(nIndex++,_T("备注"),LVCFMT_CENTER,120,-1);
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
			Logtxt.Format("CamSet,码[%d]",nCode);
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
		Logtxt.Format("开启存库失败,错误码[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="开启存库成功";
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
		Logtxt.Format("关闭存库失败,错误码[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="关闭存库成功";
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
		Logtxt.Format("关闭存储波形失败,错误码[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="关闭存储波形成功";
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
		Logtxt.Format("开启存储波形失败,错误码[%d]",nCode);
		OnWriteLog(Logtxt);
	}
	else
	{
		Logtxt="开启存储波形成功";
		OnWriteLog(Logtxt);
	}			
}

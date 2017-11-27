// AVIMAIN.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "Avi.h"
#include "Avi_Main.h"
#include "IniProc.h"
#include "Avi_Comm.h"
#include "NetMgr.h"
#include "JGProxy.h"
#include "LmdsProc.h"
#include  "Camera.h"
#include "Avi_DB.h"
#include "Avi_SvWv.h"
/************************************************************************/
/*全局变量相关定义                                                      */
/************************************************************************/
VehSet     g_tempVehSet;	//车辆信息
ModelSet   g_ModelSet;      //算法模型参数
LaserSet   g_LaserSet;      //激光连接参数
SystemSet  g_SysTem;        //系统参数 方向、单双激光
VOutSet    g_VOutSet;       //出车阈值设置
WvFiles    g_WvFiles;       //模拟波形
AqcState   g_AqcState;      //波形状态
CLmdProc   g_LmdProc;       //LMD波形载入操作
CJGProxy   g_JGProxy;       //JG设备代理操作

QueSet     g_QueSet;        //队列管理相关
CamSet     g_CamSet;        //相机管理相关
SvWave     g_SvWave;        //存储波形相关


PFReportVh g_pfMainVeh=NULL;
PFPlotIm   g_pfPlotIm=NULL;
int g_NoPlateNum=0;//车牌为空次数
int g_CamReconNum=0;//相机重连次数
/************************************************************************/
/*队列线程相关定义                                                      */
/************************************************************************/
HANDLE g_HEvent[4]={NULL};
HANDLE g_HdMEvent=NULL;
BOOL   g_bMEvent=FALSE;
VehSet g_Vehqueue[255];  //车辆队列缓存
UINT  TimerQue = NULL;//定时清队列计时器
//队列全局变量申明
int g_VehNum = 0;//队列车辆总数
int g_Queuethred=1;//队列处理线程标志位

/************************************************************************/
/*读取LMD/LMDS波形线程                                                  */
/************************************************************************/
DWORD WINAPI WaveThreadProc(LPVOID lpParam);
HANDLE g_WaveHd= NULL;//申明线程句柄
BOOL g_bWaveSW=FALSE;//采集波形任务线程
UINT TimerAqc=NULL;//采集波形定时器
UINT TimerAUT=NULL;//授权认证定时器
UINT TimerFVh=NULL;//强制收尾定时器


//********************************************************************
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define new DEBUG_NEW


/////////////////////////////////////////////////////////////////////////////
// global data


// The following symbol used to force inclusion of this module for _USRDLL
#ifdef _X86_
extern "C" { int _afxForceUSRDLL; }
#else
extern "C" { int __afxForceUSRDLL; }
#endif
/**************************************
功能：0/测试函数
参数：
返回：TRUE 为成功，FALSE 为失败
**************************************/
AVIMAIN int __stdcall Avi_Test(int VPattern,int VDir,int VehId,int d)
{
	g_tempVehSet.AxleNum=2;
	g_tempVehSet.VDir=VDir;
	g_tempVehSet.VPattern=VPattern;
	g_tempVehSet.VehId=VehId;
	SetEvent(g_HEvent[0]);
	
	//	 int bb=99;
	//	 bb=TrigForceOut();
	//	 int data=0;
	//	WriteiniFlie("Avi_Camer");
	//	ReadIniFile();
	////	InitEngine(g_ModelSet, g_SysTem, g_VOutSet);
	//	WriteLogFile("Hello");//写系统日志
	//	RegPFReportV(ReportVh);//注册回调函数
	//	int Type=0;
	//	DWORD ErrCode=0;
	//	char IP[50]={0};
	// 	char password[50]={0};
	//	strcpy(IP,CarmerIP.c_str());
	//	strcpy(password,CarmerPassWord.c_str());
	//	WJ_camer_Init(IP, password, &Type, &ErrCode);//ok
	
	return 0;
}


/************************************************************************/
/* 1/定义回调函数内容
//提取车型信息
/************************************************************************/
int ReportVh(VehSet Veh)
{
	char LogBuf[255]={0};
	int i=0;
	//g_tempVehSet = Veh;//取出车型信息
	if (TimerFVh!=NULL)
	{
		KillTimer(NULL,TimerFVh);
		TimerFVh=0;
	}
	memcpy(&g_tempVehSet,&Veh,sizeof(Veh));

	SetEvent(g_HEvent[0]);
	CString StrTmp="";
	CString StrAx="";
	for (i=0;i<g_tempVehSet.AxleNum;i++)
	{
		StrAx.Format("%d",g_tempVehSet.AxleType[i]);
		StrTmp+=StrAx;
	}
	sprintf(LogBuf, "Algo|ID:%d|TP:%d|AX:%d/%s",g_tempVehSet.VehId,g_tempVehSet.VPattern,g_tempVehSet.AxleNum,StrTmp);
	WriteLogFile(LogBuf);
	return 0;
}

/************************************************************************/
/* 轮轴通过回调函数							                            */
/************************************************************************/
int ReportAx(int AxType)
{
	
	
	
	return ERR_SUCC;
}


/************************************************************************/
/* 车辆通过状态回调函数 0--进车；1--刚好收尾                            */
/************************************************************************/
int __stdcall VehState(int nState)
{
	switch(nState) 
	{
	case 0:
		SetEvent(g_HEvent[2]);
		break;
	case 1:	
		SetEvent(g_HEvent[3]);
		break;
	default:
		break;
	}
	return ERR_SUCC;
}

/************************************************************************/
/* 车辆通过绘图并保存						                            */
/************************************************************************/
int __stdcall DrawVehBmp(char *cName)
{
	CString FullPath="";
	char TmpPath[1024]={0};
	PlotInit(500,500);
	PlotVeh();
	GetImageRoot(TmpPath);
	FullPath.Format("%s%s",TmpPath,cName);
	SaveBmp(FullPath.GetBuffer(FullPath.GetLength()));
	PlotDispose();
	if (g_pfPlotIm!=NULL)
	{
		g_pfPlotIm(FullPath.GetBuffer(FullPath.GetLength()),cName);
	}
	FullPath.ReleaseBuffer();
	return ERR_SUCC;
}


/************************************************************************/
/* 车辆通过状态回调函数 0--进车；1--刚好收尾                            */
/************************************************************************/
int CALLBACK FormImage(int FrIdx,int *pX,int *pZ,int MaxH)
{
	IntoRows(FrIdx,pX,pZ,MaxH);
	return ERR_SUCC;
}

/**************************************
功能：2/系统初始化函数
参数：无 全部从配置文件或者上位机读取参数
返回：0 为成功，其他 为失败
**************************************/
AVIMAIN int __stdcall Avi_Init(void)
{
	int RetCode=ERR_FAIL;
	ClearGlobal();
	CheckRunDirectory();
	
	WriteLogFile("Init|CLR");
	LoadInISet();
	//
	RetCode=StartAviAlgo();
	if (RetCode!=ERR_SUCC)
	{
		WriteLogFile("Algo|Fail");
		return RetCode;
	}
	
	RetCode=StartAqcWave();
	if (RetCode!=ERR_SUCC)
	{
		WriteLogFile("AqcW|Fail");
		return RetCode;
	}
	
	RetCode=InitMutiEvent();
	if (RetCode!=ERR_SUCC)
	{
		WriteLogFile("InitMutiEvent..fail!");
		return RetCode;
	}
	RetCode=InitCamera();
	if (RetCode!=ERR_SUCC)
	{
		WriteLogFile("InitCamera..fail!");
		return RetCode;
	}
	
	RetCode=InitDb();
	if (RetCode!=ERR_SUCC)
	{
		WriteLogFile("InitDb..fail!");
		return RetCode;
	}
	
	RetCode=PreSaveWave();
	if (RetCode!=ERR_SUCC)
	{
		WriteLogFile("Prepare SvWave..fail!");
		return RetCode;
	}
	
	RetCode=InitPlotEnVi();
	if (RetCode!=ERR_SUCC)
	{
		WriteLogFile("Init Plot..fail!");
		return RetCode;
	}
	
    WriteLogFile("Avi_Init Success!");
	return ERR_SUCC;
}





/************************************************************************/
/* 初始化画图缓存及引擎                                                 */
/************************************************************************/
int InitPlotEnVi(void)
{
	ClearPlot();
	return ERR_SUCC;
}

/************************************************************************/
/* 初始化数据库存储                                                     */
/************************************************************************/
int InitDb(void)
{
	int RetCode=0;
	if (g_SysTem.SvDB==1)
	{
		RetCode=OpenMdb();
		if (RetCode)
		{
			return ERR_SUCC;
		}
		else
			return ERR_FAIL;
	}
	else
		return ERR_SUCC;
}









/************************************************************************/
/*初始化全局参数                                                        */
/************************************************************************/
void ClearGlobal(void)
{
	//清空配置参数
	memset(&g_ModelSet, 0, sizeof(g_ModelSet));
	memset(&g_SysTem, 0, sizeof(g_SysTem));
	memset(&g_VOutSet, 0, sizeof(g_VOutSet));
	memset(&g_CamSet,0,sizeof(g_CamSet));
	memset(&g_QueSet,0,sizeof(g_QueSet));
	memset(&g_WvFiles,0,sizeof(g_WvFiles));
	memset(&g_SvWave,0,sizeof(g_SvWave));
}

/************************************************************************/
/*加载ini文件参数                                                       */
/************************************************************************/
AVIMAIN void __stdcall LoadInISet(void)
{
	ReadAllPara();//读取配置文 获取配置参数
	WriteLogFile("ReadIniFile");
}


/************************************************************************/
/*启动算法引擎	                                                        */
/************************************************************************/
AVIMAIN int __stdcall StartAviAlgo(void)
{
	//1、初始化算法模块*/
	int RetCode=ERR_FAIL;
	RetCode=InitEngine();//初始化算法引擎
	if (RetCode!=ERR_SUCC)
	{
		return RetCode;
	}
	RetCode=RegFormImage(FormImage);	//注册回调函数
	if (RetCode!=ERR_SUCC)
	{
		return RetCode;
	}
    WriteLogFile("StartAviAlgo Success");
	return ERR_SUCC;
}

/************************************************************************/
/* End AqcWave before StartAqcWave                                      */
/************************************************************************/
void EndAqcWave(void)
{
	if ((g_SysTem.RunMode==LASERIN||g_SysTem.RunMode==SOFTSIM)
		&&g_JGProxy.m_Net.bConnected)
	{
		g_JGProxy.m_Net.Dispose();
		//关闭定时器
		if (TimerAqc)
		{
			KillTimer(NULL,TimerAqc);
		}
	}
	else if (g_SysTem.RunMode==FILESIN)
	{
		if (g_WaveHd!=NULL)
		{
			CloseHandle(g_WaveHd);
		}
		g_bWaveSW=FALSE;
		g_WvFiles.PathExist=0;
	}
}


/************************************************************************/
/*启动波形采集任务                                                      */
/************************************************************************/
AVIMAIN int __stdcall StartAqcWave(void)
{
	
	EndAqcWave();
	if (g_SysTem.RunMode==LASERIN
		||g_SysTem.RunMode==SOFTSIM)
	{
		g_JGProxy.m_DesIp=g_LaserSet.IP0;
		g_JGProxy.m_DesPort=g_LaserSet.Port0;
		g_JGProxy.m_Id=1;
		g_JGProxy.RegRecCallBack(JGRecNetData);
		g_JGProxy.ConnectJG();
		if (g_SysTem.RunMode==SOFTSIM)
		{
			g_JGProxy.m_Permited=true;//软件模拟授权通过；
		}
		else
		{
			g_JGProxy.m_Permited=false;
			if (TimerAUT)
			{
				KillTimer(NULL,TimerAUT);
				TimerAUT=0;
			}
			TimerAUT = SetTimer(NULL, 3, 3000, (TIMERPROC)TimerProc);//3000ms定时进行激光器认证检查
		}
		//开启定时器
		if (TimerAqc)
		{
			KillTimer(NULL,TimerAqc);
		}
		TimerAqc = SetTimer(NULL, 2, 18, (TIMERPROC)TimerProc);//18ms定时获取波形
		
	}
	else if (g_SysTem.RunMode==FILESIN)
	{
		if (g_WaveHd!=NULL)
		{
			CloseHandle(g_WaveHd);
		}
		g_bWaveSW=TRUE;
		g_WaveHd = CreateThread(NULL, 0, WaveThreadProc, NULL, 0, NULL);//创建线程
		if (g_WaveHd==NULL)
		{
			g_bWaveSW=FALSE;
			return ERR_FAIL;
		}
	}
	char LogBuf[255]={0};
	sprintf(LogBuf, "g_SysTem.RunMode %d",g_SysTem.RunMode);
	WriteLogFile(LogBuf);
	return ERR_SUCC;
}

/************************************************************************/
/* 接受到激光波形                                                       */
/************************************************************************/
void JGRecNetData(char *pDataBuf, int nDataBufSize)
{
	UINT CurIndx=0;
	UINT32 CurTm=0;
	if (pDataBuf[0]==0x02&&pDataBuf[1]==0x02
		&&pDataBuf[2]==0x02&&pDataBuf[3]==0x02)
	{
		if (nDataBufSize==831&&g_JGProxy.m_Permited)
		{
			InsertWave((unsigned char *)pDataBuf,831);
			g_JGProxy.m_WvLost=0;
			if (g_SysTem.WvCheck)
			{
				CurTm=(((BYTE)pDataBuf[42]<<24)+((BYTE)pDataBuf[43]<<16)+((BYTE)pDataBuf[44]<<8)+(BYTE)pDataBuf[45]);
				CurIndx=(((BYTE)pDataBuf[50]<<8)+(BYTE)pDataBuf[51]);
				g_JGProxy.StaticLostWave(CurIndx,CurTm);
			}
		}
		else if ((BYTE)pDataBuf[9]==0xA1&&nDataBufSize==123)
		{
			g_JGProxy.CheckPermit(pDataBuf,nDataBufSize);
			if (g_JGProxy.m_Permited)
			{
				WriteLogFile("激光认证通过");
			}
		}
		else if ((BYTE)pDataBuf[9]==0xAE&&nDataBufSize==17)
		{
			g_JGProxy.SendBeatCmd();
			WriteLogFile("应答心跳包");
		}
	}
	else
	{
		g_AqcState.RecWvErr++;
	}
}
/************************************************************************/
/* 读取波形线程                                                         */
/************************************************************************/
DWORD WINAPI WaveThreadProc(LPVOID lpParam)
{
	LmdInfo Wv;
	char LogBuf[255]={0};
	g_LmdProc.m_ReadSet.nAutoEn=g_WvFiles.AddEmFram;
	g_LmdProc.m_ReadSet.nForOut=g_WvFiles.ForceOut;
	BOOL WvSearched=FALSE;
	while (g_bWaveSW)
	{
		try
		{
			if (g_WvFiles.PathExist)
			{
				if(g_WvFiles.bSearChed==FALSE)
				{
					g_LmdProc.SearchWave(g_WvFiles.WvPath);
					g_WvFiles.bSearChed=TRUE;
				}
				memset(&Wv,0,sizeof(Wv));
				g_LmdProc.ReadNextFram(&Wv);
				if (Wv.lmddata1[0]==0x02&&Wv.lmddata1[1]==0x02
					&&Wv.lmddata1[2]==0x02&&Wv.lmddata1[3]==0x02)
				{
					memcpy(&Wv.lmddata1[52],g_LmdProc.m_ReadSet.ItemName,24);
					InsertWave(Wv.lmddata1,831);
				}
				else
				{
					g_AqcState.RecWvErr++;
				}
				Sleep(7);
			}
			else
			{
				Sleep(3000);
			}
		}
		catch(...)
		{
			sprintf(LogBuf, "线程异常 读取波形");
			WriteLogFile(LogBuf);
		}
		
	}
	WriteLogFile("采集波形线程关闭");
	return ERR_SUCC;
}

/************************************************************************/
/*导入波形路径                                                          */
/************************************************************************/
AVIMAIN int __stdcall ImportWvPath(CString Path)
{
	if (PathFileExists(Path))
	{
		memset(g_WvFiles.WvPath,0,sizeof(g_WvFiles.WvPath));
		memcpy(g_WvFiles.WvPath,Path.GetBuffer(Path.GetLength()),Path.GetLength());
		Path.ReleaseBuffer();
		g_WvFiles.bSearChed=FALSE;
		g_WvFiles.PathExist=1;
		return ERR_SUCC;
	}
	else
	{
		return ERR_FAIL;
	}
}


/************************************************************************/
/* 初始化多事件及处理线程                                               */
/************************************************************************/
int InitMutiEvent(void)
{
	if(g_HEvent[0]!=NULL)
	{
		CloseHandle(g_HEvent[0]);
		g_HEvent[0]=NULL;
	}
	g_HEvent[0]=CreateEvent(NULL, TRUE, FALSE, "IntoQue");//创建事件检查队列
	if(g_HEvent[1]!=NULL)
	{
		CloseHandle(g_HEvent[1]);
		g_HEvent[1]=NULL;
	}
	g_HEvent[1]=CreateEvent(NULL,TRUE,FALSE,"EndHandle");//创建强制收尾事件
	if(g_HEvent[2]!=NULL)
	{
		CloseHandle(g_HEvent[2]);
		g_HEvent[2]=NULL;
	}
	g_HEvent[2]=CreateEvent(NULL,TRUE,FALSE,"VehInto");//车刚进入
	if(g_HEvent[3]!=NULL)
	{
		CloseHandle(g_HEvent[3]);
		g_HEvent[3]=NULL;
	}
	g_HEvent[3]=CreateEvent(NULL,TRUE,FALSE,"VehOut");//车刚离开
	if (g_HdMEvent!=NULL)
	{
		CloseHandle(g_HdMEvent);
		g_HdMEvent=NULL;
	}
	g_bMEvent=TRUE;
	g_HdMEvent=CreateThread(NULL, 0, ThreadMutiEvProc, NULL, 0, NULL);//创建多事件处理线程
	if (g_HdMEvent=NULL)
	{
		g_bMEvent=FALSE;
		return ERR_FAIL;
	}
	return ERR_SUCC;	
}

/************************************************************************/
/* 多事件任务处理线程                                                   */
/************************************************************************/
DWORD WINAPI ThreadMutiEvProc(LPVOID lpParam)
{
	DWORD nCode=0;
	char LogBuf[255]={0};
	char PlateColor[3]={0};//车牌颜色校验
	char YellowPlate[3]="黄";
	char BluePlate[3]  ="蓝";
	char checkplate[20]={0};
	while (g_bMEvent)//如何关闭线程
	{
		nCode= WaitForMultipleObjects(4,g_HEvent,FALSE,INFINITE);
		
		if (nCode==WAIT_OBJECT_0)//车辆入队
		{
			try
			{
				WriteLogFile("nCode==WAIT_OBJECT_0");	
				ResetEvent(g_HEvent[0]);
				
				if(strlen(CameraInfo0)<1)
				{
					memcpy(g_tempVehSet.Plate,"no Plate",sizeof(CameraInfo0));
				}
				else
				{
					memcpy(g_tempVehSet.Plate,CameraInfo0,sizeof(CameraInfo0));
				}
				//********************2、轴型转换*************************//
				AxleTransform(&g_tempVehSet);				//轴型转换 计算轴组
				//********************1、车型校验*************************//
				memcpy(PlateColor,g_tempVehSet.Plate,2);
				if((strcmp(BluePlate,PlateColor)==0)&&(g_tempVehSet.VPattern>5)) //蓝牌 货2以上转货1
				{
					g_tempVehSet.AxlegroupNum  = 2;
					g_tempVehSet.AxleNum=2;
					if(g_tempVehSet.VPattern>6)
					{
						g_tempVehSet.AxleType[0]=1;
						g_tempVehSet.AxleType[1]=2;
						g_tempVehSet.TranAxleType=12;
					}
					g_tempVehSet.VPattern=5;
					WriteLogFile("车型转换 蓝牌 货2-5转货1");			
				}
				
				if((strcmp(YellowPlate,PlateColor)==0)&&(g_tempVehSet.VPattern==5))//黄牌 货1转货2
				{
					g_tempVehSet.VPattern=6;
					WriteLogFile("车型转换 黄牌 货1转货2");
				}
				if((strcmp(YellowPlate,PlateColor)==0)&&(g_tempVehSet.VPattern==1))//黄牌 客1转客2
				{
					if (g_tempVehSet.VPattern==1&&g_tempVehSet.VehHeight<1700)
					{
					}
					else
					{
						g_tempVehSet.VPattern=2;
						g_tempVehSet.AxlegroupNum  = 2;
						g_tempVehSet.TranAxleType=12;
						g_tempVehSet.AxleNum=2;
						g_tempVehSet.AxleType[0]=1;
						g_tempVehSet.AxleType[1]=2;
						WriteLogFile("车型转换 黄牌 客1转客2");
					}
			
				}
					
				if((strcmp(BluePlate,PlateColor)==0)&&(g_tempVehSet.VPattern>2)&&(g_tempVehSet.VPattern<5))//蓝牌 客3、4转客2
				{
					g_tempVehSet.VPattern=2;
					WriteLogFile("车型转换 蓝牌 客3/4转客2");			
				}
				if((strcmp(BluePlate,PlateColor)==0)&&(g_tempVehSet.VPattern==2))//蓝牌 客2转客1
				{
					g_tempVehSet.VPattern=1;
					g_tempVehSet.AxlegroupNum  = 2;
					g_tempVehSet.TranAxleType=11;
					g_tempVehSet.AxleNum=2;
					g_tempVehSet.AxleType[0]=1;
					g_tempVehSet.AxleType[1]=1;
					WriteLogFile("车型转换 蓝牌 客2转客1");			
				}							
			}
			catch(...)
			{		
				//写日志 不处理CException& e
				DWORD nErr=GetLastError();
				sprintf(LogBuf, "线程异常1[%d] 加入队列",nErr);
				WriteLogFile(LogBuf);
			}
			try
			{
				if (g_pfMainVeh!=NULL)
				{
					g_pfMainVeh(g_tempVehSet);
				}
				if (g_SysTem.SvDB==1)
				{
					Add2Db(g_tempVehSet);
				}
				else
				{
					if (g_SysTem.SvDB)
					{
						Add2Txt(g_tempVehSet);
					}
				}
			}
			catch(...)
			{		
				//写日志 不处理CException& e 
				DWORD nErr=GetLastError();
				sprintf(LogBuf, "线程异常2[%d] 回调存数据",nErr);
				WriteLogFile(LogBuf);
			}
			try
			{
				sprintf(LogBuf, "接收车型，车辆ID%d,plate:%s,车型%d,轴数%d,出车类型：%d",
					g_tempVehSet.VehId, g_tempVehSet.Plate,g_tempVehSet.VPattern, g_tempVehSet.AxleNum, g_tempVehSet.VoutFlag);
				WriteLogFile(LogBuf);
				memset(CameraInfo0, 0, sizeof(CameraInfo0));
				//		if (g_tempVehSet.VDir == 1)//正常进车 加入队列
				//		{ 
				//********************3、加入队列*************************//
				g_VehNum++;
				memcpy(&g_Vehqueue[g_VehNum - 1],&g_tempVehSet,sizeof(g_tempVehSet));//加入队列
				//g_Vehqueue[g_VehNum - 1] = g_tempVehSet;	//加入队列
				
				sprintf(LogBuf, "加入队列 车辆ID%d,plate:%s,车型%d,轴数%d,出车类型：%d,队列总数%d", 
					g_Vehqueue[g_VehNum - 1].VehId,g_Vehqueue[g_VehNum - 1].Plate, g_Vehqueue[g_VehNum - 1].VPattern, g_Vehqueue[g_VehNum - 1].AxleNum, g_Vehqueue[g_VehNum - 1].VoutFlag,g_VehNum);
				WriteLogFile(LogBuf);
				//********************4、车牌重连判断*************************//
				memcpy(checkplate,g_tempVehSet.Plate,sizeof(checkplate));
				CameraCheck(checkplate);
			}
			catch(...)
			{		
				//写日志 不处理CException& e
				DWORD nErr=GetLastError();
				sprintf(LogBuf, "线程异常3[%d] 加入队列",nErr);
				WriteLogFile(LogBuf);
			}
			try
			{
				//********************5、队列超长判断*************************//
				if (g_VehNum > (g_QueSet.MaxVehCnt - 1))
				{
					if(strlen(checkplate)>6)//车牌不为空 且超过队列最大值 删除头车
					{
						WriteLogFile("队列维护 车牌不为空 且超过队列最大值 删除头车");
						CleanEndVeh();
					}
					else
					{
						WriteLogFile("队列维护 车牌无效 且超过队列最大值 删除当前车");
						CleanHeadVeh();
					}
					
				}
				//	 VehQueCheck();
			}
			catch(...)
			{		
				//写日志 不处理CException& e
				DWORD nErr=GetLastError();
				sprintf(LogBuf, "线程异常4[%d] 队列超长判断",nErr);
				WriteLogFile(LogBuf);
			}
			
			
	}
	//	else if (g_tempVehSet.VDir == 2)//完全倒车
	//	{
	//	  if (g_VehNum == 0)
	//	  {
	//		  //写日志 无车出现倒车
	//		  sprintf(LogBuf, "不入队列 队列无车不可能倒车！车辆ID%d,plate:%s,车型%d,轴数%d,出车类型：%d,队列总数%d", 
	//			  g_tempVehSet.VehId, g_tempVehSet.Plate,g_tempVehSet.VPattern, g_tempVehSet.AxleNum, g_tempVehSet.VoutFlag,g_VehNum);
	//		  WriteLogFile(LogBuf);
	//		  return 1;
	//	  }
	//	  else
	//	  {
	//		  sprintf(LogBuf, "完全倒车 该车不入列，车辆ID%d,plate:%s,车型%d,轴数%d,出车类型：%d,队列总数%d",
	//			  g_tempVehSet.VehId, g_tempVehSet.Plate,g_tempVehSet.VPattern, g_tempVehSet.AxleNum, g_tempVehSet.VoutFlag,g_VehNum);
	//		  WriteLogFile(LogBuf);
	//		  CleanEndVeh();//删除上一辆车
	//	  }				
	//	}
	//	else if (g_tempVehSet.VDir == 3)//不完全进车、倒车
	//	{
	//	  //写日志 不处理
	//	  sprintf(LogBuf, "半倒车，该车不入列，不处理，车辆ID%d,plate:%s,车型%d,轴数%d,出车类型：%d,队列总数%d",
	//		  g_tempVehSet.VehId, g_tempVehSet.Plate,g_tempVehSet.VPattern, g_tempVehSet.AxleNum, g_tempVehSet.VoutFlag,g_VehNum);
	//	  WriteLogFile(LogBuf);
	//	}
	//	else
	
	//			  ;
	//		}
	else if (nCode==(WAIT_OBJECT_0+1))//强制收尾超时
	{
		try
		{
			WriteLogFile("强制收尾 没反应 造车!");
			CreateHeadVeh();
			//出客1
			// return -1;			
			ResetEvent(g_HEvent[1]);
		}
		catch(...)
		{		
			//写日志 不处理CException& e
			DWORD nCode=GetLastError();
			sprintf(LogBuf, "线程异常7[%d] 强制收尾超时,nCode");
			WriteLogFile(LogBuf);
		}
		
	}
	else if (nCode==(WAIT_OBJECT_0+2))//车辆刚进来
	{
		try
		{
			OnCapture();
			ResetEvent(g_HEvent[2]);
		}
		catch(...)
		{		
			//写日志 不处理CException& e
			DWORD nCode=GetLastError();
			sprintf(LogBuf, "线程异常6[%d] 车辆刚进来,nCode");
			WriteLogFile(LogBuf);
		}
	}
	else if (nCode==(WAIT_OBJECT_0+3))//车辆收尾
	{
		try
		{
			CString BmpName="";
			CString strtm="";
			CTime tm = CTime::GetCurrentTime();
			
			if (g_SysTem.PlotImg)
			{
				strtm=tm.Format("%Y-%m-%d %H-%M-%S ");
				BmpName.Format("%s-%d.bmp",strtm,g_tempVehSet.VehId+1);
				DrawVehBmp(BmpName.GetBuffer(BmpName.GetLength()));
				BmpName.ReleaseBuffer();
			}
			ResetEvent(g_HEvent[3]);
		}
		
		catch(...)
		{		
			//写日志 不处理CException& e
			DWORD nCode=GetLastError();
			sprintf(LogBuf, "线程异常5[%d] 车辆收尾,nCode");
			WriteLogFile(LogBuf);
		}
		
	}
	}
	WriteLogFile("多事件处理线程结束");
	return 0;
}


/************************************************************************/
/* 初始相机                                                             */
/************************************************************************/
//int InitCamera(void)
//{
//	//1.2、初始化摄像头模块*/
//
//	NET_DVR_Init();
//	DWORD FLAG=0;
//	int ret = 0;
//	//设置连接时间与重连时间
//	NET_DVR_SetConnectTime(2000, 1);
//	NET_DVR_SetReconnect(10000, true);
//
//	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
//	lUserID = NET_DVR_Login_V30((char *)CarmerIP.data(), 8000, "admin",(char *)CarmerPassWord.data(), &struDeviceInfo);
//	if (lUserID < 0)
//	{
//		FLAG=NET_DVR_GetLastError();
//		sprintf(log_buf,"登录摄像头失败，代码:%d",FLAG);
//		WriteLogFile(log_buf);
//		return FALSE;
//	}
//	WriteLogFile("登录摄像头成功");
//	//设置报警回调函数
//	NET_DVR_SetDVRMessageCallBack_V30(MSesGCallback, NULL);
//
//	//启用布防
//	NET_DVR_SETUPALARM_PARAM struSetupParam={0};
//	struSetupParam.dwSize=sizeof(NET_DVR_SETUPALARM_PARAM);
//
//	//上传报警信息类型: 0- 老报警信息(NET_DVR_PLATE_RESULT), 1- 新报警信息(NET_ITS_PLATE_RESULT)
//	struSetupParam.byAlarmInfoType=1;
//	struSetupParam.byLevel=1;
//	lHandle = NET_DVR_SetupAlarmChan_V41(lUserID,&struSetupParam);
//	if (lHandle < 0)
//	{
//		FLAG=NET_DVR_GetLastError();
//
//		sprintf(log_buf,"摄像头布放失败，代码:%d",FLAG);
//		WriteLogFile(log_buf);
//		return FALSE;
//	}
//	WriteLogFile("摄像头布防成功");
//	return TRUE;
//}
//BOOL Closecarmer(void)
//{
//	DWORD FLAG=0;
//	//撤销布防上传通道
//	if (!NET_DVR_CloseAlarmChan_V30(lHandle))
//	{
//		FLAG=NET_DVR_GetLastError();
//		NET_DVR_Logout(lUserID);
//		//NET_DVR_Cleanup();
//		return FALSE;
//	} 
//	//注销用户
//	NET_DVR_Logout(lUserID);
//	//释放 SDK 资源
//	//NET_DVR_Cleanup();
//	return TRUE;
// }

/**************************************
说明：摄像头回调函数
参数：无
返回值：无
**************************************/
//void CALLBACK MSesGCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser)
//{
//	//char filename[100];
//	//FILE *fSnapPic=NULL;
//	//FILE *fSnapPicPlate=NULL;
//	//CString tempIP=pAlarmer->sDeviceIP;
//	//CString slog="";
//	switch (lCommand)
//	{
//	case COMM_UPLOAD_PLATE_RESULT:
//		{
//			NET_DVR_PLATE_RESULT struPlateResult={0};
//			memcpy(&struPlateResult, pAlarmInfo, sizeof(struPlateResult));
//			resStr=struPlateResult.struPlateInfo.sLicense;
//			strcpy(resch,resStr.c_str());
//			PlateInfo_flag=1;
//			WriteLogFile(resch);
//			break;
//		}
//	case COMM_ITS_PLATE_RESULT:
//		{
//			NET_ITS_PLATE_RESULT struITSPlateResult={0};
//			memcpy(&struITSPlateResult, pAlarmInfo, sizeof(struITSPlateResult));
//			resStr=struITSPlateResult.struPlateInfo.sLicense;
//			strcpy(resch,resStr.c_str());
//			PlateInfo_flag=1;
//			WriteLogFile(resch);
//
//			//CF_C_V12Dlg::old1_flag=0;
//			//CF_C_V12Dlg::new1_flag=1;
//			//memcpy(&CF_C_V12Dlg::struITSPlateResult_HC, pAlarmInfo, sizeof(CF_C_V12Dlg::struITSPlateResult_HC));
//
//			//CString LogData="";
//
//			//for (int i=0;i<CF_C_V12Dlg::struITSPlateResult_HC.dwPicNum;i++)
//			//{
//			//	LogData.Format("车牌号: %s",CF_C_V12Dlg::struITSPlateResult_HC.struPlateInfo.sLicense);
//			//	switch(CF_C_V12Dlg::struITSPlateResult_HC.struPlateInfo.byColor)//车牌颜色
//			//	{
//			//	case VCA_BLUE_PLATE:
//			//		LogData=LogData+" 车牌颜色：蓝色";
//			//		break;
//			//	case VCA_YELLOW_PLATE:
//			//		LogData=LogData+" 车牌颜色：黄色";
//			//		break;
//			//	case VCA_WHITE_PLATE:
//			//		LogData=LogData+" 车牌颜色：白色";
//			//		break;
//			//	case VCA_BLACK_PLATE:
//			//		LogData=LogData+" 车牌颜色：黑色";
//			//		break;
//			//	default:
//			//		break;
//			//	}
//
//			//	//保存场景图
//			//	if ((CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].dwDataLen != 0)&&(CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].byType==
//			//		1)||(CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].byType == 2))
//			//	{
//			//		CF_C_V12Dlg::sPic(0,CF_C_V12Dlg::struITSPlateResult_HC,i);
//			//	}
//			//	//车牌小图片
//			//	if ((CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].dwDataLen != 0)&&(CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].byType == 0))
//			//	{
//			//		CF_C_V12Dlg::sPic(1,CF_C_V12Dlg::struITSPlateResult_HC,i);
//			//	}
//
//			//	//其他信息处理......
//			//}
//			//CF_C_V12Dlg::uploadLog("设备1:1->上传车辆信息");
//
//			//LogData=LogData+'\n';	
//			//CF_C_V12Dlg::wLog(LogData);
//
//			////CF_C_V12Dlg::iNum++;
//			//::SendMessage(CF_C_V12Dlg::m_hwnd,WM_ADDLISTHC,0,0);
//			break;
//		}
//	default:
//		break;
//	}
//	return;
//}

/************************************************************************/
/* 定时器功能执行体                                                     */
/************************************************************************/
void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT nIDEvent,DWORD dwTime)
{
	if (nIDEvent==NULL||nIDEvent==0)
	{
		return;
	}
	/************************************************************************/
	/* 定时器定时 队列有车超时 清队列
	/************************************************************************/
	else if (nIDEvent==TimerQue)
	{
		if (g_VehNum >0)
		{
			char LogBuf[255]={0};
			sprintf(LogBuf, "%dms 未取卡删队列,队列总数%d", g_QueSet.ClearQueTime, g_VehNum);
			WriteLogFile(LogBuf);
			CleanQueVeh();
		}
		KillTimer(NULL, TimerQue);
		TimerQue = SetTimer(NULL, 1, g_QueSet.ClearQueTime, (TIMERPROC)TimerProc);//分钟没有获取车型即清队列
		//	 TimerQue = 0;
	}
	else if (nIDEvent==TimerAqc)
	{
		if (g_JGProxy.m_Net.bConnected)
		{
			if (g_JGProxy.m_Permited)
			{
				if (g_LaserSet.AqcMode==1)
				{
					if (g_JGProxy.m_WvLost>20)
					{
						g_JGProxy.AqcContiWv();
					}
				}
				else 
				{
					g_JGProxy.AqcSingleWv();
				}
			}
		}
		if (g_JGProxy.m_Permited)
		{
			g_JGProxy.m_WvLost++;
			//连续200次没收到发连续要数指令
			if ((g_JGProxy.m_WvLost>50)&&(g_JGProxy.m_WvLost<52))
			{	
				if (g_JGProxy.m_Net.bConnected)
				{
					//超过4s每收到波形，主动给关闭再重连
					g_JGProxy.m_Net.DisConnSvr();
					WriteLogFile("连续1s丢数,主动断开激光发起重连");
					SetEvent(g_JGProxy.m_Net.hSvrDis);
				}
			}
			else if(g_JGProxy.m_WvLost>51)
			{
				g_JGProxy.m_WvLost=52;
			}
		}
		
	}
	else if (nIDEvent==TimerAUT)
	{
		if (g_JGProxy.m_Net.bConnected)
		{
			g_JGProxy.SendStateCmd();
		}
		if (g_JGProxy.m_Permited)
		{
			KillTimer(NULL,TimerAUT);
			TimerAUT=0;
		}
	}
	else if (nIDEvent==TimerFVh)//强制收尾超时
	{
		SetEvent(g_HEvent[1]);
		if (TimerFVh!=NULL)
		{
			KillTimer(NULL,TimerFVh);
			TimerFVh=0;
		}
	}
}


/**************************************
功能：3/系统关闭
参数：无 全部从配置文件或者上位机读取参数
返回：0 为成功，其他 为失败
**************************************/
AVIMAIN int __stdcall Avi_Close(void)
{
	ExitEngine();// 关闭算法引擎 
	g_Queuethred=0;//关闭队列接收线程
	// 激光器通道
	DWORD ErrCode = 0;
	//		WJ_Closecarmer(&ErrCode);
	Closecarmer();
	//x写日志
	SaveAllPara();
	WriteLogFile("Avi_Close");
	return 0;
}

/**************************************
4/获取设备车辆数
int  Avi_VehCount(void)
说明：获取车型识别设备当前车辆总数；
参数：无；
返回值：
≥0	总车数，0为没有车；
- 1 	失败。
**************************************/
AVIMAIN int __stdcall Avi_VehCount(void)
{
	//写日志
	char LogBuf[255]={0};
	sprintf(LogBuf, "队列总数 :%d", g_VehNum);
	WriteLogFile(LogBuf);
	return g_VehNum;
}

/**************************************
功能：5/获取车辆信息
参数：
[输出]int AxisGroupNum		轴组数；
[输出]int AxisNum			轴数；
[输出]BYTE VehicleType  	车型(1 - 11)；
（1 - 4 为 客1，客2，客3，客4
5 - 11为 货1，货2，货3，货4，货5，货6，货7）
[输出]long AxisWeight    	轴组重（预留）；
[输出]int AxisType 		轴型（1 - 7）；
[输出]int Speed				车速度；
[输出]BYTE iLeiXing 	0 - 正常过车；1 - 倒车；2 - 清除此车辆数据，且将之前收到的缓冲区中的数据全部清除。
返回：0 为成功，其他 为失败
**************************************/
bool  GetVehicle(VehSet *tempVehSet,char* plate)
{
	char LogBuf[255]={0};
	char tempPlate1[20]={0};
	int platenum=-1;
	int likemost=0;//最相似
	int templike=0;
	char noplate[20]="无牌车";
	
	/************************************************************************/
	/* 车牌匹配度模拟测试	
	/************************************************************************/	
	char platesame[20]="111";//与头车车牌相同
	char plate80[20]="888";//与头车车牌相似度80
	char plate20[20]="222";//与头车车牌相似度20
	char plateSecond[20]="333";//第二车车牌相同
	
	
	if(plate==NULL)//只要有一个为空 返回0
	{
		return 0;
	}
	else if(strlen(plate)==3)//测试车牌代码
	{
		if(strcmp(plate,platesame)==0)//与头车车牌相同
		{	
			memcpy(plate,g_Vehqueue[0].Plate,sizeof(tempPlate1));
			sprintf(LogBuf, "车牌模拟 相同 模拟车牌%s", plate);
			WriteLogFile(LogBuf);
		}
		else if(strcmp(plate,plateSecond)==0)//与二车车牌相同
		{
			memcpy(plate,g_Vehqueue[1].Plate,sizeof(tempPlate1));
			sprintf(LogBuf, "车牌模拟 二车相同 模拟车牌%s", plate);
			WriteLogFile(LogBuf);
		}
		
		else if(strcmp(plate,plate80)==0)//与头车车牌相似度80
		{
			for(int i=0;i<10;i++)
			{
				if((i==4)||(i==5))
					memcpy(plate+i,":",sizeof(char));	
				else
					plate[i]=g_Vehqueue[0].Plate[i];	
			}
			sprintf(LogBuf, "车牌模拟 头车80 模拟车牌%s, 头车车牌:%s", plate,g_Vehqueue[0].Plate);
			WriteLogFile(LogBuf);
		}
		else if(strcmp(plate,plate20)==0)//与头车车牌相似度20
		{
			for(int i=0;i<10;i++)
			{
				if((i==4)||(i==5))
					plate[i]=g_Vehqueue[0].Plate[i];
				else
					memcpy(plate+i,":",sizeof(char));	
			}
			sprintf(LogBuf, "车牌模拟 头车20 模拟车牌%s, 头车车牌:%s", plate,g_Vehqueue[0].Plate);
			WriteLogFile(LogBuf);
		}
	}
	else 
	{
		
	}
	/************************************************************************/
	/* 车牌匹配度模拟测试end	
	/************************************************************************/	
	sprintf(LogBuf, "来取车牌 车牌%s, 头车车牌:%s", plate,g_Vehqueue[0].Plate);
	WriteLogFile(LogBuf);
	
	//1、判断队列数 
	if (g_VehNum > 0)//队列有车
	{
		//车牌匹配
		for(int i=g_VehNum-1;i>=0;i--)//从队列尾开始匹配
		{
			memcpy(tempPlate1,g_Vehqueue[i].Plate,sizeof(tempPlate1));
			templike=platecheck(tempPlate1,plate);
			if((templike>=likemost)&&(templike>=80))//相似度达到80以上
			{
				platenum=i;//记录匹配序号 最匹配的车牌序号
				likemost=templike;
			}
		}
		memcpy(tempPlate1,g_Vehqueue[0].Plate,sizeof(tempPlate1));
		if(platenum==-1)//车牌匹配失败
		{
			if (
				(strcmp(noplate,tempPlate1))==0 
				||(strcmp(noplate,plate)==0)
				||(strlen(tempPlate1)==0)
				||(strlen(plate)==0)
				)
				//车牌匹配失败 并且 两者有一方是 无牌车或空车牌 此时出头车
			{
				memcpy(tempVehSet,&g_Vehqueue[0],sizeof(g_Vehqueue[0]));
				sprintf(LogBuf, "车牌匹配 失败 头车是无牌车 出头车\n车辆ID%d,取车车牌%s,车型%d,轴数%d,出车类型：%d,队列总数%d", 
					tempVehSet->VehId, plate,tempVehSet->VPattern, tempVehSet->AxleNum, tempVehSet->VoutFlag,g_VehNum);
				WriteLogFile(LogBuf);
				if(g_QueSet.DelVehTime<0)//立即自动删除车辆
				{
					CleanHeadVeh();
				}
				
			}	
			//车牌匹配失败 头车有车牌  此时造车出客1  队列不变
			else//双方都是有效车牌 但匹配失败 造车
			{
				tempVehSet->VPattern=1;
				tempVehSet->AxleNum=2;
				tempVehSet->AxleType[0]=1;
				tempVehSet->AxleType[1]=1;
				tempVehSet->VSpeed=0;//
				tempVehSet->VehId=-1;//
				memcpy(tempVehSet->Plate,noplate,sizeof(noplate));
				
				sprintf(LogBuf, "匹配失败 车辆ID%d,车牌%s,车型%d,轴数%d,出车类型：造车,队列总数%d,相似度:%d",
					tempVehSet->VehId, tempVehSet->Plate,tempVehSet->VPattern, tempVehSet->AxleNum,g_VehNum,likemost);
				WriteLogFile(LogBuf);
			}
		}
		
		else //车牌匹配成功 出匹配成功的车型
		{
			memcpy(tempVehSet,&g_Vehqueue[platenum],sizeof(g_Vehqueue[platenum]));
			sprintf(LogBuf, "车牌匹配 %d 车辆ID%d,车牌%s,车型%d,轴数%d,出车类型：%d,队列总数%d,相似度:%d", platenum,
				tempVehSet->VehId, plate,tempVehSet->VPattern, tempVehSet->AxleNum, tempVehSet->VoutFlag,g_VehNum,likemost);
			WriteLogFile(LogBuf);
			for(int ii=0;ii<platenum;ii++)//队列维护 清理匹配车牌之前的所有车量信息
			{
				CleanHeadVeh();
			}
			if(g_QueSet.DelVehTime<0)//立即自动删除车辆
			{
				CleanHeadVeh();
			}
		}
	}
	else
	{
		WriteLogFile("队列无车 GetVehicle不出车型");
	}
	//	else//队列无车 强制收尾
	//	{
	//		if(strlen(CameraInfo0)>6)//长度大于6 表示肯定有车牌 进行匹配
	//		{
	//			if(platecheck(CameraInfo0,plate)>80)//车牌匹配成功 进行强制收尾
	//
	//				if(TrigForceOut()==1)//可以收尾
	//				//此时应当出收尾车型 但车型从会调函数上来 不能立马出车
	//
	//		}
	//				memset(CameraInfo0, 0, sizeof(CameraInfo0));
	//	memset(tempVehSet,0,sizeof(g_Vehqueue[0]));	
	//	}
	//	if(g_QueSet.DelVehTime<0)//立即自动删除车辆
	//	{
	//		CleanHeadVeh();
	//	}
	//计时 长时间未取卡清除队列
	if (TimerQue)
	{
		KillTimer(NULL, TimerQue);
		TimerQue = 0;
		WriteLogFile("retimer");
	}
	TimerQue = SetTimer(NULL, 1, g_QueSet.ClearQueTime, (TIMERPROC)TimerProc);//分钟没有获取车型即清队列
	
	return 0;
}
/************************************************************************/
/* 车牌匹配度计算

  /************************************************************************/
  int platecheck(char* plate0,char* plate1)
  {
	  if((plate0==NULL)||(plate1==NULL))//只要有一个为空 返回0
		  return 0;
	  if((strlen(plate0)==0)||(strlen(plate1)==0))//只要有一个为空 返回0
		  return 0;
	  if(strcmp(plate0,plate1)==0)//车牌完全匹配
		  return 100;
	  else
	  {
		  int samenum=0;
		  for(int i=0;i<strlen(plate0);i++)
		  {
			  if(plate0[i]==plate1[i])
				  samenum++;		
		  }
		  return samenum*10;
	  }
	  
  }
  
  /************************************************************************/
  /* 智能摄像头状态
  1、如果连续4次没有获取到车牌 重新连接摄像头
  /************************************************************************/
  void CameraCheck(char* plate)
  {
	  char LogBuf[255]={0};
	  if(strlen(plate)==0)//车牌为空
	  {
		  g_NoPlateNum++;
	  }
	  else
	  {
		  g_NoPlateNum=0;
	  }
	  if((g_NoPlateNum>10)/*&&(g_CamReconNum<10)*/)//连续4次车牌为空 重新连接摄像头
	  {
		  sprintf(LogBuf, "重连相机 连续10次车牌为空 重连次数%d", g_CamReconNum);
		  WriteLogFile(LogBuf);
		  Closecarmer();
		  g_NoPlateNum=0;
		  InitCamera();
		  g_CamReconNum++;
	  }
  }
  /************************************************************************/
  /* 轴型转换函数
  将原始轴型转换为通用轴型
  /************************************************************************/
  void AxleTransform(VehSet *tempVeh)
  {
	  if(g_QueSet.IsETC==1)
	  {
		  if(tempVeh->VPattern>4)//货车转客车
		  {
			  tempVeh->VPattern=VehClassfier(tempVeh->VehHeight,tempVeh->VehHeadHt);
		  }		 
	  }
	  //客车校验	  
	  switch(tempVeh->VPattern)
	  {
	  case 1:
		  tempVeh->AxlegroupNum  = 2;
		  tempVeh->TranAxleType=11;
		  tempVeh->AxleNum=2;
		  tempVeh->AxleType[0]=1;
		  tempVeh->AxleType[1]=1;
		  WriteLogFile("车型转换 ETC 客1修正");
		  break;
	  case 2:
		  break;
	  case 3:
		  
	  case 4:
		  tempVeh->AxlegroupNum  = 2;
		  tempVeh->TranAxleType=12;
		  tempVeh->AxleNum=2;
		  tempVeh->AxleType[0]=1;
		  tempVeh->AxleType[1]=2;
		  WriteLogFile("车型转换 ETC 非客1修正");
		  break;
	  default:	
		  break;
	  }
	  int TyreNum=0;//轮胎数量
	  int luntype=0;//原始轴型
	  //计算轮子数量
	  for(int i=0;i<tempVeh->AxleNum;i++)
	  {
		  if (tempVeh->AxleType[i] == 1)
		  {
			  TyreNum += 2;
		  }
		  else if (tempVeh->AxleType[i] == 2)
		  {
			  TyreNum += 4;
		  }
		  else
			  ;
		  luntype=luntype*10+tempVeh->AxleType[i];
	  }
	  //轴组数 默认轴型判断
	  switch(tempVeh->AxleNum)
	  {
	  case 2:
		  tempVeh->AxlegroupNum  = 2;	
		  if (tempVeh->VPattern== 1)
		  {
			  tempVeh->TranAxleType=11;
		  }
		  else if (TyreNum == 4)
		  {
			  tempVeh->TranAxleType=11;
		  }
		  else if (tempVeh->VPattern== 6)//所有2轴的货2 矫正为12 轴型
		  {
			  tempVeh->TranAxleType=12;
		  }
		  else
		  {
			  tempVeh->TranAxleType=12;
		  }
		  break;
	  case 3:
		  tempVeh->AxlegroupNum  = 3;
		  if (TyreNum==10&&tempVeh->AxleType[0]==2)
		  {
			  //20171114 修改212
			  tempVeh->TranAxleType=12;
			  tempVeh->VPattern=6;
		  }
		  else if(TyreNum == 10)
		  {
			  tempVeh->TranAxleType=122;
		  }
		  else if (TyreNum == 8 && tempVeh->VPattern == 4)
		  {
			  tempVeh->TranAxleType=121;
		  }
		  else
		  {
			  tempVeh->TranAxleType=112;
		  }
		  break;
	  case 4:
		  tempVeh->AxlegroupNum  = 3;
		  if(TyreNum == 10) // 1112
		  {
			  tempVeh->TranAxleType=114;					
		  }
		  else if(TyreNum == 14) //1222 
		  {
			  tempVeh->TranAxleType=125;	
		  }
		  else
		  {
			  tempVeh->TranAxleType=115;	//1122
		  }		
		  break;
	  case 5:
		  tempVeh->AxlegroupNum  = 3;
		  if (TyreNum == 16)
		  {
			  tempVeh->TranAxleType=1125;	
		  }
		  else if (TyreNum == 20)
		  {
			  tempVeh->TranAxleType=157;	
		  }
		  else
		  {
			   //20171114 修改22222
			  tempVeh->TranAxleType=127;	
			  //VD[cLastVeh].AxisType[0] = 1;//122 22
			  //VD[cLastVeh].AxisType[1] = 5;
			  //VD[cLastVeh].AxisType[2] = 5;
		  }
		  break;
	  case 6:
		  tempVeh->AxlegroupNum  = 3;
		  if(TyreNum ==  20)
		  {
			  tempVeh->TranAxleType=1127;	
		  }
		  else
		  {
			  tempVeh->TranAxleType=157;	
		  }
		  break;
	  default:
		  //				*AxisGroupNum = 2;
		  //				* AxisType=11;	
		  break;
	  }
	  //各省市特殊车型轴型修正
	  //*************一、江西省****************************//
	  if(g_QueSet.TypecheckID==1)
	  {
		  switch(tempVeh->AxleNum)
		  {
		  case 3:
			  if(luntype==111)
			  {
				  tempVeh->VPattern=6;
				  tempVeh->TranAxleType=13;
				  WriteLogFile("车型转换 江西111货2");
			  }
			  else if(luntype==122)//江西协议 122判为货4
			  {
				  tempVeh->VPattern=8;
				  WriteLogFile("车型转换 江西122货4");
			  }
			  break;
		  case 4:
			  if(luntype==1211)
			  {
				  tempVeh->TranAxleType=123;
				  WriteLogFile("车型转换 江西1211货4 轴型123");
			  }	
			  else if(luntype==1122)
			  {
				  tempVeh->VPattern=9;
				  WriteLogFile("车型转换 江西1122 货5");
			  }
			  else if(luntype==1212)
			  {
				  tempVeh->VPattern=9;
				  tempVeh->TranAxleType=124;
				  WriteLogFile("车型转换 江西1212 货5");
			  }
			  else if(luntype==1111)
			  {
				  tempVeh->VPattern=9;
				  tempVeh->TranAxleType=16;
				  WriteLogFile("车型转换 江西1111 货5 轴型16");
			  }
			  else if (luntype==1112)
			  {
				  tempVeh->VPattern=8;
				  tempVeh->TranAxleType=114;
				  WriteLogFile("车型转换 江西1112 货4 轴型114");
			  }
			  else
				  tempVeh->VPattern=9;
			  break;
		  }		
	  }
}

/************************************************************************/
/* 客车分类器（车高）

  /************************************************************************/
  int VehClassfier(int VehHeight,int HeadHeight)
  {
	  
	  int Vehtype=1;//返回车型 
	  if (VehHeight> 3200)  //车高
	  {
		  //客4
		  Vehtype=4;
	  }
	  else if (HeadHeight > 2600)
	  {
		  if (HeadHeight < 2000) //车头高较低
		  {
			  //客2
			  Vehtype=2;
		  }
		  else
		  {
			  //客3
			  Vehtype=3;
		  }
		  
	  }
	  else if (VehHeight > 2000)
	  {
		  if (HeadHeight >1250)
		  {
			  //客2
			  Vehtype=2;
		  }
		  else
		  {
			  //客1
			  Vehtype=1;
		  }
		  
	  }
	  else
	  {
		  //客1或客2
		  Vehtype=1;
	  }
	  return 	Vehtype;
  }
  /************************************************************************/
  /* 智能检查队列
  1、如果进货5，清空前面所有车辆
  2、如果前面队列有1辆货5，清掉当前车辆
  3、超过最大车辆数 当前车清除
  /************************************************************************/
  void VehQueCheck(void)
  {
	  //	 QueCheckTime = 0;    //计时开始，超时检查队列
	  if(g_QueSet.Quecheck==1)
	  {
		  //如果进货5，清空前面所有车辆
		  if ((g_Vehqueue[g_VehNum - 1].VPattern == 9) && (g_VehNum > 0))
		  {
			  while (g_VehNum-1)
			  {
				  //清除头车			 
				  WriteLogFile("队列维护 收到货5 清除头车");
				  CleanHeadVeh();
			  }
		  }
		  //队列维护，如果前面队列有1辆货5，清掉当前车
		  if ((g_VehNum > 1) && (g_Vehqueue[g_VehNum - 2].VPattern == 9))
		  {
			  WriteLogFile("队列维护 前面有货5 直接删除");
			  CleanEndVeh();
		  }
		  //队列维护，队列不能超过最大值，此时进车还没有队列加1，所以是MaxVehCnt - 1
		  if (g_VehNum > (g_QueSet.MaxVehCnt - 1))
		  {
			  WriteLogFile("队列维护 不能超过最大值 删除当前车");
			  CleanEndVeh();
		  }
	  }
	  
  }
  /************************************************************************/
  /* 清除头车
  1、清除头车信息结构体
  2、总数减一
  3、队列一次迁移
  /************************************************************************/
  AVIMAIN bool __stdcall  CleanHeadVeh(void)
  {
	  char LogBuf[255]={0};
	  if (g_VehNum > 0)
	  {
		  //写日志
		  sprintf(LogBuf, "删除头车 ID:%d,队列总数%d", g_Vehqueue[0].VehId, g_VehNum-1);
		  WriteLogFile(LogBuf);
		  memset(&g_Vehqueue[0], 0, sizeof(g_Vehqueue[0]));//清除头车车辆信息
		  g_VehNum--;
		  //清除之后队列还有车 则一次往前挪一位				 
		  for (int i = 0; i < g_VehNum; i++)
		  {
			  g_Vehqueue[i] = g_Vehqueue[i + 1];
		  }
		  memset(&g_Vehqueue[g_VehNum], 0, sizeof(g_Vehqueue[0]));//清除尾车车辆信息
		  return 0;
	  }
	  else
	  {
		  WriteLogFile("队列无车 删除头车失败");
		  return 1;
	  }
	  //无车 清除失败
  }
  /************************************************************************/
  /* 造车 头车 客1
  1、清除头车信息结构体
  2、总数减一
  3、队列一次迁移
  /************************************************************************/
  bool CreateHeadVeh(void)
  {
	  char LogBuf[255]={0};
	  VehSet  tempVeh;
	  char noplate[20]="无牌车";
	  memset(&tempVeh,0, sizeof(tempVeh));//初始化临时车辆结构体
	  //队列依次后挪一位 腾出队列头				 
	  for (int i = g_VehNum; i >0; i--)
	  {
		  g_Vehqueue[i] = g_Vehqueue[i - 1];
	  }
	  //造车 客1
	  tempVeh.VPattern=1;
	  tempVeh.AxleNum=2;
	  tempVeh.AxleType[0]=1;
	  tempVeh.AxleType[1]=1;
	  tempVeh.VSpeed=0;//
	  tempVeh.VehId=-2;//
	  memcpy(tempVeh.Plate,noplate,sizeof(noplate));
	  g_VehNum++;
	  //加入队列头
	  memcpy(&g_Vehqueue[0],&tempVeh,sizeof(tempVeh));
	  
	  sprintf(LogBuf, "造--车 车辆ID%d,车牌%s,车型%d,轴数%d,出车类型：造车,队列总数%d",
		  tempVeh.VehId, tempVeh.Plate,tempVeh.VPattern, tempVeh.AxleNum,g_VehNum);
	  WriteLogFile(LogBuf);
	  
	  return 0;
  }
  /************************************************************************/
  /* 清除当前车
  1、清除当前车信息结构体
  2、总数减一
  /************************************************************************/
  bool CleanEndVeh(void)
  {
	  char LogBuf[255]={0};
	  if (g_VehNum > 0)
	  {
		  //写日志
		  sprintf(LogBuf, "删除尾车 ID:%d,队列总数%d", g_Vehqueue[g_VehNum - 1].VehId, g_VehNum - 1);
		  WriteLogFile(LogBuf);
		  memset(&g_Vehqueue[g_VehNum-1], 0, sizeof(g_Vehqueue[0]));//清除尾车辆信息
		  g_VehNum--;
		  return 0;
	  }
	  else
	  {
		  WriteLogFile("队列无车 删除尾车失败");
		  return 1;
	  }
	  //无车 清除失败
  }
  /************************************************************************/
  /* 清除队列
  1、清除缓存信息结构体
  2、总数为0
  /************************************************************************/
  AVIMAIN bool __stdcall CleanQueVeh(void)
  {
	  if (g_VehNum > 0)
	  {
		  //写日志
		  memset(&g_Vehqueue, 0, sizeof(g_Vehqueue));
		  g_VehNum = 0;
		  WriteLogFile("队列缓存 清空完成");
		  return 0;
	  }
	  else
	  {
		  WriteLogFile("队列缓存 已清空");
	  }
	  return 1;
	  //无车 清除失败
  }
  
  /**************************************
  BOOL _stdcall EndVeh()
  功能： 此函数是当超长车到达取卡处时，车尾还没通过分型器，因此队列是空的。
  上位机可以发送此命令，强行结束这辆车的通行，将其车型放入队列中。
  因此，本命令成功后，队列数一定是1. 收尾成功的条件是队列里面没有车，车辆通行中，已经通过的轴数至少有4 个轴。
  强行收尾的车，取卡前行完全通过光幕区后，不会再将车型加入队列（已经提前加入了）。
  返回值： TRUE 为收尾成功，此时队列里面一定是1 辆车。FALSE 为失败。
  **************************************/
  AVIMAIN int _stdcall WJ_EndVeh(void)
  {
	  //	else//队列无车 强制收尾
	  //	{
	  //		if(strlen(CameraInfo0)>6)//长度大于6 表示肯定有车牌 进行匹配
	  //		{
	  //			if(platecheck(CameraInfo0,plate)>80)//车牌匹配成功 进行强制收尾
	  //
	  //				if(TrigForceOut()==1)//可以收尾
	  //				//此时应当出收尾车型 但车型从会调函数上来 不能立马出车
	  //
	  //		}
	  //				memset(CameraInfo0, 0, sizeof(CameraInfo0));
	  //	memset(tempVehSet,0,sizeof(g_Vehqueue[0]));	
	  //	}
	  //	if(g_QueSet.DelVehTime<0)//立即自动删除车辆
	  //	{
	  //		CleanHeadVeh();
	  //	}
	  int res=TrigForceOut();
	  WriteLogFile("强制收尾 开始");
	  if(res==0)//不能收尾 造车
	  {
		  WriteLogFile("不能收尾 造车客1");
		  CreateHeadVeh();
		  //	 return 0;
		  
	  }
	  else if(res==1)//可以收尾即将上传
	  {
		  ResetEvent(g_HEvent[1]);
		  WriteLogFile("可以收尾 即将上传");
		  if (TimerFVh!=NULL)
		  {
			  KillTimer(NULL,TimerFVh);
			  TimerFVh=0;
		  }
		  TimerFVh=SetTimer(NULL,3,g_QueSet.EndVehWaitTime,(TIMERPROC)TimerProc);
		  //return 1;
	  }
	  else
	  {
		  WriteLogFile("强制收尾 无返回 造车!");
		  CreateHeadVeh();
		  //	return -1;
		  
	  }
	  return 1;
  }
  
  
  /************************************************************************/
  /* 主控程序获取系统参数信息                                             */
  /************************************************************************/
  AVIMAIN SystemSet __stdcall GetMainSysSet(void)
  {
	  return g_SysTem;
  }
  
  /************************************************************************/
  /* 主控程序设置系统参数                                                 */
  /************************************************************************/
  AVIMAIN int __stdcall SetMainSysSet(SystemSet SysSet)
  {
	  int nCode=ERR_FAIL;
	  memcpy(&g_SysTem,&SysSet,sizeof(g_SysTem));
	  nCode=SetSysParam(g_SysTem);
	  return nCode;
  }
  
  
  /************************************************************************/
  /* 主控程序获取模型参数                                                 */
  /************************************************************************/
  AVIMAIN ModelSet __stdcall GetMainMdlSet(void)
  {
	  return g_ModelSet;
  }
  
  /************************************************************************/
  /* 主控程序设置模型参数                                                 */
  /************************************************************************/
  AVIMAIN int __stdcall SetMainMdlSet(ModelSet Mdl)
  {
	  int nCode=ERR_FAIL;
	  memcpy(&g_ModelSet,&Mdl,sizeof(ModelSet));
	  nCode=SetModelParam(g_ModelSet);
	  return nCode;
  }
  
  /************************************************************************/
  /* 主控程序获取出车参数                                                 */
  /************************************************************************/
  AVIMAIN VOutSet __stdcall GetMainVoutSet(void)
  {
	  return g_VOutSet;
  }
  
  
  
  /************************************************************************/
  /* 主控程序设置出车参数                                                 */
  /************************************************************************/
  AVIMAIN int __stdcall SetMainVoutSet(VOutSet VoSet)
  {
	  int nCode=ERR_FAIL;
	  memcpy(&g_VOutSet,&VoSet,sizeof(g_VOutSet));
	  nCode=SetVoutParam(g_VOutSet);
	  return nCode;
  }
  
  /************************************************************************/
  /* 获取主控激光器参数和                                                 */
  /************************************************************************/
  AVIMAIN LaserSet __stdcall GetMainLaserSet(void)
  {
	  return g_LaserSet;
  }
  
  
  /************************************************************************/
  /* 设置主控激光器参                                                     */
  /************************************************************************/
  AVIMAIN int __stdcall SetMainLaserSet(LaserSet Ls)
  {
	  int nCode=ERR_SUCC;
	  memcpy(&g_LaserSet,&Ls,sizeof(g_LaserSet));
	  return nCode;
  }
  
  /************************************************************************/
  /* 主控程序写日志                                                       */
  /************************************************************************/
  AVIMAIN void __stdcall Avi_WriteLog(char* msg)
  {
	  WriteLogFile(msg);
  }
  
  
  /************************************************************************/
  /* 初始化算法引擎                                                       */
  /************************************************************************/
  AVIMAIN int __stdcall InitEngine(void)
  {
	  
	  uint16 nRet=ERR_FAIL;
	  //初始化模型参数
	  nRet=SetModelParam(g_ModelSet);
	  if (nRet!=ERR_SUCC)
	  {
		  return nRet;
	  }
	  
	  nRet=SetSysParam(g_SysTem);
	  if (nRet!=ERR_SUCC)
	  {
		  return nRet;
	  }
	  
	  nRet=SetVoutParam(g_VOutSet);
	  if (nRet!=ERR_SUCC)
	  {
		  return nRet;
	  }
	  
	  
	  InitGlobal();
	  
	  nRet=StartEngine();
	  
	  return nRet;
  }
  /************************************************************************/
  /* 重新加载引擎                                                         */
  /************************************************************************/
  AVIMAIN uint8 __stdcall ReLoadEngine(void)
  {
	  int nRet=ERR_FAIL;
	  InitGlobal();
	  nRet=StartEngine();
	  return nRet;
  }
  
  /************************************************************************/
  /* 关闭算法引擎                                                         */
  /************************************************************************/
  AVIMAIN uint8 __stdcall ExitEngine(void)
  {
	  int nRet=ERR_FAIL;
	  nRet=InitGlobal();
	  return nRet;
  }
  
  /************************************************************************/
  /* 主控程序注册出车回调函数                                             */
  /************************************************************************/
  AVIMAIN int __stdcall RegMainVReport(PFReportVh pfReportVh)
  {
	  int nRet=ERR_FAIL;
	  if (pfReportVh!=NULL)
	  {
		  g_pfMainVeh=pfReportVh;
		  nRet=ERR_SUCC;
	  }
	  else
	  {
		  nRet=ERR_PFVEH;
	  }
	  return nRet;
  }
  
  
  /************************************************************************/
  /* 主控程序取扫描图像回调函数                                           */
  /************************************************************************/
  AVIMAIN int __stdcall RegMainPlotIm(PFPlotIm pfPlotIm)
  {
	  int nRet=ERR_FAIL;
	  if (pfPlotIm!=NULL)
	  {
		  g_pfPlotIm=pfPlotIm;
		  nRet=ERR_SUCC;
	  }
	  else
	  {
		  nRet=ERR_PFPLT;
	  }
	  return nRet;
  }
  
  
  /************************************************************************/
  /* 获取统计分析路径                                                     */
  /************************************************************************/
  void GetStatRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\Stat\\");
  }
  
  /************************************************************************/
  /* 获取系统存储路径                                                    */
  /************************************************************************/
  void GetSyRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\");
  }
  
  /************************************************************************/
  /* 获取Wave存储路径                                                    */
  /************************************************************************/
  void GetWaveRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\Wave\\");
  }
  
  /************************************************************************/
  /* 获取Image存储路径                                                    */
  /************************************************************************/
  void GetImageRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\PLot\\");
  }
  
  /************************************************************************/
  /* 获取WJLog存储路径                                                    */
  /************************************************************************/
  void GetWJLogRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\Log\\");
  }
  /************************************************************************/
  /* 获取MDB存储路径                                                    */
  /************************************************************************/
  void GetMDBRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\MDB\\");
  }
  /************************************************************************/
  /* 获取相机存储路径                                                    */
  /************************************************************************/
  void GetCameraRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\CAM\\");
  }
  /************************************************************************/
  /* 检查目录                                                             */
  /************************************************************************/
  void CheckRunDirectory(void)
  {
	  CFileFind finder;
	  char TmpPath[1024]={0};
	  GetSyRoot(TmpPath);//创建根目录
	  if (!finder.FindFile(TmpPath))
	  {
		  CreateDirectory(TmpPath,NULL);
	  }
	  
	  GetImageRoot(TmpPath);
	  if (!finder.FindFile(TmpPath))
	  {
		  CreateDirectory(TmpPath,NULL);
	  }
	  GetWJLogRoot(TmpPath);
	  if (!finder.FindFile(TmpPath))
	  {
		  CreateDirectory(TmpPath,NULL);
	  }
	  GetWaveRoot(TmpPath);
	  if (!finder.FindFile(TmpPath))
	  {
		  CreateDirectory(TmpPath,NULL);
	  }
	  GetMDBRoot(TmpPath);
	  if (!finder.FindFile(TmpPath))
	  {
		  CreateDirectory(TmpPath,NULL);
	  }
	  GetCameraRoot(TmpPath);
	  if (!finder.FindFile(TmpPath))
	  {
		  CreateDirectory(TmpPath,NULL);
	  }
	  GetStatRoot(TmpPath);
	  if (!finder.FindFile(TmpPath))
	  {
		  CreateDirectory(TmpPath,NULL);
	  }
	  finder.Close();
  }
  
  
  ///**************************************
  //重庆车型识别动态库接口函数部分20170921
  //**************************************/
  //AVIMAIN bool _stdcall Model_InitDevice(HWND hHandle, int nMsg, int nPorType, char* cPortName, long lBaudrate);
  //AVIMAIN bool _stdcall Model_CloseDevice(void);
  //AVIMAIN bool _stdcall Model_Getinfo(int *nCarorTruck, int *nModel);
  //AVIMAIN bool _stdcall Model_GetState(int *nState);
  // AVIMAIN bool _stdcall Model_Capture(void);
  
  
  
  /**************************************
  重庆车型识别动态库接口函数部分20170921
  初始化函数 主要接收 句柄和消息号
  **************************************/
  HWND g_hHandle = NULL;//车道软件的句柄
  int g_nMsg = 0;//车道软件的消息号
  AVIMAIN bool _stdcall Model_InitDevice(HWND hHandle, int nMsg, int nPorType, char* cPortName, long lBaudrate)
  {
	  g_hHandle = hHandle;
	  g_nMsg = nMsg;
	  int err=0;
	  err=Avi_Init();
	  //调用内部初始化函数 算法、队列、激光器、摄像头、
	  return true;
  }
  /**************************************
  重庆车型识别动态库接口函数部分20170921
  关闭退出
  **************************************/
  AVIMAIN bool _stdcall Model_CloseDevice(void)
  {
	  //主要关闭设备：算法、队列线程、摄像机、激光器
	  Avi_Close();//已关闭 算法引擎  队列线程
	  return true;
  }
  /**************************************
  重庆车型识别动态库接口函数部分20170921
  获取车型信息  客/货+车型
  **************************************/
  AVIMAIN bool _stdcall Model_Getinfo(int *nCarorTruck, int *nModel)
  {
	  VehSet  tempVehSet;
	  char LogBuf[255]={0};
	  GetVehicle(&tempVehSet,CameraInfo1);//
	  if((tempVehSet.VPattern<5)&&(tempVehSet.VPattern>0))
	  {
		  *nCarorTruck=1;
		  *nModel=tempVehSet.VPattern;
		  sprintf(LogBuf, "发卡机取车 ID:%d  客%d  队列总数%d",tempVehSet.VehId, *nModel, g_VehNum);
		  WriteLogFile(LogBuf);
		  
	  }
	  else if((tempVehSet.VPattern>4)&&(tempVehSet.VPattern<10))
	  {
		  *nCarorTruck=2;
		  *nModel=tempVehSet.VPattern-4;
		  sprintf(LogBuf, "发卡机取车 ID:%d  货%d  队列总数%d",tempVehSet.VehId, *nModel, g_VehNum);
		  WriteLogFile(LogBuf);
	  }
	  else
	  {
		  *nCarorTruck=1;
		  *nModel=1;
		  sprintf(LogBuf, "发卡机取车异常 ID:%d  Veh%d  队列总数%d",tempVehSet.VehId, tempVehSet.VPattern, g_VehNum);
		  WriteLogFile(LogBuf);
	  }
	  
	  return true;
  }
  /**************************************
  重庆车型识别动态库接口函数部分20170921
  获取状态
  **************************************/
  AVIMAIN bool _stdcall Model_GetState(int *nState)
  {
	  *nState=0;
	  WriteLogFile("获取状态");
	  return true;
  }
  /**************************************
  重庆车型识别动态库接口函数部分20170921
  模拟取车  有车时回复消息
  **************************************/
  AVIMAIN bool _stdcall Model_Capture(void)
  {
	  if (g_VehNum > 0)
	  {
		  //g_hHandle//消息窗口
		  //g_nMsg//消息ID 
		  //发消息到窗口
		  WriteLogFile("队列有车，向上发消息");
		  //		::SendMessage(g_hHandle, WM_USER + g_nMsg, 1, 0);
		  ::PostMessage(g_hHandle, g_nMsg, 1, 0);
		  //返回TRUE
		  return TRUE;
	  }
	  else//强制收尾
	  {
		  int ret = WJ_EndVeh();
		  if (ret == -1)
		  {
			  //发消息到窗口
			  WriteLogFile("队列无车 收尾失败不发消息");
			  //			::PostMessage(g_hHandle, g_nMsg, 1, 0);//测试用 实际使用应当屏蔽
			  return FALSE;   //收尾不成功，直接返回0
		  }
		  else
		  {
			  WriteLogFile("队列无车 收尾并发消息");
			  ::PostMessage(g_hHandle, g_nMsg, 1, 0);
			  //发消息到窗口
			  return TRUE;
		  }
		  
	  }
	  
	  return true;
  }
  
  /**************************************
  万集福州车型识别动态库接口20171012
  **************************************/
  //AVIMAIN int  _stdcall  WjVr_FlushQueue(void);
  //AVIMAIN bool _stdcall  WjVr_GetCoID(int* CoID, int* idenType, PSTR pchType, PSTR pchVers);
  //AVIMAIN bool _stdcall  WjVr_ClearOne(void);
  //AVIMAIN bool _stdcall  WjVr_GetVehicleData(int * AxisGroupNum, int* AxisNum, BYTE* VehicleType, long* AxisWeight, int* AxisType, int* Speed, BYTE* iLeiXing);
  //AVIMAIN int  _stdcall  WjVr_GetState(void);
  //AVIMAIN bool _stdcall  WjVr_InitDevice(int nPorType, char*cPortName, long lBaudrate);
  //AVIMAIN int  _stdcall  WjVr_GetVehicleCount(void) ;
  //AVIMAIN int  _stdcall  WjVr_EndVehicle(void);
  //AVIMAIN bool _stdcall  WjVr_Close(void);
  //AVIMAIN bool _stdcall  WjVr_GetPcl(uint16 * PclData, long *Pclsize);
  
  /**************************************
  万集福州车型识别动态库接口20171012
  初始化函数
  **************************************/
  AVIMAIN bool _stdcall  WjVr_InitDevice(int nPorType, char*cPortName, long lBaudrate)
  {
	  int err=0;
	  err=Avi_Init();
	  //调用内部初始化函数 算法、队列、激光器、摄像头、
	  return true;
  }
  /**************************************
  万集福州车型识别动态库接口20171012
  获取状态
  **************************************/
  AVIMAIN int  _stdcall  WjVr_GetState(void)
  {
	  WriteLogFile("获取状态");
	  return 0;
  }
  /**************************************
  万集福州车型识别动态库接口20171012
  清除头车
  **************************************/
  AVIMAIN bool _stdcall  WjVr_ClearOne(void)
  {
	  CleanHeadVeh();
	  return true;
  }
  /**************************************
  万集福州车型识别动态库接口20171012
  获取车辆数
  **************************************/
  AVIMAIN int  _stdcall  WjVr_GetVehicleCount(void)
  {
	  int num=0;
	  num=Avi_VehCount();
	  return num;
  }
  /**************************************
  万集福州车型识别动态库接口20171012
  获取车型数据
  **************************************/
  AVIMAIN bool _stdcall  WjVr_GetVehicleData(int * AxisGroupNum, int* AxisNum, 
	  BYTE* VehicleType, long* AxisWeight, int* AxisType, int* Speed, BYTE* iLeiXing,char* plate)
  {
	  VehSet  tempVehSet;
	  int TyreNum=0;//轮胎数量
	  int luntype=0;//原始轴型
	  char LogBuf[255]={0};
	  memset(&tempVehSet,0,sizeof(tempVehSet));//初始化车辆信息结构体
	  GetVehicle(&tempVehSet,plate);//获取车型信息结构体
	  *AxisNum=tempVehSet.AxleNum;//轴数
	  *AxisGroupNum  = tempVehSet.AxlegroupNum;	
	  *AxisType=tempVehSet.TranAxleType;
	  *VehicleType = tempVehSet.VPattern;//车型
	  *AxisWeight = 0;//
	  *Speed = tempVehSet.VSpeed;//
	  *iLeiXing = 0;
	  for(int i=0;i<tempVehSet.AxleNum;i++)
	  {
		  luntype=luntype*10+tempVehSet.AxleType[i];
	  }
	  
	  sprintf(LogBuf, "上传车型 ID:%d  车牌:%s 车型:%d 轴数:%d 轴组数:%d 原始轴型:%d，转换轴型:%d ",
		  tempVehSet.VehId,tempVehSet.Plate,tempVehSet.VPattern,tempVehSet.AxleNum,*AxisGroupNum,luntype,* AxisType);
	  WriteLogFile(LogBuf);
	  if(plate!=NULL)//凤凰山测试用 显示取车车牌
	  {
		  memcpy(plate,tempVehSet.Plate,sizeof(tempVehSet.Plate));
	  }
	  
	  return true;
  }
  /**************************************
  万集福州车型识别动态库接口20171012
  强制收尾
  **************************************/
  AVIMAIN int  _stdcall  WjVr_EndVehicle(void)
  {
	  int ret = WJ_EndVeh();
	  return ret; 
  }
  /**************************************
  万集福州车型识别动态库接口20171012
  关闭设备
  **************************************/
  AVIMAIN bool _stdcall  WjVr_Close(void)
  {
	  //主要关闭设备：算法、队列线程、摄像机、激光器
	  Avi_Close();//已关闭 算法引擎  队列线程
	  return true;
  }
  
  /**************************************
  万集福州车型识别动态库接口20171012
  获取PCL 点云
  **************************************/
  AVIMAIN bool _stdcall  WjVr_GetPcl(uint16 * PclData, long *Pclsize)
  {
	  *PclData = 0;
	  *Pclsize = 0;
	  return true;
  }
  
  /**************************************
  万集福州车型识别动态库接口20171012
  获取厂商信息
  **************************************/
  AVIMAIN int  _stdcall  WjVr_FlushQueue(void)
  {
	  CleanQueVeh();
	  return g_VehNum;
  }
  
  /**************************************
  万集福州车型识别动态库接口20171012
  获取厂商信息
  **************************************/
  AVIMAIN bool _stdcall  WjVr_GetCoID(int* CoID, int* idenType, PSTR pchType, PSTR pchVers)
  {
	  
	  *CoID = 1;
	  *idenType = 2;
	  char type[] = "WDL-864-892";
	  char Ver[] = "WJ_CKJND20171012";
	  memcpy(pchType, type, 20);
	  memcpy(pchVers, Ver, 20);
	  return true;
  }
  
  
  
  
  
  
  
  
  

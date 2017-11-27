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
/*ȫ�ֱ�����ض���                                                      */
/************************************************************************/
VehSet     g_tempVehSet;	//������Ϣ
ModelSet   g_ModelSet;      //�㷨ģ�Ͳ���
LaserSet   g_LaserSet;      //�������Ӳ���
SystemSet  g_SysTem;        //ϵͳ���� ���򡢵�˫����
VOutSet    g_VOutSet;       //������ֵ����
WvFiles    g_WvFiles;       //ģ�Ⲩ��
AqcState   g_AqcState;      //����״̬
CLmdProc   g_LmdProc;       //LMD�����������
CJGProxy   g_JGProxy;       //JG�豸�������

QueSet     g_QueSet;        //���й������
CamSet     g_CamSet;        //����������
SvWave     g_SvWave;        //�洢�������


PFReportVh g_pfMainVeh=NULL;
PFPlotIm   g_pfPlotIm=NULL;
int g_NoPlateNum=0;//����Ϊ�մ���
int g_CamReconNum=0;//�����������
/************************************************************************/
/*�����߳���ض���                                                      */
/************************************************************************/
HANDLE g_HEvent[4]={NULL};
HANDLE g_HdMEvent=NULL;
BOOL   g_bMEvent=FALSE;
VehSet g_Vehqueue[255];  //�������л���
UINT  TimerQue = NULL;//��ʱ����м�ʱ��
//����ȫ�ֱ�������
int g_VehNum = 0;//���г�������
int g_Queuethred=1;//���д����̱߳�־λ

/************************************************************************/
/*��ȡLMD/LMDS�����߳�                                                  */
/************************************************************************/
DWORD WINAPI WaveThreadProc(LPVOID lpParam);
HANDLE g_WaveHd= NULL;//�����߳̾��
BOOL g_bWaveSW=FALSE;//�ɼ����������߳�
UINT TimerAqc=NULL;//�ɼ����ζ�ʱ��
UINT TimerAUT=NULL;//��Ȩ��֤��ʱ��
UINT TimerFVh=NULL;//ǿ����β��ʱ��


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
���ܣ�0/���Ժ���
������
���أ�TRUE Ϊ�ɹ���FALSE Ϊʧ��
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
	//	WriteLogFile("Hello");//дϵͳ��־
	//	RegPFReportV(ReportVh);//ע��ص�����
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
/* 1/����ص���������
//��ȡ������Ϣ
/************************************************************************/
int ReportVh(VehSet Veh)
{
	char LogBuf[255]={0};
	int i=0;
	//g_tempVehSet = Veh;//ȡ��������Ϣ
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
/* ����ͨ���ص�����							                            */
/************************************************************************/
int ReportAx(int AxType)
{
	
	
	
	return ERR_SUCC;
}


/************************************************************************/
/* ����ͨ��״̬�ص����� 0--������1--�պ���β                            */
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
/* ����ͨ����ͼ������						                            */
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
/* ����ͨ��״̬�ص����� 0--������1--�պ���β                            */
/************************************************************************/
int CALLBACK FormImage(int FrIdx,int *pX,int *pZ,int MaxH)
{
	IntoRows(FrIdx,pX,pZ,MaxH);
	return ERR_SUCC;
}

/**************************************
���ܣ�2/ϵͳ��ʼ������
�������� ȫ���������ļ�������λ����ȡ����
���أ�0 Ϊ�ɹ������� Ϊʧ��
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
/* ��ʼ����ͼ���漰����                                                 */
/************************************************************************/
int InitPlotEnVi(void)
{
	ClearPlot();
	return ERR_SUCC;
}

/************************************************************************/
/* ��ʼ�����ݿ�洢                                                     */
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
/*��ʼ��ȫ�ֲ���                                                        */
/************************************************************************/
void ClearGlobal(void)
{
	//������ò���
	memset(&g_ModelSet, 0, sizeof(g_ModelSet));
	memset(&g_SysTem, 0, sizeof(g_SysTem));
	memset(&g_VOutSet, 0, sizeof(g_VOutSet));
	memset(&g_CamSet,0,sizeof(g_CamSet));
	memset(&g_QueSet,0,sizeof(g_QueSet));
	memset(&g_WvFiles,0,sizeof(g_WvFiles));
	memset(&g_SvWave,0,sizeof(g_SvWave));
}

/************************************************************************/
/*����ini�ļ�����                                                       */
/************************************************************************/
AVIMAIN void __stdcall LoadInISet(void)
{
	ReadAllPara();//��ȡ������ ��ȡ���ò���
	WriteLogFile("ReadIniFile");
}


/************************************************************************/
/*�����㷨����	                                                        */
/************************************************************************/
AVIMAIN int __stdcall StartAviAlgo(void)
{
	//1����ʼ���㷨ģ��*/
	int RetCode=ERR_FAIL;
	RetCode=InitEngine();//��ʼ���㷨����
	if (RetCode!=ERR_SUCC)
	{
		return RetCode;
	}
	RetCode=RegFormImage(FormImage);	//ע��ص�����
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
		//�رն�ʱ��
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
/*�������βɼ�����                                                      */
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
			g_JGProxy.m_Permited=true;//���ģ����Ȩͨ����
		}
		else
		{
			g_JGProxy.m_Permited=false;
			if (TimerAUT)
			{
				KillTimer(NULL,TimerAUT);
				TimerAUT=0;
			}
			TimerAUT = SetTimer(NULL, 3, 3000, (TIMERPROC)TimerProc);//3000ms��ʱ���м�������֤���
		}
		//������ʱ��
		if (TimerAqc)
		{
			KillTimer(NULL,TimerAqc);
		}
		TimerAqc = SetTimer(NULL, 2, 18, (TIMERPROC)TimerProc);//18ms��ʱ��ȡ����
		
	}
	else if (g_SysTem.RunMode==FILESIN)
	{
		if (g_WaveHd!=NULL)
		{
			CloseHandle(g_WaveHd);
		}
		g_bWaveSW=TRUE;
		g_WaveHd = CreateThread(NULL, 0, WaveThreadProc, NULL, 0, NULL);//�����߳�
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
/* ���ܵ����Ⲩ��                                                       */
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
				WriteLogFile("������֤ͨ��");
			}
		}
		else if ((BYTE)pDataBuf[9]==0xAE&&nDataBufSize==17)
		{
			g_JGProxy.SendBeatCmd();
			WriteLogFile("Ӧ��������");
		}
	}
	else
	{
		g_AqcState.RecWvErr++;
	}
}
/************************************************************************/
/* ��ȡ�����߳�                                                         */
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
			sprintf(LogBuf, "�߳��쳣 ��ȡ����");
			WriteLogFile(LogBuf);
		}
		
	}
	WriteLogFile("�ɼ������̹߳ر�");
	return ERR_SUCC;
}

/************************************************************************/
/*���벨��·��                                                          */
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
/* ��ʼ�����¼��������߳�                                               */
/************************************************************************/
int InitMutiEvent(void)
{
	if(g_HEvent[0]!=NULL)
	{
		CloseHandle(g_HEvent[0]);
		g_HEvent[0]=NULL;
	}
	g_HEvent[0]=CreateEvent(NULL, TRUE, FALSE, "IntoQue");//�����¼�������
	if(g_HEvent[1]!=NULL)
	{
		CloseHandle(g_HEvent[1]);
		g_HEvent[1]=NULL;
	}
	g_HEvent[1]=CreateEvent(NULL,TRUE,FALSE,"EndHandle");//����ǿ����β�¼�
	if(g_HEvent[2]!=NULL)
	{
		CloseHandle(g_HEvent[2]);
		g_HEvent[2]=NULL;
	}
	g_HEvent[2]=CreateEvent(NULL,TRUE,FALSE,"VehInto");//���ս���
	if(g_HEvent[3]!=NULL)
	{
		CloseHandle(g_HEvent[3]);
		g_HEvent[3]=NULL;
	}
	g_HEvent[3]=CreateEvent(NULL,TRUE,FALSE,"VehOut");//�����뿪
	if (g_HdMEvent!=NULL)
	{
		CloseHandle(g_HdMEvent);
		g_HdMEvent=NULL;
	}
	g_bMEvent=TRUE;
	g_HdMEvent=CreateThread(NULL, 0, ThreadMutiEvProc, NULL, 0, NULL);//�������¼������߳�
	if (g_HdMEvent=NULL)
	{
		g_bMEvent=FALSE;
		return ERR_FAIL;
	}
	return ERR_SUCC;	
}

/************************************************************************/
/* ���¼��������߳�                                                   */
/************************************************************************/
DWORD WINAPI ThreadMutiEvProc(LPVOID lpParam)
{
	DWORD nCode=0;
	char LogBuf[255]={0};
	char PlateColor[3]={0};//������ɫУ��
	char YellowPlate[3]="��";
	char BluePlate[3]  ="��";
	char checkplate[20]={0};
	while (g_bMEvent)//��ιر��߳�
	{
		nCode= WaitForMultipleObjects(4,g_HEvent,FALSE,INFINITE);
		
		if (nCode==WAIT_OBJECT_0)//�������
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
				//********************2������ת��*************************//
				AxleTransform(&g_tempVehSet);				//����ת�� ��������
				//********************1������У��*************************//
				memcpy(PlateColor,g_tempVehSet.Plate,2);
				if((strcmp(BluePlate,PlateColor)==0)&&(g_tempVehSet.VPattern>5)) //���� ��2����ת��1
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
					WriteLogFile("����ת�� ���� ��2-5ת��1");			
				}
				
				if((strcmp(YellowPlate,PlateColor)==0)&&(g_tempVehSet.VPattern==5))//���� ��1ת��2
				{
					g_tempVehSet.VPattern=6;
					WriteLogFile("����ת�� ���� ��1ת��2");
				}
				if((strcmp(YellowPlate,PlateColor)==0)&&(g_tempVehSet.VPattern==1))//���� ��1ת��2
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
						WriteLogFile("����ת�� ���� ��1ת��2");
					}
			
				}
					
				if((strcmp(BluePlate,PlateColor)==0)&&(g_tempVehSet.VPattern>2)&&(g_tempVehSet.VPattern<5))//���� ��3��4ת��2
				{
					g_tempVehSet.VPattern=2;
					WriteLogFile("����ת�� ���� ��3/4ת��2");			
				}
				if((strcmp(BluePlate,PlateColor)==0)&&(g_tempVehSet.VPattern==2))//���� ��2ת��1
				{
					g_tempVehSet.VPattern=1;
					g_tempVehSet.AxlegroupNum  = 2;
					g_tempVehSet.TranAxleType=11;
					g_tempVehSet.AxleNum=2;
					g_tempVehSet.AxleType[0]=1;
					g_tempVehSet.AxleType[1]=1;
					WriteLogFile("����ת�� ���� ��2ת��1");			
				}							
			}
			catch(...)
			{		
				//д��־ ������CException& e
				DWORD nErr=GetLastError();
				sprintf(LogBuf, "�߳��쳣1[%d] �������",nErr);
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
				//д��־ ������CException& e 
				DWORD nErr=GetLastError();
				sprintf(LogBuf, "�߳��쳣2[%d] �ص�������",nErr);
				WriteLogFile(LogBuf);
			}
			try
			{
				sprintf(LogBuf, "���ճ��ͣ�����ID%d,plate:%s,����%d,����%d,�������ͣ�%d",
					g_tempVehSet.VehId, g_tempVehSet.Plate,g_tempVehSet.VPattern, g_tempVehSet.AxleNum, g_tempVehSet.VoutFlag);
				WriteLogFile(LogBuf);
				memset(CameraInfo0, 0, sizeof(CameraInfo0));
				//		if (g_tempVehSet.VDir == 1)//�������� �������
				//		{ 
				//********************3���������*************************//
				g_VehNum++;
				memcpy(&g_Vehqueue[g_VehNum - 1],&g_tempVehSet,sizeof(g_tempVehSet));//�������
				//g_Vehqueue[g_VehNum - 1] = g_tempVehSet;	//�������
				
				sprintf(LogBuf, "������� ����ID%d,plate:%s,����%d,����%d,�������ͣ�%d,��������%d", 
					g_Vehqueue[g_VehNum - 1].VehId,g_Vehqueue[g_VehNum - 1].Plate, g_Vehqueue[g_VehNum - 1].VPattern, g_Vehqueue[g_VehNum - 1].AxleNum, g_Vehqueue[g_VehNum - 1].VoutFlag,g_VehNum);
				WriteLogFile(LogBuf);
				//********************4�����������ж�*************************//
				memcpy(checkplate,g_tempVehSet.Plate,sizeof(checkplate));
				CameraCheck(checkplate);
			}
			catch(...)
			{		
				//д��־ ������CException& e
				DWORD nErr=GetLastError();
				sprintf(LogBuf, "�߳��쳣3[%d] �������",nErr);
				WriteLogFile(LogBuf);
			}
			try
			{
				//********************5�����г����ж�*************************//
				if (g_VehNum > (g_QueSet.MaxVehCnt - 1))
				{
					if(strlen(checkplate)>6)//���Ʋ�Ϊ�� �ҳ����������ֵ ɾ��ͷ��
					{
						WriteLogFile("����ά�� ���Ʋ�Ϊ�� �ҳ����������ֵ ɾ��ͷ��");
						CleanEndVeh();
					}
					else
					{
						WriteLogFile("����ά�� ������Ч �ҳ����������ֵ ɾ����ǰ��");
						CleanHeadVeh();
					}
					
				}
				//	 VehQueCheck();
			}
			catch(...)
			{		
				//д��־ ������CException& e
				DWORD nErr=GetLastError();
				sprintf(LogBuf, "�߳��쳣4[%d] ���г����ж�",nErr);
				WriteLogFile(LogBuf);
			}
			
			
	}
	//	else if (g_tempVehSet.VDir == 2)//��ȫ����
	//	{
	//	  if (g_VehNum == 0)
	//	  {
	//		  //д��־ �޳����ֵ���
	//		  sprintf(LogBuf, "������� �����޳������ܵ���������ID%d,plate:%s,����%d,����%d,�������ͣ�%d,��������%d", 
	//			  g_tempVehSet.VehId, g_tempVehSet.Plate,g_tempVehSet.VPattern, g_tempVehSet.AxleNum, g_tempVehSet.VoutFlag,g_VehNum);
	//		  WriteLogFile(LogBuf);
	//		  return 1;
	//	  }
	//	  else
	//	  {
	//		  sprintf(LogBuf, "��ȫ���� �ó������У�����ID%d,plate:%s,����%d,����%d,�������ͣ�%d,��������%d",
	//			  g_tempVehSet.VehId, g_tempVehSet.Plate,g_tempVehSet.VPattern, g_tempVehSet.AxleNum, g_tempVehSet.VoutFlag,g_VehNum);
	//		  WriteLogFile(LogBuf);
	//		  CleanEndVeh();//ɾ����һ����
	//	  }				
	//	}
	//	else if (g_tempVehSet.VDir == 3)//����ȫ����������
	//	{
	//	  //д��־ ������
	//	  sprintf(LogBuf, "�뵹�����ó������У�����������ID%d,plate:%s,����%d,����%d,�������ͣ�%d,��������%d",
	//		  g_tempVehSet.VehId, g_tempVehSet.Plate,g_tempVehSet.VPattern, g_tempVehSet.AxleNum, g_tempVehSet.VoutFlag,g_VehNum);
	//	  WriteLogFile(LogBuf);
	//	}
	//	else
	
	//			  ;
	//		}
	else if (nCode==(WAIT_OBJECT_0+1))//ǿ����β��ʱ
	{
		try
		{
			WriteLogFile("ǿ����β û��Ӧ �쳵!");
			CreateHeadVeh();
			//����1
			// return -1;			
			ResetEvent(g_HEvent[1]);
		}
		catch(...)
		{		
			//д��־ ������CException& e
			DWORD nCode=GetLastError();
			sprintf(LogBuf, "�߳��쳣7[%d] ǿ����β��ʱ,nCode");
			WriteLogFile(LogBuf);
		}
		
	}
	else if (nCode==(WAIT_OBJECT_0+2))//�����ս���
	{
		try
		{
			OnCapture();
			ResetEvent(g_HEvent[2]);
		}
		catch(...)
		{		
			//д��־ ������CException& e
			DWORD nCode=GetLastError();
			sprintf(LogBuf, "�߳��쳣6[%d] �����ս���,nCode");
			WriteLogFile(LogBuf);
		}
	}
	else if (nCode==(WAIT_OBJECT_0+3))//������β
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
			//д��־ ������CException& e
			DWORD nCode=GetLastError();
			sprintf(LogBuf, "�߳��쳣5[%d] ������β,nCode");
			WriteLogFile(LogBuf);
		}
		
	}
	}
	WriteLogFile("���¼������߳̽���");
	return 0;
}


/************************************************************************/
/* ��ʼ���                                                             */
/************************************************************************/
//int InitCamera(void)
//{
//	//1.2����ʼ������ͷģ��*/
//
//	NET_DVR_Init();
//	DWORD FLAG=0;
//	int ret = 0;
//	//��������ʱ��������ʱ��
//	NET_DVR_SetConnectTime(2000, 1);
//	NET_DVR_SetReconnect(10000, true);
//
//	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
//	lUserID = NET_DVR_Login_V30((char *)CarmerIP.data(), 8000, "admin",(char *)CarmerPassWord.data(), &struDeviceInfo);
//	if (lUserID < 0)
//	{
//		FLAG=NET_DVR_GetLastError();
//		sprintf(log_buf,"��¼����ͷʧ�ܣ�����:%d",FLAG);
//		WriteLogFile(log_buf);
//		return FALSE;
//	}
//	WriteLogFile("��¼����ͷ�ɹ�");
//	//���ñ����ص�����
//	NET_DVR_SetDVRMessageCallBack_V30(MSesGCallback, NULL);
//
//	//���ò���
//	NET_DVR_SETUPALARM_PARAM struSetupParam={0};
//	struSetupParam.dwSize=sizeof(NET_DVR_SETUPALARM_PARAM);
//
//	//�ϴ�������Ϣ����: 0- �ϱ�����Ϣ(NET_DVR_PLATE_RESULT), 1- �±�����Ϣ(NET_ITS_PLATE_RESULT)
//	struSetupParam.byAlarmInfoType=1;
//	struSetupParam.byLevel=1;
//	lHandle = NET_DVR_SetupAlarmChan_V41(lUserID,&struSetupParam);
//	if (lHandle < 0)
//	{
//		FLAG=NET_DVR_GetLastError();
//
//		sprintf(log_buf,"����ͷ����ʧ�ܣ�����:%d",FLAG);
//		WriteLogFile(log_buf);
//		return FALSE;
//	}
//	WriteLogFile("����ͷ�����ɹ�");
//	return TRUE;
//}
//BOOL Closecarmer(void)
//{
//	DWORD FLAG=0;
//	//���������ϴ�ͨ��
//	if (!NET_DVR_CloseAlarmChan_V30(lHandle))
//	{
//		FLAG=NET_DVR_GetLastError();
//		NET_DVR_Logout(lUserID);
//		//NET_DVR_Cleanup();
//		return FALSE;
//	} 
//	//ע���û�
//	NET_DVR_Logout(lUserID);
//	//�ͷ� SDK ��Դ
//	//NET_DVR_Cleanup();
//	return TRUE;
// }

/**************************************
˵��������ͷ�ص�����
��������
����ֵ����
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
//			//	LogData.Format("���ƺ�: %s",CF_C_V12Dlg::struITSPlateResult_HC.struPlateInfo.sLicense);
//			//	switch(CF_C_V12Dlg::struITSPlateResult_HC.struPlateInfo.byColor)//������ɫ
//			//	{
//			//	case VCA_BLUE_PLATE:
//			//		LogData=LogData+" ������ɫ����ɫ";
//			//		break;
//			//	case VCA_YELLOW_PLATE:
//			//		LogData=LogData+" ������ɫ����ɫ";
//			//		break;
//			//	case VCA_WHITE_PLATE:
//			//		LogData=LogData+" ������ɫ����ɫ";
//			//		break;
//			//	case VCA_BLACK_PLATE:
//			//		LogData=LogData+" ������ɫ����ɫ";
//			//		break;
//			//	default:
//			//		break;
//			//	}
//
//			//	//���泡��ͼ
//			//	if ((CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].dwDataLen != 0)&&(CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].byType==
//			//		1)||(CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].byType == 2))
//			//	{
//			//		CF_C_V12Dlg::sPic(0,CF_C_V12Dlg::struITSPlateResult_HC,i);
//			//	}
//			//	//����СͼƬ
//			//	if ((CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].dwDataLen != 0)&&(CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].byType == 0))
//			//	{
//			//		CF_C_V12Dlg::sPic(1,CF_C_V12Dlg::struITSPlateResult_HC,i);
//			//	}
//
//			//	//������Ϣ����......
//			//}
//			//CF_C_V12Dlg::uploadLog("�豸1:1->�ϴ�������Ϣ");
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
/* ��ʱ������ִ����                                                     */
/************************************************************************/
void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT nIDEvent,DWORD dwTime)
{
	if (nIDEvent==NULL||nIDEvent==0)
	{
		return;
	}
	/************************************************************************/
	/* ��ʱ����ʱ �����г���ʱ �����
	/************************************************************************/
	else if (nIDEvent==TimerQue)
	{
		if (g_VehNum >0)
		{
			char LogBuf[255]={0};
			sprintf(LogBuf, "%dms δȡ��ɾ����,��������%d", g_QueSet.ClearQueTime, g_VehNum);
			WriteLogFile(LogBuf);
			CleanQueVeh();
		}
		KillTimer(NULL, TimerQue);
		TimerQue = SetTimer(NULL, 1, g_QueSet.ClearQueTime, (TIMERPROC)TimerProc);//����û�л�ȡ���ͼ������
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
			//����200��û�յ�������Ҫ��ָ��
			if ((g_JGProxy.m_WvLost>50)&&(g_JGProxy.m_WvLost<52))
			{	
				if (g_JGProxy.m_Net.bConnected)
				{
					//����4sÿ�յ����Σ��������ر�������
					g_JGProxy.m_Net.DisConnSvr();
					WriteLogFile("����1s����,�����Ͽ����ⷢ������");
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
	else if (nIDEvent==TimerFVh)//ǿ����β��ʱ
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
���ܣ�3/ϵͳ�ر�
�������� ȫ���������ļ�������λ����ȡ����
���أ�0 Ϊ�ɹ������� Ϊʧ��
**************************************/
AVIMAIN int __stdcall Avi_Close(void)
{
	ExitEngine();// �ر��㷨���� 
	g_Queuethred=0;//�رն��н����߳�
	// ������ͨ��
	DWORD ErrCode = 0;
	//		WJ_Closecarmer(&ErrCode);
	Closecarmer();
	//xд��־
	SaveAllPara();
	WriteLogFile("Avi_Close");
	return 0;
}

/**************************************
4/��ȡ�豸������
int  Avi_VehCount(void)
˵������ȡ����ʶ���豸��ǰ����������
�������ޣ�
����ֵ��
��0	�ܳ�����0Ϊû�г���
- 1 	ʧ�ܡ�
**************************************/
AVIMAIN int __stdcall Avi_VehCount(void)
{
	//д��־
	char LogBuf[255]={0};
	sprintf(LogBuf, "�������� :%d", g_VehNum);
	WriteLogFile(LogBuf);
	return g_VehNum;
}

/**************************************
���ܣ�5/��ȡ������Ϣ
������
[���]int AxisGroupNum		��������
[���]int AxisNum			������
[���]BYTE VehicleType  	����(1 - 11)��
��1 - 4 Ϊ ��1����2����3����4
5 - 11Ϊ ��1����2����3����4����5����6����7��
[���]long AxisWeight    	�����أ�Ԥ������
[���]int AxisType 		���ͣ�1 - 7����
[���]int Speed				���ٶȣ�
[���]BYTE iLeiXing 	0 - ����������1 - ������2 - ����˳������ݣ��ҽ�֮ǰ�յ��Ļ������е�����ȫ�������
���أ�0 Ϊ�ɹ������� Ϊʧ��
**************************************/
bool  GetVehicle(VehSet *tempVehSet,char* plate)
{
	char LogBuf[255]={0};
	char tempPlate1[20]={0};
	int platenum=-1;
	int likemost=0;//������
	int templike=0;
	char noplate[20]="���Ƴ�";
	
	/************************************************************************/
	/* ����ƥ���ģ�����	
	/************************************************************************/	
	char platesame[20]="111";//��ͷ��������ͬ
	char plate80[20]="888";//��ͷ���������ƶ�80
	char plate20[20]="222";//��ͷ���������ƶ�20
	char plateSecond[20]="333";//�ڶ���������ͬ
	
	
	if(plate==NULL)//ֻҪ��һ��Ϊ�� ����0
	{
		return 0;
	}
	else if(strlen(plate)==3)//���Գ��ƴ���
	{
		if(strcmp(plate,platesame)==0)//��ͷ��������ͬ
		{	
			memcpy(plate,g_Vehqueue[0].Plate,sizeof(tempPlate1));
			sprintf(LogBuf, "����ģ�� ��ͬ ģ�⳵��%s", plate);
			WriteLogFile(LogBuf);
		}
		else if(strcmp(plate,plateSecond)==0)//�����������ͬ
		{
			memcpy(plate,g_Vehqueue[1].Plate,sizeof(tempPlate1));
			sprintf(LogBuf, "����ģ�� ������ͬ ģ�⳵��%s", plate);
			WriteLogFile(LogBuf);
		}
		
		else if(strcmp(plate,plate80)==0)//��ͷ���������ƶ�80
		{
			for(int i=0;i<10;i++)
			{
				if((i==4)||(i==5))
					memcpy(plate+i,":",sizeof(char));	
				else
					plate[i]=g_Vehqueue[0].Plate[i];	
			}
			sprintf(LogBuf, "����ģ�� ͷ��80 ģ�⳵��%s, ͷ������:%s", plate,g_Vehqueue[0].Plate);
			WriteLogFile(LogBuf);
		}
		else if(strcmp(plate,plate20)==0)//��ͷ���������ƶ�20
		{
			for(int i=0;i<10;i++)
			{
				if((i==4)||(i==5))
					plate[i]=g_Vehqueue[0].Plate[i];
				else
					memcpy(plate+i,":",sizeof(char));	
			}
			sprintf(LogBuf, "����ģ�� ͷ��20 ģ�⳵��%s, ͷ������:%s", plate,g_Vehqueue[0].Plate);
			WriteLogFile(LogBuf);
		}
	}
	else 
	{
		
	}
	/************************************************************************/
	/* ����ƥ���ģ�����end	
	/************************************************************************/	
	sprintf(LogBuf, "��ȡ���� ����%s, ͷ������:%s", plate,g_Vehqueue[0].Plate);
	WriteLogFile(LogBuf);
	
	//1���ж϶����� 
	if (g_VehNum > 0)//�����г�
	{
		//����ƥ��
		for(int i=g_VehNum-1;i>=0;i--)//�Ӷ���β��ʼƥ��
		{
			memcpy(tempPlate1,g_Vehqueue[i].Plate,sizeof(tempPlate1));
			templike=platecheck(tempPlate1,plate);
			if((templike>=likemost)&&(templike>=80))//���ƶȴﵽ80����
			{
				platenum=i;//��¼ƥ����� ��ƥ��ĳ������
				likemost=templike;
			}
		}
		memcpy(tempPlate1,g_Vehqueue[0].Plate,sizeof(tempPlate1));
		if(platenum==-1)//����ƥ��ʧ��
		{
			if (
				(strcmp(noplate,tempPlate1))==0 
				||(strcmp(noplate,plate)==0)
				||(strlen(tempPlate1)==0)
				||(strlen(plate)==0)
				)
				//����ƥ��ʧ�� ���� ������һ���� ���Ƴ���ճ��� ��ʱ��ͷ��
			{
				memcpy(tempVehSet,&g_Vehqueue[0],sizeof(g_Vehqueue[0]));
				sprintf(LogBuf, "����ƥ�� ʧ�� ͷ�������Ƴ� ��ͷ��\n����ID%d,ȡ������%s,����%d,����%d,�������ͣ�%d,��������%d", 
					tempVehSet->VehId, plate,tempVehSet->VPattern, tempVehSet->AxleNum, tempVehSet->VoutFlag,g_VehNum);
				WriteLogFile(LogBuf);
				if(g_QueSet.DelVehTime<0)//�����Զ�ɾ������
				{
					CleanHeadVeh();
				}
				
			}	
			//����ƥ��ʧ�� ͷ���г���  ��ʱ�쳵����1  ���в���
			else//˫��������Ч���� ��ƥ��ʧ�� �쳵
			{
				tempVehSet->VPattern=1;
				tempVehSet->AxleNum=2;
				tempVehSet->AxleType[0]=1;
				tempVehSet->AxleType[1]=1;
				tempVehSet->VSpeed=0;//
				tempVehSet->VehId=-1;//
				memcpy(tempVehSet->Plate,noplate,sizeof(noplate));
				
				sprintf(LogBuf, "ƥ��ʧ�� ����ID%d,����%s,����%d,����%d,�������ͣ��쳵,��������%d,���ƶ�:%d",
					tempVehSet->VehId, tempVehSet->Plate,tempVehSet->VPattern, tempVehSet->AxleNum,g_VehNum,likemost);
				WriteLogFile(LogBuf);
			}
		}
		
		else //����ƥ��ɹ� ��ƥ��ɹ��ĳ���
		{
			memcpy(tempVehSet,&g_Vehqueue[platenum],sizeof(g_Vehqueue[platenum]));
			sprintf(LogBuf, "����ƥ�� %d ����ID%d,����%s,����%d,����%d,�������ͣ�%d,��������%d,���ƶ�:%d", platenum,
				tempVehSet->VehId, plate,tempVehSet->VPattern, tempVehSet->AxleNum, tempVehSet->VoutFlag,g_VehNum,likemost);
			WriteLogFile(LogBuf);
			for(int ii=0;ii<platenum;ii++)//����ά�� ����ƥ�䳵��֮ǰ�����г�����Ϣ
			{
				CleanHeadVeh();
			}
			if(g_QueSet.DelVehTime<0)//�����Զ�ɾ������
			{
				CleanHeadVeh();
			}
		}
	}
	else
	{
		WriteLogFile("�����޳� GetVehicle��������");
	}
	//	else//�����޳� ǿ����β
	//	{
	//		if(strlen(CameraInfo0)>6)//���ȴ���6 ��ʾ�϶��г��� ����ƥ��
	//		{
	//			if(platecheck(CameraInfo0,plate)>80)//����ƥ��ɹ� ����ǿ����β
	//
	//				if(TrigForceOut()==1)//������β
	//				//��ʱӦ������β���� �����ʹӻ���������� �����������
	//
	//		}
	//				memset(CameraInfo0, 0, sizeof(CameraInfo0));
	//	memset(tempVehSet,0,sizeof(g_Vehqueue[0]));	
	//	}
	//	if(g_QueSet.DelVehTime<0)//�����Զ�ɾ������
	//	{
	//		CleanHeadVeh();
	//	}
	//��ʱ ��ʱ��δȡ���������
	if (TimerQue)
	{
		KillTimer(NULL, TimerQue);
		TimerQue = 0;
		WriteLogFile("retimer");
	}
	TimerQue = SetTimer(NULL, 1, g_QueSet.ClearQueTime, (TIMERPROC)TimerProc);//����û�л�ȡ���ͼ������
	
	return 0;
}
/************************************************************************/
/* ����ƥ��ȼ���

  /************************************************************************/
  int platecheck(char* plate0,char* plate1)
  {
	  if((plate0==NULL)||(plate1==NULL))//ֻҪ��һ��Ϊ�� ����0
		  return 0;
	  if((strlen(plate0)==0)||(strlen(plate1)==0))//ֻҪ��һ��Ϊ�� ����0
		  return 0;
	  if(strcmp(plate0,plate1)==0)//������ȫƥ��
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
  /* ��������ͷ״̬
  1���������4��û�л�ȡ������ ������������ͷ
  /************************************************************************/
  void CameraCheck(char* plate)
  {
	  char LogBuf[255]={0};
	  if(strlen(plate)==0)//����Ϊ��
	  {
		  g_NoPlateNum++;
	  }
	  else
	  {
		  g_NoPlateNum=0;
	  }
	  if((g_NoPlateNum>10)/*&&(g_CamReconNum<10)*/)//����4�γ���Ϊ�� ������������ͷ
	  {
		  sprintf(LogBuf, "������� ����10�γ���Ϊ�� ��������%d", g_CamReconNum);
		  WriteLogFile(LogBuf);
		  Closecarmer();
		  g_NoPlateNum=0;
		  InitCamera();
		  g_CamReconNum++;
	  }
  }
  /************************************************************************/
  /* ����ת������
  ��ԭʼ����ת��Ϊͨ������
  /************************************************************************/
  void AxleTransform(VehSet *tempVeh)
  {
	  if(g_QueSet.IsETC==1)
	  {
		  if(tempVeh->VPattern>4)//����ת�ͳ�
		  {
			  tempVeh->VPattern=VehClassfier(tempVeh->VehHeight,tempVeh->VehHeadHt);
		  }		 
	  }
	  //�ͳ�У��	  
	  switch(tempVeh->VPattern)
	  {
	  case 1:
		  tempVeh->AxlegroupNum  = 2;
		  tempVeh->TranAxleType=11;
		  tempVeh->AxleNum=2;
		  tempVeh->AxleType[0]=1;
		  tempVeh->AxleType[1]=1;
		  WriteLogFile("����ת�� ETC ��1����");
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
		  WriteLogFile("����ת�� ETC �ǿ�1����");
		  break;
	  default:	
		  break;
	  }
	  int TyreNum=0;//��̥����
	  int luntype=0;//ԭʼ����
	  //������������
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
	  //������ Ĭ�������ж�
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
		  else if (tempVeh->VPattern== 6)//����2��Ļ�2 ����Ϊ12 ����
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
			  //20171114 �޸�212
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
			   //20171114 �޸�22222
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
	  //��ʡ�����⳵����������
	  //*************һ������ʡ****************************//
	  if(g_QueSet.TypecheckID==1)
	  {
		  switch(tempVeh->AxleNum)
		  {
		  case 3:
			  if(luntype==111)
			  {
				  tempVeh->VPattern=6;
				  tempVeh->TranAxleType=13;
				  WriteLogFile("����ת�� ����111��2");
			  }
			  else if(luntype==122)//����Э�� 122��Ϊ��4
			  {
				  tempVeh->VPattern=8;
				  WriteLogFile("����ת�� ����122��4");
			  }
			  break;
		  case 4:
			  if(luntype==1211)
			  {
				  tempVeh->TranAxleType=123;
				  WriteLogFile("����ת�� ����1211��4 ����123");
			  }	
			  else if(luntype==1122)
			  {
				  tempVeh->VPattern=9;
				  WriteLogFile("����ת�� ����1122 ��5");
			  }
			  else if(luntype==1212)
			  {
				  tempVeh->VPattern=9;
				  tempVeh->TranAxleType=124;
				  WriteLogFile("����ת�� ����1212 ��5");
			  }
			  else if(luntype==1111)
			  {
				  tempVeh->VPattern=9;
				  tempVeh->TranAxleType=16;
				  WriteLogFile("����ת�� ����1111 ��5 ����16");
			  }
			  else if (luntype==1112)
			  {
				  tempVeh->VPattern=8;
				  tempVeh->TranAxleType=114;
				  WriteLogFile("����ת�� ����1112 ��4 ����114");
			  }
			  else
				  tempVeh->VPattern=9;
			  break;
		  }		
	  }
}

/************************************************************************/
/* �ͳ������������ߣ�

  /************************************************************************/
  int VehClassfier(int VehHeight,int HeadHeight)
  {
	  
	  int Vehtype=1;//���س��� 
	  if (VehHeight> 3200)  //����
	  {
		  //��4
		  Vehtype=4;
	  }
	  else if (HeadHeight > 2600)
	  {
		  if (HeadHeight < 2000) //��ͷ�߽ϵ�
		  {
			  //��2
			  Vehtype=2;
		  }
		  else
		  {
			  //��3
			  Vehtype=3;
		  }
		  
	  }
	  else if (VehHeight > 2000)
	  {
		  if (HeadHeight >1250)
		  {
			  //��2
			  Vehtype=2;
		  }
		  else
		  {
			  //��1
			  Vehtype=1;
		  }
		  
	  }
	  else
	  {
		  //��1���2
		  Vehtype=1;
	  }
	  return 	Vehtype;
  }
  /************************************************************************/
  /* ���ܼ�����
  1���������5�����ǰ�����г���
  2�����ǰ�������1����5�������ǰ����
  3������������� ��ǰ�����
  /************************************************************************/
  void VehQueCheck(void)
  {
	  //	 QueCheckTime = 0;    //��ʱ��ʼ����ʱ������
	  if(g_QueSet.Quecheck==1)
	  {
		  //�������5�����ǰ�����г���
		  if ((g_Vehqueue[g_VehNum - 1].VPattern == 9) && (g_VehNum > 0))
		  {
			  while (g_VehNum-1)
			  {
				  //���ͷ��			 
				  WriteLogFile("����ά�� �յ���5 ���ͷ��");
				  CleanHeadVeh();
			  }
		  }
		  //����ά�������ǰ�������1����5�������ǰ��
		  if ((g_VehNum > 1) && (g_Vehqueue[g_VehNum - 2].VPattern == 9))
		  {
			  WriteLogFile("����ά�� ǰ���л�5 ֱ��ɾ��");
			  CleanEndVeh();
		  }
		  //����ά�������в��ܳ������ֵ����ʱ������û�ж��м�1��������MaxVehCnt - 1
		  if (g_VehNum > (g_QueSet.MaxVehCnt - 1))
		  {
			  WriteLogFile("����ά�� ���ܳ������ֵ ɾ����ǰ��");
			  CleanEndVeh();
		  }
	  }
	  
  }
  /************************************************************************/
  /* ���ͷ��
  1�����ͷ����Ϣ�ṹ��
  2��������һ
  3������һ��Ǩ��
  /************************************************************************/
  AVIMAIN bool __stdcall  CleanHeadVeh(void)
  {
	  char LogBuf[255]={0};
	  if (g_VehNum > 0)
	  {
		  //д��־
		  sprintf(LogBuf, "ɾ��ͷ�� ID:%d,��������%d", g_Vehqueue[0].VehId, g_VehNum-1);
		  WriteLogFile(LogBuf);
		  memset(&g_Vehqueue[0], 0, sizeof(g_Vehqueue[0]));//���ͷ��������Ϣ
		  g_VehNum--;
		  //���֮����л��г� ��һ����ǰŲһλ				 
		  for (int i = 0; i < g_VehNum; i++)
		  {
			  g_Vehqueue[i] = g_Vehqueue[i + 1];
		  }
		  memset(&g_Vehqueue[g_VehNum], 0, sizeof(g_Vehqueue[0]));//���β��������Ϣ
		  return 0;
	  }
	  else
	  {
		  WriteLogFile("�����޳� ɾ��ͷ��ʧ��");
		  return 1;
	  }
	  //�޳� ���ʧ��
  }
  /************************************************************************/
  /* �쳵 ͷ�� ��1
  1�����ͷ����Ϣ�ṹ��
  2��������һ
  3������һ��Ǩ��
  /************************************************************************/
  bool CreateHeadVeh(void)
  {
	  char LogBuf[255]={0};
	  VehSet  tempVeh;
	  char noplate[20]="���Ƴ�";
	  memset(&tempVeh,0, sizeof(tempVeh));//��ʼ����ʱ�����ṹ��
	  //�������κ�Ųһλ �ڳ�����ͷ				 
	  for (int i = g_VehNum; i >0; i--)
	  {
		  g_Vehqueue[i] = g_Vehqueue[i - 1];
	  }
	  //�쳵 ��1
	  tempVeh.VPattern=1;
	  tempVeh.AxleNum=2;
	  tempVeh.AxleType[0]=1;
	  tempVeh.AxleType[1]=1;
	  tempVeh.VSpeed=0;//
	  tempVeh.VehId=-2;//
	  memcpy(tempVeh.Plate,noplate,sizeof(noplate));
	  g_VehNum++;
	  //�������ͷ
	  memcpy(&g_Vehqueue[0],&tempVeh,sizeof(tempVeh));
	  
	  sprintf(LogBuf, "��--�� ����ID%d,����%s,����%d,����%d,�������ͣ��쳵,��������%d",
		  tempVeh.VehId, tempVeh.Plate,tempVeh.VPattern, tempVeh.AxleNum,g_VehNum);
	  WriteLogFile(LogBuf);
	  
	  return 0;
  }
  /************************************************************************/
  /* �����ǰ��
  1�������ǰ����Ϣ�ṹ��
  2��������һ
  /************************************************************************/
  bool CleanEndVeh(void)
  {
	  char LogBuf[255]={0};
	  if (g_VehNum > 0)
	  {
		  //д��־
		  sprintf(LogBuf, "ɾ��β�� ID:%d,��������%d", g_Vehqueue[g_VehNum - 1].VehId, g_VehNum - 1);
		  WriteLogFile(LogBuf);
		  memset(&g_Vehqueue[g_VehNum-1], 0, sizeof(g_Vehqueue[0]));//���β������Ϣ
		  g_VehNum--;
		  return 0;
	  }
	  else
	  {
		  WriteLogFile("�����޳� ɾ��β��ʧ��");
		  return 1;
	  }
	  //�޳� ���ʧ��
  }
  /************************************************************************/
  /* �������
  1�����������Ϣ�ṹ��
  2������Ϊ0
  /************************************************************************/
  AVIMAIN bool __stdcall CleanQueVeh(void)
  {
	  if (g_VehNum > 0)
	  {
		  //д��־
		  memset(&g_Vehqueue, 0, sizeof(g_Vehqueue));
		  g_VehNum = 0;
		  WriteLogFile("���л��� ������");
		  return 0;
	  }
	  else
	  {
		  WriteLogFile("���л��� �����");
	  }
	  return 1;
	  //�޳� ���ʧ��
  }
  
  /**************************************
  BOOL _stdcall EndVeh()
  ���ܣ� �˺����ǵ�����������ȡ����ʱ����β��ûͨ������������˶����ǿյġ�
  ��λ�����Է��ʹ����ǿ�н�����������ͨ�У����䳵�ͷ�������С�
  ��ˣ�������ɹ��󣬶�����һ����1. ��β�ɹ��������Ƕ�������û�г�������ͨ���У��Ѿ�ͨ��������������4 ���ᡣ
  ǿ����β�ĳ���ȡ��ǰ����ȫͨ����Ļ���󣬲����ٽ����ͼ�����У��Ѿ���ǰ�����ˣ���
  ����ֵ�� TRUE Ϊ��β�ɹ�����ʱ��������һ����1 ������FALSE Ϊʧ�ܡ�
  **************************************/
  AVIMAIN int _stdcall WJ_EndVeh(void)
  {
	  //	else//�����޳� ǿ����β
	  //	{
	  //		if(strlen(CameraInfo0)>6)//���ȴ���6 ��ʾ�϶��г��� ����ƥ��
	  //		{
	  //			if(platecheck(CameraInfo0,plate)>80)//����ƥ��ɹ� ����ǿ����β
	  //
	  //				if(TrigForceOut()==1)//������β
	  //				//��ʱӦ������β���� �����ʹӻ���������� �����������
	  //
	  //		}
	  //				memset(CameraInfo0, 0, sizeof(CameraInfo0));
	  //	memset(tempVehSet,0,sizeof(g_Vehqueue[0]));	
	  //	}
	  //	if(g_QueSet.DelVehTime<0)//�����Զ�ɾ������
	  //	{
	  //		CleanHeadVeh();
	  //	}
	  int res=TrigForceOut();
	  WriteLogFile("ǿ����β ��ʼ");
	  if(res==0)//������β �쳵
	  {
		  WriteLogFile("������β �쳵��1");
		  CreateHeadVeh();
		  //	 return 0;
		  
	  }
	  else if(res==1)//������β�����ϴ�
	  {
		  ResetEvent(g_HEvent[1]);
		  WriteLogFile("������β �����ϴ�");
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
		  WriteLogFile("ǿ����β �޷��� �쳵!");
		  CreateHeadVeh();
		  //	return -1;
		  
	  }
	  return 1;
  }
  
  
  /************************************************************************/
  /* ���س����ȡϵͳ������Ϣ                                             */
  /************************************************************************/
  AVIMAIN SystemSet __stdcall GetMainSysSet(void)
  {
	  return g_SysTem;
  }
  
  /************************************************************************/
  /* ���س�������ϵͳ����                                                 */
  /************************************************************************/
  AVIMAIN int __stdcall SetMainSysSet(SystemSet SysSet)
  {
	  int nCode=ERR_FAIL;
	  memcpy(&g_SysTem,&SysSet,sizeof(g_SysTem));
	  nCode=SetSysParam(g_SysTem);
	  return nCode;
  }
  
  
  /************************************************************************/
  /* ���س����ȡģ�Ͳ���                                                 */
  /************************************************************************/
  AVIMAIN ModelSet __stdcall GetMainMdlSet(void)
  {
	  return g_ModelSet;
  }
  
  /************************************************************************/
  /* ���س�������ģ�Ͳ���                                                 */
  /************************************************************************/
  AVIMAIN int __stdcall SetMainMdlSet(ModelSet Mdl)
  {
	  int nCode=ERR_FAIL;
	  memcpy(&g_ModelSet,&Mdl,sizeof(ModelSet));
	  nCode=SetModelParam(g_ModelSet);
	  return nCode;
  }
  
  /************************************************************************/
  /* ���س����ȡ��������                                                 */
  /************************************************************************/
  AVIMAIN VOutSet __stdcall GetMainVoutSet(void)
  {
	  return g_VOutSet;
  }
  
  
  
  /************************************************************************/
  /* ���س������ó�������                                                 */
  /************************************************************************/
  AVIMAIN int __stdcall SetMainVoutSet(VOutSet VoSet)
  {
	  int nCode=ERR_FAIL;
	  memcpy(&g_VOutSet,&VoSet,sizeof(g_VOutSet));
	  nCode=SetVoutParam(g_VOutSet);
	  return nCode;
  }
  
  /************************************************************************/
  /* ��ȡ���ؼ�����������                                                 */
  /************************************************************************/
  AVIMAIN LaserSet __stdcall GetMainLaserSet(void)
  {
	  return g_LaserSet;
  }
  
  
  /************************************************************************/
  /* �������ؼ�������                                                     */
  /************************************************************************/
  AVIMAIN int __stdcall SetMainLaserSet(LaserSet Ls)
  {
	  int nCode=ERR_SUCC;
	  memcpy(&g_LaserSet,&Ls,sizeof(g_LaserSet));
	  return nCode;
  }
  
  /************************************************************************/
  /* ���س���д��־                                                       */
  /************************************************************************/
  AVIMAIN void __stdcall Avi_WriteLog(char* msg)
  {
	  WriteLogFile(msg);
  }
  
  
  /************************************************************************/
  /* ��ʼ���㷨����                                                       */
  /************************************************************************/
  AVIMAIN int __stdcall InitEngine(void)
  {
	  
	  uint16 nRet=ERR_FAIL;
	  //��ʼ��ģ�Ͳ���
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
  /* ���¼�������                                                         */
  /************************************************************************/
  AVIMAIN uint8 __stdcall ReLoadEngine(void)
  {
	  int nRet=ERR_FAIL;
	  InitGlobal();
	  nRet=StartEngine();
	  return nRet;
  }
  
  /************************************************************************/
  /* �ر��㷨����                                                         */
  /************************************************************************/
  AVIMAIN uint8 __stdcall ExitEngine(void)
  {
	  int nRet=ERR_FAIL;
	  nRet=InitGlobal();
	  return nRet;
  }
  
  /************************************************************************/
  /* ���س���ע������ص�����                                             */
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
  /* ���س���ȡɨ��ͼ��ص�����                                           */
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
  /* ��ȡͳ�Ʒ���·��                                                     */
  /************************************************************************/
  void GetStatRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\Stat\\");
  }
  
  /************************************************************************/
  /* ��ȡϵͳ�洢·��                                                    */
  /************************************************************************/
  void GetSyRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\");
  }
  
  /************************************************************************/
  /* ��ȡWave�洢·��                                                    */
  /************************************************************************/
  void GetWaveRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\Wave\\");
  }
  
  /************************************************************************/
  /* ��ȡImage�洢·��                                                    */
  /************************************************************************/
  void GetImageRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\PLot\\");
  }
  
  /************************************************************************/
  /* ��ȡWJLog�洢·��                                                    */
  /************************************************************************/
  void GetWJLogRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\Log\\");
  }
  /************************************************************************/
  /* ��ȡMDB�洢·��                                                    */
  /************************************************************************/
  void GetMDBRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\MDB\\");
  }
  /************************************************************************/
  /* ��ȡ����洢·��                                                    */
  /************************************************************************/
  void GetCameraRoot(char *pPath)
  {
	  GetCurrentDirectory(1024, pPath);
	  strcat(pPath, "\\WJLOG\\CAM\\");
  }
  /************************************************************************/
  /* ���Ŀ¼                                                             */
  /************************************************************************/
  void CheckRunDirectory(void)
  {
	  CFileFind finder;
	  char TmpPath[1024]={0};
	  GetSyRoot(TmpPath);//������Ŀ¼
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
  //���쳵��ʶ��̬��ӿں�������20170921
  //**************************************/
  //AVIMAIN bool _stdcall Model_InitDevice(HWND hHandle, int nMsg, int nPorType, char* cPortName, long lBaudrate);
  //AVIMAIN bool _stdcall Model_CloseDevice(void);
  //AVIMAIN bool _stdcall Model_Getinfo(int *nCarorTruck, int *nModel);
  //AVIMAIN bool _stdcall Model_GetState(int *nState);
  // AVIMAIN bool _stdcall Model_Capture(void);
  
  
  
  /**************************************
  ���쳵��ʶ��̬��ӿں�������20170921
  ��ʼ������ ��Ҫ���� �������Ϣ��
  **************************************/
  HWND g_hHandle = NULL;//��������ľ��
  int g_nMsg = 0;//�����������Ϣ��
  AVIMAIN bool _stdcall Model_InitDevice(HWND hHandle, int nMsg, int nPorType, char* cPortName, long lBaudrate)
  {
	  g_hHandle = hHandle;
	  g_nMsg = nMsg;
	  int err=0;
	  err=Avi_Init();
	  //�����ڲ���ʼ������ �㷨�����С�������������ͷ��
	  return true;
  }
  /**************************************
  ���쳵��ʶ��̬��ӿں�������20170921
  �ر��˳�
  **************************************/
  AVIMAIN bool _stdcall Model_CloseDevice(void)
  {
	  //��Ҫ�ر��豸���㷨�������̡߳��������������
	  Avi_Close();//�ѹر� �㷨����  �����߳�
	  return true;
  }
  /**************************************
  ���쳵��ʶ��̬��ӿں�������20170921
  ��ȡ������Ϣ  ��/��+����
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
		  sprintf(LogBuf, "������ȡ�� ID:%d  ��%d  ��������%d",tempVehSet.VehId, *nModel, g_VehNum);
		  WriteLogFile(LogBuf);
		  
	  }
	  else if((tempVehSet.VPattern>4)&&(tempVehSet.VPattern<10))
	  {
		  *nCarorTruck=2;
		  *nModel=tempVehSet.VPattern-4;
		  sprintf(LogBuf, "������ȡ�� ID:%d  ��%d  ��������%d",tempVehSet.VehId, *nModel, g_VehNum);
		  WriteLogFile(LogBuf);
	  }
	  else
	  {
		  *nCarorTruck=1;
		  *nModel=1;
		  sprintf(LogBuf, "������ȡ���쳣 ID:%d  Veh%d  ��������%d",tempVehSet.VehId, tempVehSet.VPattern, g_VehNum);
		  WriteLogFile(LogBuf);
	  }
	  
	  return true;
  }
  /**************************************
  ���쳵��ʶ��̬��ӿں�������20170921
  ��ȡ״̬
  **************************************/
  AVIMAIN bool _stdcall Model_GetState(int *nState)
  {
	  *nState=0;
	  WriteLogFile("��ȡ״̬");
	  return true;
  }
  /**************************************
  ���쳵��ʶ��̬��ӿں�������20170921
  ģ��ȡ��  �г�ʱ�ظ���Ϣ
  **************************************/
  AVIMAIN bool _stdcall Model_Capture(void)
  {
	  if (g_VehNum > 0)
	  {
		  //g_hHandle//��Ϣ����
		  //g_nMsg//��ϢID 
		  //����Ϣ������
		  WriteLogFile("�����г������Ϸ���Ϣ");
		  //		::SendMessage(g_hHandle, WM_USER + g_nMsg, 1, 0);
		  ::PostMessage(g_hHandle, g_nMsg, 1, 0);
		  //����TRUE
		  return TRUE;
	  }
	  else//ǿ����β
	  {
		  int ret = WJ_EndVeh();
		  if (ret == -1)
		  {
			  //����Ϣ������
			  WriteLogFile("�����޳� ��βʧ�ܲ�����Ϣ");
			  //			::PostMessage(g_hHandle, g_nMsg, 1, 0);//������ ʵ��ʹ��Ӧ������
			  return FALSE;   //��β���ɹ���ֱ�ӷ���0
		  }
		  else
		  {
			  WriteLogFile("�����޳� ��β������Ϣ");
			  ::PostMessage(g_hHandle, g_nMsg, 1, 0);
			  //����Ϣ������
			  return TRUE;
		  }
		  
	  }
	  
	  return true;
  }
  
  /**************************************
  �򼯸��ݳ���ʶ��̬��ӿ�20171012
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
  �򼯸��ݳ���ʶ��̬��ӿ�20171012
  ��ʼ������
  **************************************/
  AVIMAIN bool _stdcall  WjVr_InitDevice(int nPorType, char*cPortName, long lBaudrate)
  {
	  int err=0;
	  err=Avi_Init();
	  //�����ڲ���ʼ������ �㷨�����С�������������ͷ��
	  return true;
  }
  /**************************************
  �򼯸��ݳ���ʶ��̬��ӿ�20171012
  ��ȡ״̬
  **************************************/
  AVIMAIN int  _stdcall  WjVr_GetState(void)
  {
	  WriteLogFile("��ȡ״̬");
	  return 0;
  }
  /**************************************
  �򼯸��ݳ���ʶ��̬��ӿ�20171012
  ���ͷ��
  **************************************/
  AVIMAIN bool _stdcall  WjVr_ClearOne(void)
  {
	  CleanHeadVeh();
	  return true;
  }
  /**************************************
  �򼯸��ݳ���ʶ��̬��ӿ�20171012
  ��ȡ������
  **************************************/
  AVIMAIN int  _stdcall  WjVr_GetVehicleCount(void)
  {
	  int num=0;
	  num=Avi_VehCount();
	  return num;
  }
  /**************************************
  �򼯸��ݳ���ʶ��̬��ӿ�20171012
  ��ȡ��������
  **************************************/
  AVIMAIN bool _stdcall  WjVr_GetVehicleData(int * AxisGroupNum, int* AxisNum, 
	  BYTE* VehicleType, long* AxisWeight, int* AxisType, int* Speed, BYTE* iLeiXing,char* plate)
  {
	  VehSet  tempVehSet;
	  int TyreNum=0;//��̥����
	  int luntype=0;//ԭʼ����
	  char LogBuf[255]={0};
	  memset(&tempVehSet,0,sizeof(tempVehSet));//��ʼ��������Ϣ�ṹ��
	  GetVehicle(&tempVehSet,plate);//��ȡ������Ϣ�ṹ��
	  *AxisNum=tempVehSet.AxleNum;//����
	  *AxisGroupNum  = tempVehSet.AxlegroupNum;	
	  *AxisType=tempVehSet.TranAxleType;
	  *VehicleType = tempVehSet.VPattern;//����
	  *AxisWeight = 0;//
	  *Speed = tempVehSet.VSpeed;//
	  *iLeiXing = 0;
	  for(int i=0;i<tempVehSet.AxleNum;i++)
	  {
		  luntype=luntype*10+tempVehSet.AxleType[i];
	  }
	  
	  sprintf(LogBuf, "�ϴ����� ID:%d  ����:%s ����:%d ����:%d ������:%d ԭʼ����:%d��ת������:%d ",
		  tempVehSet.VehId,tempVehSet.Plate,tempVehSet.VPattern,tempVehSet.AxleNum,*AxisGroupNum,luntype,* AxisType);
	  WriteLogFile(LogBuf);
	  if(plate!=NULL)//���ɽ������ ��ʾȡ������
	  {
		  memcpy(plate,tempVehSet.Plate,sizeof(tempVehSet.Plate));
	  }
	  
	  return true;
  }
  /**************************************
  �򼯸��ݳ���ʶ��̬��ӿ�20171012
  ǿ����β
  **************************************/
  AVIMAIN int  _stdcall  WjVr_EndVehicle(void)
  {
	  int ret = WJ_EndVeh();
	  return ret; 
  }
  /**************************************
  �򼯸��ݳ���ʶ��̬��ӿ�20171012
  �ر��豸
  **************************************/
  AVIMAIN bool _stdcall  WjVr_Close(void)
  {
	  //��Ҫ�ر��豸���㷨�������̡߳��������������
	  Avi_Close();//�ѹر� �㷨����  �����߳�
	  return true;
  }
  
  /**************************************
  �򼯸��ݳ���ʶ��̬��ӿ�20171012
  ��ȡPCL ����
  **************************************/
  AVIMAIN bool _stdcall  WjVr_GetPcl(uint16 * PclData, long *Pclsize)
  {
	  *PclData = 0;
	  *Pclsize = 0;
	  return true;
  }
  
  /**************************************
  �򼯸��ݳ���ʶ��̬��ӿ�20171012
  ��ȡ������Ϣ
  **************************************/
  AVIMAIN int  _stdcall  WjVr_FlushQueue(void)
  {
	  CleanQueVeh();
	  return g_VehNum;
  }
  
  /**************************************
  �򼯸��ݳ���ʶ��̬��ӿ�20171012
  ��ȡ������Ϣ
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
  
  
  
  
  
  
  
  
  

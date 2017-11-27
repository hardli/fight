#include "StdAfx.h"
#include "Avi_SvWv.h"
#include "Avi_Main.h"
#include "IniProc.h"
#include "JZGlobal.h"


CFile * m_DATA_file=NULL;
CArchive * m_DATA_archive=NULL;

void ClearSvWv(void)
{
	if (g_SvWave.SvThdHd!=NULL)
	{
		CloseHandle(g_SvWave.SvThdHd);
		g_SvWave.SvThdHd=NULL;
	}
	memset(&g_SvWave,0,sizeof(g_SvWave));
}

void IntoWaveCache(char *pWv,int Len)
{
	memcpy(g_SvWave.WvCache[g_SvWave.RecIndx],pWv,Len);
	g_SvWave.RecIndx=(g_SvWave.RecIndx+1)%Max_CACHE;
}

/************************************************************************/
/* 初始化出波形缓存等                                                   */
/************************************************************************/
AVIMAIN int __stdcall PreSaveWave(void)
{
	ClearSvWv();
	if (g_SysTem.SvWv)
	{
		g_SvWave.bSvFlag=true;
		g_SvWave.SvThdHd = CreateThread(NULL, 0, SvWaveProc, NULL, 0, NULL);//创建线程
		if (g_SvWave.SvThdHd==NULL)
		{
			g_SvWave.bSvFlag=false;
			return ERR_FAIL;
		}
	}
	CString Logtxt="";
	Logtxt.Format("存储波形标志%d",g_SysTem.SvWv);
	WriteLogFile(Logtxt.GetBuffer(Logtxt.GetLength()));
	Logtxt.ReleaseBuffer();
	return ERR_SUCC;
}

//DWORD WINAPI SvWaveProc(LPVOID lpParam)
//{
//	SYSTEMTIME st;
//	CString strFileName="";
//	CString strDirName="";
//	char WavePath[1024]={0};
//	GetWaveRoot(WavePath);
//	while (g_SvWave.bSvFlag)
//	{
//		if (g_SvWave.RecIndx!=g_SvWave.SvIndx)
//		{
//			char timestr[20]={0}; 
//			GetLocalTime(&st);
//			sprintf(timestr, "%02d-%02d-%02d-%03d",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
//			if (g_SvWave.bStaSv&&!g_SvWave.nEndSv)
//			{
//				g_SvWave.SvCnt++;
//				if (g_SvWave.bNewCar)
//				{
//					g_SvWave.SvCnt = 1;
//					CTime tm=CTime::GetCurrentTime();
//					CString Tmp="";
//					Tmp=tm.Format("%Y-%m-%d %H-%M-%S");
//					strFileName.Format("%s%s.lmds",WavePath,Tmp);
//					g_SvWave.bNewCar=0;
//					WriteDatafile(strFileName); //创建文件
//				}
//				WriteDatafileData(g_SvWave.WvCache[g_SvWave.SvIndx],831);
//				WriteDatafileData(timestr,12);     //保存时间  每帧长度831+12
//			}
//			else if (g_SvWave.bStaSv&&g_SvWave.nEndSv)
//			{
//				g_SvWave.SvCnt++;
//				WriteDatafileData(g_SvWave.WvCache[g_SvWave.SvIndx],831);
//				WriteDatafileData(timestr,12);     //保存时间  每帧长度831+12
//				CloseDatafile();
//				g_SvWave.bStaSv=0;
//			}
//			g_SvWave.SvIndx=(g_SvWave.SvIndx+1)%Max_CACHE;
//			Sleep(5);
//		}
//		else
//		{
//			Sleep(10);
//		}
//	}
//	return ERR_SUCC;
//}

/************************************************************************/
/*波形分车操作                                                          */
/************************************************************************/
void SwCarSpiltProc(int nCmd,int indx)
{
	g_SvWave.WvCache[g_SvWave.RecIndx][Full_Size]=nCmd;//新车标志或过车标志
	g_SvWave.WvCache[g_SvWave.RecIndx][Full_Size+1]=((g_VehIndex.u32Index+1)>>24&0xFF);
	g_SvWave.WvCache[g_SvWave.RecIndx][Full_Size+2]=((g_VehIndex.u32Index+1)>>16&0xFF);
	g_SvWave.WvCache[g_SvWave.RecIndx][Full_Size+3]=((g_VehIndex.u32Index+1)>>8&0xFF);
	g_SvWave.WvCache[g_SvWave.RecIndx][Full_Size+4]=((g_VehIndex.u32Index+1)&0xFF);
}

DWORD WINAPI SvWaveProc(LPVOID lpParam)
{
	SYSTEMTIME st;
	CString strFileName="";
	CString strDirName="";
	while (g_SvWave.bSvFlag)
	{
		try
		{
			if (g_SvWave.RecIndx!=g_SvWave.SvIndx)
			{
				char timestr[20]={0}; 
				GetLocalTime(&st);
				sprintf(timestr, "%02d-%02d-%02d-%03d",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
				g_SvWave.SvCnt++;
				//新车标志 分文件夹存储
				if (g_SvWave.WvCache[g_SvWave.SvIndx][831])
				{
					g_SvWave.SvCnt = 1;
					CTime tm=CTime::GetCurrentTime();
					CString Tmp="";
					int SvVehIdx=0;
					SvVehIdx=((g_SvWave.WvCache[g_SvWave.SvIndx][832]<<24)+(g_SvWave.WvCache[g_SvWave.SvIndx][833]<<16)
							+(g_SvWave.WvCache[g_SvWave.SvIndx][834]<<8)+g_SvWave.WvCache[g_SvWave.SvIndx][835]);
					Tmp.Format("_%d",SvVehIdx);
					Tmp=tm.Format("%Y-%m-%d %H-%M-%S")+Tmp;
					strDirName=CheckWaveDir(Tmp.GetBuffer(Tmp.GetLength()));
					Tmp.ReleaseBuffer();
					//同时清空标志 防止空白帧建立文件夹20171119
					memset(&g_SvWave.WvCache[g_SvWave.SvIndx][831],0,5);
				}
				strFileName.Format("%s\\A_%s_%d.lmd",strDirName, timestr,g_SvWave.SvCnt);
				WriteDatafile(strFileName); //创建文件
				WriteDatafileData(g_SvWave.WvCache[g_SvWave.SvIndx],831);
				g_SvWave.SvIndx=(g_SvWave.SvIndx+1)%Max_CACHE;
				//fileUtils.WriteDatafileData(g_cNetRecvBuf2[nBufID2],831);
				CloseDatafile();
				Sleep(5);
			}
			else
			{
				Sleep(100);
			}
		}
		catch(...)
		{
			WriteLogFile("存波形异常");
		}
		
	}
	return ERR_SUCC;
}

CString CheckWaveDir(char *TmpFloder)
{
	CFileFind finder;
	char TmpPath[1024]={0};
	GetWaveRoot(TmpPath);
	strcat(TmpPath, TmpFloder);
	if (!finder.FindFile(TmpPath))
	{
		CreateDirectory(TmpPath,NULL);
	}
	CString Fullpath="";
	Fullpath.Format("%s",TmpPath);
	return Fullpath;
}




BOOL StopSaveWave(void)
{
	if (g_SvWave.SvThdHd!=NULL)
	{
		CloseHandle(g_SvWave.SvThdHd);
		g_SvWave.SvThdHd=NULL;
	}
	g_SvWave.bSvFlag=false;
	g_SvWave.RecIndx=0;
	g_SvWave.SvIndx=0;
	g_SvWave.SvCnt=0;
	return true;
}

BOOL WriteDatafile( LPCSTR lpFileName)
{
	m_DATA_file = new CFile(lpFileName,CFile::modeCreate | CFile::modeWrite);
	m_DATA_archive = new CArchive(m_DATA_file,CArchive::store);
	return TRUE;
}

BOOL WriteDatafileData( LPVOID lpData, UINT dataSize)
{
	m_DATA_archive->Write(lpData,dataSize);
	m_DATA_archive->Flush();
	return TRUE;
}

void CloseDatafile()
{
	if(m_DATA_archive)
	{
		m_DATA_archive->Flush();
		m_DATA_archive->Close();
		m_DATA_file->Close();
		delete m_DATA_archive;
		delete m_DATA_file;
		m_DATA_archive = NULL;
		m_DATA_file = NULL;
	} 
}

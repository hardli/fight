// JGProxy.cpp: implementation of the CJGProxy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JGProxy.h"
#include "Avi_Comm.h"
#include "Avi_Main.h"
#include "IniProc.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CJGProxy::CJGProxy(CString DesIp,UINT32 DesPort,int Id)
{
	m_DesIp=DesIp;
	m_DesPort=DesPort;
	m_Id=Id;
}


CJGProxy::CJGProxy()
{
	m_DesIp="192.168.0.2";
	m_DesPort=2110;
	m_Id=0;
	pfRec=NULL;
	m_nTimer=0;
	m_WvLost=0;
	m_Permited=false;
	m_StaIdx=0;
	m_StaTm=0;
	m_LastIdx=0;
	m_LastTm=0;
}


CJGProxy::~CJGProxy()
{
	
}

int CJGProxy::ConnectJG(void)
{
	m_Net.dwServerIP=ntohl(inet_addr(m_DesIp));
	m_Net.unServerPort=m_DesPort;
	m_Net.nID=m_Id;
	if (pfRec!=NULL)
	{
		m_Net.lpRecvFun=(void *)pfRec;
	}
	else
	{
		m_Net.lpRecvFun=NULL;
	}
	if (m_Net.ConnectSvr()!=ERR_SUCC)
	{
		SetEvent(m_Net.hSvrDis);
	}
	return ERR_SUCC;
}


int CJGProxy::RegRecCallBack(PFCB_JG_RECV pf)
{
	pfRec=pf;
	return ERR_SUCC;
}

int CJGProxy::AqcSingleWv()
{
	char sCmd[24]={0x02,0x02,0x02 ,0x02 ,0x00 ,0x00 ,0x00 ,0x0F ,0x73 ,0x52 
		,0x4E,0x20 ,0x4C ,0x4D ,0x44 ,0x73 ,0x63 ,0x61 ,0x6E ,0x64 ,0x61 ,0x74 ,0x61 ,0x05};
	if (m_Net.bConnected)
	{
		m_Net.SendData(sCmd,24);
	}
	else
	{
		return ERR_AQCSINGLE;
	}
	return ERR_SUCC;
}


int CJGProxy::AqcContiWv()
{
	char sCmd[26]={0x02 ,0x02 ,0x02 ,0x02 ,0x00 ,0x00 ,0x00 ,0x11 ,0x73 ,0x45 
		,0x4E ,0x20 ,0x4C ,0x4D ,0x44 ,0x73 ,0x63 ,0x61 ,0x6E ,0x64 ,0x61 ,0x74 ,0x61 ,0x20 ,0x01 ,0x33};
	if (m_Net.bConnected)
	{
		m_Net.SendData(sCmd,26);
	}
	else
	{
		return ERR_AQCCONTIN;
	}
	return ERR_SUCC;
}

int CJGProxy::StopContiWv()
{
	char sCmd[26]={0x02 ,0x02 ,0x02 ,0x02 ,0x00 ,0x00 ,0x00 ,0x11 ,0x73 ,0x45 
		,0x4E ,0x20 ,0x4C ,0x4D ,0x44 ,0x73 ,0x63 ,0x61 ,0x6E ,0x64 ,0x61 ,0x74 ,0x61 ,0x20 ,0x00 ,0x32};
	if (m_Net.bConnected)
	{
		m_Net.SendData(sCmd,26);
	}
	else
	{
		return ERR_STOPCONTI;
	}
	return ERR_SUCC;
}

int CJGProxy::SendStateCmd()
{
	char sCmd[25]={0x02 ,0x02 ,0x02 ,0x02 ,0x00 ,0x00 ,0x00 ,0x10 ,0x73 ,0x51 
		,0x4E ,0x51 ,0x4C ,0x4D ,0x44 ,0x73 ,0x63 ,0x61 ,0x6E ,0x64 ,0x61 ,0x74 ,0x61 ,0x20 ,0x57};
	if (m_Net.bConnected)
	{
		m_Net.SendData(sCmd,25);
	}
	else
	{
		return ERR_STOPCONTI;
	}
	return ERR_SUCC;
}



int CJGProxy::CheckPermit(char *pbuff,int len)
{
	int result = 0;
	int j=0;
	if (len==123)
	{
		if (pbuff[0]==0x02&&pbuff[1]==0x02)
		{
			
            for (j = 8; j < 122; j++)
            {
                result ^= pbuff[j];
            }
			
			if (result == pbuff[122])
            {
                if (pbuff[26] == 87
                    && pbuff[27] == 76
                    && pbuff[28] == 82
                    && pbuff[29] == 45
                    && pbuff[30] == 77
                    && pbuff[31] == 55
                    && pbuff[32] == 49
                    && pbuff[33] == 49) //分别对应WLR_M711的ASCII码值
                {
					m_Permited=true;
				}
				else
				{
					m_Permited=false;
				}
			}
			else
			{
				m_Permited=false;
			}
		}
		else
		{
			m_Permited=false;
		}
	}
	else
	{
		m_Permited=false;
	}
	return 0;
}

void CJGProxy::StaticLostWave(UINT CurIndx,UINT32 CurTm)
{
	int i=0;
	BOOL nOk=TRUE;
	int nFlag=0;
	CString filename="";
	char TempStr[512]={0};
	char Contstr[1024]={0};

	time_t now_time=time(NULL);
	struct tm *p;

	CString strIndx=""; 
	strIndx.Format("%04X",CurIndx);
	
	char a_cStatPath[512]={0};

	//第二步 打开年月日编号命名的文本文件 如果没有则追加新建
	//CString strTime= CTime::GetCurrentTime().Format("%Y-%m-%d");
	CString strTime= CTime::GetCurrentTime().Format("%Y-%m-%d %H");
	filename=strTime+".log";
	
	//第一步 获取当前目录 得到根目录
	GetStatRoot(a_cStatPath);
	if(a_cStatPath[strlen(a_cStatPath)-1]!='\\') 
	{
		a_cStatPath[strlen(a_cStatPath)]='\\';
	}
	strcat(a_cStatPath,filename); 

	//统计分析
	if (m_StaIdx==0)
	{
		m_StaIdx=CurIndx;
		m_StaTm=CurTm;
		m_LastIdx=CurIndx;
		m_LastTm=CurTm;
		m_LsIdxCnt=0;
		m_LsTmCnt=0;
	}
	
	if (CurIndx>m_LastIdx&&CurIndx-m_LastIdx>1)
	{
		m_LsIdxCnt=m_LsIdxCnt+CurIndx-m_LastIdx;
		nFlag=nFlag|1;
		nOk=FALSE;
	}
	m_LastIdx=CurIndx;

	if ((CurTm>m_LastTm)
		&&((CurTm-m_LastTm)/1000>25
		||(CurTm-m_LastTm)/1000<15))
	{
		m_LsTmCnt++;
		nFlag=nFlag|2;
		nOk=FALSE;
	}
	else if (CurTm<m_LastTm&&((m_LastTm-CurTm)/1000>25
		||(m_LastTm-CurTm)/1000<15))
	{
		m_LsTmCnt++;
		nFlag=nFlag|2;
		nOk=FALSE;
	}
	m_LastTm=CurTm;

	
	if ((CurTm-m_StaTm)/1000>999)
	{
		p=localtime(&now_time);//	
		sprintf(TempStr,"%02d:%02d:%02d\t\t",p->tm_hour,p->tm_min,p->tm_sec);
		strcat(Contstr,TempStr);
		sprintf(TempStr,"断序%u\t\t",m_LsIdxCnt);
		strcat(Contstr,TempStr);
		sprintf(TempStr,"断时%u\n",m_LsTmCnt);
		strcat(Contstr,TempStr);
		m_LsIdxCnt=0;
		m_LsTmCnt=0;
		m_StaIdx=CurIndx;
		m_StaTm=CurTm;
	}
	sprintf(TempStr,"%s\t\t",strIndx);
	strcat(Contstr,TempStr);
	sprintf(TempStr,"%u\t\t",CurIndx);
	strcat(Contstr,TempStr);
	if (nOk!=TRUE)
	{
		sprintf(TempStr,"%u\t\t",CurTm);
		strcat(Contstr,TempStr);
		sprintf(TempStr,"%u\n",nFlag);
		strcat(Contstr,TempStr);
	}
	else
	{
		sprintf(TempStr,"%u\n",CurTm);
		strcat(Contstr,TempStr);
	}

	try
	{
		FILE* fp;
		if ((fp=fopen(a_cStatPath,"a+"))==NULL)
		{
			return ;
		}
		fwrite(Contstr,sizeof(char),strlen(Contstr),fp);
		fclose(fp);
	}
	catch (...) 
	{
		WriteLogFile("统计丢帧写文件失败");
	}

}

int CJGProxy::SendBeatCmd(void)
{
	char sCmd[25]={0x02,0x02,0x02 ,0x02 ,0x00 ,0x00 ,0x00 ,0x10 ,0x73 ,0x5E 
		,0x4E,0x51 ,0x4C ,0x4D ,0x44 ,0x73 ,0x63 ,0x61 ,0x6E ,0x64 ,0x61 ,0x74 ,0x61 ,0x20,0x58};
	if (m_Net.bConnected)
	{
		m_Net.SendData(sCmd,25);
	}
	else
	{
		return ERR_FAIL;
	}
	return ERR_SUCC;	
}
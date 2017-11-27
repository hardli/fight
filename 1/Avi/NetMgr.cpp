// CNetMgr.cpp: implementation of the CNetMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetMgr.h"
#include "Avi_Comm.h"
#include "IniProc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NetRecBuf g_sNetBuf;


CNetMgr::CNetMgr()
{
	OnInit();
}

CNetMgr::~CNetMgr()
{
	Dispose();
}

void CNetMgr::OnInit()
{
	ManClosed=FALSE;
	dwServerIP = 0;
	unServerPort = 0;
	bConnected = FALSE;
	nID = -1;
	pcSendDataBuf = NULL;
	nSendBufSize = 0;
	nRecConCnt=0;
	bUseBkPort=FALSE;
	lpRecvFun = NULL;
	lpWnd = NULL;
	hThRecv=NULL;
	hThRCon=NULL;
	m_Sock=INVALID_SOCKET;
	hSvrDis=CreateEvent(NULL,true,FALSE,NULL);
	StaRecConTh();
	InitNetCache();
}

void CNetMgr::InitNetCache()
{
	memset(&g_sNetBuf,0,sizeof(g_sNetBuf));
}

int CNetMgr::ConnectSvr(void)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err=0;
	int len = sizeof(int);
	unsigned long ul = 1;
	bool ret = false;
	UINT nPort=0;
	wVersionRequested = MAKEWORD(1,1);
	err = WSAStartup(wVersionRequested,&wsaData);
	if (err != 0)
		return ERR_SOCKVERERR;

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion != 1))
	{
		WSACleanup();
		return ERR_SOCKVERERR;
	}

	if (m_Sock!=INVALID_SOCKET)
	{
		closesocket(m_Sock);
	}
	//清空缓存
	InitNetCache();
	// 创建套接字
	m_Sock = socket(AF_INET,SOCK_STREAM,0);
	
	if (m_Sock == INVALID_SOCKET)
	{
		return ERR_CREATESOCK;
	}
	
	if (bUseBkPort)
	{
		nPort=7070;
	}
	else
	{
		nPort=unServerPort;
	}
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(dwServerIP);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(nPort);
	
	// 向服务器发出连接请求
	int iConnect=connect(m_Sock,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	if(iConnect==0)
	{
		if (hThRecv!=NULL)
		{
			CloseHandle(hThRecv);
			hThRecv=NULL;
		}
		bConnected=true;
		hThRecv=CreateThread(NULL, 0, RecvThread, this, 0, NULL);//创建线程
		if (hThRecv==NULL)
		{
			return ERR_RECVTHREAD;
		}
		SetThreadPriority(hThRecv, 	THREAD_PRIORITY_TIME_CRITICAL);
			
	}
	else
	{
		closesocket(m_Sock);
		WSACleanup();	
		return FALSE;
	}
	return ERR_SUCC;
}


int CNetMgr::StaRecConTh()
{
	//	unsigned threadID;
	if (hThRCon!=NULL)
	{
		CloseHandle(hThRCon);
	}
	hThRCon=CreateThread(NULL, 0, RecConThread, this, 0, NULL);//创建线程
	//hThRCon=(HANDLE)_beginthreadex(NULL,0,(unsigned(WINAPI *)(void *))RecConThread,this,0,&threadID);
	if (hThRCon==NULL)
	{
		return ERR_RECCONNERR;
	}
	return ERR_SUCC;
}

// 接收线程处理函数
DWORD WINAPI CNetMgr::RecvThread(LPVOID lpParam)	
{
	char LogBuf[255]={0};
	CNetMgr *pNetMgr = ((CNetMgr *)lpParam);
	unsigned int l_u32reallen = 0;
	int nRecvSize = 0;
	char acRecvDataBuf[MAX_BLOCK]={0};
	if (pNetMgr->lpRecvFun==NULL)
	{
		return ERR_RECFUNNULL;
	}
	PFCB_NET_RECV recvFun = (PFCB_NET_RECV)(pNetMgr->lpRecvFun);

	while(pNetMgr->bConnected)
	{
		try
		{
			nRecvSize = recv(pNetMgr->m_Sock,acRecvDataBuf,MAX_BLOCK,0);
			// recv 函数返回值
			//   <0 出错 
			//   =0 连接关闭 
			//   >0 接收到数据大小，
			if (nRecvSize>0 && recvFun!=NULL)
			{
				if ((g_sNetBuf.m_u32in + nRecvSize)>=MAX_CACHES*MAX_BLOCK)
				{
					WriteLogFile("(g_sNetBuf.m_u32in + nRecvSize)>=10*MAX_BLOCK\n");
					memset(&g_sNetBuf,0,sizeof(g_sNetBuf));//大于最大缓存，原来的数全清0
					continue;
				}
				memcpy(&g_sNetBuf.m_acbuf[g_sNetBuf.m_u32in],acRecvDataBuf,nRecvSize*sizeof(char));
				g_sNetBuf.m_u32in += nRecvSize;

				while(g_sNetBuf.m_u32out < g_sNetBuf.m_u32in)
				{
					if (g_sNetBuf.m_u32out+8>g_sNetBuf.m_u32in)
					{
						WriteLogFile("不够计算帧长等下一次");
						break;
					}
					if(((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out] == 0x02 &&
						(unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out+1] == 0x02 && 
						(unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out+2] == 0x02 && 
						(unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out+3] == 0x02 ))
					{
						//计算包长度
						l_u32reallen = ((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out+4] << 24) | 
							((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out+5] << 16) |
							((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out+6] << 8) |
							((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out+7] << 0);
						l_u32reallen = l_u32reallen + 9;
					
						//判断包长度与实际缓存长度之间的关系
						if (l_u32reallen <= (g_sNetBuf.m_u32in - g_sNetBuf.m_u32out+1))
						{
							(*recvFun)(&g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out],l_u32reallen);
							g_sNetBuf.m_u32out += l_u32reallen;
						}
						else if(l_u32reallen >= MAX_BLOCK)
						{
							sprintf(LogBuf,"计算包长异常%d\n",l_u32reallen);
							WriteLogFile(LogBuf);
							memset(&g_sNetBuf,0,sizeof(g_sNetBuf));//大于最大缓存，原来的数全清0
							continue;
						}
						else
						{
							WriteLogFile("不够一帧长等下一次");
							break;
						}
					}
					else
					{
						g_sNetBuf.m_u32out++;
					}
				}			

				if ((g_sNetBuf.m_u32in + nRecvSize)>=MAX_BLOCK*MAX_CACHES)
				{
					WriteLogFile("(g_sNetBuf.m_u32in + nRecvSize)>=MAX_BLOCK*MAX_CACHES\n");
					sprintf(LogBuf,"g_sNetBuf.m_u32in is %d,g_sNetBuf.m_u32out is%d\n",g_sNetBuf.m_u32in,g_sNetBuf.m_u32out);
					WriteLogFile(LogBuf);
					memset(&g_sNetBuf,0,sizeof(g_sNetBuf));//大于最大缓存，原来的数全清0
					continue;
				}
				if (g_sNetBuf.m_u32out >= g_sNetBuf.m_u32in)
				{
					memset(&g_sNetBuf,0,sizeof(g_sNetBuf));
				}
			}
			if (nRecvSize <= 0 && recvFun!=NULL)//另外一端已关闭
			{
				(*recvFun)(acRecvDataBuf,nRecvSize);	
				pNetMgr->bConnected = FALSE;
				WriteLogFile("激光主动断开,发起重连");
				Sleep(1000);
				SetEvent(pNetMgr->hSvrDis);
				return ERR_SVRCLOSED;
			}
		}
		catch(...)
		{
			sprintf(LogBuf, "线程异常 接收线程");
			WriteLogFile(LogBuf);
		}
	}	
	return ERR_SUCC;
}




int CNetMgr::DisConnSvr()
{
	if (bConnected!=FALSE)
	{
		if (m_Sock!=INVALID_SOCKET)
		{
			closesocket(m_Sock);
		}
		WSACleanup();
		bConnected=FALSE;
	}						
	return ERR_SUCC;
}


int CNetMgr::SendData(char *pcSendBuf,int nSendSize)
{
	int Result=0;
	int RetCode=0;
	DWORD SendLen=0;
	DWORD Flags=0; 
	WSABUF wsa; 
	OVERLAPPED Overlapped;
	Flags = 0;
	
	memset(&Overlapped,0, sizeof(OVERLAPPED));
	wsa.buf=pcSendBuf;
	wsa.len=nSendSize;
	RetCode = WSASend(m_Sock,
		&wsa,
		1,
		&SendLen,
		Flags,
		&Overlapped,
		NULL);
	if (RetCode == SOCKET_ERROR)
	{
		RetCode = WSAGetLastError();
		if (RetCode == ERROR_IO_PENDING) 
			return RetCode;
	} 
	return ERR_SUCC;
}

DWORD WINAPI CNetMgr::RecConThread(LPVOID lpParam)
{
	CNetMgr *pNetMgr = ((CNetMgr *)lpParam);
	DWORD nCode=0;
	while (1)
	{
		nCode=WaitForSingleObject(pNetMgr->hSvrDis,INFINITE);
		if (nCode==WAIT_OBJECT_0)
		{
			if(pNetMgr->bConnected==FALSE)
			{
				pNetMgr->nRecConCnt++;
				if (pNetMgr->nRecConCnt>10)
				{
					pNetMgr->bUseBkPort=TRUE;
					WriteLogFile("使用备份端口发起重连");
				}
				else
					WriteLogFile("使用目的端口发起重连");
			
				pNetMgr->ConnectSvr();
				
			}
			else
			{
				WriteLogFile("重连成功");
				ResetEvent(pNetMgr->hSvrDis);
				pNetMgr->nRecConCnt=0;
				if (pNetMgr->bUseBkPort)
				{
					pNetMgr->DisConnSvr();
					WriteLogFile("关闭备份端口连接");
					pNetMgr->bUseBkPort=FALSE;
					SetEvent(pNetMgr->hSvrDis);
				}
			}
		}
		Sleep(1500);
	}
}

int CNetMgr::Dispose()
{
	if (m_Sock!=INVALID_SOCKET)
	{
		closesocket(m_Sock);
	}
	if (hThRecv!=NULL)
	{
		CloseHandle(hThRecv);
	}
	if (hThRCon!=NULL)
	{
		CloseHandle(hThRCon);
	}
	OnInit();
	return ERR_SUCC;
}
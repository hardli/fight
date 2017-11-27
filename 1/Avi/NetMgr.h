// CNetMgr.h: interface for the CNetMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CNetMgr_H__9128D212_C80B_489A_8453_527D1A84C14E__INCLUDED_)
#define AFX_CNetMgr_H__9128D212_C80B_489A_8453_527D1A84C14E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"

#define MAX_BLOCK 10240

typedef void (*PFCB_NET_RECV)(char *pDataBuf, int nDataBufSize);


#define MAX_CACHES 50
typedef struct TagNetRecBuf
{
	char m_acbuf[MAX_BLOCK*MAX_CACHES];
	unsigned int m_u32in;
	unsigned int m_u32out;
}NetRecBuf;


class CNetMgr  
{
public:
	CNetMgr();
	virtual ~CNetMgr();
public:
	int nID;
	DWORD dwServerIP;
	UINT unServerPort;
	BOOL bConnected;
	BOOL bUseBkPort;
	int  nRecConCnt;


	char *pcSendDataBuf;
	int nSendBufSize;
	LPVOID lpRecvFun;
	LPVOID lpWnd;
public:
	HANDLE hSvrDis;
	BOOL ManClosed;
	HANDLE hThRecv;
	HANDLE hThRCon;
	SOCKET m_Sock;
	void OnInit();
	int StaRecConTh(void);
	int ConnectSvr(void);
	int DisConnSvr(void);
	int SendData(char *pcSendBuf,int nSendSize);
	static DWORD _stdcall RecvThread(LPVOID lpParam);	// 接收线程处理函数
	static DWORD _stdcall RecConThread(LPVOID lpParam);
	int Dispose();
	void InitNetCache(void);
};

#endif // !defined(AFX_CNetMgr_H__9128D212_C80B_489A_8453_527D1A84C14E__INCLUDED_)

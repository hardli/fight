// JGProxy.h: interface for the CJGProxy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JGPROXY_H__CF78EAE2_B075_49EE_A81A_66A33924584F__INCLUDED_)
#define AFX_JGPROXY_H__CF78EAE2_B075_49EE_A81A_66A33924584F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "NetMgr.h"

typedef void (*PFCB_JG_RECV)(char *pDataBuf, int nDataBufSize);


class CJGProxy  
{
public:
	CJGProxy();
	CJGProxy(CString DesIp,UINT32 DesPort,int Id);
	virtual ~CJGProxy();
private:
	LPVOID pfRec;

public:
	CString m_DesIp;
	UINT32 m_DesPort;
	int m_Id;
	int m_nTimer;
	int m_WvLost;
    BOOL m_Permited;//认证标志


	//丢帧统计
	int m_LsIdxCnt;//帧序号不连续计数
	int m_LsTmCnt;//时间不连续计数
	UINT m_StaIdx;//开始统计帧
	UINT32 m_StaTm;//开始统计时间戳
	UINT  m_LastIdx;//最后帧序号
	UINT32 m_LastTm;//最后帧时间戳

	CNetMgr m_Net;
	int ConnectJG(void);
	int DisConnJG(void);
	int AqcSingleWv();
	int AqcContiWv();
	int StopContiWv();
	int SendStateCmd(void);
	int CheckPermit(char *pbuff,int len);
	int RegRecCallBack(PFCB_JG_RECV pf);
	void StaticLostWave(UINT CurIndx,UINT32 CurTm);
	int SendBeatCmd(void);
};

#endif // !defined(AFX_JGPROXY_H__CF78EAE2_B075_49EE_A81A_66A33924584F__INCLUDED_)

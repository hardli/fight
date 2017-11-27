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
    BOOL m_Permited;//��֤��־


	//��֡ͳ��
	int m_LsIdxCnt;//֡��Ų���������
	int m_LsTmCnt;//ʱ�䲻��������
	UINT m_StaIdx;//��ʼͳ��֡
	UINT32 m_StaTm;//��ʼͳ��ʱ���
	UINT  m_LastIdx;//���֡���
	UINT32 m_LastTm;//���֡ʱ���

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

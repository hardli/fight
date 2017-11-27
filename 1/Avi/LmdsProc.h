#ifndef _LmdsProc_H_
#define _LmdsProc_H_

#include <vector>

using namespace std;

#include <afxwin.h>
#include <stdio.h>
#include <io.h>
#include <iostream>
//#include <list>


using namespace std;

//#include <windows.h>


#define LMDLEN 843
#define LMDSMAXLEN 1678
#define MAX_LMDSNUN 1
#define AUTO_FRAMCNT 18

typedef struct tagWaveList
{
	vector<CString> m_DirLmdList;//��ǰ�ļ�����lmd�б�
	vector<CString> m_LmdsList; //��ѡlmds�б�
	vector<CString> m_DirList; //�ļ������ļ����б�
}WaveList;



typedef struct tagReadWaveSet
{
	int nReadI;//��ǰ��������
	int nNum;//��ǰ��Ҫ��������
	int nChannel;//����ͨ�����
	int nCurLmdsI;//��ǰLmds���
	int nCurDirI;//��ǰĿ¼���
	int nCurState;//��ǰ����״̬��-1--�����˻ص���㣻0--��ʼ��ȡLMDS��1--��ʼ��ȡ�ļ���;2--��ȡ�����һ���ļ�
	int nAutoEm;//�Զ����ӿհ�֡����
	int nAutoEn;//�Զ����ӿհ�֡ʹ��;
	int nForOut;//ǿ����β��־
	char ItemName[30];//�ļ�������
}ReadWaveSet;



typedef struct tagLmdInfo
{
	int nCh;//ͨ��
	unsigned char lmddata1[831];//��һ·����
	unsigned char lmddata2[831];//�ڶ�·����
	char timestamp[12];//ʱ���
}LmdInfo;


class CLmdProc
{
public:
	CLmdProc();
	~CLmdProc();
	
public:
	WaveList m_WaveSet;//���ز��εļ�¼��
	ReadWaveSet m_ReadSet;//��ȡ���εļ�¼��
	
public:
	BOOL IsLmdsFileCheck(CString filepath);//�Ƿ���lmds�ļ��ж� ����ȫ·��
	void SearchFolder(CString path);//�����ļ��� һ���ļ���
	void SearchChildFd(CString path);//�����ڶ����ļ���;
	void ClearWaveSet(void);//��ղ����б�
	void SearchWave(CString path);//��Ѱ����·���Ĳ����ļ�
	void ReadNextFram(LmdInfo *pLmd);//��ȡ��һ֡
	void ReadPrevFram(LmdInfo *pLmd);//��ȡ��һ֡
	BOOL ParseLmdsFile(CString path);//����lmds�ļ�
	BOOL GetLmdByIndx(int nIndex,LmdInfo *pLmd);
	BOOL ReadSingleLmd(CString filepath,LmdInfo *pLmd);//��ȡ����lmd�ļ�
	void ResetReadSet(void);
	void GetItemName(char *pItenName,CString filepath);
	
};

#endif
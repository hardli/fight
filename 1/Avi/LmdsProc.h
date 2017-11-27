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
	vector<CString> m_DirLmdList;//当前文件夹下lmd列表
	vector<CString> m_LmdsList; //所选lmds列表
	vector<CString> m_DirList; //文件夹下文件夹列表
}WaveList;



typedef struct tagReadWaveSet
{
	int nReadI;//当前读的索引
	int nNum;//当前需要读的总数
	int nChannel;//波形通道情况
	int nCurLmdsI;//当前Lmds序号
	int nCurDirI;//当前目录序号
	int nCurState;//当前读的状态；-1--倒着退回到起点；0--开始读取LMDS；1--开始读取文件夹;2--读取到最后一个文件
	int nAutoEm;//自动增加空白帧计数
	int nAutoEn;//自动增加空白帧使能;
	int nForOut;//强制收尾标志
	char ItemName[30];//文件夹名称
}ReadWaveSet;



typedef struct tagLmdInfo
{
	int nCh;//通道
	unsigned char lmddata1[831];//第一路波形
	unsigned char lmddata2[831];//第二路波形
	char timestamp[12];//时间戳
}LmdInfo;


class CLmdProc
{
public:
	CLmdProc();
	~CLmdProc();
	
public:
	WaveList m_WaveSet;//记载波形的记录集
	ReadWaveSet m_ReadSet;//读取波形的记录集
	
public:
	BOOL IsLmdsFileCheck(CString filepath);//是否是lmds文件判断 输入全路径
	void SearchFolder(CString path);//遍历文件夹 一级文件夹
	void SearchChildFd(CString path);//遍历第二级文件夹;
	void ClearWaveSet(void);//清空波形列表
	void SearchWave(CString path);//搜寻所给路径的波形文件
	void ReadNextFram(LmdInfo *pLmd);//读取下一帧
	void ReadPrevFram(LmdInfo *pLmd);//读取上一帧
	BOOL ParseLmdsFile(CString path);//解析lmds文件
	BOOL GetLmdByIndx(int nIndex,LmdInfo *pLmd);
	BOOL ReadSingleLmd(CString filepath,LmdInfo *pLmd);//读取单个lmd文件
	void ResetReadSet(void);
	void GetItemName(char *pItenName,CString filepath);
	
};

#endif
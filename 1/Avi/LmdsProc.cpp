#include "StdAfx.h"
#include "LmdsProc.h"


CLmdProc::~CLmdProc()
{
	
}

CLmdProc::CLmdProc()
{
	ClearWaveSet();
}

void CLmdProc::ClearWaveSet()
{
	//清空列表
	m_WaveSet.m_DirLmdList.clear();
	m_WaveSet.m_DirList.clear();
	memset(&m_ReadSet,0,sizeof(m_ReadSet));
}


void CLmdProc::ResetReadSet()
{
	m_ReadSet.nCurDirI=0;
	m_ReadSet.nCurState=0;
	m_ReadSet.nCurLmdsI=0;
	m_ReadSet.nNum=0;
	m_WaveSet.m_DirList.clear();
	m_WaveSet.m_DirLmdList.clear();
	m_WaveSet.m_LmdsList.clear();
}

void CLmdProc::SearchWave(CString path)
{
	BOOL ret = 0;
	CString TmpDir="";
	WIN32_FIND_DATA FindFileData;
	
	ResetReadSet();

	if(path == _T(""))
	{
		return;
	}
	else
	{
		if(path.Right(1) != _T("\\"))
		{
			TmpDir = path ;
		}
		else
		{
			TmpDir =path.Left(path.GetLength()-1);
		}
	}
	
	HANDLE hFind=::FindFirstFile(TmpDir,&FindFileData);
	if(INVALID_HANDLE_VALUE!=hFind)
	{
		//判断一个路径是文件还是文件夹
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!='.')
			{
				SearchFolder(path);
			}
		}
		else if (IsLmdsFileCheck(path))
		{
			m_WaveSet.m_LmdsList.push_back(path);
		}
	}
	FindClose(hFind);
}



void CLmdProc::SearchChildFd(CString path)
{
	CFileFind fileFind;
	CString strtmp="";
	CString TmpDir="";
	BOOL ret = 0;
	if(path == _T(""))
	{
		return;
	}
	else
	{
		if(path.Right(1) != _T("\\"))
			TmpDir = path + L"\\";
		else
			TmpDir = path;
		TmpDir =TmpDir+_T("*.*");
	}
	ret = fileFind.FindFile(TmpDir);
	while (ret)
	{
		ret = fileFind.FindNextFile();
		if (fileFind.IsDots())
			continue;
		
		if (fileFind.IsDirectory())
		{
			continue;
		}
		else
		{
			strtmp = fileFind.GetFilePath();
			if(_tcsstr(".lmd",strtmp.Right(4)))
			{
				m_WaveSet.m_DirLmdList.push_back(strtmp);
			}
		}				
	}
	fileFind.Close();
}


void CLmdProc::SearchFolder(CString path)
{
	CFileFind fileFind;
	CString strtmp="";
	CString TmpDir="";
	BOOL ret = 0;
	if(path == _T(""))
	{
		return;
	}
	else
	{
		if(path.Right(1) != _T("\\"))
			TmpDir = path + L"\\";
		else
			TmpDir = path;
		
		TmpDir =TmpDir+_T("*.*");
	}
	ret = fileFind.FindFile(TmpDir);
	while (ret)
	{
		ret = fileFind.FindNextFile();
		if (fileFind.IsDots())
			continue;
		
		if (fileFind.IsDirectory())
		{
			m_WaveSet.m_DirList.push_back(fileFind.GetFilePath());
		}
		else
		{
			strtmp = fileFind.GetFilePath();
			if(_tcsstr(".lmd",strtmp.Right(4)))
			{
				//m_WaveSet.m_LmdList.push_back(strtmp);
				m_WaveSet.m_DirList.push_back(path);
				fileFind.Close();
				return;
			}
			else if (_tcsstr(".lmds",strtmp.Right(5)))
			{
				m_WaveSet.m_LmdsList.push_back(strtmp);
			}
		}				
	}
	fileFind.Close();
}


BOOL CLmdProc::IsLmdsFileCheck(CString filepath)
{
	CString Ext=".lmds";
	if (_tcsstr(Ext,filepath.Right(5)))
	{
		return true;
	}
	return false;
}



BOOL CLmdProc::GetLmdByIndx(int nIndex,LmdInfo *pLmd)
{
	FILE *fp_r=NULL;
	DWORD FileSize=0;
	char LMDSBUF[LMDSMAXLEN]={0};
	int nOffset=0;
	fp_r=fopen(m_WaveSet.m_LmdsList[m_ReadSet.nCurLmdsI],"rb");
	if(fp_r == NULL)
	{
		//不允许读数据
		return FALSE;
	}  
	if (m_ReadSet.nChannel == 1)	
	{
		nOffset=LMDLEN*nIndex;
		fseek(fp_r,nOffset,SEEK_SET);
		fread(LMDSBUF,sizeof(char),LMDLEN ,fp_r);
		fclose(fp_r);
		if(LMDSBUF[0]  ==  0x02  &&  LMDSBUF[1]  ==  0x02  &&  LMDSBUF[2]  ==  0x02  &&  LMDSBUF[3]  ==  0x02 )
		{
			//1路波形解析
			pLmd->nCh=1;
			memcpy(pLmd->lmddata1,LMDSBUF,831);
			memcpy(pLmd->timestamp,&LMDSBUF[831],12);
		}
		else 
		{
			return FALSE;
		}
	}
	else if (m_ReadSet.nChannel == 2)
	{
		nOffset=(LMDLEN+831)*nIndex;
		fseek(fp_r,nOffset,SEEK_SET);
		fread(LMDSBUF,sizeof(char),LMDLEN+831 ,fp_r);
		fclose(fp_r);
		if (LMDSBUF[0]  ==  0x02  &&  LMDSBUF[1]  ==  0x02  &&  LMDSBUF[2]  ==  0x02  &&  LMDSBUF[3]  ==  0x02 
			&&  LMDSBUF[831]  ==  0x02  &&  LMDSBUF[832]  ==  0x02  &&  LMDSBUF[833]  ==  0x02  &&  LMDSBUF[834]  ==  0x02)
		{
			//2路波形解析
			pLmd->nCh=2;
			memcpy(pLmd->lmddata1,LMDSBUF,831);
			memcpy(pLmd->lmddata2,&LMDSBUF[831],831);
			memcpy(pLmd->timestamp,&LMDSBUF[1662],12);
		}
		else 
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}



BOOL CLmdProc::ParseLmdsFile(CString path)
{
	FILE *fp_r=NULL;
	DWORD FileSize=0;
	char LMDSBUF[LMDSMAXLEN]={0};
	fp_r=fopen(path,"rb");
	if(fp_r == NULL)
	{
		//不允许读数据
		return false;
	}    
	
	fseek(fp_r,0,SEEK_SET);
	fread(LMDSBUF,sizeof(char),LMDSMAXLEN ,fp_r);
	
	if(LMDSBUF[0]  ==  0x02  &&  LMDSBUF[1]  ==  0x02  &&  LMDSBUF[2]  ==  0x02  &&  LMDSBUF[3]  ==  0x02 
		&&  LMDSBUF[843]  ==  0x02  &&  LMDSBUF[844]  ==  0x02  &&  LMDSBUF[845]  ==  0x02  &&  LMDSBUF[846]  ==  0x02)
	{
		//1路波形解析
		fseek(fp_r, 0, SEEK_END); 
		FileSize=ftell(fp_r);
		m_ReadSet.nChannel=1;
		m_ReadSet.nNum=FileSize/LMDLEN;
	}
	else if (LMDSBUF[0]  ==  0x02  &&  LMDSBUF[1]  ==  0x02  &&  LMDSBUF[2]  ==  0x02  &&  LMDSBUF[3]  ==  0x02 
		&&  LMDSBUF[831]  ==  0x02  &&  LMDSBUF[832]  ==  0x02  &&  LMDSBUF[833]  ==  0x02  &&  LMDSBUF[834]  ==  0x02
		&& LMDSBUF[1674]  ==  0x02  &&  LMDSBUF[1675]  ==  0x02  &&  LMDSBUF[1676]  ==  0x02  &&  LMDSBUF[1677]  ==  0x02)
	{
		//2路波形解析
		fseek(fp_r, 0, SEEK_END); 
		FileSize=ftell(fp_r);
		m_ReadSet.nChannel=2;
		m_ReadSet.nNum=FileSize/(LMDLEN+831);
	}
	else 
	{
		return false;
	}
	fclose(fp_r);
	return true;
}




void CLmdProc::ReadNextFram(LmdInfo *pLmd)
{
	//读取lmds文件
	
	if (m_ReadSet.nCurState<0)
	{
		m_ReadSet.nCurState=0;
	}
	
	if (m_ReadSet.nCurState==0 )
	{
		if	(m_WaveSet.m_LmdsList.size()>0)
		{
			if (m_ReadSet.nCurLmdsI<0)
			{
				m_ReadSet.nCurLmdsI=0;
			}
			
			if (m_ReadSet.nCurLmdsI<m_WaveSet.m_LmdsList.size())
			{
				if (m_ReadSet.nReadI==0)
				{
					ParseLmdsFile(m_WaveSet.m_LmdsList[m_ReadSet.nCurLmdsI]);
					GetItemName(m_ReadSet.ItemName,m_WaveSet.m_LmdsList[m_ReadSet.nCurLmdsI]);
				}
				
				if (m_ReadSet.nReadI<m_ReadSet.nNum)
				{
					GetLmdByIndx(m_ReadSet.nReadI,pLmd);
					m_ReadSet.nReadI++;
				}
				if (m_ReadSet.nReadI+1>m_ReadSet.nNum)
				{
					m_ReadSet.nCurLmdsI++;
					m_ReadSet.nReadI=0;
					m_ReadSet.nNum=0;
				}
				return;
			}
			else
			{
				m_ReadSet.nNum=0;
				m_ReadSet.nReadI=0;
				m_ReadSet.nCurState=1;
			}
		}
		else
		{
			m_ReadSet.nNum=0;
			m_ReadSet.nReadI=0;
			m_ReadSet.nCurState=1;
		}
	}
	
	
	if (m_ReadSet.nCurState==1 )
	{
		if(m_WaveSet.m_DirList.size()>0)
		{
			if (m_ReadSet.nCurDirI<0)
			{
				m_ReadSet.nCurDirI=0;
			}
			
			if (m_ReadSet.nCurDirI<m_WaveSet.m_DirList.size())
			{
				if (m_ReadSet.nReadI==0)
				{
					m_WaveSet.m_DirLmdList.clear();
					SearchChildFd(m_WaveSet.m_DirList[m_ReadSet.nCurDirI]);
					GetItemName(m_ReadSet.ItemName,m_WaveSet.m_DirList[m_ReadSet.nCurDirI]);
					m_ReadSet.nNum=m_WaveSet.m_DirLmdList.size();
				}
				
				if (m_ReadSet.nReadI<m_ReadSet.nNum)
				{
					ReadSingleLmd(m_WaveSet.m_DirLmdList[m_ReadSet.nReadI],pLmd);
					m_ReadSet.nReadI++;
				}
				
				if (m_ReadSet.nReadI+1>m_ReadSet.nNum)
				{
					if (m_ReadSet.nAutoEn>0&&m_ReadSet.nAutoEm<AUTO_FRAMCNT)
					{
						memset(&pLmd->lmddata1[85],0,361*2);
						m_ReadSet.nReadI=m_ReadSet.nReadI-1;
						m_ReadSet.nAutoEm++;
					}
					else
					{
						m_ReadSet.nReadI=0;
						m_ReadSet.nNum=0;
						m_ReadSet.nCurDirI++;
						m_ReadSet.nAutoEm=0;
					}
					
				}
				return;
			}
			else
			{
				m_ReadSet.nNum=0;
				m_ReadSet.nReadI=0;
				m_ReadSet.nCurState=2;
			}
		}
		else
		{
			m_ReadSet.nNum=0;
			m_ReadSet.nReadI=0;
			m_ReadSet.nCurState=2;
		}
	}
	
}

void CLmdProc::GetItemName(char *pItenName,CString filepath)
{
	CString str="";
	str=filepath.Mid(filepath.ReverseFind( '\\')+1);
	if (str.Find(".lmds")!=-1)
	{
		memcpy(pItenName,str.GetBuffer(str.GetLength()),min(str.GetLength()-5,30));
		str.ReleaseBuffer();
	}
	else
	{
		memcpy(pItenName,str.GetBuffer(str.GetLength()),min(str.GetLength(),30));
		str.ReleaseBuffer();
	}
}


void CLmdProc::ReadPrevFram(LmdInfo *pLmd)
{
	if (m_ReadSet.nCurState>1)
	{
		m_ReadSet.nCurState=1;
	}
	
	if (m_ReadSet.nCurState==1 )
	{
		if(m_WaveSet.m_DirList.size()>0)
		{
			
			if (m_ReadSet.nCurDirI+1>m_WaveSet.m_DirList.size())
			{
				m_ReadSet.nCurDirI--;
			}
			
			if (m_ReadSet.nCurDirI>-1)
			{
				if (m_ReadSet.nReadI==0)
				{
					m_WaveSet.m_DirLmdList.clear();
					SearchChildFd(m_WaveSet.m_DirList[m_ReadSet.nCurDirI]);
					GetItemName(m_ReadSet.ItemName,m_WaveSet.m_DirList[m_ReadSet.nCurDirI]);
					m_ReadSet.nReadI=m_WaveSet.m_DirLmdList.size();
					m_ReadSet.nNum=m_WaveSet.m_DirLmdList.size();
				}
				
				if (m_ReadSet.nReadI>0)
				{
					m_ReadSet.nReadI--;
					ReadSingleLmd(m_WaveSet.m_DirLmdList[m_ReadSet.nReadI],pLmd);
				}
				
				if (m_ReadSet.nReadI<1)
				{
					m_ReadSet.nCurDirI--;
					m_ReadSet.nReadI=0;
					m_ReadSet.nNum=0;
				}
				return;
			}
			else
			{
				m_ReadSet.nCurDirI=0;
				m_ReadSet.nNum=0;
				m_ReadSet.nReadI=0;
				m_ReadSet.nCurState=0;
			}
		}
		else
		{
			m_ReadSet.nNum=0;
			m_ReadSet.nReadI=0;
			m_ReadSet.nCurState=0;
		}
	}
	
	//读取lmds文件
	if (m_ReadSet.nCurState==0)
	{
		if( m_WaveSet.m_LmdsList.size()>0)
		{
			if (m_ReadSet.nCurLmdsI+1>m_WaveSet.m_LmdsList.size())
			{
				m_ReadSet.nCurLmdsI--;
			}
			
			if (m_ReadSet.nCurLmdsI>-1)
			{
				if (m_ReadSet.nReadI==0)
				{
					ParseLmdsFile(m_WaveSet.m_LmdsList[m_ReadSet.nCurLmdsI]);
					GetItemName(m_ReadSet.ItemName,m_WaveSet.m_LmdsList[m_ReadSet.nCurLmdsI]);
					m_ReadSet.nReadI=m_ReadSet.nNum;
				}
				if (m_ReadSet.nReadI>0)
				{
					m_ReadSet.nReadI--;
					GetLmdByIndx(m_ReadSet.nReadI,pLmd);
					
				}
				if (m_ReadSet.nReadI<1)
				{
					m_ReadSet.nCurLmdsI--;
					m_ReadSet.nReadI=0;
					m_ReadSet.nNum=0;
				}
				return;
			}
			else
			{
				m_ReadSet.nNum=0;
				m_ReadSet.nReadI=0;
				m_ReadSet.nCurState=-1;
			}
		}
		else
		{
			m_ReadSet.nNum=0;
			m_ReadSet.nReadI=0;
			m_ReadSet.nCurState=-1;
		}
	}
}



BOOL CLmdProc::ReadSingleLmd(CString filepath,LmdInfo *pLmd)
{
	int nFileLen=0;
	FILE * l_pFile = NULL;
	if ((l_pFile = fopen(filepath,"rb+")) != NULL) //打开成功
	{
		fseek(l_pFile, 0L, SEEK_END);  //文件尾部
		nFileLen = ftell(l_pFile); //文件长度
		if (nFileLen == 831)
		{
			memset(pLmd->lmddata1, 0, 831);
			pLmd->nCh=1;
			fseek(l_pFile, 0L, SEEK_SET);
			if (fread(pLmd->lmddata1, 831,1,l_pFile) < 1)  //读取失败
			{
				
			}
			fclose(l_pFile);
		}
		else if (nFileLen == 1661)
		{
			memset(pLmd->lmddata1, 0, 831);
			memset(pLmd->lmddata2, 0, 831);
			pLmd->nCh=2;
			fseek(l_pFile, 0L, SEEK_SET);
			if (fread(pLmd->lmddata1, 831,1,l_pFile) < 1)  //读取失败
			{
				
			}
			
			fseek(l_pFile, 831, SEEK_SET);
			if (fread(pLmd->lmddata2, 831,1,l_pFile) < 1)  //读取失败
			{
				
			}
			
			fclose(l_pFile);
		}
		memcpy(pLmd->timestamp,filepath.Right(12),12);
	}
	return TRUE;
}













#include "stdafx.h"
#include "Avi_Main.h"
#include "IniProc.h"
#include "Avi.h"

char IniPath[1024]={0};
/**************************************
功能：写系统日志
参数：日志消息 年月日
返回：
**************************************/
bool WriteLogFile(char* msg)
{
	time_t now_time=time(NULL);
	char tmp[64]={0}; 
	struct tm *p;
	p=localtime(&now_time);//
	sprintf(tmp,"%d-%02d-%02d %02d:%02d:%02d ",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	char sum[2048]="";
	strcpy(sum,tmp);
	strcat(sum,msg);
	strcat(sum,"\r\n");
	
	char tempbuf[1024]={0};
//	GetCurrentDirectory(256,tempbuf);

	GetWJLogRoot(tempbuf);
	string filepath=tempbuf;

	char fileName[50]={'\0'};
	sprintf(fileName,"\\%d-%02d-%02d.txt",p->tm_year+1900,p->tm_mon+1,p->tm_mday);
	filepath += fileName;
	
	FILE* fp;
	if ((fp=fopen(filepath.c_str(),"a+"))==NULL)
	{
		return false;
	}
	fwrite(sum,sizeof(char),strlen(sum),fp);
	fclose(fp);
	
	return true;
}

/************************************************************************/
/* 获取InI存储路径                                                      */
/************************************************************************/
char * GetIniRoot(void)
{
	GetCurrentDirectory(1024, IniPath);
	strcat(IniPath, "\\WJ_Avi.ini");
	return IniPath;
}

/************************************************************************/
/* read all para                                                        */
/************************************************************************/
void ReadAllPara(void)
{
	char *pPath=NULL;
	char LogBuf[255]={0};
	pPath=GetIniRoot();
	sprintf(LogBuf, "IniPath: %s", IniPath);
	WriteLogFile(LogBuf);
	WriteLogFile("Read Set from Ini");
	ReadModelPara(pPath);
	ReadLaserPara(pPath);
	ReadSysParam(pPath);
	ReadVoutPara(pPath);
	ReadWvFilePara(pPath);
	ReadQuePara(pPath);
	ReadCamPara(pPath);
}

/************************************************************************/
/* 读取模型参数                                                         */
/************************************************************************/
void ReadModelPara(char *pPath)
{
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	//算法配置[ModelSet]
	g_ModelSet.IsLanHt=		GetPrivateProfileInt("ModelSet", "IsLanHt",		800, pPath);
	g_ModelSet.IsLanWd=		GetPrivateProfileInt("ModelSet", "IsLanWd",		300, pPath);
	g_ModelSet.JGHeight=	GetPrivateProfileInt("ModelSet", "JGHeight",	1500, pPath);
	g_ModelSet.LaneWidth=	GetPrivateProfileInt("ModelSet", "LaneWidth",	2800, pPath);
	g_ModelSet.MaxHt=		GetPrivateProfileInt("ModelSet", "MaxHt",		5500, pPath);
	g_ModelSet.StartX=		GetPrivateProfileInt("ModelSet", "StartX",		800, pPath);
	g_ModelSet.ZeroPos=		GetPrivateProfileInt("ModelSet", "ZeroPos",		320, pPath);
}


/************************************************************************/
/* 存储模型参数                                                         */
/************************************************************************/
AVIMAIN void __stdcall SaveModelPara(char *pPath)
{
	char temp[30]={0};
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	WritePrivateProfileString("ModelSet", "IsLanHt",  itoa(g_ModelSet.IsLanHt, temp, 10), pPath);
	WritePrivateProfileString("ModelSet", "IsLanWd",  itoa(g_ModelSet.IsLanWd, temp, 10), pPath);
	WritePrivateProfileString("ModelSet", "JGHeight", itoa(g_ModelSet.JGHeight, temp, 10), pPath);
	WritePrivateProfileString("ModelSet", "LaneWidth", itoa(g_ModelSet.JGHeight, temp, 10), pPath);
	WritePrivateProfileString("ModelSet", "MaxHt",	  itoa(g_ModelSet.MaxHt, temp, 10), pPath);
	WritePrivateProfileString("ModelSet", "StartX",	  itoa(g_ModelSet.StartX, temp, 10), pPath);
	WritePrivateProfileString("ModelSet", "ZeroPos",  itoa(g_ModelSet.ZeroPos, temp, 10), pPath);
}

/************************************************************************/
/* 读取激光参数                                                         */
/************************************************************************/
void ReadLaserPara(char *pPath)
{
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	g_LaserSet.AqcMode=		GetPrivateProfileInt("LaserSet", "AqcMode",		1, pPath);
	GetPrivateProfileString("LaserSet", "IP0", "192.168.0.2", g_LaserSet.IP0, sizeof(g_LaserSet.IP0), pPath);
	GetPrivateProfileString("LaserSet", "IP1", "192.168.0.3", g_LaserSet.IP1, sizeof(g_LaserSet.IP1), pPath);
	g_LaserSet.Port0=		GetPrivateProfileInt("LaserSet", "Port0",		2110, pPath);
	g_LaserSet.Port1=		GetPrivateProfileInt("LaserSet", "Port1",		2112, pPath);
	
}

/************************************************************************/
/* 存储激光器参                                                         */
/************************************************************************/
AVIMAIN void __stdcall SaveLaserPara(char *pPath)
{
	char temp[30]={0};
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	WritePrivateProfileString("LaserSet", "AqcMode",  itoa(g_LaserSet.AqcMode, temp, 10), pPath);
	WritePrivateProfileString("LaserSet", "IP0",	g_LaserSet.IP0, pPath);
	WritePrivateProfileString("LaserSet", "IP1",	g_LaserSet.IP1, pPath);
	WritePrivateProfileString("LaserSet", "Port0",	 itoa(g_LaserSet.Port0, temp, 10), pPath);
	WritePrivateProfileString("LaserSet", "Port1",	 itoa(g_LaserSet.Port1, temp, 10), pPath);
}

/************************************************************************/
/* 读取系统参数                                                         */
/************************************************************************/
void ReadSysParam(char *pPath)
{
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	g_SysTem.DirEnable=		GetPrivateProfileInt("SysTem", "DirEnable",	0, pPath);
	g_SysTem.DirType=		GetPrivateProfileInt("SysTem", "DirType",		0, pPath);
	g_SysTem.PlotImg=		GetPrivateProfileInt("SysTem", "PlotImg",		0, pPath);
	g_SysTem.ProjectId=		GetPrivateProfileInt("SysTem", "ProjectId",	1, pPath);
	g_SysTem.RunMode=		GetPrivateProfileInt("SysTem", "RunMode",		0, pPath);
	g_SysTem.SvDB=			GetPrivateProfileInt("SysTem", "SvDB",		0, pPath);
	g_SysTem.SvWv=		GetPrivateProfileInt("SysTem", "SvWave",		0, pPath);
	g_SysTem.WvCheck =		GetPrivateProfileInt("SysTem", "WvCheck",		0, pPath);
}
/************************************************************************/
/* 存储系统参数                                                         */
/************************************************************************/
AVIMAIN void __stdcall SaveSysPara(char *pPath)
{
	char temp[30]={0};
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	WritePrivateProfileString("SysTem", "DirEnable",itoa(g_SysTem.DirEnable, temp, 10), pPath);
	WritePrivateProfileString("SysTem", "DirType", itoa(g_SysTem.DirType, temp, 10), pPath);
	WritePrivateProfileString("SysTem", "PlotImg", itoa(g_SysTem.PlotImg, temp, 10), pPath);
	WritePrivateProfileString("SysTem", "ProjectId",itoa(g_SysTem.ProjectId, temp, 10), pPath);
	WritePrivateProfileString("SysTem", "RunMode",  itoa(g_SysTem.RunMode, temp, 10), IniPath);
	WritePrivateProfileString("SysTem", "SvDB",	  itoa(g_SysTem.SvDB, temp, 10), pPath);
	WritePrivateProfileString("SysTem", "SvWave",   itoa(g_SysTem.SvWv, temp, 10), pPath);
	WritePrivateProfileString("SysTem", "WvCheck",   itoa(g_SysTem.WvCheck, temp, 10), pPath);
}

/************************************************************************/
/* 读取出车参数                                                         */
/************************************************************************/
void ReadVoutPara(char *pPath)
{
	char temp[30]={0};
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	g_VOutSet.A3ForOut=		GetPrivateProfileInt("VoutSet", "A3ForOut",	0, pPath);
	g_VOutSet.AxReport=		GetPrivateProfileInt("VoutSet", "AxReport",	0, pPath);
	g_VOutSet.EmFramCnt=	GetPrivateProfileInt("VoutSet", "EmFramCnt",	10, pPath);
	g_VOutSet.GratHt=		GetPrivateProfileInt("VoutSet", "GratHt",		1800, pPath);
	g_VOutSet.GratPos=		GetPrivateProfileInt("VoutSet", "GratPos",		0, pPath);
	g_VOutSet.HiFramCnt=	GetPrivateProfileInt("VoutSet", "HiFramCnt",	40, pPath);
	g_VOutSet.LvFramCnt=	GetPrivateProfileInt("VoutSet", "LvFramCnt",	10, pPath);
	g_VOutSet.LwFramCnt=	GetPrivateProfileInt("VoutSet", "LwFramCnt",	15, pPath);
}

/************************************************************************/
/* 存储出车参数                                                         */
/************************************************************************/
AVIMAIN void __stdcall SaveVoutPara(char *pPath)
{
	char temp[30]={0};
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	WritePrivateProfileString("VoutSet", "A3ForOut",  itoa(g_VOutSet.A3ForOut, temp, 10), pPath);
	WritePrivateProfileString("VoutSet", "AxReport",  itoa(g_VOutSet.AxReport, temp, 10), pPath);
	WritePrivateProfileString("VoutSet", "EmFramCnt", itoa(g_VOutSet.EmFramCnt, temp, 10), pPath);
	WritePrivateProfileString("VoutSet", "GratHt",	   itoa(g_VOutSet.GratHt, temp, 10), pPath);
	WritePrivateProfileString("VoutSet", "HiFramCnt", itoa(g_VOutSet.HiFramCnt, temp, 10), pPath);
	WritePrivateProfileString("VoutSet", "LvFramCnt", itoa(g_VOutSet.LvFramCnt, temp, 10), pPath);
	WritePrivateProfileString("VoutSet", "LwFramCnt", itoa(g_VOutSet.LwFramCnt, temp, 10), pPath);
	
}

/************************************************************************/
/* read lmd file Set	                                                */
/************************************************************************/
void ReadWvFilePara(char *pPath)
{
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	g_WvFiles.AddEmFram=	GetPrivateProfileInt("WvFiles", "AddEmFram",	1, pPath);
	g_WvFiles.ForceOut=		GetPrivateProfileInt("WvFiles", "ForceOut",	1, pPath);
}

/************************************************************************/
/* save lmd file Set	                                                */
/************************************************************************/
AVIMAIN void __stdcall SaveWvFilePara(char *pPath)
{
	char temp[30]={0};
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	WritePrivateProfileString("WvFiles", "AddEmFram", itoa(g_WvFiles.AddEmFram, temp, 10), pPath);
	WritePrivateProfileString("WvFiles", "ForceOut",  itoa(g_WvFiles.ForceOut, temp, 10), pPath);
}

/************************************************************************/
/* read camera Set                                                      */
/************************************************************************/
void ReadCamPara(char *pPath)
{
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}	
	
	//摄像头配置[Avi_Camer]
	g_CamSet.CamEnable = GetPrivateProfileInt("CamSet", "CamEnable", 0, pPath);
	GetPrivateProfileString("CamSet", "CamIp", "192.168.2.223", g_CamSet.CamIp, sizeof(g_CamSet.CamIp), pPath);
	GetPrivateProfileString("CamSet", "CamPwd", "8080", g_CamSet.CamPwd, sizeof(g_CamSet.CamPwd), pPath);
	g_CamSet.CamPort = GetPrivateProfileInt("CamSet", "CamPort", 8080, pPath);
	g_CamSet.CamPicCar=GetPrivateProfileInt("CamSet", "CamPicCar", 0, pPath);
	g_CamSet.CamPicPlate=GetPrivateProfileInt("CamSet", "CamPicPlate", 0, pPath);
	
	g_CamSet.CamEnable1 = GetPrivateProfileInt("CamSet", "CamEnable1", 0, pPath);
	GetPrivateProfileString("CamSet", "CamIp1", "192.168.2.224", g_CamSet.CamIp1, sizeof(g_CamSet.CamIp1), pPath);
	GetPrivateProfileString("CamSet", "CamPwd1", "8080", g_CamSet.CamPwd1, sizeof(g_CamSet.CamPwd1), pPath);
	g_CamSet.CamPort1 = GetPrivateProfileInt("CamSet", "CamPort1", 8080, pPath);
	g_CamSet.CamPicCar1=GetPrivateProfileInt("CamSet", "CamPicCar1", 0, pPath);
	g_CamSet.CamPicPlate1=GetPrivateProfileInt("CamSet", "CamPicPlate1", 0, pPath);
}
/************************************************************************/
/* save Camera Set                                                      */
/************************************************************************/
AVIMAIN void __stdcall SaveCamPara(char *pPath)
{
	char temp[30]={0};
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	WritePrivateProfileString("CamSet", "CamEnable", itoa(g_CamSet.CamEnable, temp, 10), pPath);
	WritePrivateProfileString("CamSet", "CamIp",   g_CamSet.CamIp, pPath);
	WritePrivateProfileString("CamSet", "CamPwd",  g_CamSet.CamPwd, pPath);
	WritePrivateProfileString("CamSet", "CamPort", itoa(g_CamSet.CamPort, temp, 10), pPath);
	WritePrivateProfileString("CamSet", "CamPicCar", itoa(g_CamSet.CamPicCar, temp, 10), pPath);
	WritePrivateProfileString("CamSet", "CamPicPlate", itoa(g_CamSet.CamPicPlate, temp, 10), pPath);
	
	WritePrivateProfileString("CamSet", "CamEnable1", itoa(g_CamSet.CamEnable1, temp, 10), pPath);
	WritePrivateProfileString("CamSet", "CamIp1",   g_CamSet.CamIp1, pPath);
	WritePrivateProfileString("CamSet", "CamPwd1",  g_CamSet.CamPwd1, pPath);
	WritePrivateProfileString("CamSet", "CamPort1", itoa(g_CamSet.CamPort1, temp, 10), pPath);
	WritePrivateProfileString("CamSet", "CamPicCar1", itoa(g_CamSet.CamPicCar1, temp, 10), pPath);
	WritePrivateProfileString("CamSet", "CamPicPlate1", itoa(g_CamSet.CamPicPlate1, temp, 10), pPath);
	
}

/************************************************************************/
/* Read Que Set                                                         */
/************************************************************************/
void ReadQuePara(char *pPath)
{
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	g_QueSet.DelVehTime		=	GetPrivateProfileInt("QueSet", "DelVehTime", -1, IniPath);    //删除单车时间，默认6秒
	g_QueSet.EndVehWaitTime =	GetPrivateProfileInt("QueSet", "EndVehWaitTime", 1000, IniPath);   //强制收尾时等待出车时间
	g_QueSet.ClearQueTime	=	GetPrivateProfileInt("QueSet", "ClearQueTime", 300000, IniPath);   //出车后间隔一段时间清除队列
	g_QueSet.MaxVehCnt		=	GetPrivateProfileInt("QueSet", "MaxVehCnt", 4, IniPath);   //队列允许的最大车辆数
	g_QueSet.Quecheck		=	GetPrivateProfileInt("QueSet", "Quecheck", 0, IniPath);   //队列维护默认不开
	g_QueSet.TypecheckID	=	GetPrivateProfileInt("QueSet", "TypecheckID", 0, IniPath);   //车型纠正代码
	g_QueSet.IsETC			=	GetPrivateProfileInt("QueSet", "IsETC", 0, IniPath);   //车型纠正代码	
}
/************************************************************************/
/* save Que Set                                                         */
/************************************************************************/
AVIMAIN void __stdcall SaveQuePara(char *pPath)
{
	char temp[30]={0};
	if (pPath==NULL)
	{
		pPath=GetIniRoot();
	}
	WritePrivateProfileString("QueSet", "DelVehTime",	 itoa(g_QueSet.DelVehTime,    temp, 10), pPath);
	WritePrivateProfileString("QueSet", "EndVehWaitTime", itoa(g_QueSet.EndVehWaitTime, temp, 10), pPath);
	WritePrivateProfileString("QueSet", "ClearQueTime",   itoa(g_QueSet.ClearQueTime, temp, 10), pPath);
	WritePrivateProfileString("QueSet", "MaxVehCnt",	  itoa(g_QueSet.MaxVehCnt, temp, 10), pPath);
	WritePrivateProfileString("QueSet", "Quecheck",		 itoa(g_QueSet.Quecheck,   temp, 10), pPath);
	WritePrivateProfileString("QueSet", "TypecheckID",	 itoa(g_QueSet.Quecheck,   temp, 10), pPath);
	WritePrivateProfileString("QueSet", "IsETC",		 itoa(g_QueSet.IsETC,      temp, 10), pPath);
}

/************************************************************************/
/* save all Set                                                         */
/************************************************************************/
AVIMAIN void __stdcall SaveAllPara(void)
{
	char *pPath=NULL;
	pPath=GetIniRoot();
	SaveModelPara(pPath);
	SaveLaserPara(pPath);
	SaveSysPara(pPath);
	SaveVoutPara(pPath);
	SaveWvFilePara(pPath);
	SaveCamPara(pPath);
	SaveQuePara(pPath);
}


#include "stdafx.h"
#include "Avi_Algo.h"
#include "JZGlobal.h"
#include "Avi_Comm.h"
#include "Avi_MainProc.h"
#include "Avi_Main.h"
#include "Avi.h"
#include "IniProc.h"

SystemSet g_SysSet;//系统参数
VOutSet g_VoutSet;//准则参数
uint8 g_LmdCache[LMDMAXCACHE][Full_Size+1]; //激光数据缓存 500个；
uint8 JG0_CurBuff[Full_Size]={0};//μ±?°??2¨D?êy?Y	
uint16 g_LmdCacheIn; //两激光缓存计数；
uint16 g_LmdCacheOut; //两激光缓存计数；

PFFormImage g_pfFormImage=NULL;
HANDLE hd_Algo=NULL;
//车型算法线程及开关
DWORD WINAPI AlgoThread(LPVOID p);
BOOL AlgoThRunFlag=false;


/************************************************************************/
/* 设置构建车辆图像回调函数                                             */
/************************************************************************/
int __stdcall RegFormImage(PFFormImage pfFormImage)
{
	if (pfFormImage!=NULL)
	{
		g_pfFormImage=pfFormImage;
		return ERR_SUCC;
	}
	else
	{
		return ERR_PFAXL;
	}
}


/************************************************************************/
/* 触发强制收尾                                                         */
/************************************************************************/
int __stdcall TrigForceOut(void)
{
	//	g_DllAtState.ManHanded=1;
	//	if(CanBeForceOut(&g_VehicleSet))
	//	{
	//		return ERR_SUCC;
	//	}
	//	return ERR_SUCC;
	
	g_DllAtState.ManHanded=1;
	int force=CanBeForceOut(&g_VehicleSet);
	return force;
}


/************************************************************************/
/* 导入波形数据                                                         */
/************************************************************************/
void __stdcall InsertWave(uint8 *WvBuff,int nLen)
{
	g_AqcState.RecTotal++;
	if(WvBuff[0]==0x02 && WvBuff[1]==0x02 
		&& WvBuff[2]==0x02 && WvBuff[3]==0x02)
	{
		if(nLen == Full_Size)
		{
			if(((WvBuff[83]<<8)+WvBuff[84])==POINT_SUM)
			{
				memcpy(g_LmdCache[g_LmdCacheIn],WvBuff,Full_Size);
				g_LmdCache[g_LmdCacheIn][Full_Size]=10;	
				g_LmdCacheIn=g_LmdCacheIn+1;
				g_LmdCacheIn=g_LmdCacheIn%LMDMAXCACHE;
				g_AqcState.RecWvCnt++;
			}
			else
			{
				g_AqcState.RecWvErr++;
			}
		}
		else
		{
			g_AqcState.RecCmdCnt++;
		}
	}
	else
	{
		g_AqcState.RecErrCnt++;
	}
}

/************************************************************************/
/* 获取采集波形状态                                                     */
/************************************************************************/
int __stdcall GetWvState(AqcState *pAqc)
{
	if (pAqc!=NULL)
	{
		pAqc->ProWvCnt=g_AqcState.ProWvCnt;
		pAqc->RecTotal=g_AqcState.RecTotal;
		pAqc->RecWvCnt=g_AqcState.RecWvCnt;
		pAqc->RecWvErr=g_AqcState.RecWvErr;
		pAqc->RecErrCnt=g_AqcState.RecErrCnt;
		pAqc->RecCmdCnt=g_AqcState.RecCmdCnt;
		return ERR_SUCC;
	}
	else
	{
		return ERR_PTNUL;
	}
}


/************************************************************************/
/* 清空采集波形状态                                                     */
/************************************************************************/
int __stdcall ClrWvState(void)
{
	memset(&g_AqcState,0,sizeof(g_AqcState));
	return ERR_SUCC;
}


/************************************************************************/
/* 初始化全局缓存                                                       */
/************************************************************************/
uint8 InitGlobal(void)
{
	AlgoThRunFlag=false;
	g_LmdCacheOut=0;
	g_LmdCacheIn=0;
	
	memset(&g_VehicleSet,0,sizeof(g_VehicleSet));
	
	ClrWvState();
	
	//初始化采集波形状态
	memset(&g_AqcState,0,sizeof(g_AqcState));
	
	return ERR_SUCC;
}



/************************************************************************/
/* 启动引擎                                                             */
/************************************************************************/
uint8 StartEngine(void)
{
	
	//开启算法运行线程
	DWORD dwThreadId=0L;
	if (hd_Algo!=NULL)
	{
		AlgoThRunFlag=false;
		CloseHandle(hd_Algo);
	}
	AlgoThRunFlag=true;
    hd_Algo = CreateThread( 
        NULL,              // no security attribute 
        0,                 // default stack size 
        AlgoThread,           // WorkThread, 
        (LPVOID) 0,     // thread parameter 
        0,                 // not suspended 
        &dwThreadId);      // returns thread ID 
	
    if (hd_Algo == NULL) 
    {
        return ERR_THNUL;
    }
	else
		SetThreadPriority(hd_Algo, THREAD_PRIORITY_ABOVE_NORMAL);  
	return ERR_SUCC;
}




/************************************************************************/
/* 算法线程                                                             */
/************************************************************************/
DWORD WINAPI AlgoThread(LPVOID p)
{
	char LogBuf[255]={0};
	uint32 Len=0;
	int32 i=0;
	int32 j=0;
	while(AlgoThRunFlag)
	{
		try
		{
			if(g_LmdCacheOut!= g_LmdCacheIn)
			{	
				memset(JG0_CurBuff,0,sizeof(JG0_CurBuff));
				memcpy(JG0_CurBuff,g_LmdCache[g_LmdCacheOut],Full_Size);	
				g_LmdCacheOut = g_LmdCacheOut +1;
				g_LmdCacheOut = g_LmdCacheOut % LMDMAXCACHE;
				g_AqcState.ProWvCnt++;
				
				Len=(JG0_CurBuff[83]<<8)+JG0_CurBuff[84];
				memset(LMS_data_1,0,sizeof(LMS_data_1));
				
				if (Len == POINT_SUM)
				{ 
					for(i=85,j=0;j < POINT_SUM; i=i+2,j++)	
					{
						LMS_data_1[j]=	(JG0_CurBuff[i]<<8)+JG0_CurBuff[i+1];
						if(LMS_data_1[j] < 0)
							LMS_data_1[j] = 0;
					}
					
					//获取激光器时间戳	
					LMS_data_1[POINT_SUM] = ((JG0_CurBuff[42]<<24)+(JG0_CurBuff[43]<<16)+(JG0_CurBuff[44]<<8)+JG0_CurBuff[45])/1000;//±?D?3yò?1000 ·??ò???á?D?a??
					g_n32WaveNumber=LMS_data_1[POINT_SUM];
					
					//模拟版运行线圈信号来至波形
					if(g_SysSetUp.u8WorkMode)
					{
						memcpy(g_u8ItemName,&JG0_CurBuff[52],24);//模拟波形文件名字
					}
					VehIdentProc();
				}
			}
			else
			{
				Sleep(10);
			}
		}
		catch (...)
		{
			sprintf(LogBuf, "线程异常 算法线程");
			WriteLogFile(LogBuf);	
		}
	}
	WriteLogFile("算法线程结束");
	return 0L;
}





/************************************************************************/
/* 检查模型参数合法性                                                  */
/************************************************************************/
uint8 CheckMdlParam(void)
{	
	if(Smp.ZeroPos>POINT_END||Smp.ZeroPos<1)
	{
		//中心点超出范围
		return ERR_FAIL;
	}
	else if(Smp.IsLanWd<ISLANWD ||Smp.IsLanHt<ISLANHT)
	{
		//安全岛宽高不符合要求
		return ERR_FAIL;
	}
	else if(Smp.MaxHt<5500)
	{
		//最大净空高满足要求
		return ERR_FAIL;
	}
	return ERR_SUCC;
}

/************************************************************************/
/* 检查系统参数合法性	                                                */
/************************************************************************/
uint8 CheckSysParam(void)
{
	if (g_SysSet.DirEnable>2||g_SysSet.PlotImg>2
		||g_SysSet.ProjectId>3||g_SysSet.RunMode>3
		||g_SysSet.SvWv>2||g_SysSet.SvDB>2)
	{
		return ERR_FAIL;
	}
	
	return ERR_SUCC;
}


/************************************************************************/
/* 检查出车阈值合法性	                                                */
/************************************************************************/
uint8 CheckVoutParam(void)
{
	if (g_VoutSet.A3ForOut>2||g_VoutSet.EmFramCnt<10
		||g_VoutSet.GratHt<300||g_VoutSet.GratHt>1800
		||g_VoutSet.AxReport>2||g_VoutSet.GratPos>2)
	{
		return ERR_FAIL;
	}
	
	return ERR_SUCC;
}


/************************************************************************/
/* 查询模型参数                                                         */
/************************************************************************/
ModelSet __stdcall GetModelParam(void)
{
	ModelSet TmpMdSet;
	memset(&TmpMdSet,0,sizeof(TmpMdSet));
	memcpy(&TmpMdSet,&Smp,sizeof(TmpMdSet));
	return TmpMdSet;
}


/************************************************************************/
/* 设置模型参数                                                         */
/************************************************************************/
int __stdcall SetModelParam(ModelSet MdSet)
{
	//初始化模型参数ss
	memset(&Smp,0,sizeof(Smp));
	memcpy(&Smp,&MdSet,sizeof(MdSet));
	
	//第二步 检查参数合法性及生效
	if(CheckMdlParam()==ERR_FAIL)
	{
		return ERR_MdSET;
	}
	else
	{
		InitModeParam();
	}
	return ERR_SUCC;
}

/************************************************************************/
/* 查询系统参数                                                         */
/************************************************************************/
SystemSet __stdcall GetSysParam(void)
{
	return g_SysSet;
}

/************************************************************************/
/* 设置系统参数                                                         */
/************************************************************************/
int __stdcall SetSysParam(SystemSet SySet)
{
	//初始化系统参数
	memset(&g_SysSet,0,sizeof(g_SysSet));
	memcpy(&g_SysSet,&SySet,sizeof(g_SysSet));
	
	//第二步 检查参数合法性及生效
	if(CheckSysParam()==ERR_FAIL)
	{
		return ERR_SySet;
	}
	else
	{
		//全局参初始值
		g_SysSetUp.u8WorkMode=g_SysSet.RunMode;
		g_SysSetUp.u8DirDevEnable=g_SysSet.DirEnable;
		g_SysSetUp.u8DirDevType=g_SysSet.DirType;
	}
	return ERR_SUCC;
}



/************************************************************************/
/* 查询出车参数                                                         */
/************************************************************************/
VOutSet __stdcall GetVoutParam(void)
{
	return g_VoutSet;
}

/************************************************************************/
/* 设置出车参数                                                         */
/************************************************************************/
int __stdcall SetVoutParam(VOutSet VoSet)
{
	//初始化相关准则参数
	memset(&g_VoutSet,0,sizeof(g_VoutSet));
	memcpy(&g_VoutSet,&VoSet,sizeof(g_VoutSet));
	
	if (CheckVoutParam()==ERR_FAIL)
	{
		return ERR_VOSet;
	}
	else
	{
		g_VehOutSet.u16GratHt=g_VoutSet.GratHt;
		g_VehOutSet.u8Ax3Force=g_VoutSet.A3ForOut;
		g_VehOutSet.u8DirHiCnt=g_VoutSet.HiFramCnt;
		g_VehOutSet.u8DirLvCnt=g_VoutSet.LvFramCnt;
		g_VehOutSet.u8DirLwCnt=g_VoutSet.LwFramCnt;
		g_VehOutSet.u8EmptyFrCnt=g_VoutSet.EmFramCnt;
		g_VehOutSet.u8GratPos=g_VoutSet.GratPos;
		g_WeigthAt.u8wAxleReprot=g_VoutSet.AxReport;
	}
	return ERR_SUCC;
}

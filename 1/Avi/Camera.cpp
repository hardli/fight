#include "stdafx.h"
#include  "Camera.h"
//#include  "Common.h"
#include <io.h>
#include  <afx.h>
#include  <string.h>
#include <oaidl.h>
#include "IniProc.h"
#include "Avi_Main.h"
//#include "FileUtils.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Shlwapi.lib")
#define CAMETEST 1

#if CAMETEST
#pragma comment(lib, "ice_ipcsdk.lib")
#endif
/*******************双摄像头说明
序号：默认激光器摄像头为 0  岗亭摄像头为1
触发：0 软触发  1硬触发
*/
int  InitCamera();		//两个初始化摄像头 0、1判断标志位触发
void  ConnectCamera();	//两个初连接注册摄像头 0、1判断标志位触发
void  OnCapture();		//0摄像头软触发
void Closecarmer(void);
void  GetCarnumInfo();
void  ReadModelParam();
void colortest();//临时测试
void mkdir_p(const char *path);
//0相机 设置车牌回调函数
#if CAMETEST
void __stdcall SDK_OnPlate0(void *pvParam, const ICE_CHAR *pcIP,
							const ICE_CHAR *pcNumber, const ICE_CHAR *pcColor,
							ICE_CHAR *pcPicData, ICE_U32 u32PicLen, ICE_CHAR *pcCloseUpPicData, ICE_U32 u32CloseUpPicLen,
							ICE_S16 s16Speed, ICE_S16 s16VehicleType, ICE_S16 s16Reserved1, ICE_S16 s16Reserved2,
							ICE_FLOAT fPlateConfidence, ICE_U32 u32VehicleColor, ICE_U32 u32PlateType, ICE_U32 u32VehicleDir,
							ICE_U32 u32AlarmType, ICE_U32 u32SerialNum, ICE_U32 uCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow);


void  ICEIPCSDKPlateSdk0(LPCTSTR bstrIP, LPCTSTR bstrNumber, LPCTSTR bstrColor, long nPicData, long nPicLen, long nCloseUpPicData, long nCloseUpPicLen, long nSpeed, long nVehicleType, long nReserved1, long nReserved2,
						 float fPlateConfidence, long nVehicleColor, long nPlateType, long nVehicleDir, long nAlarmType, long u32SerialNum, long nCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow, int index);

//1相机 设置车牌回调函数
void __stdcall SDK_OnPlate1(void *pvParam, const ICE_CHAR *pcIP,
							const ICE_CHAR *pcNumber, const ICE_CHAR *pcColor,
							ICE_CHAR *pcPicData, ICE_U32 u32PicLen, ICE_CHAR *pcCloseUpPicData, ICE_U32 u32CloseUpPicLen,
							ICE_S16 s16Speed, ICE_S16 s16VehicleType, ICE_S16 s16Reserved1, ICE_S16 s16Reserved2,
							ICE_FLOAT fPlateConfidence, ICE_U32 u32VehicleColor, ICE_U32 u32PlateType, ICE_U32 u32VehicleDir,
							ICE_U32 u32AlarmType, ICE_U32 u32SerialNum, ICE_U32 uCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow);


void  ICEIPCSDKPlateSdk1(LPCTSTR bstrIP, LPCTSTR bstrNumber, LPCTSTR bstrColor, long nPicData, long nPicLen, long nCloseUpPicData, long nCloseUpPicLen, long nSpeed, long nVehicleType, long nReserved1, long nReserved2,
						 float fPlateConfidence, long nVehicleColor, long nPlateType, long nVehicleDir, long nAlarmType, long u32SerialNum, long nCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow, int index);

#endif



//****************0摄像头全局变量
HANDLE m_hSDK0[1];
SAFEARRAY *m_psa0[1];
BOOL m_bExit0; 
char  CameraInfo0[20]={0};
CString   strIP0;
char g_cplate_path0[512]={0};//车牌图片保存绝对路径

//***************1摄像头全局变量
HANDLE m_hSDK1[1];
SAFEARRAY *m_psa1[1];
BOOL m_bExit1;
char   CameraInfo1[20]={0};
CString   strIP1;
char g_cplate_path1[512]={0};//车牌图片保存绝对路径


SYSTEMTIME g_systime;//系统时间结构体

int  InitCamera()
{
	//	colortest();
	//	WriteLogFile("初始化摄像头....");
	char LogBuf[255]={0};
	if (g_CamSet.CamEnable==1)//0摄像头标志位
	{
		memset(CameraInfo0, 0, sizeof(CameraInfo0));
		if (NULL != m_psa0[0])
		{
			SafeArrayDestroy(m_psa0[0]);
			m_psa0[0] = NULL;
		}
		SAFEARRAYBOUND sab0 = { 4194304, 0 };
		m_psa0[0] = SafeArrayCreate(VT_I1, 1, &sab0);
		if (NULL == m_psa0[0])
		{
			WriteLogFile("初始相机0失败");		
		}
		else
		{
			WriteLogFile("初始相机0成功");
			//		CString ip = "192.168.2.153";		
			//		char strip[20] = {0};
			//		strip = (LPSTR)(LPCTSTR)strIP0;
			//		char *strip = NULL;
			//		strip = (LPSTR)(LPCTSTR)g_CamSet.CamIp;
			//		strip = (LPSTR)(LPCTSTR)strIP0;
			//		strip = (LPSTR)(LPCTSTR)ip;
			//		memcpy(strip,g_CamSet.CamIp,sizeof(g_CamSet.CamIp));
#if CAMETEST
			m_hSDK0[0] = ICE_IPCSDK_OpenDevice(g_CamSet.CamIp);
#endif
			if (m_hSDK0[0] == NULL)
			{
				WriteLogFile("连接相机0失败m_hSDK0[0] == NULL");
				//	MessageBox(NULL, "相机0连接失败！", "提示", MB_ICONINFORMATION | MB_OK);
			}
			else
			{
#if CAMETEST
				ICE_IPCSDK_SetPlateCallback(m_hSDK0[0], SDK_OnPlate0, NULL);//设置车牌回调
#endif
				sprintf(LogBuf, "连接相机0成功:%s", g_CamSet.CamIp);				
				WriteLogFile(LogBuf);
				//*************车牌保存路径********
			//	GetCurrentDirectory(256, g_cplate_path0);
				GetCameraRoot(g_cplate_path0);
				strcat(g_cplate_path0, "\\Plate0\\");
			}
		}		
	}
	else
	{
		//写日志 提示 摄像头0未使能
		WriteLogFile("摄像头0 未使能");
	}
	if (g_CamSet.CamEnable1==1)//1摄像头标志位
	{
		memset(CameraInfo1, 0, sizeof(CameraInfo1));
		if (NULL != m_psa1[0])
		{
			SafeArrayDestroy(m_psa1[0]);
			m_psa1[0]= NULL;
		}		
		SAFEARRAYBOUND sab1 = { 4194304, 0 };
		m_psa1[0] = SafeArrayCreate(VT_I1, 1, &sab1);
		if (NULL == m_psa1[0])
		{
			WriteLogFile("初始相机1 失败");
		}
		else
		{
			WriteLogFile("初始相机1 成功");
			
			//		char ip[20] = "192.168.2.154";
			//		char *strip = NULL;
			//		strip = (LPSTR)(LPCTSTR)ip;
			//			strip = (LPSTR)(LPCTSTR)g_CamSet.CamIp1;
			//		memcpy(strip,g_CamSet.CamIp1,sizeof(g_CamSet.CamIp1));
#if CAMETEST
			m_hSDK1[0] = ICE_IPCSDK_OpenDevice(g_CamSet.CamIp1);
#endif
			if (m_hSDK1[0] == NULL)
			{
				WriteLogFile("连接相机1 失败m_hSDK1[0] == NULL");
				//	MessageBox(NULL, "相机1连接失败！", "提示", MB_ICONINFORMATION | MB_OK);
			}
			else
			{
				sprintf(LogBuf, "连接相机1 成功:%s", g_CamSet.CamIp1);	
				WriteLogFile(LogBuf);
#if CAMETEST
				ICE_IPCSDK_SetPlateCallback(m_hSDK1[0], SDK_OnPlate1, NULL);//设置车牌回调
#endif
				//*************车牌保存路径********
				//GetCurrentDirectory(256, g_cplate_path1);
				GetCameraRoot(g_cplate_path1);
				strcat(g_cplate_path1, "\\Plate1\\");
			
			}
		}		
	}
	else
	{
		//写日志 提示 摄像头1未使能
		WriteLogFile("摄像头1未使能");
	}
	
	return 1;
}
void Closecarmer(void)
{
	
	if((m_hSDK0[0]!=0)&&(g_CamSet.CamEnable==1))
	{
#if CAMETEST
		ICE_IPCSDK_Close(m_hSDK0[0]);  //断开连接
#endif
		m_hSDK0[0]=0;
	}
	else
	{
		WriteLogFile("摄像头0断开fail");
	}
	
	if((m_hSDK1[0]!=0)&&(g_CamSet.CamEnable1==1))
	{
#if CAMETEST
		ICE_IPCSDK_Close(m_hSDK1[0]);  //断开连接
#endif
		m_hSDK1[0]=0;
	}
	else
	{
		WriteLogFile("摄像头1断开fail");
	}
	
	if (NULL != m_psa0[0])
	{
		SafeArrayDestroy(m_psa0[0]);
		m_psa0[0] = NULL;
	}	
	if (NULL != m_psa1[0])
	{
		SafeArrayDestroy(m_psa1[0]);
		m_psa1[0]= NULL;
	}	
	
}

#if CAMETEST
void __stdcall SDK_OnPlate1(void *pvParam, const ICE_CHAR *pcIP, 
							const ICE_CHAR *pcNumber, const ICE_CHAR *pcColor, 
							ICE_CHAR *pcPicData, ICE_U32 u32PicLen, ICE_CHAR *pcCloseUpPicData, ICE_U32 u32CloseUpPicLen, 
							ICE_S16 s16Speed, ICE_S16 s16VehicleType, ICE_S16 s16Reserved1, ICE_S16 s16Reserved2, 
							ICE_FLOAT fPlateConfidence, ICE_U32 u32VehicleColor, ICE_U32 u32PlateType, ICE_U32 u32VehicleDir, 
							ICE_U32 u32AlarmType, ICE_U32 u32SerialNum, ICE_U32 uCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow)
{
	
	
	ICEIPCSDKPlateSdk1(pcIP, pcNumber, pcColor, 
		(long)pcPicData, u32PicLen, (long)pcCloseUpPicData, u32CloseUpPicLen, 
		s16Speed, s16VehicleType, s16Reserved1, s16Reserved2, 
		fPlateConfidence, u32VehicleColor, u32PlateType, u32VehicleDir, u32AlarmType,u32SerialNum,
		uCapTime, u32ResultHigh, u32ResultLow, 0);
}
void __stdcall SDK_OnPlate0(void *pvParam, const ICE_CHAR *pcIP,
							const ICE_CHAR *pcNumber, const ICE_CHAR *pcColor,
							ICE_CHAR *pcPicData, ICE_U32 u32PicLen, ICE_CHAR *pcCloseUpPicData, ICE_U32 u32CloseUpPicLen,
							ICE_S16 s16Speed, ICE_S16 s16VehicleType, ICE_S16 s16Reserved1, ICE_S16 s16Reserved2,
							ICE_FLOAT fPlateConfidence, ICE_U32 u32VehicleColor, ICE_U32 u32PlateType, ICE_U32 u32VehicleDir,
							ICE_U32 u32AlarmType, ICE_U32 u32SerialNum, ICE_U32 uCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow)
{
	
	
	ICEIPCSDKPlateSdk0(pcIP, pcNumber, pcColor,
		(long)pcPicData, u32PicLen, (long)pcCloseUpPicData, u32CloseUpPicLen,
		s16Speed, s16VehicleType, s16Reserved1, s16Reserved2,
		fPlateConfidence, u32VehicleColor, u32PlateType, u32VehicleDir, u32AlarmType, u32SerialNum,
		uCapTime, u32ResultHigh, u32ResultLow, 0);
}
#endif
void  ConnectCamera() 
{
	char LogBuf[255]={0};
	if (g_CamSet.CamEnable == 1)//连接摄像头0
	{
		if (0 != m_hSDK0[0])
		{
			WriteLogFile("连接相机 0失败");
			return;
		}
		CString ip = "192.168.2.153";		
		char *strip = NULL;
		//		ReadModelParam();
		
		strip = (LPSTR)(LPCTSTR)g_CamSet.CamIp;
		//		strip = (LPSTR)(LPCTSTR)strIP0;
		//		strip = (LPSTR)(LPCTSTR)ip;
#if CAMETEST
		m_hSDK0[0] = ICE_IPCSDK_OpenDevice(strip);
#endif
		if (m_hSDK0[0] == NULL)
		{
			WriteLogFile("连接相机 0失败m_hSDK0[0] == NULL");
			//	MessageBox(NULL, "相机0连接失败！", "提示", MB_ICONINFORMATION | MB_OK);
			//		return;
		}
#if CAMETEST
		ICE_IPCSDK_SetPlateCallback(m_hSDK0[0], SDK_OnPlate0, NULL);//设置车牌回调
#endif
		sprintf(LogBuf, "连接相机 0成功:%s", g_CamSet.CamIp);				
		WriteLogFile(LogBuf);
	}
	else
	{
		//写日志 提示 摄像头0未使能
		WriteLogFile("摄像头0 未使能");
	}
	if (g_CamSet.CamEnable1 == 1)//连接摄像头1
	{
		if (0 != m_hSDK1[0])
		{
			WriteLogFile("连接相机 1失败");
			return;
		}
		
		//		char ip[20] = "192.168.2.153";
		char *strip = NULL;
		strip = (LPSTR)(LPCTSTR)g_CamSet.CamIp1;
#if CAMETEST
		m_hSDK1[0] = ICE_IPCSDK_OpenDevice(strip);
#endif
		if (m_hSDK1[0] == NULL)
		{
			WriteLogFile("连接相机 1失败m_hSDK1[0] == NULL");
			//	MessageBox(NULL, "相机1连接失败！", "提示", MB_ICONINFORMATION | MB_OK);
			return;
		}
		sprintf(LogBuf, "连接相机 1成功:%s", g_CamSet.CamIp1);	
		WriteLogFile(LogBuf);
#if CAMETEST
		ICE_IPCSDK_SetPlateCallback(m_hSDK1[0], SDK_OnPlate1, NULL);//设置车牌回调
#endif
	}
	else
	{
		//写日志 提示 摄像头1未使能
		WriteLogFile("摄像头1 未使能");
	}
}
#if CAMETEST
void  ICEIPCSDKPlateSdk1(LPCTSTR bstrIP, LPCTSTR bstrNumber, LPCTSTR bstrColor, long nPicData, long nPicLen, long nCloseUpPicData, long nCloseUpPicLen, long nSpeed, long nVehicleType, long nReserved1, long nReserved2, 
						 float fPlateConfidence, long nVehicleColor, long nPlateType, long nVehicleDir, long nAlarmType, long u32SerialNum, long nCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow, int index)
{
	// TODO: 在此处添加消息处理程序代码
	char LogBuf[255]={0};
	CString	tempplate = "";
	tempplate.Format("%s%s",bstrColor,bstrNumber);
//	memcpy(tempplate.GetBuffer(tempplate.GetLength()),bstrColor,2);//取出颜色  单字
//	memcpy(tempplate.GetBuffer(tempplate.GetLength())+2,bstrNumber,strlen(bstrNumber)*2);//_tcslen(str) * sizeof(TCHAR);
	
	memset(CameraInfo1,0,sizeof(CameraInfo1));//清空车牌缓存
	
	if(tempplate.GetLength()<20)
	{
		memcpy(CameraInfo1, tempplate.GetBuffer(tempplate.GetLength()),tempplate.GetLength());
	}
	else
	{
		memcpy(CameraInfo1,tempplate.GetBuffer(19),19);
	}
	
	tempplate.ReleaseBuffer();
	sprintf(LogBuf, " 实时车牌 1:%s",CameraInfo1);
	WriteLogFile(LogBuf);
	//	AfxMessageBox(CameraInfo1);
	
	//******************保存车牌**********************
	
#ifdef VERSION64
	ICE_VDC_PICTRUE_INFO_S *info = (ICE_VDC_PICTRUE_INFO_S *)((uint64_t)(u32ResultHigh<<32) + (uint64_t)u32ResultLow);
#else
	ICE_VDC_PICTRUE_INFO_S *info = (ICE_VDC_PICTRUE_INFO_S *)u32ResultLow;
#endif
	
	char path[256]={0};
	GetLocalTime(&g_systime);
	sprintf(path, "%s\\%04d%02d%02d", 
		g_cplate_path1, g_systime.wYear, g_systime.wMonth, g_systime.wDay);
	
	mkdir_p(path);
	char name[1024];
	char picName[256];
	
	if ((nPicLen > 0)&&(g_CamSet.CamPicCar1==1))//存图
	{
		memset(name, 0, sizeof(name));
		memset(picName, 0, sizeof(picName));
		if (NULL != info && NULL != info->pstVbrResult)
			sprintf(picName, "%04d%02d%02d%02d%02d%02d_%s_%s.jpg", 
			g_systime.wYear, g_systime.wMonth, g_systime.wDay, g_systime.wHour, g_systime.wMinute,
			g_systime.wSecond, info->pstVbrResult->szLogName, bstrNumber);
		else
			sprintf(picName, "%04d%02d%02d%02d%02d%02d_%s.jpg", 
			g_systime.wYear, g_systime.wMonth, g_systime.wDay, 
			g_systime.wHour, g_systime.wMinute, g_systime.wSecond, bstrNumber);
		sprintf(name, "%s\\%s", path, picName);
		
		if (-1 != (_access(name, 0)))
		{
			int count = 1;
			while (count <= 10)
			{
				sprintf(name, "%s\\%04d%02d%02d%02d%02d%02d_%s_%d.jpg", 
					path, g_systime.wYear, g_systime.wMonth, g_systime.wDay, 
					g_systime.wHour, g_systime.wMinute, g_systime.wSecond, bstrNumber, count++);
				if (-1 == (_access(name, 0)))
					break;
			}
		}
		
		FILE *fp = fopen(name, "wb");
		if (NULL != fp)
		{
			fwrite((void*)nPicData, 1, nPicLen, fp);
			fclose(fp);
		}
		//		if (NULL != info && NULL != info->pstVbrResult)
		//			StoreResFeature(info->pstVbrResult, path, picName, (LPSTR)bstrNumber);//存特征码
		//		if (NULL != info && NULL != info->pPhoneMacList)
		//			StorePhoneMacList(info->pPhoneMacList, path, picName, (LPSTR)bstrNumber);//存mac列表
	}
	
	if ((nCloseUpPicLen > 0)&&(g_CamSet.CamPicPlate1==1))//存车牌图
	{
		memset(name, 0, sizeof(name));
		if (NULL != info && NULL != info->pstVbrResult)
			sprintf(name, "%s\\%04d%02d%02d%02d%02d%02d_%s_%s_closeup.jpg", 
			path, g_systime.wYear, g_systime.wMonth, g_systime.wDay, 
			g_systime.wHour, g_systime.wMinute, g_systime.wSecond,info->pstVbrResult->szLogName, bstrNumber);
		else
			sprintf(name, "%s\\%04d%02d%02d%02d%02d%02d_%s_closeup.jpg", 
			path, g_systime.wYear, g_systime.wMonth, g_systime.wDay, 
			g_systime.wHour, g_systime.wMinute, g_systime.wSecond, bstrNumber);
		if (-1 != (_access(name, 0)))
		{
			int count = 1;
			while (count <= 10)
			{
				sprintf(name, "%s\\%04d%02d%02d%02d%02d%02d_%s_closeup_%d.jpg", 
					path, g_systime.wYear, g_systime.wMonth, g_systime.wDay, 
					g_systime.wHour, g_systime.wMinute, g_systime.wSecond, bstrNumber, count++);
				if (-1 == (_access(name, 0)))
					break;
			}
		}
		
		FILE *fp = fopen(name, "wb");
		if (NULL != fp)
		{
			fwrite((void*)nCloseUpPicData, 1, nCloseUpPicLen, fp);
			fclose(fp);
		}
	}
} 
void  ICEIPCSDKPlateSdk0(LPCTSTR bstrIP, LPCTSTR bstrNumber, LPCTSTR bstrColor, long nPicData, long nPicLen, long nCloseUpPicData, long nCloseUpPicLen, long nSpeed, long nVehicleType, long nReserved1, long nReserved2,
						 float fPlateConfidence, long nVehicleColor, long nPlateType, long nVehicleDir, long nAlarmType, long u32SerialNum, long nCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow, int index)
{
	// TODO: 在此处添加消息处理程序代码
	char LogBuf[255]={0};
	CString	tempplate = "";
	tempplate.Format("%s%s",bstrColor,bstrNumber);
//	memcpy(tempplate.GetBuffer(tempplate.GetLength()),bstrColor,2);//取出颜色  单字
//	memcpy(tempplate.GetBuffer(tempplate.GetLength())+2,bstrNumber,strlen(bstrNumber)*2);//_tcslen(str) * sizeof(TCHAR);
	memset(CameraInfo0,0,sizeof(CameraInfo0));//清空车牌缓存
	
	if(tempplate.GetLength()<20)
	{
		memcpy(CameraInfo0, tempplate.GetBuffer(tempplate.GetLength()),tempplate.GetLength());
	}
	else
	{
		memcpy(CameraInfo0,tempplate.GetBuffer(19),19);
	}
	tempplate.ReleaseBuffer();
	sprintf(LogBuf, "实时车牌 0:%s",CameraInfo0);
	WriteLogFile(LogBuf);
	//	AfxMessageBox(CameraInfo0);
	//设置车牌回调函数提取	
	//******************保存车牌**********************
#ifdef VERSION64
	ICE_VDC_PICTRUE_INFO_S *info = (ICE_VDC_PICTRUE_INFO_S *)((uint64_t)(u32ResultHigh<<32) + (uint64_t)u32ResultLow);
#else
	ICE_VDC_PICTRUE_INFO_S *info = (ICE_VDC_PICTRUE_INFO_S *)u32ResultLow;
#endif
	
	char path[256]={0};
	GetLocalTime(&g_systime);
	sprintf(path, "%s\\%04d%02d%02d", 
		g_cplate_path0, g_systime.wYear, g_systime.wMonth, g_systime.wDay);
	
	mkdir_p(path);
	char name[1024];
	char picName[256];
	
	if ((nPicLen > 0)&&(g_CamSet.CamPicCar==1))//存图
	{
		memset(name, 0, sizeof(name));
		memset(picName, 0, sizeof(picName));
		if (NULL != info && NULL != info->pstVbrResult)
			sprintf(picName, "%04d%02d%02d%02d%02d%02d_%s_%s.jpg", 
			g_systime.wYear, g_systime.wMonth, g_systime.wDay, g_systime.wHour, g_systime.wMinute,
			g_systime.wSecond, info->pstVbrResult->szLogName, bstrNumber);
		else
			sprintf(picName, "%04d%02d%02d%02d%02d%02d_%s.jpg", 
			g_systime.wYear, g_systime.wMonth, g_systime.wDay, 
			g_systime.wHour, g_systime.wMinute, g_systime.wSecond, bstrNumber);
		sprintf(name, "%s\\%s", path, picName);
		
		if (-1 != (_access(name, 0)))
		{
			int count = 1;
			while (count <= 10)
			{
				sprintf(name, "%s\\%04d%02d%02d%02d%02d%02d_%s_%d.jpg", 
					path, g_systime.wYear, g_systime.wMonth, g_systime.wDay, 
					g_systime.wHour, g_systime.wMinute, g_systime.wSecond, bstrNumber, count++);
				if (-1 == (_access(name, 0)))
					break;
			}
		}
		
		FILE *fp = fopen(name, "wb");
		if (NULL != fp)
		{
			fwrite((void*)nPicData, 1, nPicLen, fp);
			fclose(fp);
		}
		//		if (NULL != info && NULL != info->pstVbrResult)
		//			StoreResFeature(info->pstVbrResult, path, picName, (LPSTR)bstrNumber);//存特征码
		//		if (NULL != info && NULL != info->pPhoneMacList)
		//			StorePhoneMacList(info->pPhoneMacList, path, picName, (LPSTR)bstrNumber);//存mac列表
	}
	
	if ((nCloseUpPicLen > 0)&&(g_CamSet.CamPicPlate==1))//存车牌图
	{
		memset(name, 0, sizeof(name));
		if (NULL != info && NULL != info->pstVbrResult)
			sprintf(name, "%s\\%04d%02d%02d%02d%02d%02d_%s_%s_closeup.jpg", 
			path, g_systime.wYear, g_systime.wMonth, g_systime.wDay, 
			g_systime.wHour, g_systime.wMinute, g_systime.wSecond,info->pstVbrResult->szLogName, bstrNumber);
		else
			sprintf(name, "%s\\%04d%02d%02d%02d%02d%02d_%s_closeup.jpg", 
			path, g_systime.wYear, g_systime.wMonth, g_systime.wDay, 
			g_systime.wHour, g_systime.wMinute, g_systime.wSecond, bstrNumber);
		if (-1 != (_access(name, 0)))
		{
			int count = 1;
			while (count <= 10)
			{
				sprintf(name, "%s\\%04d%02d%02d%02d%02d%02d_%s_closeup_%d.jpg", 
					path, g_systime.wYear, g_systime.wMonth, g_systime.wDay, 
					g_systime.wHour, g_systime.wMinute, g_systime.wSecond, bstrNumber, count++);
				if (-1 == (_access(name, 0)))
					break;
			}
		}
		
		FILE *fp = fopen(name, "wb");
		if (NULL != fp)
		{
			fwrite((void*)nCloseUpPicData, 1, nCloseUpPicLen, fp);
			fclose(fp);
		}
	}
	
	
}
#endif
//抓拍 软触发
void  OnCapture() 
{
	// TODO: Add your control notification handler code here
	
	
	if (0 == m_hSDK0[0])
		return;
	
	char number[32], color[64];
	char *buf;
	unsigned len;
	SafeArrayAccessData(m_psa0[0], (void**)&buf);
#if CAMETEST
	ICE_IPCSDK_Trigger(m_hSDK0[0], 
		number, color, buf, 4194304, &len);//软触发，会返回识别数据
#endif
	SafeArrayUnaccessData(m_psa0[0]);
	
}

void   GetCamerInfor( CString str)
{
	InitCamera();//初始化
	ConnectCamera();//网络连接
	// OnCapture();//进行抓拍 
	//str=CameraInfo0;
}
void  ReadModelParam()
{
	// TODO: 在此添加命令处理程序代码
	
	//CFileUtils reader;
	//reader.GetConfigfileString("Param","CameraIP","192.168.2.153",strIP.GetBuffer(MAX_PATH),MAX_PATH,"Config.ini");
	//strIP.ReleaseBuffer();
}
void colortest()
{
	char LogBuf[255]={0};
	CString	tempplate = "";
	LPCTSTR bstrColor="蓝色";
	LPCTSTR bstrNumber="湘E1C169";
	char camtest[20]={0};
	memcpy(tempplate.GetBuffer(tempplate.GetLength()),bstrColor,2);
	memcpy(tempplate.GetBuffer(tempplate.GetLength())+2,bstrNumber,strlen(bstrNumber)*2);//_tcslen(str) * sizeof(TCHAR);
	if(tempplate.GetLength()<20)
		strcpy(camtest, tempplate);
	else
		memcpy(camtest,tempplate.GetBuffer(20),20);
	sprintf(LogBuf, "摄像头0 :%s",camtest);
	tempplate.ReleaseBuffer();
	WriteLogFile(LogBuf);
}
//新建文件夹
void mkdir_p(const char *path)
{
	if (PathFileExists(path) || ('\0' == path[0]))
		return;
	
	CString strPath = path;
	mkdir_p(strPath.Left(strPath.ReverseFind('\\')));
	
	CreateDirectory(strPath, NULL);
}

/************************************************************************/
/* 初始相机                                                             */
/************************************************************************/
//int InitCamera(void)
//{
//	//1.2、初始化摄像头模块*/
//
//	NET_DVR_Init();
//	DWORD FLAG=0;
//	int ret = 0;
//	//设置连接时间与重连时间
//	NET_DVR_SetConnectTime(2000, 1);
//	NET_DVR_SetReconnect(10000, true);
//
//	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
//	lUserID = NET_DVR_Login_V30((char *)CarmerIP.data(), 8000, "admin",(char *)CarmerPassWord.data(), &struDeviceInfo);
//	if (lUserID < 0)
//	{
//		FLAG=NET_DVR_GetLastError();
//		sprintf(log_buf,"登录摄像头失败，代码:%d",FLAG);
//		WriteLogFile(log_buf);
//		return FALSE;
//	}
//	WriteLogFile("登录摄像头成功");
//	//设置报警回调函数
//	NET_DVR_SetDVRMessageCallBack_V30(MSesGCallback, NULL);
//
//	//启用布防
//	NET_DVR_SETUPALARM_PARAM struSetupParam={0};
//	struSetupParam.dwSize=sizeof(NET_DVR_SETUPALARM_PARAM);
//
//	//上传报警信息类型: 0- 老报警信息(NET_DVR_PLATE_RESULT), 1- 新报警信息(NET_ITS_PLATE_RESULT)
//	struSetupParam.byAlarmInfoType=1;
//	struSetupParam.byLevel=1;
//	lHandle = NET_DVR_SetupAlarmChan_V41(lUserID,&struSetupParam);
//	if (lHandle < 0)
//	{
//		FLAG=NET_DVR_GetLastError();
//
//		sprintf(log_buf,"摄像头布放失败，代码:%d",FLAG);
//		WriteLogFile(log_buf);
//		return FALSE;
//	}
//	WriteLogFile("摄像头布防成功");
//	return TRUE;
//}
//BOOL Closecarmer(void)
//{
//	DWORD FLAG=0;
//	//撤销布防上传通道
//	if (!NET_DVR_CloseAlarmChan_V30(lHandle))
//	{
//		FLAG=NET_DVR_GetLastError();
//		NET_DVR_Logout(lUserID);
//		//NET_DVR_Cleanup();
//		return FALSE;
//	} 
//	//注销用户
//	NET_DVR_Logout(lUserID);
//	//释放 SDK 资源
//	//NET_DVR_Cleanup();
//	return TRUE;
// }

/**************************************
说明：摄像头回调函数
参数：无
返回值：无
**************************************/
//void CALLBACK MSesGCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser)
//{
//	//char filename[100];
//	//FILE *fSnapPic=NULL;
//	//FILE *fSnapPicPlate=NULL;
//	//CString tempIP=pAlarmer->sDeviceIP;
//	//CString slog="";
//	switch (lCommand)
//	{
//	case COMM_UPLOAD_PLATE_RESULT:
//		{
//			NET_DVR_PLATE_RESULT struPlateResult={0};
//			memcpy(&struPlateResult, pAlarmInfo, sizeof(struPlateResult));
//			resStr=struPlateResult.struPlateInfo.sLicense;
//			strcpy(resch,resStr.c_str());
//			PlateInfo_flag=1;
//			WriteLogFile(resch);
//			break;
//		}
//	case COMM_ITS_PLATE_RESULT:
//		{
//			NET_ITS_PLATE_RESULT struITSPlateResult={0};
//			memcpy(&struITSPlateResult, pAlarmInfo, sizeof(struITSPlateResult));
//			resStr=struITSPlateResult.struPlateInfo.sLicense;
//			strcpy(resch,resStr.c_str());
//			PlateInfo_flag=1;
//			WriteLogFile(resch);
//
//			//CF_C_V12Dlg::old1_flag=0;
//			//CF_C_V12Dlg::new1_flag=1;
//			//memcpy(&CF_C_V12Dlg::struITSPlateResult_HC, pAlarmInfo, sizeof(CF_C_V12Dlg::struITSPlateResult_HC));
//
//			//CString LogData="";
//
//			//for (int i=0;i<CF_C_V12Dlg::struITSPlateResult_HC.dwPicNum;i++)
//			//{
//			//	LogData.Format("车牌号: %s",CF_C_V12Dlg::struITSPlateResult_HC.struPlateInfo.sLicense);
//			//	switch(CF_C_V12Dlg::struITSPlateResult_HC.struPlateInfo.byColor)//车牌颜色
//			//	{
//			//	case VCA_BLUE_PLATE:
//			//		LogData=LogData+" 车牌颜色：蓝色";
//			//		break;
//			//	case VCA_YELLOW_PLATE:
//			//		LogData=LogData+" 车牌颜色：黄色";
//			//		break;
//			//	case VCA_WHITE_PLATE:
//			//		LogData=LogData+" 车牌颜色：白色";
//			//		break;
//			//	case VCA_BLACK_PLATE:
//			//		LogData=LogData+" 车牌颜色：黑色";
//			//		break;
//			//	default:
//			//		break;
//			//	}
//
//			//	//保存场景图
//			//	if ((CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].dwDataLen != 0)&&(CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].byType==
//			//		1)||(CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].byType == 2))
//			//	{
//			//		CF_C_V12Dlg::sPic(0,CF_C_V12Dlg::struITSPlateResult_HC,i);
//			//	}
//			//	//车牌小图片
//			//	if ((CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].dwDataLen != 0)&&(CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].byType == 0))
//			//	{
//			//		CF_C_V12Dlg::sPic(1,CF_C_V12Dlg::struITSPlateResult_HC,i);
//			//	}
//
//			//	//其他信息处理......
//			//}
//			//CF_C_V12Dlg::uploadLog("设备1:1->上传车辆信息");
//
//			//LogData=LogData+'\n';	
//			//CF_C_V12Dlg::wLog(LogData);
//
//			////CF_C_V12Dlg::iNum++;
//			//::SendMessage(CF_C_V12Dlg::m_hwnd,WM_ADDLISTHC,0,0);
//			break;
//		}
//	default:
//		break;
//	}
//	return;
//}

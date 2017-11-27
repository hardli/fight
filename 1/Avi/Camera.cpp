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
/*******************˫����ͷ˵��
��ţ�Ĭ�ϼ���������ͷΪ 0  ��ͤ����ͷΪ1
������0 ����  1Ӳ����
*/
int  InitCamera();		//������ʼ������ͷ 0��1�жϱ�־λ����
void  ConnectCamera();	//����������ע������ͷ 0��1�жϱ�־λ����
void  OnCapture();		//0����ͷ����
void Closecarmer(void);
void  GetCarnumInfo();
void  ReadModelParam();
void colortest();//��ʱ����
void mkdir_p(const char *path);
//0��� ���ó��ƻص�����
#if CAMETEST
void __stdcall SDK_OnPlate0(void *pvParam, const ICE_CHAR *pcIP,
							const ICE_CHAR *pcNumber, const ICE_CHAR *pcColor,
							ICE_CHAR *pcPicData, ICE_U32 u32PicLen, ICE_CHAR *pcCloseUpPicData, ICE_U32 u32CloseUpPicLen,
							ICE_S16 s16Speed, ICE_S16 s16VehicleType, ICE_S16 s16Reserved1, ICE_S16 s16Reserved2,
							ICE_FLOAT fPlateConfidence, ICE_U32 u32VehicleColor, ICE_U32 u32PlateType, ICE_U32 u32VehicleDir,
							ICE_U32 u32AlarmType, ICE_U32 u32SerialNum, ICE_U32 uCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow);


void  ICEIPCSDKPlateSdk0(LPCTSTR bstrIP, LPCTSTR bstrNumber, LPCTSTR bstrColor, long nPicData, long nPicLen, long nCloseUpPicData, long nCloseUpPicLen, long nSpeed, long nVehicleType, long nReserved1, long nReserved2,
						 float fPlateConfidence, long nVehicleColor, long nPlateType, long nVehicleDir, long nAlarmType, long u32SerialNum, long nCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow, int index);

//1��� ���ó��ƻص�����
void __stdcall SDK_OnPlate1(void *pvParam, const ICE_CHAR *pcIP,
							const ICE_CHAR *pcNumber, const ICE_CHAR *pcColor,
							ICE_CHAR *pcPicData, ICE_U32 u32PicLen, ICE_CHAR *pcCloseUpPicData, ICE_U32 u32CloseUpPicLen,
							ICE_S16 s16Speed, ICE_S16 s16VehicleType, ICE_S16 s16Reserved1, ICE_S16 s16Reserved2,
							ICE_FLOAT fPlateConfidence, ICE_U32 u32VehicleColor, ICE_U32 u32PlateType, ICE_U32 u32VehicleDir,
							ICE_U32 u32AlarmType, ICE_U32 u32SerialNum, ICE_U32 uCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow);


void  ICEIPCSDKPlateSdk1(LPCTSTR bstrIP, LPCTSTR bstrNumber, LPCTSTR bstrColor, long nPicData, long nPicLen, long nCloseUpPicData, long nCloseUpPicLen, long nSpeed, long nVehicleType, long nReserved1, long nReserved2,
						 float fPlateConfidence, long nVehicleColor, long nPlateType, long nVehicleDir, long nAlarmType, long u32SerialNum, long nCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow, int index);

#endif



//****************0����ͷȫ�ֱ���
HANDLE m_hSDK0[1];
SAFEARRAY *m_psa0[1];
BOOL m_bExit0; 
char  CameraInfo0[20]={0};
CString   strIP0;
char g_cplate_path0[512]={0};//����ͼƬ�������·��

//***************1����ͷȫ�ֱ���
HANDLE m_hSDK1[1];
SAFEARRAY *m_psa1[1];
BOOL m_bExit1;
char   CameraInfo1[20]={0};
CString   strIP1;
char g_cplate_path1[512]={0};//����ͼƬ�������·��


SYSTEMTIME g_systime;//ϵͳʱ��ṹ��

int  InitCamera()
{
	//	colortest();
	//	WriteLogFile("��ʼ������ͷ....");
	char LogBuf[255]={0};
	if (g_CamSet.CamEnable==1)//0����ͷ��־λ
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
			WriteLogFile("��ʼ���0ʧ��");		
		}
		else
		{
			WriteLogFile("��ʼ���0�ɹ�");
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
				WriteLogFile("�������0ʧ��m_hSDK0[0] == NULL");
				//	MessageBox(NULL, "���0����ʧ�ܣ�", "��ʾ", MB_ICONINFORMATION | MB_OK);
			}
			else
			{
#if CAMETEST
				ICE_IPCSDK_SetPlateCallback(m_hSDK0[0], SDK_OnPlate0, NULL);//���ó��ƻص�
#endif
				sprintf(LogBuf, "�������0�ɹ�:%s", g_CamSet.CamIp);				
				WriteLogFile(LogBuf);
				//*************���Ʊ���·��********
			//	GetCurrentDirectory(256, g_cplate_path0);
				GetCameraRoot(g_cplate_path0);
				strcat(g_cplate_path0, "\\Plate0\\");
			}
		}		
	}
	else
	{
		//д��־ ��ʾ ����ͷ0δʹ��
		WriteLogFile("����ͷ0 δʹ��");
	}
	if (g_CamSet.CamEnable1==1)//1����ͷ��־λ
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
			WriteLogFile("��ʼ���1 ʧ��");
		}
		else
		{
			WriteLogFile("��ʼ���1 �ɹ�");
			
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
				WriteLogFile("�������1 ʧ��m_hSDK1[0] == NULL");
				//	MessageBox(NULL, "���1����ʧ�ܣ�", "��ʾ", MB_ICONINFORMATION | MB_OK);
			}
			else
			{
				sprintf(LogBuf, "�������1 �ɹ�:%s", g_CamSet.CamIp1);	
				WriteLogFile(LogBuf);
#if CAMETEST
				ICE_IPCSDK_SetPlateCallback(m_hSDK1[0], SDK_OnPlate1, NULL);//���ó��ƻص�
#endif
				//*************���Ʊ���·��********
				//GetCurrentDirectory(256, g_cplate_path1);
				GetCameraRoot(g_cplate_path1);
				strcat(g_cplate_path1, "\\Plate1\\");
			
			}
		}		
	}
	else
	{
		//д��־ ��ʾ ����ͷ1δʹ��
		WriteLogFile("����ͷ1δʹ��");
	}
	
	return 1;
}
void Closecarmer(void)
{
	
	if((m_hSDK0[0]!=0)&&(g_CamSet.CamEnable==1))
	{
#if CAMETEST
		ICE_IPCSDK_Close(m_hSDK0[0]);  //�Ͽ�����
#endif
		m_hSDK0[0]=0;
	}
	else
	{
		WriteLogFile("����ͷ0�Ͽ�fail");
	}
	
	if((m_hSDK1[0]!=0)&&(g_CamSet.CamEnable1==1))
	{
#if CAMETEST
		ICE_IPCSDK_Close(m_hSDK1[0]);  //�Ͽ�����
#endif
		m_hSDK1[0]=0;
	}
	else
	{
		WriteLogFile("����ͷ1�Ͽ�fail");
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
	if (g_CamSet.CamEnable == 1)//��������ͷ0
	{
		if (0 != m_hSDK0[0])
		{
			WriteLogFile("������� 0ʧ��");
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
			WriteLogFile("������� 0ʧ��m_hSDK0[0] == NULL");
			//	MessageBox(NULL, "���0����ʧ�ܣ�", "��ʾ", MB_ICONINFORMATION | MB_OK);
			//		return;
		}
#if CAMETEST
		ICE_IPCSDK_SetPlateCallback(m_hSDK0[0], SDK_OnPlate0, NULL);//���ó��ƻص�
#endif
		sprintf(LogBuf, "������� 0�ɹ�:%s", g_CamSet.CamIp);				
		WriteLogFile(LogBuf);
	}
	else
	{
		//д��־ ��ʾ ����ͷ0δʹ��
		WriteLogFile("����ͷ0 δʹ��");
	}
	if (g_CamSet.CamEnable1 == 1)//��������ͷ1
	{
		if (0 != m_hSDK1[0])
		{
			WriteLogFile("������� 1ʧ��");
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
			WriteLogFile("������� 1ʧ��m_hSDK1[0] == NULL");
			//	MessageBox(NULL, "���1����ʧ�ܣ�", "��ʾ", MB_ICONINFORMATION | MB_OK);
			return;
		}
		sprintf(LogBuf, "������� 1�ɹ�:%s", g_CamSet.CamIp1);	
		WriteLogFile(LogBuf);
#if CAMETEST
		ICE_IPCSDK_SetPlateCallback(m_hSDK1[0], SDK_OnPlate1, NULL);//���ó��ƻص�
#endif
	}
	else
	{
		//д��־ ��ʾ ����ͷ1δʹ��
		WriteLogFile("����ͷ1 δʹ��");
	}
}
#if CAMETEST
void  ICEIPCSDKPlateSdk1(LPCTSTR bstrIP, LPCTSTR bstrNumber, LPCTSTR bstrColor, long nPicData, long nPicLen, long nCloseUpPicData, long nCloseUpPicLen, long nSpeed, long nVehicleType, long nReserved1, long nReserved2, 
						 float fPlateConfidence, long nVehicleColor, long nPlateType, long nVehicleDir, long nAlarmType, long u32SerialNum, long nCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow, int index)
{
	// TODO: �ڴ˴������Ϣ����������
	char LogBuf[255]={0};
	CString	tempplate = "";
	tempplate.Format("%s%s",bstrColor,bstrNumber);
//	memcpy(tempplate.GetBuffer(tempplate.GetLength()),bstrColor,2);//ȡ����ɫ  ����
//	memcpy(tempplate.GetBuffer(tempplate.GetLength())+2,bstrNumber,strlen(bstrNumber)*2);//_tcslen(str) * sizeof(TCHAR);
	
	memset(CameraInfo1,0,sizeof(CameraInfo1));//��ճ��ƻ���
	
	if(tempplate.GetLength()<20)
	{
		memcpy(CameraInfo1, tempplate.GetBuffer(tempplate.GetLength()),tempplate.GetLength());
	}
	else
	{
		memcpy(CameraInfo1,tempplate.GetBuffer(19),19);
	}
	
	tempplate.ReleaseBuffer();
	sprintf(LogBuf, " ʵʱ���� 1:%s",CameraInfo1);
	WriteLogFile(LogBuf);
	//	AfxMessageBox(CameraInfo1);
	
	//******************���泵��**********************
	
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
	
	if ((nPicLen > 0)&&(g_CamSet.CamPicCar1==1))//��ͼ
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
		//			StoreResFeature(info->pstVbrResult, path, picName, (LPSTR)bstrNumber);//��������
		//		if (NULL != info && NULL != info->pPhoneMacList)
		//			StorePhoneMacList(info->pPhoneMacList, path, picName, (LPSTR)bstrNumber);//��mac�б�
	}
	
	if ((nCloseUpPicLen > 0)&&(g_CamSet.CamPicPlate1==1))//�泵��ͼ
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
	// TODO: �ڴ˴������Ϣ����������
	char LogBuf[255]={0};
	CString	tempplate = "";
	tempplate.Format("%s%s",bstrColor,bstrNumber);
//	memcpy(tempplate.GetBuffer(tempplate.GetLength()),bstrColor,2);//ȡ����ɫ  ����
//	memcpy(tempplate.GetBuffer(tempplate.GetLength())+2,bstrNumber,strlen(bstrNumber)*2);//_tcslen(str) * sizeof(TCHAR);
	memset(CameraInfo0,0,sizeof(CameraInfo0));//��ճ��ƻ���
	
	if(tempplate.GetLength()<20)
	{
		memcpy(CameraInfo0, tempplate.GetBuffer(tempplate.GetLength()),tempplate.GetLength());
	}
	else
	{
		memcpy(CameraInfo0,tempplate.GetBuffer(19),19);
	}
	tempplate.ReleaseBuffer();
	sprintf(LogBuf, "ʵʱ���� 0:%s",CameraInfo0);
	WriteLogFile(LogBuf);
	//	AfxMessageBox(CameraInfo0);
	//���ó��ƻص�������ȡ	
	//******************���泵��**********************
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
	
	if ((nPicLen > 0)&&(g_CamSet.CamPicCar==1))//��ͼ
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
		//			StoreResFeature(info->pstVbrResult, path, picName, (LPSTR)bstrNumber);//��������
		//		if (NULL != info && NULL != info->pPhoneMacList)
		//			StorePhoneMacList(info->pPhoneMacList, path, picName, (LPSTR)bstrNumber);//��mac�б�
	}
	
	if ((nCloseUpPicLen > 0)&&(g_CamSet.CamPicPlate==1))//�泵��ͼ
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
//ץ�� ����
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
		number, color, buf, 4194304, &len);//�������᷵��ʶ������
#endif
	SafeArrayUnaccessData(m_psa0[0]);
	
}

void   GetCamerInfor( CString str)
{
	InitCamera();//��ʼ��
	ConnectCamera();//��������
	// OnCapture();//����ץ�� 
	//str=CameraInfo0;
}
void  ReadModelParam()
{
	// TODO: �ڴ���������������
	
	//CFileUtils reader;
	//reader.GetConfigfileString("Param","CameraIP","192.168.2.153",strIP.GetBuffer(MAX_PATH),MAX_PATH,"Config.ini");
	//strIP.ReleaseBuffer();
}
void colortest()
{
	char LogBuf[255]={0};
	CString	tempplate = "";
	LPCTSTR bstrColor="��ɫ";
	LPCTSTR bstrNumber="��E1C169";
	char camtest[20]={0};
	memcpy(tempplate.GetBuffer(tempplate.GetLength()),bstrColor,2);
	memcpy(tempplate.GetBuffer(tempplate.GetLength())+2,bstrNumber,strlen(bstrNumber)*2);//_tcslen(str) * sizeof(TCHAR);
	if(tempplate.GetLength()<20)
		strcpy(camtest, tempplate);
	else
		memcpy(camtest,tempplate.GetBuffer(20),20);
	sprintf(LogBuf, "����ͷ0 :%s",camtest);
	tempplate.ReleaseBuffer();
	WriteLogFile(LogBuf);
}
//�½��ļ���
void mkdir_p(const char *path)
{
	if (PathFileExists(path) || ('\0' == path[0]))
		return;
	
	CString strPath = path;
	mkdir_p(strPath.Left(strPath.ReverseFind('\\')));
	
	CreateDirectory(strPath, NULL);
}

/************************************************************************/
/* ��ʼ���                                                             */
/************************************************************************/
//int InitCamera(void)
//{
//	//1.2����ʼ������ͷģ��*/
//
//	NET_DVR_Init();
//	DWORD FLAG=0;
//	int ret = 0;
//	//��������ʱ��������ʱ��
//	NET_DVR_SetConnectTime(2000, 1);
//	NET_DVR_SetReconnect(10000, true);
//
//	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
//	lUserID = NET_DVR_Login_V30((char *)CarmerIP.data(), 8000, "admin",(char *)CarmerPassWord.data(), &struDeviceInfo);
//	if (lUserID < 0)
//	{
//		FLAG=NET_DVR_GetLastError();
//		sprintf(log_buf,"��¼����ͷʧ�ܣ�����:%d",FLAG);
//		WriteLogFile(log_buf);
//		return FALSE;
//	}
//	WriteLogFile("��¼����ͷ�ɹ�");
//	//���ñ����ص�����
//	NET_DVR_SetDVRMessageCallBack_V30(MSesGCallback, NULL);
//
//	//���ò���
//	NET_DVR_SETUPALARM_PARAM struSetupParam={0};
//	struSetupParam.dwSize=sizeof(NET_DVR_SETUPALARM_PARAM);
//
//	//�ϴ�������Ϣ����: 0- �ϱ�����Ϣ(NET_DVR_PLATE_RESULT), 1- �±�����Ϣ(NET_ITS_PLATE_RESULT)
//	struSetupParam.byAlarmInfoType=1;
//	struSetupParam.byLevel=1;
//	lHandle = NET_DVR_SetupAlarmChan_V41(lUserID,&struSetupParam);
//	if (lHandle < 0)
//	{
//		FLAG=NET_DVR_GetLastError();
//
//		sprintf(log_buf,"����ͷ����ʧ�ܣ�����:%d",FLAG);
//		WriteLogFile(log_buf);
//		return FALSE;
//	}
//	WriteLogFile("����ͷ�����ɹ�");
//	return TRUE;
//}
//BOOL Closecarmer(void)
//{
//	DWORD FLAG=0;
//	//���������ϴ�ͨ��
//	if (!NET_DVR_CloseAlarmChan_V30(lHandle))
//	{
//		FLAG=NET_DVR_GetLastError();
//		NET_DVR_Logout(lUserID);
//		//NET_DVR_Cleanup();
//		return FALSE;
//	} 
//	//ע���û�
//	NET_DVR_Logout(lUserID);
//	//�ͷ� SDK ��Դ
//	//NET_DVR_Cleanup();
//	return TRUE;
// }

/**************************************
˵��������ͷ�ص�����
��������
����ֵ����
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
//			//	LogData.Format("���ƺ�: %s",CF_C_V12Dlg::struITSPlateResult_HC.struPlateInfo.sLicense);
//			//	switch(CF_C_V12Dlg::struITSPlateResult_HC.struPlateInfo.byColor)//������ɫ
//			//	{
//			//	case VCA_BLUE_PLATE:
//			//		LogData=LogData+" ������ɫ����ɫ";
//			//		break;
//			//	case VCA_YELLOW_PLATE:
//			//		LogData=LogData+" ������ɫ����ɫ";
//			//		break;
//			//	case VCA_WHITE_PLATE:
//			//		LogData=LogData+" ������ɫ����ɫ";
//			//		break;
//			//	case VCA_BLACK_PLATE:
//			//		LogData=LogData+" ������ɫ����ɫ";
//			//		break;
//			//	default:
//			//		break;
//			//	}
//
//			//	//���泡��ͼ
//			//	if ((CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].dwDataLen != 0)&&(CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].byType==
//			//		1)||(CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].byType == 2))
//			//	{
//			//		CF_C_V12Dlg::sPic(0,CF_C_V12Dlg::struITSPlateResult_HC,i);
//			//	}
//			//	//����СͼƬ
//			//	if ((CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].dwDataLen != 0)&&(CF_C_V12Dlg::struITSPlateResult_HC.struPicInfo[i].byType == 0))
//			//	{
//			//		CF_C_V12Dlg::sPic(1,CF_C_V12Dlg::struITSPlateResult_HC,i);
//			//	}
//
//			//	//������Ϣ����......
//			//}
//			//CF_C_V12Dlg::uploadLog("�豸1:1->�ϴ�������Ϣ");
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

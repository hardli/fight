#ifndef _AVI_MAINI_H_
#define _AVI_MAINI_H_


#include <process.h>
#include <time.h>
#include <direct.h>
#include <io.h>
#include <WinSock2.h>
#include <fstream>
#pragma comment(lib,"user32")
#include <sys/timeb.h>
#include <windows.h>
#include <string>
#include <afx.h>  // ��������
#include <windows.h>

#ifdef _DEBUG
#pragma comment(lib, "libcmtd.lib")
#else
#pragma comment(lib, "libcmt.lib")
#endif

#include<winsock2.h>
#include<stdio.h>

using namespace std;
//*******���ظ���ģ�鶯̬��*********//
#include "Avi_Algo.h"
#include "Avi_Plot.h"

//#include "HCNetSDK.h"
//#pragma comment(lib,"HCNetSDK.lib")//����
//void CALLBACK MSesGCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser);
//BOOL Initcarmer(void);
//BOOL Closecarmer(void);

//#include "dhnetsdk.h"
//#pragma comment(lib,"dhnetsdk.lib")//��SDK

//#include "HvDeviceBaseType.h"
//#include "HvDeviceCommDef.h"
//#include "HvDeviceNew.h"
//#pragma comment(lib, "HvDevice.lib")//��·��SDK




//#include "WJ_Avi_DB.h"
// #pragma comment(lib,"WJ_Avi_DB.lib")//���ݿ�
//*******�ڲ��ӿڵ�����������*********//
int ReportVh(VehSet Veh);
int ReportAx(int AxType);
/*********ȫ�ֱ�������***********/

extern VehSet   g_VehSet;   //������Ϣ
extern ModelSet g_ModelSet; //�㷨ģ�Ͳ���
extern LaserSet g_LaserSet; //�������Ӳ���
extern SystemSet g_SysTem;   //ϵͳ���� ���򡢵�˫����
extern VOutSet  g_VOutSet;     //������ֵ����
extern WvFiles  g_WvFiles;     //ģ�Ⲩ��
extern AqcState g_AqcState;    //����״̬
extern QueSet   g_QueSet;        //���й������
extern CamSet   g_CamSet;        //����������
extern SvWave   g_SvWave;        //�洢�������

//����ȫ�ֱ�������
extern int g_VehNum;//���г�������
/************************************************************************/
/*��ʼ��ȫ�ֲ���                                                        */
/************************************************************************/
void ClearGlobal(void);



/************************************************************************/
/* ���ܵ����Ⲩ��                                                       */
/************************************************************************/
void JGRecNetData(char *pDataBuf, int nDataBufSize);



/************************************************************************/
/* ��ȡ�����߳�                                                         */
/************************************************************************/
DWORD WINAPI WaveThreadProc(LPVOID lpParam);

/************************************************************************/
/* ��ʼ�����в���                                                       */
/************************************************************************/
int InitQueProc(void);


/************************************************************************/
/* ��ʼ���                                                             */
/************************************************************************/
int InitCamera(void);

/************************************************************************/
/* ��ʱ������ִ����                                                     */
/************************************************************************/
void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT nIDEvent,DWORD dwTime);


/************************************************************************/
/* ����ͨ��״̬�ص����� 0--������1--�պ���β                            */
/************************************************************************/
int __stdcall FormImage(int FrIdx,int *pX,int *pZ,int MaxH);


/************************************************************************/
/* ����ͨ��״̬�ص����� 0--������1--�պ���β                            */
/************************************************************************/
int __stdcall VehState(int n);



/************************************************************************/
/* ����ͨ����ͼ������						                            */
/************************************************************************/
int __stdcall DrawVehBmp(char *cName);



/************************************************************************/
/* ��ȡImage�洢·��                                                    */
/************************************************************************/
void GetImageRoot(char *pPath);



/************************************************************************/
/* ��ȡWJLog�洢·��                                                    */
/************************************************************************/
void GetWJLogRoot(char *pPath);

/************************************************************************/
/* ���Ŀ¼                                                             */
/************************************************************************/
void CheckRunDirectory(void);


/************************************************************************/
/* ��ʼ�����ݿ�洢                                                     */
/************************************************************************/
int InitDb(void);



void GetWaveRoot(char *pPath);
void GetSyRoot(char *pPath);
void GetMDBRoot(char *pPath);
void GetStatRoot(char *pPath);
void GetCameraRoot(char *pPath);
/************************************************************************/
/* ��ʼ����ͼ���漰����                                                 */
/************************************************************************/
int InitPlotEnVi(void);


void VehQueCheck(void);
bool CleanEndVeh(void);
int platecheck(char* plate0,char* plate1);//����ƥ��ȼ���
bool  CreateHeadVeh(void);
void CameraCheck(char* plate);//�����������
void AxleTransform(VehSet *tempVeh);//����ת������
  int VehClassfier(int VehHeight,int HeadHeight);//����У���ת��
/************************************************************************/
/* ��ʼ�����¼��������߳�                                               */
/************************************************************************/
int InitMutiEvent(void);

/************************************************************************/
/* ���¼��������߳�                                                   */
/************************************************************************/
DWORD WINAPI ThreadMutiEvProc(LPVOID lpParam);


#endif
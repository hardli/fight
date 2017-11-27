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
#include <afx.h>  // 加在这里
#include <windows.h>

#ifdef _DEBUG
#pragma comment(lib, "libcmtd.lib")
#else
#pragma comment(lib, "libcmt.lib")
#endif

#include<winsock2.h>
#include<stdio.h>

using namespace std;
//*******加载各个模块动态库*********//
#include "Avi_Algo.h"
#include "Avi_Plot.h"

//#include "HCNetSDK.h"
//#pragma comment(lib,"HCNetSDK.lib")//海康
//void CALLBACK MSesGCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser);
//BOOL Initcarmer(void);
//BOOL Closecarmer(void);

//#include "dhnetsdk.h"
//#pragma comment(lib,"dhnetsdk.lib")//大华SDK

//#include "HvDeviceBaseType.h"
//#include "HvDeviceCommDef.h"
//#include "HvDeviceNew.h"
//#pragma comment(lib, "HvDevice.lib")//信路威SDK




//#include "WJ_Avi_DB.h"
// #pragma comment(lib,"WJ_Avi_DB.lib")//数据库
//*******内部接口导出函数申明*********//
int ReportVh(VehSet Veh);
int ReportAx(int AxType);
/*********全局变量声明***********/

extern VehSet   g_VehSet;   //车辆信息
extern ModelSet g_ModelSet; //算法模型参数
extern LaserSet g_LaserSet; //激光连接参数
extern SystemSet g_SysTem;   //系统参数 方向、单双激光
extern VOutSet  g_VOutSet;     //出车阈值设置
extern WvFiles  g_WvFiles;     //模拟波形
extern AqcState g_AqcState;    //波形状态
extern QueSet   g_QueSet;        //队列管理相关
extern CamSet   g_CamSet;        //相机管理相关
extern SvWave   g_SvWave;        //存储波形相关

//队列全局变量申明
extern int g_VehNum;//队列车辆总数
/************************************************************************/
/*初始化全局参数                                                        */
/************************************************************************/
void ClearGlobal(void);



/************************************************************************/
/* 接受到激光波形                                                       */
/************************************************************************/
void JGRecNetData(char *pDataBuf, int nDataBufSize);



/************************************************************************/
/* 读取波形线程                                                         */
/************************************************************************/
DWORD WINAPI WaveThreadProc(LPVOID lpParam);

/************************************************************************/
/* 初始化队列操作                                                       */
/************************************************************************/
int InitQueProc(void);


/************************************************************************/
/* 初始相机                                                             */
/************************************************************************/
int InitCamera(void);

/************************************************************************/
/* 定时器功能执行体                                                     */
/************************************************************************/
void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT nIDEvent,DWORD dwTime);


/************************************************************************/
/* 车辆通过状态回调函数 0--进车；1--刚好收尾                            */
/************************************************************************/
int __stdcall FormImage(int FrIdx,int *pX,int *pZ,int MaxH);


/************************************************************************/
/* 车辆通过状态回调函数 0--进车；1--刚好收尾                            */
/************************************************************************/
int __stdcall VehState(int n);



/************************************************************************/
/* 车辆通过绘图并保存						                            */
/************************************************************************/
int __stdcall DrawVehBmp(char *cName);



/************************************************************************/
/* 获取Image存储路径                                                    */
/************************************************************************/
void GetImageRoot(char *pPath);



/************************************************************************/
/* 获取WJLog存储路径                                                    */
/************************************************************************/
void GetWJLogRoot(char *pPath);

/************************************************************************/
/* 检查目录                                                             */
/************************************************************************/
void CheckRunDirectory(void);


/************************************************************************/
/* 初始化数据库存储                                                     */
/************************************************************************/
int InitDb(void);



void GetWaveRoot(char *pPath);
void GetSyRoot(char *pPath);
void GetMDBRoot(char *pPath);
void GetStatRoot(char *pPath);
void GetCameraRoot(char *pPath);
/************************************************************************/
/* 初始化画图缓存及引擎                                                 */
/************************************************************************/
int InitPlotEnVi(void);


void VehQueCheck(void);
bool CleanEndVeh(void);
int platecheck(char* plate0,char* plate1);//车牌匹配度计算
bool  CreateHeadVeh(void);
void CameraCheck(char* plate);//相机重连函数
void AxleTransform(VehSet *tempVeh);//轴型转换函数
  int VehClassfier(int VehHeight,int HeadHeight);//车型校验货转客
/************************************************************************/
/* 初始化多事件及处理线程                                               */
/************************************************************************/
int InitMutiEvent(void);

/************************************************************************/
/* 多事件任务处理线程                                                   */
/************************************************************************/
DWORD WINAPI ThreadMutiEvProc(LPVOID lpParam);


#endif
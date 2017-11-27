#ifndef _AVI_H_
#define _AVI_H_

#include "Avi_Comm.h"


#ifdef AVIMAIN
#define AVIMAIN extern "C"  _declspec(dllimport) 
#else
#define AVIMAIN extern "C"  _declspec(dllexport) 
#endif

//*******����ͷ�ļ�*********//
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



//*******����ӿڵ�����������*********//
AVIMAIN int __stdcall Avi_Test(int VPattern,int VDir,int VehId,int d);
AVIMAIN int __stdcall Avi_Init(void);
AVIMAIN int __stdcall Avi_Close(void);
AVIMAIN int __stdcall Avi_VehCount(void);
AVIMAIN int __stdcall WJ_EndVeh(void);
AVIMAIN bool __stdcall  CleanHeadVeh(void);
AVIMAIN bool __stdcall CleanQueVeh(void);
AVIMAIN void __stdcall Avi_WriteLog(char* msg);
AVIMAIN int __stdcall WJ_EndVeh(void);


/************************************************************************/
/* �����ص�����                                                         */
/************************************************************************/
typedef int (CALLBACK *PFReportVh)(VehSet Veh);




/************************************************************************/
/* ����ص�����                                                         */
/************************************************************************/
typedef int (CALLBACK *PFReportAx)(int AxType);

/************************************************************************/
/* ���س���ע������ص�����                                             */
/************************************************************************/
AVIMAIN int __stdcall RegMainVReport(PFReportVh pfReportVh);

/************************************************************************/
/* ȡɨ��ͼ��ص�����                                                   */
/************************************************************************/
typedef int (CALLBACK *PFPlotIm)(char *pFilePath,char *pFileName);

/************************************************************************/
/* ���س���ȡɨ��ͼ��ص�����                                           */
/************************************************************************/
AVIMAIN int __stdcall RegMainPlotIm(PFPlotIm pfPlotIm);

/************************************************************************/
/* ���س����ȡϵͳ������Ϣ                                             */
/************************************************************************/
AVIMAIN SystemSet __stdcall GetMainSysSet(void);

/************************************************************************/
/* ���س�������ϵͳ����                                                 */
/************************************************************************/
AVIMAIN int __stdcall SetMainSysSet(SystemSet SysSet);


/************************************************************************/
/* ���س����ȡģ�Ͳ���                                                 */
/************************************************************************/
AVIMAIN ModelSet __stdcall GetMainMdlSet(void);

/************************************************************************/
/* ���س�������ģ�Ͳ���                                                 */
/************************************************************************/
AVIMAIN int __stdcall SetMainMdlSet(ModelSet Mdl);


/************************************************************************/
/* ���س������ó�������                                                 */
/************************************************************************/
AVIMAIN int __stdcall SetMainVoutSet(VOutSet VoSet);


/************************************************************************/
/* ���س����ȡ��������                                                 */
/************************************************************************/
AVIMAIN  VOutSet __stdcall GetMainVoutSet(void);

/************************************************************************/
/* ��ȡ���ؼ�����������                                                 */
/************************************************************************/
AVIMAIN LaserSet __stdcall GetMainLaserSet(void);

/************************************************************************/
/* �������ؼ�������                                                     */
/************************************************************************/
AVIMAIN int __stdcall SetMainLaserSet(LaserSet Ls);



/************************************************************************/
/* ��ʼ���㷨����                                                       */
/************************************************************************/
AVIMAIN int __stdcall InitEngine(void);

/************************************************************************/
/* ���¼�������                                                         */
/************************************************************************/
AVIMAIN uint8 __stdcall ReLoadEngine(void);


/************************************************************************/
/* �ر��㷨����                                                         */
/************************************************************************/
AVIMAIN uint8 __stdcall ExitEngine(void);



/**************************************
���쳵��ʶ��̬��ӿں�������20170921
**************************************/
AVIMAIN bool _stdcall Model_InitDevice(HWND hHandle, int nMsg, int nPorType, char* cPortName, long lBaudrate);
AVIMAIN bool _stdcall Model_CloseDevice(void);
AVIMAIN bool _stdcall Model_Getinfo(int *nCarorTruck, int *nModel);
AVIMAIN bool _stdcall Model_GetState(int *nState);
AVIMAIN bool _stdcall Model_Capture(void);

/**************************************
�򼯸��ݳ���ʶ��̬��ӿ�20171012
**************************************/
AVIMAIN int  _stdcall  WjVr_FlushQueue(void);
AVIMAIN bool _stdcall  WjVr_GetCoID(int* CoID, int* idenType, PSTR pchType, PSTR pchVers);
AVIMAIN bool _stdcall  WjVr_ClearOne(void);
AVIMAIN bool _stdcall  WjVr_GetVehicleData(int * AxisGroupNum, int* AxisNum, BYTE* VehicleType, long* AxisWeight, int* AxisType, int* Speed, BYTE* iLeiXing,char* plate);
AVIMAIN int  _stdcall  WjVr_GetState(void);
AVIMAIN bool _stdcall  WjVr_InitDevice(int nPorType, char*cPortName, long lBaudrate);
AVIMAIN int  _stdcall  WjVr_GetVehicleCount(void) ;
AVIMAIN int  _stdcall  WjVr_EndVehicle(void);
AVIMAIN bool _stdcall  WjVr_Close(void);
AVIMAIN bool _stdcall  WjVr_GetPcl(uint16 * PclData, long *Pclsize);

/************************************************************************/
/*����ini�ļ�����                                                       */
/************************************************************************/
AVIMAIN void __stdcall LoadInISet(void);



/************************************************************************/
/*�����㷨����	                                                        */
/************************************************************************/
AVIMAIN int __stdcall  StartAviAlgo(void);


/************************************************************************/
/*�������βɼ�����                                                      */
/************************************************************************/
AVIMAIN int __stdcall StartAqcWave(void);


/************************************************************************/
/*���벨��·��                                                          */
/************************************************************************/
AVIMAIN int __stdcall ImportWvPath(CString Path);


AVIMAIN void __stdcall SaveAllPara(void);
AVIMAIN void __stdcall SaveModelPara(char *pPath);
AVIMAIN void __stdcall SaveLaserPara(char *pPath);
AVIMAIN void __stdcall SaveSysPara(char *pPath);
AVIMAIN void __stdcall SaveVoutPara(char *pPath);
AVIMAIN void __stdcall SaveWvFilePara(char *pPath);
AVIMAIN void __stdcall SaveCamPara(char *pPath);
AVIMAIN void __stdcall SaveQuePara(char *pPath);

AVIMAIN int __stdcall PreSaveWave(void);

#endif
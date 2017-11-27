#ifndef _AVI_H_
#define _AVI_H_

#include "Avi_Comm.h"


#ifdef AVIMAIN
#define AVIMAIN extern "C"  _declspec(dllimport) 
#else
#define AVIMAIN extern "C"  _declspec(dllexport) 
#endif

//*******包含头文件*********//
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



//*******对外接口导出函数申明*********//
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
/* 出车回调函数                                                         */
/************************************************************************/
typedef int (CALLBACK *PFReportVh)(VehSet Veh);




/************************************************************************/
/* 过轴回调函数                                                         */
/************************************************************************/
typedef int (CALLBACK *PFReportAx)(int AxType);

/************************************************************************/
/* 主控程序注册出车回调函数                                             */
/************************************************************************/
AVIMAIN int __stdcall RegMainVReport(PFReportVh pfReportVh);

/************************************************************************/
/* 取扫描图像回调函数                                                   */
/************************************************************************/
typedef int (CALLBACK *PFPlotIm)(char *pFilePath,char *pFileName);

/************************************************************************/
/* 主控程序取扫描图像回调函数                                           */
/************************************************************************/
AVIMAIN int __stdcall RegMainPlotIm(PFPlotIm pfPlotIm);

/************************************************************************/
/* 主控程序获取系统参数信息                                             */
/************************************************************************/
AVIMAIN SystemSet __stdcall GetMainSysSet(void);

/************************************************************************/
/* 主控程序设置系统参数                                                 */
/************************************************************************/
AVIMAIN int __stdcall SetMainSysSet(SystemSet SysSet);


/************************************************************************/
/* 主控程序获取模型参数                                                 */
/************************************************************************/
AVIMAIN ModelSet __stdcall GetMainMdlSet(void);

/************************************************************************/
/* 主控程序设置模型参数                                                 */
/************************************************************************/
AVIMAIN int __stdcall SetMainMdlSet(ModelSet Mdl);


/************************************************************************/
/* 主控程序设置出车参数                                                 */
/************************************************************************/
AVIMAIN int __stdcall SetMainVoutSet(VOutSet VoSet);


/************************************************************************/
/* 主控程序获取出车参数                                                 */
/************************************************************************/
AVIMAIN  VOutSet __stdcall GetMainVoutSet(void);

/************************************************************************/
/* 获取主控激光器参数和                                                 */
/************************************************************************/
AVIMAIN LaserSet __stdcall GetMainLaserSet(void);

/************************************************************************/
/* 设置主控激光器参                                                     */
/************************************************************************/
AVIMAIN int __stdcall SetMainLaserSet(LaserSet Ls);



/************************************************************************/
/* 初始化算法引擎                                                       */
/************************************************************************/
AVIMAIN int __stdcall InitEngine(void);

/************************************************************************/
/* 重新加载引擎                                                         */
/************************************************************************/
AVIMAIN uint8 __stdcall ReLoadEngine(void);


/************************************************************************/
/* 关闭算法引擎                                                         */
/************************************************************************/
AVIMAIN uint8 __stdcall ExitEngine(void);



/**************************************
重庆车型识别动态库接口函数部分20170921
**************************************/
AVIMAIN bool _stdcall Model_InitDevice(HWND hHandle, int nMsg, int nPorType, char* cPortName, long lBaudrate);
AVIMAIN bool _stdcall Model_CloseDevice(void);
AVIMAIN bool _stdcall Model_Getinfo(int *nCarorTruck, int *nModel);
AVIMAIN bool _stdcall Model_GetState(int *nState);
AVIMAIN bool _stdcall Model_Capture(void);

/**************************************
万集福州车型识别动态库接口20171012
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
/*加载ini文件参数                                                       */
/************************************************************************/
AVIMAIN void __stdcall LoadInISet(void);



/************************************************************************/
/*启动算法引擎	                                                        */
/************************************************************************/
AVIMAIN int __stdcall  StartAviAlgo(void);


/************************************************************************/
/*启动波形采集任务                                                      */
/************************************************************************/
AVIMAIN int __stdcall StartAqcWave(void);


/************************************************************************/
/*导入波形路径                                                          */
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
#ifndef _CAMMERA_H_
#define  _CAMMERA_H_
#include  "stdafx.h"


#include "ice_ipcsdk//include/ice_base_type.h"
#include "ice_ipcsdk//include/ice_ipcsdk.h"
#include "ice_ipcsdk//include//ice_vehibrand_interface.h"
#include "ice_ipcsdk//include//ice_com_type.h"
//#include  "ice_ipcsdk//include/ice_vdc_config.h"
#include  "ice_ipcsdk//include//ice_vdc_result.h"
//#include  "ice_ipcsdk//include/ice_vehibrand_interface.h"
//#include   "ice_ipcsdk//include/ice_vehicolor_interface.h"
//#include  "ice_ipcsdk//include/ice_vlpr_result.h"
//#include   "ice_ipcsdk//include/ice_vd_config.h"


extern char  CameraInfo0[20];	//0摄像头识别的车牌信息
extern CString   strIP0;		//0摄像头IP
extern char   CameraInfo1[20];
extern CString   strIP1;
extern int  InitCamera();
extern void  ConnectCamera();
extern void  OnCapture();
extern void  GetCarnumInfo();
extern void  ReadModelParam();
extern void  Closecarmer();

extern void __stdcall SDK_OnPlate1(void *pvParam, const ICE_CHAR *pcIP,
								   const ICE_CHAR *pcNumber, const ICE_CHAR *pcColor, 
								   ICE_CHAR *pcPicData, ICE_U32 u32PicLen, ICE_CHAR *pcCloseUpPicData, ICE_U32 u32CloseUpPicLen, 
								   ICE_S16 s16Speed, ICE_S16 s16VehicleType, ICE_S16 s16Reserved1, ICE_S16 s16Reserved2, 
								   ICE_FLOAT fPlateConfidence, ICE_U32 u32VehicleColor, ICE_U32 u32PlateType, ICE_U32 u32VehicleDir, 
								   ICE_U32 u32AlarmType, ICE_U32 u32SerialNum, ICE_U32 uCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow);


extern void  ICEIPCSDKPlateSdk1(LPCTSTR bstrIP, LPCTSTR bstrNumber, LPCTSTR bstrColor, long nPicData, long nPicLen, long nCloseUpPicData, long nCloseUpPicLen, long nSpeed, long nVehicleType, long nReserved1, long nReserved2,
								float fPlateConfidence, long nVehicleColor, long nPlateType, long nVehicleDir, long nAlarmType, long u32SerialNum, long nCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow, int index);


extern void __stdcall SDK_OnPlate0(void *pvParam, const ICE_CHAR *pcIP,
								   const ICE_CHAR *pcNumber, const ICE_CHAR *pcColor,
								   ICE_CHAR *pcPicData, ICE_U32 u32PicLen, ICE_CHAR *pcCloseUpPicData, ICE_U32 u32CloseUpPicLen,
								   ICE_S16 s16Speed, ICE_S16 s16VehicleType, ICE_S16 s16Reserved1, ICE_S16 s16Reserved2,
								   ICE_FLOAT fPlateConfidence, ICE_U32 u32VehicleColor, ICE_U32 u32PlateType, ICE_U32 u32VehicleDir,
								   ICE_U32 u32AlarmType, ICE_U32 u32SerialNum, ICE_U32 uCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow);


extern void  ICEIPCSDKPlateSdk0(LPCTSTR bstrIP, LPCTSTR bstrNumber, LPCTSTR bstrColor, long nPicData, long nPicLen, long nCloseUpPicData, long nCloseUpPicLen, long nSpeed, long nVehicleType, long nReserved1, long nReserved2,
								float fPlateConfidence, long nVehicleColor, long nPlateType, long nVehicleDir, long nAlarmType, long u32SerialNum, long nCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow, int index);






#endif
#ifndef _ICE_IPCSDK__H_
#define _ICE_IPCSDK__H_

#include "ice_com_type.h"
#include "ice_base_type.h"
#include "ice_vd_config.h"
/*#include "ice_clbr_config.h"
#include "ice_dtca_config.h"
#include "ice_tcnt_interface.h"
#include "ice_hcnt_config.h"
#include "ice_ipg_config.h"*/
#include "ice_vdc_config.h"
#include "ice_vlpr_result.h"

#ifdef ICE_IPCSDK_EXPORTS
#define ICE_IPCSDK_API __declspec(dllexport)
#else
#define ICE_IPCSDK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define PAYLOAD_TYPE_JPEG  26	//抓拍图片
#define PAYLOAD_TYPE_H264  96	//h264码流
#define PAYLOAD_TYPE_DTCA  100	//周界结果
#define PAYLOAD_TYPE_VLPR  101	//车牌结果
#define PAYLOAD_TYPE_TCNT  102	//计数结果
#define PAYLOAD_TYPE_HCNT  103	//计数结果
#define PAYLOAD_TYPE_IPG   104	//车位结果
#define PAYLOAD_TYPE_VDM   105	//移动卡口

typedef enum
{
	ICE_DEVICE_OFFLINE,		//离线
	ICE_DEVICE_ONLINE,		//在线
	ICE_DEVICE_IO_CHANGED			//IO状态改变
} ICE_DEVICE_EVENT_TYPE;
/*
网络流回调接口，当网络上有数据时，sdk会回调该接口

pvParam为用户自定义参数，用来区分不同的sdk使用者，类似于线程入口函数的参数
u8PayloadType为payload类型，取值为上面所定义的宏
u32Timestamp为时间戳
pvData为收到的数据地址
s32Len为收到的数据长度

当pvData为NULL时，表示特殊事件，事件类型用s32Len来区分
s32Len为0表示与相机断开连接
s32Len为1表示与相机重新连接
*/
typedef void (__stdcall *ICE_IPCSDK_OnStream)(void *pvParam, 
	ICE_U8 u8PayloadType, ICE_U32 u32Timestamp, 
	void *pvData, ICE_S32 s32Len);

/*
图像帧回调接口，当解码出一帧图像时，sdk会回调该接口

pvParam为用户自定义参数，用来区分不同的sdk使用者，类似于线程入口函数的参数
u32Timestamp为时间戳
pu8Data为图像数据地址数组，分别存放y，u和v
s32Linesize为每扫描行数据长度数组，分别存放y，u和v的每扫描行长度
s32Width为图像宽度
s32Height为图像高度
*/
typedef void (__stdcall *ICE_IPCSDK_OnFrame)(
	void *pvParam, ICE_U32 u32Timestamp, 
	ICE_U8 *pu8Data[], ICE_S32 s32Linesize[], 
	ICE_S32 s32Width, ICE_S32 s32Height, 
	ICE_U8 *rawdata, ICE_S32 rawlen);

/*#define REQ_TYPE_H264 0x01	//请求h264码流
#define REQ_TYPE_JPEG 0x02	//请求抓拍图像
#define REQ_TYPE_ALGO 0x04	//请求算法数据*/

ICE_IPCSDK_API void ICE_IPCSDK_Init();
ICE_IPCSDK_API void ICE_IPCSDK_Fini();

/*
连接ipc相机接口

pcIP为相机ip地址
u8ReqType为请求数据类型，可以为三种请求类型的任意组合
pfOnStream为网络流回调地址，可以为NULL
pvStreamParam为网络流回调参数，能区分开不同的使用者即可
pfOnFrame为图像帧回调地址，可以为NULL，只有当u8ReqType包含了REQ_TYPE_H264时才有意义
pvFrameParam为图像帧回调参数，能区分开不同的使用者即可

返回值为sdk句柄，失败时返回NULL
*/
ICE_IPCSDK_API ICE_HANDLE ICE_IPCSDK_Open(const ICE_CHAR *pcIP, ICE_U8 u8OverTCP, 
	ICE_U16 u16RTSPPort, ICE_U16 u16ICEPort, ICE_U16 u16OnvifPort, ICE_U8 u8MainStream, 
	ICE_IPCSDK_OnStream pfOnStream, void *pvStreamParam, 
	ICE_IPCSDK_OnFrame pfOnFrame, void *pvFrameParam);
ICE_IPCSDK_API ICE_HANDLE ICE_IPCSDK_Open_Passwd(const ICE_CHAR *pcIP, const ICE_CHAR *pcPasswd, 
	ICE_U8 u8OverTCP, ICE_U16 u16RTSPPort, ICE_U16 u16ICEPort, 
	ICE_U16 u16OnvifPort, ICE_U8 u8MainStream, 
	ICE_IPCSDK_OnStream pfOnStream, void *pvStreamParam, 
	ICE_IPCSDK_OnFrame pfOnFrame, void *pvFrameParam);

typedef void (__stdcall *ICE_IPCSDK_OnPlate)(void *pvParam, const ICE_CHAR *pcIP, 
	const ICE_CHAR *pcNumber, const ICE_CHAR *pcColor, 
	ICE_CHAR *pcPicData, ICE_U32 u32PicLen, ICE_CHAR *pcCloseUpPicData, ICE_U32 u32CloseUpPicLen, 
	ICE_S16 s16Speed, ICE_S16 s16VehicleType, ICE_S16 s16Reserved1, ICE_S16 s16Reserved2, 
	ICE_FLOAT fPlateConfidence, ICE_U32 u32VehicleColor, ICE_U32 u32PlateType, ICE_U32 u32VehicleDir, 
	ICE_U32 u32AlarmType, ICE_U32 u32SerialNum, ICE_U32 uCapTime, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow);

typedef void (*ICE_IPCSDK_OnPlate_Cdecl)(void *pvParam, const ICE_CHAR *pcIP, 
	const ICE_CHAR *pcNumber, const ICE_CHAR *pcColor, 
	ICE_CHAR *pcPicData, ICE_U32 u32PicLen, ICE_CHAR *pcCloseUpPicData, ICE_U32 u32CloseUpPicLen, 
	ICE_S16 s16PlatePosLeft, ICE_S16 s16PlatePosTop, ICE_S16 s16PlatePosRight, ICE_S16 s16PlatePosBottom, 
	ICE_FLOAT fPlateConfidence, ICE_U32 u32VehicleColor, ICE_U32 u32PlateType, ICE_U32 u32VehicleDir, 
	ICE_U32 u32AlarmType, ICE_U32 u32SerialNum, ICE_U32 u32Reserved2, ICE_U32 u32ResultHigh, ICE_U32 u32ResultLow);

ICE_IPCSDK_API ICE_HANDLE ICE_IPCSDK_OpenPreview(const ICE_CHAR *pcIP, 
	ICE_U8 u8OverTCP, ICE_U8 u8MainStream, ICE_U32 hWnd, 
	ICE_IPCSDK_OnPlate pfOnPlate, void *pvPlateParam);
ICE_IPCSDK_API ICE_HANDLE ICE_IPCSDK_OpenPreview_Passwd(const ICE_CHAR *pcIP, const ICE_CHAR *pcPasswd, 
	ICE_U8 u8OverTCP, ICE_U8 u8MainStream, ICE_U32 hWnd, 
	ICE_IPCSDK_OnPlate pfOnPlate, void *pvPlateParam);

ICE_IPCSDK_API ICE_HANDLE ICE_IPCSDK_OpenPreviewExt(const ICE_CHAR *pcIP, 
	ICE_U8 u8OverTCP, ICE_U8 u8MainStream, ICE_U32 hWnd, 
	ICE_IPCSDK_OnPlate pfOnPlate, void *pvPlateParam, 
	ICE_U16 u16RTSPPort, ICE_U16 u16ICEPort, 
	ICE_U16 u16OnvifPort, ICE_U16 u16EvtpPort);
ICE_IPCSDK_API ICE_HANDLE ICE_IPCSDK_OpenPreviewExt_Passwd(const ICE_CHAR *pcIP, const ICE_CHAR *pcPasswd, 
	ICE_U8 u8OverTCP, ICE_U8 u8MainStream, ICE_U32 hWnd, 
	ICE_IPCSDK_OnPlate pfOnPlate, void *pvPlateParam, 
	ICE_U16 u16RTSPPort, ICE_U16 u16ICEPort, 
	ICE_U16 u16OnvifPort, ICE_U16 u16EvtpPort);

ICE_IPCSDK_API ICE_HANDLE ICE_IPCSDK_OpenPreview_Cdecl(const ICE_CHAR *pcIP, 
	ICE_U8 u8OverTCP, ICE_U8 u8MainStream, ICE_U32 hWnd, 
	ICE_IPCSDK_OnPlate_Cdecl pfOnPlate, void *pvPlateParam);
ICE_IPCSDK_API ICE_HANDLE ICE_IPCSDK_OpenPreview_Cdecl_Passwd(const ICE_CHAR *pcIP, const ICE_CHAR *pcPasswd, 
	ICE_U8 u8OverTCP, ICE_U8 u8MainStream, ICE_U32 hWnd, 
	ICE_IPCSDK_OnPlate_Cdecl pfOnPlate, void *pvPlateParam);

ICE_IPCSDK_API ICE_HANDLE ICE_IPCSDK_OpenDevice(const ICE_CHAR *pcIP);
ICE_IPCSDK_API ICE_HANDLE ICE_IPCSDK_OpenDevice_Passwd(const ICE_CHAR *pcIP, const ICE_CHAR *pcPasswd);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_StartStream(ICE_HANDLE hSDK, 
	ICE_U8 u8MainStream, ICE_U32 hWnd);

ICE_IPCSDK_API void ICE_IPCSDK_StopStream(ICE_HANDLE hSDK);

ICE_IPCSDK_API void ICE_IPCSDK_SetDataCallback(ICE_HANDLE hSDK, 
	ICE_IPCSDK_OnStream pfOnStream, void *pvParam);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_TriggerExt(ICE_HANDLE hSDK);

typedef void (__stdcall *ICE_IPCSDK_OnFrame_Planar)(
	void *pvParam, ICE_U32 u32Timestamp, 
	ICE_U8 *pu8DataY, ICE_U8 *pu8DataU, ICE_U8 *pu8DataV, 
	ICE_S32 s32LinesizeY, ICE_S32 s32LinesizeU, ICE_S32 s32LinesizeV, 
	ICE_S32 s32Width, ICE_S32 s32Height);

ICE_IPCSDK_API void ICE_IPCSDK_SetFrameCallback(ICE_HANDLE hSDK, 
	ICE_IPCSDK_OnFrame_Planar pfOnFrame, void *pvParam);

ICE_IPCSDK_API void ICE_IPCSDK_SetPlateCallback(ICE_HANDLE hSDK, 
	ICE_IPCSDK_OnPlate pfOnPlate, void *pvParam);
ICE_IPCSDK_API void ICE_IPCSDK_SetPlateCallback_Cdecl(ICE_HANDLE hSDK, 
	ICE_IPCSDK_OnPlate_Cdecl pfOnPlate, void *pvParam);

/*
断开ipc相机接口
hSDK为ICE_IPCSDK_Open所返回的sdk句柄
*/
ICE_IPCSDK_API void ICE_IPCSDK_Close(ICE_HANDLE hSDK);

/*
设置算法规则接口

hSDK为ICE_IPCSDK_Open所返回的sdk句柄
pstParam为算法规则结构体指针

返回值为ICE_COM_STATUS_E枚举值，参见ice_com_type.h
*/
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetVDParam(ICE_HANDLE hSDK, 
	const ICE_VD_PARAM_S *pstParam);

/*
获取当前算法规则接口

hSDK为ICE_IPCSDK_Open所返回的sdk句柄
pstParam为算法规则结构体指针

返回值为ICE_COM_STATUS_E枚举值，参见ice_com_type.h
*/
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetVDParam(ICE_HANDLE hSDK, 
	ICE_VD_PARAM_S *pstParam);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetVDCParam(ICE_HANDLE hSDK, 
	const ICE_VDC_PARAM_S *pstParam);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetVDCParam(ICE_HANDLE hSDK, 
	ICE_VDC_PARAM_S *pstParam);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetArm(ICE_HANDLE hSDK, ICE_U8 u8Arm);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetArm(ICE_HANDLE hSDK, ICE_U8 *pu8Arm);

/*
设置算法规则接口

hSDK为ICE_IPCSDK_Open所返回的sdk句柄
pstParam为算法规则结构体指针

返回值为ICE_COM_STATUS_E枚举值，参见ice_com_type.h
*/
/*ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetDTCAParam(ICE_HANDLE hSDK, 
	const ICE_CLBR_PARAM *pstCLBRParam, const ICE_DTCA_PARAM_ALGO *pstDTCAParam);*/

/*
获取当前算法规则接口

hSDK为ICE_IPCSDK_Open所返回的sdk句柄
pstParam为算法规则结构体指针

返回值为ICE_COM_STATUS_E枚举值，参见ice_com_type.h
*/
/*ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetDTCAParam(ICE_HANDLE hSDK, 
	ICE_CLBR_PARAM *pstCLBRParam, ICE_DTCA_PARAM_ALGO *pstDTCAParam);*/

/*
设置算法规则接口

hSDK为ICE_IPCSDK_Open所返回的sdk句柄
pstParam为算法规则结构体指针

返回值为ICE_COM_STATUS_E枚举值，参见ice_com_type.h
*/
/*ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetTCNTParam(ICE_HANDLE hSDK, 
	const ICE_TCNT_PARAM_ALGO *pstTCNTParam);*/

/*
获取当前算法规则接口

hSDK为ICE_IPCSDK_Open所返回的sdk句柄
pstParam为算法规则结构体指针

返回值为ICE_COM_STATUS_E枚举值，参见ice_com_type.h
*/
/*ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetTCNTParam(ICE_HANDLE hSDK, 
	ICE_TCNT_PARAM_ALGO *pstTCNTParam);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetHCNTParam(ICE_HANDLE hSDK, 
	const ICE_CLBR_PARAM *pstCLBRParam, const ICE_HCNT_PARAM_ALGO *pstHCNTParam);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetHCNTParam(ICE_HANDLE hSDK, 
	ICE_CLBR_PARAM *pstCLBRParam, ICE_HCNT_PARAM_ALGO *pstHCNTParam);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetIPGParam(ICE_HANDLE hSDK, const ICE_IPG_PARAM_S *pstParam);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetIPGParam(ICE_HANDLE hSDK, ICE_IPG_PARAM_S *pstParam);*/

/*
设置曝光接口

hSDK为ICE_IPCSDK_Open所返回的sdk句柄
u32ExpLine为曝光时间
s32AGain为模拟增益
s32DGain为数字增益

返回值为ICE_COM_STATUS_E枚举值，参见ice_com_type.h
*/
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetExpAttr(ICE_HANDLE hSDK, const ICE_ExpAttr_S *pstExpAttr);

/*
获取当前算法规则接口

hSDK为ICE_IPCSDK_Open所返回的sdk句柄
pu32ExpLine为曝光时间
ps32AGain为模拟增益
ps32DGain为数字增益

返回值为ICE_COM_STATUS_E枚举值，参见ice_com_type.h
*/
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetExpAttr(ICE_HANDLE hSDK, ICE_ExpAttr_S *pstExpAttr);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SyncTime(ICE_HANDLE hSDK, 
	ICE_U16 u16Year, ICE_U8 u8Month, ICE_U8 u8Day, 
	ICE_U8 u8Hour, ICE_U8 u8Min, ICE_U8 u8Sec = 0);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetSnapCamera(ICE_HANDLE hSDK, 
	ICE_SNAP_CAMERA_S *pstSnapCamrea);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetSnapCamera(ICE_HANDLE hSDK, 
	const ICE_SNAP_CAMERA_S *pstSnapCamrea);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetFTP(ICE_HANDLE hSDK, ICE_U32 *pu32Enable, 
	ICE_U32 *pu32IP, ICE_S8 *ps8Username, ICE_S8 *ps8Password, ICE_U32 *pu32Interval, 
	ICE_S8 *ps8Path, ICE_U32 *pu32Plate, ICE_U32 *pu32XML);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetFTP(ICE_HANDLE hSDK, ICE_U32 u32Enable, 
	ICE_U32 u32IP, const ICE_S8 *ps8Username, const ICE_S8 *ps8Password, ICE_U32 u32Interval, 
	const ICE_S8 *ps8Path, ICE_U32 u32Plate, ICE_U32 u32XML);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetIPAddr(ICE_HANDLE hSDK, 
	ICE_U32 *pu32IP, ICE_U32 *pu32Mask, ICE_U32 *pu32Gateway);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetIPAddr(ICE_HANDLE hSDK, 
	ICE_U32 u32IP, ICE_U32 u32Mask, ICE_U32 u32Gateway);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetRelayOutput(
	ICE_HANDLE hSDK, ICE_U8 *pu8IdleState, ICE_U32 *pu32DelayTime);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetRelayOutputSettings(
	ICE_HANDLE hSDK, ICE_U8 u8IdleState, ICE_U32 u32DelayTime);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetRelayOutput2(
	ICE_HANDLE hSDK, ICE_U8 *pu8IdleState, ICE_U32 *pu32DelayTime);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetRelayOutput2Settings(
	ICE_HANDLE hSDK, ICE_U8 u8IdleState, ICE_U32 u32DelayTime);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetAlarmOutConfig(
	ICE_HANDLE hSDK, ICE_U32 u32Index, ICE_U32 *pu32IdleState, 
	ICE_U32 *pu32DelayTime, ICE_U32 *pu32Reserve);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetAlarmOutConfig(
	ICE_HANDLE hSDK, ICE_U32 u32Index, ICE_U32 u32IdleState, 
	ICE_U32 u32DelayTime, ICE_U32 u32Reserve);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetEncoderSettings(
	ICE_HANDLE hSDK, ICE_U8 u8MainStream, 
	ICE_U32 *pu32Width, ICE_U32 *pu32Height, 
	ICE_U32 *pu32FrameRate, ICE_U32 *pu32BitRate);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetEncoderSettings(
	ICE_HANDLE hSDK, ICE_U8 u8MainStream, 
	ICE_U32 u32Width, ICE_U32 u32Height, 
	ICE_U32 u32FrameRate, ICE_U32 u32BitRate);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_Reboot(ICE_HANDLE hSDK);

/*ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetHCNTCfg(ICE_HANDLE hSDK, 
	ICE_S32 *ps32RS485Type, ICE_U32 *pu32DoorType, ICE_U32 *pu32Railway);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetHCNTCfg(ICE_HANDLE hSDK, 
	ICE_S32 s32RS485Type, ICE_U32 u32DoorType, ICE_U32 u32Railway);*/

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetAlarmInCfg(
	ICE_HANDLE hSDK, ICE_U32 *pu32IdleState);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetAlarmInCfg(
	ICE_HANDLE hSDK, ICE_U32 u32IdleState);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetNTPCfg(
	ICE_HANDLE hSDK, ICE_U32 *pu32Mode, char *pcNTPServer, ICE_U32 *pu32TimeZone);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetNTPCfg(
	ICE_HANDLE hSDK, ICE_U32 u32Mode, const char *pcNTPServer, ICE_U32 u32TimeZone);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetOSDCfg(ICE_HANDLE hSDK, ICE_OSDAttr_S *pstOSDAttr);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetOSDCfg(ICE_HANDLE hSDK, const ICE_OSDAttr_S *pstOSDAttr);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetSchedCfg(ICE_HANDLE hSDK, 
	ICE_SCHED_TASK_S *pstTasks, ICE_U32 *pu32Count);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetSchedCfg(ICE_HANDLE hSDK, 
	const ICE_SCHED_TASK_S *pstTasks, const ICE_U32 u32Count);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetImagingSettings(
	ICE_HANDLE hSDK, ICE_U32 *pu32Luma, ICE_U32 *pu32Contr, 
	ICE_U32 *pu32Hue, ICE_U32 *pu32Satu);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetImagingSettings(
	ICE_HANDLE hSDK, ICE_U32 u32Luma, ICE_U32 u32Contr, 
	ICE_U32 u32Hue, ICE_U32 u32Satu);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_OpenGate(ICE_HANDLE hSDK);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_OpenGate2(ICE_HANDLE hSDK);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_ControlAlarmOut(ICE_HANDLE hSDK, ICE_U32 u32Index);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_BeginTalk(ICE_HANDLE hSDK);
ICE_IPCSDK_API void ICE_IPCSDK_EndTalk(ICE_HANDLE hSDK);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_IsTalking(ICE_HANDLE hSDK);

ICE_IPCSDK_API void ICE_IPCSDK_Preview(ICE_HANDLE hSDK, ICE_U32 u32Preview);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetPhyChnCfg(ICE_HANDLE hSDK, ICE_PHYCHN_CFG_S *pstPhyChnCfg);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetPhyChnCfg(ICE_HANDLE hSDK, const ICE_PHYCHN_CFG_S *pstPhyChnCfg);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetARegCfg(ICE_HANDLE hSDK, ICE_EXP_AREGULATE_PARAM *pstARegCfg);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetARegCfg(ICE_HANDLE hSDK, const ICE_EXP_AREGULATE_PARAM *pstARegCfg);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetUARTCfg(ICE_HANDLE hSDK, ICE_UART_PARAM *pstUARTCfg);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetUARTCfg(ICE_HANDLE hSDK, const ICE_UART_PARAM *pstUARTCfg);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetSDCfg(ICE_HANDLE hSDK, SD_card_param *pstSDCfg);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetSDCfg(ICE_HANDLE hSDK, const SD_card_param *pstSDCfg);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SDFormat(ICE_HANDLE hSDK, int timeout);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SDUnload(ICE_HANDLE hSDK);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetHttpPostCfg(ICE_HANDLE hSDK, ICE_HTTP_POST_PARAM *pstHttpPostCfg);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetHttpPostCfg(ICE_HANDLE hSDK, const ICE_HTTP_POST_PARAM *pstHttpPostCfg);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetWhiteListParam(ICE_HANDLE hSDK, 
	int mode, int match, int bmode, int bmatch, int tmode);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetWhiteListParam(ICE_HANDLE hSDK, 
	int* mode, int* match, int *bmode, int *bmatch, int *tmode);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_Trigger(ICE_HANDLE hSDK, ICE_CHAR *pcNumber, ICE_CHAR *pcColor, 
	ICE_CHAR *pcPicData, ICE_U32 u32PicSize, ICE_U32 *pu32PicLen);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetStatus(ICE_HANDLE hSDK);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_Capture(ICE_HANDLE hSDK, 
	ICE_CHAR *pcPicData, ICE_U32 u32PicSize, ICE_U32 *pu32PicLen);

typedef void (__stdcall *ICE_IPCSDK_OnSerialPort)(void *pvParam, 
	const ICE_CHAR *pcIP, ICE_CHAR *pcData, ICE_U32 u32Len);

ICE_IPCSDK_API void ICE_IPCSDK_SetSerialPortCallBack(ICE_HANDLE hSDK, 
	ICE_IPCSDK_OnSerialPort pfOnSerialPort, void *pvSerialPortParam);

typedef void (__stdcall *ICE_IPCSDK_OnSerialPort_RS232)(void *pvParam, 
	const ICE_CHAR *pcIP, ICE_CHAR *pcData, ICE_U32 u32Len);

ICE_IPCSDK_API void ICE_IPCSDK_SetSerialPortCallBack_RS232(ICE_HANDLE hSDK, 
	ICE_IPCSDK_OnSerialPort_RS232 pfOnSerialPort, void *pvSerialPortParam);

typedef void (*ICE_IPCSDK_OnSerialPort_Cdecl)(void *pvParam, 
	const ICE_CHAR *pcIP, ICE_CHAR *pcData, ICE_U32 u32Len);

ICE_IPCSDK_API void ICE_IPCSDK_SetSerialPortCallBack_Cdecl(ICE_HANDLE hSDK, 
	ICE_IPCSDK_OnSerialPort_Cdecl pfOnSerialPort, void *pvSerialPortParam);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_TransSerialPort(ICE_HANDLE hSDK, ICE_CHAR *pcData, ICE_U32 u32Len);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_TransSerialPort_RS232(ICE_HANDLE hSDK, 
	ICE_CHAR *pcData, ICE_U32 u32Len);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WhiteListGetCount(ICE_HANDLE hSDK, ICE_U32 *pu32Count);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WhiteListGetItem(ICE_HANDLE hSDK, ICE_S32 s32Index, ICE_CHAR* pcNumber, 
	ICE_CHAR* pcDateBegin, ICE_CHAR* pcDateEnd, ICE_CHAR* pcTimeBegin, ICE_CHAR* pcTimeEnd, 
	ICE_CHAR* pcRsrv1, ICE_CHAR* pcRsrv2, ICE_CHAR* pcRsrv3, ICE_CHAR* pcRsrv4);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WhiteListEditItem(ICE_HANDLE hSDK, ICE_S32 s32Index, const ICE_CHAR* pcNumber, 
	const ICE_CHAR* pcDateBegin, const ICE_CHAR* pcDateEnd, const ICE_CHAR* pcTimeBegin, const ICE_CHAR* pcTimeEnd, 
	const ICE_CHAR* pcRsrv1, const ICE_CHAR* pcRsrv2, const ICE_CHAR* pcRsrv3, const ICE_CHAR* pcRsrv4);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WhiteListInsertItem(ICE_HANDLE hSDK, ICE_S32 s32Index, const ICE_CHAR* pcNumber, 
	const ICE_CHAR* pcDateBegin, const ICE_CHAR* pcDateEnd, const ICE_CHAR* pcTimeBegin, const ICE_CHAR* pcTimeEnd, 
	const ICE_CHAR* pcRsrv1, const ICE_CHAR* pcRsrv2, const ICE_CHAR* pcRsrv3, const ICE_CHAR* pcRsrv4);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WhiteListDeleteItem(ICE_HANDLE hSDK, ICE_S32 s32Index);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WhiteListDelAllItems(ICE_HANDLE hSDK);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WhiteListFindItem(ICE_HANDLE hSDK, 
	const ICE_CHAR* pcNumber, ICE_S32 s32Start, ICE_S32 *s32Index);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WhiteListEditItem_ByNumber(ICE_HANDLE hSDK, const ICE_CHAR* pcNumber, 
	const ICE_CHAR* pcDateBegin, const ICE_CHAR* pcDateEnd, const ICE_CHAR* pcTimeBegin, const ICE_CHAR* pcTimeEnd, const ICE_CHAR* pcType, 
	const ICE_CHAR* pcRsrv1, const ICE_CHAR* pcRsrv2, const ICE_CHAR* pcRsrv3, const ICE_CHAR* pcRsrv4);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WhiteListInsertItem_ByNumber(ICE_HANDLE hSDK, const ICE_CHAR* pcNumber, 
	const ICE_CHAR* pcDateBegin, const ICE_CHAR* pcDateEnd, const ICE_CHAR* pcTimeBegin, const ICE_CHAR* pcTimeEnd, const ICE_CHAR* pcType, 
	const ICE_CHAR* pcRsrv1, const ICE_CHAR* pcRsrv2, const ICE_CHAR* pcRsrv3, const ICE_CHAR* pcRsrv4);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WhiteListDeleteItem_ByNumber(ICE_HANDLE hSDK, const ICE_CHAR* pcNumber);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WhiteListDelAllItems_ByNumber(ICE_HANDLE hSDK);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WhiteListFindItem_ByNumber(ICE_HANDLE hSDK, const ICE_CHAR* pcNumber, 
	ICE_CHAR* pcDateBegin, ICE_CHAR* pcDateEnd, ICE_CHAR* pcTimeBegin, ICE_CHAR* pcTimeEnd, ICE_CHAR* pcType, 
	ICE_CHAR* pcRsrv1, ICE_CHAR* pcRsrv2, ICE_CHAR* pcRsrv3, ICE_CHAR* pcRsrv4);

typedef void (__stdcall *ICE_IPCSDK_OnPastPlate)(void *pvParam, const ICE_CHAR *pcIP, 
	ICE_U32 u32CapTime, const ICE_CHAR *pcNumber, const ICE_CHAR *pcColor, 
	ICE_CHAR *pcPicData, ICE_U32 u32PicLen, ICE_CHAR *pcCloseUpPicData, ICE_U32 u32CloseUpPicLen, 
	ICE_S16 s16PlatePosLeft, ICE_S16 s16PlatePosTop, ICE_S16 s16PlatePosRight, ICE_S16 s16PlatePosBottom, 
	ICE_FLOAT fPlateConfidence, ICE_U32 u32VehicleColor, ICE_U32 u32PlateType, ICE_U32 u32VehicleDir, 
	ICE_U32 u32AlarmType, ICE_U32 u32Reserved1, ICE_U32 u32Reserved2, ICE_U32 u32Reserved3, ICE_U32 u32Reserved4);

ICE_IPCSDK_API void ICE_IPCSDK_SetPastPlateCallBack(ICE_HANDLE hSDK, 
	ICE_IPCSDK_OnPastPlate pfOnPastPlate, void *pvPastPlateParam);

typedef void (*ICE_IPCSDK_OnPastPlate_Cdecl)(void *pvParam, const ICE_CHAR *pcIP, 
	ICE_U32 u32CapTime, const ICE_CHAR *pcNumber, const ICE_CHAR *pcColor, 
	ICE_CHAR *pcPicData, ICE_U32 u32PicLen, ICE_CHAR *pcCloseUpPicData, ICE_U32 u32CloseUpPicLen, 
	ICE_S16 s16PlatePosLeft, ICE_S16 s16PlatePosTop, ICE_S16 s16PlatePosRight, ICE_S16 s16PlatePosBottom, 
	ICE_FLOAT fPlateConfidence, ICE_U32 u32VehicleColor, ICE_U32 u32PlateType, ICE_U32 u32VehicleDir, 
	ICE_U32 u32AlarmType, ICE_U32 u32Reserved1, ICE_U32 u32Reserved2, ICE_U32 u32Reserved3, ICE_U32 u32Reserved4);

ICE_IPCSDK_API void ICE_IPCSDK_SetPastPlateCallBack_Cdecl(ICE_HANDLE hSDK, 
	ICE_IPCSDK_OnPastPlate_Cdecl pfOnPastPlate, void *pvPastPlateParam);

typedef void (__stdcall *ICE_IPCSDK_OnDeviceEvent)(void *pvParam, const ICE_CHAR *pcIP, ICE_U32 u32EventType, 
	ICE_U32 u32EventData1, ICE_U32 u32EventData2, ICE_U32 u32EventData3, ICE_U32 u32EventData4);

ICE_IPCSDK_API void ICE_IPCSDK_SetDeviceEventCallBack(ICE_HANDLE hSDK, 
	ICE_IPCSDK_OnDeviceEvent pfOnDeviceEvent, void *pvDeviceEventParam);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetDevID(ICE_HANDLE hSDK, ICE_CHAR* szDevID);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_StartRecord(ICE_HANDLE hSDK, const ICE_CHAR *pcFileName);
ICE_IPCSDK_API void ICE_IPCSDK_StopRecord(ICE_HANDLE hSDK);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetHDTriggerParam(ICE_HANDLE hSDK, HDTrigger_Param *pstParam);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetHDTriggerParam(ICE_HANDLE hSDK, const HDTrigger_Param *pstParam);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WriteUserData(ICE_HANDLE hSDK, const char* pcData);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_ReadUserData(ICE_HANDLE hSDK, char* pcData, int nSize);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_WriteUserData_Binary(
	ICE_HANDLE hSDK, const char* pcData, ICE_U32 nOffset, ICE_U32 nLen);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_ReadUserData_Binary(
	ICE_HANDLE hSDK, char* pcData, ICE_U32 nSize, ICE_U32 nOffset, ICE_U32 nLen);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetWhiteCount(ICE_HANDLE hSDK, ICE_U32 *pu32Count);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetWhiteItem(ICE_HANDLE hSDK, ICE_S32 s32Index, ICE_CHAR* pcNumber, 
	ICE_CHAR* pcDateBegin, ICE_CHAR* pcDateEnd, ICE_CHAR* pcTimeBegin, ICE_CHAR* pcTimeEnd, 
	ICE_CHAR* pcRsrv1, ICE_CHAR* pcRsrv2, ICE_CHAR* pcRsrv3, ICE_CHAR* pcRsrv4);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_EditWhiteItem(ICE_HANDLE hSDK, const ICE_CHAR* pcNumber, 
	const ICE_CHAR* pcDateBegin, const ICE_CHAR* pcDateEnd, const ICE_CHAR* pcTimeBegin, const ICE_CHAR* pcTimeEnd, 
	const ICE_CHAR* pcRsrv1, const ICE_CHAR* pcRsrv2, const ICE_CHAR* pcRsrv3, const ICE_CHAR* pcRsrv4);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_InsertWhiteItem(ICE_HANDLE hSDK, const ICE_CHAR* pcNumber, 
	const ICE_CHAR* pcDateBegin, const ICE_CHAR* pcDateEnd, const ICE_CHAR* pcTimeBegin, const ICE_CHAR* pcTimeEnd, 
	const ICE_CHAR* pcRsrv1, const ICE_CHAR* pcRsrv2, const ICE_CHAR* pcRsrv3, const ICE_CHAR* pcRsrv4);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetBlackCount(ICE_HANDLE hSDK, ICE_U32 *pu32Count);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetBlackItem(ICE_HANDLE hSDK, ICE_S32 s32Index, ICE_CHAR* pcNumber, 
	ICE_CHAR* pcDateBegin, ICE_CHAR* pcDateEnd, ICE_CHAR* pcTimeBegin, ICE_CHAR* pcTimeEnd, 
	ICE_CHAR* pcRsrv1, ICE_CHAR* pcRsrv2, ICE_CHAR* pcRsrv3, ICE_CHAR* pcRsrv4);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_EditBlackItem(ICE_HANDLE hSDK, const ICE_CHAR* pcNumber, 
	const ICE_CHAR* pcDateBegin, const ICE_CHAR* pcDateEnd, const ICE_CHAR* pcTimeBegin, const ICE_CHAR* pcTimeEnd, 
	const ICE_CHAR* pcRsrv1, const ICE_CHAR* pcRsrv2, const ICE_CHAR* pcRsrv3, const ICE_CHAR* pcRsrv4);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_InsertBlackItem(ICE_HANDLE hSDK, const ICE_CHAR* pcNumber, 
	const ICE_CHAR* pcDateBegin, const ICE_CHAR* pcDateEnd, const ICE_CHAR* pcTimeBegin, const ICE_CHAR* pcTimeEnd, 
	const ICE_CHAR* pcRsrv1, const ICE_CHAR* pcRsrv2, const ICE_CHAR* pcRsrv3, const ICE_CHAR* pcRsrv4);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_DelAllWhiteItems(ICE_HANDLE hSDK);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_DelAllBlackItems(ICE_HANDLE hSDK);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_DeleteWhiteItem(ICE_HANDLE hSDK, const ICE_CHAR* pcNumber);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_FindWhiteItem(ICE_HANDLE hSDK, const ICE_CHAR* pcNumber, 
	ICE_CHAR* pcDateBegin, ICE_CHAR* pcDateEnd, ICE_CHAR* pcTimeBegin, ICE_CHAR* pcTimeEnd, 
	ICE_CHAR* pcRsrv1, ICE_CHAR* pcRsrv2, ICE_CHAR* pcRsrv3, ICE_CHAR* pcRsrv4);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_DeleteBlackItem(ICE_HANDLE hSDK, const ICE_CHAR* pcNumber);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_FindBlackItem(ICE_HANDLE hSDK, const ICE_CHAR* pcNumber, 
	ICE_CHAR* pcDateBegin, ICE_CHAR* pcDateEnd, ICE_CHAR* pcTimeBegin, ICE_CHAR* pcTimeEnd, 
	ICE_CHAR* pcRsrv1, ICE_CHAR* pcRsrv2, ICE_CHAR* pcRsrv3, ICE_CHAR* pcRsrv4);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetAutoRebootParam(ICE_HANDLE hSDK, AutoRebootParam *pstParam);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetAutoRebootParam(ICE_HANDLE hSDK, const AutoRebootParam *pstParam);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_Broadcast(ICE_HANDLE hSDK, ICE_U16 nIndex);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetCity(ICE_HANDLE hSDK, ICE_U32 u32Index);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_BroadcastGroup(ICE_HANDLE hSDK, const ICE_CHAR* pcIndex);

ICE_IPCSDK_API void ICE_IPCSDK_SearchDev(char* szDevs);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetDNSAddr(ICE_HANDLE hSDK, ICE_U32 *pu32DNS, ICE_U32 *pu32DNS2);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetDNSAddr(ICE_HANDLE hSDK, ICE_U32 u32DNS, ICE_U32 u32DNS2);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetLightParam(ICE_HANDLE hSDK, LightParam *pstParam);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetLightParam(ICE_HANDLE hSDK, const LightParam *pstParam);
ICE_IPCSDK_API void ICE_IPCSDK_LogConfig(int openLog, char *logPath);

ICE_IPCSDK_API void ICE_IPCSDK_EnableUTF8(ICE_HANDLE hSDK, int enable);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetReOpenParam(ICE_HANDLE hSDK, ReOpenParam *pstParam);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetReOpenParam(ICE_HANDLE hSDK, const ReOpenParam *pstParam);

ICE_IPCSDK_API ICE_FLOAT ICE_IPCSDK_VBR_CompareFeat(ICE_FLOAT *_pfResFeat1, ICE_U32 _iFeat1Len,
												   ICE_FLOAT *_pfReaFeat2, ICE_U32 _iFeat2Len);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetOfflineAoParam(ICE_HANDLE hSDK, OfflineAoParam *pstParam);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetOfflineAoParam(ICE_HANDLE hSDK, const OfflineAoParam *pstParam);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_Broadcast_ByName(ICE_HANDLE hSDK, const ICE_CHAR* pcName);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_BroadcastGroup_ByName(ICE_HANDLE hSDK, const ICE_CHAR* pcName);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetIOState(ICE_HANDLE hSDK, ICE_U32 u32Index, ICE_U32 *pu32IOState, 
												ICE_U32 *pu32Reserve1, ICE_U32 *pu32Reserve2);

ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_GetLEDParam(ICE_HANDLE hSDK, ICE_LED_PARAM *pstParam);
ICE_IPCSDK_API ICE_U32 ICE_IPCSDK_SetLEDParam(ICE_HANDLE hSDK, const ICE_LED_PARAM *pstParam);

#ifdef __cplusplus
}
#endif

#endif
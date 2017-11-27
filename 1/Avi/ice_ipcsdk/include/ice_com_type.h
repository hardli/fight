/*! \file ice_com_type.h
\brief ice_com_type.h document.
*/
#ifndef _ICE_COM_TYPE_H_
#define _ICE_COM_TYPE_H_

#include "ice_base_type.h"

//! \defgroup group_communication 通信协议
//!  通信协议数据类型
//! \{
//! \}

#ifdef __cplusplus
extern "C"
{
#endif


#define ICE_BUF_SIZE_CONTROL	(64-32)
#define ICE_BUF_SIZE_EVENT		(64-32)
#define ICE_BUF_SIZE_VPRO		(16*1024-32)
#define ICE_BUF_SIZE_CONFIG		(128*1024-32)

#define ICE_BUFLEN_PACKET	(1024*512)

#define ICE_EVENT_ID_SLAVE_MIN	0x00000000
#define ICE_EVENT_ID_SLAVE_MAX	0xFFFFFFFF

#define ICE_BUF_SIZE_FRAME		(1920*1080*2 + 128)



//! \ingroup group_communication
//! 状态码
typedef enum
{
	ICE_STATUS_SUCCESS				= 0x00000000,		//!<  成功
	ICE_STATUS_COMMAND				= 0x00010000,		//!<  命令
	ICE_STATUS_ERROR2				= 0x00080000,		//!<  错误
	ICE_STATUS_ERROR_NOT_FOUND		= 0x00080001,		//!<  没有查找到
	ICE_STATUS_ERROR_NOT_SUPPORTED	= 0x00080002,		//!<  不支持
} ICE_COM_STATUS_E;

//! \ingroup group_communication
//! 操作通用数据
typedef struct 
{
	ICE_U32	u32Size;							//!<  数据包长度
	ICE_U32	u32Type;							//!<  类型
	ICE_U32	u32Status;							//!<  状态
	ICE_U32	u32Channel;							//!<  通道号
	ICE_U32	u32Time;							//!<  时间
	ICE_U32	u32Date;							//!<  日期
	ICE_U32	u32Id;								//!<  标识
	ICE_U32	u32Level;							//!<  警报级别
} ICE_OPER_DATA_S;

///////////////////////////////////////////////////////////////////////////////////////////////////
// 控制相关数据类型

// 语音播报
typedef struct
{
	char filename[128];		//语音播报的文件名，.g711a
	int index;				//成组播放语音的索引

	char reserved[124];
	
} Broadcast_Param;

typedef struct
{
	unsigned char idle_state;
	unsigned int delay_time;
	
} relay_output;


typedef struct
{
	unsigned int IPaddr;
	unsigned int netmaskAddr;
	unsigned int gatewayAddr;
}NET_BASE_PARAM;


//! \ingroup group_communication
//! 控制类数据类型
typedef enum
{		
	ICE_CONTROL_TYPE_BASE 						= 0x1000,			//!< 控制基础字段
	ICE_CONTROL_TYPE_VDR_ARM					= 0x1001,			//!< 启动警戒
	ICE_CONTROL_TYPE_VDR_ARM_RSP				= 0x1002,			//!< 启动警戒应答
	ICE_CONTROL_TYPE_VDR_DISARM					= 0x1003,			//!< 解除警戒
	ICE_CONTROL_TYPE_VDR_DISARM_RSP			    = 0x1004,			//!< 解除警戒应答
	ICE_CONTROL_TYPE_VDR_CLEAR_ALARAM			= 0x1005,			//!< 清除警报
	ICE_CONTROL_TYPE_VDR_CLEAR_ALARAM_RSP     	= 0x1006,			//!< 清除警报应答
	ICE_CONTROL_TYPE_VDR_REQUEST_ALARM_DATA		= 0x1007,			//!< 请求报警数据
	ICE_CONTROL_TYPE_VDR_REQUEST_ALARM_DATA_RSP = 0x1008,			//!< 请求报警数据应答	
	ICE_CONTROL_TYPE_STREAM_CONNECT			    = 0x1010,			//!< 请求视频连接
	ICE_CONTROL_TYPE_STREAM_CONNECT_RSP     	= 0x1011,			//!< 请求视频连接应答
	ICE_CONTROL_TYPE_STREAM_DISCONNECT			= 0x1012,			//!< 断开视频连接
	ICE_CONTROL_TYPE_STREAM_DISCONNECT_RSP     	= 0x1013,			//!< 断开视频连接应答
	ICE_CONTROL_TYPE_RESTART					= 0x1020,			//!< 重启设备
	ICE_CONTROL_TYPE_RESTART_RSP     			= 0x1021,			//!< 重启设备应答	
	ICE_CONTROL_TYPE_HEART_BEAT					= 0x1022,			//!< 设备心跳
	ICE_CONTROL_TYPE_HEART_BEAT_RSP     		= 0x1023,			//!< 设备心跳应答	
	ICE_CONTROL_TYPE_ERROR_REPORT				= 0x1024,			//!< 设备错误上报
	ICE_CONTROL_TYPE_ERROR_REPORT_RSP     		= 0x1025,			//!< 设备错误上报应答	
	ICE_CONTROL_TYPE_REQUEST_COUNT				= 0x1026,			//!< 请求计数统计结果
	ICE_CONTROL_TYPE_REQUEST_COUNT_RSP     		= 0x1027,			//!< 请求计数统计结果应答
	ICE_CONTROL_TYPE_TALK_DATA					= 0x1028,			//!< 语音对讲
	ICE_CONTROL_TYPE_TALK_BEGIN					= 0x1029,			//!< 语音对讲开始
	ICE_CONTROL_TYPE_TALK_END					= 0x1030,			//!< 语音对讲结束
	ICE_CONTROL_TYPE_SDCARD_FORMAT              = 0x1031,           //!<sd卡格式化	
	ICE_CONTROL_TYPE_SDCARD_FORMAT_RSP          = 0x1032,           //!<sd卡格式化应答	
	ICE_CONTROL_TYPE_CAPTURE					= 0x1033,           //!< 抓拍（不做车牌识别）
	ICE_CONTROL_TYPE_CAPTURE_RSP				= 0x1034,           //!< 抓拍（不做车牌识别）应答
	ICE_CONTROL_TYPE_SDCARD_UNLOAD				= 0x1035,			//!< SD卡卸载
	ICE_CONTROL_TYPE_SDCARD_UNLOAD_RSP			= 0x1036,			//!<SD 卡卸载应答
	
	ICE_CONTROL_TYPE_TRANS_SERIAL_PORT			= 0x1037,           //!< 透传串口数据请求
	ICE_CONTROL_TYPE_TRANS_SERIAL_PORT_RSP		= 0x1038,           //!< 透传串口数据请求应答
	
	ICE_CONTROL_TYPE_BROADCSAT					= 0x1039,           //!< 语音播报请求
	ICE_CONTROL_TYPE_BROADCSAT_RSP				= 0x1040,           //!< 语音播报请求应答

	ICE_CONTROL_TYPE_VDR_REQUEST_ALARM_DATA_PASSWD		= 0x1041,			//!< 请求报警数据（带密码）
	ICE_CONTROL_TYPE_VDR_REQUEST_ALARM_DATA_PASSWD_RSP	= 0x1042,			//!< 请求报警数据（带密码）应答	
	
	ICE_CONTROL_TYPE_TRANS_SERIAL_PORT_RS232			= 0x1043,           //!< 透传串口数据请求
	ICE_CONTROL_TYPE_TRANS_SERIAL_PORT_RS232_RSP		= 0x1044,           //!< 透传串口数据请求应答
	
	ICE_CONTROL_TYPE_GET_IO_IN_STATE							= 0x1045,		//!<IO 输入状态获取请求
	ICE_CONTROL_TYPE_GET_IO_IN_STATE_RSP					= 0x1046,			//!<IO 输入状态获取请求应答

	ICE_CONTROL_TYPE_VDR_REQUEST_ALARM_DATA_NO_PIC	= 0x1047,			//!< 请求报警数据(无图片数据)
	ICE_CONTROL_TYPE_VDR_REQUEST_ALARM_DATA_NO_PIC_RSP = 0x1048,			//!< 请求报警数据应答(无图片数据)	

	ICE_CONTROL_TYPE_VDR_REQUEST_ALARM_DATA_NO_PIC_PASSWD = 0x1049,			//!< 请求报警数据(无图片数据)
	ICE_CONTROL_TYPE_VDR_REQUEST_ALARM_DATA_NO_PIC_PASSWD_RSP = 0x1050,			//!< 请求报警数据应答(无图片数据)
	
	ICE_CONTROL_TYPE_END						= 0x1051,			
} ICE_CONTROL_TYPE_E;


//! 控制类数据缓存
typedef union 
{
	ICE_U8 au8Buffer[ICE_BUF_SIZE_CONTROL];
} ICE_CONTROL_DATA_INTERNAL_U;

//! \ingroup group_communication
//! 控制类数据头和数据缓存
typedef struct 
{
	ICE_OPER_DATA_S stHead;						
	ICE_CONTROL_DATA_INTERNAL_U unData;			
} ICE_CONTROL_DATA_S;

// 控制操作相关数据类型
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
// 事件相关数据类型

//! \ingroup group_communication
//! 事件数据类型
typedef enum
{
	ICE_EVENT_TYPE_BASE			= 0x2000,		//!< 事件基础字段
	ICE_EVENT_TYPE_DTCA_EVENT	= 0x2001,		//!< 周界事件集合
	ICE_EVENT_TYPE_VLPR_EVENT	= 0x2002,		//!< 卡口事件集合
    ICE_EVENT_TYPE_SFDV_EVENT	= 0x2004,		//!< 行车事件集合
	ICE_EVENT_TYPE_VLPREXT_EVENT	= 0x2005,	//!< 车牌扩展事件集合
	ICE_EVENT_TYPE_HCNT_EVENT	= 0x2006,		//!< 人数事件集合
	ICE_EVENT_TYPE_RS232_EVENT	= 0x20ff,		//!< 透明串口(rs232)回传数据
	ICE_EVENT_TYPE_RS485_EVENT	= 0x2100,		//!< 透明串口(rs485)回传数据
	ICE_EVENT_TYPE_IO_EVENT     = 0x2101,		//!< 透明IO状态回调
	ICE_EVENT_TYPE_END			= 0x2102,		
} ICE_EVENT_TYPE_E;




//! 事件数据缓存
typedef union 
{
	ICE_U8 au8Buffer[ICE_BUF_SIZE_EVENT];			//!< 事件数据缓存
} ICE_EVENT_DATA_INTERNAL_U;

//! \ingroup group_communication
//! 事件数据头和数据缓存
typedef struct 
{
	ICE_OPER_DATA_S stHead;							//!< 操作头
	ICE_EVENT_DATA_INTERNAL_U unData;				//!< 事件数据
} ICE_EVENT_DATA_S;

// 事件相关数据类型
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
// 视频处理相关数据类型

//! \ingroup group_communication
//! 目标处理数据类型
typedef enum
{
	ICE_VPRO_TYPE_BASE 				= 0x3000,		//!< 目标数据基础字段
	ICE_VPRO_TYPE_DTCA_TARGET_SET	= 0x3001,		//!< 周界算法目标
	ICE_VPRO_TYPE_VLPR_TARGET_SET	= 0x3002,		//!< 卡口算法目标
	ICE_VPRO_TYPE_SFDV_TARGET_SET	= 0x3004,		//!< 行车算法目标
	ICE_VPRO_TYPE_SNAP				= 0x3100,		//!< 抓拍数据
	ICE_VPRO_TYPE_END				= 0x3101,		
} ICE_VPRO_TYPE_E;


//! 目标处理数据缓存
typedef union 
{
	ICE_U8 au8Buffer[ICE_BUF_SIZE_VPRO];			//!< 目标数据缓存
} ICE_VPRO_DATA_INTERNAL_U;

//! \ingroup group_communication
//! 目标处理数据头和数据缓存
typedef struct 
{
	ICE_OPER_DATA_S stHead;							//!< 操作头
	ICE_VPRO_DATA_INTERNAL_U unData;				//!< 目标数据
} ICE_VPRO_DATA_S;

// 视频处理相关数据类型
///////////////////////////////////////////////////////////////////////////////////////////////////


//! \ingroup group_communication
//! 算法配置相关数据类型
typedef enum
{
	ICE_ALGO_CONFIG_TYPE_BASE 						= 0x4000,		//!< 算法配置类型基础字段

	ICE_ALGO_CONFIG_TYPE_SET_AGLO_MODULE			= 0x4001,		//!< 设置算法模块
	ICE_ALGO_CONFIG_TYPE_SET_AGLO_MODULE_RSP		= 0x4002,		//!< 设置算法模块应答	
	ICE_ALGO_CONFIG_TYPE_GET_AGLO_MODULE			= 0x4003,		//!< 获取算法模块
	ICE_ALGO_CONFIG_TYPE_GET_AGLO_MODULE_RSP		= 0x4004,		//!< 获取算法模块应答

	ICE_ALGO_CONFIG_TYPE_SET_VIDEO_SOURCE_MODE		= 0x4100,		//!< 设置视频输入制式
	ICE_ALGO_CONFIG_TYPE_SET_VIDEO_SOURCE_MODE_RSP	= 0x4101,		//!< 设置视频输入制式应答
	
	ICE_ALGO_CONFIG_TYPE_GET_VIDEO_SOURCE_MODE		= 0x4102,		//!< 获取视频输入制式
	ICE_ALGO_CONFIG_TYPE_GET_VIDEO_SOURCE_MODE_RSP	= 0x4103,		//!< 获取视频输入制式应答
	

	ICE_ALGO_CONFIG_TYPE_SET_DTCA_ALGO_LIB_INFO		= 0x4200,		//!< 设置周界算法库信息(功能码 )
	ICE_ALGO_CONFIG_TYPE_SET_DTCA_ALGO_LIB_INFO_RSP	= 0x4201,		//!< 设置周界算法库信息应答	
	ICE_ALGO_CONFIG_TYPE_GET_DTCA_ALGO_LIB_INFO		= 0x4202,		//!< 获取周界算法库信息（功能码、版本号、最大输出目标能力等）
	ICE_ALGO_CONFIG_TYPE_GET_DTCA_ALGO_LIB_INFO_RSP	= 0x4203,		//!< 获取周界算法库信息应答
	
	ICE_ALGO_CONFIG_TYPE_SET_VLPR_ALGO_LIB_INFO		= 0x4204,		//!< 设置卡口算法库信息(功能码 )
	ICE_ALGO_CONFIG_TYPE_SET_VLPR_ALGO_LIB_INFO_RSP	= 0x4205,		//!< 设置卡口算法库信息应答	
	ICE_ALGO_CONFIG_TYPE_GET_VLPR_ALGO_LIB_INFO		= 0x4206,		//!< 获取卡口算法库信息（功能码、版本号、最大输出目标能力等）
	ICE_ALGO_CONFIG_TYPE_GET_VLPR_ALGO_LIB_INFO_RSP	= 0x4207,		//!< 获取卡口算法库信息应答
	
	ICE_ALGO_CONFIG_TYPE_SET_SFDV_ALGO_LIB_INFO		= 0x4208,		//!< 设置行车算法库信息(功能码 )
	ICE_ALGO_CONFIG_TYPE_SET_SFDV_ALGO_LIB_INFO_RSP	= 0x4209,		//!< 设置行车算法库信息应答	
	ICE_ALGO_CONFIG_TYPE_GET_SFDV_ALGO_LIB_INFO		= 0x4210,		//!< 获取行车算法库信息（功能码、版本号、最大输出目标能力等）
	ICE_ALGO_CONFIG_TYPE_GET_SFDV_ALGO_LIB_INFO_RSP	= 0x4211,		//!< 获取行车算法库信息应答
	
	ICE_ALGO_CONFIG_TYPE_SET_ITSE_ALGO_LIB_INFO		= 0x4212,		//!< 设置交通事件算法库信息(功能码 )
	ICE_ALGO_CONFIG_TYPE_SET_ITSE_ALGO_LIB_INFO_RSP	= 0x4213,		//!< 设置交通事件算法库信息应答	
	ICE_ALGO_CONFIG_TYPE_GET_ITSE_ALGO_LIB_INFO		= 0x4214,		//!< 获取交通事件算法库信息（功能码、版本号、最大输出目标能力等）
	ICE_ALGO_CONFIG_TYPE_GET_ITSE_ALGO_LIB_INFO_RSP	= 0x4215,		//!< 获取交通事件算法库信息应答

	ICE_ALGO_CONFIG_TYPE_SET_DTCA_RULE_PARAM		= 0x4300,		//!< 设置周界算法分析规则
	ICE_ALGO_CONFIG_TYPE_SET_DTCA_RULE_PARAM_RSP	= 0x4301,		//!< 设置周界算法分析规则应答
	ICE_ALGO_CONFIG_TYPE_GET_DTCA_RULE_PARAM		= 0x4302, 		//!< 获取周界算法分析规则
	ICE_ALGO_CONFIG_TYPE_GET_DTCA_RULE_PARAM_RSP	= 0x4303, 		//!< 获取周界算法分析规则应答
	
	ICE_ALGO_CONFIG_TYPE_SET_VLPR_RULE_PARAM		= 0x4304,		//!< 设置卡口算法分析规则
	ICE_ALGO_CONFIG_TYPE_SET_VLPR_RULE_PARAM_RSP	= 0x4305,		//!< 设置卡口算法分析规则应答
	ICE_ALGO_CONFIG_TYPE_GET_VLPR_RULE_PARAM		= 0x4305,		//!< 获取卡口算法分析规则
	ICE_ALGO_CONFIG_TYPE_GET_VLPR_RULE_PARAM_RSP	= 0x4306,		//!< 获取卡口算法分析规则应答
	
	ICE_ALGO_CONFIG_TYPE_SET_SFDV_RULE_PARAM		= 0x4307,		//!< 设置行车算法分析规则
	ICE_ALGO_CONFIG_TYPE_SET_SFDV_RULE_PARAM_RSP	= 0x4308,		//!< 设置行车算法分析规则应答
	ICE_ALGO_CONFIG_TYPE_GET_SFDV_RULE_PARAM		= 0x4309, 		//!< 获取行车算法分析规则
	ICE_ALGO_CONFIG_TYPE_GET_SFDV_RULE_PARAM_RSP	= 0x4310, 		//!< 获取行车算法分析规则应答
	
	ICE_ALGO_CONFIG_TYPE_SET_HCNT_RULE_PARAM		= 0x4311,		//!< 设置计数算法分析规则
	ICE_ALGO_CONFIG_TYPE_SET_HCNT_RULE_PARAM_RSP	= 0x4312,		//!< 设置计数算法分析规则应答
	ICE_ALGO_CONFIG_TYPE_GET_HCNT_RULE_PARAM		= 0x4313, 		//!< 获取计数算法分析规则
	ICE_ALGO_CONFIG_TYPE_GET_HCNT_RULE_PARAM_RSP	= 0x4314, 		//!< 获取计数算法分析规则应答
	
	ICE_ALGO_CONFIG_TYPE_END						= 0x4315,		
} ICE_ALGO_CONFIG_TYPE_E;

///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	ICE_U32 u32Expline;			//手动曝光时间
	ICE_S32 s32AGain;			//手动模拟增益
	ICE_S32 s32DGain;			//手动数字增益

	ICE_U32 u32Mode;			//模式：0自动，1定时，2手动

	ICE_U32 u32DayBegin;		//白天开始时间（分钟为单位，例如8点为480）
	ICE_U32 u32DayEnd;			//白天结束时间（分钟为单位，例如18点为1080）
	ICE_U32 u32DayExplineMin;	//白天最小曝光时间
	ICE_U32 u32DayExplineMax;	//白天最大曝光时间
	ICE_S32 s32DayAGainMin;		//白天最小模拟增益
	ICE_S32 s32DayAGainMax;		//白天最大模拟增益
	ICE_S32 s32DayDGainMin;		//白天最小数字增益
	ICE_S32 s32DayDGainMax;		//白天最大数字增益

	ICE_U32 u32NightBegin;		//晚上开始时间（分钟为单位，例如20点为1200）
	ICE_U32 u32NightEnd;		//晚上结束时间（分钟为单位，例如6点为360）
	ICE_U32 u32NightExplineMin;	//晚上最小曝光时间
	ICE_U32 u32NightExplineMax;	//晚上最大曝光时间
	ICE_S32 s32NightAGainMin;	//晚上最小模拟增益
	ICE_S32 s32NightAGainMax;	//晚上最大模拟增益
	ICE_S32 s32NightDGainMin;	//晚上最小数字增益
	ICE_S32 s32NightDGainMax;	//晚上最大数字增益

	ICE_U32 u32OtherExplineMin;	//其他时间最小曝光时间
	ICE_U32 u32Focus;			//焦距
	ICE_S32 s32Height;			//安装高度
	ICE_S32 s32HeightAlgo;		//安装高度自动关联
	ICE_S32 s32Distance;		//安装距离
	ICE_S32 s32DistAlgo;		//安装距离自动关联

	ICE_U32 u32LightMode;		//补光灯模式
	ICE_U32 u32ColorMode;		//彩转黑模式

	ICE_U32 u32AlgoWidth;
	ICE_U32 u32AlgoHeight;

} ICE_ExpAttr_S;

//红外灯控制模式
typedef enum
{
	ICE_LIGHT_MODE_AUTO = 0,		//光敏自动检测
	ICE_LIGHT_MODE_BY_TIME,			//时间控制
	ICE_LIGHT_MODE_BY_TMLS,			//光敏与时间综合控制，白天是光敏，其他为时间控制
} ICE_LIGHT_MODE_TYPE_E;

typedef enum
{
	ICE_COLOR_MODE_AUTO = 0,		//光敏自动检测
	ICE_COLOR_MODE_COLOR,			//彩色
	ICE_COLOR_MODE_NOCOLOR,			//黑白
	
} ICE_COLOR_MODE_TYPE_E;


typedef enum
{
	ICE_RS485_PROTOCOL_HN = 1,		//海南客流分析仪
	ICE_RS485_PROTOCOL_HL,			//上海灏泷客流计
	ICE_RS485_PROTOCOL_BA,			//上海本安
	ICE_RS485_PROTOCOL_ICE,		//智芯原动
	ICE_RS485_PROTOCOL_AJS,     //安吉升协议
	
} ICE_RS485_PROTOCOL_TYPE_E;

typedef enum
{
	ICE_HCNT_DOOR_FRONT = 0,		//前门
	ICE_HCNT_DOOR_MID,				//中门
	ICE_HCNT_DOOR_BACK				//后门
	
} ICE_HCNT_DOOR_TYPE_E;

typedef enum
{
	ICE_HCNT_LINK_SERVER_NET = 0,		//与服务器连接采用以太网模式
	ICE_HCNT_LINK_SERVER_RS485,			//与服务器连接采用人生rs485 模式
	
} ICE_HCNT_IPC_LINK_SERVER_MODE_E;

typedef enum
{
	ICE_HCNT_CLEAR_PERIOD = 0,		//周期
	ICE_HCNT_CLEAR_TRIGGER,			//外触发
	
} ICE_HCNT_CLEAR_MODE_E;

typedef enum
{
	ICE_HCNT_TRIGGER_RELAYIN = 0,		//开关量触发
	ICE_HCNT_TRIGGER_RS485,				//RS485触发
	ICE_HCNT_TRIGGER_PROGRAM,			//软件触发
	
} ICE_HCNT_TRIGGER_MODE_E;

typedef enum
{
	ICE_HCNT_RELAYIN_OFF = 0,		//闭合触发清零
	ICE_HCNT_RELAYIN_ON,			//打开触发清零
	
} ICE_HCNT_RELAYIN_MODE_E;

typedef enum
{
	ICE_HCNT_STORE_FLASH = 0,		//FLASH存储
	ICE_HCNT_STORE_SDCARD,			//SD卡存储

} ICE_HCNT_STORE_MODE;

typedef enum
{
	ICE_HCNT_COLLECT_HOUR = 0,		//小时汇总
	ICE_HCNT_COLLECT_DAY,			//天汇总

} ICE_HCNT_COLLECT_MODE;

#define MAX_HCNT_TEXT 64
typedef struct
{
	ICE_S32 s32RS485Type;
	ICE_U32 u32DoorType;
	ICE_U32 u32Railway;
	
	ICE_S8 as8ServerIP[16];
	ICE_S8 as8Username[MAX_HCNT_TEXT];
	ICE_S8 as8Password[MAX_HCNT_TEXT];
	ICE_S8 as8CameraID[MAX_HCNT_TEXT];
	ICE_U32 u32HBInterval;			//心跳周期
	
	ICE_U32 u32ClearMode;			//清零模式
	ICE_U32 u32ClearInterval;		//清零周期
	ICE_U32 u32TriggerMode;			//外触发模式
	ICE_U32 u32RelayinMode;			//开关量触发模式
	
	ICE_U32 u32DataStore;			//是否数据存储
	ICE_U32 u32StoreMode;			//数据存储模式
	ICE_U32 u32StoreSize;			//存储空间
	ICE_U32 u32StoreStat;			//是否统计存储
	ICE_U32 u32StatCollect;			//是否数据汇总
	ICE_U32 u32CollectMode;			//汇总模式
	
	ICE_U32 au32Reserved[32];

} ICE_HCNTAttr_S;


//图像里OSD位置类型

typedef enum
{
	ICE_OSD_LOCATION_LEFT_TOP = 0,	//左上
	ICE_OSD_LOCATION_RIGHT_TOP,		//右上
	ICE_OSD_LOCATION_LEFT_BOTTOM,	//左下
	ICE_OSD_LOCATION_RIGHT_BOTTOM,	//右下
	ICE_OSD_LOCATION_TOP_MID,		//上居中
	ICE_OSD_LOCATION_BOTTOM_MID,		//下居中
} ICE_OSD_LOCATION_TYPE_E;

//图像里外OSD位置类型
typedef enum
{
	ICE_OSD_LOCATION_IN_IMAGE = 0,
	ICE_OSD_LOCATION_OUT_TOP,	   //图像上方
	ICE_OSD_LOCATION_OUT_BOTTOM,   //图像下方
} ICE_OSD_BIG_LOCATION_TYPE_E;



typedef enum
{
	ICE_CAMERA_DIRECTION_NORTH = 0,	//由南向北
	ICE_CAMERA_DIRECTION_WEST,		//由东向西
	ICE_CAMERA_DIRECTION_EAST,		//由西向东
	ICE_CAMERA_DIRECTION_SOUTH		//由北向南
	
} ICE_CAMERA_DIRECTION_TYPE_E;


typedef enum
{
	ICE_OSD_FONTSIZE_SMALL= 0,	//小
	ICE_OSD_FONTSIZE_MEDIUM,		//中
	ICE_OSD_FONTSIZE_LARGE,		//大
} ICE_OSD_FONTSIZE_TYPE_E;


#define MAX_OSD_TEXT 64
#define MAX_CUSTOM_OSD_LINE_NUM 6

typedef struct
{
	//video
	ICE_U32 u32OSDLocationVideo;		//叠加位置
	ICE_U32 u32ColorVideo;				//颜色

	ICE_U32 u32DateVideo;				//是否叠加日期时间
	ICE_U32 u32License;					//是否叠加授权信息

	ICE_U32 u32CustomVideo;				//是否叠加自定义信息
	char szCustomVideo[MAX_OSD_TEXT];	//自定义信息

	//jpeg
	ICE_U32 u32OSDLocationJpeg;			//叠加位置
	ICE_U32 u32ColorJpeg;				//颜色

	ICE_U32 u32DateJpeg;				//是否叠加日期时间
	ICE_U32 u32Algo;					//是否叠加算法信息

	ICE_U32 u32DeviceID;				//是否叠加设备ID
	char szDeviceID[MAX_OSD_TEXT];		//设备ID

	ICE_U32 u32DeviceName;				//是否叠加设备名称
	char szDeviceName[MAX_OSD_TEXT];	//设备名称

	ICE_U32 u32CamreaLocation;			//是否叠加地点信息
	char szCamreaLocation[MAX_OSD_TEXT];	//地点信息

	ICE_U32 u32SubLocation;				//是否叠加子地点信息
	char szSubLocation[MAX_OSD_TEXT];	//子地点信息

	ICE_U32 u32ShowDirection;			//是否叠加相机方向
	ICE_U32 u32CameraDirection;			//相机方向

	ICE_U32 u32CustomJpeg;				//是否叠加自定义信息

	ICE_U32 u32ItemDisplayMode;             //图片信息显示模式，0，多行显示，1，单行显示,默认0
	ICE_U32 u32DateMode;                 //日期显示模式，0，xxxx/xx/xx   1，xxxx年xx月xx日，默认0
	ICE_U32 u32BgColor;                  //OSD 背景色，0背景全透明，1，背景黑色，默认0
	ICE_U32 u32FontSize;				//字体大小，0:小，1:中 2:大,默认为中，在540P 以下，中会转换为小
	ICE_U32 u32ExpandMode;				//扩张模式，0:不扩张，1:图像上扩张，2:图像下扩张
	char szCustomJpeg[MAX_OSD_TEXT - 20];	//自定义信息
	
	char szCustomVideo6[MAX_CUSTOM_OSD_LINE_NUM * MAX_OSD_TEXT];	//自定义信息
	char szCustomJpeg6[MAX_CUSTOM_OSD_LINE_NUM * MAX_OSD_TEXT];	//自定义信息

} ICE_OSDAttr_S;


#define ICE_SCHED_TASK_MASK_ARM		0x00000003
#define ICE_SCHED_TASK_MASK_RELAY	0x0000000C
#define ICE_SCHED_TASK_MASK_COLOR	0x00000030

typedef enum
{
	ICE_SCHED_TASK_TYPE_ARM			= 0x00000001, 	//布防
	ICE_SCHED_TASK_TYPE_DISARM		= 0x00000002, 	//撤防
	ICE_SCHED_TASK_TYPE_RELAY_ON	= 0x00000004, 	//开关量闭合
	ICE_SCHED_TASK_TYPE_RELAY_OFF	= 0x00000008, 	//开关量打开
	ICE_SCHED_TASK_TYPE_GRAY		= 0x00000010,	//彩转黑
	ICE_SCHED_TASK_TYPE_COLOR		= 0x00000020, 	//黑转彩
	
} ICE_SCHED_TASK_TYPE_E;

typedef struct
{
	ICE_U32 u32Time;		//时间（分钟为单位，例如20点为1200）
	ICE_U32 u32Type;		//任务类型：布撤防或者开关量输出
	ICE_U32 u32TimeEnd;		//时间（分钟为单位，例如20点为1200）
	ICE_U32 u32Reserved[7];	//保留字段
	
} ICE_SCHED_TASK_S;

typedef struct
{
	ICE_U8 u8UseSnapCamera;
	ICE_U32 u32IP;
	ICE_U32 u32Delay;
	ICE_U32 u32SnapCount;
	ICE_U32 u32SnapInterval;
	ICE_U32 u32Reserved[8];
	
} ICE_SNAP_CAMERA_S;

typedef enum
{
	ICE_PHYCHN_MODE_SUB_VENC = 0,		//子码流
	ICE_PHYCHN_MODE_VO,					//vo输出
	
} ICE_PHYCHN_MODE_TYPE_E;


typedef struct
{
	ICE_U32 u32PhyChn;
	ICE_U32 u32RCMain;
	ICE_U32 u32RCSub;
	ICE_U32 u32CloseupEn; //是否输出车牌特写图像
	ICE_U32 u32Qfactor;//抓拍图片画质1~99,默认90
	ICE_U32 u32FullEn;//是否抓拍车牌全景图
	ICE_U32 u32CloseupInFullEn; //是否在全景图上叠加车牌特写图
	ICE_U16 u16Width;//抓拍分辨率 宽度
	ICE_U16 u16Height;//抓拍分辨率高度
	ICE_U32 u32PlateSize;	//车牌图放大倍数，1 2 3
	ICE_U32 u32Reserved[2];
	
} ICE_PHYCHN_CFG_S;


typedef struct
{
	ICE_U32 u32VFDRelayout;			//火焰关联开关量输出
	ICE_U32 u32DTCARelayout;		//周界关联开关量输出
	
	ICE_U32 u32Expline;			//从相机手动曝光时间
	ICE_S32 s32AGain;			//从相机手动模拟增益
	ICE_S32 s32DGain;			//从相机手动数字增益
	ICE_U32 u32Mode;			//从相机曝光模式：0完全自动，1自动，2手动
	ICE_U32 u32ExplineMin;		//从相机最小曝光时间
	ICE_U32 u32ExplineMax;		//从相机最大曝光时间
	ICE_S32 s32AGainMin;		//从相机最小模拟增益
	ICE_S32 s32AGainMax;		//从相机最大模拟增益
	ICE_S32 s32DGainMin;		//从相机最小数字增益
	ICE_S32 s32DGainMax;		//从相机最大数字增益
	
	ICE_U32 u32Reserved[1024];			//保留字段
	
} ICE_VFD_PARAM_S;


//自动曝光自调节参数
typedef struct
{
	int expARegulateEn;//是否开启曝光自调节，0关闭，1开启，默认为：1 开启
	int dayOrNightJudgeCnt; //白天判断的连续次数，一次大概一秒；默认为：10
	int lumLimitForDay; //白天亮度分界线，默认为：75
	int lumLimitForNight; //夜晚亮度分界线，默认为：65
	int maxValidBright; //车牌识别效果亮度上限，默认为：120
	int minValidBright; //车牌识别效果亮度下限，默认为：80
	int maxValidForEnvBright; //针对无牌车环境的亮度上限，默认为：110
	int minValidForEnvBright; //针对无牌车环境的亮度下限，默认为：90
	int maxExpCompensation; //曝光补偿量的最大值，默认为：160
	int minExpCompensation;//曝光补偿量的最小值，默认为：30
	int initExpCompensation;//曝光补偿值初始值，默认为：70
	int expReguateInterval;//对于正常车牌曝光补偿量的调整间隔，默认为：5
	
	int antiflickerEn;  //ISP抗闪烁是否使能 0 关闭， 1使能，默认值为：0 关闭
	int antiflickerFrequency; //ISP 抗闪频率值，当电源频率为 50Hz 时，抗闪频率值设置为 50；当电源频率为 60Hz 时，抗闪频率值设置为 60，默认为： 50HZ
	int antiflickerMode; //ISP 抗闪模式 0，1， 默认为：1
	
	int as32Reserved[10]; //预留参数
}ICE_EXP_AREGULATE_PARAM;

//注：自动曝光自调节参数界面上只需要显示开启和关闭即可，ISP抗闪烁参数要暴露出来，其他参数如果需要调试，可用下载参数文件手动修改，然后再导入到相机


//HTTP post 参数：


typedef struct
{
	int httpPostEn; //是否打开http post 功能，默认关闭
	int directionFalg; //出口入口方向标志，0 入口，1 出口默认值为：0 入口
	int parkID; //停车场ID，默认为：13
	char ipStr[20]; //服务器IP地址，默认为：123.57.136.148
	int port; //服务器访问端口，默认为：80
	char domainName[44]; //服务器IP对应的域名，默认为：service.tigmall.com
	char ipStr_bak[20]; //备用服务器IP地址
	char path[64]; //路径，会结合出入口方向标志，停车场序号处理，默认为: /parking/count
	int maxSendCnt; //发送最大次数, 默认为：3
	int mode; // 2代表工作模式2 ，   1和3代表工作模式1
	int autoAlarmoutEn; //是否启用入口自动开闸,默认不启用
	int autoAlarmoutMode;//  模式1：识别成功开闸；（默认）,模式2：全部开闸；
	int snap_full;//  是否发送全景图；
	int snap_closeup;//  是否发送车牌特写图；
	int enable_baksvr;	//是否启用备用服务器
	short enable_offline;	//是否启用断网续传
	short interval;	//断网检测心跳周期
	int as32Reserved[3]; //预留参数
}ICE_HTTP_POST_PARAM;



#define MAX_USEABLE_UART_CNT 2  //最大可配置的串口个数

typedef enum{
	UART_PLATENUM_PROTOCOL_ONE =0,	//车牌协议1
	UART_TRANS_TRANSMIT,	//透明串口
	UART_LED_CONTROL,	//LED屏控制
	UART_MIX_MODE,		//混合模式
	UART_PLATENUM_PROTOCOL_TWO,   //车牌协议2
	UART_PLATENUM_PROTOCOL_THR   //车牌协议3
}ICE_UART_WORK_MODE_E;


typedef struct
{
	int uartEn; //串口是否使能0 不适用，1使用，默认为1使用
	int uartWorkMode; //串口工作模式，0:车牌协议，1:透明串口，默认为0 车牌协议
	int baudRate;  //波特率，默认值为9600，常用波特率 1200、2400、4800、9600、19200、38400、115200
	int dataBits;  //数据位，默认值为 8，可选值为： 5、6、7、8
	int parity;    //校验位，默认值为 无，可选值为：无、奇校验、偶校验、标记、空格
	int stopBits;  //停止位，默认值为 1，可选值为：1、2
	int flowControl; //流控模式，默认值为 无，可选值为：  无、硬件，Xon/Xoff
	int as32Reserved[5]; //预留参数
}ice_uart;

typedef struct{
	ice_uart uart_param[MAX_USEABLE_UART_CNT];//多个串口数组
	int as32Reserved[10]; //预留参数
}ICE_UART_PARAM;

//SD 卡相关参数

typedef enum{
	SDS_NO =1,	//SD卡不存在
	SDS_ERROR,	//SD卡出现错误
	SDS_ENOUGH,	//SD卡容量没有满，可用
	SDS_FULL,		//SD卡已满
	SDS_UNFORMATTED   //SD卡未格式化
}DISK_STATUS;

typedef enum{
	STORE_NO_CLIENT_CONNET =0,	//SD卡断网存储
	STORE_ALWAYS,	//SD卡实时存储
	STORE_NEVER	//SD card not store
}DISK_STORE_MODE;

typedef struct
{
	int status; //SD 卡状态，
	int totalSize; //总空间大小
	int usedSize;//已用空间大小
	int availableSize; //可用空间大小
	int SDStoreMode; //sd 存储模式，断网存储，实时存储，默认断网
	int SDFormatFlag; //sd 卡是否处于格式化中
	int SDUnloadFlag; //SD 卡正在卸载
	int SDUmountFlag;//SD 是否可以解挂，用于格式化和卸载
	int u32Reserved[6];
}SD_card_param;

#define	MAX_DISK_NAME_LEN		128	//磁盘名称最长长度
#define MAX_DISK_MOUNT_PATH_LEN	128	//磁盘Mount路径的最长长度
#define DISK_DEV_NODE_LEN			12	// EX: /dev/sda	


typedef struct
{
	int SDTotalNum;
	SD_card_param SDCard_param;
	char	MountPoint[MAX_DISK_MOUNT_PATH_LEN];	//如果有磁盘，Mount的目录名称，注意最后加上最后的"/"符号
	char	DevNode[DISK_DEV_NODE_LEN];	// EX: /dev/mmcblk0
}SD_CARD;

typedef struct
{
	char	id[64];

} Dev_ID;



typedef struct
{
	int mode;		//0表示断网关联，1表示实时工作,2 不关联开闸或不工作
	int match;		//白名单匹配相似，范围（60%-100%），默认：80%
	int black_mode; //黑名单关联道闸模式，0 不关联开闸，1 开闸
	int black_match; //黑名单匹配相似，范围（60%-100%），默认：80%
	int temp_mode;  //临时车关联道闸模式，0 不关联开闸，1 开闸
	char reserved[112];
	
} WhiteList_Param;


typedef struct
{
	int alarm_in_delay;
	int alarm_in_stable_frame;   
	int alarm_in_check_interval; //地感线圈模式下，触发延时参数0~2000ms,默认0
	int max_again_process_cnt;   //地感线圈模式下，无牌车二次处理次数0~30,默认10
	int u32Reserved[10];  
} HDTrigger_Param;

#define WHITELIST_VALID_TIME_LEN 10
typedef struct{
	char plate_number[16];
	char valid_sdate[WHITELIST_VALID_TIME_LEN+1];
	char valid_edate[WHITELIST_VALID_TIME_LEN+1];	
	char valid_stime[WHITELIST_VALID_TIME_LEN+1];
	char valid_etime[WHITELIST_VALID_TIME_LEN+1];	
	char type[WHITELIST_VALID_TIME_LEN+1];	
	char reserved[64];	
}WHITELIST_ITEM;

typedef struct
{
	ICE_U32 u32Enable;		//是否开启
	ICE_U32 u32Time;		//分钟为单位，例如8点为480
	ICE_U32 u32Reserved[16];
	
} AutoRebootParam;

typedef struct
{
	char old_passwd[16];
	char new_passwd[16];
	ICE_U32 u32Reserved[64];
	
} PasswdParam;

typedef struct
{
	ICE_U32 u32DNS;		//首选DNS
	ICE_U32 u32DNS2;	//备用DNS
	ICE_U32 u32Reserved[6];
	
} DNSParam;

typedef struct
{
	ICE_U32 u32Luminance;		//亮度
	ICE_U32 u32Reserved[7];
	
} LightParam;

typedef struct
{
	ICE_U32 u32Enable;			//是否开启
	ICE_U32 u32Interval;		//二次开闸周期
	ICE_U32 u32Reserved[16];
	
} ReOpenParam;

typedef struct
{
	ICE_U8 io_in_0;
	ICE_U8 io_in_1;
	ICE_U8 io_in_2;
	ICE_U8 io_in_3;
}IoInState;

typedef struct
{
	ICE_U8 isOpen;
}TimerReboot;

typedef struct
{
	ICE_U32 u32IP;		//FTP server ip
	char szUserName[64];		//user name
	char szPasswd[64];		//passwd
	ICE_U32 u32HeartBeatInt;		//heart inverval
	ICE_U32 u32Enable;  //FTP upload open or close
	ICE_U32 u32Plate; // 文件名是否包含车牌号
	ICE_U32 u32XML; //是否上传XML 文件
	char s8Path[100]; //上传路径
	ICE_U32 u32Reserved[2];
	
}FTPParam;

typedef struct
{
	int enable;	//是否使能
	char ledip[16];//led的ip
	int ledport;//led的端口号
	int as32Reserved[4]; //预留参数

} ICE_LED_PARAM;

#define ISP_CFG_FILE "ISP_param.cfg"
#define UART_CFG_FILE "uart_param.cfg"
#define SD_CFG_FILE "SD_param.cfg"
#define WHITELIST_CFG_FILE "whitelist_param.cfg"
#define WHITELIST_FILE "whitelist.txt"
#define USER_DATA_FILE "user_data.cfg"
#define HDTRIGGER_CFG_FILE "alarm_in_stable.txt"
#define UAER_PASSWD_FILE "user_passwd.cfg"
#define FILE_LIGHT_FILE "fill_light.cfg"




//! \ingroup group_communication
//! 相机配置相关数据类型
typedef enum
{
	ICE_CAMERA_CONFIG_TYPE_BASE 					= 0x5000,		//!< 相机配置类型基础字段

	ICE_CAMERA_CONFIG_TYPE_SET_BASE_PARAM			= 0x5001,		//!< 设置相机基本参数（名称、IP地址、设备编号等）
	ICE_CAMERA_CONFIG_TYPE_SET_BASE_PARAM_RSP		= 0x5002,		//!< 设置相机基本参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_BASE_PARAM			= 0x5003,		//!< 获取相机基本参数
	ICE_CAMERA_CONFIG_TYPE_GET_BASE_PARAM_RSP		= 0x5004,		//!< 获取相机基本参数应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_SYSTEM_TIME			= 0x5005,		//!< 设置相机系统时间
	ICE_CAMERA_CONFIG_TYPE_SET_SYSTEM_TIME_RSP		= 0x5006,		//!< 设置相机系统时间应答
	ICE_CAMERA_CONFIG_TYPE_GET_SYSTEM_TIME			= 0x5007,		//!< 获取相机系统时间
	ICE_CAMERA_CONFIG_TYPE_GET_SYSTEM_TIME_RSP		= 0x5008,		//!< 获取相机系统时间应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_ISP_PARAM			= 0x5009,		//!< 设置ISP参数（曝光、增益、白平衡)
	ICE_CAMERA_CONFIG_TYPE_SET_ISP_PARAM_RSP		= 0x5010,		//!< 设置ISP参数应!答
	ICE_CAMERA_CONFIG_TYPE_GET_ISP_PARAM			= 0x5011,		//!< 获取ISP参数
	ICE_CAMERA_CONFIG_TYPE_GET_ISP_PARAM_RSP		= 0x5012,		//!< 获取ISP参数应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_VENC_PARAM			= 0x5013,		//!< 设置编码参数（码流、分辨率、码率控制方式等）
	ICE_CAMERA_CONFIG_TYPE_SET_VENC_PARAM_RSP		= 0x5014,		//!< 设置编码参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_VENC_PARAM			= 0x5015,		//!< 获取编码参数
	ICE_CAMERA_CONFIG_TYPE_GET_VENC_PARAM_RSP		= 0x5016,		//!< 设置编码参数应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_IO_PARAM				= 0x5017,		//!< 设置IO参数（电平输出（高、低、上升沿、下降沿）、光耦输出）
	ICE_CAMERA_CONFIG_TYPE_SET_IO_PARAM_RSP			= 0x5018,		//!< 设置IO参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_IO_PARAM				= 0x5019,		//!< 获取IO参数
	ICE_CAMERA_CONFIG_TYPE_GET_IO_PARAM_RSP			= 0x5020,		//!< 设置IO参数应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_COM_PARAM			= 0x5021,		//!< 设置串口参数（232/485 波特率 ）
	ICE_CAMERA_CONFIG_TYPE_SET_COM_PARAM_RSP		= 0x5022,		//!< 设置串口参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_COM_PARAM			= 0x5023,		//!< 获取串口参数
	ICE_CAMERA_CONFIG_TYPE_GET_COM_PARAM_RSP		= 0x5024,		//!< 设置串口参数应答
	
	ICE_CAMERA_CONFIG_TYPE_UPDATE_PROGRAM			= 0x5025,		//!< 升级程序
	ICE_CAMERA_CONFIG_TYPE_UPDATE_PROGRAM_RSP		= 0x5026,		//!< 升级程序应答
	
	ICE_CAMERA_CONFIG_TYPE_DEFAULT_PROGRAM			= 0x5027,		//!< 恢复出厂默认程序
	ICE_CAMERA_CONFIG_TYPE_DEFAULT_PROGRAM_RSP		= 0x5028,		//!< 恢复出厂默认应答
	
	ICE_CAMERA_CONFIG_TYPE_DEFAULT_PARAM			= 0x5029,		//!< 恢复默认参数
	ICE_CAMERA_CONFIG_TYPE_DEFAULT_PARAM_RSP		= 0x5030,		//!< 恢复默认参数应答
	
	ICE_CAMERA_CONFIG_TYPE_DEFAULT_PARAM_EXP_IP		= 0x5031,		//!< 恢复除IP外的默认参数
	ICE_CAMERA_CONFIG_TYPE_DEFAULT_PARAM_EXP_IP_RSP	= 0x5032,		//!< 恢复除IP外的默认参数应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_FTP_PARAM			= 0x5033,		//!< 设置FTP参数（FTP地址、用户名、密码）
	ICE_CAMERA_CONFIG_TYPE_SET_FTP_PARAM_RSP		= 0x5034,		//!< 设置FTP参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_FTP_PARAM			= 0x5035,		//!< 获取FTP参数
	ICE_CAMERA_CONFIG_TYPE_GET_FTP_PARAM_RSP		= 0x5036,		//!< 获取FTP参数应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_SNAP_PARAM			= 0x5037,		//!< 设置抓拍机参数（是否启用抓拍机、抓拍机地址、抓拍延时）
	ICE_CAMERA_CONFIG_TYPE_SET_SNAP_PARAM_RSP		= 0x5038,		//!< 设置抓拍机参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_SNAP_PARAM			= 0x5039,		//!< 获取抓拍机参数
	ICE_CAMERA_CONFIG_TYPE_GET_SNAP_PARAM_RSP		= 0x5040,		//!< 获取抓拍机参数应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_HCNT_PARAM			= 0x5041,		//!< 设置人数统计参数（rs485协议类型、车门类型）
	ICE_CAMERA_CONFIG_TYPE_SET_HCNT_PARAM_RSP		= 0x5042,		//!< 设置人数统计参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_HCNT_PARAM			= 0x5043,		//!< 获取人数统计参数
	ICE_CAMERA_CONFIG_TYPE_GET_HCNT_PARAM_RSP		= 0x5044,		//!< 获取人数统计参数应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_ALARM_IN_CFG			= 0x5045,		//!< 设置告警输入配置（常开或常闭）
	ICE_CAMERA_CONFIG_TYPE_SET_ALARM_IN_CFG_RSP		= 0x5046,		//!< 设置告警输入配置应答
	ICE_CAMERA_CONFIG_TYPE_GET_ALARM_IN_CFG			= 0x5047,		//!< 获取告警输入配置
	ICE_CAMERA_CONFIG_TYPE_GET_ALARM_IN_CFG_RSP		= 0x5048,		//!< 获取告警输入配置应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_TMSYNC_CFG				= 0x5049,		//!< 设置时间同步配置
	ICE_CAMERA_CONFIG_TYPE_SET_TMSYNC_CFG_RSP			= 0x5050,		//!< 设置时间同步配置应答
	ICE_CAMERA_CONFIG_TYPE_GET_TMSYNC_CFG				= 0x5051,		//!< 获取时间同步配置
	ICE_CAMERA_CONFIG_TYPE_GET_TMSYNC_CFG_RSP			= 0x5052,		//!< 获取时间同步配置应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_OSD_CFG				= 0x5053,		//!< 设置OSD配置
	ICE_CAMERA_CONFIG_TYPE_SET_OSD_CFG_RSP			= 0x5054,		//!< 设置OSD配置应答
	ICE_CAMERA_CONFIG_TYPE_GET_OSD_CFG				= 0x5055,		//!< 获取OSD配置
	ICE_CAMERA_CONFIG_TYPE_GET_OSD_CFG_RSP			= 0x5056,		//!< 获取OSD配置应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_SCHED_TASK_CFG		= 0x5061,		//!< 设置计划任务配置
	ICE_CAMERA_CONFIG_TYPE_SET_SCHED_TASK_CFG_RSP	= 0x5062,		//!< 设置计划任务配置应答
	ICE_CAMERA_CONFIG_TYPE_GET_SCHED_TASK_CFG		= 0x5063,		//!< 获取计划任务配置
	ICE_CAMERA_CONFIG_TYPE_GET_SCHED_TASK_CFG_RSP	= 0x5064,		//!< 获取计划任务配置应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_RELAY_OUT		= 0x5065,		//!< 设置开关量输出
	ICE_CAMERA_CONFIG_TYPE_SET_RELAY_OUT_RSP	= 0x5066,		//!< 设置开关量输出应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_PHYCHN_CFG				= 0x5067,		//!< 设置物理通道配置
	ICE_CAMERA_CONFIG_TYPE_SET_PHYCHN_CFG_RSP			= 0x5068,		//!< 设置物理通道配置应答
	ICE_CAMERA_CONFIG_TYPE_GET_PHYCHN_CFG				= 0x5069,		//!< 获取物理通道配置
	ICE_CAMERA_CONFIG_TYPE_GET_PHYCHN_CFG_RSP			= 0x5070,		//!< 获取物理通道配置应答

	
	ICE_CAMERA_CONFIG_TYPE_CHECK_VFD_COMM			= 0x5071,		//!< 验证火焰相机主从通讯
	ICE_CAMERA_CONFIG_TYPE_CHECK_VFD_COMM_RSP		= 0x5072,		//!< 验证火焰相机主从通讯应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_VFD_PARAM		= 0x5073,		//!< 设置火焰相机参数
	ICE_CAMERA_CONFIG_TYPE_SET_VFD_PARAM_RSP	= 0x5074,		//!< 设置火焰相机参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_VFD_PARAM		= 0x5075,		//!< 获取火焰相机参数
	ICE_CAMERA_CONFIG_TYPE_GET_VFD_PARAM_RSP	= 0x5076,		//!< 获取火焰相机参数应答

	ICE_CAMERA_CONFIG_TYPE_SET_UART_PARAM		= 0x5077,		//!< 设置相机串口参数
	ICE_CAMERA_CONFIG_TYPE_SET_UART_PARAM_RSP	= 0x5078,		//!< 设置相机串口参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_UART_PARAM		= 0x5079,		//!< 获取相机串口参数
	ICE_CAMERA_CONFIG_TYPE_GET_UART_PARAM_RSP	= 0x5080,		//!< 获取相机串口参数应答

	ICE_CAMERA_CONFIG_TYPE_SET_HTTP_POST_PARAM		= 0x5081,		//!< 设置HTTP POST参数
	ICE_CAMERA_CONFIG_TYPE_SET_HTTP_POST_PARAM_RSP	= 0x5082,		//!< 设置HTTP POST参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_HTTP_POST_PARAM		= 0x5083,		//!< 获取HTTP POST参数
	ICE_CAMERA_CONFIG_TYPE_GET_HTTP_POST_PARAM_RSP	= 0x5084,		//!< 获取HTTP POST参数应答

	ICE_CAMERA_CONFIG_TYPE_SET_EXP_AREGULATE_PARAM		= 0x5085,		//!< 设置曝光自动调节参数
	ICE_CAMERA_CONFIG_TYPE_SET_EXP_AREGULATE_PARAM_RSP	= 0x5086,		//!< 设置曝光自动调节参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_EXP_AREGULATE_PARAM		= 0x5087,		//!< 获取曝光自动调节参数
	ICE_CAMERA_CONFIG_TYPE_GET_EXP_AREGULATE_PARAM_RSP	= 0x5088,		//!< 获取曝光自动调节参数应答


	ICE_CAMERA_CONFIG_TYPE_SET_SDCARD_PARAM		= 0x5089,		//!< 设置VDC SD卡存储参数
	ICE_CAMERA_CONFIG_TYPE_SET_SDCARD_PARAM_RSP	= 0x5090,		//!< 设置VDC SD卡存储参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_SDCARD_PARAM		= 0x5091,		//!< 获取VDC SD卡存储参数
	ICE_CAMERA_CONFIG_TYPE_GET_SDCARD_PARAM_RSP	= 0x5092,		//!< 获取VDC SD卡存储参数应答

	ICE_CAMERA_CONFIG_TYPE_GET_DEV_ID		= 0x5093,		//!< 获取设备唯一ID
	ICE_CAMERA_CONFIG_TYPE_GET_DEV_ID_RSP	= 0x5094,		//!< 获取设备唯一ID应答

	ICE_CAMERA_CONFIG_TYPE_SET_WHITELIST_PARAM		= 0x5095,		//!< 设置白名单参数
	ICE_CAMERA_CONFIG_TYPE_SET_WHITELIST_PARAM_RSP	= 0x5096,		//!< 设置白名单参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_WHITELIST_PARAM		= 0x5097,		//!< 获取白名单参数
	ICE_CAMERA_CONFIG_TYPE_GET_WHITELIST_PARAM_RSP	= 0x5098,		//!< 获取白名单参数应答

	ICE_CAMERA_CONFIG_TYPE_WRITE_USER_DATA		= 0x5099,		//!< 写入用户数据
	ICE_CAMERA_CONFIG_TYPE_WRITE_USER_DATA_RSP	= 0x5100,		//!< 写入用户数据应答
	ICE_CAMERA_CONFIG_TYPE_READ_USER_DATA		= 0x5101,		//!< 读出用户数据
	ICE_CAMERA_CONFIG_TYPE_READ_USER_DATA_RSP	= 0x5102,		//!< 读出用户数据应答

	ICE_CAMERA_CONFIG_TYPE_WRITE_WHITE_LIST		= 0x5103,		//!< 写入白名单
	ICE_CAMERA_CONFIG_TYPE_WRITE_WHITE_LIST_RSP	= 0x5104,		//!< 写入白名单应答
	ICE_CAMERA_CONFIG_TYPE_READ_WHITE_LIST		= 0x5105,		//!< 读出白名单
	ICE_CAMERA_CONFIG_TYPE_READ_WHITE_LIST_RSP	= 0x5106,		//!< 读出白名单应答
	

	ICE_CAMERA_CONFIG_TYPE_SET_HDTRIGGER_PARAM		= 0x5107,		//!< 设置硬触发参数
	ICE_CAMERA_CONFIG_TYPE_SET_HDTRIGGER_PARAM_RSP	= 0x5108,		//!< 设置硬触发参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_HDTRIGGER_PARAM		= 0x5109,		//!< 获取硬触发参数
	ICE_CAMERA_CONFIG_TYPE_GET_HDTRIGGER_PARAM_RSP	= 0x5110,		//!< 获取硬触发参数应答

	ICE_CAMERA_CONFIG_TYPE_ADD_WHITE_LIST		= 0x5111,		//!< 增加白名单
	ICE_CAMERA_CONFIG_TYPE_ADD_WHITE_LIST_RSP	= 0x5112,		//!< 增加白名单应答
	ICE_CAMERA_CONFIG_TYPE_DEL_WHITE_LIST		= 0x5113,		//!< 删除白名单
	ICE_CAMERA_CONFIG_TYPE_DEL_WHITE_LIST_RSP	= 0x5114,		//!< 删除白名单应答
	ICE_CAMERA_CONFIG_TYPE_EDIT_WHITE_LIST		= 0x5115,		//!< 编辑白名单
	ICE_CAMERA_CONFIG_TYPE_EDIT_WHITE_LIST_RSP	= 0x5116,		//!< 编辑白名单应答
	ICE_CAMERA_CONFIG_TYPE_FIND_WHITE_LIST		= 0x5117,		//!< 查找白名单
	ICE_CAMERA_CONFIG_TYPE_FIND_WHITE_LIST_RSP	= 0x5118,		//!< 查找白名单应答
	ICE_CAMERA_CONFIG_TYPE_DELALL_WHITE_LIST		= 0x5119,		//!< 删除所有白名单
	ICE_CAMERA_CONFIG_TYPE_DELALL_WHITE_LIST_RSP	= 0x5120,		//!< 删除所有白名单应答

	ICE_CAMERA_CONFIG_TYPE_SET_AUTO_REBOOT_PARAM		= 0x5121,		//!< 设置自动重启参数
	ICE_CAMERA_CONFIG_TYPE_SET_AUTO_REBOOT_PARAM_RSP	= 0x5122,		//!< 设置自动重启参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_AUTO_REBOOT_PARAM		= 0x5123,		//!< 获取自动重启参数
	ICE_CAMERA_CONFIG_TYPE_GET_AUTO_REBOOT_PARAM_RSP	= 0x5124,		//!< 获取自动重启参数应答

	ICE_CAMERA_CONFIG_TYPE_SET_PASSWD_PARAM		= 0x5125,		//!< 设置密码参数
	ICE_CAMERA_CONFIG_TYPE_SET_PASSWD_PARAM_RSP	= 0x5126,		//!< 设置密码参数应答
	ICE_CAMERA_CONFIG_TYPE_RM_LOG_FILE		= 0x5127,		//!< 删除日志文件
	ICE_CAMERA_CONFIG_TYPE_RM_LOG_FILE_RSP	= 0x5128,		//!<  删除日志文件应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_DNS_PARAM			= 0x5129,		//!< 设置相机DNS参数
	ICE_CAMERA_CONFIG_TYPE_SET_DNS_PARAM_RSP		= 0x5130,		//!< 设置相机DNS参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_DNS_PARAM			= 0x5131,		//!< 获取相机DNS参数
	ICE_CAMERA_CONFIG_TYPE_GET_DNS_PARAM_RSP		= 0x5132,		//!< 获取相机DNS参数应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_LIGHT_PARAM			= 0x5133,		//!< 设置补光灯参数
	ICE_CAMERA_CONFIG_TYPE_SET_LIGHT_PARAM_RSP		= 0x5134,		//!< 设置补光灯参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_LIGHT_PARAM			= 0x5135,		//!< 获取补光灯参数
	ICE_CAMERA_CONFIG_TYPE_GET_LIGHT_PARAM_RSP		= 0x5136,		//!< 获取补光灯参数应答
	
	ICE_CAMERA_CONFIG_TYPE_SET_RELAY_OUT2		= 0x5137,		//!< 设置开关量2输出
	ICE_CAMERA_CONFIG_TYPE_SET_RELAY_OUT2_RSP	= 0x5138,		//!< 设置开关量2输出应答

	ICE_CAMERA_CONFIG_TYPE_SET_REOPEN_PARAM		= 0x5139,		//!< 设置二次开闸参数
	ICE_CAMERA_CONFIG_TYPE_SET_REOPEN_PARAM_RSP	= 0x5140,		//!< 设置二次开闸参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_REOPEN_PARAM		= 0x5141,		//!< 获取二次开闸参数
	ICE_CAMERA_CONFIG_TYPE_GET_REOPEN_PARAM_RSP	= 0x5142,		//!< 获取二次开闸参数应答

	ICE_CAMERA_CONFIG_TYPE_SET_OFFLINE_AO_PLAY_PARAM = 0x5143, //!<设置脱机语音播报类型
	ICE_CAMERA_CONFIG_TYPE_SET_OFFLINE_AO_PLAY_PARAM_RSP = 0x5144,//!>设置脱机语音播报类型应答
	ICE_CAMERA_CONFIG_TYPE_GET_OFFLINE_AO_PLAY_PARAM = 0x5145, //!> 获取脱机语音播报类型
	ICE_CAMERA_CONFIG_TYPE_GET_OFFLINE_AO_PLAY_PARAM_RSP = 0x5146,//!>获取脱机语音播报类型应答

	ICE_CAMERA_CONFIG_TYPE_SET_LED_PARAM		= 0x5147,		//!< 设置LED参数
	ICE_CAMERA_CONFIG_TYPE_SET_LED_PARAM_RSP	= 0x5148,		//!< 设置LED参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_LED_PARAM		= 0x5149,		//!< 获取LED参数
	ICE_CAMERA_CONFIG_TYPE_GET_LED_PARAM_RSP	= 0x514A,		//!< 获取LED参数应答

	ICE_CAMERA_CONFIG_TYPE_SET_CGI_PARAM		= 0x514B,		//!< 设置CGI参数
	ICE_CAMERA_CONFIG_TYPE_SET_CGI_PARAM_RSP	= 0x514C,		//!< 设置CGI参数应答
	ICE_CAMERA_CONFIG_TYPE_GET_CGI_PARAM		= 0x514D,		//!< 获取CGI参数
	ICE_CAMERA_CONFIG_TYPE_GET_CGI_PARAM_RSP	= 0x514E,		//!< 获取CGI参数应答
	
	ICE_CAMERA_CONFIG_TYPE_END                      = 0x514F,       

} ICE_CAMERA_CONFIG_TYPE_E;

//!配置脱机语音播报类型 
typedef struct
{
        ICE_U32 u32Mode;            //0 不播报，1，只播报欢迎光临/一路顺风   2，播报欢迎光临/一路顺风 + 车牌
        ICE_U32 u32Reserved[16];
                
} OfflineAoParam;


//! 配置数据缓存
typedef union
{
	ICE_U8 au8Buffer[ICE_BUF_SIZE_CONFIG];		//!< 配置数据缓存
} ICE_CONFIG_DATA_INTERNAL_U;

//! \ingroup group_communication
//! 配置数据头和数据缓存
typedef struct  
{
	ICE_OPER_DATA_S stHead;					//!< 操作头
	ICE_CONFIG_DATA_INTERNAL_U unData;		//!< 配置数据
} ICE_CONFIG_DATA_S;

///////////////////////////////////////////////////////////////////////////////////////////////////
//! \ingroup group_communication
//! 编码视频流数据类型
typedef enum
{
	ICE_VIDEO_STREAM_TYPE_BASE 						= 0x6000,		//!< 编码视频流数据类型基础字段
	ICE_VIDEO_STREAM_TYPE_VENC_H264					= 0x6001,		//!< H264编码
	ICE_VIDEO_STREAM_TYPE_VENC_MJPEG				= 0x6002,		//!< MJPEG编码
	ICE_VIDEO_STREAM_TYPE_VENC_MPEG2				= 0x6004,		//!< MPEG2编码
	ICE_VIDEO_STREAM_TYPE_VENC_MPEG4				= 0x6008,		//!< MPEG4编码
	ICE_VIDEO_STREAM_TYPE_END						= 0x6009,		
}ICE_VIDEO_STREAM_E;

typedef struct {
	int mac_list_size;  //  list数量
	char phone_mac_list[100][18];
	char reserved[50*18];
} ICE_Phone_Mac_List;

#ifdef __cplusplus
}
#endif


#endif // __ICE_COM_H__



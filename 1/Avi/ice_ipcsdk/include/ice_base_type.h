/*! \file ice_base_type.h
\brief ice_base_type.h document.
*/

#ifndef __ICE_BASE_TYPE_H__
#define __ICE_BASE_TYPE_H__

#if !defined _CRT_SECURE_NO_DEPRECATE && _MSC_VER > 1300
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>


//! \defgroup group_core 基础设施
//!  基础设施数据类型与函数
//! \{
//! \}


#ifdef __cplusplus
extern "C"{
#endif


//----------------------------------------------
// EXPORTS and IMPORTS definition
//----------------------------------------------
#if (defined WIN32 || defined _WIN32 || defined WINCE) && defined ICE_API_EXPORTS
    #define ICE_EXPORTS __declspec(dllexport)
#else
    #define ICE_EXPORTS
#endif


//----------------------------------------------
// The common data type, will be used in the whole project.
//----------------------------------------------

typedef unsigned char           ICE_U8;
typedef unsigned short          ICE_U16;
typedef unsigned int            ICE_U32;

typedef char                    ICE_S8;
typedef short                   ICE_S16;
typedef int                     ICE_S32;
typedef float 					ICE_FLOAT;

#ifndef _M_IX86
    typedef unsigned long long  ICE_U64;
    typedef long long           ICE_S64;
#else
    typedef __int64             ICE_U64;
    typedef __int64             ICE_S64;
#endif

typedef char                    ICE_CHAR;
typedef void*                   ICE_HANDLE;


//----------------------------------------------
// constant definition
//----------------------------------------------

typedef enum
{
    ICE_FALSE = 0,
    ICE_TRUE  = 1,
} ICE_BOOL;

#ifndef NULL
    #define NULL    0L
#endif

#define ICE_NULL     0L
#define ICE_SUCCESS  0
#define ICE_FAILURE  (-1)


#define ICE_MODULE_DATLEN	1024
//! \ingroup group_core
//! object definition
typedef struct
{
	ICE_U8 auBuffer[ICE_MODULE_DATLEN];				//!< Buffer
} ICE_OBJ;

//! \ingroup group_core
//! status definition
typedef enum
{
	ICE_STATUS_ERROR = 0,                           //!< Error
	ICE_STATUS_OK    = 1,                           //!< Ok
	ICE_STATUS_SKIP  = 2,                           //!< Skip
} ICE_STATUS_E;

//! \ingroup group_core
//! error code definition
typedef enum
{
	ICE_ERR_NULL_PTR       = 0xA00A0001,            //!< null pointer
	ICE_ERR_ILLEGAL_PARAM  = 0xA00A0002,            //!< illegal param
	ICE_ERR_INVALID_ADDR   = 0xA00A0003,            //!< invalid address
	ICE_ERR_NOT_CONFIG     = 0xA00A0004,            //!< not config
	ICE_ERR_NO_LISENCE     = 0xA00A0005,            //!< no valid liceence
	ICE_ERR_INVALID_IMAGE  = 0xA00A0006,            //!< invalid image
} ICE_ERROR_CODE_E;

//! \ingroup group_core
//! system time definition
typedef struct 
{
	ICE_U16 u16Year;								//!< year
	ICE_U16 u16Month;								//!< month
	ICE_U16 u16DayOfWeek;							//!< week
	ICE_U16 u16Day;									//!< day
	ICE_U16 u16Hour;								//!< hour
	ICE_U16 u16Minute;								//!< minute
	ICE_U16 u16Second;								//!< second
	ICE_U16 u16Milliseconds;						//!< milliseconds
}ICE_SYSTEMTIME_S;

//! \ingroup group_core
//! pixel format of the Yuv image 
typedef enum
{
	ICE_YUV_PLANAR_422 = 0x01,                    //!<  YYY UUU VVV	
	ICE_YUV_SEMIPLANAR_422 = 0x02                 //!<  [Y0 Y1 Y2] [U0V0 U1V1 V2V2] 
} ICE_YUV_FORMAT_E;

//! \ingroup group_core
//! point definition
typedef struct
{
	ICE_S16 s16X;               //!< x
	ICE_S16 s16Y;               //!< y
} ICE_POINT_S;

//! \ingroup group_core
//! line definition
typedef struct
{
	ICE_POINT_S stPStart;       //!< start point
	ICE_POINT_S stPEnd;         //!< end point
} ICE_LINE_S;

//! \ingroup group_core
//! rect definition
typedef struct
{
	ICE_S16 s16Left;            //!< letf x
	ICE_S16 s16Top;             //!< top y
	ICE_S16 s16Right;           //!< right x
	ICE_S16 s16Bottom;          //!< bottom y
} ICE_RECT_S;

#define  ICE_MAX_POLYGON_NUM 32
//! \ingroup group_core
//! polygon definition
typedef struct
{
	ICE_S32 s32Num;             //!< polygon point num
	ICE_POINT_S astPoints[ICE_MAX_POLYGON_NUM]; //!< polygon point array
} ICE_POLYGON_S;

//! \ingroup group_core
//! quadrangle definition
typedef struct
{
	ICE_POINT_S astPoints[4];   //!< quadrangle point array
} ICE_QUADRANGLE_S;

//! \ingroup group_core
//! BRG pixel definition
typedef struct
{
	ICE_U8 u8B;                 //!< blue
	ICE_U8 u8G;                 //!< green
	ICE_U8 u8R;                 //!< red
} ICE_PIXEL_S;

//! \ingroup group_core
//! gray image definition
typedef struct
{
	ICE_S32 s32Width;           //!< width of the image
	ICE_S32 s32Height;          //!< height of the image
	ICE_U8 *pu8Data;            //!< data buffer of the image
} ICE_GRAY_IMAGE_S;

//! \ingroup group_core
//! yuv422p image definition
typedef struct
{
	ICE_YUV_FORMAT_E  enPixelFormat;     //!< pixel format of the image()
	ICE_S32 s32Width;					 //!< width of the image
	ICE_S32 s32Height;					 //!< height of the image
	ICE_U8 *pu8YData;					 //!< Y data buffer of the image
	ICE_U8 *pu8UData;					 //!< U data buffer of the image
	ICE_U8 *pu8VData;				     //!< V data buffer of the image
	ICE_U8 *pu8UVData;					 //!< UV data buffer of the image
} ICE_YUV422P_IMAGE_S;

//! \ingroup group_core
//! RBG image definition
typedef struct
{
	ICE_S32 s32Width;           //!< width of the image
	ICE_S32 s32Height;          //!< height of the image
	ICE_U8 *pu8RData;           //!< red data buffer of the image
	ICE_U8 *pu8GData;           //!< green data buffer of the image
	ICE_U8 *pu8BData;           //!< blue data buffer of the image
} ICE_RBG_IMAGE_S;

//! 切面线段
typedef struct
{
	ICE_LINE_S line;            //!< 线段位置(单位：像素)
} ICE_CUT_LINE;

//! 禁止范围
typedef struct
{
	ICE_S32 forbidden;              //!< 禁止标志（0：允许；1：禁止）
	ICE_S32 minimum;                //!< 最小值，取值-1表示不作约束
	ICE_S32 maximum;                //!< 最大值，取值-1表示不作约束
} ICE_FORBIDDEN_INTERVAL;


//----------------------------------------------
// 目标定义
//----------------------------------------------

//! \ingroup group_core
//! 目标类型
typedef enum
{
	ICETGT_TYPE_UNKNOWN			= 0,        //!< 未知
	ICETGT_TYPE_HUMAN			= 1,        //!< 人
	ICETGT_TYPE_VEHICLE			= 2         //!< 车
} ICE_TGT_TYPE;

//! \ingroup group_core
//! 目标轨迹
#define ICE_MAX_TRAJECT_LEN	40
typedef struct
{
	ICE_S32 length;							//!< 轨迹长度
	ICE_POINT_S	points[ICE_MAX_TRAJECT_LEN];//!< 轨迹点数组
} ICE_TGT_TRAJECT;

//! \ingroup group_core
//! 目标数据
typedef struct
{
	ICE_U32 u32ID;                          //!< 目标ID
	ICE_S32 u32Type;                        //!< 目标类型，参考\ref ICE_TGT_TYPE
	ICE_POINT_S stPoint;                    //!< 目标位置
	ICE_RECT_S stRect;                      //!< 目标矩形框
	ICE_S32 s32AreaPix;                     //!< 目标面积(单位：像素)
	ICE_S32 s32Area;                        //!< 目标面积(单位：平方厘米)
	ICE_S32 s32Direct;                      //!< 运动方向(单位：角度)
	ICE_FLOAT fSpeed;                       //!< 运动速度(单位：千米/小时)
	ICE_TGT_TRAJECT stTraject;				//!< 目标轨迹
} ICE_TARGET;

#define ICE_MAX_TGT_NUM 64                  //!< 目标最大数目
//! \ingroup group_core
//! 目标集合
typedef struct
{
	ICE_S32 s32TargetNum;                   //!< 有效目标数目
	ICE_TARGET astTargets[ICE_MAX_TGT_NUM]; //!< 目标数组
} ICE_TARGET_SET;


//----------------------------------------------
// 事件定义
//----------------------------------------------

//! \ingroup group_core
//! 事件类型
typedef enum
{
	ICEEVT_TYPE_UNKNOWN             = 0000, //!< 未知类型

	ICEEVT_TYPE_PERIMETER           = 1000, //!< 周界入侵
	ICEEVT_TYPE_LOITER              = 1001,	//!< 徘徊
	ICEEVT_TYPE_ABNORMALVEL         = 1002,	//!< 非正常速度
	ICEEVT_TYPE_CONVERSE            = 1003,	//!< 逆行
	ICEEVT_TYPE_TRIPWIRE            = 1004,	//!< 绊线
	ICEEVT_TYPE_MTRIPWIRE           = 1005,	//!< 双绊线
	ICEEVT_TYPE_OBJLEFT             = 1006,	//!< 物品遗留
	ICEEVT_TYPE_OBJREMOVED          = 1007,	//!< 物品移走
	ICEEVT_TYPE_MLOITER             = 1008, //!< 多人徘徊
	ICEEVT_TYPE_NOPARKING           = 1009, //!< 非法停车
	ICEEVT_TYPE_VEHICLE_CONVERSE    = 1101, //!< 车辆逆行
	ICEEVT_TYPE_VEHICLE_PARK        = 1102, //!< 车辆停车
	ICEEVT_TYPE_VEHICLE_OVERSPEED   = 1103, //!< 车辆超速
	ICEEVT_TYPE_VEHICLE_UNDERSPEED  = 1104, //!< 车辆低速
	ICEEVT_TYPE_HUMAN_ACROSS        = 1105, //!< 行人穿越
	ICEEVT_TYPE_OBJECT_LEFT         = 1106, //!< 物品遗撒
	ICEEVT_TYPE_VEHICLE_OCCUPATION  = 1107, //!< 占用应急车道

	ICEEVT_TYPE_TGTCOUNT            = 1201,	//!< 目标计数
	ICEEVT_TYPE_HUMANCOUNT          = 1202,	//!< 人员计数

	ICEEVT_TYPE_SIGNAL_BAD  		= 2001  //!< 视频信号异常

} ICE_EVT_TYPE;

//! \ingroup group_core
//! 事件状态
typedef enum
{
	ICEEVT_STATUS_NOSTA	= 0,                //!< 无状态
	ICEEVT_STATUS_START	= 1,                //!< 开始
	ICEEVT_STATUS_END	= 2                 //!< 结束
} ICE_EVT_STATUS;

//! \ingroup group_core
//! 事件数据
typedef struct
{
	ICE_U32 u32Type;                        //!< 事件类型，参考\ref ICE_EVT_TYPE
	ICE_U32 u32ID;                          //!< 事件标识
	ICE_U32 u32Status;                      //!< 事件状态，参考\ref ICE_EVT_STATUS
	ICE_U32 u32Zone;                        //!< 事件发生区域
	ICE_U32 u32TgtID;                       //!< 目标标识
	ICE_U8  u8Data[16];                     //!< 事件私有数据
} ICE_EVENT;

#define ICE_MAX_EVT_NUM	128                  //!< 事件最大数目
//! \ingroup group_core
//! 事件集合
typedef struct
{
	ICE_S32	s32EventNum;                    //!< 有效事件数目
	ICE_EVENT astEvents[ICE_MAX_EVT_NUM];   //!< 事件数组
} ICE_EVENT_SET;

//! \ingroup group_core
//! 算法处理结果
typedef struct
{
	ICE_TARGET_SET stTargetSet;             //!< 目标结果数据
	ICE_EVENT_SET stEventSet;               //!< 事件结果数据
} ICE_ALGO_RESULT;

//! \ingroup group_core
//! 警报级别
typedef enum
{
	ICEALARM_LEVEL_ALARM      = 0,      //!< 报警
	ICEALARM_LEVEL_PREALARM   = 1       //!< 预警
} ICE_ALARM_LEVEL;


#define ICE_VERSION_A( type1, type2, seri_m1, seri_m2, seri_s ) \
	( 0	\
	| ( (((ICE_U32)type1)		& 0xFF) << 24 ) \
	| ( (((ICE_U32)type2)		& 0xFF) << 16 ) \
	| ( (((ICE_U32)seri_m1)		& 0x0F) << 12 ) \
	| ( (((ICE_U32)seri_m2)		& 0x1F) << 7 ) \
	| ( (((ICE_U32)seri_s)		& 0x7F) << 0 ) \
	)

#define ICE_VER_A_TYPE1( ver )		( (((ICE_U32)ver)>>24)	& 0xFF )
#define ICE_VER_A_TYPE2( ver )		( (((ICE_U32)ver)>>16)	& 0xFF )
#define ICE_VER_A_SER_M1( ver )		( (((ICE_U32)ver)>>12)	& 0x0F )
#define ICE_VER_A_SER_M2( ver )		( (((ICE_U32)ver)>>7)	& 0x1F )
#define ICE_VER_A_SER_S( ver )		( (((ICE_U32)ver)>>0)	& 0x7F )

//! \brief 格式化版本字符串
//! \param nSeriNum     [in] 版本序列号
//! \param pVersion     [out] 版本字符串，字符串缓冲区长度至少是32
//! \return             操作状态，参考\ref ICE_STATUS_E
ICE_EXPORTS void ICECORE_FormatVersion( ICE_U32 nSeriNum, char *pVerion );


#ifdef __cplusplus
}
#endif


#endif // __ICE_TYPE_H__


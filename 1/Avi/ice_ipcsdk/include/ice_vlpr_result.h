/*! \file ice_vlpr_result.h
\brief ice_vlpr_result.h document.
*/
#ifndef __ICE_VLPR_RESULT_H__
#define __ICE_VLPR_RESULT_H__

#include "ice_base_type.h"

#ifdef __cplusplus
extern "C"
{
#endif



//-------------------------------------------
// 字符类型定义              		 
//-------------------------------------------

//! 字符类型定义
typedef enum 
{
	ICE_CHARTYPE_CHARACTER			= 0,	//!< 数字及字母
	ICE_CHARTYPE_ONLYDIGITAL		= 1,	//!< 数字
	ICE_CHARTYPE_ONLYALPHABET		= 2,	//!< 字母 

	ICE_HANZI_CHINA					= 3,	//!< 全国
	ICE_HANZI_BEIJING				= 4,	//!< 北京
	ICE_HANZI_SHANGHAI				= 5,	//!< 上海
	ICE_HANZI_TIANJIN				= 6,	//!< 天津
	ICE_HANZI_CHONGQING				= 7,	//!< 重庆
	ICE_HANZI_HEILONGJIANG			= 8,	//!< 黑龙江
	ICE_HANZI_JILIN					= 9,	//!< 吉林
	ICE_HANZI_LIAONING				= 10,	//!< 辽宁
	ICE_HANZI_NEIMENG				= 11,	//!< 内蒙古
	ICE_HANZI_HEBEI					= 12,	//!< 河北
	ICE_HANZI_SHANDONG				= 13,	//!< 山东
	ICE_HANZI_SHANXI				= 14,	//!< 山西
	ICE_HANZI_ANHUI					= 15,	//!< 安徽
	ICE_HANZI_JIANGSU				= 16,	//!< 江苏
	ICE_HANZI_ZHEJIANG				= 17,	//!< 浙江
	ICE_HANZI_FUJIAN				= 18,	//!< 福建
	ICE_HANZI_GUANGDONG				= 19,	//!< 广东
	ICE_HANZI_HENAN					= 20,	//!< 河南
	ICE_HANZI_JIANGXI				= 21,	//!< 江西
	ICE_HANZI_HUNAN					= 22,	//!< 湖南
	ICE_HANZI_HUBEI					= 23,	//!< 湖北
	ICE_HANZI_GUANGXI				= 24,	//!< 广西
	ICE_HANZI_HAINAN				= 25,	//!< 海南
	ICE_HANZI_YUNNAN				= 26,	//!< 云南
	ICE_HANZI_GUIZHOU				= 27,	//!< 贵州
	ICE_HANZI_SICHUAN				= 28,	//!< 四川
	ICE_HANZI_XIAN					= 29,	//!< 陕西
	ICE_HANZI_GANSU					= 30,	//!< 甘肃
	ICE_HANZI_NINGXIA				= 31,	//!< 宁夏
	ICE_HANZI_QINGHAI				= 32,	//!< 青海
	ICE_HANZI_XIZANG				= 33,	//!< 西藏
	ICE_HANZI_XINJIANG				= 34	//!< 新疆
}ICE_VLPR_CHARTYPE_E;	

//-------------------------------------------------------
// 车牌类型定义      									 
//-------------------------------------------------------

//! 车牌类型定义
typedef enum 
{
	ICE_PLATE_UNCERTAIN			= 0,			//!< 不确定的
	ICE_PLATE_BLUE				= 1,			//!< 蓝牌车
	ICE_PLATE_YELLOW			= 2,			//!< 单层黄牌车
	ICE_PLATE_POLICE			= 4,			//!< 警车
	ICE_PLATE_WUJING			= 8,			//!< 武警车辆
	ICE_PLATE_DBYELLOW			= 16,			//!< 双层黄牌
	ICE_PLATE_MOTOR				= 32,			//!< 摩托车
	ICE_PLATE_INSTRUCTIONCAR	= 64,			//!< 教练车
	ICE_PLATE_MILITARY			= 128,			//!< 军车
	ICE_PLATE_PERSONAL			= 256,			//!< 个性化车
	ICE_PLATE_GANGAO			= 512,			//!< 港澳车
	ICE_PLATE_EMBASSY			= 1024,			//!< 使馆车
	ICE_PLATE_NONGLARE			= 2048,			//!< 老式车牌(不反光)
	ICE_PLATE_AVIATION			= 4096,			//!< 民航车牌

	ICE_PLATE_WHITE_TWOTWO		= 0x10000001,	//!< 2+2模型；
	ICE_PLATE_WHITE_TWOTHR		= 0x10000002,	//!< 2+3模型；
	ICE_PLATE_WHITE_THRTWO		= 0x10000004,	//!< 3+2模型；
	ICE_PLATE_WHITE_THRTHR		= 0x10000008,	//!< 3+3模型；
	ICE_PLATE_WHITE_THRFOR		= 0x10000010,	//!< 3+4模型；

	ICE_PLATE_BLACK_THRTWO		= 0x10000020,	//!< 3+2模型；
	ICE_PLATE_BLACK_TWOTHR		= 0x10000040,	//!< 2+3模型；
	ICE_PLATE_BLACK_THRTHR		= 0x10000080,	//!< 3+3模型；
	ICE_PLATE_BLACK_TWOFOR		= 0x10000100,	//!< 2+4模型；
	ICE_PLATE_BLACK_FORTWO		= 0x10000200,	//!< 4+2模型；
	ICE_PLATE_BLACK_THRFOR		= 0x10000400,	//!< 3+4模型；
}ICE_PLATETYPE_E;

//-------------------------------------------------------
// 车牌颜色定义
//-------------------------------------------------------

// 车牌颜色
typedef enum
{
	ICE_PLATE_COLOR_UNKNOWN		= 0,	//!<  未知
	ICE_PLATE_COLOR_BLUE		= 1,	//!<  蓝底白字
	ICE_PLATE_COLOR_YELLOW		= 2,	//!<  黄底黑字
	ICE_PLATE_COLOR_WHITE		= 3,	//!<  白底黑字
	ICE_PLATE_COLOR_BLACK		= 4,	//!<  黑底白字
	ICE_PLATE_COLOR_GREEN		= 5		//!<  绿底白字
}ICE_PLATECOLOR_E;

typedef enum
{
	ICE_VEHICLE_COLOR_UNKOWN= 0,		//!<  未知
	ICE_VEHICLE_COLOR_RED = 1,			//!<  红色
	ICE_VEHICLE_COLOR_GREEN = 2,		//!<  绿色
	ICE_VEHICLE_COLOR_BLUE = 3,			//!<  蓝色
	ICE_VEHICLE_COLOR_YELLOW = 4,		//!<  黄色
	ICE_VEHICLE_COLOR_WHITE = 5,		//!<  白色
	ICE_VEHICLE_COLOR_GRAY = 6,			//!<  灰色
	ICE_VEHICLE_COLOR_BLACK = 7,		//!<  黑色
	ICE_VEHICLE_COLOR_PURPLE=8,			//!<  紫色
	ICE_VEHICLE_COLOR_BROWN=9,			//!<  棕色
	ICE_VEHICLE_COLOR_PINK=10			//!<  粉色
}ICE_VEHICLECOLOR_E;

//----------------------------------------------
// 车牌识别输出结构定义
//----------------------------------------------

#define ICE_VLPR_PLATE_CHAR_NUM		7								//!<  车牌字符数    
#define ICE_VLPR_PLATE_BUFLEN		16                              //!<  车牌号最大值  

//! \ingroup group_vlpr
//! 车牌识别输出结构
typedef struct 
{
	ICE_S8				as8PlateNum[ICE_VLPR_PLATE_BUFLEN];			//!<  车牌号
	ICE_RECT_S			stPlateRect;								//!<  车牌矩形框；
	ICE_RECT_S			astPlateCharRect[ICE_VLPR_PLATE_CHAR_NUM];	//!<  车牌字符矩形框
	ICE_FLOAT			aflPlateCharConfid[ICE_VLPR_PLATE_CHAR_NUM];//!<  车牌字符置信度
	ICE_FLOAT			flConfidence;								//!<  车牌置信度
	ICE_S32				s32PlateIntensity;							//!<  车牌亮度
	ICE_PLATECOLOR_E	ePlateColor;								//!<  车牌颜色
	ICE_PLATETYPE_E		ePlateType;									//!<  车牌类型
	ICE_VEHICLECOLOR_E	eVehicleColor;								//!<  车身颜色
	ICE_FLOAT			flPlateAngleH;								//!<  车牌水平倾斜角度
	ICE_FLOAT			flPlateAngleV;								//!<  车牌竖直倾斜角度
	ICE_S8				astReserve[20];								//!<  预留参数
	ICE_U32             u32FrameId;									//!<  时间戳ID
}ICE_VLPR_OUTPUT_S;



//----------------------------------------------
// 车牌识别处理结果定义
//----------------------------------------------

#define ICE_VLPR_MAX_OUTPUT_NUM		16							//!< 最大车牌数  

//! \ingroup group_vlpr
//! 车牌识别算法处理结果
typedef struct 
{
	ICE_S32				s32NumOutput;							//!< 输出结果个数   
	ICE_VLPR_OUTPUT_S	astOutput[ICE_VLPR_MAX_OUTPUT_NUM];		//!< 多个输出结果
}ICE_VLPR_RULT_S;


#ifdef __cplusplus
}
#endif


#endif

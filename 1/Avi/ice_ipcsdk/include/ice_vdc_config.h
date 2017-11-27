/*! \file ice_vdc_config.h
\brief ice_vdc_config.h document.
*/

#ifndef __ICE_VDC_CONFIG_H__
#define __ICE_VDC_CONFIG_H__

#include "ice_base_type.h"

#ifdef __cplusplus
extern "C"
{
#endif

//! \ingroup group_vd
//! 出入口车道高级参数定义
typedef struct 
{
	ICE_S32 s32AdaptiveThreshOnOff;					//!<  自适应阈值开关  (1:开 0:关  默认:关)								
	ICE_S32 s32DayTriggerThresh;					//!<  白天触发阈值    (范围:1-100 默认:25)													
	ICE_S32 s32NightTriggerThresh;					//!<  夜间触发阈值    (范围:1-100 默认:50)													
	ICE_S32 s32TriggerSensitivity;					//!<  触发灵敏度      (范围:1-100 默认:10)													
	ICE_S32 s32FilterByPlate;						//!<  无车牌过滤开关  (1:开 0:关  默认:开)														
	ICE_S32 s32TrigerInterval;						//!<  无牌车触发间隔  (范围:10-500  默认:50)
	ICE_S32 s32DetectDoubleDirectOnOff;				//!<  检测双向车辆开关(2:支持双向车辆并输出方向信息 1:支持双向车辆但不输出方向信息 0:只输出单向车辆  默认:1)
	ICE_S32 s32EnableSamePlateCar;					//!<  允许相同车辆开关(1:开 0:关  默认:关)
	ICE_S32 s32SamePlateCarOutputTime;				//!<  相同车辆输出间隔(范围:1-3600 默认:30 单位:秒)
	ICE_S32 s32EnableNonMotorizedVehicles;			//!<  开启非机动车检测开关(1:开 0:关  默认:关)
	ICE_S32 s32FilterNonMotorizedVehicles;			//!<  是否过滤非机动车辆(1:开 0:关  默认:关)
	ICE_S32 u32Reserved[10];                        //!<  保留值     																					
}ICE_VDC_ADV_PARAM_S;

//! \ingroup group_vdc
//! 出入口方向定义
typedef enum 
{
	ICE_VDC_VERHICLE_DIR_HEAD			 = 0,				//!<  车头方向 			
	ICE_VDC_VERHICLE_DIR_TAIL			 = 1,				//!<  车尾方向 			
	ICE_VDC_VERHICLE_DIR_HEAD_AND_TAIL	 = 2				//!<  车头和车尾方向 	
}ICE_VDC_VERHICLE_DIR_E;

//! \ingroup group_vdc
//! 触发方式定义
typedef enum 
{
	ICE_VDC_TRIGER_MODE_HDWARE				= 0,				//!<  外触发（开关量输入） 			
	ICE_VDC_TRIGER_MODE_VIDEO				= 1,				//!<  视频触发 			
	ICE_VDC_TRIGER_MODE_HDWARE_AND_VIDEO	= 2					//!<  外触发和视频混合触发 	
}ICE_VDC_TRIGER_MODE_E;

//! \ingroup group_vdc
//! 安装位置定义
typedef enum 
{
	ICE_VDC_INSTALL_POS_DRIVER_SIDE					= 0,				//!<  司机同侧（推荐）		
	ICE_VDC_INSTALL_POS_DRIVER_REVERSE_SIDE			= 1					//!<  司机异侧 			
}ICE_VDC_INSTALL_POS_E;

//! \ingroup group_vd
//! 车道参数定义
typedef struct 
{																						
	ICE_VDC_VERHICLE_DIR_E enVehiceleDirection;				//!<  车辆方向(默认:车头)											
	ICE_RECT_S astLoop;				 						//!<  虚拟线圈(识别区域)																					
	ICE_VDC_ADV_PARAM_S stAdvParam;							//!<  高级参数
	ICE_VDC_TRIGER_MODE_E enTrigerMode;						//!<  触发模式
	ICE_S32 u32Reserved[15];                        		//!<  保留     																					
}ICE_VDC_LANE_S;

//! \ingroup group_vd
//! 车牌参数定义
typedef struct 
{
	ICE_S32	s32MinPlateWidth;						//!<  最小车牌宽度 									   
	ICE_S32	s32MaxPlateWidth;						//!<  最大车牌宽度 									 
	ICE_CHAR cPriorCity[32];					 	//!<  优先城市 										 
	ICE_S32	s32PlateConfidThrld;					//!<  车牌可信度阈值（-28-28），默认：15 			 
	ICE_FLOAT fPlateAngle;							//!<  车牌倾斜角度，默认：0 
	ICE_S32 s32PlateTypeSp;							//!<  支持车牌类型     0默认全部开启；
	ICE_S32	s32CarColorOn;							//!<  车身颜色额启用； 0不启用
	ICE_S32 s32PriorCityType;						//!<  优先城市类型
	ICE_S32 s32PriorArmyPlate;						//!<  军牌优先
	ICE_VDC_INSTALL_POS_E enCameraInstallPos;		//!<  相机安装位置（默认：司机同侧）
	ICE_S32 s32EnableOnePlateSameChars;				//!<  支持1车牌重复字符（例如:京N11111） 0不启用 1启动 ，默认：0（默认：0）
	ICE_S32 s32EnableOneCarMultiPlates;				//!<  支持1车多牌   0不启用 1启动 ，默认：0
	ICE_S32 s32EnableWJChineseChar;					//!<  支持武警汉字输出   0不启用 1启动 ，默认：0
	ICE_S32 u32Reserved[7];                         //!<  保留     									
}ICE_VDC_PLATE_CONFIG_S;

//! \ingroup group_vd
//! 出入口参数定义
typedef struct  
{
	ICE_VDC_LANE_S astLane;				 						 //!<  车道参数 		  						  
	ICE_VDC_PLATE_CONFIG_S stPlateParam;						 //!<  车牌参数   		  						  	  
	ICE_S32 s32Width;											 //!<  配置所在参考图像宽度		  						  	  
	ICE_S32 s32Height;											 //!<  配置所在参考图像高度
	ICE_S32 as32Reserved[8];              						 //!<  保留 											  
}ICE_VDC_PARAM_S;

#ifdef __cplusplus
}
#endif


#endif// __ICE_VDC_CONFIG_H__

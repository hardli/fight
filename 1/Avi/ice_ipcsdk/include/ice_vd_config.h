/*! \file ice_vd_config.h
\brief ice_vd_config.h document.
*/

#ifndef __ICE_VD_CONFIG_H__
#define __ICE_VD_CONFIG_H__

#include "ice_base_type.h"

#ifdef __cplusplus
extern "C"
{
#endif

//! \ingroup group_vd
//! 车辆检测事件类型定义
typedef enum 
{
	ICE_VD_LANE_EVENT_TYPE_NOON 						= 0x0000,				//!<  无										
	ICE_VD_LANE_EVENT_TYPE_COMMON_PASS 					= 0x0001,				//!<  普通卡口			
	ICE_VD_LANE_EVENT_TYPE_TRUCK_FORBIDDEN  			= 0x0002,				//!<  大货车禁行 								
	ICE_VD_LANE_EVENT_TYPE_BUS_LANE_FORBIDDEN 			= 0x0004,				//!<  非法闯公交车道 							
	ICE_VD_LANE_EVENT_TYPE_EMERGENCY_LANE_FORBIDDEN 	= 0x0008,				//!<  非法占用应急车道 							
	ICE_VD_LANE_EVENT_TYPE_NORMOTOR_FORBIDDEN 			= 0x0010,				//!<  非法占用非机动车道						
	ICE_VD_LANE_EVENT_TYPE_CROSS_LINE_FORBIDDEN 		= 0x0020,				//!<  非法压线								
	ICE_VD_LANE_EVENT_TYPE_CHANGE_LANE_FORBIDDEN 		= 0x0040,				//!<  非法变换车道							
	ICE_VD_LANE_EVENT_TYPE_REVERSE_TRAFFIC 				= 0x0080,				//!<  逆向行驶								
	ICE_VD_LANE_EVENT_TYPE_OVER_SPEED 					= 0x0100				//!<  超速行驶									
}ICE_VD_LANE_EVENT_TYPE_E;

//! \ingroup group_vd
//! 车辆检测车道高级参数定义
typedef struct 
{
	ICE_S32 s32AdaptiveThreshOnOff;					//!<  自适应阈值开关  (1:开 0:关  默认:关)								
	ICE_S32 s32DayTriggerThresh;					//!<  白天触发阈值    (范围:1-100 默认:25)													
	ICE_S32 s32NightTriggerThresh;					//!<  夜间触发阈值    (范围:1-100 默认:50)													
	ICE_S32 s32TriggerSensitivity;					//!<  触发灵敏度      (范围:1-100 默认:10)													
	ICE_S32 s32FilterByPlate;						//!<  无车牌过滤开关  (1:开 0:关  默认:关)														
	ICE_S32 s32TrigerInterval;						//!<  无牌车触发间隔  (范围:10-500  默认:50)
	ICE_S32 s32DetectDoubleDirectOnOff;				//!<  检测双向车辆开关(2:支持双向车辆并输出方向信息 1:支持双向车辆但不输出方向信息 0:只输出单向车辆  默认:1)
	ICE_S32 s32EnableSamePlateCar;					//!<  允许相同车辆开关(1:开 0:关  默认:关)
	ICE_S32 s32SamePlateCarOutputTime;				//!<  相同车辆输出间隔(范围:1-3600 默认:30 单位:秒)
	ICE_S32 s32EnableNonMotorizedVehicles;			//!<  开启非机动车检测开关(1:开 0:关  默认:关)
	ICE_S32 s32FilterNonMotorizedVehicles;			//!<  是否过滤非机动车辆(1:开 0:关  默认:关)
	ICE_S32 u32Reserved[10];                        //!<  保留值     																					
}ICE_VD_LANE_ADV_PARAM_S;

//! \ingroup group_vd
//! 车辆检测方向定义
typedef enum 
{
	ICE_VD_VERHICLE_DIR_HEAD			 = 0,				//!<  车头方向 			
	ICE_VD_VERHICLE_DIR_TAIL			 = 1,				//!<  车尾方向 			
	ICE_VD_VERHICLE_DIR_HEAD_AND_TAIL	 = 2				//!<  车头和车尾方向 	
}ICE_VD_VERHICLE_DIR_E;

//! \ingroup group_vd
//! 速度检测方法定义
typedef enum 
{
	ICE_VD_MEASURE_SPEED_METHOD_VIDEO 		= 0,				//!<  视频测速			
	ICE_VD_MEASURE_SPEED_METHOD_RADAR 		= 1,				//!<  雷达测速 			
	ICE_VD_MEASURE_SPEED_METHOD_INDUC_LOOP = 2					//!<  地感线圈测速	
}ICE_VD_MEASURE_SPEED_METHOD_E;


//! \ingroup group_vd
//! 雷达类型定义
typedef enum 
{
	ICE_VD_RADAR_TYPE_NOON 				= 0,				//!<  无						
	ICE_VD_RADAR_TYPE_ADL 					= 1,				//!<  安道雷雷达 			
	ICE_VD_RADAR_TYPE_CS 					= 2,				//!<  川速雷达 				
	ICE_VD_RADAR_TYPE_MTY 					= 3					//!<  猫头鹰 				
}ICE_VD_RADAR_TYPE_E;

//! \ingroup group_vd
//! 雷达参数定义
typedef struct 
{
	ICE_VD_RADAR_TYPE_E enRadarType;				 	//!<  雷达类型 									
	ICE_S32 s32ComId;								//!<  串口 id（1-10） 	
	ICE_S32 s32Baud;								//!<  波特率
	ICE_S32 s32RadarSensitivity;					//!<  雷达灵敏度参数								
	ICE_S32 u32Reserved[15];                        //!<  保留     										
}ICE_VD_RADAR_PARAM_S;


//! \ingroup group_vd
//! IO状态定义
typedef enum 
{
	ICE_VD_IO_STATUS_TYPE_FAILING_EDGE 						= 0,						//!<  下降沿有效 			
	ICE_VD_IO_STATUS_TYPE_RISING_EDGE 						= 1,						//!<  上升沿有效 			
	ICE_VD_IO_STATUS_TYPE_RISING_EDGE_AND_FAILING_EDGE		= 2, 						//!<  上升沿或下降沿有效		
	ICE_VD_IO_STATUS_TYPE_HIGH_LEVEL 						= 3,						//!<  高电平有效 			
	ICE_VD_IO_STATUS_TYPE_LOW_LEVEL 						= 4							//!<  低电平有效 			
}ICE_VD_IO_STATUS_TYPE_E;

//! \ingroup group_vd
//! IO线圈组参数定义
 typedef struct 
{
	ICE_S32 s32OnOff;				 				//!<  是否启用标志  	  默认：1						
	ICE_S32 s32IoLoop1Id;				 			//!<  IO线圈1的id （1-8） 默认：1						
	ICE_VD_IO_STATUS_TYPE_E enIoLoop1InStatus;		//!<  IO线圈1输入状态     默认：0						
	ICE_S32 s32IoLoop2Id;				 			//!<  IO线圈2的id （1-8） 默认：2						
	ICE_VD_IO_STATUS_TYPE_E enIoLoop2InStatus;		//!<  IO线圈2输入状态     默认：0						
	ICE_S32 s32LoopDistance;						//!<  线圈实际距离（单位：里面）        默认：600cm		
	ICE_S32 u32Reserved[16];                        //!<  保留     										
}ICE_VD_IO_LOOP_GROUP_PARAM_S;

 //! \ingroup group_vd
 //! 单IO线圈参数定义
 typedef struct 
{
	ICE_S32 s32OnOff;				 				//!<  是否启用标志  		 默认：1						
	ICE_S32 s32IoLoopId;				 			//!<  IO线圈的id （1-4）  默认：1						
	ICE_VD_IO_STATUS_TYPE_E enIoLoopInStatus;		//!<  IO线圈输入状态      默认：0						
	ICE_S32 u32Reserved[16];                        //!<  保留     										
}ICE_VD_IO_LOOP_SINGLE_PARAM_S;

#define ICE_MAX_IO_LOOP_GROUP_NUM 4

 //! \ingroup group_vd
 //! IO线圈组集合参数定义
typedef struct 
{
	ICE_VD_IO_LOOP_GROUP_PARAM_S stLoopGroupParam[ICE_MAX_IO_LOOP_GROUP_NUM];		//!<  IO线圈组配置参数 ,用于卡口+测速使用    				
	ICE_S32 u32Reserved[16];                        							//!<  保留     											
}ICE_VD_IO_LOOP_G_PARAM_S;

#define ICE_MAX_IO_LOOP_SINGLE_NUM 4

//! \ingroup group_vd
//! 单IO线圈集合参数定义
typedef struct 
{
	ICE_VD_IO_LOOP_SINGLE_PARAM_S stLoopSingleParam[ICE_MAX_IO_LOOP_SINGLE_NUM];	//!<  IO单线圈配置参数 ,用于卡口使用    						
	ICE_S32 u32Reserved[16];                        								//!<  保留     											
}ICE_VD_IO_LOOP_S_PARAM_S;

//! \ingroup group_vd
//! 硬件车检器参数定义
typedef struct 
{
	ICE_S32 u32Reserved[16];                        //!<  保留     										
}ICE_VD_LOOP_VD_PARAM_S;

//! \ingroup group_vd
//! 硬件设备参数定义
typedef union 
{
	ICE_VD_RADAR_PARAM_S stRadarParam;					//!<  雷达参数 
	ICE_VD_IO_LOOP_S_PARAM_S stIoLoopSingleParam;		//!<  单IO线圈 
	ICE_VD_IO_LOOP_G_PARAM_S stIoLoopGroupParam;		//!<  组IO线圈 
	ICE_VD_LOOP_VD_PARAM_S stLoopVdParam;				//!<  车检器 
	ICE_S32 u32Reserved[16];							//!<  保留     										
}ICE_VD_HARDWARE_DEVICE_PARAM_S;

//! \ingroup group_vd
//! 速度参数定义
typedef struct 
{
	ICE_S32 s32DetectSpeedOnOff;				 		//!<  速度检测开关 (1:开 0:关  默认:0) 			
	ICE_VD_MEASURE_SPEED_METHOD_E enMeasureSpdMethod;	//!<  速度检测方法																 			
	ICE_S32 s32LimitSpeedValue;				 			//!<  限速值(单位:千米/小时)   		  														
	ICE_S32 s32SignLimitSpeedValue;				 		//!<  参考限速值(单位:千米/小时)  										
	ICE_FLOAT fLoopRealDist;				 			//!<  线圈实际距离(米)   		  																
	ICE_FLOAT fSpeedAdjustCoeff;				 		//!<  速度调整系数(范围:0.1-10)   		  															
	ICE_S32 u32Reserved[8];								//!<  保留     																					
}ICE_VD_SPEED_PARAM_S;

//! \ingroup group_vd
//! 压线参数定义
typedef struct
{
	ICE_S32 s32DetectCrossLineOnOff;				 //!<  压线检测开关 (1:开 0:关  默认:0) 	
	ICE_LINE_S stCrossLine;							 //!<  线的位置信息 																					
	ICE_S32 s32MinDist;								 //!<  压线最大距离限制					
	ICE_S32 u32Reserved[4];							 //!<  保留     																					
}ICE_VD_CROSS_LINE_PARAM_S;

//! \ingroup group_vd
//! 时间限制参数定义
typedef struct 
{
	ICE_S32 s32LimitTimeOnOff;				 		//!<  时间限制开关 (1:开 0:关  默认:0) 
	ICE_S32 s32BeginTime;				 			//!<  开始时间(高8位（预留） 次高8位(时) 次低8位（分） 低8位（秒）)   							
	ICE_S32 s32EndTime;				 				//!<  结束时间(高8位（预留） 次高8位(时) 次低8位（分） 低8位（秒）)   		 					
	ICE_S32 s32LimitEventCode;						//!<  限制时间的事件码(事件码相或)			
	ICE_S32 u32Reserved[15];						//!<  保留     																					
}ICE_VD_LIMIT_TIME_PARAM_S;

//! \ingroup group_vd
//! 限制参数定义
#define ICE_VD_MAX_LIMIT_TIME_NUM 4
typedef struct 
{
	ICE_S32 s32ValidTimeQuantum;											//!<  有效时间段数量 (0-4) 
	ICE_VD_LIMIT_TIME_PARAM_S astLimitTimeParam[ICE_VD_MAX_LIMIT_TIME_NUM];	//!<  时间限制参数     							
	ICE_S32 u32Reserved[15];                       							//!<  保留     								
}ICE_VD_LIMIT_PARAM_S;

//! \ingroup group_vd
//! 闪光抓拍参数定义
typedef struct 
{
	ICE_S32 s32FlashOnOff;				 			//!<  闪光灯开关 (1:开 0:关  默认:0) 											
	ICE_S32 s32FlashId;				 				//!<  闪光灯 ID  																				
	ICE_S32 s32TrigerFlashLoopId;				    //!<  触发闪光灯的线圈ID(0对应loop1 1对应loop2)  																
	ICE_S32 s32FlashCapWaitTime;				    //!<  闪光抓拍图片最长等待时间 默认:100（单位：毫秒） 	
	ICE_S32 u32Reserved[16];                        //!<  保留     								
}ICE_VD_FLASH_CAP_PARAM_S;

//! \ingroup group_vd
//! 抓拍参数定义
typedef struct 
{
	ICE_S32 s32Loop1CapNum;							//!<  线圈1抓拍张数（0-2）	默认：1									
	ICE_S32 s32Loop2CapNum;							//!<  线圈2抓拍张数（0-2）	默认：0									
	ICE_S32 s32CapInterval;							//!<  间隔帧数 											
	ICE_VD_FLASH_CAP_PARAM_S stFlashCapParam;		//!<  闪光抓怕参数 											
	ICE_S32 u32Reserved[16];                        //!<  保留     													
}ICE_VD_CAP_PARAM_S;

//! \ingroup group_vd
//! 触发类型定义
typedef enum 
{
	ICE_VD_TRIGER_TYPE_VIRTUAL_LOOP   						= 0,					//!<  视频虚拟线圈触发 									
	ICE_VD_TRIGER_TYPE_IO_SINGLE_LOOP   					= 1,					//!<  单地感线圈触发 								
	ICE_VD_TRIGER_TYPE_IO_GROUP_LOOP   						= 2,					//!<  组地感线圈触发 								
	ICE_VD_TRIGER_TYPE_VD_LOOP   							= 3,					//!<  车检器触发 									
	ICE_VD_TRIGER_TYPE_RADAR   								= 4,					//!<  雷达触发 										
	ICE_VD_TRIGER_TYPE_IO_SINGLE_LOOP_AND_VIRTUAL_LOOP   	= 5,					//!<  单地感线圈结合视频虚拟线圈触发，地感优先 			
	ICE_VD_TRIGER_TYPE_IO_GROUP_LOOP_AND_VIRTUAL_LOOP   	= 6,					//!<  组地感线圈结合视频虚拟线圈触发，地感优先 			
	ICE_VD_TRIGER_TYPE_VD_LOOP_AND_VIRTUAL_LOOP   			= 7,					//!<  车检器结合视频虚拟线圈触发，车检优先 				
	ICE_VD_TRIGER_TYPE_RADAR_AND_VIRTUAL_LOOP   			= 8,					//!<  雷达结合视频虚拟线圈触发，雷达优先 				
	ICE_VD_TRIGER_TYPE_IO_VIRTUAL_LOOP_AND_SINGLE_LOOP 		= 9,					//!<  视频虚拟线圈结合单地感线圈触发，视频虚拟线圈优先 		
	ICE_VD_TRIGER_TYPE_IO_VIRTUAL_LOOP_AND_GROUP_LOOP   	= 10,					//!<  视频虚拟线圈结合组地感线圈触发，视频虚拟线圈优先 		
	ICE_VD_TRIGER_TYPE_VIRTUAL_LOOP_AND_VD_LOOP   			= 11,					//!<  视频虚拟线圈结合车检器触发，视频虚拟线圈优先 			
	ICE_VD_TRIGER_TYPE_VIRTUAL_LOOP_AND_RADAR   			= 12					//!<  视频虚拟线圈结合雷达触发，视频虚拟线圈优先 			
}ICE_VD_TRIGER_TYPE_E;

        
#define ICE_MAX_VD_NUM 8											//!< max vehicele num  		
#define ICE_MAX_DETECT_REGION_NUM	4								//!< max detect region num  
#define ICE_MAX_LANE_NUM	     ICE_MAX_DETECT_REGION_NUM			//!< max lane num  	        

//! \ingroup group_vd
//! lane region defination             			
typedef struct 
{
	ICE_S32 s32LaneId;								//!< 车道ID  (1-4)    																				
	ICE_POLYGON_S stPoly;				 			//!< 车牌识别多边形区域  
	ICE_S32 s32OnOff;								//!< 识别区域是否开启
	ICE_S32 u32Reserved[15];                        //!< 保留     																					
}ICE_VD_LANE_REGION_S;

//! \ingroup group_vd
//! 车道参数定义
typedef struct 
{																						
	ICE_VD_TRIGER_TYPE_E enTrigerType;						//!<  触发方式																					
	ICE_VD_VERHICLE_DIR_E enVehiceleDirection;				//!<  车辆方向											
	ICE_VD_LANE_EVENT_TYPE_E enEventCode;					//!<  事件码(支持多事件码按位相或)
	ICE_VD_LANE_REGION_S stLaneRegion;						//!<  车道区域																			
	ICE_RECT_S astLoop[2];				 					//!<  虚拟线圈
	ICE_VD_SPEED_PARAM_S stSpeedParam;						//!<  速度参数
	ICE_VD_CROSS_LINE_PARAM_S stCrossLineParam;				//!<  压线参数
	ICE_VD_LIMIT_PARAM_S stLimitParam;						//!<  限制参数																					
	ICE_VD_CAP_PARAM_S stCapParam;							//!<  抓拍参数																						
	ICE_VD_HARDWARE_DEVICE_PARAM_S stHardWareDeviceParam;	//!<  硬件设备参数																					
	ICE_VD_LANE_ADV_PARAM_S stAdvParam;						//!<  高级参数																					
	ICE_S32 u32Reserved[16];                        		//!<  保留     																					
}ICE_VD_LANE_S;

//! \ingroup group_vd
//! OSD参数定义
typedef struct 
{
	ICE_S32   s32OsdOnOff;							//!<  是否启动前端OSD叠加							
	ICE_CHAR  cDeviceID[64];  						//!<  设备（编号）									
	ICE_CHAR  cDeviceName[32];  					//!<  设备名称										
	ICE_CHAR  cCrossInfo[256];  					//!<  道路信息										
	ICE_CHAR  cDirInfo[32];     					//!<  方向信息										
	ICE_CHAR  cRoadInfo[32];    					//!<  道路信息										
	ICE_CHAR  cRoadNum[32];     					//!<  车道编号										
	ICE_S32  s32SpeedOsdOnOff;						//!<  是否叠加速度信息（限速值和当前车辆速度）		
	ICE_S32  s32VehicleOsdOnOff;					//!<  是否叠加车辆信息（车辆颜色 车辆类型）			
	ICE_S32  s32PlateOsdOnOff;						//!<  是否叠加车牌信息（车牌号码 车牌颜色 车牌类型）	
	ICE_S16  s16PositionX;							//!<  叠加的其实位置	x坐标							
	ICE_S16  s16PositionY;							//!<  叠加的其实位置	y坐标							
	ICE_S32  s32RColor; 							//!<  叠加OSD R颜色(0-255)									
	ICE_S32  s32GColor; 							//!<  叠加OSD G颜色(0-255)									
	ICE_S32  s32BColor; 							//!<  叠加OSD B颜色(0-255)								
	ICE_S32  s32AlphaColor; 						//!<  叠加OSD 透明通道颜色(0-255)								
	ICE_S32 u32Reserved[20];                        //!<  保留     									
}ICE_VD_OSD_CONFIG_S;

//! \ingroup group_vd
//! 交通参数定义
typedef struct 
{
	ICE_S32   s32TrafficOnOff;							//!<  启动交通参数统计开关 (1:开 0:关  默认:0) 						
	ICE_S32   s32TrafficReportInterval;					//!<  数据上报时间间隔（默认：60，单位：秒 0为实时上报）			
	ICE_S32 u32Reserved[16];                        	//!<  保留     									
}ICE_VD_TRAFFIC_CONFIG_S;

//! \ingroup group_vd
//! 车牌参数定义
typedef struct 
{
	ICE_S32	s32MinPlateWidth;						//!<  最小车牌宽度 									   
	ICE_S32	s32MaxPlateWidth;						//!<  最大车牌宽度 									 
	ICE_CHAR cPriorCity[32];					 	//!<  优先城市范围 										 
	ICE_S32 s32PlateConfidThrld;					//!<  车牌可信度阈值（-28-28），默认：15 			 
	ICE_FLOAT fPlateAngle;							//!<  车牌倾斜角度，默认：0
	ICE_S32 s32PlateTypeSp;							//!<  支持车牌类型     0默认全部开启；
	ICE_S32	s32CarColorOn;							//!<  车身颜色额启用； 0不启用
	ICE_S32 s32PriorCityType;						//!<  优先城市类型
	ICE_S32 s32PriorArmyPlate;						//!<  军牌优先
	ICE_S32 s32CameraInstallPos;					//!<  相机安装位置（默认：司机同侧）
	ICE_S32 s32EnableOnePlateSameChars;				//!<  支持1车牌重复字符（例如:京N11111） 0不启用 1启动 ，默认：0（默认：0）
	ICE_S32 s32EnableOneCarMultiPlates;				//!<  支持1车多牌   0不启用 1启动 ，默认：0
	ICE_S32 u32Reserved[8];                         //!<  保留     									
}ICE_VD_PLATE_CONFIG_S;

//! \ingroup group_vd
//! 存储参数定义
typedef struct
{
	ICE_CHAR cSavePath[256];						//!<  存储路径 										   
	ICE_S32 u32Reserved[16];                        //!<  保留     									
}ICE_VD_SAVE_CONFIG_S;

//! \ingroup group_vd
//! 标定参数定义
typedef struct
{
	ICE_S32 s32CalibrationOnOff;					//!<  是否开启标定
	ICE_LINE_S stHLine[2];							//!<  水平两条线段
	ICE_FLOAT fHLineDist;							//!<  水平两条线段实际距离
	ICE_LINE_S stVLine[2];							//!<  竖直两条线段
	ICE_FLOAT fVLineDist;							//!<  竖直两条线段实际距离
	ICE_S32 as32Reserved;							//!<  保留 
}ICE_VD_CALIBRATION_S;
//! \ingroup group_vd
//! 车辆检测参数定义
typedef struct  
{
 	ICE_S32 s32LaneNum;									 	 	 //!<  有效车道数量  		  					  		  
	ICE_VD_LANE_S astLane[ICE_MAX_LANE_NUM];				 	 //!<  车道参数 		  						  
	ICE_VD_OSD_CONFIG_S stOsdParam;								 //!<  OSD参数   		  						  		  
	ICE_VD_TRAFFIC_CONFIG_S stTrafficParam;						 //!<  交通参数  		  						  	  
	ICE_VD_PLATE_CONFIG_S stPlateParam;							 //!<  车牌参数   		  						  	  
	ICE_VD_SAVE_CONFIG_S stSaveParam;							 //!<  存储参数   		  						  		 
	ICE_S32 s32Width;											 //!<  配置所在参考图像宽度		  						  	  
	ICE_S32 s32Height;											 //!<  配置所在参考图像高度
	ICE_VD_CALIBRATION_S stCalbiration;							 //!<  标定数据
	ICE_S32 as32Reserved[8];              						 //!<  保留 											  
}ICE_VD_PARAM_S;

#ifdef __cplusplus
}
#endif


#endif// __ICE_VD_CONFIG_H__

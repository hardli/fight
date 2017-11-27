/*! \file ice_vd_result.h
\brief ice_vd_result.h document.
*/
#ifndef __ICE_VDC_RESULT_H__
#define __ICE_VDC_RESULT_H__

#include "ice_base_type.h"
#include "ice_vlpr_result.h"
#include "ice_vdc_config.h"
#include "ice_vehibrand_interface.h"
#include "ice_com_type.h"

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum{
	ICE_VDC_HD_TRIGER,						//实时_硬触发+临时车辆(0),
	ICE_VDC_VIDEO_TRIGER, 					//实时_视频触发+临时车辆（1），
	ICE_VDC_SOFT_TRIGER,					//实时_软触发+临时车辆（2），
	ICE_VDC_HD_TRIGER_AND_WHITELIST,		//实时_硬触发+有效白名单(3),
	ICE_VDC_VIDEO_TRIGER_AND_WHITELIST,		//实时_视频触发+有效白名单（4），
	ICE_VDC_SOFT_TRIGER_AND_WHITELIST,  	//实时_软触发+有效白名单（5），
	ICE_VDC_HD_TRIGER_AND_BLACKLIST,		//实时_硬触发+黑名单(6),
	ICE_VDC_VIDEO_TRIGER_AND_BLACKLIST,		//实时_视频触发+黑名单（7），
	ICE_VDC_SOFT_TRIGER_AND_BLACKLIST,  	//实时_软触发+黑名单（8），
	
	ICE_VDC_NC_HD_TRIGER,					//脱机_硬触发+临时车辆(9),
	ICE_VDC_NC_VIDEO_TRIGER, 				//脱机_视频触发+临时车辆（10），
	ICE_VDC_NC_SOFT_TRIGER,					//脱机_软触发+临时车辆（11），
	ICE_VDC_NC_HD_TRIGER_AND_WHITELIST,		//脱机_硬触发+有效白名单(12),
	ICE_VDC_NC_VIDEO_TRIGER_AND_WHITELIST,	//脱机_视频触发+有效白名单（13），
	ICE_VDC_NC_SOFT_TRIGER_AND_WHITELIST,  	//脱机_软触发+有效白名单（14），
	ICE_VDC_NC_HD_TRIGER_AND_BLACKLIST,		//脱机_硬触发+黑名单(15),
	ICE_VDC_NC_VIDEO_TRIGER_AND_BLACKLIST,	//脱机_视频触发+黑名单（16），
	ICE_VDC_NC_SOFT_TRIGER_AND_BLACKLIST,  	//脱机_软触发+黑名单（17），
	
	ICE_VDC_HD_TRIGER_AND_OVERDUE_WHITELIST,		//实时_硬触发+过期白名单(18),
	ICE_VDC_VIDEO_TRIGER_AND_OVERDUE_WHITELIST,		//实时_视频触发+过期白名单（19），
	ICE_VDC_SOFT_TRIGER_AND_OVERDUE_WHITELIST,  	//实时_软触发+过期白名单（20），
	ICE_VDC_NC_HD_TRIGER_AND_OVERDUE_WHITELIST,		//脱机_硬触发+过期白名单(21),
	ICE_VDC_NC_VIDEO_TRIGER_AND_OVERDUE_WHITELIST,	//脱机_视频触发+过期白名单（22），
	ICE_VDC_NC_SOFT_TRIGER_AND_OVERDUE_WHITELIST,  	//脱机_软触发+过期白名单（23），
	
	ICE_VDC_ALARM_UNKOWN,
}ICE_VDC_ALARM_TYPE;

//触发类型
typedef enum{
	ICE_VDC_HD_TRIGGER,						//硬触发,
	ICE_VDC_VIDEO_TRIGGER, 					//视频触发，
	ICE_VDC_SOFT_TRIGGER,					//软触发，
	ICE_VDC_TRIGGER_UNKOWN,
}ICE_VDC_TRIGGER_TYPE;

//!车牌类型
typedef enum
{
	ICE_VDC_PLATE_TEMP,			  //临时车辆0
	ICE_VDC_PLATE_WHITELIST,		  //有效白名单1
	ICE_VDC_PLATE_BLACKLIST,		  //黑名单2,
	ICE_VDC_PLATE_OVERDUE_WHITELIST, //过期白名单3
	ICE_VDC_PLATE_UNKOWN,
}ICE_VDC_PLATE_TYPE; 

typedef struct
{
	ICE_VLPR_OUTPUT_S stPlateInfo;//车牌信息
	ICE_VDC_PLATE_TYPE s32PlateType; //车辆类型
	ICE_U16 u32StartYear; //白名单有效期起始时间
	ICE_U8 u32StartMon;
	ICE_U8 u32StartDay;
	ICE_U16 u32EndYear; //白名单有效期结束时间
	ICE_U8 u32EndMon;
	ICE_U8 u32EndDay;
	ICE_SYSTEMTIME_S  stLiveCapTime;    					//!< 实时数据 抓拍时间
	ICE_S32 u32Reserved[6];  
}ICE_VDC_CAR_INFO_S;


typedef enum
{
	ICE_VDC_VEHICLE_TYPE_UNKOWN				= 0,			//!<  未知
	ICE_VDC_VEHICLE_TYPE_NORMAL_CAR			= 1,			//!<  普通汽车
	ICE_VDC_VEHICLE_TYPE_MINIBUS			= 2,			//!<  面包车
	ICE_VDC_VEHICLE_TYPE_BUS				= 3,			//!<  大客车
	ICE_VDC_VEHICLE_TYPE_BOXTRUCK			= 4,			//!<  箱式货车
	ICE_VDC_VEHICLE_TYPE_TRUCK				= 5,			//!<  大货车
	ICE_VDC_VEHICLE_TYPE_NONMOTORIZED	    = 6,			//!<  非机动车
}ICE_VDC_VEHICLETYPE_E;



//! \ingroup group_vd
//! 图片信息数据结构
typedef struct 
{
	ICE_S32 s32EventCode;									//!< 事件码
	ICE_S32   s32LandId;									//!< 车道ID
#ifdef _WIN64
	ICE_CHAR  cFileName[120];  								//!< 文件名
#else
	ICE_CHAR  cFileName[124];  								//!< 文件名
#endif
	ICE_VBR_RESULT_S *pstVbrResult;							//!< 车款结果
	ICE_VDC_ALARM_TYPE   s32AlarmType;						//!< 报警触发类型
	ICE_SYSTEMTIME_S  stLiveCapTime;    					//!< 实时数据 抓拍时间
	ICE_U32  u32SerialNum;  								//!< 车辆流水号     	
	ICE_U32  u32PlateWidth;  								//!< 实际车辆宽度	
	ICE_U32  u32ExtSnapPixel;  								//!< 抓拍图外osd像素高度	
	ICE_Phone_Mac_List* pPhoneMacList;						//!< 接收的终端mac列表
#ifdef _WIN64
	ICE_CHAR  cReserve[80];									//!< 预留
#else
	ICE_CHAR  cReserve[84];									//!< 预留
#endif  
	ICE_S32 enVehicleColor;									//!< 车身颜色
	ICE_VDC_VEHICLETYPE_E s32VehicleType;					//!< 车辆类型
	ICE_SYSTEMTIME_S  stCapTime;    						//!< 抓拍时间
	ICE_S32    s32HaveCloseUp;     							//!< 是否有特写图像
	ICE_RECT_S stVehilceRect;	  							//!< 车辆外框
	ICE_VDC_VERHICLE_DIR_E 	enDirect;         	            //!< 车辆方向
	ICE_FLOAT 	fSpeed;         	                		//!< 车辆速度(单位:千米/小时)
	ICE_CHAR 	cVehicleColor[32];							//!< 车辆颜色
	ICE_CHAR 	cPlateColor[32];							//!< 车牌颜色
	ICE_S32    s32HavePlate;       							//!< 是否存在车牌                						
	ICE_VLPR_OUTPUT_S stPlateInfo;							//!< 车牌信息
	ICE_S16    s16PicWidth;     							//!< 图片宽度					   						
	ICE_S16    s16PicHeight;     							//!< 图片高度
	ICE_S32   s32PicDataLen;								//!< 图片大小
	ICE_S8    *ps8PictureData;     							//!< 图片数据					
}ICE_VDC_PICTRUE_INFO_S;


typedef struct 
{
	ICE_S32 s32EventCode;									//!< 事件码
	ICE_S32   s32LandId;									//!< 车道ID    			   	   						
#ifdef _WIN64
	ICE_CHAR  cFileName[120];  								//!< 文件名
#else
	ICE_CHAR  cFileName[124];  								//!< 文件名
#endif
	ICE_VBR_RESULT_S *pstVbrResult;							//!< 车款结果
	ICE_VDC_ALARM_TYPE   s32AlarmType;						//!< 报警触发类型
	ICE_SYSTEMTIME_S  stLiveCapTime;    					//!< 实时数据 抓拍时间
	ICE_U32  u32SerialNum;  								//!< 车辆流水号     	
	ICE_U32  u32PlateWidth;  								//!< 实际车辆宽度	
	ICE_U32  u32ExtSnapPixel;  								//!< 抓拍图外osd像素高度
	ICE_Phone_Mac_List* pPhoneMacList;						//!< 接收的终端mac列表
#ifdef _WIN64
	ICE_CHAR  cReserve[80];									//!< 预留
#else
	ICE_CHAR  cReserve[84];									//!< 预留
#endif
	ICE_S32 enVehicleColor;					//!< 车身颜色
	ICE_VDC_VEHICLETYPE_E s32VehicleType;					//!< 车辆类型
	ICE_SYSTEMTIME_S  stCapTime;    						//!< 抓拍时间     			   						
	ICE_S32    s32HaveCloseUp;     							//!< 是否有特写图像     						
	ICE_RECT_S stVehilceRect;	  							//!< 车辆外框    			   						
	ICE_VDC_VERHICLE_DIR_E 	enDirect;         	            //!< 车辆方向 								
	ICE_FLOAT 	fSpeed;         	                		//!< 车辆速度(单位:千米/小时) 							
	ICE_CHAR 	cVehicleColor[32];							//!< 车辆颜色
	ICE_CHAR 	cPlateColor[32];							//!< 车牌颜色
	ICE_S32    s32HavePlate;       							//!< 是否存在车牌                						
	ICE_VLPR_OUTPUT_S stPlateInfo;							//!< 车牌信息
	ICE_S16    s16PicWidth;     							//!< 图片宽度					   						
	ICE_S16    s16PicHeight;     							//!< 图片高度
	ICE_S32   s32PicDataLen;								//!< 图片大小
	ICE_S8    as8PictureData[1024*1024];     				//!< 图片数据					   						
}ICE_VDC_PICTRUE_INFO_BUFFER_S;

//! \ingroup group_vd
//!车辆检测调试信息数据结构
typedef struct 
{
	ICE_S32 s32EventCode;									//!< 事件码
	ICE_S32   s32LandId;									//!< 车道ID    			   	   						
#ifdef _WIN64
	ICE_CHAR  cFileName[120];  								//!< 文件名
#else
	ICE_CHAR  cFileName[124];  								//!< 文件名
#endif
	ICE_VBR_RESULT_S *pstVbrResult;							//!< 车款结果
	ICE_VDC_ALARM_TYPE   s32AlarmType;						//!< 报警触发类型
	ICE_SYSTEMTIME_S  stLiveCapTime;    					//!< 实时数据 抓拍时间
	ICE_U32  u32SerialNum;  								//!< 车辆流水号     	
	ICE_U32  u32PlateWidth;  								//!< 实际车辆宽度	
	ICE_U32  u32ExtSnapPixel;  								//!< 抓拍图外osd像素高度	
	ICE_CHAR  cReserve[88];  								//!< 预留 
	ICE_S32 enVehicleColor;					//!< 车身颜色
	ICE_VDC_VEHICLETYPE_E s32VehicleType;					//!< 车辆类型
	ICE_SYSTEMTIME_S  stCapTime;    						//!< 抓拍时间     			   						
	ICE_S32    s32HaveCloseUp;     							//!< 是否有特写图像     						
	ICE_RECT_S stVehilceRect;	  							//!< 车辆外框    			   						
	ICE_VDC_VERHICLE_DIR_E 	enDirect;         	            //!< 车辆方向 								
	ICE_FLOAT 	fSpeed;         	                		//!< 车辆速度(单位:千米/小时) 							
	ICE_CHAR 	cVehicleColor[32];							//!< 车辆颜色
	ICE_CHAR 	cPlateColor[32];							//!< 车牌颜色
	ICE_S32    s32HavePlate;       							//!< 是否存在车牌                						
	ICE_VLPR_RULT_S	stVlprRult;								//!< 车牌信息
	ICE_S16    s16PicWidth;     							//!< 图片宽度					   						
	ICE_S16    s16PicHeight;     							//!< 图片高度
	ICE_S32   s32PicDataLen;								//!< 图片大小
	ICE_S8    as8PictureData[1024*1024];     				//!< 图片数据					   						
}ICE_VDC_PICTRUE_INFO_BUFFER_EXT_S;



typedef struct 
{				   						
	ICE_S32   s32CloseupPicDataLen;								//!< 图片大小
	ICE_S8    as8CloseupPictureData[64*1024];     				//!< 图片数据	
}ICE_VDC_PICTRUE_INFO_BUFFER2_S;




//! \ingroup group_vd
//!车辆检测调试信息数据结构
typedef struct 
{
	void *pRgbOImage;								//!< 原始RBG图像
	void *pRgbPImage;								//!< 虚拟线圈处理的RBG图像
	void *pRgbCImage[3];							//!< 特写RBG图像列表,对应不同的车道
	void *pRgbPlateImage[3];						//!< 车牌RBG图像列表,对应不同的车道
	ICE_S32 s32EventCode[4];						//!< 车道事件码			
	ICE_S32    s32HavePlate[4];						//!< 是否存在车牌
	ICE_VLPR_OUTPUT_S pVlprResult[4];				//!< 车牌信息
	ICE_CHAR cPlateColor[4][32];					//!< 车牌颜色
}ICE_VDC_DEBUG_INFO_S;

/*----------------------------------------------*
* vdc result defination            				*
*----------------------------------------------*/ 
typedef struct 
{
	ICE_S32 s32IsNight;										//!< 夜间标志(1表示夜间 0表示白天)    			
	ICE_S32 s32IsTriger;									//!< 外触发标志(1表示强制触发 0表示内部触发)    			
	ICE_S32 s32TrigerLaneId;								//!< 外触发车道ID(0-3，-1表示所有车道全部触发)
	ICE_S32 s32TrigerSpecialDeal;							//!< 外触发特殊处理标志(1表示启动特殊处理 0不启动特殊处理)
	ICE_S32 u32Reserved[13];                         		//!< 保留
}ICE_VDC_OUT_INTFC_S;

#ifdef __cplusplus
}
#endif


#endif
	
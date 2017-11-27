#ifndef ice_vehicolor_interface_h__
#define ice_vehicolor_interface_h__

/********************************************************************
Copyright (C), 1994-2016, 北京智芯原动科技有限公司

文件名称: 	ice_vehicolor_interface.h
文件描述:	车辆颜色识别头文件
文件作者：	
修改历史：	2016-6-3 14:00:36	 creat V1.0.001 
*********************************************************************/

#include "ice_base_type.h"

typedef enum
{
	ICE_VCR_VEHICLE_COLOR_UNKOWN 	= -1,	 //!<  未知
	ICE_VCR_VEHICLE_COLOR_BLACK 	= 0,	 //!<  黑色
	ICE_VCR_VEHICLE_COLOR_BLUE 		= 1,	 //!<  蓝色

	ICE_VCR_VEHICLE_COLOR_GRAY 		= 2,	 //!<  灰色	
	ICE_VCR_VEHICLE_COLOR_BROWN 	= 3,	 //!<  棕色
	ICE_VCR_VEHICLE_COLOR_GREEN 	= 4,	 //!<  绿色
	ICE_VCR_VEHICLE_COLOR_NIGHTDARK	= 5,   	 //!<  夜间深色
	ICE_VCR_VEHICLE_COLOR_PURPLE	= 6,	 //!<  紫色
	ICE_VCR_VEHICLE_COLOR_RED 		= 7,	 //!<  红色
	ICE_VCR_VEHICLE_COLOR_WHITE 	= 8,	 //!<  白色
	ICE_VCR_VEHICLE_COLOR_YELLOW 	= 9,	 //!<  黄色
	
}ICE_VCR_VEHICLECOLOR_E;

typedef struct
{
	ICE_S8      eVehiColor;
	ICE_FLOAT   fScore;    

	ICE_S8      szLabelTotal[40];           //总label
	ICE_U32     iLabelIndex;               // label index
	ICE_U32     iReserved[4];             //预留
}ICE_VCR_RESULT_S;


typedef struct
{
	char * strModelPath;              //cnn model路径

	ICE_U32 iInputW;                   //输入图像大小
	ICE_U32 iInuptH;                    
	ICE_U32  iReserved[4];             //预留
}ICE_VCR_INPUTPARAM_S;

typedef struct
{
	ICE_RECT_S stRect;
	ICE_U32    iType;//0-from plate.1-from vehicle detection
	ICE_U32    iReserved[4];
}ICE_VCR_RTEX_S;



//////////////////////////////////////////////////////////////////////////
//vehicle brand recognization  interface
ICE_EXPORTS ICE_HANDLE ICE_VehiColorRegCreate();
ICE_EXPORTS ICE_STATUS_E ICE_VehiColorRegInit(ICE_HANDLE _hHandle, ICE_VCR_INPUTPARAM_S *_pstInputRaram);

ICE_EXPORTS ICE_STATUS_E ICE_VehiColorRegProcess(ICE_HANDLE hHandle,ICE_YUV422P_IMAGE_S * InputImageyuv, 
												 ICE_VCR_RTEX_S _stRectex, ICE_VCR_RESULT_S *pstRult);

ICE_EXPORTS ICE_STATUS_E ICE_VehiColorRegRelease(ICE_HANDLE hHandle);

//! \brief 获取算法版本号
//! \param Object	算法引擎
//! \param Version  输出版本号地址指针。要求指针所指数组长度大于64 BYTE
//! \return			操作状态，参考\ref ICE_STATUS_E
ICE_EXPORTS ICE_STATUS_E  ICE_VehiColorReg_GetVersion(void   *_hHandle, char * Version);


#endif // ice_vehibrand_interface_h__

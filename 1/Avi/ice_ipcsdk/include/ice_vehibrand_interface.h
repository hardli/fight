#ifndef ice_vehibrand_interface_h__
#define ice_vehibrand_interface_h__

/********************************************************************
Copyright (C), 1994-2016, 科技有限公司

文件名称: 	ice_vehibrand_interface.h
文件描述:	车款识别头文件
文件作者：	
修改历史：	2016-4-26 15:10:38	 creat V1.0.001 
*********************************************************************/

#include "ice_base_type.h"

#define VBR_RES_TOPN 10               //返回结果个数

typedef struct
{
	ICE_S8 szLogName[20];             //品牌
	ICE_S8 szSubLogName[20];          //子品牌
	ICE_S8 szProductYearName[20];     //生产年代

	ICE_S8 szLabelTotal[40];           //总label
	ICE_U32 iLabelIndex;               //总label index
	ICE_FLOAT fScore;                  //置信度

	//first VBR_RES_TOPN:label
	//last VBR_RES_TOPN:score
	ICE_FLOAT fResFeature[VBR_RES_TOPN*2];
 
// 	ICE_S8 szLabelTotalTopN[VBR_RES_TOPN][40]; //返回top n个结果,测试识别率时可以导出

// 	ICE_FLOAT fScoreTopN[VBR_TOP_N];
// 	ICE_U32 iIndexTopN[VBR_TOP_N];

	ICE_U32  iReserved[4];             //预留
}ICE_VBR_RESULT_S;

typedef struct      
{
	char * strModelPath;              //cnn model路径

	ICE_U32 iInputW;                   //输入图像大小
	ICE_U32 iInuptH;                    
	ICE_U32  iReserved[4];             //预留
}ICE_VBR_INPUTPARAM_S;

typedef struct
{
	ICE_RECT_S stRect;
	ICE_U32 iType;//0-from plate.1-from vehicle detection
	ICE_U32  iReserved[4];
}ICE_VBR_RTEX_S;

//////////////////////////////////////////////////////////////////////////
//vehicle brand recognization  interface
ICE_EXPORTS ICE_HANDLE ICE_VehiBrandRegCreate();
ICE_EXPORTS ICE_STATUS_E ICE_VehiBrandRegInit(ICE_HANDLE _hHandle, ICE_VBR_INPUTPARAM_S *_pstInputRaram);

ICE_EXPORTS ICE_STATUS_E ICE_VehiBrandRegProcess(ICE_HANDLE hHandle,ICE_GRAY_IMAGE_S *pGrayImage, 
												 ICE_VBR_RTEX_S _stRectex, ICE_VBR_RESULT_S *pstRult);

ICE_EXPORTS ICE_STATUS_E ICE_VehiBrandRegRelease(ICE_HANDLE hHandle);

//! \brief 获取算法版本号
//! \param Object	算法引擎
//! \param Version  输出版本号地址指针。要求指针所指数组长度大于64 BYTE
//! \return			操作状态，参考\ref ICE_STATUS_E
ICE_EXPORTS ICE_STATUS_E  ICE_VehiBrandReg_GetVersion(void   *_hHandle, char * Version);

//算法复杂度为：VBR_RES_TOPN*VBR_RES_TOPN*COMPARE_CNT(特征比较次数).
ICE_EXPORTS ICE_FLOAT ICE_VehiBrandReg_CompareFeat(ICE_FLOAT *_pfResFeat1, ICE_U32 _iFeat1Len,
												  ICE_FLOAT *_pfReaFeat2, ICE_U32 _iFeat2Len);


#endif // ice_vehibrand_interface_h__

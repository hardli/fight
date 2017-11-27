#include "stdafx.h"
#include "Avi_Algo.h"
#include "WJ_Avi_Main.h"
typedef struct vehInfo      //车辆信息结构体
{	
	int AxisGroupNum;//轴组数
	int AxisNumber; //轴数	
	int VehicleType; //车型
	long AxisWeight[MAX_AXLE_NUM];	//轴组重
	int AxisType[MAX_AXLE_NUM];	//轴型
	int Speed;		//车速度
	int Wide;       //0：正常过车，1：倒车，2：清除此车辆数据
	
}VEHINFO;

VEHINFO VD[VDSIZE];//车辆队列缓存
unsigned char cVehNum = 0;//总车数

unsigned char cCurVeh = 0;//当前待发送的车辆指针
unsigned char cLastVeh = 0;//要插入车辆数据的指针
int g_status =0;     //设备运行状态



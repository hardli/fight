#ifndef _VEHIDENTPROC_H
#define _VEHIDENTPROC_H


#include "JZGlobal.h"


JZG_EXT void VehIdentProc(void);//车型识别算法处理
JZG_EXT void InitModeParam(void);//模型参数初始化


uint8 Polar2Rect(void);//极坐标转直角坐标附带滤波 分大小点
void FramAnlsye(void);//帧分析
void ClearVehicleErr(void);//出错处理
uint8 IsRegionMatch(FramInfo FI1,FramInfo FI2);//是否区域匹配
void UpdateVehRegion(VehicleSet *pVehicle);//更新车辆区域信息
void OutPutVeh(VehicleSet *pVehicle);//车型识别并输出结果
void Write2TxtFile(VehicleSet *pVehicle);//结果写入txt文件
void VehIdentifier(VehicleSet *pVehicle);//车型识别器
void CheckVehState(void);//检查车辆状态
void VehRegionMatch(void);//区域车辆匹配
uint8 IsVehCheck(VehicleSet *pVehicle);//判断是否为车辆
void InitMiddleWare(void);//清空中间件
void VehDirCheck(void);//感知车辆行进方向 

void ReportNormalVeh(VehicleSet *pVehicle);//上报车辆信息
void ForceCloseProc(void);//强制出车处理
uint8 ForceVehJudge(VehicleSet *pVehicle);//强制出车车型判断
void ReportForceVeh(VehicleSet *pVehicle);//强制出车发送车辆信息
void Dev_CoilDirCheck(void);//线圈感知方向计数 
void Dev_GratDirCheck(void);//光栅感知方向计数 
void Dev_LaseDirCheck(void);//激光感知方向计数  
void VsInitVOutTH(void);//Vs版设置出车阈值
uint8 CanBeForceOut(VehicleSet *pVehicle);//判断当前车辆是否可以被强制出车
void FramDelNoise(void);//剔除每帧干扰点 20171116
#endif

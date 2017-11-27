#ifndef _AVISPECHECK_H_
#define _AVISPECHECK_H_

#include "JZGlobal.h"


void JinBeiCheck(VehicleSet *pVehicle);//金杯车检测
void TruckTailCheck(VehicleSet *pVehicle);//货车检测
void PickUpCheck(VehicleSet *pVehicle);//皮卡检测
void PickUpFramCal(VehicleSet *pVehicle);//皮卡帧检测算子
void SpecialVehCheck(VehicleSet *pVehicle);//特殊车辆检测
void SpecialVehCal(VehicleSet *pVehicle);//特殊车辆检测算子
void PickUpTailCheck(VehicleSet *pVehicle);//皮卡尾部检测
void EnCloseWinCal(VehicleSet *pVehicle);//封闭车窗检测算子
void EnCloseWinCheck(VehicleSet *pVehicle);//封闭车窗检测
void Axle2WinCheckEx(VehicleSet *pVehicle);//第二轴车窗客车检测

void EnCloseWinCalEx(VehicleSet *pVehicle);//封闭车窗检测算子
void EnCloseWinCheckEx(VehicleSet *pVehicle);//封闭车窗检测


void FramArcCal(VehicleSet *pVehicle);//罐体车检测算子
int ArcTruckClassify(VehicleSet *pVehilce);//罐体车分类
void ArcTruckCheck(VehicleSet *pVehicle);//油罐车输出

#endif

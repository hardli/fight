#ifndef _AVISPECHECK_H_
#define _AVISPECHECK_H_

#include "JZGlobal.h"


void JinBeiCheck(VehicleSet *pVehicle);//�𱭳����
void TruckTailCheck(VehicleSet *pVehicle);//�������
void PickUpCheck(VehicleSet *pVehicle);//Ƥ�����
void PickUpFramCal(VehicleSet *pVehicle);//Ƥ��֡�������
void SpecialVehCheck(VehicleSet *pVehicle);//���⳵�����
void SpecialVehCal(VehicleSet *pVehicle);//���⳵���������
void PickUpTailCheck(VehicleSet *pVehicle);//Ƥ��β�����
void EnCloseWinCal(VehicleSet *pVehicle);//��ճ����������
void EnCloseWinCheck(VehicleSet *pVehicle);//��ճ������
void Axle2WinCheckEx(VehicleSet *pVehicle);//�ڶ��ᳵ���ͳ����

void EnCloseWinCalEx(VehicleSet *pVehicle);//��ճ����������
void EnCloseWinCheckEx(VehicleSet *pVehicle);//��ճ������


void FramArcCal(VehicleSet *pVehicle);//���峵�������
int ArcTruckClassify(VehicleSet *pVehilce);//���峵����
void ArcTruckCheck(VehicleSet *pVehicle);//�͹޳����

#endif

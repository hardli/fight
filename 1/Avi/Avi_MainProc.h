#ifndef _VEHIDENTPROC_H
#define _VEHIDENTPROC_H


#include "JZGlobal.h"


JZG_EXT void VehIdentProc(void);//����ʶ���㷨����
JZG_EXT void InitModeParam(void);//ģ�Ͳ�����ʼ��


uint8 Polar2Rect(void);//������תֱ�����긽���˲� �ִ�С��
void FramAnlsye(void);//֡����
void ClearVehicleErr(void);//������
uint8 IsRegionMatch(FramInfo FI1,FramInfo FI2);//�Ƿ�����ƥ��
void UpdateVehRegion(VehicleSet *pVehicle);//���³���������Ϣ
void OutPutVeh(VehicleSet *pVehicle);//����ʶ��������
void Write2TxtFile(VehicleSet *pVehicle);//���д��txt�ļ�
void VehIdentifier(VehicleSet *pVehicle);//����ʶ����
void CheckVehState(void);//��鳵��״̬
void VehRegionMatch(void);//������ƥ��
uint8 IsVehCheck(VehicleSet *pVehicle);//�ж��Ƿ�Ϊ����
void InitMiddleWare(void);//����м��
void VehDirCheck(void);//��֪�����н����� 

void ReportNormalVeh(VehicleSet *pVehicle);//�ϱ�������Ϣ
void ForceCloseProc(void);//ǿ�Ƴ�������
uint8 ForceVehJudge(VehicleSet *pVehicle);//ǿ�Ƴ��������ж�
void ReportForceVeh(VehicleSet *pVehicle);//ǿ�Ƴ������ͳ�����Ϣ
void Dev_CoilDirCheck(void);//��Ȧ��֪������� 
void Dev_GratDirCheck(void);//��դ��֪������� 
void Dev_LaseDirCheck(void);//�����֪�������  
void VsInitVOutTH(void);//Vs�����ó�����ֵ
uint8 CanBeForceOut(VehicleSet *pVehicle);//�жϵ�ǰ�����Ƿ���Ա�ǿ�Ƴ���
void FramDelNoise(void);//�޳�ÿ֡���ŵ� 20171116
#endif

#include "stdafx.h"
#include "Avi_Algo.h"
#include "WJ_Avi_Main.h"
typedef struct vehInfo      //������Ϣ�ṹ��
{	
	int AxisGroupNum;//������
	int AxisNumber; //����	
	int VehicleType; //����
	long AxisWeight[MAX_AXLE_NUM];	//������
	int AxisType[MAX_AXLE_NUM];	//����
	int Speed;		//���ٶ�
	int Wide;       //0������������1��������2������˳�������
	
}VEHINFO;

VEHINFO VD[VDSIZE];//�������л���
unsigned char cVehNum = 0;//�ܳ���

unsigned char cCurVeh = 0;//��ǰ�����͵ĳ���ָ��
unsigned char cLastVeh = 0;//Ҫ���복�����ݵ�ָ��
int g_status =0;     //�豸����״̬



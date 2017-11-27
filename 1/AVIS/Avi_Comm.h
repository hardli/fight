#ifndef _AVICOM_H_
#define _AVICOM_H_

#include "Config.h"




typedef struct tagVehSet
{
	uint8 VoutFlag;//������������ 0--����������1--ǿ�Ƴ�����2--�ֶ�����
	uint32 VehId;
	uint8 VPattern;
	uint8 VDir;
	uint8 VSpeed;
	uint8 AxleNum;
	uint8 AxleType[6];
	uint8 TyreCnt;
	uint8 VJudCode;
	uint8 VAdjCode;
	uint8 VSpeCode;
	uint32 TranAxleType;//ת���������
	uint8 AxlegroupNum; //������
	uint16 VDpHt;//���̸�
	uint16 VehHeight;//����
	uint16 VehHeadHt;//��ͷ��
	uint8 SideUiform;//�Ƿ�߱�����һ����
	uint8 CastGrd;//�Ƿ���м�Ͷ��������
	uint8 Hopper;//�Ƿ�Ϊ������1-Ϊԭʼ��������2-������භ��4-���ᶨλ��
	uint8 Container;//�Ƿ�߱����� 0--�����ڻ��䣻1--����ƽ����2--��ʻ�ҵ���ߣ�
	uint8 HeadJump;//�Ƿ���ڳ�ͷ���� 0--�����ڣ�1--ֱ���ҵ����ڣ�2--���ɶδ��ڣ�
	uint8 TailJump;//�Ƿ����β������ 0--�����ڣ�1--���ڣ�
	uint8 Cargo;//�Ƿ��ػ� 1--X�����ػ���2--�����ػ���
	uint8 FillWin;//�Ƿ��ճ�����
	uint8 GlassWin;//�Ƿ���ڲ�������
	uint8 TruckTail;//�Ƿ���ڳ�β;

	uint8 FoldName[24];
	uint8 Plate[20];//

}VehSet;//������Ϣ��¼


typedef struct tagModelSet
{
	uint16 ZeroPos;//���ĵ�
	uint16 JGHeight;//�������߶�
	uint16 StartX;//��������ʼ��X(��Ѱ�������ʼX)
	uint16 LaneWidth;//��·���
	uint16 MaxHt;//��󾻿ո߶�
	uint16 IsLanHt;//��ȫ���߶�
	uint16 IsLanWd;//��ȫ�����
	uint16 u16CRC;//CRCУ��	
}ModelSet;


typedef struct tagLaserSet
{
	char IP0[15];
	uint16 Port0;
	char IP1[15];
	uint16 Port1;
	uint8 AqcMode;
}LaserSet;

#define FILESIN 2
#define SOFTSIM 1
#define LASERIN 0

typedef struct tagSysTem
{
	uint8 RunMode;//����ģʽ
	uint8 ProjectId;//�������
	uint8 DirEnable;//�����豸ʹ��
	uint8 DirType;//�����豸����
	uint8 SvWv;//�沨�α�־λ
	uint8 PlotImg;//��ͼ��־λ
	uint8 SvDB;//�����ݿ��־λ
	uint8 WvCheck;//���ζ�֡����־λ
}SystemSet;

typedef struct tagVOutSet
{
	uint8 A3ForOut;
	uint8 AxReport;
	uint8 EmFramCnt;
	uint8 HiFramCnt;
	uint8 LwFramCnt;
	uint8 LvFramCnt;
	uint8 GratPos;
	uint16 GratHt;
}VOutSet;

typedef struct tagWvFiles
{
	uint8 AddEmFram;
	uint8 ForceOut;
	uint8 PathExist;
	uint8 bSearChed;
	uint8 WvPath[MAX_PATH];
}WvFiles;


#define Max_CACHE 10000
#define	Max_Size  831
typedef struct tagSvWave
{
	uint8  bSvFlag;
	uint16 SvIndx;
	HANDLE SvThdHd;
	uint16 RecIndx;
	uint16 SvCnt;
	uint8 WvCache[Max_CACHE][Max_Size+5];//�ṹ��� 831����+�³���־1λ
}SvWave;

typedef struct tagAqcState
{
	uint32 RecTotal;
	uint32 ProWvCnt;
	uint32 RecWvErr;
	uint32 RecWvCnt;
	uint32 RecErrCnt;
	uint32 RecCmdCnt;
}AqcState;

typedef struct tagCameSet
{
	uint8 CamEnable;
	char CamIp[20];
	char CamPwd[30];
	uint16 CamPort;
	uint8 CamPicCar;
	uint8 CamPicPlate;

	uint8 CamEnable1;
	char CamIp1[20];
	char CamPwd1[30];
	uint16 CamPort1;
	uint8 CamPicCar1;
	uint8 CamPicPlate1;
}CamSet;

typedef struct tagQueSet
{
	 int DelVehTime ;		//ɾ������ʱ�䣬Ĭ��6��
	 int EndVehWaitTime;	//ǿ����βʱ�ȴ�����ʱ��
	 int ClearQueTime;		//��������һ��ʱ���������
	 int MaxVehCnt;			//����������������
	 int Quecheck;			//����ά��ʹ��
	 int TypecheckID;		//��ʡ�ݳ�������ת������
	 int IsETC;				//�Ƿ���ETC����
}QueSet;

/************************************************************************/
/* �궨�������                                                         */
/************************************************************************/
#define ERR_FAIL 0  //ʧ�ܻ�����
#define ERR_SUCC 1  //�ɹ�������



//2-200 Ϊ����DLLԤ��


//200-300 Ϊ�㷨DLLԤ��
#define ERR_MdSET 201
#define ERR_LsSet 202
#define ERR_SySet 203
#define ERR_VOSet 204
#define ERR_PTNUL 205
#define ERR_THNUL 206
#define ERR_PFVEH 207
#define ERR_PFAXL 208
#define ERR_PFPLT 209

//301-400 Ϊ������������
#define ERR_SOCKVERERR 301
#define ERR_CREATESOCK 302
#define ERR_CONNFAILED 303
#define ERR_RECVTHREAD 304
#define ERR_RECFUNNULL 305
#define ERR_SVRCLOSED  306
#define ERR_RECCONNERR 307
#define ERR_SENDBUFERR 308
#define ERR_DISCONNERR 309
#define ERR_AQCSINGLE 310 
#define ERR_AQCCONTIN 311
#define ERR_STOPCONTI 312

#endif
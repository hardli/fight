#include "stdafx.h"
#include "Avi_MainProc.h"
#include "Avi_MathProc.h"
#include "Avi_AxleCheck.h"
#include "Avi_FeatureCal.h"
#include "Avi_Classifier.h"
#include "Avi_AxleCheck.h"
#include "Avi_SpeCheck.h"
#include "Avi_Algo.h"
#include "Avi_Main.h"
#include "Avi_SvWv.h"

uint8  g_u8ItemName[24];//��¼���������ļ�������
uint32 g_n32WaveNumber;//vc����
int32 g_CoorX[POINT_SUM] = {0}; //ɨ���X����
int32 g_CoorZ[POINT_SUM] = {0}; //ɨ���Z����
int32 g_FiltX[POINT_SUM]; //ɨ���X�����˲�������
int32 g_FiltZ[POINT_SUM]; //ɨ���Z�����˲�������
int32 LMS_data_1[POINT_SUM+1];//ɨ���������

SysModelParam Smp;//ϵͳģ�Ͳ���
FramInfo CurFramInfo;//��ǰ֡��Ϣ
VehOutSet g_VehOutSet;//������β�����ֵ
VehIndexSet  g_VehIndex;//������ż�¼��
WeightAtSet g_WeigthAt;//��̨��������
SysTemSet   g_SysSetUp;//ϵͳ����
DllAtState g_DllAtState;//����λ��DLL������״̬
VehicleSet g_VehicleSet;


/************************************************************************/
/* ����ʶ����                                                         */
/************************************************************************/
void VehIdentProc(void)
{
	//����м��
	InitMiddleWare();
	
#ifdef _KeilEnVi
	ForceCloseProc();
#endif	
	
	Polar2Rect();
	FramDelNoise();
	FramAnlsye();
	
	VehRegionMatch();
	
#ifdef _KeilEnVi
	VehDirCheck();
#endif	
	CheckVehState();
	
}

/************************************************************************/
/* ����м��                                                           */
/************************************************************************/
void InitMiddleWare(void)
{
	//�����������
	memset(g_FiltZ,0,sizeof(g_FiltZ));
	memset(g_FiltX,0,sizeof(g_FiltX));
	memset(g_CoorX,0,sizeof(g_CoorX));
	memset(g_CoorZ,0,sizeof(g_CoorZ));
	
	//���֡��¼��
	memset(&CurFramInfo,0,sizeof(CurFramInfo));
}

#ifndef _KeilEnVi
/************************************************************************/
/* ��λ����ʼ��������ֵ����                                             */
/************************************************************************/
void VsInitVOutTH(void)
{
	g_VehOutSet.u8EmptyFrCnt=NORMAL_MAX_EMPTYFRAME;
	g_VehOutSet.u8DirLwCnt=15;
	g_VehOutSet.u8DirHiCnt=40;
	g_VehOutSet.u8DirLvCnt=9;
	g_VehOutSet.u16GratHt=1800;
	g_VehOutSet.u8GratPos=0;
}
#endif


/************************************************************************/
/* ģ�Ͳ�����ʼ��                                                       */
/************************************************************************/
void InitModeParam(void)
{	
#ifdef _KeilEnVi	
	
#else 
	//���ɽ���� 20170524
	//Smp.ZeroPos = 320;
	//Smp.JGHeight = 1150;
	//Smp.StartX=600;
	//Smp.EndX=3200;
	//Smp.MaxHt=5000;
	//Smp.StartX0=Smp.StartX-ISLANWD;
	//Smp.IsLanHt=ISLANHT;
	//Smp.ScanMode=H2LSCAN;	
#endif	
	
	if (Smp.ZeroPos>POINT_CEN)
	{
		Mid_Point_Cal=Smp.ZeroPos;
	}
	else
	{
		Mid_Point_Cal=POINT_END-Smp.ZeroPos;
	}
	SetAxleParam();
}

/************************************************************************/
/* ������תֱ������ �����˲�   �ִ�С�����                             */
/************************************************************************/
uint8 Polar2Rect(void)
{
	int16 u16Index=0;
	uint32 Rx=0;
	uint32 Rz=0;
	int32 dif = 0;
	uint32 ThresDis[POINT_SUM] = {0};//ÿ���˲���ֵ
	uint32 min_neighour = 65535;
	int32 TmpL=0;
	int32 abs_temp = 0;
	
	if( Smp.ZeroPos > POINT_CEN ) 
	{
		//�ɸߵ���
	}
	else 
	{
		// �ɵ͵���
		for(u16Index = 0; u16Index <POINT_CEN; u16Index++)
		{
			TmpL  = LMS_data_1[u16Index];
			LMS_data_1[u16Index] = LMS_data_1[POINT_END -u16Index];
			LMS_data_1[POINT_END -u16Index] = TmpL ;
		}
	}
	
	
	for (u16Index = 0; u16Index<=Mid_Point_Cal; u16Index++)
	{
		if (LMS_data_1[u16Index]<LMS_MINTH)
		{
			//��Ϊ��ɻ��಻����ɨ���
			g_CoorX[u16Index]=0;
			g_CoorZ[u16Index]=0;
		}
		else
		{
			//����ֱ������ת��
			if(u16Index < Mid_Point_Cal && u16Index >=0)//����ת����
			{
				if(ABS(Mid_Point_Cal-u16Index)<=POINT_CEN)
				{
					abs_temp = Smp.JGHeight - (LMS_data_1[u16Index] * Tabcos[ABS(Mid_Point_Cal - u16Index)] >> 15);
					g_CoorZ[u16Index] = (abs_temp);
					g_CoorX[u16Index] = (LMS_data_1[u16Index]*Tabsin[ABS(Mid_Point_Cal-u16Index)]>>15);
				}
				else
				{
					abs_temp = Smp.JGHeight + (LMS_data_1[u16Index] * Tabsin[ABS(Mid_Point_Cal - u16Index) % POINT_CEN] >> 15);
					g_CoorZ[u16Index] = (abs_temp);
					g_CoorX[u16Index] = (LMS_data_1[u16Index]*Tabcos[ABS(Mid_Point_Cal-u16Index)%POINT_CEN]>>15);
				}
			}
			else
			{
				abs_temp = Smp.JGHeight - ((LMS_data_1[u16Index] * Tabcos[ABS(Mid_Point_Cal - u16Index)]) >> 15);
				g_CoorZ[u16Index] = (abs_temp);
				g_CoorX[u16Index] = -1*(LMS_data_1[u16Index]*Tabsin[ABS(Mid_Point_Cal-u16Index)])>>15;
				
			}
			
			//���ڳ�����Χ�Ͳ��������߶ȷ�Χ��ɨ��� ��Ϊ0
			if (g_CoorX[u16Index]>Smp.StartX+Smp.LaneWidth||g_CoorZ[u16Index]>Smp.MaxHt)
			{
				g_CoorX[u16Index]=0;
				g_CoorZ[u16Index]=0;
			}
		}
		
		//ֻ��һ���߶Ⱥ�X��Χ�ڵĵ�����˲����� ����CPUʹ����
		if (g_CoorX[u16Index]<Smp.StartX - 300
			|| g_CoorZ[u16Index]>1200)
		{
			g_FiltX[u16Index]=g_CoorX[u16Index];
			g_FiltZ[u16Index]=g_CoorZ[u16Index];
			continue;
		}
		
		
		Rx = TabRsin[ABS(u16Index - Mid_Point_Cal)];
		Rz = TabRcos[ABS(u16Index - Mid_Point_Cal)];
		
		
		//�����ֵΪ�˲���ֵ
		if( Rx > Rz )
			ThresDis[u16Index] = Rx*LMS_data_1[u16Index]/1000;
		else
			ThresDis[u16Index] = Rz*LMS_data_1[u16Index]/1000;
		
		//���û�У���ֵ��СĬ��Ϊ30mm
		if( ThresDis[u16Index] < 30 )
			ThresDis[u16Index] = 30;
		
		//3�����˲�
		if( u16Index > 2 && u16Index < 358 )
		{
			min_neighour = 65535;
			abs_temp = 0;
			//if (g_CoorZ[u16Index] < 30 && g_CoorX[u16Index] > Smp.StartX  &&  g_CoorX[u16Index] < Smp.StartX+Smp.LaneWidth ) //�Ը߶�100���µĵ����3���ֵ�˲�
			//{
			//	for (dif = -1; dif <= 1; dif++)
			//		abs_temp += g_CoorZ[u16Index + dif];
			//	
			//	g_CoorZ[u16Index] = abs_temp / 3;
			//}
			//abs_temp = 0;
			for(dif = -3; dif <= 3; dif++)
			{	
				if (dif == 0)
					continue;
				abs_temp = (LMS_data_1[u16Index] - LMS_data_1[u16Index + dif]) / dif;
				abs_temp = ABS(abs_temp);
				if ( abs_temp < min_neighour)
				{
					min_neighour = abs_temp;
				}
			}
			if(min_neighour > ThresDis[u16Index] * 115/100 && LMS_data_1[u16Index+1] > 20 && LMS_data_1[u16Index -1]>20)
			{
				//�˵���Ҫ
			}
			else
			{
				g_FiltZ[u16Index] = g_CoorZ[u16Index];
				g_FiltX[u16Index] = g_CoorX[u16Index];
			}
		}
		else if( u16Index >= 0 && u16Index <= 360  && g_CoorX[u16Index] && g_CoorZ[u16Index] )
		{
			g_FiltZ[u16Index] = g_CoorZ[u16Index];
			g_FiltX[u16Index] = g_CoorX[u16Index];
		}			
	}
	return ERROR_NONE;
}


/************************************************************************/
/* ֡���ŵ��˳�                                                         */
/************************************************************************/
void FramDelNoise(void)
{
	int16 FstZ=0;
	int16 FstZI=0;
	int16 SecZ=0;
	int16 SecZI=0;
	int16 ThdZ=0;
	int16 ThdZI=0;
	int16 FstX=9999;
	int16 FstXI=0;
	int16 SecX=9999;
	int16 SecXI=0;
	int16 ThdX=9999;
	int16 ThdXI=0;


	int16 i16IndxI=0;

	//�ӵ͵��߱����޳��쳣��
	for (i16IndxI=Mid_Point_Cal;i16IndxI>0;i16IndxI--)
	{
		if (g_FiltZ[i16IndxI]>800 )
		{
			if (g_FiltZ[i16IndxI]>FstZ)
			{
				ThdZ=SecZ;
				ThdZI=SecZI;
				SecZ=FstZ;
				SecZI=FstZI;
				FstZ=g_FiltZ[i16IndxI];
				FstZI=i16IndxI;
			}
			else if (g_FiltZ[i16IndxI]>SecZ)
			{
				ThdZ=SecZ;
				ThdZI=SecZI;
				SecZ=g_FiltZ[i16IndxI];
				SecZI=i16IndxI;
			}
			else if (g_FiltZ[i16IndxI]>ThdZ)
			{
				ThdZ=g_FiltZ[i16IndxI];
				ThdZI=i16IndxI;
			}

			if (FstX>g_FiltX[i16IndxI])
			{
				ThdX=SecX;
				ThdXI=SecXI;
				SecX=FstX;
				SecXI=FstXI;
				FstX=g_FiltX[i16IndxI];
				FstXI=i16IndxI;
			}
			else if (SecX>g_FiltX[i16IndxI])
			{
				ThdX=SecX;
				ThdXI=SecXI;
				SecX=g_FiltX[i16IndxI];
				SecXI=i16IndxI;
			}
			else if (ThdX>g_FiltX[i16IndxI])
			{
				ThdX=g_FiltX[i16IndxI];
				ThdXI=i16IndxI;
			}
		}
	}
	//�߶ȷ����޳�����2���ĵ�
	if (ThdZ  &&  SecZ > ThdZ + 300  &&  SecZ) //2�����쳣
	{
		g_FiltX[SecZI]=0;
		g_FiltZ[SecZI]=0;
		g_FiltX[FstZI]=0;
		g_FiltZ[FstZI]=0;
	}
	else if (SecZ  &&  FstZ > SecZ + 300  &&  FstZ) //1�����쳣
	{
		g_FiltX[FstZI]=0;
		g_FiltZ[FstZI]=0;
	}

	//�����һ��͵ڶ���x�쳣
	if (ThdX<9999 && ThdX>SecX+300 && SecX<9999)
	{
		g_FiltX[SecXI]=0;
		g_FiltZ[SecXI]=0;
		g_FiltX[FstXI]=0;
		g_FiltZ[FstXI]=0;
	}
	else if (FstX<9999 && SecX>FstX+300 && SecX<9999)
	{
		g_FiltX[FstXI]=0;
		g_FiltZ[FstXI]=0;
	}

}
//void FramDelNoise(void)
//{
//	int16 i16IndxI=0;
//	int16 i16IndxJ=0;
//	int16 i16LastI=0;//���һ���Ǵ�ɵ�����
//	int16 DiffX=0;
//	int16 DiffZ=0;
//	int16 AvgX=0;
//	int16 AvgZ=0;
//	int16 CalCnt=0;
//	int16 DiffAvgX=0;
//	int16 DiffAvgZ=0;
//	//�ӵ͵��߱����޳��쳣��
//	for (i16IndxI=Mid_Point_Cal;i16IndxI>0;i16IndxI--)
//	{
//		//800�߶����Ͻ��и��ŵ���
//		if (g_FiltZ[i16IndxI]>800 && i16LastI)
//		{
//			//�����ǰ��Ⱥ�һ����x��z���ܴ� ������ŵ���
//			DiffX=g_FiltX[i16IndxI]-g_FiltX[i16LastI];
//			DiffZ=g_FiltZ[i16IndxI]-g_FiltZ[i16LastI];
//			if ((ABS(DiffX)>300&&DiffZ>500)||
//				(ABS(DiffX)>500&&DiffZ>300)/*||
//				DiffX<-600||DiffZ>1000*/)
//			{
//				AvgX=0;
//				AvgZ=0;
//				CalCnt=0;
//				DiffAvgZ=0;
//				DiffAvgX=0;
//				for (i16IndxJ=i16LastI;i16IndxJ<Mid_Point_Cal;i16IndxJ++)
//				{
//					if (g_FiltZ[i16IndxJ]>0)
//					{
//						AvgX+=g_FiltX[i16IndxJ];
//						AvgZ+=g_FiltZ[i16IndxJ];
//						CalCnt++;
//					}
//
//					if (CalCnt>2)
//					{
//						//�ҵ�3�����˳�
//						break;
//					}
//				}
//
//				//����2����
//				if (CalCnt>1)
//				{
//					AvgX=AvgX/CalCnt;
//					AvgZ=AvgZ/CalCnt;
//				}
//
//				//��ȡ����ֵ
//				if (AvgX)
//				{
//					DiffAvgX=g_FiltX[i16IndxI]-AvgX;
//					DiffAvgZ=g_FiltZ[i16IndxI]-AvgZ;
//
//					//�޳���ǰ���� Ҫ��Ϊ�õ�X��Z�����ܱߵ���ܴ�
//					if ((ABS(DiffAvgX)>300&&DiffAvgZ>500)||
//						(ABS(DiffAvgX)>500&&DiffAvgZ>300)/*||
//														 DiffAvgX<-600||DiffAvgZ>1000*/)
//					{
//						g_FiltZ[i16IndxI]=0;
//						g_FiltX[i16IndxI]=0;
//					}
//				}
//			}
//		}
//
//		if (g_FiltZ[i16IndxI])
//		{
//			//��¼���һ���Ǵ�ɵ����
//			i16LastI=i16IndxI;
//		}
//	}
//}

/************************************************************************/
/* ֡����                                                           */
/************************************************************************/
void FramAnlsye()
{
	uint16 u16Index=0;
	int32 iTmpMaxX=0;
	int32 iTmpMinX=0xFFFF;
	uint32 TmpSumX=0;
	//��ʼ��֡��¼��
	CurFramInfo.u16MaxHt=0;
	CurFramInfo.u16SecHt=0;
	CurFramInfo.n32xMin=0xFFFF;
	CurFramInfo.n32xMax=0;
	CurFramInfo.u32TimeStamp=LMS_data_1[POINT_SUM];
	//��һ�α���  Ѱ�ҳ���ɨ���  �����߶�����ʼ��
	for( u16Index = Mid_Point_Cal; u16Index>1; u16Index--)
	{
		//���ڳ���ʶ�� Ѱ�ҳ�������ɨ���
		if (g_FiltX[u16Index] > Smp.StartX-Smp.IsLanWd &&  g_FiltX[u16Index] <Smp.StartX+Smp.LaneWidth)
		{
			if (g_FiltZ[u16Index] > VEHTH_LOW  &&  g_FiltZ[u16Index]<VEHTH_UP  &&  
				((g_FiltX[u16Index]<Smp.StartX  &&  g_FiltZ[u16Index] > Smp.IsLanHt)||(g_FiltX[u16Index]>Smp.StartX)))
			{
#ifdef _KeilEnVi 				
				if(g_FiltZ[u16Index]<MIN(g_VehOutSet.u16GratHt,3000))
				{
					CurFramInfo.u16GrFitCnt++;
				}
#endif				
				if(CurFramInfo.u16EndPos == 0)
				{
					//�ҵ�������ֹ��
					CurFramInfo.u16StaPos=u16Index;
					CurFramInfo.u16EndPos=u16Index;
				}
				else
				{
					//������ʼ��
					CurFramInfo.u16StaPos=u16Index;
				}
				
				if (CurFramInfo.u16MaxHt<g_FiltZ[u16Index])
				{
					//������ߵ� 
					CurFramInfo.u16SecHt=CurFramInfo.u16MaxHt;
					CurFramInfo.u16MaxHt=g_FiltZ[u16Index];
					CurFramInfo.u16MaxHtPt=u16Index;
					
				}
				else if (CurFramInfo.u16SecHt<g_FiltZ[u16Index])
				{
					CurFramInfo.u16SecHt=g_FiltZ[u16Index];
				}
				
				
				if (g_FiltX[u16Index]>CurFramInfo.n32xMax)
				{
					//������Զ��
					CurFramInfo.n32xMax=g_FiltX[u16Index];
					CurFramInfo.u16xMaxPt=u16Index;
				}
				
				if (g_FiltX[u16Index]<CurFramInfo.n32xMin)
				{
					//���������
					CurFramInfo.n32xMin=g_FiltX[u16Index];
					CurFramInfo.u16xMinPt=u16Index;
				}
				
				TmpSumX+=g_FiltX[u16Index];
				
				if (g_FiltZ[u16Index]>500)
				{
					if (g_FiltX[u16Index]<iTmpMinX)
					{
						iTmpMinX=g_FiltX[u16Index];
					}
					if (g_FiltX[u16Index]>iTmpMaxX)
					{
						iTmpMaxX=g_FiltX[u16Index];
					}
					CurFramInfo.u16Width=iTmpMaxX-iTmpMinX;
				}
				CurFramInfo.u16PtSum++;
				
				if (g_FiltX[u16Index]>Smp.StartX)
				{
					if (g_FiltZ[u16Index]>1000)
					{
						CurFramInfo.u8FitMid=1;
					}
					else if (g_FiltZ[u16Index]<350)
					{
						CurFramInfo.u8FitLow=1;
					}
				}
			}
			else
			{
				//�������͵� ��Ϊ-1 ������
				//g_FiltZ[u16Index]=-1;
			}
		}
		else
		{
			//���ڷ�Χ�ڵĵ�ֱ��ȥ������ֹƽ�澵�������ɢ�� 20170729
			g_FiltZ[u16Index]=0;
			g_FiltX[u16Index]=0;
		}
		
		
	}
	
	//�жϸ������Ƿ�Ϊ�г��Ϸ�����
	if (CurFramInfo.u16PtSum>2 && CurFramInfo.u16MaxHt>300)
	{
		CurFramInfo.u8Valid=1;
		CurFramInfo.u16AvgX=TmpSumX/CurFramInfo.u16PtSum;
#ifdef _KeilEnVi 		
		if(CurFramInfo.u16GrFitCnt>8)
		{
			g_VehicleSet.DirSet.u8GR_Valided=1;
			g_VehicleSet.DirSet.u16GR_FitFrCnt++;
		}
		g_VehicleSet.DirSet.u8CL_Valided=1;
#endif		
	}
	else
	{
#ifdef _KeilEnVi 		
		g_VehicleSet.DirSet.u8GR_Valided=0;
		g_VehicleSet.DirSet.u8CL_Valided=0;
#endif		
	}
}


/************************************************************************/
/* ������ƥ��                                                         */
/************************************************************************/
void VehRegionMatch()
{
	if (CurFramInfo.u8Valid == 1)
	{
		if (g_VehicleSet.u8State!=NO_USED)
		{
			if (IsRegionMatch(CurFramInfo,g_VehicleSet.CurFrmInfo))
			{
				CurFramInfo.u8Matched=1;
				g_VehicleSet.EndWaveIndex=g_n32WaveNumber;
				UpdateVehRegion(&g_VehicleSet);
				SwCarSpiltProc(0,g_VehIndex.u32Index+1);//������־
			}
		}
		
		if (CurFramInfo.u8Matched == 0)
		{
			//������δƥ��ɹ�����Ϊ���³�
			if(g_VehicleSet.u8State  ==  NO_USED)
			{
				g_VehicleSet.u8State = OCCURING_USED;
				g_VehicleSet.StaWaveIndex=g_n32WaveNumber;
				g_VehicleSet.EndWaveIndex=g_n32WaveNumber;
				CurFramInfo.u8Matched=1;
				SwCarSpiltProc(1,g_VehIndex.u32Index+1);//�³���־
				VehState(0);//0--���������պý���;
				UpdateVehRegion(&g_VehicleSet);
			}
		}
	}
}

/************************************************************************/
/* ��������Ƿ�ƥ��                                                     */
/* ������Ĭ��ƥ��ɹ�    �˴������������λ��Լ�����ڷֳ�������         */
/************************************************************************/
uint8 IsRegionMatch(FramInfo FI1,FramInfo FI2)
{
	//ͬһ���� Ĭ��ƥ��ɹ�  �˴������������λ��Լ�����ڷֳ�������
	if (IS_INSIDE(FI1.n32xMin,FI1.n32xMax,FI2.n32xMin,FI2.n32xMax))
	{
		//�����ص�
		return 1;
	}
	else if (abs((FI1.n32xMax+FI1.n32xMin)-(FI2.n32xMax+FI2.n32xMin))<800)
	{
		//�������ĵ���400����
		return 1;
	}
	else if ((FI1.n32xMax<FI2.n32xMin&&FI2.n32xMin-FI1.n32xMax<400)
		||(FI2.n32xMax<FI1.n32xMin&&FI1.n32xMin-FI2.n32xMax<400))
	{
		//����߽���400����
		return 1;
	}
	else
	{
		return 1;
	}
}

#ifdef _KeilEnVi
/************************************************************************/
/* ��֪�����н�����                                                     */
/************************************************************************/
void VehDirCheck(void)
{
	
	if(g_VehicleSet.u8State!=NO_USED)
	{
		switch(g_SysSetUp.u8DirDevType)
		{
		case DEV_COIL:
			Dev_CoilDirCheck();
			break;
		case DEV_GRAT:
			Dev_GratDirCheck();
			break;
		case DEV_LASE:
			Dev_LaseDirCheck();
			break;
		default:
			break;
		}
	}
}

/************************************************************************/
/* ��Ȧ��֪�������                                                     */
/************************************************************************/
void Dev_CoilDirCheck(void)
{
	
}

/************************************************************************/
/* ��դ��֪�������                                                     */
/************************************************************************/
void Dev_GratDirCheck(void)
{
	
}

#endif


/************************************************************************/
/* �����֪�������                                                     */
/************************************************************************/
void Dev_LaseDirCheck(void)
{
	
}



/************************************************************************/
/* ǿ�Ƴ�������		                                                    */
/************************************************************************/
void ForceCloseProc(void)
{
	uint8 JudgeType=0;
	
	if(g_DllAtState.ManHanded||g_DllAtState.ForceVout)
	{
		
		g_DllAtState.ManHanded=0;
		g_DllAtState.ForceVout=0;
		
		JudgeType=ForceVehJudge(&g_VehicleSet);
		
		if(JudgeType>0)
		{
			g_VehicleSet.u8VoutFlag=1;
			ReportForceVeh(&g_VehicleSet);
			
		}
		else
		{
			//��յ�ǰ������¼
			g_VehicleSet.u8VoutFlag=2;
		}	
	}
}

/************************************************************************/
/* �жϵ�ǰ�����Ƿ���Ա�ǿ�Ƴ���			                            */
/************************************************************************/
uint8 CanBeForceOut(VehicleSet *pVehicle)
{
	if(pVehicle->AxleSet.AxleSum>TWO_AXLE)
	{
		//3��
		return 1;
	}
	else if(pVehicle->AxleSet.AxleSum>ONE_AXLE)
	{
		//���㳵��
		VehHeightCheck(pVehicle);
		if(pVehicle->AxleSet.Axle[1].AxleType==2
			&&pVehicle->EdgeSet.u16VehHeight>2300)
		{
			//2�� 12+����>2300
			return 1;
		}
	}
	return 0;
}

/************************************************************************/
/* ǿ�Ƴ��������ж�	  �ж���254                                         */
/************************************************************************/
uint8  ForceVehJudge(VehicleSet *pVehicle)
{
	uint8 l_u8Retalue = 0;
	uint8 l_u8count = 0;
	
	if (pVehicle == NULL)
		return 0;
	
	if(CanBeForceOut(pVehicle))
	{
		pVehicle->AxleSet.AxleSum = SIX_AXLE;
		for(l_u8count = 2; l_u8count < 6; l_u8count ++ )
		{
			pVehicle->AxleSet.Axle[l_u8count].AxleType = 0x02;
		}
		l_u8Retalue = FIFTH_HUOCHE;
	}
	else
	{
		return 0;
	}
	
	pVehicle->JudgeSet.u8JudgePattern=FIFTH_HUOCHE;
	pVehicle->JudgeSet.u8JudgeRule=254;
	pVehicle->Dp.DpHeight1_2=0;
	
	//ר�����
	ArcTruckCheck(pVehicle);//���峵ϸ��
	
	return l_u8Retalue;
}

/************************************************************************/
/* ����ǿ�Ƴ�������			                                            */
/************************************************************************/
void ReportForceVeh(VehicleSet *pVehicle)
{
	
	int32 i=0;
	VehSet TmpVeh;
	memset(&TmpVeh,0,sizeof(TmpVeh));
	
	
	TmpVeh.VoutFlag=pVehicle->u8VoutFlag;
	g_VehIndex.u32Index++;
	TmpVeh.VehId=g_VehIndex.u32Index&0xFF;//�����
	TmpVeh.VPattern=pVehicle->JudgeSet.u8JudgePattern;//����
	TmpVeh.VDir=1;//����
	TmpVeh.AxleNum=pVehicle->AxleSet.AxleSum;//����
	
	if(pVehicle->AxleSet.Axle[1].AxleType== 0x01 )
		TmpVeh.TyreCnt = 20;	 //����
	else
		TmpVeh.TyreCnt = 22;	 //����	
	
	//��������
	for (i=0;i<TmpVeh.AxleNum;i++)
	{
		TmpVeh.AxleType[i]=pVehicle->AxleSet.Axle[i].AxleType;
	}
	
	//�ٶ�
	TmpVeh.VSpeed=0;
	//������Ϣ
	TmpVeh.VJudCode=pVehicle->JudgeSet.u8JudgeRule;
	TmpVeh.VAdjCode=pVehicle->JudgeSet.u8ModifyRule;
	TmpVeh.VSpeCode=pVehicle->JudgeSet.u8SpeVehCode;
	
	TmpVeh.Cargo=pVehicle->FeatureSet.IsCargo;
	TmpVeh.CastGrd=pVehicle->FeatureSet.IsCastGrd;
	TmpVeh.Container=pVehicle->FeatureSet.IsContainer;
	TmpVeh.FillWin=pVehicle->FeatureSet.IsFillWin;
	
	memcpy(TmpVeh.FoldName,g_u8ItemName,24);
	
	TmpVeh.GlassWin=pVehicle->FeatureSet.IsGlassWin;
	TmpVeh.HeadJump=pVehicle->FeatureSet.IsHeadJump;
	TmpVeh.Hopper=pVehicle->FeatureSet.IsHopper;
	TmpVeh.SideUiform=pVehicle->FeatureSet.IsSideUiform;
	TmpVeh.TailJump=pVehicle->FeatureSet.IsTailJump;
	TmpVeh.TruckTail=pVehicle->FeatureSet.IsTruckTail;
	TmpVeh.VehHeadHt=pVehicle->EdgeSet.u16HeadHeight;
	TmpVeh.VehHeight=pVehicle->EdgeSet.u16VehHeight;
	TmpVeh.VDpHt=0;
	ReportVh(TmpVeh);
	
}


/************************************************************************/
/* ������                                                             */
/************************************************************************/
void ClearVehicleErr()
{
	
}

/************************************************************************/
/* ���³���������Ϣ                                                     */
/************************************************************************/
void UpdateVehRegion(VehicleSet *pVehicle)
{
	//��������
	uint16 l_u16PtCnt=0;
	memcpy(&pVehicle->CurFrmInfo,&CurFramInfo,sizeof(CurFramInfo));
	memset(&pVehicle->CurRegion,0,sizeof(RegionSet));
	
	
	if (CurFramInfo.u16EndPos-CurFramInfo.u16StaPos+1>FRAME_BUFLEN)
	{
		l_u16PtCnt=FRAME_BUFLEN;
	}
	else
	{
		l_u16PtCnt=CurFramInfo.u16EndPos-CurFramInfo.u16StaPos+1;
	}
	
	//�������һ֡����
	memcpy(&pVehicle->CurRegion.xdata[1],&g_FiltX[CurFramInfo.u16StaPos],l_u16PtCnt*sizeof(int));
	pVehicle->CurRegion.xdata[0]=l_u16PtCnt;
	memcpy(&pVehicle->CurRegion.zdata[1],&g_FiltZ[CurFramInfo.u16StaPos],l_u16PtCnt*sizeof(int));
	pVehicle->CurRegion.zdata[0]=l_u16PtCnt;
	pVehicle->CurRegion.xMax=CurFramInfo.n32xMax;
	pVehicle->CurRegion.xMin=CurFramInfo.n32xMin;
	pVehicle->CurRegion.zMax=CurFramInfo.u16MaxHt;
	pVehicle->EdgeSet.u16MaxHeight[pVehicle->u16FrameCnt]=CurFramInfo.u16MaxHt;
	pVehicle->EdgeSet.u16SecHeight[pVehicle->u16FrameCnt]=CurFramInfo.u16SecHt;
	pVehicle->EdgeSet.u16MinXEdge[pVehicle->u16FrameCnt]=CurFramInfo.n32xMin;
	
	//�˴�����ȷ�������������͵�Ҫ�� ����Ϊ������
	if (CurFramInfo.u8FitLow)
	{
		pVehicle->u8FitLow=1;
	}
	if (CurFramInfo.u8FitMid)
	{
		pVehicle->u8FitMid=1;
	}
	if (pVehicle->u16LeftX==0)
	{
		pVehicle->u16LeftX=CurFramInfo.n32xMin;
	}
	else if (pVehicle->u16LeftX>CurFramInfo.n32xMin)
	{
		pVehicle->u16LeftX=CurFramInfo.n32xMin;
	}

	if (pVehicle->u16RightX<CurFramInfo.n32xMax)
	{
		pVehicle->u16RightX=CurFramInfo.n32xMax;
	}

	if (CurFramInfo.n32xMin+800<CurFramInfo.u16AvgX)
	{
		//��ֹɢ�� ��ɱ߽�ͶӰ�쳣 20170725
		if (pVehicle->u16FrameCnt<1)
		{
			pVehicle->CurRegion.xMin=CurFramInfo.u16AvgX;
			pVehicle->EdgeSet.u16MinXEdge[pVehicle->u16FrameCnt]=CurFramInfo.u16AvgX;
		}
		else
		{
			pVehicle->CurRegion.xMin=pVehicle->EdgeSet.u16MinXEdge[pVehicle->u16FrameCnt-1];
			pVehicle->EdgeSet.u16MinXEdge[pVehicle->u16FrameCnt]=pVehicle->EdgeSet.u16MinXEdge[pVehicle->u16FrameCnt-1];
		}
	}
	
	if (pVehicle->EdgeSet.u16MinX == 0)
	{
		pVehicle->EdgeSet.u16MinX=CurFramInfo.n32xMin;
		pVehicle->EdgeSet.u16MinXIndex=pVehicle->u16FrameCnt;
	}
	else if (pVehicle->EdgeSet.u16MinX>CurFramInfo.n32xMin)
	{
		pVehicle->EdgeSet.u16MinX=CurFramInfo.n32xMin;
		pVehicle->EdgeSet.u16MinXIndex=pVehicle->u16FrameCnt;
	}
	
	if (pVehicle->EdgeSet.u16Width == 0)
	{
		pVehicle->EdgeSet.u16Width=CurFramInfo.u16Width;
	}
	else if (pVehicle->EdgeSet.u16Width<CurFramInfo.u16Width)
	{
		pVehicle->EdgeSet.u16Width=CurFramInfo.u16Width;
	}
	
#ifdef _DEBUG
	//����������������
	if (pVehicle->u16FrameCnt < FRAME_MASK )
	{
		memcpy(&pVehicle->PC[pVehicle->u16FrameCnt],&pVehicle->CurRegion,sizeof(RegionSet));
	}
	else
	{
		memcpy(&pVehicle->PC[FRAME_MAXCNT],&pVehicle->CurRegion,sizeof(RegionSet));
	}
#endif
	
	if (g_pfFormImage!=NULL)
	{
		g_pfFormImage(pVehicle->u16FrameCnt,pVehicle->CurRegion.xdata,pVehicle->CurRegion.zdata,pVehicle->CurRegion.zMax);
	}
	
	pVehicle->u16FrameCnt++;
	pVehicle->EmptyFrame=0;
	
	//����������ȡ
	FeatureCal(pVehicle);
}


/************************************************************************/
/* ��鳵��״̬                                                         */
/************************************************************************/
void CheckVehState()
{
	if(g_VehicleSet.u8State != NO_USED)
	{
		if(g_VehicleSet.EmptyFrame + 1> g_VehOutSet.u8EmptyFrCnt && g_VehicleSet.u16FrameCnt)
		{ 		
			g_VehicleSet.u8State = PASSED_USED;  //�ѽ���������β�ĳ�	
		}
		
		//�ɼ�����
		IntoWaveCache((char *)JG0_CurBuff,Full_Size);
		
		if(g_VehicleSet.u8State == PASSED_USED) 
		{  
			//�ص�����״̬
			VehState(1);
			//��β���� ���г���ʶ��	
			OutPutVeh(&g_VehicleSet);			
			memset(&g_VehicleSet,0,sizeof(VehicleSet));
			g_VehicleSet.u8State = NO_USED;
		}
		else
		{
			if(g_VehOutSet.u8Ax3Force && g_VehicleSet.u8VoutFlag<1)
			{
				//�����˳�3��ǿ�Ƴ���
				if(g_VehicleSet.AxleSet.AxleSum>3)
				{
					g_DllAtState.ForceVout=1;
				}
			}			
		}
		g_VehicleSet.EmptyFrame++;
	} 	
}

/************************************************************************/
/*IsVehCheck ����Ƿ�Ϊ����                                             */
/*1-�ǳ�����0-���ǳ���		                                            */
/************************************************************************/
uint8 IsVehCheck(VehicleSet *pVehicle)
{
	if (pVehicle->u16FrameCnt<2||pVehicle->EdgeSet.u16VehHeight<500)
	{
		return 0;
	}
	else if(pVehicle->u8FitLow<1||pVehicle->u8FitMid<1)
	{
		return 0;
	}
	else if (pVehicle->u16LeftX*3>(Smp.StartX+Smp.LaneWidth)*2)
	{
		//��λ�޳�����
		return 0;
	}
	else if (pVehicle->u8FitLow<1&&pVehicle->u16LeftX<Smp.StartX
		&&pVehicle->u16RightX<Smp.StartX)
	{
		//��λ�޳�����
		return 0;
	}
	else
	{
		return 1;
	}
}


/************************************************************************/
/* ��������                                                             */
/************************************************************************/
void OutPutVeh(VehicleSet *pVehicle)
{
	VehIdentifier(pVehicle);
	
	AxleClearGlobal();
	
	if (IsVehCheck(pVehicle))
	{
		
		if(pVehicle->u8VoutFlag!=1)
		{
			ReportNormalVeh(pVehicle);
			
		}
	}
	
}

/************************************************************************/
/* �ϴ�������Ϣ  		                                                */
/************************************************************************/
void ReportNormalVeh(VehicleSet *pVehicle)
{
	
	int32 i=0;
	VehSet TmpVeh;
	memset(&TmpVeh,0,sizeof(TmpVeh));
	
	
	TmpVeh.VoutFlag=pVehicle->u8VoutFlag;
	g_VehIndex.u32Index++;
	TmpVeh.VehId=g_VehIndex.u32Index;//�����
	TmpVeh.VPattern=pVehicle->JudgeSet.u8JudgePattern;//����
	TmpVeh.VDir=pVehicle->DirSet.u8VehDir;//����
	TmpVeh.AxleNum=pVehicle->AxleSet.AxleSum;//����
	
	//��������
	for (i=0;i<TmpVeh.AxleNum;i++)
	{
		TmpVeh.AxleType[i]=pVehicle->AxleSet.Axle[i].AxleType;
		TmpVeh.TyreCnt+=pVehicle->AxleSet.Axle[i].AxleType*2;
	}
	
	//�ٶ�
	TmpVeh.VSpeed=0;
	//������Ϣ
	TmpVeh.VJudCode=pVehicle->JudgeSet.u8JudgeRule;
	TmpVeh.VAdjCode=pVehicle->JudgeSet.u8ModifyRule;
	TmpVeh.VSpeCode=pVehicle->JudgeSet.u8SpeVehCode;
	
	TmpVeh.Cargo=pVehicle->FeatureSet.IsCargo;
	TmpVeh.CastGrd=pVehicle->FeatureSet.IsCastGrd;
	TmpVeh.Container=pVehicle->FeatureSet.IsContainer;
	TmpVeh.FillWin=pVehicle->FeatureSet.IsFillWin;
	
	memcpy(TmpVeh.FoldName,g_u8ItemName,24);
	
	TmpVeh.GlassWin=pVehicle->FeatureSet.IsGlassWin;
	TmpVeh.HeadJump=pVehicle->FeatureSet.IsHeadJump;
	TmpVeh.Hopper=pVehicle->FeatureSet.IsHopper;
	TmpVeh.SideUiform=pVehicle->FeatureSet.IsSideUiform;
	TmpVeh.TailJump=pVehicle->FeatureSet.IsTailJump;
	TmpVeh.TruckTail=pVehicle->FeatureSet.IsTruckTail;
	TmpVeh.VehHeadHt=pVehicle->EdgeSet.u16HeadHeight;
	TmpVeh.VehHeight=pVehicle->EdgeSet.u16VehHeight;
	TmpVeh.VDpHt=pVehicle->Dp.DpHeight1_2;
	ReportVh(TmpVeh);
	
	
}




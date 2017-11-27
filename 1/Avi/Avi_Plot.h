#ifndef _AVI_PLOT_H_
#define _AVI_PLOT_H_


#pragma once
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "User32.lib")


#define MaxRow 30000
#define MaxCow 256

typedef struct tagPlotCache
{
	int RowCnt;
	int MaxH;
	int X_Coor[MaxRow][MaxCow];
	int Z_Coor[MaxRow][MaxCow];
}PlotCache;



#define ERR_SUCC 1
#define ERR_FAIL 0


//Draw avi Err Code Table  start from 401-501
#define ERR_PLOTLOGI 401  //�߼�����
#define ERR_PLOTSIZE 402  //ͼ��ߴ��С
#define ERR_PLOTHDC0 403  //HDC����ʧ��
#define ERR_PLOTHBMP 404  //BitMap�����un���Ӱ���ʧ��
#define ERR_PLOTGDIO 405  //GDI����ѡ��ʧ��
#define ERR_PLOTPFID 406  //���ظ�ʽѡ�����
#define ERR_PLOTSPFI 407  //����DC���ظ�ʽʧ��
#define ERR_PLOTGLRC 408  //Opengl��ͼ��Դ����ʧ��
#define ERR_PLOTCRBM 409 //����BMP�ļ�ʧ��
#define ERR_PLHDNULL 410 //�������Ϊ��
#define ERR_PLOTNULL 411 



#define MIN_WIDTH 100
#define MIN_HEIGHT 100



/************************************************************************/
/* ��ʼ����ͼ����                                                       */
/************************************************************************/
int __stdcall PlotInit(int nWidth , int nHeight);


/************************************************************************/
/* ��ͼ����                                                             */
/************************************************************************/
int __stdcall PlotTest(int angle);



/************************************************************************/
/* �洢ͼ��                                                             */
/************************************************************************/
int __stdcall SaveBmp(char * FileName);



/************************************************************************/
/* �������ͼ��                                                         */
/************************************************************************/
int __stdcall ClearPlot();

/************************************************************************/
/* �ͷŻ�ͼ��������Դ                                                   */
/************************************************************************/
int __stdcall PlotDispose();





/************************************************************************/
/* ��ȡ���һ�γ�����                                                   */
/************************************************************************/
int __stdcall GetPlotLastErr(void);


/************************************************************************/
/* ���뵱ǰɨ��������                                                   */
/************************************************************************/
int __stdcall IntoRows(int FrIdx,int *pX,int *pZ,int MaxH);


/************************************************************************/
/* ���Ƴ���ͼƬ		                                                    */
/************************************************************************/
int __stdcall PlotVeh(void);


#endif
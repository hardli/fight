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
#define ERR_PLOTLOGI 401  //逻辑错误
#define ERR_PLOTSIZE 402  //图像尺寸过小
#define ERR_PLOTHDC0 403  //HDC创建失败
#define ERR_PLOTHBMP 404  //BitMap对象差un更加爱你失败
#define ERR_PLOTGDIO 405  //GDI对象选择失败
#define ERR_PLOTPFID 406  //像素格式选择出错
#define ERR_PLOTSPFI 407  //设置DC像素格式失败
#define ERR_PLOTGLRC 408  //Opengl绘图资源申请失败
#define ERR_PLOTCRBM 409 //创建BMP文件失败
#define ERR_PLHDNULL 410 //创建句柄为空
#define ERR_PLOTNULL 411 



#define MIN_WIDTH 100
#define MIN_HEIGHT 100



/************************************************************************/
/* 初始化画图环境                                                       */
/************************************************************************/
int __stdcall PlotInit(int nWidth , int nHeight);


/************************************************************************/
/* 画图测试                                                             */
/************************************************************************/
int __stdcall PlotTest(int angle);



/************************************************************************/
/* 存储图像                                                             */
/************************************************************************/
int __stdcall SaveBmp(char * FileName);



/************************************************************************/
/* 清除点云图像                                                         */
/************************************************************************/
int __stdcall ClearPlot();

/************************************************************************/
/* 释放画图环境及资源                                                   */
/************************************************************************/
int __stdcall PlotDispose();





/************************************************************************/
/* 获取最后一次出错码                                                   */
/************************************************************************/
int __stdcall GetPlotLastErr(void);


/************************************************************************/
/* 插入当前扫描行数据                                                   */
/************************************************************************/
int __stdcall IntoRows(int FrIdx,int *pX,int *pZ,int MaxH);


/************************************************************************/
/* 绘制车辆图片		                                                    */
/************************************************************************/
int __stdcall PlotVeh(void);


#endif
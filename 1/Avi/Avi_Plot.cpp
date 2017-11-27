// Avi_Plot.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Avi_Plot.h"
#include <gl/glut.h>

int WIDTH = 250;//绘图环境的宽度(图片的宽)
int HEIGHT = 250;//绘图环境的高度(图片的高)
int ErrCode = 0; //错误码
HDC hdc=0;//绘图HDC
HBITMAP hbm=0;//bitmap
HGDIOBJ r=0;//GDI对象
HGLRC hglrc=0;//Opengl绘图资源


bool FirstRun=true;
PlotCache g_Plot;
/************************************************************************/
/* 获取最后一次出错码                                                   */
/************************************************************************/
int __stdcall GetPlotLastErr()
{
	return ErrCode;
}


/************************************************************************/
/* 初始化画图环境                                                       */
/************************************************************************/
int __stdcall PlotInit(int nWidth , int nHeight)
{
	
	if (nWidth<MIN_WIDTH||nHeight<MIN_HEIGHT)
	{
		ErrCode=ERR_PLOTSIZE;
		return ErrCode;
	}
	else
	{
		WIDTH = nWidth;
		HEIGHT = nHeight;
	}
	
	// Create a memory DC compatible with the screen
	if (hdc!=0)
	{
		DeleteDC(hdc);
	}
	hdc = CreateCompatibleDC(0);
	if (hdc == 0) 
	{
		ErrCode=ERR_PLOTHDC0;//创建内存HDC失败 Could not create memory device context
		return ErrCode;
	}
	
	
	// Create a bitmap compatible with the DC
	// must use CreateDIBSection(), and this means all pixel ops must be synchronised
	// using calls to GdiFlush() (see CreateDIBSection() docs)
	BITMAPINFO bmi = {
		{ sizeof(BITMAPINFOHEADER), WIDTH, HEIGHT, 1, 32, BI_RGB, 0, 0, 0, 0, 0 },
		{ 0 }
	};
	
	DWORD *pbits; // pointer to bitmap bits
	if (hbm!=0)
	{
		DeleteObject(hbm); // Delete bitmap
	}
	hbm = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void **) &pbits,
		0, 0);
	
	if (hbm == 0)
	{
		ErrCode=ERR_PLOTHBMP;//"Could not create bitmap";
		return ErrCode;
		
	}
	
	// Select the bitmap into the DC
	r = SelectObject(hdc, hbm);
	if (r == 0)
	{ 
		ErrCode=ERR_PLOTGDIO;//"Could not select bitmap into DC";
		return ErrCode;
		
	}
	
	// Choose the pixel format
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof (PIXELFORMATDESCRIPTOR), // struct size
			1, // Version number
			PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL, // use OpenGL drawing to BM
			PFD_TYPE_RGBA, // RGBA pixel values
			32, // color bits
			0, 0, 0, // RGB bits shift sizes...
			0, 0, 0, // Don't care about them
			0, 0, // No alpha buffer info
			0, 0, 0, 0, 0, // No accumulation buffer
			32, // depth buffer bits
			0, // No stencil buffer
			0, // No auxiliary buffers
			PFD_MAIN_PLANE, // Layer type
			0, // Reserved (must be 0)
			0, // No layer mask
			0, // No visible mask
			0 // No damage mask
	};
	
	int pfid = ChoosePixelFormat(hdc, &pfd);
	if (pfid == 0) 
	{
		ErrCode=ERR_PLOTGDIO;//"Pixel format selection failed";
		return ErrCode;
	}
	
	// Set the pixel format
	// - must be done *after* the bitmap is selected into DC
	BOOL b = SetPixelFormat(hdc, pfid, &pfd);
	if (!b) 
	{
		ErrCode=ERR_PLOTGDIO;//"Pixel format set failed";
		return ErrCode;
	}
	
	// Create the OpenGL resource context (RC) and make it current to the thread
	if (hglrc!=0)
	{
		wglDeleteContext(hglrc); // Delete RC
	}
	hglrc = wglCreateContext(hdc);
	if (hglrc == 0) 
	{
		ErrCode=ERR_PLOTGLRC;//"OpenGL resource context creation failed";
		return ErrCode;
	}
	wglMakeCurrent(hdc, hglrc);//Make Hglrc as Current hdc
	
	FirstRun=true;
	
	return ERR_SUCC;
}

/************************************************************************/
/* 画图测试                                                             */
/************************************************************************/
int __stdcall PlotTest(int angle)
{
	glClearColor(0.3,0.7,0.5,0);  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //清理颜色和深度缓存       
	
	// 创建透视效果视图        
	glMatrixMode(GL_PROJECTION);     
	glLoadIdentity();      
	gluPerspective(80.0f, 1.0f, 1.0f, 20.0f);     
	
	glMatrixMode(GL_MODELVIEW);     
	glLoadIdentity();     
	gluLookAt(0.0, 12.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);         
	
	// 定义太阳光源，它是一种白色的光源     
	{      
		GLfloat sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f}; //光源的位置在世界坐标系圆心，齐次坐标形式  
		GLfloat sun_light_ambient[]   = {0.0f, 0.0f, 0.0f, 1.0f}; //RGBA模式的环境光，为0  
		GLfloat sun_light_diffuse[]   = {1.0f, 1.0f, 1.0f, 1.0f}; //RGBA模式的漫反射光，全白光  
		GLfloat sun_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};  //RGBA模式下的镜面光 ，全白光  
		glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);       
		glLightfv(GL_LIGHT0, GL_AMBIENT,   sun_light_ambient);    
		glLightfv(GL_LIGHT0, GL_DIFFUSE,   sun_light_diffuse);     
		glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);       
		
		//开启灯光  
		glEnable(GL_LIGHT0);      
		glEnable(GL_LIGHTING);     
		glEnable(GL_DEPTH_TEST);    
	}  
	
	// 定义太阳的材质并绘制太阳      
	{         
		GLfloat sun_mat_ambient[]   = {0.0f, 0.0f, 0.0f, 1.0f};  //定义材质的环境光颜色，为0  
		GLfloat sun_mat_diffuse[]   = {0.0f, 0.0f, 0.0f, 1.0f};  //定义材质的漫反射光颜色，为0  
		GLfloat sun_mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};   //定义材质的镜面反射光颜色，为0  
		GLfloat sun_mat_emission[] = {0.8f, 0.0f, 0.0f, 1.0f};   //定义材质的辐射广颜色，为偏红色  
		GLfloat sun_mat_shininess   = 0.0f;          
		glMaterialfv(GL_FRONT, GL_AMBIENT,    sun_mat_ambient);      
		glMaterialfv(GL_FRONT, GL_DIFFUSE,    sun_mat_diffuse);     
		glMaterialfv(GL_FRONT, GL_SPECULAR,   sun_mat_specular);      
		glMaterialfv(GL_FRONT, GL_EMISSION,   sun_mat_emission);     
		glMaterialf (GL_FRONT, GL_SHININESS, sun_mat_shininess);      
		glutSolidSphere(3.0, 40, 32);      
	}  
	
	// 定义地球的材质并绘制地球      
	{          
		GLfloat earth_mat_ambient[]   = {0.0f, 0.0f, 1.0f, 1.0f};  //定义材质的环境光颜色，骗蓝色  
		GLfloat earth_mat_diffuse[]   = {0.0f, 0.0f, 0.5f, 1.0f};  //定义材质的漫反射光颜色，偏蓝色  
		GLfloat earth_mat_specular[] = {1.0f, 0.0f, 0.0f, 1.0f};   //定义材质的镜面反射光颜色，红色  
		GLfloat earth_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};   //定义材质的辐射光颜色，为0  
		GLfloat earth_mat_shininess   = 30.0f;         
		glMaterialfv(GL_FRONT, GL_AMBIENT,    earth_mat_ambient);     
		glMaterialfv(GL_FRONT, GL_DIFFUSE,    earth_mat_diffuse);      
		glMaterialfv(GL_FRONT, GL_SPECULAR,   earth_mat_specular);       
		glMaterialfv(GL_FRONT, GL_EMISSION,   earth_mat_emission);     
		glMaterialf (GL_FRONT, GL_SHININESS, earth_mat_shininess);      
		glRotatef(angle*1.0f, 0.0f, -1.0f, 0.0f);         
		glTranslatef(7.0f, 0.0f, 0.0f);          
		glutSolidSphere(3.0, 40, 32);       
	}       
	glEnd();
	glFlush(); // remember to flush GL output!
	
	return ERR_SUCC;
}

/************************************************************************/
/* 存储图像                                                             */
/************************************************************************/
int __stdcall SaveBmp(char * FileName)
{
	HDC hDC;
	//当前分辨率下每象素所占字节数
	int iBits;
	//位图中每象素所占字节数
	WORD wBitCount;
	//定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数
	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;
	//位图属性结构
	BITMAP Bitmap;
	//位图文件头结构
	BITMAPFILEHEADER bmfHdr;
	//位图信息头结构
	BITMAPINFOHEADER bi;
	//指向位图信息头结构
	LPBITMAPINFOHEADER lpbi;
	//定义文件，分配内存句柄，调色板句柄
	HANDLE fh, hDib, hPal,hOldPal=NULL;
	
	//计算位图文件每个像素所占字节数
	hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1) wBitCount = 1;
	else if (iBits <= 4) wBitCount = 4;
	else if (iBits <= 8) wBitCount = 8;
	else wBitCount = 24;
	
	GetObject(hbm, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;
	
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
	
	//为位图内容分配内存
	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	
	// 处理调色板
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}
	
	if (hDC==NULL||hbm==NULL)
	{
		ErrCode=ERR_PLHDNULL;
		return ErrCode;
	}
	
	
	
	// 获取该调色板下新的像素值
	GetDIBits(hDC, hbm, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);
	
	//恢复调色板
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	
	//创建位图文件
	fh = CreateFile(FileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	
	
	if (fh == INVALID_HANDLE_VALUE)
	{
		ErrCode=ERR_PLOTCRBM;
		return ERR_PLOTCRBM;
	}
	
	// 设置位图文件头
	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	// 写入位图文件头
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	// 写入位图文件其余内容
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//清除
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	
	return ERR_SUCC;
}

/************************************************************************/
/* 释放画图环境及资源                                                   */
/************************************************************************/
int __stdcall PlotDispose()
{
	// Clean up
	wglDeleteContext(hglrc); // Delete RC
	SelectObject(hdc, r); // Remove bitmap from DC
	DeleteObject(hbm); // Delete bitmap
	DeleteDC(hdc); // Delete DC
	//InitCacheImg();
	FirstRun=true;
	return ERR_SUCC;
}

/************************************************************************/
/* 清除点云图像                                                         */
/************************************************************************/
int __stdcall ClearPlot()
{
	memset(&g_Plot,0,sizeof(g_Plot));
	return ERR_SUCC;
}


/************************************************************************/
/* 插入当前扫描行数据                                                   */
/************************************************************************/
int __stdcall IntoRows(int FrIdx,int *pX,int *pZ,int MaxH)
{
	
	if (FrIdx<1)
	{
		ClearPlot();
		FirstRun=false;
	}
	
	if (g_Plot.RowCnt >= MaxRow)
	{
		ClearPlot();
	}
	else
	{
		g_Plot.RowCnt = g_Plot.RowCnt + 1;
		memcpy(g_Plot.X_Coor[FrIdx],pX,MaxCow*sizeof(int));
		memcpy(g_Plot.Z_Coor[FrIdx],pZ,MaxCow*sizeof(int));
		if (g_Plot.MaxH<MaxH)
		{
			g_Plot.MaxH=MaxH;
		}
	}
	return ERR_SUCC;
}


/************************************************************************/
/* 插入当前扫描行数据                                                   */
/************************************************************************/
int __stdcall PlotVeh(void)
{
	int m=0;
	int j=0;
	int factor=20;
	int Y_Coor=0;
	if (g_Plot.RowCnt<1)
	{
		ErrCode=ERR_PLOTNULL;
		return ErrCode;
	}
	else
	{
		
		if (g_Plot.RowCnt<50)
		{
			factor=60;
		}
		else if (g_Plot.RowCnt<100)
		{
			factor=40;
		}
		else if (g_Plot.RowCnt<500)
		{
			factor=20;
		}
		else
		{
			factor=20;
		}
		
		glClearColor(0, 0, 0, 0);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_PROJECTION);
		//  Load the identity matrix.
		glLoadIdentity();
		
		if (g_Plot.MaxH<500)
		{
			gluOrtho2D(-500, g_Plot.RowCnt*factor+500, -500, 2000);
		}
		else
			gluOrtho2D(-500, g_Plot.RowCnt*factor+500, -500, g_Plot.MaxH+500);
		
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glPointSize(1.5f);//1.5 点尺寸
		glBegin(GL_POINTS);
		glColor3f(0.5f, 0.8f, 1.0f);
		glShadeModel(GL_SMOOTH);
		
		for(m=0;m<g_Plot.RowCnt;m++)// Datacnt_Point[j] 一帧点数
		{
			Y_Coor = (g_Plot.RowCnt-m)*factor;//z轴帧间距
			for (j = 1; j < g_Plot.X_Coor[m][0]; j++)
			{
				if(g_Plot.Z_Coor[m][j]<1000) 
					glColor3f(1.0f*(140-g_Plot.Z_Coor[m][j]/10),7*1.0f,150*1.0f);//设置颜色
				else if((g_Plot.Z_Coor[m][j]<2000)) 
					glColor3f(40*1.0f,1.0f*(50+(g_Plot.Z_Coor[m][j]-1000)/20),1.0f*115);
				else if((g_Plot.Z_Coor[m][j]<3000)) 
					glColor3f(1.0f*(60+(g_Plot.Z_Coor[m][j]-2000)/20),1.0f*109,1.0f*43);
				else if((g_Plot.Z_Coor[m][j]<4000)) 
					glColor3f(1.0f*124,1.0f*(125-(g_Plot.Z_Coor[m][j]-3000)/10),1.0f*32);
				else 
					glColor3f(1.0f*250,1.0*(190-(g_Plot.Z_Coor[m][j]-4000)/10),1.0f*32);
				
				glVertex2i(Y_Coor,g_Plot.Z_Coor[m][j]);//画车辆轮廓的最后调用函数
			}
		}
		
		glEnd();
		glFlush(); 
		return ERR_SUCC;
	}
}
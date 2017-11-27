// Avi_Plot.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Avi_Plot.h"
#include <gl/glut.h>

int WIDTH = 250;//��ͼ�����Ŀ��(ͼƬ�Ŀ�)
int HEIGHT = 250;//��ͼ�����ĸ߶�(ͼƬ�ĸ�)
int ErrCode = 0; //������
HDC hdc=0;//��ͼHDC
HBITMAP hbm=0;//bitmap
HGDIOBJ r=0;//GDI����
HGLRC hglrc=0;//Opengl��ͼ��Դ


bool FirstRun=true;
PlotCache g_Plot;
/************************************************************************/
/* ��ȡ���һ�γ�����                                                   */
/************************************************************************/
int __stdcall GetPlotLastErr()
{
	return ErrCode;
}


/************************************************************************/
/* ��ʼ����ͼ����                                                       */
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
		ErrCode=ERR_PLOTHDC0;//�����ڴ�HDCʧ�� Could not create memory device context
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
/* ��ͼ����                                                             */
/************************************************************************/
int __stdcall PlotTest(int angle)
{
	glClearColor(0.3,0.7,0.5,0);  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //������ɫ����Ȼ���       
	
	// ����͸��Ч����ͼ        
	glMatrixMode(GL_PROJECTION);     
	glLoadIdentity();      
	gluPerspective(80.0f, 1.0f, 1.0f, 20.0f);     
	
	glMatrixMode(GL_MODELVIEW);     
	glLoadIdentity();     
	gluLookAt(0.0, 12.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);         
	
	// ����̫����Դ������һ�ְ�ɫ�Ĺ�Դ     
	{      
		GLfloat sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f}; //��Դ��λ������������ϵԲ�ģ����������ʽ  
		GLfloat sun_light_ambient[]   = {0.0f, 0.0f, 0.0f, 1.0f}; //RGBAģʽ�Ļ����⣬Ϊ0  
		GLfloat sun_light_diffuse[]   = {1.0f, 1.0f, 1.0f, 1.0f}; //RGBAģʽ��������⣬ȫ�׹�  
		GLfloat sun_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};  //RGBAģʽ�µľ���� ��ȫ�׹�  
		glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);       
		glLightfv(GL_LIGHT0, GL_AMBIENT,   sun_light_ambient);    
		glLightfv(GL_LIGHT0, GL_DIFFUSE,   sun_light_diffuse);     
		glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);       
		
		//�����ƹ�  
		glEnable(GL_LIGHT0);      
		glEnable(GL_LIGHTING);     
		glEnable(GL_DEPTH_TEST);    
	}  
	
	// ����̫���Ĳ��ʲ�����̫��      
	{         
		GLfloat sun_mat_ambient[]   = {0.0f, 0.0f, 0.0f, 1.0f};  //������ʵĻ�������ɫ��Ϊ0  
		GLfloat sun_mat_diffuse[]   = {0.0f, 0.0f, 0.0f, 1.0f};  //������ʵ����������ɫ��Ϊ0  
		GLfloat sun_mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};   //������ʵľ��淴�����ɫ��Ϊ0  
		GLfloat sun_mat_emission[] = {0.8f, 0.0f, 0.0f, 1.0f};   //������ʵķ������ɫ��Ϊƫ��ɫ  
		GLfloat sun_mat_shininess   = 0.0f;          
		glMaterialfv(GL_FRONT, GL_AMBIENT,    sun_mat_ambient);      
		glMaterialfv(GL_FRONT, GL_DIFFUSE,    sun_mat_diffuse);     
		glMaterialfv(GL_FRONT, GL_SPECULAR,   sun_mat_specular);      
		glMaterialfv(GL_FRONT, GL_EMISSION,   sun_mat_emission);     
		glMaterialf (GL_FRONT, GL_SHININESS, sun_mat_shininess);      
		glutSolidSphere(3.0, 40, 32);      
	}  
	
	// �������Ĳ��ʲ����Ƶ���      
	{          
		GLfloat earth_mat_ambient[]   = {0.0f, 0.0f, 1.0f, 1.0f};  //������ʵĻ�������ɫ��ƭ��ɫ  
		GLfloat earth_mat_diffuse[]   = {0.0f, 0.0f, 0.5f, 1.0f};  //������ʵ����������ɫ��ƫ��ɫ  
		GLfloat earth_mat_specular[] = {1.0f, 0.0f, 0.0f, 1.0f};   //������ʵľ��淴�����ɫ����ɫ  
		GLfloat earth_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};   //������ʵķ������ɫ��Ϊ0  
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
/* �洢ͼ��                                                             */
/************************************************************************/
int __stdcall SaveBmp(char * FileName)
{
	HDC hDC;
	//��ǰ�ֱ�����ÿ������ռ�ֽ���
	int iBits;
	//λͼ��ÿ������ռ�ֽ���
	WORD wBitCount;
	//�����ɫ���С�� λͼ�������ֽڴ�С ��λͼ�ļ���С �� д���ļ��ֽ���
	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;
	//λͼ���Խṹ
	BITMAP Bitmap;
	//λͼ�ļ�ͷ�ṹ
	BITMAPFILEHEADER bmfHdr;
	//λͼ��Ϣͷ�ṹ
	BITMAPINFOHEADER bi;
	//ָ��λͼ��Ϣͷ�ṹ
	LPBITMAPINFOHEADER lpbi;
	//�����ļ��������ڴ�������ɫ����
	HANDLE fh, hDib, hPal,hOldPal=NULL;
	
	//����λͼ�ļ�ÿ��������ռ�ֽ���
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
	
	//Ϊλͼ���ݷ����ڴ�
	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	
	// �����ɫ��
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
	
	
	
	// ��ȡ�õ�ɫ�����µ�����ֵ
	GetDIBits(hDC, hbm, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);
	
	//�ָ���ɫ��
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	
	//����λͼ�ļ�
	fh = CreateFile(FileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	
	
	if (fh == INVALID_HANDLE_VALUE)
	{
		ErrCode=ERR_PLOTCRBM;
		return ERR_PLOTCRBM;
	}
	
	// ����λͼ�ļ�ͷ
	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	// д��λͼ�ļ�ͷ
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	// д��λͼ�ļ���������
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//���
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	
	return ERR_SUCC;
}

/************************************************************************/
/* �ͷŻ�ͼ��������Դ                                                   */
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
/* �������ͼ��                                                         */
/************************************************************************/
int __stdcall ClearPlot()
{
	memset(&g_Plot,0,sizeof(g_Plot));
	return ERR_SUCC;
}


/************************************************************************/
/* ���뵱ǰɨ��������                                                   */
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
/* ���뵱ǰɨ��������                                                   */
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
		glPointSize(1.5f);//1.5 ��ߴ�
		glBegin(GL_POINTS);
		glColor3f(0.5f, 0.8f, 1.0f);
		glShadeModel(GL_SMOOTH);
		
		for(m=0;m<g_Plot.RowCnt;m++)// Datacnt_Point[j] һ֡����
		{
			Y_Coor = (g_Plot.RowCnt-m)*factor;//z��֡���
			for (j = 1; j < g_Plot.X_Coor[m][0]; j++)
			{
				if(g_Plot.Z_Coor[m][j]<1000) 
					glColor3f(1.0f*(140-g_Plot.Z_Coor[m][j]/10),7*1.0f,150*1.0f);//������ɫ
				else if((g_Plot.Z_Coor[m][j]<2000)) 
					glColor3f(40*1.0f,1.0f*(50+(g_Plot.Z_Coor[m][j]-1000)/20),1.0f*115);
				else if((g_Plot.Z_Coor[m][j]<3000)) 
					glColor3f(1.0f*(60+(g_Plot.Z_Coor[m][j]-2000)/20),1.0f*109,1.0f*43);
				else if((g_Plot.Z_Coor[m][j]<4000)) 
					glColor3f(1.0f*124,1.0f*(125-(g_Plot.Z_Coor[m][j]-3000)/10),1.0f*32);
				else 
					glColor3f(1.0f*250,1.0*(190-(g_Plot.Z_Coor[m][j]-4000)/10),1.0f*32);
				
				glVertex2i(Y_Coor,g_Plot.Z_Coor[m][j]);//�����������������ú���
			}
		}
		
		glEnd();
		glFlush(); 
		return ERR_SUCC;
	}
}
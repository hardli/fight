// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F2C0B5B2_BD55_41EB_AF0F_CBB75122B2D0__INCLUDED_)
#define AFX_STDAFX_H__F2C0B5B2_BD55_41EB_AF0F_CBB75122B2D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>



#include<winsock2.h>
#include<stdio.h>

#pragma comment(lib,"ws2_32.lib")//这句关键;
#include <process.h>  
#include <afxmt.h>

#include <shlwapi.h>

#pragma comment(lib,"Shlwapi.lib") //如果没有这行，会出现link错误
// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F2C0B5B2_BD55_41EB_AF0F_CBB75122B2D0__INCLUDED_)
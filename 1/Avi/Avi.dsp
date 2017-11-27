# Microsoft Developer Studio Project File - Name="Avi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Avi - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Avi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Avi.mak" CFG="Avi - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Avi - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Avi - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Avi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Avi_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Avi_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "Avi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Avi_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Avi_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Avi - Win32 Release"
# Name "Avi - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Avi.def
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Avi.h
# End Source File
# Begin Source File

SOURCE=.\Avi_Comm.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Avi_Algo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Avi_Algo.cpp
# End Source File
# Begin Source File

SOURCE=.\Avi_Algo.h
# End Source File
# Begin Source File

SOURCE=.\Avi_AxleCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\Avi_AxleCheck.h
# End Source File
# Begin Source File

SOURCE=.\Avi_Classifier.cpp
# End Source File
# Begin Source File

SOURCE=.\Avi_Classifier.h
# End Source File
# Begin Source File

SOURCE=.\Avi_FeatureCal.cpp
# End Source File
# Begin Source File

SOURCE=.\Avi_FeatureCal.h
# End Source File
# Begin Source File

SOURCE=.\Avi_MainProc.cpp
# End Source File
# Begin Source File

SOURCE=.\Avi_MainProc.h
# End Source File
# Begin Source File

SOURCE=.\Avi_MathProc.cpp
# End Source File
# Begin Source File

SOURCE=.\Avi_MathProc.h
# End Source File
# Begin Source File

SOURCE=.\Avi_SpeCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\Avi_SpeCheck.h
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\JZGlobal.h
# End Source File
# Begin Source File

SOURCE=.\JZStructure.h
# End Source File
# End Group
# Begin Group "Avi_Plot"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Avi_Plot.cpp
# End Source File
# Begin Source File

SOURCE=.\Avi_Plot.h
# End Source File
# End Group
# Begin Group "Avi_Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Avi_Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Avi_Main.h
# End Source File
# Begin Source File

SOURCE=.\Avi_Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\Avi_Queue.h
# End Source File
# Begin Source File

SOURCE=.\IniProc.cpp
# End Source File
# Begin Source File

SOURCE=.\IniProc.h
# End Source File
# Begin Source File

SOURCE=.\JGProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\JGProxy.h
# End Source File
# Begin Source File

SOURCE=.\LmdsProc.cpp
# End Source File
# Begin Source File

SOURCE=.\LmdsProc.h
# End Source File
# Begin Source File

SOURCE=.\NetMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\NetMgr.h
# End Source File
# End Group
# Begin Group "Avi_Camera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\Camera.h
# End Source File
# End Group
# Begin Group "Avi_DB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Avi_DB.cpp
# End Source File
# Begin Source File

SOURCE=.\Avi_DB.h
# End Source File
# End Group
# Begin Group "Avi_SvWv"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Avi_SvWv.cpp
# End Source File
# Begin Source File

SOURCE=.\Avi_SvWv.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ProjLog.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

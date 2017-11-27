; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAVISDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "AVIS.h"

ClassCount=7
Class1=CAVISApp
Class2=CAVISDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_ALGSET
Resource2=IDR_MAINFRAME
Resource3=IDD_AVIS_DIALOG
Resource4=IDD_LSMAN
Class4=CDbSet
Resource5=IDD_CamSet
Class5=CAlgoSet
Resource6=IDD_ABOUTBOX
Class6=CJGMan
Class7=CCamSet
Resource7=IDR_MAINMENU

[CLS:CAVISApp]
Type=0
HeaderFile=AVIS.h
ImplementationFile=AVIS.cpp
Filter=N

[CLS:CAVISDlg]
Type=0
HeaderFile=AVISDlg.h
ImplementationFile=AVISDlg.cpp
Filter=D
LastObject=IDC_PLOTIM
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=AVISDlg.h
ImplementationFile=AVISDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_AVIS_DIALOG]
Type=1
Class=CAVISDlg
ControlCount=7
Control1=IDC_AlgoList,SysListView32,1350631425
Control2=IDC_PLOTIM,static,1342177806
Control3=IDC_LOG,RICHEDIT,1353777156
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_PlotName,static,1342308865

[MNU:IDR_MAINMENU]
Type=1
Class=CAVISDlg
Command1=ID_ENSVDB
Command2=ID_DISSVDB
Command3=ID_ENPLOT
Command4=IDC_DISPLOT
Command5=IDC_RLINI
Command6=IDC_STALGO
Command7=IDC_CLALGO
Command8=ID_ENSVWV
Command9=ID_DISSVWV
Command10=ID_SETALGO
Command11=IDC_IMPWV
Command12=ID_SimGetCard
Command13=IDC_LASERMAN
Command14=IDC_SOFTMODE
Command15=IDC_FILEMODE
Command16=IDC_TRUEMODE
Command17=ID_CAMMAN
CommandCount=17

[CLS:CDbSet]
Type=0
HeaderFile=DbSet.h
ImplementationFile=DbSet.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDbSet

[DLG:IDD_ALGSET]
Type=1
Class=CAlgoSet
ControlCount=34
Control1=IDC_ALGEXIT,button,1342242816
Control2=IDC_ALGSET,button,1342242816
Control3=IDC_JGHEIGHT,edit,1350631552
Control4=IDC_STARTX,edit,1350631552
Control5=IDC_LANEWD,edit,1350631552
Control6=IDC_MAXHT,edit,1350631552
Control7=IDC_ZEROPOS,edit,1350631552
Control8=IDC_ISLANWD,edit,1350631552
Control9=IDC_ISLANHT,edit,1350631552
Control10=IDC_EMPTYFR,edit,1350631552
Control11=IDC_HIFRAM,edit,1350631552
Control12=IDC_LWFRAM,edit,1350631552
Control13=IDC_LVFRAM,edit,1350631552
Control14=IDC_GRATPOS,combobox,1344340227
Control15=IDC_GRATHT,edit,1350631552
Control16=IDC_A3OUT,combobox,1344340227
Control17=IDC_AXREPORT,combobox,1344340227
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,button,1342177287
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,static,1342308352
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352

[CLS:CAlgoSet]
Type=0
HeaderFile=algoset.h
ImplementationFile=algoset.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CAlgoSet

[DLG:IDD_LSMAN]
Type=1
Class=CJGMan
ControlCount=16
Control1=IDC_RADIO,button,1342308361
Control2=IDC_RADIO1,button,1342177289
Control3=IDC_IP0,SysIPAddress32,1342242816
Control4=IDC_PORT0,edit,1350631552
Control5=IDC_IP1,SysIPAddress32,1342242816
Control6=IDC_PORT1,edit,1350631552
Control7=IDC_STATIC,static,1342308354
Control8=IDC_STATIC,static,1342308354
Control9=IDC_STATIC,static,1342308354
Control10=IDC_STATIC,static,1342308354
Control11=IDC_STATIC,static,1342308354
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,button,1342177287
Control15=IDC_COMFIRM,button,1342242816
Control16=IDC_Exit,button,1342242816

[CLS:CJGMan]
Type=0
HeaderFile=JGMan.h
ImplementationFile=JGMan.cpp
BaseClass=CDialog
Filter=D
LastObject=CJGMan
VirtualFilter=dWC

[DLG:IDD_CamSet]
Type=1
Class=CCamSet
ControlCount=22
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CamEnlable0,button,1342242819
Control4=IDC_CamIP0,SysIPAddress32,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_CamPort0,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_CamUser0,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_CamPassWord0,edit,1350631552
Control12=IDC_CamEnlable1,button,1342242819
Control13=IDC_CamIP1,SysIPAddress32,1342242816
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_CamPort1,edit,1350631552
Control17=IDC_STATIC,static,1342308352
Control18=IDC_CamUser1,edit,1350631552
Control19=IDC_STATIC,static,1342308352
Control20=IDC_CamPassWord1,edit,1350631552
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,button,1342177287

[CLS:CCamSet]
Type=0
HeaderFile=CamSet.h
ImplementationFile=CamSet.cpp
BaseClass=CDialog
Filter=D
LastObject=CCamSet


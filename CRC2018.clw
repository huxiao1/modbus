; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CListenSocket
LastTemplate=CSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CRC2018.h"

ClassCount=5
Class1=CCRC2018App
Class2=CCRC2018Dlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CServerSocket
Class5=CListenSocket
Resource3=IDD_CRC2018_DIALOG

[CLS:CCRC2018App]
Type=0
HeaderFile=CRC2018.h
ImplementationFile=CRC2018.cpp
Filter=N

[CLS:CCRC2018Dlg]
Type=0
HeaderFile=CRC2018Dlg.h
ImplementationFile=CRC2018Dlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_DISTANCE

[CLS:CAboutDlg]
Type=0
HeaderFile=CRC2018Dlg.h
ImplementationFile=CRC2018Dlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CRC2018_DIALOG]
Type=1
Class=CCRC2018Dlg
ControlCount=23
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUT_CALC_CRC,button,1342242816
Control4=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control5=IDC_BUTT_OPEN_COMM,button,1342242816
Control6=IDC_BUTT_SEND_DATA,button,1342242816
Control7=IDC_EDIT_DISPLAY,edit,1352728708
Control8=IDC_BUTT_5_ON,button,1342242816
Control9=IDC_BUTT_5_OFF,button,1342242816
Control10=IDC_STATIC,button,1342177287
Control11=IDC_BUTT_FAN_ON,button,1342242816
Control12=IDC_BUTT_FAN_OFF,button,1342242816
Control13=IDC_STATIC,button,1342177287
Control14=IDC_BUT_FAST,button,1342242816
Control15=IDC_BUT_SLOW,button,1342242816
Control16=IDC_EDIT_DISTANCE,edit,1350631552
Control17=IDC_BUT_DISTANCE,button,1342242816
Control18=IDC_BUT_START,button,1342242816
Control19=IDC_BUT_STOP,button,1342242816
Control20=IDC_BUT_CREATE_SERVER,button,1342242816
Control21=IDC_BUT_SER_SEND,button,1342242816
Control22=IDC_BUT_CLI_CON,button,1342242816
Control23=IDC_BUT_CLI_SEND,button,1342242816

[CLS:CServerSocket]
Type=0
HeaderFile=ServerSocket.h
ImplementationFile=ServerSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq

[CLS:CListenSocket]
Type=0
HeaderFile=ListenSocket.h
ImplementationFile=ListenSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq


; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLmr096View
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "lmr096.h"
LastPage=0

ClassCount=6
Class1=CLmr096App
Class2=CLmr096Doc
Class3=CLmr096View
Class4=CMainFrame

ResourceCount=4
Resource1=IDR_MAINFRAME
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX
Class6=CDlgHistogram
Resource3=IDD_DIALOG1
Resource4=IDD_DIALOG2

[CLS:CLmr096App]
Type=0
HeaderFile=lmr096.h
ImplementationFile=lmr096.cpp
Filter=N

[CLS:CLmr096Doc]
Type=0
HeaderFile=lmr096Doc.h
ImplementationFile=lmr096Doc.cpp
Filter=N
LastObject=CLmr096Doc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CLmr096View]
Type=0
HeaderFile=lmr096View.h
ImplementationFile=lmr096View.cpp
Filter=C
LastObject=ID_FFT
BaseClass=CScrollView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_FFT_FILTER




[CLS:CAboutDlg]
Type=0
HeaderFile=lmr096.cpp
ImplementationFile=lmr096.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_RESET
Command10=ID_GRAY
Command11=ID_HISTOGRAM
Command12=ID_LINEAR
Command13=ID_FOURIER
Command14=ID_IFOURIER
Command15=ID_AVG
Command16=ID_RAPLAS
Command17=ID_MID
Command18=ID_GRAD
Command19=ID_FFT
Command20=ID_IFFT
Command21=ID_FFT_FILTER
CommandCount=21

[DLG:IDD_DIALOG1]
Type=1
Class=CDlgHistogram
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[CLS:CDlgHistogram]
Type=0
HeaderFile=DlgHistogram.h
ImplementationFile=DlgHistogram.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgHistogram
VirtualFilter=dWC

[DLG:IDD_DIALOG2]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816


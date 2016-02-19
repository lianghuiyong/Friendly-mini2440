; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProgressDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WriteFee.h"
LastPage=0

ClassCount=8
Class1=CWriteFeeApp
Class2=CWriteFeeDoc
Class3=CWriteFeeView
Class4=CMainFrame

ResourceCount=5
Resource1=IDD_DIALOG2
Class5=CAboutDlg
Resource2=IDR_MAINFRAME
Class6=CDownFile
Resource3=IDD_DIALOG1
Class7=CPortDlg
Resource4=IDD_ABOUTBOX
Class8=CProgressDlg
Resource5=CG_IDD_PROGRESS

[CLS:CWriteFeeApp]
Type=0
HeaderFile=WriteFee.h
ImplementationFile=WriteFee.cpp
Filter=N

[CLS:CWriteFeeDoc]
Type=0
HeaderFile=WriteFeeDoc.h
ImplementationFile=WriteFeeDoc.cpp
Filter=N

[CLS:CWriteFeeView]
Type=0
HeaderFile=WriteFeeView.h
ImplementationFile=WriteFeeView.cpp
Filter=C
BaseClass=CListView 
VirtualFilter=VWC
LastObject=CWriteFeeView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=WriteFee.cpp
ImplementationFile=WriteFee.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
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
Command14=ID_PARAM
Command15=ID_FEE
Command16=ID_SFEE
Command17=ID_FONT
Command18=ID_PROGRM
Command19=ID_ALL
Command20=ID_VIEW_TOOLBAR
Command21=ID_VIEW_STATUS_BAR
Command22=ID_APP_ABOUT
CommandCount=22

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
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_DIALOG1]
Type=1
Class=CDownFile
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_EDIT1,edit,1350633600

[CLS:CDownFile]
Type=0
HeaderFile=DownFile.h
ImplementationFile=DownFile.cpp
BaseClass=CDialog
Filter=D
LastObject=CDownFile
VirtualFilter=dWC

[DLG:IDD_DIALOG2]
Type=1
Class=CPortDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_COMBO1,combobox,1344339971

[CLS:CPortDlg]
Type=0
HeaderFile=PortDlg.h
ImplementationFile=PortDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMBO1
VirtualFilter=dWC

[DLG:CG_IDD_PROGRESS]
Type=1
Class=CProgressDlg
ControlCount=3
Control1=IDCANCEL,button,1342242817
Control2=CG_IDC_PROGDLG_PROGRESS,msctls_progress32,1082130432
Control3=CG_IDC_PROGDLG_STATUS,static,1342308352

[CLS:CProgressDlg]
Type=0
HeaderFile=ProgDlg.h
ImplementationFile=ProgDlg.cpp
BaseClass=CDialog


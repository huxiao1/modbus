// CRC2018Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CRC2018.h"
#include "CRC2018Dlg.h"
#include "ModbusProtocol.h"
#include "ServerSocket.h"
#include "ListenSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CModbusProtocol ModMotor;
class CServerSocket ServerClientMotor;
class CListenSocket ListenSocketMotor;

unsigned char tcpReceDataBuf[1024];
int tcpReceDataNum = 0;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCRC2018Dlg dialog

CCRC2018Dlg::CCRC2018Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCRC2018Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCRC2018Dlg)
	m_display = _T("");
	m_distance = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCRC2018Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCRC2018Dlg)
	DDX_Control(pDX, IDC_MSCOMM1, m_Comm);
	DDX_Text(pDX, IDC_EDIT_DISPLAY, m_display);
	DDX_Text(pDX, IDC_EDIT_DISTANCE, m_distance);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCRC2018Dlg, CDialog)
	//{{AFX_MSG_MAP(CCRC2018Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUT_CALC_CRC, OnButCalcCrc)
	ON_BN_CLICKED(IDC_BUTT_OPEN_COMM, OnButtOpenComm)
	ON_BN_CLICKED(IDC_BUTT_SEND_DATA, OnButtSendData)
	ON_BN_CLICKED(IDC_BUTT_5_ON, OnButt5On)
	ON_BN_CLICKED(IDC_BUTT_5_OFF, OnButt5Off)
	ON_BN_CLICKED(IDC_BUTT_FAN_ON, OnButtFanOn)
	ON_BN_CLICKED(IDC_BUTT_FAN_OFF, OnButtFanOff)
	ON_BN_CLICKED(IDC_BUT_FAST, OnButFast)
	ON_BN_CLICKED(IDC_BUT_SLOW, OnButSlow)
	ON_BN_CLICKED(IDC_BUT_DISTANCE, OnButDistance)
	ON_BN_CLICKED(IDC_BUT_START, OnButStart)
	ON_BN_CLICKED(IDC_BUT_STOP, OnButStop)
	ON_BN_CLICKED(IDC_BUT_CREATE_SERVER, OnButCreateServer)
	ON_BN_CLICKED(IDC_BUT_SER_SEND, OnButSerSend)
	ON_BN_CLICKED(IDC_BUT_CLI_CON, OnButCliCon)
	ON_BN_CLICKED(IDC_BUT_CLI_SEND, OnButCliSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCRC2018Dlg message handlers

BOOL CCRC2018Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	initComm();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCRC2018Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCRC2018Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCRC2018Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCRC2018Dlg::OnButCalcCrc() 
{
	// TODO: Add your control notification handler code here
	// CRC   
	unsigned char InputData[6] = {0x01, 0x02, 0x00, 0x00, 0x00, 0x01};
	unsigned short CrcResult = 0;

	unsigned char SendData[8];

	//计算出6个八位数的CRC结果
	CrcResult = ModMotor.CRC16(InputData, 6);
	
	//显示CRC计算的结果，用弹出对话框
	//把数据转化为string格式
	CString CrcResultShow;
	CrcResultShow.Format("Crc Result is: %X, %X", LOBYTE_NORMAL(CrcResult), HIBYTE_NORMAL(CrcResult));

	AfxMessageBox(CrcResultShow);

	for (int countFor = 0; countFor<6; countFor++)
	{
		SendData[countFor] = InputData[countFor];
	}

	SendData[6] = LOBYTE_NORMAL(CrcResult);
	SendData[7] = HIBYTE_NORMAL(CrcResult);
}

void CCRC2018Dlg::initComm()
{
	CommOpenFlag = FALSE;
	//设置串口端口号
	m_Comm.SetCommPort(4);
	//设置接收模式为中断模式
	m_Comm.SetInputMode(1);
	//设置串口接收发送缓冲区大小
	m_Comm.SetInBufferSize(1024);
	m_Comm.SetOutBufferSize(1024);
	//设置串口波特率，位，校验方式
	m_Comm.SetSettings("9600, n, 8, 1");
}

void CCRC2018Dlg::OnButtOpenComm() 
{
	// TODO: Add your control notification handler code here
	//如果串口没用打开
	if (!m_Comm.GetPortOpen())
	{
		//打开串口
		m_Comm.SetPortOpen(TRUE);
		//设置中断阈值
		m_Comm.SetRThreshold(1);
		m_Comm.SetInputLen(0);
		m_Comm.GetInput();
		
		CommOpenFlag = true;
		
		AfxMessageBox("打开串口成功");
	}
	else
	{
		CommOpenFlag = false;
		AfxMessageBox("打开串口失败，检查串口是否被占用");
	}
}

void CCRC2018Dlg::OnButtSendData() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CByteArray sendArray;
	UINT8 SendBuf[4] = {0x33, 0x44, 0x55, 0x66};
	UINT16 sendNum = 4;
	UINT16 countFor = 0;
	
	sendArray.SetSize(sendNum);
	
	for (countFor=0; countFor<sendNum; countFor++)
	{
		sendArray.SetAt(countFor, SendBuf[countFor]);
	}
	
	m_Comm.SetOutput((COleVariant(sendArray)));
}

BEGIN_EVENTSINK_MAP(CCRC2018Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCRC2018Dlg)
	ON_EVENT(CCRC2018Dlg, IDC_MSCOMM1, 1 /* OnComm */, OnOnCommMscomm1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCRC2018Dlg::OnOnCommMscomm1() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	UINT8 receDataTemp[1024];
	int receDataLen = 0;
	long CountFor = 0;
	
	if(m_Comm.GetCommEvent()==2)                           //事件值为2表示接收缓冲区内有字符
	{
		variant_inp   = m_Comm.GetInput();                 //读缓冲区
		safearray_inp = variant_inp;                       //VARIANT型变量转换为ColeSafeArray型变量
		receDataLen   = safearray_inp.GetOneDimSize();     //得到有效数据长度
		
		for(CountFor=0; CountFor<receDataLen; CountFor++)
		{
			safearray_inp.GetElement(&CountFor,receDataTemp+CountFor);   //转换为BYTE型数组
		}
		
		displayMessage(receDataTemp, receDataLen);
	}
}

//DEL void CCRC2018Dlg::OnButton3() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	UpdateData(TRUE);
//DEL 
//DEL 	AfxMessageBox(m_send_str);
//DEL //	m_send_str
//DEL }

void CCRC2018Dlg::sendCommData(UINT8 *sendData, UINT16 num)
{
	CByteArray sendArray;
	unsigned short countFor = 0;                            
	
	//对数据进行转换
	sendArray.SetSize(num);
	
	for (countFor=0; countFor<num; countFor++)
	{
		sendArray.SetAt(countFor, sendData[countFor]);
	}
	
	//将转换完的数据通过串口发送出去
	m_Comm.SetOutput((COleVariant(sendArray)));
}

void CCRC2018Dlg::displayMessage(UINT8 *data, UINT16 num)
{
	//将接收的数据转化为显示的字符串
	CString str, strTemp;
	
	for (int i=0; i<num; i++)
	{
		strTemp.Format("%02x ", data[i]);
		str += strTemp;
	}
	
	//为回车、换行
	str +="\r\n";
	
	//将要显示的变量关联到前台显示界面
	m_display += str;
	
	//UpdataData(FALSE)为将变量更新到显示界面，UpdataData(TRUE)为将显示界面输入值更新到变量
	UpdateData(FALSE);
}

void CCRC2018Dlg::OnButt5On() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	unsigned char sendTemp[128];
	int sendNum = 0;
	
	sendNum = ModMotor.WriteSigleCoil(0x01, 0x00, 0xFF00, sendTemp);
	
	displayMessage(sendTemp, sendNum);
	sendCommData(sendTemp, sendNum);
}

void CCRC2018Dlg::OnButt5Off() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	unsigned char sendTemp[128];
	int sendNum = 0;
	
	sendNum = ModMotor.WriteSigleCoil(0x01, 0x00, 0x0000, sendTemp);
	
	displayMessage(sendTemp, sendNum);
	sendCommData(sendTemp, sendNum);
}

void CCRC2018Dlg::OnButtFanOn() 
{
	// TODO: Add your control notification handler code here
	unsigned char sendTemp[128];
	int sendNum = 0;
	
	sendNum = ModMotor.WriteSigleCoil(0x01, 0x01, 0xFF00, sendTemp);
	
	displayMessage(sendTemp, sendNum);
	sendCommData(sendTemp, sendNum);
}

void CCRC2018Dlg::OnButtFanOff() 
{
	// TODO: Add your control notification handler code here
	unsigned char sendTemp[128];
	int sendNum = 0;
	
	sendNum = ModMotor.WriteSigleCoil(0x01, 0x01, 0x0000, sendTemp);
	
	displayMessage(sendTemp, sendNum);
	sendCommData(sendTemp, sendNum);
}

void CCRC2018Dlg::OnButFast() 
{
	// TODO: Add your control notification handler code here
	UINT8 sendTemp[128];   //串口发送数据缓存器
	INT16 sendCommNum=0;       //串口需要发送数据个数
	
	UINT16 RegStartAddr = 0x0604;   //配置速度寄存器地址，0x0604
	UINT16 Reg16[32];               //配置寄存器命令
	
	Reg16[0] = 0x0042;            //配置寄存器命令，第一个16为固定命令0x0042，为什么是这个可以查询变频器PDF文件，对每个寄存器配置都有说明
	Reg16[1] = 0x0008;    //配置速度，0x0000为低速度，0x0008为高速，至于其他速度，速度怎么换算，可以查阅PDF文件，这些配置都为厂家自己定义的规范
	
	sendCommNum = ModMotor.ControDataMaster(0x01,RegStartAddr,2,sendTemp, (UINT8 *)Reg16); //地址，起始地址，2个16位，数据打包后结果，寄存器数据
	
	sendCommData(sendTemp, sendCommNum);
}

void CCRC2018Dlg::OnButSlow() 
{
	// TODO: Add your control notification handler code here
	UINT8 sendTemp[128];   //串口发送数据缓存器
	INT16 sendCommNum=0;       //串口需要发送数据个数
	
	UINT16 RegStartAddr = 0x0604;   //配置速度寄存器地址，0x0604
	UINT16 Reg16[32];               //配置寄存器命令
	
	Reg16[0] = 0x0042;            //配置寄存器命令，第一个16为固定命令0x0042，为什么是这个可以查询变频器PDF文件，对每个寄存器配置都有说明
	Reg16[1] = 0x0000;    //配置速度，0x0000为低速度，0x0008为高速，至于其他速度，速度怎么换算，可以查阅PDF文件，这些配置都为厂家自己定义的规范
	
	sendCommNum = ModMotor.ControDataMaster(0x01,RegStartAddr,2,sendTemp, (UINT8 *)Reg16); //地址，起始地址，2个16位，数据打包后结果，寄存器数据
	
	sendCommData(sendTemp, sendCommNum);
}

void CCRC2018Dlg::OnButDistance() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	UINT8 sendTemp[128];
	INT16 sendCommNum=0;
	
	UINT16 RegStartAddr = 0x0606;  //地址0x0606
	UINT16 Reg16[32];
	
	UpdateData(TRUE);
	Reg16[0] = m_distance & 0x0000FFFF;      //距离是有符号32位数据，正数代表正转，负数代表反转,此处将32位数变为16位数
	Reg16[1] = m_distance >> 16 & 0x0000FFFF;
	
	//后面发送原理相同，先打包数据
	sendCommNum = ModMotor.ControDataMaster(0x01,RegStartAddr,2,sendTemp, (UINT8 *)Reg16);
	
	sendCommData(sendTemp, sendCommNum);
}

void CCRC2018Dlg::OnButStart() 
{
	// TODO: Add your control notification handler code here
	UINT8 sendTemp[128];
	INT16 sendCommNum=0;
	
	UINT16 RegStartAddr = 0x050E;  //发送地址为0x050E
	UINT16 Reg16[32];
	
	UpdateData(TRUE);    //发送数据内容
	Reg16[0] = 0x0001;   //0x0001为发送命令
	Reg16[1] = 0x0000;  
	
	sendCommNum = ModMotor.ControDataMaster(0x01,RegStartAddr,2,sendTemp, (UINT8 *)Reg16);
	
	sendCommData(sendTemp, sendCommNum);
}

void CCRC2018Dlg::OnButStop() 
{
	// TODO: Add your control notification handler code here
	UINT8 sendTemp[128];
	INT16 sendCommNum=0;
	
	UINT16 RegStartAddr = 0x050E;   //对0x050E地址写数
	UINT16 Reg16[32];
	
	UpdateData(TRUE);
	Reg16[0] = 1000;      //写0x0001为启动，写10进制1000为停止
	Reg16[1] = 0x0000;

	sendCommNum = ModMotor.ControDataMaster(0x01,RegStartAddr,2,sendTemp, (UINT8 *)Reg16);
	sendCommData(sendTemp, sendCommNum);
}

void CCRC2018Dlg::OnButCreateServer() 
{
	// TODO: Add your control notification handler code here
	if (ListenSocketMotor.Create(502))
	{
		if (ListenSocketMotor.Listen())
		{
			ListenSocketMotor.m_pParentWnd = this;
			AfxMessageBox("TCP服务器创建成功");
		}
		else
		{
			AfxMessageBox("TCP服务器创建失败");
		}
	}
	else
	{
		AfxMessageBox("TCP服务器创建失败");
	}
}

void CCRC2018Dlg::OnButSerSend() 
{
	// TODO: Add your control notification handler code here
	unsigned char SendBuf[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

	ListenSocketMotor.m_ServerSocket->Send(SendBuf, 6);
}

LRESULT CCRC2018Dlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	switch(message)
	{
	case WM_POST_TCP_MESSAGE:
		tcpReceDataProcess((UINT8 *)wParam, (int)lParam);
		displayMessage(tcpReceDataBuf, tcpReceDataNum);

		break;

	default:
		break;
	}


	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CCRC2018Dlg::tcpReceDataProcess(UINT8 *receData, int receNum)
{
	for (int countFor = 0; countFor< receNum; countFor++)
	{
		tcpReceDataBuf[countFor] = receData[countFor];
	}

	tcpReceDataNum = receNum;
}

void CCRC2018Dlg::OnButCliCon() 
{
	// TODO: Add your control notification handler code here
	ServerClientMotor.m_pParentWnd = this;

	if (ServerClientMotor.Create())
	{
		if (ServerClientMotor.Connect("192.168.1.144", 1000))
		{
			AfxMessageBox("TCP 客户端连接成功");
		}
		else
		{
			AfxMessageBox("TCP 客户端连接失败");
		}
	}
	else
	{
		AfxMessageBox("TCP 客户端连接失败");
	}


}

void CCRC2018Dlg::OnButCliSend() 
{
	// TODO: Add your control notification handler code here
	unsigned char sendBuf[3] = {0x01, 0x02, 0x03};

	ServerClientMotor.Send(sendBuf, 3);
}

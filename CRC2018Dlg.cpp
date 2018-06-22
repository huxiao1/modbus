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

	//�����6����λ����CRC���
	CrcResult = ModMotor.CRC16(InputData, 6);
	
	//��ʾCRC����Ľ�����õ����Ի���
	//������ת��Ϊstring��ʽ
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
	//���ô��ڶ˿ں�
	m_Comm.SetCommPort(4);
	//���ý���ģʽΪ�ж�ģʽ
	m_Comm.SetInputMode(1);
	//���ô��ڽ��շ��ͻ�������С
	m_Comm.SetInBufferSize(1024);
	m_Comm.SetOutBufferSize(1024);
	//���ô��ڲ����ʣ�λ��У�鷽ʽ
	m_Comm.SetSettings("9600, n, 8, 1");
}

void CCRC2018Dlg::OnButtOpenComm() 
{
	// TODO: Add your control notification handler code here
	//�������û�ô�
	if (!m_Comm.GetPortOpen())
	{
		//�򿪴���
		m_Comm.SetPortOpen(TRUE);
		//�����ж���ֵ
		m_Comm.SetRThreshold(1);
		m_Comm.SetInputLen(0);
		m_Comm.GetInput();
		
		CommOpenFlag = true;
		
		AfxMessageBox("�򿪴��ڳɹ�");
	}
	else
	{
		CommOpenFlag = false;
		AfxMessageBox("�򿪴���ʧ�ܣ���鴮���Ƿ�ռ��");
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
	
	if(m_Comm.GetCommEvent()==2)                           //�¼�ֵΪ2��ʾ���ջ����������ַ�
	{
		variant_inp   = m_Comm.GetInput();                 //��������
		safearray_inp = variant_inp;                       //VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
		receDataLen   = safearray_inp.GetOneDimSize();     //�õ���Ч���ݳ���
		
		for(CountFor=0; CountFor<receDataLen; CountFor++)
		{
			safearray_inp.GetElement(&CountFor,receDataTemp+CountFor);   //ת��ΪBYTE������
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
	
	//�����ݽ���ת��
	sendArray.SetSize(num);
	
	for (countFor=0; countFor<num; countFor++)
	{
		sendArray.SetAt(countFor, sendData[countFor]);
	}
	
	//��ת���������ͨ�����ڷ��ͳ�ȥ
	m_Comm.SetOutput((COleVariant(sendArray)));
}

void CCRC2018Dlg::displayMessage(UINT8 *data, UINT16 num)
{
	//�����յ�����ת��Ϊ��ʾ���ַ���
	CString str, strTemp;
	
	for (int i=0; i<num; i++)
	{
		strTemp.Format("%02x ", data[i]);
		str += strTemp;
	}
	
	//Ϊ�س�������
	str +="\r\n";
	
	//��Ҫ��ʾ�ı���������ǰ̨��ʾ����
	m_display += str;
	
	//UpdataData(FALSE)Ϊ���������µ���ʾ���棬UpdataData(TRUE)Ϊ����ʾ��������ֵ���µ�����
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
	UINT8 sendTemp[128];   //���ڷ������ݻ�����
	INT16 sendCommNum=0;       //������Ҫ�������ݸ���
	
	UINT16 RegStartAddr = 0x0604;   //�����ٶȼĴ�����ַ��0x0604
	UINT16 Reg16[32];               //���üĴ�������
	
	Reg16[0] = 0x0042;            //���üĴ��������һ��16Ϊ�̶�����0x0042��Ϊʲô��������Բ�ѯ��Ƶ��PDF�ļ�����ÿ���Ĵ������ö���˵��
	Reg16[1] = 0x0008;    //�����ٶȣ�0x0000Ϊ���ٶȣ�0x0008Ϊ���٣����������ٶȣ��ٶ���ô���㣬���Բ���PDF�ļ�����Щ���ö�Ϊ�����Լ�����Ĺ淶
	
	sendCommNum = ModMotor.ControDataMaster(0x01,RegStartAddr,2,sendTemp, (UINT8 *)Reg16); //��ַ����ʼ��ַ��2��16λ�����ݴ���������Ĵ�������
	
	sendCommData(sendTemp, sendCommNum);
}

void CCRC2018Dlg::OnButSlow() 
{
	// TODO: Add your control notification handler code here
	UINT8 sendTemp[128];   //���ڷ������ݻ�����
	INT16 sendCommNum=0;       //������Ҫ�������ݸ���
	
	UINT16 RegStartAddr = 0x0604;   //�����ٶȼĴ�����ַ��0x0604
	UINT16 Reg16[32];               //���üĴ�������
	
	Reg16[0] = 0x0042;            //���üĴ��������һ��16Ϊ�̶�����0x0042��Ϊʲô��������Բ�ѯ��Ƶ��PDF�ļ�����ÿ���Ĵ������ö���˵��
	Reg16[1] = 0x0000;    //�����ٶȣ�0x0000Ϊ���ٶȣ�0x0008Ϊ���٣����������ٶȣ��ٶ���ô���㣬���Բ���PDF�ļ�����Щ���ö�Ϊ�����Լ�����Ĺ淶
	
	sendCommNum = ModMotor.ControDataMaster(0x01,RegStartAddr,2,sendTemp, (UINT8 *)Reg16); //��ַ����ʼ��ַ��2��16λ�����ݴ���������Ĵ�������
	
	sendCommData(sendTemp, sendCommNum);
}

void CCRC2018Dlg::OnButDistance() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	UINT8 sendTemp[128];
	INT16 sendCommNum=0;
	
	UINT16 RegStartAddr = 0x0606;  //��ַ0x0606
	UINT16 Reg16[32];
	
	UpdateData(TRUE);
	Reg16[0] = m_distance & 0x0000FFFF;      //�������з���32λ���ݣ�����������ת����������ת,�˴���32λ����Ϊ16λ��
	Reg16[1] = m_distance >> 16 & 0x0000FFFF;
	
	//���淢��ԭ����ͬ���ȴ������
	sendCommNum = ModMotor.ControDataMaster(0x01,RegStartAddr,2,sendTemp, (UINT8 *)Reg16);
	
	sendCommData(sendTemp, sendCommNum);
}

void CCRC2018Dlg::OnButStart() 
{
	// TODO: Add your control notification handler code here
	UINT8 sendTemp[128];
	INT16 sendCommNum=0;
	
	UINT16 RegStartAddr = 0x050E;  //���͵�ַΪ0x050E
	UINT16 Reg16[32];
	
	UpdateData(TRUE);    //������������
	Reg16[0] = 0x0001;   //0x0001Ϊ��������
	Reg16[1] = 0x0000;  
	
	sendCommNum = ModMotor.ControDataMaster(0x01,RegStartAddr,2,sendTemp, (UINT8 *)Reg16);
	
	sendCommData(sendTemp, sendCommNum);
}

void CCRC2018Dlg::OnButStop() 
{
	// TODO: Add your control notification handler code here
	UINT8 sendTemp[128];
	INT16 sendCommNum=0;
	
	UINT16 RegStartAddr = 0x050E;   //��0x050E��ַд��
	UINT16 Reg16[32];
	
	UpdateData(TRUE);
	Reg16[0] = 1000;      //д0x0001Ϊ������д10����1000Ϊֹͣ
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
			AfxMessageBox("TCP�����������ɹ�");
		}
		else
		{
			AfxMessageBox("TCP����������ʧ��");
		}
	}
	else
	{
		AfxMessageBox("TCP����������ʧ��");
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
			AfxMessageBox("TCP �ͻ������ӳɹ�");
		}
		else
		{
			AfxMessageBox("TCP �ͻ�������ʧ��");
		}
	}
	else
	{
		AfxMessageBox("TCP �ͻ�������ʧ��");
	}


}

void CCRC2018Dlg::OnButCliSend() 
{
	// TODO: Add your control notification handler code here
	unsigned char sendBuf[3] = {0x01, 0x02, 0x03};

	ServerClientMotor.Send(sendBuf, 3);
}

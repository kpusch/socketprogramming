
// MyClientDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MyClient.h"
#include "MyClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyClientDlg ��ȭ ����



CMyClientDlg::CMyClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mh_socket = INVALID_SOCKET;
	m_connected_flag = 0;	//0:������ ������ ���´�.
}

void CMyClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EVENT_LIST, m_event_list);
}

BEGIN_MESSAGE_MAP(CMyClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(28001, &CMyClientDlg::On28001)
	ON_BN_CLICKED(IDC_SEND_BTN, &CMyClientDlg::OnBnClickedSendBtn)
	ON_MESSAGE(28002, &CMyClientDlg::On28002)
END_MESSAGE_MAP()


// CMyClientDlg �޽��� ó����

BOOL CMyClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	mh_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in srv_addr;

	memset(&srv_addr, 0, sizeof(struct sockaddr_in));
	srv_addr.sin_family = AF_INET;
	//������ �¾ƾ� �ϴ� �κ�
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv_addr.sin_port = htons(18000);
	//

	WSAAsyncSelect(mh_socket, m_hWnd, 28001, FD_CONNECT);

	m_connected_flag = 1;	//1: ������ �õ��ϰ� �ִ� ���̴�

	//�񵿱� �ɾ����� connect ����� ������ �߸��ǰų� ���������� 28�ʰ� ��������� ���� �� �ִ�
	connect(mh_socket, (LPSOCKADDR)&srv_addr, sizeof(srv_addr));

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMyClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMyClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



afx_msg LRESULT CMyClientDlg::On28001(WPARAM wParam, LPARAM lParam)
{
	//mh_socket�� wParam���� �Ѿ�� Ŭ��� 1���̴ϱ� wParam�� �̿�����
	//lParam - �̺�Ʈ and ���� ��� �ִ�

	if (WSAGETSELECTERROR(lParam))
	{
		//���ӿ� �����ߴ�
		AfxMessageBox("������ �����ϴ�");
		m_connected_flag = 0;
		closesocket(mh_socket);
		mh_socket = INVALID_SOCKET;
	}
	else {
		//���ӿ� �����ߴ�
		AfxMessageBox("���ӿ� �����߽��ϴ�");
		m_connected_flag = 2;		//2 : ������ �����ߴ�.
	}
	return 0;
}

void CMyClientDlg::AddEvent(CString a_string) 
{
	while (m_event_list.GetCount() > 1000)
		m_event_list.DeleteString(0);

	int index = m_event_list.InsertString(-1, a_string);
	m_event_list.SetCurSel(index);		//const char * :����Ű�� ����� �ٲ��� �ʴ´�
}

void CMyClientDlg::SendFrameData(SOCKET ah_socket,	//� ��������
	unsigned char a_msg_id,	//�޽��� ���̵�
	const char *ap_data,	//� �����͸�
	unsigned short int a_data_size)	//������ �����
{

	int send_data_size = a_data_size + 4;	//+���(4byte)
	char *p_send_data = new char[send_data_size];

	*p_send_data = 27;
	*(p_send_data + 1) = a_msg_id;
	*(unsigned short int *)(p_send_data + 2) = a_data_size;

	memcpy(p_send_data + 4, ap_data, a_data_size);

	send(mh_socket, p_send_data, send_data_size, 0);

	delete[] p_send_data;
}

void CMyClientDlg::OnBnClickedSendBtn()
{
	if (m_connected_flag == 2) {
		CString str;
		GetDlgItemText(IDC_CHAT_EDIT, str);	//����Ʈ �ڽ��� �ִ°� ����

		SendFrameData(mh_socket, 1, str, str.GetLength() + 1);	//����, msg_id, ����, ������

		SetDlgItemText(IDC_CHAT_EDIT, "");	//���� ��ư ���� �� ����Ʈ�ڽ� Ŭ����
	}
}


afx_msg LRESULT CMyClientDlg::On28002(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTEVENT(lParam) == FD_READ) {
		//�������� �����͸� ���� ���
		WSAAsyncSelect(mh_socket, m_hWnd, 28002, FD_READ); //FD_READ�� ��� �߻�! ��������
		//25002 -> 28002
		unsigned char check, msg_id;
		unsigned short int body_size;

		recv(mh_socket, (char *)&check, 1, 0);		//1byte ���� - ��ȿ�� üũ(�����б�) 27��
		if (check == 27)
		{
			recv(mh_socket, (char *)&msg_id, 1, 0);		//1Byte �о���. - � ���� �޽�������.. �α���
			recv(mh_socket, (char *)&body_size, 2, 0);	//2byte �о���. - body�� ũ�⸦ �о���.()
			
			if (body_size > 0) {
				//body_size�� 0�ϼ��� �ִ�.
				char *p_body_data = new char[body_size];

				recv(mh_socket, p_body_data, body_size, 0);	//Body�� ����
				if (msg_id == 1)
					AddEvent(p_body_data);	//���� ������ �����͸� ������ �������� �ٽ� �� �ѷ���

				delete[] p_body_data;
			}
			
			//25002 -> 28002
			WSAAsyncSelect(mh_socket, m_hWnd, 28002, FD_READ | FD_CLOSE);
			//�ٽ� FD_READ �����ϵ��� ����!
			//if�� ������ �ִ� ��� : ������Ʈ !! ������ִ°Ŷ� ���
		}
	}
	else {
		//�������� ������ ���� ���
		AddEvent("������ ������ �������ϴ�.");
		m_connected_flag = 0;
		closesocket(mh_socket);
		mh_socket = INVALID_SOCKET;
	}
	return 0;
}

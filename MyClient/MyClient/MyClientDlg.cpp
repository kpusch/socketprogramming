
// MyClientDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MyClient.h"
#include "MyClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyClientDlg 대화 상자



CMyClientDlg::CMyClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mh_socket = INVALID_SOCKET;
	m_connected_flag = 0;	//0:접속을 끊어진 상태다.
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


// CMyClientDlg 메시지 처리기

BOOL CMyClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	mh_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in srv_addr;

	memset(&srv_addr, 0, sizeof(struct sockaddr_in));
	srv_addr.sin_family = AF_INET;
	//서버와 맞아야 하는 부분
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv_addr.sin_port = htons(18000);
	//

	WSAAsyncSelect(mh_socket, m_hWnd, 28001, FD_CONNECT);

	m_connected_flag = 1;	//1: 접속을 시도하고 있는 중이다

	//비동기 걸어주자 connect 실행시 서버가 잘못되거나 닫혀있을때 28초간 응답없음에 빠질 수 있다
	connect(mh_socket, (LPSOCKADDR)&srv_addr, sizeof(srv_addr));

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMyClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMyClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



afx_msg LRESULT CMyClientDlg::On28001(WPARAM wParam, LPARAM lParam)
{
	//mh_socket이 wParam으로 넘어옴 클라는 1명이니까 wParam을 이용하자
	//lParam - 이벤트 and 에러 들어 있다

	if (WSAGETSELECTERROR(lParam))
	{
		//접속에 실패했다
		AfxMessageBox("서버가 없습니다");
		m_connected_flag = 0;
		closesocket(mh_socket);
		mh_socket = INVALID_SOCKET;
	}
	else {
		//접속에 성공했다
		AfxMessageBox("접속에 성공했습니다");
		m_connected_flag = 2;		//2 : 서버에 접속했다.
	}
	return 0;
}

void CMyClientDlg::AddEvent(CString a_string) 
{
	while (m_event_list.GetCount() > 1000)
		m_event_list.DeleteString(0);

	int index = m_event_list.InsertString(-1, a_string);
	m_event_list.SetCurSel(index);		//const char * :가리키는 대상을 바꾸지 않는다
}

void CMyClientDlg::SendFrameData(SOCKET ah_socket,	//어떤 소켓으로
	unsigned char a_msg_id,	//메시지 아이디
	const char *ap_data,	//어떤 데이터를
	unsigned short int a_data_size)	//데이터 사이즈를
{

	int send_data_size = a_data_size + 4;	//+헤더(4byte)
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
		GetDlgItemText(IDC_CHAT_EDIT, str);	//에디트 박스에 있는걸 담자

		SendFrameData(mh_socket, 1, str, str.GetLength() + 1);	//소켓, msg_id, 내용, 사이즈

		SetDlgItemText(IDC_CHAT_EDIT, "");	//전송 버튼 눌린 후 에디트박스 클리어
	}
}


afx_msg LRESULT CMyClientDlg::On28002(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTEVENT(lParam) == FD_READ) {
		//서버에서 데이터를 보낸 경우
		WSAAsyncSelect(mh_socket, m_hWnd, 28002, FD_READ); //FD_READ가 계속 발생! 막아주자
		//25002 -> 28002
		unsigned char check, msg_id;
		unsigned short int body_size;

		recv(mh_socket, (char *)&check, 1, 0);		//1byte 읽음 - 유효성 체크(끊어읽기) 27번
		if (check == 27)
		{
			recv(mh_socket, (char *)&msg_id, 1, 0);		//1Byte 읽었다. - 어떤 종류 메시지인지.. 로그인
			recv(mh_socket, (char *)&body_size, 2, 0);	//2byte 읽었다. - body의 크기를 읽었다.()
			
			if (body_size > 0) {
				//body_size가 0일수도 있다.
				char *p_body_data = new char[body_size];

				recv(mh_socket, p_body_data, body_size, 0);	//Body를 읽자
				if (msg_id == 1)
					AddEvent(p_body_data);	//내가 서버에 데이터를 보내면 서버에서 다시 다 뿌려줌

				delete[] p_body_data;
			}
			
			//25002 -> 28002
			WSAAsyncSelect(mh_socket, m_hWnd, 28002, FD_READ | FD_CLOSE);
			//다시 FD_READ 수신하도록 설정!
			//if문 안으로 넣는 경우 : 블랙리스트 !! 만들어주는거랑 비슷
		}
	}
	else {
		//서버에서 연결을 끊은 경우
		AddEvent("서버가 연결을 끊었습니다.");
		m_connected_flag = 0;
		closesocket(mh_socket);
		mh_socket = INVALID_SOCKET;
	}
	return 0;
}

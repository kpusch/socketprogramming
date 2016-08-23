
// MyClientDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CMyClientDlg 대화 상자
class CMyClientDlg : public CDialogEx
{
private:
	SOCKET mh_socket;
	char m_connected_flag;	//서버에 접속이 안됐다 //서버에 접속중이다 //서버에 접속했다

// 생성입니다.
public:
	CMyClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	void SendFrameData(SOCKET ah_socket, unsigned char a_msg_id,
		const char *ap_data, unsigned short int a_data_size);
										
	//파일사이즈는 커지지만,함수로 인지x 함수로 불려진곳에 코딩으로 들어감 => 스피드증가
	//inline void AddEvent(CString a_string);

	void AddEvent(CString a_string);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT On28001(WPARAM wParam, LPARAM lParam);
public:
	CListBox m_event_list;
	afx_msg void OnBnClickedSendBtn();
protected:
	afx_msg LRESULT On28002(WPARAM wParam, LPARAM lParam);
};

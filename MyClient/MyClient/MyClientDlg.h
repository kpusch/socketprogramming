
// MyClientDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CMyClientDlg ��ȭ ����
class CMyClientDlg : public CDialogEx
{
private:
	SOCKET mh_socket;
	char m_connected_flag;	//������ ������ �ȵƴ� //������ �������̴� //������ �����ߴ�

// �����Դϴ�.
public:
	CMyClientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	void SendFrameData(SOCKET ah_socket, unsigned char a_msg_id,
		const char *ap_data, unsigned short int a_data_size);
										
	//���ϻ������ Ŀ������,�Լ��� ����x �Լ��� �ҷ������� �ڵ����� �� => ���ǵ�����
	//inline void AddEvent(CString a_string);

	void AddEvent(CString a_string);

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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


// UpperMonitorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UpperMonitor.h"
#include "UpperMonitorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx {
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUpperMonitorDlg �Ի���



CUpperMonitorDlg::CUpperMonitorDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CUpperMonitorDlg::IDD, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CUpperMonitorDlg::~CUpperMonitorDlg() {
	DeleteObject(m_MenuDebugger);
	DeleteObject(m_MenuAppdev);
	DeleteObject(m_MainMenu);
}

void CUpperMonitorDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_MainMenu);
}

BEGIN_MESSAGE_MAP(CUpperMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CUpperMonitorDlg::OnSelchangeTab)
END_MESSAGE_MAP()


// CUpperMonitorDlg ��Ϣ�������

BOOL CUpperMonitorDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if(pSysMenu != NULL) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if(!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	// 1. ����Tabѡ���ǩ
	TCITEM tcItemDebugger;
	tcItemDebugger.mask = TCIF_TEXT;
	tcItemDebugger.pszText = _T("��������");
	m_MainMenu.InsertItem(0, &tcItemDebugger);
	TCITEM tcItemAppdev;
	tcItemAppdev.mask = TCIF_TEXT;
	tcItemAppdev.pszText = _T("Ӧ�ÿ���");
	m_MainMenu.InsertItem(1, &tcItemAppdev);
	// 2. �����Ի��򣬽�TAB�ؼ���Ϊѡ���Ӧ�Ի���ĸ�����
	m_MenuDebugger.Create(IDD_DEBUGGER, GetDlgItem(IDC_TAB));
	m_MenuAppdev.Create(IDD_APPDEV, GetDlgItem(IDC_TAB));
	// 3. ��ȡTAB�ؼ��ͻ�����С�����ڵ���ѡ��Ի����ڸ������е�λ��
	CRect rect;
	m_MainMenu.GetClientRect(&rect);
	rect.top += 22;
	rect.right -= 3;
	rect.bottom -= 2;
	rect.left += 1;
	// 4. �����ӶԻ���ߴ粢�ƶ���ָ��λ��
	m_MenuDebugger.MoveWindow(&rect);
	m_MenuAppdev.MoveWindow(&rect);
	// 5. ����Ĭ��ѡ�����ѡ��Ի���������غ���ʾ
	m_MenuDebugger.ShowWindow(SW_SHOWNORMAL);
	m_MenuAppdev.ShowWindow(SW_HIDE);
	m_MainMenu.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CUpperMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUpperMonitorDlg::OnPaint() {
	if(IsIconic()) {
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CUpperMonitorDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}


// 6. ����ѡ��л�
void CUpperMonitorDlg::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult) {
	*pResult = 0;

	// ��ȡ��ǰ���ѡ���ǩ�±�
	int cursel = m_MainMenu.GetCurSel();
	// �����±꽫��Ӧ�ĶԻ�����ʾ����������
	switch(cursel) {
		case 0:
			m_MenuDebugger.ShowWindow(SW_SHOWNORMAL);
			m_MenuAppdev.ShowWindow(SW_HIDE);
			break;
		case 1:
			m_MenuDebugger.ShowWindow(SW_HIDE);
			m_MenuAppdev.ShowWindow(SW_SHOWNORMAL);
			break;
		default:
			break;
	}

}

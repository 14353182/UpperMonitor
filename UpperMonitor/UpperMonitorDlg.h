
// UpperMonitorDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "Debugger.h"
#include "Appdev.h"


// CUpperMonitorDlg �Ի���
class CUpperMonitorDlg : public CDialogEx
{
// ����
public:
	CUpperMonitorDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CUpperMonitorDlg();

// �Ի�������
	enum { IDD = IDD_UPPERMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_MainMenu;
//private:
	CDebugger m_MenuDebugger;
	CAppdev m_MenuAppdev;
public:
	afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
};

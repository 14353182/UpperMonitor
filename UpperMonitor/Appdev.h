#pragma once

#include "AdoMySQLHelper.h"
#include "RecordHelper.h"

#define NOCARD _T("no card")
#define TESTCARD _T("a486214b")

#define SCANTIMER 1
#define SCANTIMER_ID 1

// CAppdev �Ի���

class CAppdev : public CDialogEx
{
	DECLARE_DYNAMIC(CAppdev)

public:
	CAppdev(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAppdev();

// �Ի�������
	enum { IDD = IDD_APPDEV };

private:
	bool canIOPurse;
	bool canIOWeb;
	CFont m_font2;
	CAdoMySQLHelper adoMySQLHelper;
	CRecordHelper fileRecordHelper;
	UINT_PTR m_ActiveTimer;
	bool isWritingRemainTimeTable;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CString GetCardUID();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnpurseinit();
	void CString2CharStar(const CString& s, char* ch, int len);
	void HexCString2UnsignedCharStar(const CString& hexStr, unsigned char* asc, int* asc_len);
	afx_msg void OnBnClickedBtncheckbalance();
	afx_msg void OnBnClickedBtnrecharge();
	afx_msg void OnBnClickedBtncomsurge2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnstartweb();
	afx_msg void OnBnClickedBtnretimedefinit();
	afx_msg void OnBnClickedBtnexitweb();
	afx_msg void OnBnClickedBtncheckretime();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedBtnloadhis();
	afx_msg void OnBnClickedBtnclearhis();
};

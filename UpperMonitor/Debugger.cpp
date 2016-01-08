// Debugger.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UpperMonitor.h"
#include "Debugger.h"
#include "afxdialogex.h"
#include "atlrx.h"
#include "Utils.h"

#pragma comment(lib, "./libs/ZM124U.lib")
#include "./libs/ZM124U.h"

#define RED RGB(255, 0, 0)
#define BLUE RGB(0, 0, 255)
#define BLACK RGB(0, 0, 0)

// CDebugger �Ի���

IMPLEMENT_DYNAMIC(CDebugger, CDialogEx)

CDebugger::CDebugger(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebugger::IDD, pParent)
{
	// 
	m_RatioStatus = noType;
	// ��������
	m_font.CreatePointFont(93, _T("����"));
	m_font2.CreatePointFont(110, _T("������κ"));
}

CDebugger::~CDebugger()
{
	DeleteObject(m_font);
}

void CDebugger::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	GetDlgItem(IDC_OpenDevice)->SetFont(&m_font2);
	GetDlgItem(IDC_GETCARDINFO)->SetFont(&m_font2);
	GetDlgItem(IDC_LEDSET)->SetFont(&m_font2);
	GetDlgItem(IDC_IOOPERATION)->SetFont(&m_font2);
	GetDlgItem(IDC_PWDSET)->SetFont(&m_font2);
}


BEGIN_MESSAGE_MAP(CDebugger, CDialogEx)
	ON_BN_CLICKED(IDC_BTNCARDGET, &CDebugger::OnBnClickedBtncardget)
	ON_BN_CLICKED(IDC_BTNOPENDEVICE, &CDebugger::OnBnClickedBtnopendevice)
	ON_BN_CLICKED(IDC_BTNCARDCHECK, &CDebugger::OnBnClickedBtncardcheck)
	ON_BN_CLICKED(IDC_BTNLEDSET, &CDebugger::OnBnClickedBtnledset)
	ON_BN_CLICKED(IDC_BTNDEFPWD, &CDebugger::OnBnClickedBtndefpwd)
	ON_BN_CLICKED(IDC_RAOTYPEA, &CDebugger::OnBnClickedRaotypea)
	ON_BN_CLICKED(IDC_RAOTYPEB, &CDebugger::OnBnClickedRaotypeb)
	ON_BN_CLICKED(IDC_BTNREADBLOCK, &CDebugger::OnBnClickedBtnreadblock)
	ON_BN_CLICKED(IDC_BTNWRITEBLOCK, &CDebugger::OnBnClickedBtnwriteblock)
	ON_BN_CLICKED(IDC_BTNREADSECTION, &CDebugger::OnBnClickedBtnreadsection)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CDebugger ��Ϣ�������

void CDebugger::OnBnClickedBtncardget() {
	CString uid, temp;
	unsigned char buff[1024];
	int buff_len;

	// �ɹ���ȡ
	if(find_14443(buff, &buff_len) == IFD_OK) {
		uid.Empty();
		for(int i = 0; i < buff_len; i++) {
			// ����õ�UID���ݣ�1 byte��תΪ16����
			temp.Format(_T("%02x"), buff[i]);
			uid += temp;
		}
		// ��ʾUID��Ϣ
		((CEdit*)GetDlgItem(IDC_EDITCARDUID))->SetWindowTextW(uid);
		// ����״̬���ɹ�
		canGetCardInfo = true;
		((CEdit*)GetDlgItem(IDC_EDITCARDSTATUS))->SetWindowTextW(_T("�ɹ�"));
	}
	else {
		// ʧ��ʱ���UID��Ϣ����ֹ�ϴ���©����
		((CEdit*)GetDlgItem(IDC_EDITCARDUID))->SetWindowTextW(_T(""));
		// ����״̬����ʧ��
		canGetCardInfo = false;
		((CEdit*)GetDlgItem(IDC_EDITCARDSTATUS))->SetWindowTextW(_T("��ȡ�����쳣"));
		// ��������ʾʧ��
		CUtils::buzzerFailed();
	}
}


void CDebugger::OnBnClickedBtnopendevice() {
	if(IDD_PowerOn() == IFD_OK) {
		// ����״̬�����ɹ�
		isDeviceOpen = true;
		((CEdit*)GetDlgItem(IDC_EDITSTATUS))->SetWindowTextW(_T("�����豸�ɹ�"));
	}
	else {
		// ����״̬����ʧ��
		isDeviceOpen = false;
		((CEdit*)GetDlgItem(IDC_EDITSTATUS))->SetWindowTextW(_T("�����豸ʧ��"));
		// ��������ʾʧ��
		CUtils::buzzerFailed();
	}
}


void CDebugger::OnBnClickedBtncardcheck() {
	unsigned char card_version[1024];
	int card_version_len;
	// �鿴��Ƭ�汾��Ϣ
	if(Reader_Version(card_version, &card_version_len) == IFD_OK) {
		// ����״̬�����ɹ�
		canGetCardInfo = true;
		((CEdit*)GetDlgItem(IDC_EDITVERSIONINFO))->SetWindowTextW((CString)card_version);
	}
	else {
		// ����״̬����ʧ��
		canGetCardInfo = false;
		((CEdit*)GetDlgItem(IDC_EDITVERSIONINFO))->SetWindowTextW(_T("�鿴ʧ��"));
		// ��������ʾʧ��
		CUtils::buzzerFailed();
	}
}


void CDebugger::OnBnClickedBtnledset() {
	CString info, mecNum;
	// ��ȡ����
	((CEdit*)GetDlgItem(IDC_EDITLEDINFO))->GetWindowTextW(info);
	// ��ȡС��λ��
	((CEdit*)GetDlgItem(IDC_EDITLEDMECNUM))->GetWindowTextW(mecNum);
	// �ж��Ƿ��ֵ�Ƿ�
	if(info.IsEmpty() || mecNum.IsEmpty()) {
		MessageBox(_T("������LED��������"));
		return;
	}
	else if(info.SpanIncluding(_T("0123456789")) != info) {
		MessageBox(_T("LEDֻ�����봿����"));
		return;
	}
	
	int len_info = info.GetLength();
	int intMecNum = _ttoi(mecNum);
	// С��λ�����6λ
	if(len_info > 6) {
		MessageBox(_T("���ֹ���"));
		return;
	}
	// С���������λ����
	else if (intMecNum >= len_info) {
		for (int i = 0; i <= intMecNum - len_info; i++){
			info = _T("0") + info;
		}
		len_info = info.GetLength();
	}
	// ����ʵ��+1
	unsigned char point = (unsigned char)_ttoi(mecNum) + 1;
	if(point > 0x06 || point < 0x00) {
		// λ�����ܴ���6
		MessageBox(_T("С��λ������"));
		return;
	}
	else if (mecNum == _T("0")){
		// ��С���޸�
		point = 0x00;
	}
	char chinfo[7];
	CUtils::CString2CharStar(info, chinfo, len_info);
	if(LED(chinfo, len_info, point) == IFD_OK) {
		// ����״̬�����ɹ�
		canSetLED = true;
		((CEdit*)GetDlgItem(IDC_EDITLEDSTATUS))->SetWindowTextW(_T("���óɹ�"));
	}
	else {
		// ����״̬����ʧ��
		canSetLED = false;
		((CEdit*)GetDlgItem(IDC_EDITLEDSTATUS))->SetWindowTextW(_T("����ʧ��"));
		// ��������ʾʧ��
		CUtils::buzzerFailed();
	}
}


void CDebugger::OnBnClickedBtndefpwd() {
	// ��ȡĬ����Կ
	((CEdit*)GetDlgItem(IDC_EDITPWD))->SetWindowTextW(_T("FFFFFFFFFFFF"));
}


void CDebugger::OnBnClickedRaotypea() {
	// ���ÿ�Ƭ����ΪA
	m_RatioStatus = typeA;
}


void CDebugger::OnBnClickedRaotypeb() {
	// ���ÿ�Ƭ����Ϊ��
	m_RatioStatus = typeB;
}


void CDebugger::OnBnClickedBtnreadblock() {
	// ����Ƿ�ѡ����Կ����
	if(m_RatioStatus == noType) {
		MessageBox(_T("��ѡ����Կ���ͣ�"));
		return;
	}
	// ��ȡ��Կ
	CString pwd;
	((CEdit*)GetDlgItem(IDC_EDITPWD))->GetWindowTextW(pwd);
	if(pwd.IsEmpty()) {
		MessageBox(_T("��������Կ��"));
		return;
	}
	else if(pwd.GetLength() != 12) {
		MessageBox(_T("��Կ���ȷǷ���"));
		return;
	}
	// ��Կ��������ת��
	//unsigned char* chpwd = (unsigned char*)(LPCTSTR)pwd;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	pwd.MakeUpper();
	CUtils::HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// ��ȡ������
	int sectionNum = ((CComboBox*)GetDlgItem(IDC_COMBOSECTION))->GetCurSel();
	if(sectionNum == CB_ERR) {
		MessageBox(_T("��ѡ��������"));
		return;
	}
	// ��ȡ������
	int blockNum = ((CComboBox*)GetDlgItem(IDC_COMBOBLOCK))->GetCurSel();
	if(blockNum == CB_ERR) {
		MessageBox(_T("��ѡ�����"));
		return;
	}
	// ��ȡ��
	unsigned char des_data[1024];
	int des_len;
	if(read_block(sectionNum, blockNum, m_RatioStatus, chpwd, des_data, &des_len) == IFD_OK) {
		// ��ʽ��
		CString block_data, temp;
		block_data.Empty();
		for(int i = 0; i < des_len; i++) {
			temp.Format(_T("%02x"), des_data[i]);
			block_data += temp;
		}
		// ���ݿ�����ò�ͬ�Ŀ�
		switch(blockNum) {
			case 0:
				((CEdit*)GetDlgItem(IDC_EDITBL0DATA))->SetWindowTextW(block_data);
				break;
			case 1:
				((CEdit*)GetDlgItem(IDC_EDITBL1DATA))->SetWindowTextW(block_data);
				break;
			case 2:
				((CEdit*)GetDlgItem(IDC_EDITBL2DATA))->SetWindowTextW(block_data);
				break;
			default:
				// Maybe Wrong
				((CEdit*)GetDlgItem(IDC_EDITBL3DATA0))->SetWindowTextW(block_data.Left(12));
				((CEdit*)GetDlgItem(IDC_EDITBL3DATA1))->SetWindowTextW(block_data.Mid(12, 8));
				((CEdit*)GetDlgItem(IDC_EDITBL3DATA2))->SetWindowTextW(block_data.Right(12));
				break;

		}
		// ����״̬�����ɹ�
		canIO = true;
		((CEdit*)GetDlgItem(IDC_EDITIOSTATUS))->SetWindowTextW(_T("��ȡ��ɹ�"));
	}
	else {
		// �ÿ�ȫ��������״̬����ʧ��
		canIO = false;
		switch (blockNum){
		case 0:
			((CEdit*)GetDlgItem(IDC_EDITBL0DATA))->SetWindowTextW(_T(""));
			break;
		case 1:
			((CEdit*)GetDlgItem(IDC_EDITBL1DATA))->SetWindowTextW(_T(""));
			break;
		case 2:
			((CEdit*)GetDlgItem(IDC_EDITBL2DATA))->SetWindowTextW(_T(""));
			break;
		default:
			((CEdit*)GetDlgItem(IDC_EDITBL3DATA0))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL3DATA1))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL3DATA2))->SetWindowTextW(_T(""));
			break;
		}
		((CEdit*)GetDlgItem(IDC_EDITIOSTATUS))->SetWindowTextW(_T("��ȡ���쳣"));
		// ��������ʾʧ��
		CUtils::buzzerFailed();
	}
}


void CDebugger::OnBnClickedBtnwriteblock() {
	// ��ȡ��Կ����
	if(m_RatioStatus == noType) {
		MessageBox(_T("��ѡ����Կ���ͣ�"));
		return;
	}
	// ��ȡ��Կ
	CString pwd;
	((CEdit*)GetDlgItem(IDC_EDITPWD))->GetWindowTextW(pwd);
	if(pwd.IsEmpty()) {
		MessageBox(_T("��������Կ��"));
		return;
	}
	else if(pwd.GetLength() != 12) {
		MessageBox(_T("��Կ���ȷǷ���"));
		return;
	}
	// ��Կ����ת��
	//unsigned char* chpwd = (unsigned char*)(LPCTSTR)pwd;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	CUtils::HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// ��ȡ����
	int sectionNum = ((CComboBox*)GetDlgItem(IDC_COMBOSECTION))->GetCurSel();
	if(sectionNum == CB_ERR) {
		MessageBox(_T("��ѡ��������"));
		return;
	}
	// ��ȡ���
	int blockNum = ((CComboBox*)GetDlgItem(IDC_COMBOBLOCK))->GetCurSel();
	if(blockNum == CB_ERR) {
		MessageBox(_T("��ѡ�����"));
		return;
	}
	// ���ݿ�Ż�ȡ����
	CString block_data;
	if(blockNum == 0) {
		((CEdit*)GetDlgItem(IDC_EDITBL0DATA))->GetWindowTextW(block_data);
		if (block_data.GetLength() % 2 == 1)	block_data += _T("0");
	}
	else if(blockNum == 1) {
		((CEdit*)GetDlgItem(IDC_EDITBL1DATA))->GetWindowTextW(block_data);
		if (block_data.GetLength() % 2 == 1)	block_data += _T("0");
	}
	else if(blockNum == 2) {
		((CEdit*)GetDlgItem(IDC_EDITBL2DATA))->GetWindowTextW(block_data);
		if (block_data.GetLength() % 2 == 1)	block_data += _T("0");
	}
	else if(blockNum == 3) {
		((CEdit*)GetDlgItem(IDC_EDITBL3DATA0))->GetWindowTextW(block_data);
		if(block_data.GetLength() < 12) {
			for(int i = block_data.GetLength(); i<12; i++) {
				block_data += _T("0");
			}
		}
		block_data = block_data + _T("FF078069") + _T("FFFFFFFFFFFF");
	}
	
	if(block_data.GetLength() > 32) {
		MessageBox(_T("д����Ϣ���ݹ���"));
		return;
	}
	/******************������ʽ��֤************************/
	block_data.MakeLower();
	CAtlRegExp<> reNumEng;
	REParseError status = reNumEng.Parse(_T("^[a-z0-9]+$"));
	if(REPARSE_ERROR_OK != status) {
		MessageBox(_T("�����Լ�ɵ��д��������ʽ��"));
		return;
	}
	CAtlREMatchContext<> mcNumEng;
	if(!reNumEng.Match(block_data, &mcNumEng)) {
		MessageBox(_T("����������������!"));
		return;
	}
	//MessageBox(_T("����ǿ���˳�"));
	//return;
	/*********************************************************/

	// ����ת��
	unsigned char src_data[200];
	int len_data = 0;
	CUtils::HexCString2UnsignedCharStar(block_data, src_data, &len_data);
	// д���
	if(write_block(blockNum, sectionNum, m_RatioStatus, chpwd, src_data, len_data) == IFD_OK) {
		canIO = true;
		((CEdit*)GetDlgItem(IDC_EDITIOSTATUS))->SetWindowTextW(_T("д���ɹ�"));
	}
	else {
		canIO = false;
		((CEdit*)GetDlgItem(IDC_EDITIOSTATUS))->SetWindowTextW(_T("д���ʧ��"));
		// ��������ʾʧ��
		CUtils::buzzerFailed();
	}
	
}


void CDebugger::OnBnClickedBtnreadsection() {
	// ����Ƿ�ѡ����Կ����
	if(m_RatioStatus == noType) {
		MessageBox(_T("��ѡ����Կ���ͣ�"));
		return;
	}
	// ��ȡ��Կ
	CString pwd;
	((CEdit*)GetDlgItem(IDC_EDITPWD))->GetWindowTextW(pwd);
	if(pwd.IsEmpty()) {
		MessageBox(_T("��������Կ��"));
		return;
	}
	else if(pwd.GetLength() != 12) {
		MessageBox(_T("��Կ���ȷǷ���"));
		return;
	}
	// ��Կ��������ת��
	//unsigned char* chpwd = (unsigned char*)(LPCTSTR)pwd;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	pwd.MakeUpper();
	CUtils::HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// ��ȡ������
	int sectionNum = ((CComboBox*)GetDlgItem(IDC_COMBOSECTION))->GetCurSel();
	if(sectionNum == CB_ERR) {
		MessageBox(_T("��ѡ��������"));
		return;
	}
	// ��ȡ������
	int blockNum = 0;
	// ��ȡ��
	unsigned char des_data[1024];
	int des_len;
	for(blockNum = 0; blockNum < 4; blockNum++) {
		if(read_block(sectionNum, blockNum, m_RatioStatus, chpwd, des_data, &des_len) == IFD_OK) {
			// ��ʽ��
			CString block_data, temp;
			block_data.Empty();
			for(int i = 0; i < des_len; i++) {
				temp.Format(_T("%02x"), des_data[i]);
				block_data += temp;
			}
			// ���ݿ�����ò�ͬ�Ŀ�
			switch(blockNum) {
				case 0:
					((CEdit*)GetDlgItem(IDC_EDITBL0DATA))->SetWindowTextW(block_data);
					break;
				case 1:
					((CEdit*)GetDlgItem(IDC_EDITBL1DATA))->SetWindowTextW(block_data);
					break;
				case 2:
					((CEdit*)GetDlgItem(IDC_EDITBL2DATA))->SetWindowTextW(block_data);
					break;
				default:
					// Maybe Wrong
					((CEdit*)GetDlgItem(IDC_EDITBL3DATA0))->SetWindowTextW(block_data.Left(12));
					((CEdit*)GetDlgItem(IDC_EDITBL3DATA1))->SetWindowTextW(block_data.Mid(12, 8));
					((CEdit*)GetDlgItem(IDC_EDITBL3DATA2))->SetWindowTextW(block_data.Right(12));
					break;

			}
			// ����״̬�����ɹ�
			canIO = true;
			((CEdit*)GetDlgItem(IDC_EDITIOSTATUS))->SetWindowTextW(_T("��ȡ�����ɹ�"));
		}
		else {
			// �ÿ�ȫ��������״̬����ʧ��
			canIO = false;
			((CEdit*)GetDlgItem(IDC_EDITBL0DATA))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL1DATA))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL2DATA))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL3DATA0))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL3DATA1))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL3DATA2))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITIOSTATUS))->SetWindowTextW(_T("��ȡ�����쳣"));
			// ��������ʾʧ��
			CUtils::buzzerFailed();
		}
	}
	return;
}


HBRUSH CDebugger::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	switch(pWnd->GetDlgCtrlID()) {
		case IDC_EDITSTATUS:
			if(this->isDeviceOpen)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		case IDC_EDITCARDSTATUS:
			if(this->canGetCardInfo)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		case IDC_EDITLEDSTATUS:
			if(this->canSetLED)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		case IDC_EDITIOSTATUS:			if(this->canIO)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		case IDC_EDITVERSIONINFO:
		case IDC_EDITCARDUID:
		case IDC_EDITLEDINFO:
		case IDC_EDITLEDMECNUM:
		case IDC_EDITPWD:
		case IDC_EDITBL0DATA:
		case IDC_EDITBL1DATA:
		case IDC_EDITBL2DATA:
		case IDC_EDITBL3DATA0:
		case IDC_EDITBL3DATA1:
		case IDC_EDITBL3DATA2:
			pDC->SelectObject(&m_font);
			break;
		default:
			pDC->SetTextColor(BLACK);
			break;
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

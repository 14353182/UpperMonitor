// Appdev.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UpperMonitor.h"
#include "Appdev.h"
#include "afxdialogex.h"

#pragma comment(lib, "./libs/ZM124U.lib")
#include "./libs/ZM124U.h"

#define RED RGB(255, 0, 0)
#define BLUE RGB(0, 0, 255)
#define BLACK RGB(0, 0, 0)

// CAppdev �Ի���

IMPLEMENT_DYNAMIC(CAppdev, CDialogEx)

CAppdev::CAppdev(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAppdev::IDD, pParent)
{
	m_font2.CreatePointFont(110, _T("������κ"));
}

CAppdev::~CAppdev()
{
	// �����Զ�������
	DeleteObject(m_font2);
}

void CAppdev::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	GetDlgItem(IDC_STELTPURSE)->SetFont(&m_font2);
	GetDlgItem(IDC_STWEBMASTER)->SetFont(&m_font2);
	GetDlgItem(IDC_STHISTORY)->SetFont(&m_font2);
	// ������ʱ��
	m_ActiveTimer = SetTimer(SCANTIMER_ID, SCANTIMER * 1000, NULL);
}


BEGIN_MESSAGE_MAP(CAppdev, CDialogEx)
	ON_BN_CLICKED(IDC_BTNPURSEINIT, &CAppdev::OnBnClickedBtnpurseinit)
	ON_BN_CLICKED(IDC_BTNCHECKBALANCE, &CAppdev::OnBnClickedBtncheckbalance)
	ON_BN_CLICKED(IDC_BTNRECHARGE, &CAppdev::OnBnClickedBtnrecharge)
	ON_BN_CLICKED(IDC_BTNCOMSURGE2, &CAppdev::OnBnClickedBtncomsurge2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTNSTARTWEB, &CAppdev::OnBnClickedBtnstartweb)
	ON_BN_CLICKED(IDC_BTNRETIMEDEFINIT, &CAppdev::OnBnClickedBtnretimedefinit)
	ON_BN_CLICKED(IDC_BTNEXITWEB, &CAppdev::OnBnClickedBtnexitweb)
	ON_BN_CLICKED(IDC_BTNCHECKRETIME, &CAppdev::OnBnClickedBtncheckretime)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNLOADHIS, &CAppdev::OnBnClickedBtnloadhis)
	ON_BN_CLICKED(IDC_BTNCLEARHIS, &CAppdev::OnBnClickedBtnclearhis)
END_MESSAGE_MAP()

// �Զ��庯��
void CAppdev::CString2CharStar(const CString& s, char* ch, int len) {
	int i;
	for(i = 0; i < len; i++) {
		ch[i] = s[i];
	}
	ch[i] = '\0';
	return;
}

void CAppdev::HexCString2UnsignedCharStar(const CString& hexStr, unsigned char* asc, int* asc_len) {
	*asc_len = 0;
	int len = hexStr.GetLength();

	char temp[200];
	char tmp[3] = { 0 };
	char* Hex;
	unsigned char* p;

	CString2CharStar(hexStr, temp, len);
	Hex = temp;
	p = asc;

	while(*Hex != '\0') {
		tmp[0] = *Hex;
		Hex++;
		tmp[1] = *Hex;
		Hex++;
		tmp[2] = '\0';
		*p = (unsigned char)strtol(tmp, NULL, 16);
		p++;
		(*asc_len)++;
	}
	*p = '\0';
	return;
}

CString CAppdev::GetCardUID() {
	CString uid, temp;
	unsigned char buff[1024];
	int buff_len;

	// �ɹ���ȡ
	if (find_14443(buff, &buff_len) == IFD_OK) {
		uid.Empty();
		for (int i = 0; i < buff_len; i++) {
			// ����õ�UID���ݣ�1 byte��תΪ16����
			temp.Format(_T("%02x"), buff[i]);
			uid += temp;
		}
		return uid;
	}
	else {
		//return NOCARD;
		return TESTCARD;
	}
}

// CAppdev ��Ϣ�������


void CAppdev::OnBnClickedBtnpurseinit() {
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDITELEBALAN))->GetWindowTextW(balance);
	if(balance.IsEmpty()) {
		// �������
		MessageBox(_T("��������ȷ�ĳ�ʼ����"));
		return;
	}
	long account = _ttol(balance);
	if(account < 0) {
		// �������
		MessageBox(_T("��ʼ���������"));
		return;
	}
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// ��Կ����ת��
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// ��ʼ��Ǯ��
	if(write_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("��ʼ��Ǯ���ɹ�"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("��ʼ��Ǯ��ʧ��"));
	}
}


void CAppdev::OnBnClickedBtncheckbalance() {
	CString balance;
	long account = 0;
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// ��Կת��
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// ��ȡǮ��
	if(read_account(sectionNum, blockNum, pswtype, chpwd, &account) == IFD_OK) {
		balance.Format(_T("%d"), account);
		((CEdit*)GetDlgItem(IDC_EDITELEBALAN))->SetWindowTextW(balance);
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("��ѯǮ���ɹ�"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("��ѯǮ��ʧ��"));
	}
}


void CAppdev::OnBnClickedBtnrecharge() {
	CString addAccount;
	((CEdit*)GetDlgItem(IDC_EDITRECHABALAN))->GetWindowTextW(addAccount);
	if(addAccount.IsEmpty()) {
		// ����ֵ����
		MessageBox(_T("��������ȷ�ĳ�ֵ��"));
		return;
	}
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDITELEBALAN))->GetWindowTextW(balance);
	long account = _ttol(addAccount);
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// ��Կת��
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// ��ȡ����
	CString uid = GetCardUID();
	// ��ֵ����
	if(add_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		OnBnClickedBtncheckbalance();
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("��ֵ�ɹ�"));
		fileRecordHelper.SaveRecharges(uid, addAccount, account + _ttol(balance), _T("�ɹ�"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("��ֵʧ��"));
		fileRecordHelper.SaveRecharges(uid, addAccount, _ttol(balance), _T("ʧ��"));
	}
}


void CAppdev::OnBnClickedBtncomsurge2() {
	CString subAccount;
	((CEdit*)GetDlgItem(IDC_EDITCOMSUBALAN2))->GetWindowTextW(subAccount);
	if(subAccount.IsEmpty()) {
		// �������
		MessageBox(_T("��������ȷ�����ѽ�"));
		return;
	}
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDITELEBALAN))->GetWindowTextW(balance);
	long account = _ttol(subAccount);
	if(account > _ttol(balance)) {
		// ������
		MessageBox(_T("���㣡"));
		return;
	}
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// ��Կ����ת��
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// ��ȡ����
	CString uid = GetCardUID();
	// ���Ѻ���
	if(sub_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		OnBnClickedBtncheckbalance();
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("���ѳɹ�"));
		fileRecordHelper.SaveConsumptions(uid, subAccount, _ttol(balance) - account, _T("�ɹ�"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("����ʧ��"));
		fileRecordHelper.SaveConsumptions(uid, subAccount, _ttol(balance), _T("ʧ��"));
	}
}


HBRUSH CAppdev::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	switch(pWnd->GetDlgCtrlID()) {
		case IDC_EDITELESTATUS:
			if(this->canIOPurse)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		default:
			pDC->SetTextColor(BLACK);
			break;
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CAppdev::OnBnClickedBtnstartweb() {
	CString uid = GetCardUID();

	// �ɹ���ȡ
	if (uid != NOCARD) {
		// ���û�����OnTable�У���ʾ��ʼ��
		if (!adoMySQLHelper.MySQL_QueryByUID(uid, ONTABLE)) {
			MessageBox(_T("���ȳ�ʼ����ʱ"));
		}
		// ���û�����OnTable��
		else {
			// ���û�����RemainTimeTable�У�ʲôҲ����
			if (adoMySQLHelper.MySQL_QueryByUID(uid, REMAINTIMETABLE)){
				// ����״̬����ʧ��
				((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("�����ϻ���"));
			}
			// ���û�������RemainTimeTable�У���OnTable�ж�ȡ��д�뵽RemainTimeTable
			else{
				CString cond = _T("UID=\'") + uid + _T("\'");
				OnRecord* pRecord = (OnRecord*)adoMySQLHelper.MySQL_Query(cond, ONTABLE);
				// ���û��Ѿ���ʱ
				if (pRecord->isOvertime) {
					// ����״̬����ʧ��
					((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("�ѳ�ʱ�����ȳ�ֵ"));
					fileRecordHelper.StartNets(uid, pRecord->RemainSeconds, _T("ʧ��"));
				}
				// �û�û�г�ʱ
				else {
					adoMySQLHelper.MySQL_Insert(RemainTime(pRecord->UID, pRecord->RemainSeconds));
					// ����״̬�����ɹ�
					((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("��ʼ�ϻ��ɹ�"));
					fileRecordHelper.StartNets(uid, pRecord->RemainSeconds, _T("�ɹ�"));
				}
				delete(pRecord); // Important!
			}
		}
	}
	else {
		// ����״̬����ʧ��
		((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("��ȡ�����쳣"));
	}
}


void CAppdev::OnBnClickedBtnretimedefinit() {
	CString uid = GetCardUID();

	// �ɹ���ȡ
	if (uid != NOCARD) {
		// ���û�����OnTable�У������¼�¼
		if (!adoMySQLHelper.MySQL_QueryByUID(uid, ONTABLE)) {
			CTime curTime = CTime::GetCurrentTime();
			adoMySQLHelper.MySQL_Insert(OnRecord(uid, DEFAULTREMAINTIME, curTime.Format(TIMEFORMAT)));
			// ����״̬�����ɹ�
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("��ʼ����ʱ�ɹ�"));
		}
		// ���û�����OnTable�У�����OnTable
		else {
			adoMySQLHelper.MySQL_UpdateRemainTime(uid, DEFAULTREMAINTIME, ONTABLE);
			// ����û������ϻ���һ������RemainTimeTable
			if (adoMySQLHelper.MySQL_QueryByUID(uid, REMAINTIMETABLE)) {
				adoMySQLHelper.MySQL_UpdateRemainTime(uid, DEFAULTREMAINTIME, REMAINTIMETABLE);
			}
			// ����״̬�����ɹ�
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("������ʱ�ɹ�"));
		}
	}
	else {
		// ����״̬����ʧ��
		((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("��ȡ�����쳣"));
	}
}


void CAppdev::OnBnClickedBtnexitweb() {
	CString uid = GetCardUID();

	// �ɹ���ȡ
	if (uid != NOCARD) {
		// �û�û�����ϻ�
		if (!adoMySQLHelper.MySQL_QueryByUID(uid, REMAINTIMETABLE)) {
			// ����״̬����ʧ��
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("û�����ϻ����޷��˳�"));
		}
		// �û������ϻ�
		else {
			CString cond = _T("UID=\'") + uid + _T("\'");
			RemainTime* pRemainTime = (RemainTime*)adoMySQLHelper.MySQL_Query(cond, REMAINTIMETABLE);
			adoMySQLHelper.MySQL_Delete(uid, REMAINTIMETABLE); // ɾ��RemainTimeTable��¼���˳��ϻ�
			adoMySQLHelper.MySQL_UpdateRemainTime(uid, pRemainTime->RemainSeconds, ONTABLE); // ����OnTable
			// ����״̬�����ɹ�
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("�ɹ��˳��ϻ�"));
			fileRecordHelper.ExitNets(uid, pRemainTime->RemainSeconds, DEFAULTREMAINTIME, _T("�ɹ�")); // TODO: Fix OverTime
			delete(pRemainTime); // important!
		}
	}
	else {
		// ����״̬����ʧ��
		((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("��ȡ�����쳣"));
	}
}


void CAppdev::OnBnClickedBtncheckretime() {
	CString uid = GetCardUID();

	// �ɹ���ȡ
	if (uid != NOCARD) {
		// �û�û�����ϻ�
		if (!adoMySQLHelper.MySQL_QueryByUID(uid, REMAINTIMETABLE)) {
			// ����״̬����ʧ��
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("û�����ϻ�"));
		}
		// �û������ϻ�
		else {
			CString cond = _T("UID=\'") + uid + _T("\'");
			RemainTime* pRemainTime = (RemainTime*)adoMySQLHelper.MySQL_Query(cond, REMAINTIMETABLE);
			CString remainHours;
			remainHours.Format(_T("%d"), pRemainTime->RemainSeconds / 3600);
			CString remainMinutes;
			remainMinutes.Format(_T("%d"), (pRemainTime->RemainSeconds % 3600) / 60);
			CString remainSeconds;
			remainSeconds.Format(_T("%d"), pRemainTime->RemainSeconds  % 60);
			// ����UI
			((CEdit*)GetDlgItem(IDC_EDITREHOUR))->SetWindowTextW(remainHours);
			((CEdit*)GetDlgItem(IDC_EDITREMINUTE))->SetWindowTextW(remainMinutes);
			((CEdit*)GetDlgItem(IDC_EDITRESECOND))->SetWindowTextW(remainSeconds);
			// ����״̬�����ɹ�
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("��ѯ��ʱ�ɹ�"));
			delete(pRemainTime); // important!
		}
	}
	else {
		// ����״̬����ʧ��
		((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("��ȡ�����쳣"));
	}
}

void CAppdev::OnBnClickedBtnloadhis(){
	((CEdit*)GetDlgItem(IDC_EDITHISTORY))->SetWindowTextW(fileRecordHelper.LoadRecords());
}


void CAppdev::OnBnClickedBtnclearhis(){
	if (fileRecordHelper.EmptyRecords()) {
		((CEdit*)GetDlgItem(IDC_EDITHISTORY))->SetWindowTextW(_T(""));
	}
}

void CAppdev::OnTimer(UINT_PTR nIDEvent){
	// �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent){
		case SCANTIMER_ID:
			adoMySQLHelper.MySQL_ScanOnTable(SCANTIMER);
			break;
		default:
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CAppdev::DestroyWindow(){
	// ���ٶ�ʱ��
	KillTimer(m_ActiveTimer);
	return CDialogEx::DestroyWindow();
}

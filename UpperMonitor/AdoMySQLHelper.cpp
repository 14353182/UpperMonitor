#include "stdafx.h"
#include "AdoMySQLHelper.h"

CAdoMySQLHelper::CAdoMySQLHelper(){
	this->MySQL_Connect();
}

CAdoMySQLHelper::~CAdoMySQLHelper(){
	this->MySQL_Close();
}

BOOL CAdoMySQLHelper::MySQL_Connect(){
	// ��ʼ��OLE/COM�⻷��
	CoInitialize(NULL);

	try{
		// ͨ�����ִ���Connection����
		HRESULT hr = this->m_pConnection.CreateInstance("ADODB.Connection");
		if (FAILED(hr)){
			AfxMessageBox(_T("����_ConnectionPtr����ָ��ʧ��"));
			return false;
		}
		// �������ӳ�ʱʱ��
		this->m_pConnection->ConnectionTimeout = 600;
		// ����ִ�����ʱʱ��
		this->m_pConnection->CommandTimeout = 120;

		// �������ݿ�
		this->m_pConnection->Open("DSN=MySQL5.7;Server=localhost;Database=ZD124UE_DEMO",
			"root",
			"root",
			adModeUnknown);
	}
	catch (_com_error &e){
		// �����Ӵ򿪣���Ҫ���쳣�����йرպ��ͷ�����
		if ((NULL != this->m_pConnection) && this->m_pConnection->State){
			this->m_pConnection->Close();
			this->m_pConnection.Release();
			this->m_pConnection = NULL;
		}
		// ��CView��CDialog��Ҫʹ��ȫ�ֺ���AfxMessageBox
		AfxMessageBox(e.Description());
	}
	return true;
}

void CAdoMySQLHelper::MySQL_Close(){
	if ((NULL != this->m_pConnection) && (this->m_pConnection->State)){
		this->m_pConnection->Close(); // �ر�����
		this->m_pConnection.Release();// �ͷ�����
		this->m_pConnection = NULL;
	}

	// ������COM���ж��COM��
	CoUninitialize();
}

BOOL CAdoMySQLHelper::MySQL_Insert(RemainTime record){
	_CommandPtr m_pCommand;
	try{
		m_pCommand.CreateInstance("ADODB.Command");

		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		// ����Ϊ�޲���
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// �����������Ӹ�ֵ����
		m_pCommand->ActiveConnection = this->m_pConnection;

		// ����ʱ�ַ�����
		CString temp;
		temp.Format(_T("%d"), record.RemainSeconds);
		// SQL���
		m_pCommand->CommandText = "insert into RemainTimeTable values(\'"
			+ (_bstr_t)record.UID + "\',"
			+ (_bstr_t)temp + ")";
		// ִ��SQL���
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}
	catch (_com_error &e){
		// ��Ҫ���쳣�������ͷ��������
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// ��CView��CDialog��Ҫʹ��ȫ�ֺ���AfxMessageBox
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

BOOL CAdoMySQLHelper::MySQL_Insert(OnRecord record){
	_CommandPtr m_pCommand;
	try{
		m_pCommand.CreateInstance("ADODB.Command");

		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		// ����Ϊ�޲���
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// �����������Ӹ�ֵ����
		m_pCommand->ActiveConnection = this->m_pConnection;

		// ����ʱ�ַ�����
		CString temp;
		temp.Format(_T("%d"), record.RemainSeconds);
		// ����ʱ�ַ�����
		CString isOverTime;
		isOverTime = (record.isOvertime) ? "TRUE" : "FALSE";
		// SQL���
		m_pCommand->CommandText = "insert into OnTable values(\'"
			+ (_bstr_t)record.UID + "\',"
			+ (_bstr_t)temp + "\',"
			+ (_bstr_t)record.StartTime + "\',"
			+ (_bstr_t)isOverTime + ")";
		// ִ��SQL���
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}
	catch (_com_error &e){
		// ��Ҫ���쳣�������ͷ��������
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// ��CView��CDialog��Ҫʹ��ȫ�ֺ���AfxMessageBox
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

BOOL CAdoMySQLHelper::MySQL_Delete(CString uid, CString table){
	_CommandPtr m_pCommand;
	try{
		m_pCommand.CreateInstance("ADODB.Command");

		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		// ����Ϊ�޲���
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// �����������Ӹ�ֵ����
		m_pCommand->ActiveConnection = this->m_pConnection;

		// SQL���
		m_pCommand->CommandText = "delete from " + (_bstr_t)table + " where UID=\'" + (_bstr_t)uid + "\'";
		// ִ��SQL���
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}
	catch (_com_error &e){
		// ��Ҫ���쳣�������ͷ��������
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// ��CView��CDialog��Ҫʹ��ȫ�ֺ���AfxMessageBox
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

BOOL CAdoMySQLHelper::MySQL_UpdateRemainTime(CString uid, int updateTime, CString table){
	_CommandPtr m_pCommand;
	try{
		m_pCommand.CreateInstance("ADODB.Command");

		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		// ����Ϊ�޲���
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		// �����������Ӹ�ֵ����
		m_pCommand->ActiveConnection = this->m_pConnection;

		// ������ʱ���ַ�����
		CString str_updateTime;
		str_updateTime.Format(_T("%d"), updateTime);
		// SQL���
		m_pCommand->CommandText = "update " + (_bstr_t)table 
			+ " set RemainTime=" + (_bstr_t)str_updateTime 
			+ " where UID=\'" + (_bstr_t)uid + "\'";
		// ִ��SQL���
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}
	catch (_com_error &e){
		// ��Ҫ���쳣�������ͷ��������
		if ((NULL != m_pCommand) && (m_pCommand->State)){
			m_pCommand.Release();
			m_pCommand = NULL;
		}
		// ��CView��CDialog��Ҫʹ��ȫ�ֺ���AfxMessageBox
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

void* CAdoMySQLHelper::MySQL_Query(CString cond, CString table){
	// �����ݼ�SQL���
	_variant_t sql = "SELECT * FROM " + (_bstr_t)table + " WHERE " + (_bstr_t)cond;
	
	OnRecord* pOnRecord = NULL;
	RemainTime* pRemainTime = NULL;
	try{
		// ����_RecordsetPtr����ָ��
		_RecordsetPtr m_pRecordset;
		HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if (FAILED(hr)){
			AfxMessageBox(_T("����_RecordsetPtr����ָ��ʧ��"));
			return (void*)false;
		}
		//  �����ӣ���ȡ���ݼ�
		m_pRecordset->Open(sql,
			_variant_t((IDispatch*)(this->m_pConnection), true),
			adOpenForwardOnly,
			adLockReadOnly,
			adCmdText);
		// ȷ����Ϊ��
		if (!m_pRecordset->ADOEOF){
			// �ƶ��α굽��ǰ
			m_pRecordset->MoveFirst();
			// ѭ���������ݼ�
			while (!m_pRecordset->ADOEOF){
				if (table == ONTABLE) {
					/********* Get UID ********/
					_variant_t varUID = m_pRecordset->Fields->GetItem(_T("UID"))->GetValue();
					varUID.ChangeType(VT_BSTR);
					CString strUID = varUID.bstrVal;
					/********* Get RemainSeconds ********/
					_variant_t varRemainTime = m_pRecordset->Fields->GetItem(_T("RemainTime"))->GetValue();
					varRemainTime.ChangeType(VT_INT);
					int intRemainTime = varRemainTime.intVal;
					/********** Get StartTime ******************/
					_variant_t varStartTime = m_pRecordset->GetCollect(_T("StartTime"));
					COleDateTime varDateTime = (COleDateTime)varStartTime;
					CString strStartTime = varDateTime.Format(TIMEFORMAT);
					/*********** Get isOverTime ***********/
					_variant_t varIsOverTime = m_pRecordset->Fields->GetItem(_T("isOverTime"))->GetValue();
					varIsOverTime.ChangeType(VT_BOOL);
					bool boolIsOverTime = varIsOverTime.boolVal;
					/************ Generate OnRecord ****************/
					pOnRecord = new OnRecord(strUID, intRemainTime, strStartTime, boolIsOverTime);
				}
				else if(table == REMAINTIMETABLE) {
					/********* Get UID ********/
					_variant_t varUID = m_pRecordset->Fields->GetItem(_T("UID"))->GetValue();
					varUID.ChangeType(VT_BSTR);
					CString strUID = varUID.bstrVal;
					/********* Get RemainSeconds ********/
					_variant_t varRemainTime = m_pRecordset->Fields->GetItem(_T("RemainTime"))->GetValue();
					varRemainTime.ChangeType(VT_INT);
					int intRemainTime = varRemainTime.intVal;
					/************ Generate RemainTime ****************/
					pRemainTime = new RemainTime(strUID, intRemainTime);
				}
				
				break; // Only Return one struct
				// BUG: Let another get more seconds to live
				m_pRecordset->MoveNext();
			}
		}
	}
	catch (_com_error &e){
		AfxMessageBox(e.Description());
	}
	if (table == ONTABLE)
		return (void*)pOnRecord;
	else
		return (void*)pRemainTime;
}

BOOL CAdoMySQLHelper::MySQL_QueryByUID(int uid, CString table){
	// �ַ�����UID
	CString str_uid;
	str_uid.Format(_T("%d"), uid);
	// �����ݼ�SQL���
	_variant_t sql = "SELECT * FROM " + (_bstr_t)table
				   + " WHERE UID=\'" + (_bstr_t)str_uid + "/'";
	bool isExist = false;
	try{
		// ����_RecordsetPtr����ָ��
		_RecordsetPtr m_pRecordset;
		HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if (FAILED(hr)){
			AfxMessageBox(_T("����_RecordsetPtr����ָ��ʧ��"));
			return false;
		}
		//  �����ӣ���ȡ���ݼ�
		m_pRecordset->Open(sql,
						   _variant_t((IDispatch*)(this->m_pConnection), true),
						   adOpenForwardOnly,
						   adLockReadOnly,
						   adCmdText);
		// ȷ����Ϊ��
		if (!m_pRecordset->ADOEOF){
			// ����UID����������Ϊ�ռ�Ϊ����
			isExist = true;
		}
	}
	catch (_com_error &e){
		AfxMessageBox(e.Description());
	}
	return isExist;
}

void CAdoMySQLHelper::MySQL_ScanOnTable(int timer) {

}

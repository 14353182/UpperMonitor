#include "stdafx.h"
#include "RecordHelper.h"


CRecordHelper::CRecordHelper(CString _mSaveFile){
	// ���ļ�·��
	this->mSaveFile = _mSaveFile;
}


CRecordHelper::~CRecordHelper(){

}

void CRecordHelper::SaveRecharges(CString uid, CString accounts, long remainings, CString result){
	// ���ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// ��ȡ��ǰʱ��
	CTime curTime = CTime::GetCurrentTime();
	curTime.Format(TIMEFORMAT);
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\nʱ�䣺%s\n�����%s\n���ݣ��û���ֵ\n��%s\n��%d\n"),
		uid, curTime, result, accounts, remainings);
	// ָ���ļ���ͷ��д��
	mFile.SeekToBegin();
	mFile.Write(contents, contents.GetLength());
	// �ر��ļ�
	mFile.Close();
}

void CRecordHelper::SaveConsumptions(CString uid, CString accounts, long remainings, CString result){
	// ���ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// ��ȡ��ǰʱ��
	CTime curTime = CTime::GetCurrentTime();
	curTime.Format(TIMEFORMAT);
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\nʱ�䣺%s\n�����%s\n���ݣ��û�����\n��%s\n��%d\n"),
		uid, curTime, result, accounts, remainings);
	// ָ���ļ���ͷ��д��
	mFile.SeekToBegin();
	mFile.Write(contents, contents.GetLength());
	// �ر��ļ�
	mFile.Close();
}

void CRecordHelper::StartNets(CString uid, int remainTime, CString result){
	// ���ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// ��ȡ��ǰʱ��
	CTime curTime = CTime::GetCurrentTime();
	curTime.Format(TIMEFORMAT);
	// ��ʽ����ʱ
	CString sRemainTime;
	sRemainTime.Format(_T("%dʱ%d��%d��"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\nʱ�䣺%s\n�����%s\n���ݣ��û��ϻ�\n��ʱ��%s\n"), 
						uid, curTime, result, sRemainTime);
	// ָ���ļ���ͷ��д��
	mFile.SeekToBegin();
	mFile.Write(contents, contents.GetLength());
	// �ر��ļ�
	mFile.Close();
}

void CRecordHelper::ExitNets(CString uid, int remainTime, int overSeconds, CString result){
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// ��ȡ��ǰʱ��
	CTime curTime = CTime::GetCurrentTime();
	curTime.Format(TIMEFORMAT);
	// ��ʽ���ϻ�ʱ��
	CString sOverTime;
	sOverTime.Format(_T("%dʱ%d��%d��"), overSeconds / 3600, (overSeconds % 3600) / 60, overSeconds % 60);
	// ��ʽ����ʱ
	CString sRemainTime;
	sRemainTime.Format(_T("%dʱ%d��%d��"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\nʱ�䣺%s\n�����%s\n���ݣ��û��˳�\n�ϻ�ʱ��:%s\n��ʱ��%s\n"),
		uid, curTime, result, sOverTime, sRemainTime);
	// ָ���ļ���ͷ��д��
	mFile.SeekToBegin();
	mFile.Write(contents, contents.GetLength());
	// �ر��ļ�
	mFile.Close();
}

CString CRecordHelper::LoadRecords(){
	// ���ļ�
	CStdioFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead);
	// ָ��ͷ��ѭ������
	mFile.SeekToBegin();
	CString contents, line;
	contents.Empty();
	while (mFile.ReadString(line)) {
		contents += (line + _T("\r\n"));
	}
	// �ر��ļ������ؽ��
	mFile.Close();
	return contents;
}

BOOL CRecordHelper::EmptyRecords(){
	// ����ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeReadWrite);
	//mFile.Write("\xff\xfe", 2); Unicode
	mFile.Close();
	return true;
}
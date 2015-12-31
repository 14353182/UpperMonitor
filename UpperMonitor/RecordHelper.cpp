#include "stdafx.h"
#include "RecordHelper.h"


// �Զ����������
int Char2Unicode(char *pchIn, CString *pstrOut){
	int nLen;
	WCHAR *ptch;
	if (pchIn == NULL) {
		return 0;
	}
	nLen = MultiByteToWideChar(CP_ACP, 0, pchIn, -1, NULL, 0);
	ptch = new WCHAR[nLen];
	MultiByteToWideChar(CP_ACP, 0, pchIn, -1, ptch, nLen);
	pstrOut->Format(_T("%s"), ptch);
	delete[] ptch;
	return nLen;
}

int Unicode2Char(CString &strIn, char *pchOut, int nCharLen){
	if (pchOut == NULL){
		return 0;
	}
	int nLen;
	// Problem: Compile Problem
	/*nLen = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)strIn.GetBuffer(BUFFER_SIZE_KILO), -1, NULL, 0, NULL, NULL);
	nLen = min(nLen, nCharLen);
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)strIn.GetBuffer(BUFFER_SIZE_KILO), -1, pchOut, nLen, NULL, NULL);
	if (nLen < nCharLen){
		pchOut[nLen] = 0;
	}*/
	return nLen;
}

BOOL FileUnicodeEncode(CFile &mFile) {
	// �ú���ֻ����mFile.Close()ǰ����
	WORD unicode = 0xFEFF;
	mFile.SeekToBegin();
	mFile.Write(&unicode, 2); // Unicode
	return true;
}

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
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û���ֵ\r\n��%s\r\n��%d\r\n\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, accounts, remainings);
	// ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
	// �ر��ļ�
	mFile.Close();
}

void CRecordHelper::SaveConsumptions(CString uid, CString accounts, long remainings, CString result){
	// ���ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// ��ȡ��ǰʱ��
	CTime curTime = CTime::GetCurrentTime();
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û�����\r\n��%s\r\n��%d\r\n\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, accounts, remainings);
	// ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
	// �ر��ļ�
	mFile.Close();
}

void CRecordHelper::StartNets(CString uid, int remainTime, CString result){
	// ���ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// ��ȡ��ǰʱ��
	CTime curTime = CTime::GetCurrentTime();
	// ��ʽ����ʱ
	CString sRemainTime;
	sRemainTime.Format(_T("%dʱ%d��%d��"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û��ϻ�\r\n��ʱ��%s\r\n\r\n"), 
					uid, curTime.Format(TIMEFORMAT), result, sRemainTime);
	// ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
	// �ر��ļ�
	mFile.Close();
}

void CRecordHelper::ExitNets(CString uid, int remainTime, int overSeconds, CString result){
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// ��ȡ��ǰʱ��
	CTime curTime = CTime::GetCurrentTime();
	// ��ʽ���ϻ�ʱ��
	CString sOverTime;
	sOverTime.Format(_T("%dʱ%d��%d��"), overSeconds / 3600, (overSeconds % 3600) / 60, overSeconds % 60);
	// ��ʽ����ʱ
	CString sRemainTime;
	sRemainTime.Format(_T("%dʱ%d��%d��"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û��˳�\r\n�ϻ�ʱ��:%s\r\n��ʱ��%s\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, sOverTime, sRemainTime);
	// ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
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
		line += "\r\n";
		char *buffer = new char[line.GetLength()];
		for (int i = 0; i < line.GetLength(); i++) {
			buffer[i] = line.GetAt(i);
		}
		line.Empty();
		Char2Unicode(buffer, &line);
		delete[] buffer;
		contents += line;
	}
	// �ر��ļ������ؽ��
	mFile.Close();
	return contents;
}

BOOL CRecordHelper::EmptyRecords(){
	// ����ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeReadWrite);
	FileUnicodeEncode(mFile);
	mFile.Close();
	return true;
}
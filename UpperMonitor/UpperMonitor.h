
// UpperMonitor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������



// CUpperMonitorApp: 
// �йش����ʵ�֣������ UpperMonitor.cpp
//

class CUpperMonitorApp : public CWinApp
{
public:
	CUpperMonitorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

};

extern CUpperMonitorApp theApp;
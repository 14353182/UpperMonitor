#include "stdafx.h"
#include "Utils.h"

#pragma comment(lib, "./libs/ZM124U.lib")
#include "./libs/ZM124U.h"

CUtils::CUtils(){}


CUtils::~CUtils(){}

BOOL CUtils::buzzerFailed() {
	// ���÷������ķ�����ʽ
	if (buzzer_setting('1000', '200', 10, 0) == IFD_OK) {
		return true;
	}
	else {
		return false;
	}
}
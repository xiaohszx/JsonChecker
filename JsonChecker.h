
// JsonChecker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CJsonCheckerApp: 
// �йش����ʵ�֣������ JsonChecker.cpp
//

class CJsonCheckerApp : public CWinApp
{
public:
	CJsonCheckerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CJsonCheckerApp theApp;
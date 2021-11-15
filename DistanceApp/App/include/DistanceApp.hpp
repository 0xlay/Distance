
// DistanceApp.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.hpp' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "DefaultTcpServer.hpp"
#include "Json.hpp"
#include "Logger.hpp"
#include "meminfo.hpp"

#define FAILED_CONNECTION_TO_CLIENT -1

// DistanceApp:
// See DistanceApp.cpp for the implementation of this class
//

class DistanceApp : public CWinApp
{
public:
	DistanceApp();

// Overrides
public:
	BOOL InitInstance() override;

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern DistanceApp theApp;

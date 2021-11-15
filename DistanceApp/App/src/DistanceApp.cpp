
// DistanceApp.cpp : Defines the class behaviors for the application.
//
#include "pch.hpp"

#include "framework.hpp"

#include <thread>
#include <chrono>

#include "DistanceApp.hpp"
#include "MainDlg.hpp"

#include "ServerHandlers.hpp"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// DistanceApp

BEGIN_MESSAGE_MAP(DistanceApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// DistanceApp construction

DistanceApp::DistanceApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only DistanceApp object

DistanceApp theApp;


// DistanceApp initialization

BOOL DistanceApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey($T"0xlay.Lab");

	using namespace Distance;
	Utils::Logger::invoke().configure(std::make_unique<Utils::LoggerFileStream>($T"logs/"));
	Network::Configurator::invoke().open($T"server_settings.json");

	auto settings = std::make_unique<Network::ServerSettings>();
	Network::Configurator::invoke().loadSettings(settings.get());

	auto server = std::make_shared<Network::DefaultTcpServer>(std::move(settings));
	Network::Message msg;

	server->setHandler(Network::Handler::AcceptHandler, msg);
	std::thread serverThread(
		[](std::shared_ptr<Network::DefaultTcpServer> server_) { server_->run(); }, server
	);

	MainDlg dlg(server, msg);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	server->stop();
	serverThread.join();

	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

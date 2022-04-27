//------------------------------------------------------------------------------
// ╔═══╗╔══╗╔═══╗╔════╗╔═══╗╔═╗─╔╗╔═══╗╔═══╗
// ╚╗╔╗║╚╣─╝║╔═╗║║╔╗╔╗║║╔═╗║║║╚╗║║║╔═╗║║╔══╝
// ─║║║║─║║─║╚══╗╚╝║║╚╝║║─║║║╔╗╚╝║║║─╚╝║╚══╗
// ─║║║║─║║─╚══╗║──║║──║╚═╝║║║╚╗║║║║─╔╗║╔══╝
// ╔╝╚╝║╔╣─╗║╚═╝║──║║──║╔═╗║║║─║║║║╚═╝║║╚══╗
// ╚═══╝╚══╝╚═══╝──╚╝──╚╝─╚╝╚╝─╚═╝╚═══╝╚═══╝
// 
// Copyright Ⓒ 0xlay. All rights reserved.
// 
// This source file is licensed under the terms of MIT license.
// For details, please read the LICENSE file.
// 
// File: DistanceApp.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#include "pch.hpp"
#include "Framework.hpp"
#include "App.hpp"
#include "ServerHandlers.hpp"
#include "DistanceApp.hpp"
#include "MainDlg.hpp"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace Distance
{

    BEGIN_MESSAGE_MAP(DistanceApp, CWinApp)
        ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
    END_MESSAGE_MAP()


    DistanceApp::DistanceApp()
    {
        m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
    }

    DistanceApp::~DistanceApp()
    {
        server_->stop();
        serverThread_->join();
    }

    DistanceApp theApp;

    BOOL DistanceApp::InitInstance()
    {
        INITCOMMONCONTROLSEX InitCtrls;
        InitCtrls.dwSize = sizeof(InitCtrls);
        InitCtrls.dwICC = ICC_WIN95_CLASSES;
        InitCommonControlsEx(&InitCtrls);

        CWinApp::InitInstance();

        AfxEnableControlContainer();

        // Activate "Windows Native" visual manager for enabling themes in MFC controls
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

        SetRegistryKey(L"0xlay.Lab");

        using namespace Distance::Utils;

        Logger::invoke().configure(std::make_unique<LoggerFileStream>(L"logs/"));

        SettingsController::invoke().openStorage(StorageType::JsonFile, L"server_settings.json");

        settings_ = std::make_shared<Network::ServerSettings>();
        SettingsController::invoke().load(
            std::reinterpret_pointer_cast<Settings>(settings_)
        );

        server_ = std::make_shared<Network::DefaultTcpServer>(std::move(settings_));
        server_->setHandler(Network::Handler::AcceptHandler, msg_);
        serverThread_ = std::make_unique<std::thread>(
            [](std::shared_ptr<Network::DefaultTcpServer> server_) { server_->run(); }, server_
        );

        MainDlg dlg(server_, msg_);
        m_pMainWnd = &dlg;
        INT_PTR nResponse = dlg.DoModal();
        if (nResponse == -1)
        {
            TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
            TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
        }

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
        ControlBarCleanUp();
#endif

        // Since the dialog has been closed, return FALSE so that we exit the
        // application, rather than start the application's message pump.
        return FALSE;
    }

} // Distance

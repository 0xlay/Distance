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
// File: PowerManagerDlg.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#include "pch.hpp"
#include "App.hpp"
#include "PowerManagerDlg.hpp"
#include "afxdialogex.h"


namespace Distance
{

    IMPLEMENT_DYNAMIC(PowerManagerDlg, CDialogEx)

    PowerManagerDlg::PowerManagerDlg(
        std::shared_ptr<Network::DefaultTcpServer> server,
        size_t clientID,
        CWnd* pParent /*nullptr*/
    )
        : server_(std::move(server)),
          clientID_(clientID),
          CDialogEx(IDD_POWER_MANAGER_DIALOG, pParent)
    { }

    PowerManagerDlg::~PowerManagerDlg()
    { }

    void PowerManagerDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialogEx::DoDataExchange(pDX);
    }

    BEGIN_MESSAGE_MAP(PowerManagerDlg, CDialogEx)
        ON_BN_CLICKED(ID_POWER_MANAGER_SHUT_DOWN, 
                      &PowerManagerDlg::OnBnClickedShutDown)
        ON_BN_CLICKED(ID_POWER_MANAGER_RESTART,
                      &PowerManagerDlg::OnBnClickedRestart)
    END_MESSAGE_MAP()
    
    void PowerManagerDlg::OnBnClickedShutDown()
    {
        using namespace Distance::Network;
        message_.clear();
        message_.setMsgType(MsgType::ShutdownPC);

        if (!server_->send(message_, clientID_))
        {
            MessageBoxW(
                L"Send message to the client was failed! "
                L"The client may have been disconnected from the server.",
                L"Distance", MB_ICONERROR
            );
            EndDialog(FAILED_CONNECTION_TO_CLIENT);
            return;
        }

        if (!server_->receive(message_, clientID_))
        {
            MessageBoxW(L"Receive message from the client was failed! "
                        L"The client may have been disconnected from the server.",
                        L"Distance",
                        MB_ICONERROR);
            EndDialog(FAILED_CONNECTION_TO_CLIENT);
            return;
        }
        if (message_.isErrorMsg())
        {
            std::string errorMsg;
            message_ >> errorMsg;
            MessageBoxError("Distance", errorMsg);
            return;
        }
    }

    void PowerManagerDlg::OnBnClickedRestart()
    {
        using namespace Distance::Network;
        message_.clear();
        message_.setMsgType(MsgType::RebootPC);

        if (!server_->send(message_, clientID_))
        {
            MessageBoxW(L"Send message to the client was failed! "
                        L"The client may have been disconnected from the server.",
                        L"Distance",
                        MB_ICONERROR);
            EndDialog(FAILED_CONNECTION_TO_CLIENT);
            return;
        }

        if (!server_->receive(message_, clientID_))
        {
            MessageBoxW(L"Receive message from the client was failed! "
                        L"The client may have been disconnected from the server.",
                        L"Distance",
                        MB_ICONERROR);
            EndDialog(FAILED_CONNECTION_TO_CLIENT);
            return;
        }
        if (message_.isErrorMsg())
        {
            std::string errorMsg;
            message_ >> errorMsg;
            MessageBoxError("Distance", errorMsg);
            return;
        }
    }

} // Distance
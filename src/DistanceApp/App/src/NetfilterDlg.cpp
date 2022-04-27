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
// File: NetfilterDlg.cpp
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
#include "NetfilterDlg.hpp"
#include "afxdialogex.h"


namespace Distance
{

    IMPLEMENT_DYNAMIC(NetfilterDlg, CDialogEx)

    NetfilterDlg::NetfilterDlg(std::shared_ptr<Network::DefaultTcpServer> server,
                               size_t clientID,
                               CWnd* pParent /*=nullptr*/)
        : CDialogEx(IDD_NETFILTER_DIALOG, pParent),
          server_(std::move(server)),
          clientID_(clientID)
    { }

    NetfilterDlg::~NetfilterDlg()
    { }

    void NetfilterDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialogEx::DoDataExchange(pDX);
    }

    BEGIN_MESSAGE_MAP(NetfilterDlg, CDialogEx)
        ON_BN_CLICKED(ID_NETFILTER_BLOCK_HTTP, &NetfilterDlg::OnBnClickedBlockHttp)
        ON_BN_CLICKED(ID_NETFILTER_UNBLOCK_HTTP, &NetfilterDlg::OnBnClickedUnblockHttp)
    END_MESSAGE_MAP()
    
    void NetfilterDlg::OnBnClickedBlockHttp()
    {
        using namespace Distance::Network;

        message_.clear();
        message_.setMsgType(MsgType::BlockTcpPorts);

        if (!server_->send(message_, clientID_))
        {
            MessageBoxW(L"Send message to the client was failed! "
                        L"The client may have been disconnected from the server.",
                        L"Distance",
                        MB_ICONERROR);

            EndDialog(FAILED_CONNECTION_TO_CLIENT);
            return;
        }

        message_.clear();

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

        std::string infoMsg;
        message_ >> infoMsg;
        MessageBoxInfo("Distance", infoMsg);
    }

    void NetfilterDlg::OnBnClickedUnblockHttp()
    {
        using namespace Distance::Network;

        message_.clear();
        message_.setMsgType(MsgType::UnblockTcpPorts);

        if (!server_->send(message_, clientID_))
        {
            MessageBoxW(L"Send message to the client was failed! "
                        L"The client may have been disconnected from the server.",
                        L"Distance",
                        MB_ICONERROR);

            EndDialog(FAILED_CONNECTION_TO_CLIENT);
            return;
        }

        message_.clear();

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

        std::string infoMsg;
        message_ >> infoMsg;
        MessageBoxInfo("Distance", infoMsg);
    }

} // Distance

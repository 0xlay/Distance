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
// File: SystemInfoDlg.cpp
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
#include "SystemInfoDlg.hpp"
#include "afxdialogex.h"


namespace Distance
{

    IMPLEMENT_DYNAMIC(SystemInfoDlg, CDialogEx)

    SystemInfoDlg::SystemInfoDlg(
        std::shared_ptr<Distance::Network::DefaultTcpServer> server,
        size_t clientID,
        CWnd* pParent /*=nullptr*/
    )
        : server_(std::move(server)), clientID_(clientID),
          CDialogEx(IDD_SYSINFO_DIALOG, pParent)
    { }

    SystemInfoDlg::~SystemInfoDlg()
    { }

    void SystemInfoDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialogEx::DoDataExchange(pDX);
        DDX_Text(pDX, IDC_EDIT_LOCALNAME, localName_);
        DDX_Text(pDX, IDC_EDIT_IP, ip_);
        DDX_Text(pDX, IDC_EDIT_RAM, ram_);
        DDX_Text(pDX, IDC_EDIT_CPU, cpu_);
    }

    BEGIN_MESSAGE_MAP(SystemInfoDlg, CDialogEx)
    END_MESSAGE_MAP()

    BOOL SystemInfoDlg::OnInitDialog()
    {
        CDialogEx::OnInitDialog();

        message_.setMsgType(Distance::Network::MsgType::GetSysInfo);
        if (!server_->send(message_, clientID_))
        {
            MessageBoxW(L"Send message to the client was failed! "
                        L"The client may have been disconnected from the server.",
                        L"Distance", MB_ICONERROR);

            EndDialog(FAILED_CONNECTION_TO_CLIENT);
            return FALSE;
        }

        if (!server_->receive(message_, clientID_))
        {
            MessageBoxW(L"Receive message from the client was failed! "
                        L"The client may have been disconnected from the server.",
                        L"Distance", MB_ICONERROR);

            EndDialog(FAILED_CONNECTION_TO_CLIENT);
            return FALSE;
        }
        if (message_.isErrorMsg())
        {
            std::string errorMsg;
            message_ >> errorMsg;
            MessageBoxError("Distance", errorMsg);
            return FALSE;
        }

        using namespace Distance::Network;
        Json::PcInfo info;
        std::string result;

        message_ >> result;
        Json::unpack(result, info);

        localName_ = info.localName.c_str();
        ip_ = info.ip.c_str();
        ram_.Format(L"Total: %llu MB, Available: %llu MB, Loaded: %d%%",
                    (info.ram.TotalMemory / 1024 / 1024), 
                    (info.ram.AvailMemory / 1024 / 1024), 
                    info.ram.MemoryLoad);

        std::ostringstream oss;
        oss << info.cpu.architecture << " " << info.cpu.name;
        cpu_ = oss.str().c_str();

        UpdateData(FALSE);
        return TRUE;
    }

} // Distance
